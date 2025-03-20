// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingItem.h"
#include "TypicalTool/Public/Tools.h"

#include <windows.h>
#include <set>
#include <vector>
#include <algorithm>



void USettingItem::OutputLog()
{
    UEtytl::DebugLog(FString::Printf(TEXT("USettingItem::OutputLog: OriginalResolutionWidth[%s]"), *this->BeginResolutionWidth));
    UEtytl::DebugLog(FString::Printf(TEXT("USettingItem::OutputLog: OriginalResolutionHeight[%s]"), *this->BeginResolutionHeight));
    UEtytl::DebugLog(FString::Printf(TEXT("USettingItem::OutputLog: TargetResolutionWidth[%s]"), *this->TargetResolutionWidth));
    UEtytl::DebugLog(FString::Printf(TEXT("USettingItem::OutputLog: TargetResolutionHeight[%s]"), *this->TargetResolutionHeight));
}


std::vector<Resolution> GetAllSupportedResolutions()
{
    std::set<Resolution> resolutions;
    DEVMODE devMode = { 0 };
    devMode.dmSize = sizeof(DEVMODE);

    // 枚举所有显示设置
    for (int i = 0; EnumDisplaySettings(nullptr, i, &devMode); i++) {
        Resolution res = { devMode.dmPelsWidth, devMode.dmPelsHeight, devMode.dmDisplayFrequency };
        resolutions.insert(res);
    }

    // 将结果转换为vector并返回
    return std::vector<Resolution>(resolutions.begin(), resolutions.end());
}