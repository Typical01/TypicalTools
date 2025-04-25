// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Blueprint/IUserObjectListEntry.h"

#include "Components/TextBlock.h"

#include "TextTips.h"

#include "TextTipsEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class TYPICALTOOLS_API UTextTipsEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TextBlockTips; //文本框: 提示

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* ProgressBarTask; //进度条: 任务

public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UFUNCTION(BlueprintCallable, Category = "Tips")
	void SetProgress(float Progres);
	UFUNCTION(BlueprintCallable, Category = "Tips")
	void SetText(FText Text);
};
