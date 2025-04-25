// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TextTipsEntryWidget.h"
#include "EntryWidgetErrorTips.generated.h"

/**
 * 
 */
UCLASS()
class TYPICALTOOLS_API UEntryWidgetErrorTips : public UTextTipsEntryWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextTips* ErrorTips;

public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};
