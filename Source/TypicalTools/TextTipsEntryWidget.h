// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextTips* TextTips; //错误路径提示

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TextBlockTips; //错误路径提示

public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};
