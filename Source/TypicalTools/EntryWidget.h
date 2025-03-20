// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"

#include "Components/ListView.h"
#include "SettingWidget.h"
#include "Components/ComboBoxString.h"

#include "EntryWidget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TYPICALTOOLS_API UEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* ComboBoxStringShellConfigMode; //ShellConfig: 模式
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ShellConfigItemIndex; //ListView<ShellConfigItem> 索引
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USettingWidget* SettingWidget; //SettingWidget
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UListView* ListViewShellConfig; //ListView
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UShellConfigItem* ShellConfigItem; //ShellConfigItem

	void OutputLog();

public:
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void Init(UUserWidget* _SettingWidget);

	virtual void NativeOnListItemObjectSet(UObject* _ListItemObject) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void SetListViewConfigItemMode(FString _ShellConfigItemMode);
};
