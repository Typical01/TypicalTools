// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingWidget.h"

#include "Engine/Engine.h"

#include "EntryWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"

#include "TypicalTool/Public/Tools.h"
#include "SettingItem.h"




USettingWidget::USettingWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void USettingWidget::Destruct()
{
    if (!IsValid(PlayerControllerTools)) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::Destruct: PlayerControllerTools 无效!")), FColor::Red);
    }
    else {
        PlayerControllerTools->World->GetTimerManager().ClearTimer(AutoPathManageTimerHandle);
    }

    Super::Destruct();
}

void USettingWidget::InitializeListView()
{
    ListViewShellConfigArray = NewObject<UListViewDataManage>();
    ListViewTipsMessageArray = NewObject<UListViewDataManage>();
    ListViewTaskProgressArray = NewObject<UListViewDataManage>();

    if (!IsValid(ListViewShellConfig)) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::InitializeListView: ListViewShellConfig 无效!")), FColor::Red);
        return;
    }
    ListViewShellConfigArray->SetView(ListViewShellConfig); //绑定 到ListView

    if (!IsValid(ListViewErrorPathTips)) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::InitializeListView: ListViewErrorPathTips 无效!")), FColor::Red);
        return;
    }
    ListViewTipsMessageArray->SetView(ListViewErrorPathTips); //绑定 到ListView

    if (!IsValid(ListViewTaskQueue)) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::InitializeListView: ListViewTaskQueue 无效!")), FColor::Red);
        return;
    }
    ListViewTaskProgressArray->SetView(ListViewTaskQueue); //绑定 到ListView

    LoadSettingConfigFile();
    bAutoStartingToQuitGame = UEtytl::GetCommandLineArgument(TEXT("-AutoQuit"), nullptr); //自动退出

    OnPathManage();
    if (!IsValid(PlayerControllerTools)) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::InitializeListView: PlayerControllerTools 无效!")), FColor::Red);
    }
    else {
        PlayerControllerTools->World->GetTimerManager().SetTimer(
            AutoPathManageTimerHandle,                // 定时器句柄
            this,                         // 调用函数的对象
            &USettingWidget::OnPathManage,        // 定时触发的函数
            600.0f,                         // 初始延迟时间（秒）
            true,                        // 是否循环调用
            -1.0f                         // 循环调用的间隔时间（秒），-1.0f 表示使用初始延迟时间
        );
    }

    int32 Index = 1;
    for (auto Item : ListViewShellConfigArray->GetList()) {
        if (!IsValid(Item)) {
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::InitializeListView: ListViewShellConfigArray 无效! 索引[%d]"), Index), FColor::Red);
            continue;
        }

        UShellConfigItem* tempShellConfigItem = Cast<UShellConfigItem>(Item);
        if (tempShellConfigItem->bStartBackup) {
            StartBackup(tempShellConfigItem);
        }
        Index++;
    }

    OnAutoQuit();
}

void USettingWidget::OnApplyButton()
{
    OnApplyButtonIsEnabled(false);
    OnPathManage();

    OutputLog();
    SaveSettingConfigFile();

    if (!IsValid(PlayerControllerTools)) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnApplyButton: PlayerControllerTools 无效!")), FColor::Red);
        return;
    }
    PlayerControllerTools->SaveConfigFile();
    OnSelfStarting();

    OnApplyButtonIsEnabled(true);
}

void USettingWidget::OnQuitButton(bool bIsButton)
{
    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnQuitButton: bIsButton [%s]!"), *LexToString(bIsButton)), FColor::Red);

    FString tempStr;

    // 任务队列未完成 | 未处理错误: 弹框提示, 并提供强制退出
    if (!ListViewTaskProgressArray->IsEmpty()) {
        tempStr += TEXT("任务进行中!\n");
    }

    if (bIsButton) {
        bQuitGame.store(true);
    }

    if (!ListViewTipsMessageArray->IsEmpty()) {
        tempStr += TEXT("有错误未处理!\n");
    }

    if (!tempStr.IsEmpty()) {
        if (!IsValid(PlayerControllerTools)) {
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnQuitButton: PlayerControllerTools 无效!")), FColor::Red);
            return;
        }
        if (!PlayerControllerTools->DialogMainWindow.IsValid()) {
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnQuitButton: PlayerControllerTools->DialogMainWindow 无效!")), FColor::Red);
            return;
        }

        // 创建自定义对话框窗口
        DialogWindow = SNew(SWindow)
            .Title(FText::FromString(TEXT("提示")))
            .ClientSize(FVector2D(600, 200))
            .SupportsMinimize(false)
            .SupportsMaximize(false);

        FSlateFontInfo FontInfo = FCoreStyle::Get().GetFontStyle("Regular");
        FontInfo.Size = 24;

        FText temp = FText::FromString(tempStr);

        // 创建按钮样式
        /*FButtonStyle RedButtonStyle;
        RedButtonStyle.Normal.TintColor = FSlateColor(FLinearColor::Red);
        RedButtonStyle.Hovered.TintColor = FSlateColor(FLinearColor(0.8f, 0.0f, 0.0f));
        RedButtonStyle.Pressed.TintColor = FSlateColor(FLinearColor(0.6f, 0.0f, 0.0f));*/

        // 创建按钮
        TSharedRef<SButton> ForceExitButton = SNew(SButton)
            //.ButtonStyle(&RedButtonStyle)
            .Text(FText::FromString(TEXT("强制退出")))
            .OnClicked(FOnClicked::CreateLambda([this]() {
                this->OnQuitGame();
                return FReply::Handled();
            }))
            .HAlign(HAlign_Center)
            .VAlign(VAlign_Center)
            .Content()
            [
                SNew(STextBlock)
                    .Text(FText::FromString(TEXT("强制退出")))
                    .Font(FontInfo)
            ];

        // 创建对话框内容
        DialogWindow->SetContent(
            SNew(SBorder)
            .Padding(10)
            .HAlign(HAlign_Center)
            .VAlign(VAlign_Center)
            [
                SNew(SVerticalBox)
                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(5)
                [
                    SNew(STextBlock)
                        .Text(temp)
                        .Font(FontInfo)
                ]
                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(5)
                [
                    ForceExitButton
                ]
            ]
        );

        // 将对话框添加到自定义窗口中
        FSlateApplication::Get().AddWindowAsNativeChild(DialogWindow.ToSharedRef(), PlayerControllerTools->DialogMainWindow.ToSharedRef());
    }
    else {
        OnQuitGame();
    }
}

void USettingWidget::OnQuitGame()
{
    if (!IsValid(PlayerControllerTools)) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnQuitGame: PlayerControllerTools 无效!")), FColor::Red);
        return;
    }
    PlayerControllerTools->QuitGame();
}

void USettingWidget::OnApplyButtonIsEnabled(bool _bIsEnabled)
{
    if (!IsValid(ButtonApply)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UEntryWidget::OnApplyButtonIsEnabled: ButtonApply 无效!")), FColor::Red);
        return;
    }
    AsyncTask(ENamedThreads::GameThread, [this, _bIsEnabled]()
        {
            ButtonApply->SetIsEnabled(_bIsEnabled);
        });
}

void USettingWidget::OnAddItem()
{
    UShellConfigItem* Item = NewObject<UShellConfigItem>();
    ListViewShellConfigArray->Add(Item);
    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnDeleteItem: OnAddItem [%d]!"), ListViewShellConfigArray->Num()));
}

void USettingWidget::OnDeleteItem(UShellConfigItem* Item)
{
    int32 RemoveCount = ListViewShellConfigArray->Remove(Item);
    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnDeleteItem: RemoveCount [%d]!"), RemoveCount));
}

void USettingWidget::OnAutoQuit()
{
    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnAutoQuit: bAutoStartingToQuitGame [%s]!"), *LexToString(bAutoStartingToQuitGame)), FColor::Red);
    if (bAutoStartingToQuitGame) {
        OnQuitButton(false);
    }
}

void USettingWidget::OnSelfStarting()
{
    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnSelfStarting: 注册开机自启动[%s]!"), *LexToString(bAutoStarting)));
    // 获取程序全路径
    FString ExecutablePath = FPlatformProcess::ExecutablePath();
    tytl::Win::SetSelfStarting(TEXT("TypicalTool_BackupFile"), ExecutablePath.GetCharArray().GetData(), TEXT("-AutoQuit"), bAutoStarting);
}

void USettingWidget::OnErrorMessageTips()
{
    //有错误路径才显示: 错误路径控件
    if (!IsValid(VerticalBoxErrorPath)) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnErrorMessageTips: VerticalBoxErrorPath 无效!")), FColor::Red);
    }
    else {
        if (ListViewTipsMessageArray->Num() > 0) {
            AsyncTask(ENamedThreads::GameThread, [this]()
                {
                    VerticalBoxErrorPath->SetVisibility(ESlateVisibility::Visible);
                });
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnErrorMessageTips: 显示 错误路径控件!")));
        }
        else {
            AsyncTask(ENamedThreads::GameThread, [this]()
                {
                    VerticalBoxErrorPath->SetVisibility(ESlateVisibility::Collapsed);
                });
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnErrorMessageTips: 隐藏 错误路径控件!")));
        }

        if (!IsValid(ListViewErrorPathTips)) {
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnErrorMessageTips: ListViewErrorPathTips 无效!")), FColor::Red);
        }
        else {
            ListViewTipsMessageArray->SetList(); //刷新
        }
    }
}

void USettingWidget::OutputLog()
{
    int32 tempCount = 0;

    UEtytl::DebugLog(FString::Printf(TEXT(" ")), FColor::Green);
    UEtytl::DebugLog(FString::Printf(TEXT(" ")));
    UEtytl::DebugLog(FString::Printf(TEXT(" ")));
    UEtytl::DebugLog(FString::Printf(TEXT(" ")));
    UEtytl::DebugLog(FString::Printf(TEXT(" ")));
    UEtytl::DebugLog(FString::Printf(TEXT(" ")));
    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OutputLog: bAutoStarting[%s]"), *LexToString(bAutoStarting)));
    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OutputLog: bAutoStartingToQuitGame[%s]"), *LexToString(bAutoStartingToQuitGame)));

    if (!IsValid(ListViewShellConfig)) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OutputLog: ListViewShellConfig 无效!")), FColor::Red);
    }

    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OutputLog: 列表视图ShellConfigItems")));

    for (auto Item : ListViewShellConfigArray->GetList()) {
        tempCount++;
        if (!IsValid(Item)) {
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OutputLog: ListViewShellConfigArray [%d]项无效!"), tempCount), FColor::Red);
            continue;
        }
        UShellConfigItem* tempShellConfigItem = Cast<UShellConfigItem>(Item);

        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OutputLog: ListItem[%d]"), tempCount));
        tempShellConfigItem->OutputLog();
    }
    for (auto Item : ListViewTipsMessageArray->GetList()) {
        tempCount++;
        if (!IsValid(Item)) {
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OutputLog: ListViewTipsMessageArray [%d]项无效!"), tempCount), FColor::Red);
            continue;
        }
        UTextTips* tempErrorTips = Cast<UTextTips>(Item);

        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OutputLog: ListViewTipsMessageArray[%d]"), tempCount));
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OutputLog: ListViewTipsMessageArray[%s]"), *tempErrorTips->Text));
    }
    tempCount = 0;
}

void USettingWidget::SaveSettingConfigFile()
{
    if (!IsValid(PlayerControllerTools)) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::SaveSettingConfigFile: PlayerControllerTools 无效!")), FColor::Red);
        return;
    }
    if (!PlayerControllerTools->ToolsConfig.IsValid()) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::SaveSettingConfigFile: PlayerControllerTools->ToolsConfig 无效!")), FColor::Red);
        return;
    }

    TSharedPtr<FJsonObject> NewToolsConfig = MakeShareable(new FJsonObject());

    if (!IsValid(ListViewShellConfig)) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::SaveSettingConfigFile: ListViewShellConfig 无效!")), FColor::Red);
    }
    if (ListViewShellConfigArray->GetList().IsEmpty()) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::SaveSettingConfigFile: ListViewShellConfigArray 没有项!")));
    }

    //保存到配置文件: ShellConfigItem
    for (auto Item : ListViewShellConfigArray->GetList()) {
        UShellConfigItem* ShellConfigItem = Cast<UShellConfigItem>(Item);

        TSharedPtr<FJsonObject> ShellConfigItemObject = MakeShareable(new FJsonObject());
        ShellConfigItemObject->SetStringField(TEXT("源文件"), ShellConfigItem->SourceFile);
        ShellConfigItemObject->SetStringField(TEXT("目的地路径"), ShellConfigItem->DestinationPath);
        ShellConfigItemObject->SetBoolField(TEXT("启动时备份"), ShellConfigItem->bStartBackup);
        ShellConfigItemObject->SetBoolField(TEXT("修改权限"), ShellConfigItem->bSetPermissions);

        //ShellConfigItemObject->SetObjectField(TEXT("有效路径"), ShellConfigItem->SaveConfigFile());

        NewToolsConfig->SetObjectField(ShellConfigItem->OperationName, ShellConfigItemObject);
    }

    TSharedPtr<FJsonObject> BaseSettingObject = MakeShareable(new FJsonObject());
    BaseSettingObject->SetBoolField(TEXT("注册表开机自启动"), bAutoStarting);
    NewToolsConfig->SetObjectField(TEXT("基本设置"), BaseSettingObject);

    //覆盖配置
    PlayerControllerTools->ToolsConfig = NewToolsConfig;
}

void USettingWidget::LoadSettingConfigFile()
{
    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::LoadSettingConfigFile: 加载设置的配置文件!")));

    int32 tempIndex = 0;
    if (!IsValid(PlayerControllerTools)) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::LoadSettingConfigFile: PlayerControllerTools 无效!")), FColor::Red);
        return;
    }

    if (!PlayerControllerTools->ToolsConfig.IsValid()) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::LoadSettingConfigFile: PlayerControllerTools->ToolsConfig 无效!")), FColor::Red);
        return;
    }

    if (!IsValid(ListViewShellConfig)) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::LoadSettingConfigFile: ListViewShellConfig 无效!")), FColor::Red);
        return;
    }

    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::LoadSettingConfigFile: 开始遍历 ToolsConfig!")));
    for (auto& tempJsonObject : PlayerControllerTools->ToolsConfig->Values) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::LoadSettingConfigFile: ToolsConfig[%d]"), ++tempIndex));

        FString& Key = tempJsonObject.Key;
        TSharedPtr<FJsonValue>& Value = tempJsonObject.Value;

        //从配置文件加载: 基本设置
        if (Key.Equals(TEXT("基本设置"))) {
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::LoadSettingConfigFile: ToolsConfig[基本设置]")));

            TSharedPtr<FJsonObject> BaseSettingObject = Value->AsObject();
            bAutoStarting = BaseSettingObject->GetBoolField(TEXT("注册表开机自启动"));

            continue;
        }

        UShellConfigItem* ConfigItem = NewObject<UShellConfigItem>();
        ConfigItem->OperationName = Key;

        TSharedPtr<FJsonObject> OtherConfigItemObject = Value->AsObject();
        ConfigItem->SourceFile = OtherConfigItemObject->GetStringField(TEXT("源文件"));
        ConfigItem->DestinationPath = OtherConfigItemObject->GetStringField(TEXT("目的地路径"));
        ConfigItem->bStartBackup = OtherConfigItemObject->GetBoolField(TEXT("启动时备份"));
        ConfigItem->bSetPermissions = OtherConfigItemObject->GetBoolField(TEXT("修改权限"));

        //ConfigItem->LoadConfigFile(OtherConfigItemObject->GetObjectField(TEXT("有效路径")));
        ConfigItem->OutputLog();
        
        //从配置文件加载 ShellConfigItem
        ListViewShellConfigArray->Add(ConfigItem);

        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::LoadSettingConfigFile: UShellConfigItem[%s] 添加成功!"), *Key));
    }
}

bool USettingWidget::IsVaildFilePath(FString& _FilePath)
{
    return std::filesystem::exists(tytl::FileSystem(*_FilePath).GetPath());
}

TArray<FString> USettingWidget::StringManage(FString& _FilePath)
{
    // 替换所有 '\n' 符号为 '|'
    FString CleanStr = _FilePath
        /*.Replace(TEXT("-"), TEXT(""))*/
        .Replace(TEXT("\r\n"), TEXT("|")) // 替换 Windows 风格的换行符
        .Replace(TEXT("\r"), TEXT("|"))   // 替换 Mac 风格的换行符
        .Replace(TEXT("\n"), TEXT("|"));  // 替换 Unix/Linux 风格的换行符

    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::StringManage: 替换后的路径[%s]!"), *CleanStr));

    TArray<FString> ResultArray;
    CleanStr.ParseIntoArray(ResultArray, TEXT("|")); // 按 '|' 分割

    ResultArray.RemoveAll([](const FString& Str) { return Str.IsEmpty(); }); // 移除可能的空字符串（处理连续空格）

    return ResultArray;
}

bool USettingWidget::FilePathManage(FString& _FilePath, UShellConfigItem* _ListViewItem)
{
    if (!IsVaildFilePath(_FilePath)) { //无效路径
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::FilePathManage: 无效路径[%s]!"), *_FilePath), FColor::Red);
        return false;
    }
    if (!IsValid(_ListViewItem)) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::FilePathManage: 无效 ShellConfigItem对象! 路径[%s]"), *_FilePath), FColor::Red);
        return false;
    }

    tytl::FileSystem FileBackup(*_FilePath);
    _ListViewItem->SourceFilePathList->push_back(FileBackup.GetPath());
    _ListViewItem->SourceFileSizeSum += std::filesystem::file_size(FileBackup.GetPath());
    return true;
}

void USettingWidget::OnPathManage()
{
    UEtytl::DebugLog(TEXT("USettingWidget::OnPathManage: "));
    UEtytl::DebugLog(TEXT("USettingWidget::OnPathManage: "));
    UEtytl::DebugLog(TEXT("USettingWidget::OnPathManage: 路径处理中..."));
    OnApplyButtonIsEnabled(false);

    ListViewTipsMessageArray->Empty(); //清空 错误信息

    int32 Index = 1;
    for (auto Item : ListViewShellConfigArray->GetList())
    {
        if (!IsValid(Item)) {
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnPathManage: ListViewShellConfigArray [%d]项无效!"), Index), FColor::Red);
            continue;
        }
        UShellConfigItem* ShellConfigItem = Cast<UShellConfigItem>(Item);
        if (!IsValid(ShellConfigItem)) {
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnPathManage: ListViewShellConfigArray [%d]项无效! 转换 [UShellConfigItem]失败!"), Index), FColor::Red);
            continue;
        }

        if (ShellConfigItem->OperationName.IsEmpty()) {
            continue;
        }
        if (ShellConfigItem->SourceFile.IsEmpty()) {
            continue;
        }
        if (ShellConfigItem->DestinationPath.IsEmpty()) {
            continue;
        }

        // 处理源文件路径
        TArray<FString> TempFilePathList = StringManage(ShellConfigItem->SourceFile);
        int32 SourceItemIndex = 1;
        ShellConfigItem->SourceFileDirectoryList.clear();
        ShellConfigItem->SourceFilePathList->clear();

        for (auto& TempFilePath : TempFilePathList)
        {
            if (!FilePathManage(TempFilePath, ShellConfigItem))
            {
                UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnPathManage: 源文件路径 处理失败! 索引[%d][%d][%s]"), Index, SourceItemIndex, *TempFilePath), FColor::Red);

                ShellConfigItem->bCorrectSourceFile = false;

                UTextTips* ErrorPathTips = NewObject<UTextTips>();
                ErrorPathTips->Text = FString::Printf(TEXT("选项[%s] | 源文件路径[%s]"), *ShellConfigItem->OperationName, *TempFilePath);
                UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnPathManage: %s"), *ErrorPathTips->Text), FColor::Red);
                ListViewTipsMessageArray->Add(ErrorPathTips); //添加错误信息

                if (!IsValid(ListViewErrorPathTips)) {
                    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnPathManage: ListViewErrorPathTips 无效!")), FColor::Red);
                }
                else {
                    ListViewTipsMessageArray->SetList(); //刷新
                }

                ErrorPathTips = nullptr;
                continue;
            }
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnPathManage: 源文件路径 有效路径! 索引[%d][%d][%s]"), Index, SourceItemIndex, *TempFilePath));
            ShellConfigItem->bCorrectSourceFile = true;

            SourceItemIndex++;
        }

        // 处理目的地路径
        TArray<FString> TempDesitiantionPath = StringManage(ShellConfigItem->DestinationPath);
        if (ShellConfigItem->DestinationPath.IsEmpty()) {
            continue;
        }
        int32 DesitiantionPathIndex = 1;
        ShellConfigItem->DestinationPathList->clear();

        for (auto& TempFilePath : TempDesitiantionPath)
        {
            if (!IsVaildFilePath(TempFilePath))
            {
                //无效路径
                UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnPathManage: 无效目的地路径[%s]!"), *TempFilePath), FColor::Red);

                UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnPathManage: 目的地路径 非有效路径! 索引[%d][%d][%s]"), Index, DesitiantionPathIndex, *TempFilePath), FColor::Red);

                ShellConfigItem->bCorrectDestinationPath = false;

                UTextTips* ErrorPathTips = NewObject<UTextTips>();
                ErrorPathTips->Text = FString::Printf(TEXT("选项[%s] | 目的地路径[%s]"), *ShellConfigItem->OperationName, *TempFilePath);
                UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnPathManage: %s"), *ErrorPathTips->Text), FColor::Red);
                ListViewTipsMessageArray->Add(ErrorPathTips); //添加错误信息

                OnErrorMessageTips();

                ErrorPathTips = nullptr;
                continue;
            }
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnPathManage: 目的地路径 有效路径! 索引[%d][%d][%s]"), Index, DesitiantionPathIndex, *TempFilePath));
            ShellConfigItem->bCorrectDestinationPath = true;
            ShellConfigItem->DestinationPathList->push_back(std::filesystem::path(*TempFilePath));

            DesitiantionPathIndex++;
        }
        Index++;
    }

    OnErrorMessageTips();

    //所有任务完成
    if (ListViewTaskProgressArray->Num() == 0) {
        OnApplyButtonIsEnabled(true);
    }
}

bool USettingWidget::OperateCopyFile(FString& OperationName, const std::filesystem::path& _SourceItem, const std::filesystem::path& _DestinationPath, bool _bSetPermissions)
{
    tytl::FileSystem FileSystem(_SourceItem);
    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OperateCopyFile: 源文件[%s] ->\n目的地[%s]"), _SourceItem.wstring().c_str(), _DestinationPath.wstring().c_str()));

    //修改权限
    if (_bSetPermissions) {
        if (!FileSystem.SetPermissions(std::filesystem::perms::owner_read | std::filesystem::perms::owner_write, std::filesystem::perm_options::replace)) {
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OperateCopyFile: 修改权限[%s]失败!"), *OperationName), FColor::Red);
        }
        else {
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OperateCopyFile: 修改权限[%s]成功!"), *OperationName));
        }
    }

    UTextTips* ErrorPathTips = NewObject<UTextTips>();
    if (!FileSystem.Copy(_DestinationPath)) {
        const FString& tempError = FileSystem.GetErrorMessage().c_str();
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OperateCopyFile: 错误信息[%s]!"), *tempError));
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OperateCopyFile: 复制失败: [%s] ->\n [%s]"),
            _SourceItem.c_str(), _DestinationPath.c_str()), FColor::Red);

        ErrorPathTips->Text = FString::Printf(TEXT("选项[%s]|错误[%s]"), *OperationName, *tempError);
        ListViewTipsMessageArray->Add(ErrorPathTips); //添加错误信息

        OnErrorMessageTips();

        return false;
    }

    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OperateCopyFile: 复制成功: [%s] ->\n [%s]"),
        _SourceItem.c_str(), _DestinationPath.c_str()), FColor::Green);
    return true;
}

void USettingWidget::StartBackup(UShellConfigItem* _ShellConfigItem)
{
    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::StartBackup: 开始备份!")));
    if (!_ShellConfigItem) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::StartBackup: 无效 ShellConfigItem!")), FColor::Red);
        return;
    }
    OnApplyButtonIsEnabled(false);

    UTextTips* TextTips = NewObject<UTextTips>();
    TextTips->Text = _ShellConfigItem->OperationName;
    ListViewTaskProgressArray->Add(TextTips);
    RunBackupTask(_ShellConfigItem, TextTips);
}

void USettingWidget::RunBackupTask(UShellConfigItem* _ShellConfigItem, UTextTips* TextTips)
{
    Async(EAsyncExecution::Thread, [this, _ShellConfigItem, TextTips]()
        {
            float Progress = 1.f; //马上显示进度
            TextTips->SetRange(Progress);

            _ShellConfigItem->bEntryButtonIsEnabled = false;

            // 预先计算操作总数
            const int32 TotalFolderOps = _ShellConfigItem->SourceFileDirectoryList.size() * _ShellConfigItem->DestinationPathList->size();
            const int32 TotalFileOps = _ShellConfigItem->SourceFilePathList->size() * _ShellConfigItem->DestinationPathList->size();
            int32 CompletedOps = 0;

            // 第一阶段：复制文件夹
            for (const auto& SourceDir : _ShellConfigItem->SourceFileDirectoryList) {
                for (const auto& DestPath : *_ShellConfigItem->DestinationPathList) {
                    FString SourceStr(SourceDir.path().c_str());
                    FString DestStr(DestPath.c_str());

                    if (!OperateCopyFile(_ShellConfigItem->OperationName, SourceDir.path(), DestPath, _ShellConfigItem->bSetPermissions)) {
                    }
                    else {
                    }

                    // 更新进度
                    Progress = (static_cast<float>(++CompletedOps) / TotalFolderOps * .4f) + .1f; //
                    TextTips->SetRange(Progress);
                }
            }
            CompletedOps = 0;

            Progress = .5f;
            TextTips->SetRange(Progress);

            // 第二阶段：复制文件
            for (const auto& SourceFile : *_ShellConfigItem->SourceFilePathList) {
                for (const auto& DestPath : *_ShellConfigItem->DestinationPathList) {
                    FString SourceStr(SourceFile.c_str());
                    FString DestStr(DestPath.c_str());

                    if (!OperateCopyFile(_ShellConfigItem->OperationName, SourceFile, DestPath, _ShellConfigItem->bSetPermissions)) {
                    }
                    else {
                    }

                    // 更新进度
                    Progress = static_cast<float>(++CompletedOps) / TotalFileOps * .5f;
                    TextTips->SetRange(Progress);
                }
            }

            Progress = 1.f;
            TextTips->SetRange(Progress);
            ListViewTaskProgressArray->Remove(TextTips); //任务队列移除

            // 任务完成通知
            AsyncTask(ENamedThreads::GameThread, [this, _ShellConfigItem]()
                {
                    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::RunBackupTask: [%s]备份完成!"), *_ShellConfigItem->OperationName));
                    _ShellConfigItem->bEntryButtonIsEnabled = true;

                    OnErrorMessageTips();

                    //所有任务完成
                    if (ListViewTaskProgressArray->Num() == 0) { 
                        if (bAutoStartingToQuitGame) { //启动备份后自动退出
                            OnAutoQuit();
                        }

                        if (bQuitGame.load()) { //提示等待后退出
                            OnQuitGame();
                        }

                        OnApplyButtonIsEnabled(true);
                    }
                });
        });
}