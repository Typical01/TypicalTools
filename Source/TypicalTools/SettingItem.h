// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SettingItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TYPICALTOOLS_API USettingItem : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString OriginalResolutionWidth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString OriginalResolutionHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TargetResolutionWidth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TargetResolutionHeight;
	
public:
	void OutputLog();
};
