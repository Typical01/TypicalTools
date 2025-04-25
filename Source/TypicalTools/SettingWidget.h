// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"
#include "Components/ProgressBar.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "PlayerControllerTools.h"

#include "ShellConfigItem.h"
#include "TextTips.h"

#include "libTypical/Tool.h"

#include "TypicalTool/Public/ListViewBaseDataManage.h"

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
	APlayerControllerTools* PlayerControllerTools;

	TSharedPtr<SWindow> DialogWindow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UListViewDataManage* ListViewShellConfigArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UListView* ListViewShellConfig; //列表视图: 备份项

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* ProgressBarMainTask; //进度条: 主任务
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* VerticalBoxErrorPath; //垂直框: 错误路径

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UListViewDataManage* ListViewTipsMessageArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UListView* ListViewErrorPathTips; //列表视图: 错误路径提示

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UListViewDataManage* ListViewTaskProgressArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UListView* ListViewTaskQueue; //列表视图: 任务队列

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* ButtonApply; //按钮: 保存

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutoStarting = false; //开机自启动
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutoStartingToQuitGame = false; //自动备份后退出游戏
	
	std::atomic<bool> bQuitGame = false; //退出游戏

	FTimerHandle AutoPathManageTimerHandle;

public:
	USettingWidget(const FObjectInitializer& ObjectInitializer);
	virtual void Destruct();

public:
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void InitializeListView();

	UFUNCTION(BlueprintCallable, Category = "Shell")
	void OnApplyButton();
	void OnApplyButtonIsEnabled(bool bIsEnabled);
	
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void OnAddItem();
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void OnDeleteItem(UShellConfigItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Shell")
	void OnQuitButton(bool bIsButton = true);
	void OnQuitGame();

	UFUNCTION(BlueprintCallable, Category = "Shell")
	void OnAutoQuit(); //自动退出

	void OnSelfStarting(); //注册表开机自启动
	void OnErrorMessageTips(); //错误信息提示

	void OutputLog();

public:
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void SaveSettingConfigFile();
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void LoadSettingConfigFile();

public:
	UFUNCTION(BlueprintCallable, Category = "Shell")
	bool IsVaildFilePath(FString& _FilePath); //文件是否有效
	UFUNCTION(BlueprintCallable, Category = "Shell")
	TArray<FString> StringManage(FString& _FilePath); //字符串处理
	UFUNCTION(BlueprintCallable, Category = "Shell")
	bool FilePathManage(FString& _FilePath, UShellConfigItem* _ListViewItem); //文件路径处理
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void OnPathManage(); //路径处理
	bool OperateCopyFile(FString& OperationName, const std::filesystem::path& _SourceItem, 
		const std::filesystem::path& _DestinationPath, bool _bSetPermissions = false); //操作: 拷贝文件/修改权限

public:
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void StartBackup(UShellConfigItem* _ShellConfigItem); //开始备份

private:
	// 实际备份任务（在后台线程运行）
	void RunBackupTask(UShellConfigItem* _ShellConfigItem, UTextTips* TextTips);
};
