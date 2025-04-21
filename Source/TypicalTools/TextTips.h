// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TextTips.generated.h"

/**
 * 
 */
UCLASS()
class TYPICALTOOLS_API UTextTips : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Text;

public:
	UTextTips();
};
