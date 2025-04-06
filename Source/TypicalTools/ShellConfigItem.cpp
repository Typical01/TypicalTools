// Fill out your copyright notice in the Description page of Project Settings.


#include "ShellConfigItem.h"

UShellConfigItem::UShellConfigItem()
    : SourceFilePathList(std::make_shared<std::vector<std::filesystem::path>>()), DestinationPathList(std::make_shared<std::vector<std::filesystem::path>>())
{
    Defualut();
}

void UShellConfigItem::Defualut()
{
    this->OperationName = TEXT("");
    this->SourceFile = TEXT("");
    this->DestinationPath = TEXT("");
    this->Progress = -1.f;
    this->StartBackup = false;
    this->SetPermissions = false;
    this->ShellConfigItemIndex = 0;

    ItemID = FGuid::NewGuid(); //唯一ID

    UEtytl::DebugLog(FString::Printf(TEXT("UShellConfigItem::Defualut: 初始化为: NULL!")), FColor::Yellow);
}

bool UShellConfigItem::Equals(const UShellConfigItem* Other) const
{
    return this->ItemID == Other->ItemID;
}

TSharedPtr<FJsonObject> UShellConfigItem::SaveConfigFile()
{
    if (SourceFileDirectoryList.size() == 0 && SourceFilePathList->size() == 0) {
        return MakeShareable(new FJsonObject());
    }

    TSharedPtr<FJsonObject> tempData = MakeShareable(new FJsonObject());
    // 创建源文件夹列表数组
    TArray<TSharedPtr<FJsonValue>> DirectoryEntryArray;
    for (auto& tempDirectoryEntry : SourceFileDirectoryList)
    {
        DirectoryEntryArray.Add(MakeShareable(new FJsonValueString(tempDirectoryEntry.path().wstring().c_str())));
    }

    // 创建源文件路径列表数组
    TArray<TSharedPtr<FJsonValue>> FilePathArray;
    for (auto& tempDirectoryEntry : *SourceFilePathList)
    {
        FilePathArray.Add(MakeShareable(new FJsonValueString(tempDirectoryEntry.wstring().c_str())));
    }

    // 创建目的地文件路径列表数组
    TArray<TSharedPtr<FJsonValue>> DestinationPathListArray;
    for (auto& tempDirectoryEntry : *DestinationPathList)
    {
        DestinationPathListArray.Add(MakeShareable(new FJsonValueString(tempDirectoryEntry.wstring().c_str())));
    }
    tempData->SetArrayField(TEXT("源文件夹列表"), DirectoryEntryArray);
    tempData->SetArrayField(TEXT("源文件路径列表"), FilePathArray);
    tempData->SetArrayField(TEXT("目的地文件路径列表"), DestinationPathListArray);

    return tempData;
}

void UShellConfigItem::LoadConfigFile(const TSharedPtr<FJsonObject>& _SaveData)
{
    // 获取 "源文件夹列表" 数组
    TArray<FString> OutDirectoryEntryList;
    if (_SaveData->HasField(TEXT("源文件夹列表")))
    {
        const TArray<TSharedPtr<FJsonValue>>& DirectoryEntryArray = _SaveData->GetArrayField(TEXT("源文件夹列表"));
        for (const TSharedPtr<FJsonValue>& Value : DirectoryEntryArray)
        {
            if (Value.IsValid() && Value->Type == EJson::String)
            {
                OutDirectoryEntryList.Add(Value->AsString());
            }
        }

        for (auto& tempOutDirectoryEntryList : OutDirectoryEntryList)
        {
            SourceFileDirectoryList.push_back(std::filesystem::directory_entry(std::filesystem::path(*tempOutDirectoryEntryList)));
        }
    }

    // 获取 "源文件路径列表" 数组
    TArray<FString> OutFilePathList;
    if (_SaveData->HasField(TEXT("源文件路径列表")))
    {
        const TArray<TSharedPtr<FJsonValue>>& FilePathArray = _SaveData->GetArrayField(TEXT("源文件路径列表"));
        for (const TSharedPtr<FJsonValue>& Value : FilePathArray)
        {
            if (Value.IsValid() && Value->Type == EJson::String)
            {
                OutFilePathList.Add(Value->AsString());
            }
        }

        for (auto& tempOutFilePathList : OutFilePathList)
        {
            SourceFilePathList->push_back(std::filesystem::path(*tempOutFilePathList));
        }
    }

    // 获取 "目的地文件路径列表" 数组
    TArray<FString> OutDestinationPathList;
    if (_SaveData->HasField(TEXT("目的地文件路径列表")))
    {
        const TArray<TSharedPtr<FJsonValue>>& DestinationPathArray = _SaveData->GetArrayField(TEXT("目的地文件路径列表"));
        for (const TSharedPtr<FJsonValue>& Value : DestinationPathArray)
        {
            if (Value.IsValid() && Value->Type == EJson::String)
            {
                OutDestinationPathList.Add(Value->AsString());
            }
        }

        for (auto& tempOutDestinationPathList : OutDestinationPathList)
        {
            DestinationPathList->push_back(std::filesystem::path(*tempOutDestinationPathList));
        }
    }
}

void UShellConfigItem::OutputLog()
{
    UEtytl::DebugLog(FString::Printf(TEXT("UShellConfigItem::OutputLog: [%s]"), *OperationName));
    UEtytl::DebugLog(FString::Printf(TEXT("UShellConfigItem::OutputLog: [%s]"), *SourceFile));
    UEtytl::DebugLog(FString::Printf(TEXT("UShellConfigItem::OutputLog: [%s]"), *DestinationPath));
    UEtytl::DebugLog(FString::Printf(TEXT("UShellConfigItem::OutputLog: [%d]"), Progress));
    UEtytl::DebugLog(FString::Printf(TEXT("UShellConfigItem::OutputLog: [%d]"), StartBackup));
    UEtytl::DebugLog(FString::Printf(TEXT("UShellConfigItem::OutputLog: [%d]"), SetPermissions));
    UEtytl::DebugLog(FString::Printf(TEXT("UShellConfigItem::OutputLog: [%d]"), SourceFileSizeSum));
    UEtytl::DebugLog(FString::Printf(TEXT("UShellConfigItem::OutputLog: [%d]"), ShellConfigItemIndex));

    UEtytl::DebugLog(FString::Printf(TEXT("UShellConfigItem::OutputLog: SourceFileDirectoryList(源文件夹路径)")));
    for (auto& tempDirectoryEntry : SourceFileDirectoryList)
    {
        UEtytl::DebugLog(FString::Printf(TEXT("UShellConfigItem::OutputLog: [%s]"), tempDirectoryEntry.path().wstring().c_str()));
    }

    UEtytl::DebugLog(FString::Printf(TEXT("UShellConfigItem::OutputLog: SourceFilePathList(源文件路径)")));
    for (auto& tempDirectoryEntry : *SourceFilePathList)
    {
        UEtytl::DebugLog(FString::Printf(TEXT("UShellConfigItem::OutputLog: [%s]"), tempDirectoryEntry.wstring().c_str()));
        
    }

    UEtytl::DebugLog(FString::Printf(TEXT("UShellConfigItem::OutputLog: DestinationPathList(目的地路径)")));
    for (auto& tempDirectoryEntry : *DestinationPathList)
    {
        UEtytl::DebugLog(FString::Printf(TEXT("UShellConfigItem::OutputLog: [%s]"), tempDirectoryEntry.wstring().c_str()));
    }
}
