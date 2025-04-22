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
    if (!IsValid(Tools_GameInstance)) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::Destruct: Tools_GameInstance 无效!")), FColor::Red);
    }
    else {
        Tools_GameInstance->GetTimerManager().ClearTimer(AutoPathManageTimerHandle);
    }

    Super::Destruct();
}

void USettingWidget::InitializeListView()
{
    LoadSettingConfigFile();
    bAutoStartingToQuitGame = UEtytl::GetCommandLineArgument(TEXT("-AutoQuit"), nullptr); //自动退出

    SetProgress(.2f);

    if (!IsValid(ListViewShellConfig)) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::InitializeListView: ListViewShellConfig 无效!")), FColor::Red);
        return;
    }
    ListViewShellConfig->SetListItems(ListViewShellConfigArray); //绑定 到ListView

    if (!IsValid(ListViewErrorPathTips)) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::InitializeListView: ListViewErrorPathTips 无效!")), FColor::Red);
        return;
    }
    ListViewErrorPathTips->SetListItems(ListViewErrorPathTipsArray); //绑定 到ListView

    OnPathManage();
    SetProgress(.4f);
    if (!IsValid(Tools_GameInstance)) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::InitializeListView: Tools_GameInstance 无效!")), FColor::Red);
    }
    else {
        Tools_GameInstance->GetTimerManager().SetTimer(
            AutoPathManageTimerHandle,                // 定时器句柄
            this,                         // 调用函数的对象
            &USettingWidget::OnPathManage,        // 定时触发的函数
            600.0f,                         // 初始延迟时间（秒）
            true,                        // 是否循环调用
            -1.0f                         // 循环调用的间隔时间（秒），-1.0f 表示使用初始延迟时间
        );
    }

    SetProgress(.6f);

    bTaskCompleteQueue.store(false);
    int32 Index = 1;
    for (auto tempShellConfigItem : ListViewShellConfigArray) {
        if (!IsValid(tempShellConfigItem)) {
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::InitializeListView: tempShellConfigItem 无效! 索引[%d]"), Index), FColor::Red);
            continue;
        }

        if (tempShellConfigItem->bStartBackup) {
            SetProgress(static_cast<float>(Index) / ListViewShellConfigArray.Num() * .5f);

            //if (!IsValid(tempShellConfigItem->EntryWidget)) {
            //    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::InitializeListView: tempShellConfigItem->EntryWidget 无效! 索引[%d]"), Index), FColor::Red);
            //}
            //else {
            //    tempShellConfigItem->EntryWidget->OnBackupButton(); //按下按钮
            //}
            StartBackup(tempShellConfigItem);
        }
        Index++;
    }

    SetProgress(1.f);
}

void USettingWidget::OnApplyButton()
{
    OnApplyButtonIsEnabled(false);

    OnPathManage();
    SetProgress(.2f);

    OutputLog();
    SetProgress(.4f);

    SaveSettingConfigFile();
    SetProgress(.6f);

    if (!IsValid(Tools_GameInstance)) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnApplyButton: Tools_GameInstance 无效!")), FColor::Red);
        return;
    }
    Tools_GameInstance->SaveConfigFile();
    SetProgress(.8f);
    OnSelfStarting();
    SetProgress(.9f);

    SetProgress(1.f);

    OnApplyButtonIsEnabled(true);
}

void USettingWidget::OnQuitButton()
{
    // 强制退出
    bQuitGame.store(true);

    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnQuitButton: bQuitGame [%s]!"), *LexToString(bQuitGame)), FColor::Red);
    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnQuitButton: bTaskCompleteQueue [%s]!"), *LexToString(bTaskCompleteQueue)), FColor::Red);
    if (!bTaskComplete.load() || !bTaskCompleteQueue.load()) {

        if (!IsValid(Tools_GameInstance)) {
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnQuitButton: Tools_GameInstance 无效!")), FColor::Red);
            return;
        }
        if (!Tools_GameInstance->DialogMainWindow.IsValid()) {
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnQuitButton: Tools_GameInstance->DialogMainWindow 无效!")), FColor::Red);
            return;
        }

        // 创建自定义对话框窗口
        TSharedRef<SWindow> DialogWindow = SNew(SWindow)
            .Title(FText::FromString(TEXT("提示")))
            .ClientSize(FVector2D(600, 120))
            .SupportsMinimize(false)
            .SupportsMaximize(false);

        FSlateFontInfo FontInfo = FCoreStyle::Get().GetFontStyle("Regular");
        FontInfo.Size = 24;

        // 创建对话框内容
        DialogWindow->SetContent(
            SNew(SBorder)
            .Padding(10)
            .HAlign(HAlign_Center)  // 水平居中
            .VAlign(VAlign_Center)  // 垂直居中
            [
                SNew(STextBlock)
                    .Text(FText::FromString(TEXT("任务进行中！完成后退出...")))
                    .Font(FontInfo)
            ]
        );

        // 将对话框添加到自定义窗口中
        FSlateApplication::Get().AddWindowAsNativeChild(DialogWindow, Tools_GameInstance->DialogMainWindow.ToSharedRef());
    }
    else {
        OnQuitGame();
    }
}

void USettingWidget::OnQuitGame()
{
    // 取消任务且退出
    if (bQuitGame.load()) {
        if (!IsValid(Tools_GameInstance)) {
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnQuitGame: Tools_GameInstance 无效!")), FColor::Red);
            return;
        }
        Tools_GameInstance->QuitGame();
    }
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

void USettingWidget::OnAutoQuit()
{
    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnAutoQuit: bAutoStartingToQuitGame [%s]!"), *LexToString(bAutoStartingToQuitGame)), FColor::Red);
    if (bAutoStartingToQuitGame) {
        OnQuitButton();
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
        if (ListViewErrorPathTipsArray.Num() != 0) {
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
            ListViewErrorPathTips->SetListItems(ListViewErrorPathTipsArray); //刷新
            
            AsyncTask(ENamedThreads::GameThread, [this]()
                {
                    ListViewErrorPathTips->RegenerateAllEntries();
                });
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

    for (auto tempShellConfigItem : ListViewShellConfigArray) {
        tempCount++;
        if (!IsValid(tempShellConfigItem)) {
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OutputLog: ListViewShellConfigArray [%d]项无效!"), tempCount), FColor::Red);
            continue;
        }

        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OutputLog: ListItem[%d]"), tempCount));
        tempShellConfigItem->OutputLog();
    }
    for (auto tempErrorTips : ListViewErrorPathTipsArray) {
        tempCount++;
        if (!IsValid(tempErrorTips)) {
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OutputLog: tempErrorTips [%d]项无效!"), tempCount), FColor::Red);
            continue;
        }

        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OutputLog: ListViewErrorPathTipsArray[%d]"), tempCount));
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OutputLog: ListViewErrorPathTipsArray[%s]"), *tempErrorTips->Text));
    }
    tempCount = 0;
}


int32 USettingWidget::FindListViewItem(UShellConfigItem* _Item)
{
    return ListViewShellConfigArray.Find(_Item);
}

UShellConfigItem* USettingWidget::FindListViewIndex(int32 _Index)
{
    if (ListViewShellConfigArray.IsEmpty()) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::FindListViewIndex: ListViewShellConfigArray 没有项!")), FColor::Red);
        return nullptr;
    }
    if (ListViewShellConfigArray.Num() <= _Index) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::FindListViewIndex: 没有找到对应索引[%d]对象!"), _Index));
        return nullptr;
    }

    return ListViewShellConfigArray[_Index];
}

void USettingWidget::DeleteListViewIndexItem(int32 _Index)
{
    UShellConfigItem* tempObj = FindListViewIndex(_Index);
    if (!IsValid(tempObj)) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::DeleteListViewIndexItem: 没有找到对应索引[%d]对象!"), _Index));
        return;
    }

    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::DeleteListViewIndexItem: 删除前: 当前数组大小[%d]!"), ListViewShellConfigArray.Num()));
    DeleteListViewItem(tempObj);
    for (int32 i = _Index; i < ListViewShellConfigArray.Num(); i++) {
        UShellConfigItem* tempShellConfigItem = Cast<UShellConfigItem>(ListViewShellConfigArray[i]);
        tempShellConfigItem->ShellConfigItemIndex = i;
    }
    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::DeleteListViewIndexItem: 删除后: 当前数组大小[%d]!"), ListViewShellConfigArray.Num()));
}

UShellConfigItem* USettingWidget::AddListViewConfigItem()
{
    UShellConfigItem* ConfigItem = NewObject<UShellConfigItem>();

    if (!IsValid(ListViewShellConfig)) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::AddListViewConfigItem: ListViewShellConfig 无效!")), FColor::Red);
        return nullptr;
    }
    ListViewShellConfigArray.Add(ConfigItem);

    ListViewShellConfig->SetListItems(ListViewShellConfigArray); // 重新设置整个列表

    AsyncTask(ENamedThreads::GameThread, [this]()
        {
            ListViewShellConfig->RegenerateAllEntries();
        });

    return ConfigItem;
}

void USettingWidget::DeleteListViewItem(UShellConfigItem* _Item)
{
    if (!IsValid(ListViewShellConfig)) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::DeleteListViewItem: ListViewShellConfig 无效!")), FColor::Red);
        return;
    }

    if (ListViewShellConfigArray.IsEmpty()) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::DeleteListViewItem: ListViewShellConfigArray 没有项!")), FColor::Red);
    }

    if (ListViewShellConfigArray.Remove(_Item) == 0) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::DeleteListViewItem: ListViewShellConfigArray: 没有该项!")), FColor::Red);
        _Item->OutputLog();
        return;
    }

    _Item = nullptr;
    ListViewShellConfig->SetListItems(ListViewShellConfigArray); // 重新设置整个列表

    AsyncTask(ENamedThreads::GameThread, [this]()
        {
            ListViewShellConfig->RegenerateAllEntries();
        });
}

int32 USettingWidget::GetListViewConfigItemIndex(UShellConfigItem* _Item)
{
    if (ListViewShellConfigArray.IsEmpty()) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::GetListViewConfigItemIndex: ListViewShellConfigArray 没有项!")));
        return int32(INDEX_NONE);
    }

    int32 tempIndex = ListViewShellConfigArray.Find(_Item);
    if (tempIndex != INDEX_NONE) {
        return tempIndex;
    }
    return int32(INDEX_NONE);
}

int32 USettingWidget::GetListViewConfigItemEnd()
{
    if (ListViewShellConfigArray.IsEmpty()) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::GetListViewConfigItemEnd: ListViewShellConfigArray 没有项!")));
        return int32(INDEX_NONE);
    }

    int32 tempIndex = ListViewShellConfigArray.Num() - 1;
 
    if (tempIndex > -1) {
        return tempIndex;
    }
    return int32(INDEX_NONE);
}

void USettingWidget::SaveSettingConfigFile()
{
    if (!IsValid(Tools_GameInstance)) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::SaveSettingConfigFile: Tools_GameInstance 无效!")), FColor::Red);
        return;
    }
    if (!Tools_GameInstance->ToolsConfig.IsValid()) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::SaveSettingConfigFile: Tools_GameInstance->ToolsConfig 无效!")), FColor::Red);
        return;
    }

    TSharedPtr<FJsonObject> NewToolsConfig = MakeShareable(new FJsonObject());

    if (!IsValid(ListViewShellConfig)) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::SaveSettingConfigFile: ListViewShellConfig 无效!")), FColor::Red);
    }
    if (ListViewShellConfigArray.IsEmpty()) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::SaveSettingConfigFile: ListViewShellConfigArray 没有项!")));
    }

    //保存到配置文件: ShellConfigItem
    for (auto ShellConfigItem : ListViewShellConfigArray) {
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
    Tools_GameInstance->ToolsConfig = NewToolsConfig;
}

void USettingWidget::LoadSettingConfigFile()
{
    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::LoadSettingConfigFile: 加载设置的配置文件!")));

    int32 tempIndex = 0;
    if (!IsValid(Tools_GameInstance)) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::LoadSettingConfigFile: Tools_GameInstance 无效!")), FColor::Red);
        return;
    }

    if (!Tools_GameInstance->ToolsConfig.IsValid()) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::LoadSettingConfigFile: Tools_GameInstance->ToolsConfig 无效!")), FColor::Red);
        return;
    }

    if (!IsValid(ListViewShellConfig)) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::LoadSettingConfigFile: ListViewShellConfig 无效!")), FColor::Red);
        return;
    }

    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::LoadSettingConfigFile: 开始遍历 ToolsConfig!")));
    for (auto& tempJsonObject : Tools_GameInstance->ToolsConfig->Values) {
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
        ListViewShellConfigArray.Add(ConfigItem);
        ListViewShellConfig->SetListItems(ListViewShellConfigArray); // 重新设置整个列表

        AsyncTask(ENamedThreads::GameThread, [this]()
            {
                ListViewShellConfig->RegenerateAllEntries();
            });

        //ConfigItem = nullptr;
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::LoadSettingConfigFile: UShellConfigItem[%s] 添加成功!"), *Key));
    }
}

void USettingWidget::SetProgress(float Progress)
{
    //if (!IsValid(ProgressBarMainTask)) {
    //    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::SetProgress: ProgressBarMainTask 无效!")), FColor::Red);
    //    return;
    //}

    //ProgressBarMainTask->SetVisibility(ESlateVisibility::Visible); //显示
    //Progress = FMath::Clamp(Progress, 0.0f, 1.0f);  //确保范围: 0-1 float
    //ProgressBarMainTask->SetPercent(Progress);
    //UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::SetProgress: Progress[%f]"), Progress), FColor::Green);

    ////满进度
    //if (Progress >= 1.f) { //进度条清空
    //    ProgressBarMainTask->SetPercent(0.f);
    //    ProgressBarMainTask->SetVisibility(ESlateVisibility::Collapsed); //隐藏
    //}
    //else {
    //    ProgressBarMainTask->SetVisibility(ESlateVisibility::Visible); //显示
    //}
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

    //if (std::filesystem::is_directory(FileBackup.GetPath())) { //目录
    //    ////遍历目录
    //    //_ListViewItem->SourceFileDirectoryList = FileBackup.DirectoryIterator(false, _ListViewItem->SourceFileSizeSum, _ListViewItem->SourceFilePathList);
    //    
    //    //遍历目录
    //    //FileBackup.DirectoryIterator(false, _ListViewItem->SourceFileSizeSum, _ListViewItem->SourceFilePathList);
    //    //_ListViewItem->SourceFileDirectoryList.push_back(std::filesystem::directory_entry(FileBackup.GetPath()));
    //}
    //else { //其他: 链接字符/普通文件
    //    _ListViewItem->SourceFilePathList->push_back(FileBackup.GetPath());
    //    _ListViewItem->SourceFileSizeSum += std::filesystem::file_size(FileBackup.GetPath());
    //}

    _ListViewItem->SourceFilePathList->push_back(FileBackup.GetPath());
    //_ListViewItem->SourceFileSizeSum += std::filesystem::file_size(FileBackup.GetPath());
    return true;
}

void USettingWidget::OnPathManage()
{
    UEtytl::DebugLog(TEXT("USettingWidget::OnPathManage: "));
    UEtytl::DebugLog(TEXT("USettingWidget::OnPathManage: "));
    UEtytl::DebugLog(TEXT("USettingWidget::OnPathManage: 路径处理中..."));
    OnApplyButtonIsEnabled(false);

    ListViewErrorPathTipsArray.Empty(); //清空 错误信息

    SetProgress(.2f);

    int32 Index = 1;
    for (auto TempShellConfigItem : ListViewShellConfigArray)
    {
        if (!IsValid(TempShellConfigItem)) {
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnPathManage: tempShellConfigItem 无效! 索引[%d]"), Index), FColor::Red);
            continue;
        }
        if (!IsValid(TempShellConfigItem)) {
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnPathManage: ListViewShellConfigArray [%d]项无效!"), Index), FColor::Red);
            continue;
        }

        float SourceFileCountProgress = 0.f;
        float DestinationPathCountProgress = 0.f;
        
        AutoBackupNum += TempShellConfigItem->bStartBackup ? 1 : 0; //统计 启动时备份的数量

        // 处理源文件路径
        TArray<FString> TempFilePathList = StringManage(TempShellConfigItem->SourceFile);
        int32 SourceItemIndex = 1;
        TempShellConfigItem->SourceFileDirectoryList.clear();
        TempShellConfigItem->SourceFilePathList->clear();

        for (auto& TempFilePath : TempFilePathList)
        {
            if (!FilePathManage(TempFilePath, TempShellConfigItem))
            {
                UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnPathManage: 源文件路径 处理失败! 索引[%d][%d][%s]"), Index, SourceItemIndex, *TempFilePath), FColor::Red);

                TempShellConfigItem->bCorrectSourceFile = false;

                UTextTips* ErrorPathTips = NewObject<UTextTips>();
                ErrorPathTips->Text = FString::Printf(TEXT("选项[%s] | 路径[%s]"), *TempShellConfigItem->OperationName, *TempFilePath);
                UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnPathManage: %s"), *ErrorPathTips->Text), FColor::Red);
                ListViewErrorPathTipsArray.Add(ErrorPathTips); //添加错误信息

                if (!IsValid(ListViewErrorPathTips)) {
                    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnPathManage: ListViewErrorPathTips 无效!")), FColor::Red);
                }
                else {
                    ListViewErrorPathTips->SetListItems(ListViewErrorPathTipsArray); //刷新

                    AsyncTask(ENamedThreads::GameThread, [this]()
                        {
                            ListViewErrorPathTips->RegenerateAllEntries();
                        });
                }

                ErrorPathTips = nullptr;
                continue;
            }
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnPathManage: 源文件路径 有效路径! 索引[%d][%d][%s]"), Index, SourceItemIndex, *TempFilePath));
            TempShellConfigItem->bCorrectSourceFile = true;
            SourceFileCountProgress = static_cast<float>(SourceItemIndex) / ListViewShellConfigArray.Num() * .5f;
            SetProgress(SourceFileCountProgress + DestinationPathCountProgress);

            SourceItemIndex++;
        }

        // 处理目的地路径
        TArray<FString> TempDesitiantionPath = StringManage(TempShellConfigItem->DestinationPath);
        int32 DesitiantionPathIndex = 1;
        TempShellConfigItem->DestinationPathList->clear();

        for (auto& TempFilePath : TempDesitiantionPath)
        {
            if (!IsVaildFilePath(TempFilePath))
            {
                //无效路径
                UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnPathManage: 无效目的地路径[%s]!"), *TempFilePath), FColor::Red);

                UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnPathManage: 目的地路径 非有效路径! 索引[%d][%d][%s]"), Index, DesitiantionPathIndex, *TempFilePath), FColor::Red);

                TempShellConfigItem->bCorrectDestinationPath = false;

                UTextTips* ErrorPathTips = NewObject<UTextTips>();
                ErrorPathTips->Text = FString::Printf(TEXT("选项[%s] | 路径[%s]"), *TempShellConfigItem->OperationName, *TempFilePath);
                UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnPathManage: %s"), *ErrorPathTips->Text), FColor::Red);
                ListViewErrorPathTipsArray.Add(ErrorPathTips); //添加错误信息

                OnErrorMessageTips();

                ErrorPathTips = nullptr;
                continue;
            }
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OnPathManage: 目的地路径 有效路径! 索引[%d][%d][%s]"), Index, DesitiantionPathIndex, *TempFilePath));
            TempShellConfigItem->bCorrectDestinationPath = true;
            TempShellConfigItem->DestinationPathList->push_back(std::filesystem::path(*TempFilePath));
            DestinationPathCountProgress = static_cast<float>(DesitiantionPathIndex) / ListViewShellConfigArray.Num() * .5f;
            SetProgress(SourceFileCountProgress + DestinationPathCountProgress);

            DesitiantionPathIndex++;
        }
        Index++;
    }

    // 最终进度 100%
    SetProgress(1.f);

    OnErrorMessageTips();
    OnApplyButtonIsEnabled(true);
}

bool USettingWidget::OperateCopyFile(std::filesystem::path _SourceItem, std::filesystem::path _DestinationPath, bool _bSetPermissions)
{
    tytl::FileSystem FileSystem(_SourceItem);
    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OperateCopyFile: 源文件[%s] ->\n目的地[%s]"), _SourceItem.wstring().c_str(), _DestinationPath.wstring().c_str()));

    //修改权限
    if (_bSetPermissions) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OperateCopyFile: 修改权限!")));
        FileSystem.SetPermissions(std::filesystem::perms::owner_read | std::filesystem::perms::owner_write |
            std::filesystem::perms::group_read | std::filesystem::perms::group_write |
            std::filesystem::perms::others_read | std::filesystem::perms::others_write);
    }

    if (!FileSystem.Copy(_DestinationPath, false)) {
        const FString& tempError = FileSystem.GetErrorMessage().c_str();
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OperateCopyFile: 错误信息[%s]!"), *tempError));
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OperateCopyFile: 复制失败: [%s] ->\n [%s]"),
            _SourceItem.c_str(), _DestinationPath.c_str()), FColor::Red);

        UTextTips* ErrorPathTips = NewObject<UTextTips>();
        ErrorPathTips->Text = tempError;
        ListViewErrorPathTipsArray.Add(ErrorPathTips); //添加错误信息

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

    RunBackupTask(_ShellConfigItem);
}

void USettingWidget::RunBackupTask(UShellConfigItem* _ShellConfigItem)
{
    bTaskComplete.store(false);

    Async(EAsyncExecution::Thread, [this, _ShellConfigItem]()
        {
            _ShellConfigItem->bEntryButtonIsEnabled = false;

            // 预先计算操作总数
            const int32 TotalFolderOps = _ShellConfigItem->SourceFileDirectoryList.size() * _ShellConfigItem->DestinationPathList->size();
            const int32 TotalFileOps = _ShellConfigItem->SourceFilePathList->size() * _ShellConfigItem->DestinationPathList->size();
            const int32 TotalOps = TotalFolderOps + TotalFileOps;
            int32 CompletedOps = 0;

            // 第一阶段：复制文件夹
            for (const auto& SourceDir : _ShellConfigItem->SourceFileDirectoryList) {
                if (bQuitGame.load()) {
                    OnQuitGame();
                    break;
                }

                for (const auto& DestPath : *_ShellConfigItem->DestinationPathList) {
                    if (bQuitGame.load()) {
                        OnQuitGame();
                        break;
                    }

                    FString SourceStr(SourceDir.path().c_str());
                    FString DestStr(DestPath.c_str());

                    if (!OperateCopyFile(SourceDir.path(), DestPath, _ShellConfigItem->bSetPermissions)) {
                    }
                    else {
                    }

                    // 更新进度
                    float Progress = static_cast<float>(++CompletedOps) / TotalOps;

                    SetProgress(Progress);
                }
            }

            // 第二阶段：复制文件
            for (const auto& SourceFile : *_ShellConfigItem->SourceFilePathList) {
                if (bQuitGame.load()) {
                    OnQuitGame();
                    break;
                }

                for (const auto& DestPath : *_ShellConfigItem->DestinationPathList) {
                    if (bQuitGame.load()) {
                        OnQuitGame();
                        break;
                    }

                    FString SourceStr(SourceFile.c_str());
                    FString DestStr(DestPath.c_str());

                    if (!OperateCopyFile(SourceFile, DestPath, _ShellConfigItem->bSetPermissions)) {
                    }
                    else {
                    }

                    // 更新进度
                    float Progress = static_cast<float>(++CompletedOps) / TotalOps;

                    SetProgress(Progress);
                }
            }

            // 任务完成通知
            AsyncTask(ENamedThreads::GameThread, [this, _ShellConfigItem]()
                {
                    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::RunBackupTask: [%s]备份完成!"), *_ShellConfigItem->OperationName));

                    _ShellConfigItem->bEntryButtonIsEnabled = true;
                    bTaskComplete.store(true);

                    ListViewShellConfigCompleteArray.Add(true);
                    if (ListViewShellConfigCompleteArray.Num() == AutoBackupNum) {
                        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::RunBackupTask: [%d]已完成的启动时备份项!"), ListViewShellConfigCompleteArray.Num()));
                        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::RunBackupTask: [%d]总启动时备份项!"), AutoBackupNum));
                        bTaskCompleteQueue.store(true);
                        OnAutoQuit();
                    }
                    /*if (!IsValid(_ShellConfigItem->EntryWidget)) {
                        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::RunBackupTask: _ShellConfigItem->EntryWidget 无效! 无法通知: 备份完成!")));
                        return;
                    }
                    _ShellConfigItem->EntryWidget->OnBackupButtonIsEnabled(true);*/

                    OnApplyButtonIsEnabled(true);
                    OnErrorMessageTips();
                    OnQuitGame();
                });
        });
}