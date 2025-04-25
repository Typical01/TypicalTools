// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Components/ProgressBar.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/MultiLineEditableText.h"
#include "Components/EditableText.h"
#include "Components/CheckBox.h"

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

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USettingWidget* SettingWidget; //设置UMG

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UShellConfigItem* ShellConfigItem; //ShellConfig项

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* ButtonBackup;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UMultiLineEditableText* MultiLineEditableTextSourceItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UMultiLineEditableText* MultiLineEditableTextDestinationPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UEditableText* EditableTextOperationName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCheckBox* CheckBoxStartBackup;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCheckBox* CheckBoxSetPermission;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ImageSouceFile; //源文件 背景

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ImageDestinationPath; //目的地路径 背景

public:
	virtual void NativeOnListItemObjectSet(UObject* ItemObject) override;

public:

	UFUNCTION(BlueprintCallable)
	void OnBackupButton();
};
