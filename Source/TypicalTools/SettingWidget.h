// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"
#include "Components/ProgressBar.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Tools_GameInstance.h"

#include "ShellConfigItem.h"
#include "TextTips.h"

#include "libTypical/Tool.h"
#include "Engine/World.h"
#include "TimerManager.h"

#include "SettingWidget.generated.h"



/**
 * 
 */
UCLASS(Blueprintable)
class TYPICALTOOLS_API USettingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTools_GameInstance* Tools_GameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UListView* ListViewShellConfig; //列表视图: 备份项

	TArray<UShellConfigItem*> ListViewShellConfigArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* ProgressBarMainTask; //进度条: 主任务

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* VerticalBoxErrorPath; //垂直框: 错误路径

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UListView* ListViewErrorPathTips; //列表视图: 错误路径提示

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* ButtonApply; //按钮: 保存

	TArray<UTextTips*> ListViewErrorPathTipsArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutoStarting = false; //开机自启动
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutoStartingToQuitGame = false; //自动备份后退出游戏

	FTimerHandle AutoPathManageTimerHandle;
	std::atomic<bool> bTaskComplete = true; //任务完成
	std::atomic<bool> bQuitGame = false; //退出

public:
	USettingWidget(const FObjectInitializer& ObjectInitializer);
	virtual void Destruct();

public:
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void InitializeListView();

	UFUNCTION(BlueprintCallable, Category = "Shell")
	void OnApplyButton();
	void OnApplyButtonIsEnabled(bool bIsEnabled);

	// 强制退出?
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void OnQuitButton();
	void OnQuitGame();

	UFUNCTION(BlueprintCallable, Category = "Shell")
	void OnAutoQuit(); //自动退出

	void OnSelfStarting(); //注册表开机自启动
	void OnErrorMessageTips(); //错误信息提示

	void OutputLog();

public:
	//UFUNCTION(BlueprintImplementableEvent, Category = "Config")
	
	//添加列表视图项
	UFUNCTION(BlueprintCallable, Category = "Shell")
	UShellConfigItem* AddListViewConfigItem();
	UFUNCTION(BlueprintCallable, Category = "Shell")
	int32 FindListViewItem(UShellConfigItem* _Item);
	UFUNCTION(BlueprintCallable, Category = "Shell")
	UShellConfigItem* FindListViewIndex(int32 _Index);
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void DeleteListViewIndexItem(int32 _Index);
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void DeleteListViewItem(UShellConfigItem* _Item);
	//获取列表视图项索引: Item
	UFUNCTION(BlueprintCallable, Category = "Shell")
	int32 GetListViewConfigItemIndex(UShellConfigItem* _Item);
	//获取列表视图项: Entry
	UFUNCTION(BlueprintCallable, Category = "Shell")
	int32 GetListViewConfigItemEnd();

	UFUNCTION(BlueprintCallable, Category = "Shell")
	void SaveSettingConfigFile();
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void LoadSettingConfigFile();
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void SetProgress(float Progress);

public:

	UFUNCTION(BlueprintCallable, Category = "Shell")
	bool IsVaildFilePath(FString& _FilePath); //文件是否有效
	UFUNCTION(BlueprintCallable, Category = "Shell")
	TArray<FString> StringManage(FString& _FilePath); //字符串处理
	UFUNCTION(BlueprintCallable, Category = "Shell")
	bool FilePathManage(FString& _FilePath, UShellConfigItem* _ListViewItem); //文件路径处理
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void OnPathManage(); //路径处理
	bool OperateCopyFile(std::filesystem::path _SourceItem, std::filesystem::path _DestinationPath, bool _bSetPermissions = false); //操作: 拷贝文件/修改权限

public:
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void StartBackup(UShellConfigItem* _ShellConfigItem); //开始备份

private:
	// 实际备份任务（在后台线程运行）
	void RunBackupTask(UShellConfigItem* _ShellConfigItem);
};
