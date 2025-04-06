// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingWidget.h"

#include "Engine/Engine.h"

#include "EntryWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"

#include "TypicalTool/Public/Tools.h"
#include "Tools_GameInstance.h"
#include "SettingItem.h"




USettingWidget::USettingWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

USettingWidget::~USettingWidget()
{
    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::~USettingWidget: 析构!")), FColor::Red);
}

void USettingWidget::InitializeListView()
{
    LoadSettingConfigFile();

    /*if (SettingItem) {
        SettingItem->DispalyDevice = SettingItem->GetAllSupportedResolutions();
        SettingItem->DispalyDevice.find(TEXT(""));
    }*/

    if (!ListViewShellConfig) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::InitializeListView: ListViewShellConfig 无效!")), FColor::Red);
    }

    MainTaskProgressBarShow(true);

    int32 Index = 1;
    for (auto tempItem : ListViewShellConfig->GetListItems()) {
        UShellConfigItem* tempShellConfigItem = Cast<UShellConfigItem>(tempItem);
        if (tempShellConfigItem) {
            if (tempShellConfigItem->StartBackup) {
                StartBackup(tempShellConfigItem);
                MainTaskProgress = static_cast<float>(Index) / ListViewShellConfig->GetNumItems() * 50.0f;
            }
        }
        else {
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::InitializeListView: tempShellConfigItem 无效! 索引[%d]"), Index), FColor::Red);
        }
        Index++;
    }

    MainTaskProgress = 100.f;
    MainTaskProgressBarShow(false);
}

int32 USettingWidget::FindListViewItem(UObject* _Item)
{
    if (ListViewShellConfig) {
        return ListViewShellConfig->GetListItems().Find(_Item);
    }
    else {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::FindListViewItem: ListViewShellConfig 无效!")), FColor::Red);
        return INDEX_NONE;
    }
}

UObject* USettingWidget::FindListViewIndex(int32 _Index)
{
    if (!ListViewShellConfig->GetListItems().IsEmpty()) {
        if (ListViewShellConfig->GetListItems().Num() > _Index) {
            return ListViewShellConfig->GetListItems()[_Index];
        }
        else {
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::FindListViewIndex: 没有找到对应索引[%d]对象!"), _Index));
            return nullptr;
        }
    }
    else {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::FindListViewIndex: ListViewShellConfig 无效!")), FColor::Red);
        return nullptr;
    }
}

void USettingWidget::DeleteListViewIndexItem(int32 _Index)
{
    if (!ListViewShellConfig->GetListItems().IsEmpty()) {
        if (ListViewShellConfig->GetListItems().Num() > _Index) {
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::FindListViewIndex: 删除前: 当前数组大小[%d]!"), ListViewShellConfig->GetListItems().Num()));

            DeleteListViewItem(ListViewShellConfig->GetListItems()[_Index]);

            for (int32 i = _Index; i < ListViewShellConfig->GetListItems().Num(); i++) {
                UShellConfigItem* tempShellConfigItem = Cast<UShellConfigItem>(ListViewShellConfig->GetListItems()[i]);
                tempShellConfigItem->ShellConfigItemIndex = i;
            }
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::FindListViewIndex: 删除后: 当前数组大小[%d]!"), ListViewShellConfig->GetListItems().Num()));
        }
        else {
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::FindListViewIndex: 没有找到对应索引[%d]对象!"), _Index));
        }
    }
    else {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::FindListViewIndex: ListViewShellConfig 无效!")), FColor::Red);
    }
}

void USettingWidget::OnApplyButton()
{
    MainTaskProgressBarShow(true);

    ComputeFileCount();
    OutputLog();

    SaveSettingConfigFile();

    if (Tools_GameInstance) {
        Tools_GameInstance->SaveConfigFile();
    }
 
    MainTaskProgressBarShow(false);
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
    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OutputLog: bAutoStarting[%d]"), bAutoStarting));
    if (ListViewShellConfig) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OutputLog: 列表视图ShellConfigItems")), FColor::Yellow);
        for (auto ListItem : ListViewShellConfig->GetListItems()) {
            tempCount++;
            UShellConfigItem* tempShellConfigItem = Cast<UShellConfigItem>(ListItem);
            if (tempShellConfigItem) {
                UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OutputLog: ListItem[%d]"), tempCount), FColor::Yellow);
                tempShellConfigItem->OutputLog();
            }
        }
    }
    tempCount = 0;
}

UShellConfigItem* USettingWidget::AddListViewConfigItem()
{
    UShellConfigItem* ConfigItem = NewObject<UShellConfigItem>();
    ConfigItem->Defualut();

    if (ListViewShellConfig) {
        ListViewShellConfig->AddItem(ConfigItem);
    }
    else {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::AddListViewConfigItem: ListViewShellConfig 无效!")));
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::AddListViewConfigItem: 添加列表视图项失败!")), FColor::Red);
    }

    return ConfigItem;
}

void USettingWidget::DeleteListViewItem(UObject* _Item)
{
    if (ListViewShellConfig && !ListViewShellConfig->GetListItems().IsEmpty()) {
        if (ListViewShellConfig->GetListItems().Contains(_Item)) {
            ListViewShellConfig->RemoveItem(_Item);
            _Item = nullptr;
        }
        else {
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::DeleteListViewItem: ListViewShellConfig: 没有该项!")));
        }
    }
    else {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::DeleteListViewItem: ListViewItem 删除失败!")), FColor::Red);
    }
}

int32 USettingWidget::GetListViewConfigItemIndex(UObject* _Item)
{
    if (!ListViewShellConfig->GetListItems().IsEmpty()) {
        int32 tempIndex = ListViewShellConfig->GetListItems().Find(_Item);
        if (tempIndex != INDEX_NONE) {
            return tempIndex;
        }
        else {
            return int32(INDEX_NONE);
        }
    }
    else {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::GetListViewConfigItemIndex: ListViewShellConfig 无效!")), FColor::Red);
        return int32(INDEX_NONE);
    }
}

int32 USettingWidget::GetListViewConfigItemEnd()
{
    if (!ListViewShellConfig->GetListItems().IsEmpty()) {
        int32 tempIndex = ListViewShellConfig->GetListItems().Num() - 1;
        if (tempIndex != -1) {
            return tempIndex;
        }
        else {
            return int32(INDEX_NONE);
        }
    }
    else {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::GetListViewConfigItemEnd: ListViewShellConfig 无效!")), FColor::Red);
        return int32(INDEX_NONE);
    }
}

void USettingWidget::SaveSettingConfigFile()
{
    if (Tools_GameInstance) {
        if (Tools_GameInstance->ToolsConfig.IsValid()) {
            TSharedPtr<FJsonObject> NewToolsConfig = MakeShareable(new FJsonObject());

            if (!ListViewShellConfig->GetListItems().IsEmpty()) {
                //保存到配置文件: ShellConfigItem
                for (auto tempJsonObject : ListViewShellConfig->GetListItems()) {
                    UShellConfigItem* ShellConfigItem = Cast<UShellConfigItem>(tempJsonObject);
                    ShellConfigItem->Defualut();

                    TSharedPtr<FJsonObject> ShellConfigItemObject = MakeShareable(new FJsonObject());
                    ShellConfigItemObject->SetStringField(TEXT("源文件"), ShellConfigItem->SourceFile);
                    ShellConfigItemObject->SetStringField(TEXT("目的地路径"), ShellConfigItem->DestinationPath);
                    ShellConfigItemObject->SetBoolField(TEXT("启动时备份"), ShellConfigItem->StartBackup);
                    ShellConfigItemObject->SetBoolField(TEXT("修改权限"), ShellConfigItem->SetPermissions);

                    ShellConfigItemObject->SetObjectField(TEXT("有效路径"), ShellConfigItem->SaveConfigFile());

                    NewToolsConfig->SetObjectField(ShellConfigItem->OperationName, ShellConfigItemObject);
                }
            }
            else {
                UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::SaveSettingConfigFile: ListViewShellConfig 无效!")), FColor::Red);
                UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::SaveSettingConfigFile: 添加列表视图项失败!")));
            }

            TSharedPtr<FJsonObject> BaseSettingObject = MakeShareable(new FJsonObject());
            BaseSettingObject->SetBoolField(TEXT("注册表开机自启动"), bAutoStarting);
            NewToolsConfig->SetObjectField(TEXT("基本设置"), BaseSettingObject);

            //覆盖配置
            Tools_GameInstance->ToolsConfig = NewToolsConfig;
        }
    }
}

void USettingWidget::LoadSettingConfigFile()
{
    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::LoadSettingConfigFile: 加载设置的配置文件!")), FColor::Yellow);

    int32 tempIndex = 0;
    if (Tools_GameInstance) {
        if (Tools_GameInstance->ToolsConfig.IsValid()) {
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::LoadSettingConfigFile: 开始遍历 ToolsConfig!")), FColor::Yellow);
            for (auto& tempJsonObject : Tools_GameInstance->ToolsConfig->Values) {
                UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::LoadSettingConfigFile: ToolsConfig[%d]"), ++tempIndex), FColor::Yellow);
                FString& Key = tempJsonObject.Key;
                TSharedPtr<FJsonValue>& Value = tempJsonObject.Value;

                //从配置文件加载: 基本设置
                UShellConfigItem* ConfigItem = NewObject<UShellConfigItem>();
                ConfigItem->Defualut();
                ConfigItem->OperationName = Key;

                TSharedPtr<FJsonObject> OtherConfigItemObject = Value->AsObject();
                ConfigItem->SourceFile = OtherConfigItemObject->GetStringField(TEXT("源文件"));
                ConfigItem->DestinationPath = OtherConfigItemObject->GetStringField(TEXT("目的地路径"));
                ConfigItem->StartBackup = OtherConfigItemObject->GetBoolField(TEXT("启动时备份"));
                ConfigItem->SetPermissions = OtherConfigItemObject->GetBoolField(TEXT("修改权限"));

                ConfigItem->LoadConfigFile(OtherConfigItemObject->GetObjectField(TEXT("有效路径")));

                UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::LoadSettingConfigFile: UShellConfigItem 添加中...")), FColor::Yellow);

                //从配置文件加载 ShellConfigItem
                ListViewShellConfig->AddItem(ConfigItem);

                UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::LoadSettingConfigFile: UShellConfigItem[%s] 添加成功!"), *Key), FColor::Yellow);
            }
        }
        else {
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::LoadSettingConfigFile: Tools_GameInstance->ToolsConfig 无效!")), FColor::Red);
        }
    }
    else {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::LoadSettingConfigFile: Tools_GameInstance 无效!")), FColor::Red);
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::LoadSettingConfigFile: SettingWidget 配置加载失败!")), FColor::Red);
    }
}

void USettingWidget::MainTaskProgressBarShow(bool bShow)
{
    if (ProgressBarMainTask) {
        if (bShow) {
            ProgressBarMainTask->SetVisibility(ESlateVisibility::Visible);
        }
        else {
            ProgressBarMainTask->SetVisibility(ESlateVisibility::Hidden);
            MainTaskProgress = -1.f;
        }
    }
    else {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::MainTaskProgressBarShow: ProgressBarMainTask 无效!")), FColor::Red);
    }
}

bool USettingWidget::IsVaildFilePath(FString& _FilePath)
{
    tytl::FileSystem FileBackup(*_FilePath);

    return std::filesystem::exists(FileBackup.GetPath());
}

TArray<FString> USettingWidget::StringManage(FString& _FilePath)
{
    //FString CleanStr = _FilePath
    //    .Replace(TEXT("-"), TEXT(""))
    //    .Replace(TEXT("\n"), TEXT(" ")); // 移除所有 "-"/"\n" 符号

    TArray<FString> ResultArray; 
    _FilePath.ParseIntoArray(ResultArray, TEXT("|")); // 按空格分割

    ResultArray.RemoveAll([](const FString& Str) { return Str.IsEmpty(); }); // 移除可能的空字符串（处理连续空格）

    return ResultArray;
}

bool USettingWidget::FilePathManage(FString& _FilePath, UShellConfigItem* _ListViewItem)
{
    if (!IsVaildFilePath(_FilePath)) { //无效路径
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::FilePathManage: 无效路径[%s]!"), *_FilePath), FColor::Red);

        return false;
    }
    else { //有效路径
        if (_ListViewItem) {
            tytl::FileSystem FileBackup(*_FilePath);

            if (std::filesystem::is_directory(FileBackup.GetPath())) { //目录
                //遍历目录
                _ListViewItem->SourceFileDirectoryList = FileBackup.DirectoryIterator(false, _ListViewItem->SourceFileSizeSum, _ListViewItem->SourceFilePathList);
            }
            else { //其他: 链接字符/普通文件
                _ListViewItem->SourceFilePathList->push_back(FileBackup.GetPath());
                _ListViewItem->SourceFileSizeSum += std::filesystem::file_size(FileBackup.GetPath());
            }
        }
        else {
            UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::FilePathManage: 无效 ShellConfigItem对象! 路径[%s]"), *_FilePath), FColor::Red);
            return false;
        }

        return true;
    }
}

void USettingWidget::ComputeFileCount()
{
    if (!ListViewShellConfig)
    {
        UEtytl::DebugLog(TEXT("无效 ListViewShellConfig!"), FColor::Red);
        return;
    }

    MainTaskProgressBarShow(true);

    int32 Index = 1;
    for (auto TempItem : ListViewShellConfig->GetListItems())
    {
        if (UShellConfigItem* TempShellConfigItem = Cast<UShellConfigItem>(TempItem))
        {
            float SourceFileCountProgress;
            float DestinationPathCountProgress;

            // 处理源文件路径
            TArray<FString> TempFilePathList = StringManage(TempShellConfigItem->SourceFile);
            int32 SourceItemIndex = 1;
            for (auto& TempFilePath : TempFilePathList)
            {
                if (!FilePathManage(TempFilePath, TempShellConfigItem))
                {
                    UEtytl::DebugLog(FString::Printf(TEXT("源文件路径 处理失败! 索引[%d][%d][%s]"), Index, SourceItemIndex, *TempFilePath), FColor::Red);
                }
                else {
                    UEtytl::DebugLog(FString::Printf(TEXT("源文件路径 有效路径! 索引[%d][%d][%s]"), Index, SourceItemIndex, *TempFilePath));
                    SourceFileCountProgress = static_cast<float>(SourceItemIndex) / ListViewShellConfig->GetNumItems() * 50.0f;

                    // 更新进度（切换到 GameThread）
                    MainTaskProgress = SourceFileCountProgress + DestinationPathCountProgress;
                }
                SourceItemIndex++;
            }

            // 处理目的地路径
            TArray<FString> TempDesitiantionPath = StringManage(TempShellConfigItem->DestinationPath);
            int32 DesitiantionPathIndex = 1;
            for (auto& TempFilePath : TempDesitiantionPath)
            {
                if (!IsVaildFilePath(TempFilePath))
                {
                    UEtytl::DebugLog(FString::Printf(TEXT("目的地路径 非有效路径! 索引[%d][%d][%s]"), Index, DesitiantionPathIndex, *TempFilePath), FColor::Red);
                }
                else {
                    UEtytl::DebugLog(FString::Printf(TEXT("目的地路径 有效路径! 索引[%d][%d][%s]"), Index, DesitiantionPathIndex, *TempFilePath));
                    DestinationPathCountProgress = static_cast<float>(DesitiantionPathIndex) / ListViewShellConfig->GetNumItems() * 50.0f;

                    // 更新进度（切换到 GameThread）
                    MainTaskProgress = SourceFileCountProgress + DestinationPathCountProgress;
                }
                DesitiantionPathIndex++;
            }
        }
        else
        {
            UEtytl::DebugLog(FString::Printf(TEXT("无效 tempShellConfigItem! 索引[%d]"), Index), FColor::Red);
        }
        Index++;
    }

    // 最终进度 100%
    MainTaskProgress = 100.f;
    MainTaskProgressBarShow(false);
}

bool USettingWidget::OperateCopyFile(std::filesystem::path _SourceItem, std::filesystem::path _DestinationPath)
{
    tytl::FileSystem FileSystem;
    FileSystem.SetPath(_SourceItem);
    return FileSystem.Copy(_DestinationPath, true);
}

void USettingWidget::StartBackup(UShellConfigItem* _ShellConfigItem)
{
    if (!_ShellConfigItem) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::StartBackup: 无效 ShellConfigItem!")), FColor::Red);
    }

    int32 SourceItemIndex = 1;
    int32 DesitiantionPathIndex = 1;
    for (auto& tempSourceFileDirectoryList : _ShellConfigItem->SourceFileDirectoryList) {
        for (auto& tempDestinationPath : *_ShellConfigItem->DestinationPathList) {
            if (!OperateCopyFile(tempSourceFileDirectoryList.path(), tempDestinationPath)) {
                UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::StartBackup: 复制失败! 源路径[%s]/目的地路径[%s]"), 
                    tempSourceFileDirectoryList.path().wstring().c_str(), tempDestinationPath.wstring().c_str()), FColor::Red);
            }
            _ShellConfigItem->Progress = static_cast<float>(SourceItemIndex) / _ShellConfigItem->SourceFileDirectoryList.size() * 50.0f;
        }
    }
    for (auto& tempSourceFilePathList : *_ShellConfigItem->SourceFilePathList) {
        for (auto& tempDestinationPath : *_ShellConfigItem->DestinationPathList) {
            if (!OperateCopyFile(tempSourceFilePathList, tempDestinationPath)) {
                UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::StartBackup: 复制失败! 源路径[%s]/目的地路径[%s]"),
                    tempSourceFilePathList.wstring().c_str(), tempDestinationPath.wstring().c_str()), FColor::Red);
            }
            _ShellConfigItem->Progress = static_cast<float>(DesitiantionPathIndex) / _ShellConfigItem->SourceFilePathList->size() * 50.0f;
        }
    }

    _ShellConfigItem->Progress = 100.f;
}
