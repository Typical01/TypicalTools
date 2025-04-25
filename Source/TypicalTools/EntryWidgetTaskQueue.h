// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TextTipsEntryWidget.h"

#include "SettingWidget.h"

#include "EntryWidgetTaskQueue.generated.h"

/**
 * 
 */
UCLASS()
class TYPICALTOOLS_API UEntryWidgetTaskQueue : public UTextTipsEntryWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextTips* Task;
	
public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};
