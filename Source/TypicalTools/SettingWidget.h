// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"
#include "Components/ProgressBar.h"

#include "ShellConfigItem.h"

#include "libTypical/Tool.h"

#include "SettingWidget.generated.h"



/**
 * 
 */
UCLASS(Blueprintable)
class TYPICALTOOLS_API USettingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	class UTools_GameInstance* Tools_GameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UListView* ListViewShellConfig; //列表视图

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutoStarting = false; //开机自启动
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutoStartingToQuitGame = false; //自动备份后退出游戏

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MainTaskProgress = -1.f; //主要任务的进度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* ProgressBarMainTask; //进度条: 主要任务

public:
	USettingWidget(const FObjectInitializer& ObjectInitializer);
 	~USettingWidget();

public:
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void InitializeListView();
	UFUNCTION(BlueprintCallable, Category = "Shell")
	int32 FindListViewItem(UObject* _Item);
	UFUNCTION(BlueprintCallable, Category = "Shell")
	UObject* FindListViewIndex(int32 _Index);
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void DeleteListViewIndexItem(int32 _Index);
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void DeleteListViewItem(UObject* _Item);

	UFUNCTION(BlueprintCallable, Category = "Shell")
	void OnApplyButton();

	void OutputLog();

public:
	//UFUNCTION(BlueprintImplementableEvent, Category = "Config")
	
	//添加列表视图项
	UFUNCTION(BlueprintCallable, Category = "Shell")
	UShellConfigItem* AddListViewConfigItem();
	//获取列表视图项索引: Item
	UFUNCTION(BlueprintCallable, Category = "Shell")
	int32 GetListViewConfigItemIndex(UObject* _Item);
	//获取列表视图项: Entry
	UFUNCTION(BlueprintCallable, Category = "Shell")
	int32 GetListViewConfigItemEnd();

	UFUNCTION(BlueprintCallable, Category = "Shell")
	void SaveSettingConfigFile();
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void LoadSettingConfigFile();
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void MainTaskProgressBarShow(bool bShow);

public:

	UFUNCTION(BlueprintCallable, Category = "Shell")
	bool IsVaildFilePath(FString& _FilePath); //文件是否有效
	UFUNCTION(BlueprintCallable, Category = "Shell")
	TArray<FString> StringManage(FString& _FilePath); //字符串处理
	UFUNCTION(BlueprintCallable, Category = "Shell")
	bool FilePathManage(FString& _FilePath, UShellConfigItem* _ListViewItem); //路径处理
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void ComputeFileCount(); //异步: 计算文件数量/大小
	bool OperateCopyFile(std::filesystem::path _SourceItem, std::filesystem::path _DestinationPath); //操作: 拷贝文件
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void StartBackup(UShellConfigItem* _ShellConfigItem); //开始备份
};