// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include <map>
#include <vector>

#include "SettingItem.generated.h"


// 用于存储分辨率的结构体
struct Resolution {
	int32 Width;
	int32 Height;
	int32 DisplayID; //显示器ID
	int32 RefreshRate; //刷新率

	// 构造函数
	Resolution() : Width(-1), Height(-1), DisplayID(-1), RefreshRate(-1) {}
	Resolution(int32 Width, int32 Height) : Width(Width), Height(Height), DisplayID(0), RefreshRate(0) {}
	Resolution(int32 Width, int32 Height, int32 DisplayID, int32 RefreshRate)
		: Width(Width), Height(Height), DisplayID(DisplayID), RefreshRate(RefreshRate) {}

	bool IsValid();

	// 用于 set 去重和排序
	bool operator<(const Resolution& other) const {
		if (Width != other.Width) return Width < other.Width;
		if (Height != other.Height) return Height < other.Height;
		if (DisplayID != other.DisplayID) return DisplayID < other.DisplayID;
		return RefreshRate < other.RefreshRate;
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
	std::map<FString, std::vector<Resolution>> DispalyDevice;


	/*FString BeginResolutionWidth = { TEXT("1920") };
	FString BeginResolutionHeight = { TEXT("1080") };
	FString TargetResolutionWidth = { TEXT("1280") };
	FString TargetResolutionHeight = { TEXT("960") };*/

public:
	USettingItem(const FObjectInitializer& ObjectInitializer);

public:

	Resolution ConvertStringToResolution(FString& ResolutionGroup);
	FString ConvertResolutionToString(const Resolution& Resolution);
	Resolution GetResolution(FString& DisplayDeviceName, FString& ResolutionGroup);
	std::map<FString, std::vector<Resolution>> GetAllSupportedResolutions();

public:
	void OutputLog();
};
