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
    bool Equals(const UShellConfigItem* Other) const;
    TSharedPtr<FJsonObject> SaveConfigFile();
    void LoadConfigFile(const TSharedPtr<FJsonObject>& _SaveData);

    void OutputLog();

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ListItem")
    FString OperationName; //操作名

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ListItem")
    FString SourceFile; //源文件(夹)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ListItem")
    FString DestinationPath; //目的地路径

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ListItem")
    bool bCorrectSourceFile; //源文件有效
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ListItem")
    bool bCorrectDestinationPath; //目的地路径有效

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ListItem")
    bool bEntryButtonIsEnabled; //按键是否有效

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ListItem")
    bool bStartBackup; //启动时备份
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ListItem")
    bool bSetPermissions; //设置文件/夹权限
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 SourceFileSizeSum; //源文件的总大小

    std::vector<std::filesystem::directory_entry> SourceFileDirectoryList; //源文件夹列表
    std::shared_ptr<std::vector<std::filesystem::path>> SourceFilePathList; //源文件路径列表
    std::shared_ptr<std::vector<std::filesystem::path>> DestinationPathList; //目的地文件路径列表

    FGuid ItemID;
};
