// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ComboBoxString.h"
#include "EntryWidget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TYPICALTOOLS_API UEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* ComboBoxStringShellConfigMode; //ShellConfig: 模式
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ShellConfigItemIndex; //ListView<ShellConfigItem> 索引

	void OutputLog();

public:
	UFUNCTION(BlueprintCallable, Category = "Shell")
	void SetListViewConfigItemMode(FString _ShellConfigItemMode);

};
