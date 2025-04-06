// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Components/ProgressBar.h"

#include "Components/ListView.h"
#include "SettingWidget.h"


#include "EntryWidget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TYPICALTOOLS_API UEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	~UEntryWidget();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USettingWidget* SettingWidget; //设置UMG
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UShellConfigItem* ShellConfigItem; //ShellConfig项
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* ProgressBarItem; //进度条

	void OutputLog();

public:
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void Init(UUserWidget* _SettingWidget);

	UFUNCTION(BlueprintCallable, Category = "Shell")
	void OnBackupButton();


	UFUNCTION(BlueprintCallable, Category = "Shell")
	void MainTaskProgressBarShow(bool bShow);
};
