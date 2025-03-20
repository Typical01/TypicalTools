// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include <vector>

#include "SettingItem.generated.h"


// 用于存储分辨率的结构体
struct Resolution {
	int width;
	int height;
	int refreshRate;

	bool operator<(const Resolution& other) const {
		if (width != other.width) return width < other.width;
		if (height != other.height) return height < other.height;
		return refreshRate < other.refreshRate;
	}
};

/**
 * 
 */
UCLASS(Blueprintable)
class TYPICALTOOLS_API USettingItem : public UObject
{
	GENERATED_BODY()

public:

	std::vector<Resolution> ResolutionVec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BeginResolutionWidth = { TEXT("1920") };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BeginResolutionHeight = { TEXT("1080") };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TargetResolutionWidth = { TEXT("1280") };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TargetResolutionHeight = { TEXT("960") };

public:
	void OutputLog();
};

std::vector<Resolution> GetAllSupportedResolutions();