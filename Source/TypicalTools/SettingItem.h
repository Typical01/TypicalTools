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
	FString BeginResolutionWidth = { TEXT("1920") };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BeginResolutionHeight = { TEXT("1080") };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TargetResolutionWidth = { TEXT("1280") };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TargetResolutionHeight = { TEXT("960") };

public:

public:
	void OutputLog();
};
