// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"
#include "Components/EditableTextBox.h"
#include "ShellConfigItem.h"
#include "SettingItem.h"
#include "EntryWidget.h"
#include "SettingWidget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TYPICALTOOLS_API USettingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UListView* ListViewShellConfig; //列表视图
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USettingItem* SettingItem; //设置项

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutoStarting = false; //开机自启动

public:
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void InitializeLiListView();
	UFUNCTION(BlueprintCallable, Category = "Shell")
	int32 FindListViewItem(UObject* _Item);
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void DeleteListViewItem(UObject* _Item);

	UFUNCTION(BlueprintCallable, Category = "Shell")
	void OnApplyButton();
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void OnDestroyListViewWidget(UObject* _Item);

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
	UUserWidget* GetListViewConfigItemEntry(UObject* _Item);
	//获取最后的列表视图项
	UFUNCTION(BlueprintCallable, Category = "Shell")
	int32 GetListViewConfigItemEnd();

	UFUNCTION(BlueprintCallable, Category = "Shell")
	void SaveListViewConfigItem();
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void LoadListViewConfigItem();
};