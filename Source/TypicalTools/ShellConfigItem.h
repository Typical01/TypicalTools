// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "libTypical/Tool.h"
#include "TypicalTool/Public/Tools.h"

#include "ShellConfigItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TYPICALTOOLS_API UShellConfigItem : public UObject
{
	GENERATED_BODY()

public:
    UShellConfigItem();

    UFUNCTION(BlueprintCallable, Category = "Shell")
    void Defualut();
    UFUNCTION(BlueprintCallable, Category = "Shell")
    bool Equals(const UShellConfigItem* Other) const;
    TSharedPtr<FJsonObject> SaveConfigFile();
    void LoadConfigFile(const TSharedPtr<FJsonObject>& _SaveData);

    void OutputLog();

public:
    UPROPERTY(BlueprintReadWrite, Category = "ListItem")
    FString OperationName; //操作名
    UPROPERTY(BlueprintReadWrite, Category = "ListItem")
    FString SourceFile; //源文件(夹)
    UPROPERTY(BlueprintReadWrite, Category = "ListItem")
    FString DestinationPath; //目的地路径
    UPROPERTY(BlueprintReadWrite, Category = "ListItem")
    float Progress; //进度
    UPROPERTY(BlueprintReadWrite, Category = "ListItem")
    bool StartBackup; //启动时备份
    UPROPERTY(BlueprintReadWrite, Category = "ListItem")
    bool SetPermissions; //设置文件/夹权限
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 SourceFileSizeSum; //源文件的总大小

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ShellConfigItemIndex; //当前项的索引

    std::vector<std::filesystem::directory_entry> SourceFileDirectoryList; //源文件夹列表
    std::shared_ptr<std::vector<std::filesystem::path>> SourceFilePathList; //源文件路径列表
    std::shared_ptr<std::vector<std::filesystem::path>> DestinationPathList; //目的地文件路径列表

    FGuid ItemID;
};
