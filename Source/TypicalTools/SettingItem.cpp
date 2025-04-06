// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingItem.h"
#include "TypicalTool/Public/Tools.h"

#include <windows.h>
#include <set>
#include <algorithm>




bool Resolution::IsValid()
{
    if (Width < 0 && Height < 0) {
        return true;
    }
    return false;
}


USettingItem::USettingItem(const FObjectInitializer& ObjectInitializer)
{
    GetAllSupportedResolutions();
}

Resolution USettingItem::ConvertStringToResolution(FString& ResolutionGroup)
{
    // 用于存储分割后的宽度和高度字符串
    FString WidthString, HeightString;

    // 尝试以 'x' 为分隔符分割字符串
    if (ResolutionGroup.Split(TEXT("x"), &WidthString, &HeightString))
    {
        // 将宽度和高度字符串转换为整数
        int32 Width = FCString::Atoi(*WidthString);
        int32 Height = FCString::Atoi(*HeightString);

        // 返回包含宽度和高度的 Resolution 结构体
        return Resolution(Width, Height);
    }

    // 如果分割失败，返回默认的 Resolution 结构体
    return Resolution();
}

FString USettingItem::ConvertResolutionToString(const Resolution& Resolution)
{
    return FString::Printf(TEXT("%dx%d"), Resolution.Width, Resolution.Height);
}


Resolution USettingItem::GetResolution(FString& DisplayDeviceName, FString& ResolutionGroup)
{
    auto tempDispalyDevice = DispalyDevice.find(DisplayDeviceName);
    if (tempDispalyDevice != DispalyDevice.end()) {

        auto ResolutionVec = tempDispalyDevice->second;

        auto tempResolution = ConvertStringToResolution(ResolutionGroup);

        //分辨率列表
        for (auto tempIndex = ResolutionVec.begin(); tempIndex != ResolutionVec.end(); tempIndex++) {
            if (tempIndex->Width == tempResolution.Width && tempIndex->Height == tempResolution.Height) {
                return Resolution(tempIndex->Width, tempIndex->Height);
            }
            else {
                UEtytl::DebugLog(FString::Printf(TEXT("USettingItem::GetResolution: 没有所选分辨率! 分辨率组:[%s]"), *ResolutionGroup), FColor::Red);
            }
        }
    }
    else {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingItem::GetResolution: 没有所选显示器名失败! 显示器名:[%s]"), *DisplayDeviceName), FColor::Red);
    }

    return Resolution();
}

// 获取所有显示设备的所有支持分辨率
std::map<FString, std::vector<Resolution>> USettingItem::GetAllSupportedResolutions()
{
    std::set<Resolution> DesplayDevice; // 使用 map 自动排除重复的分辨率
    std::map<FString, std::vector<Resolution>> Resolutions; // 使用 map 自动排除重复的分辨率

    DWORD DeviceIndex = 0; // 设备索引，从 0 开始
    DISPLAY_DEVICE DisplayDevice; // 用于存储显示设备信息的结构体
    DisplayDevice.cb = sizeof(DisplayDevice); // 设置结构体大小

    // 枚举所有显示设备
    while (EnumDisplayDevices(NULL, DeviceIndex, &DisplayDevice, 0)) {
        if (!(DisplayDevice.StateFlags & DISPLAY_DEVICE_ACTIVE)) {
            DeviceIndex++;
            continue; // 只处理激活的显示设备
        }

        DEVMODE devMode; // 用于存储显示模式信息的结构体
        devMode.dmSize = sizeof(devMode); // 设置结构体大小
        DWORD modeIndex = 0; // 显示模式索引，从 0 开始

        // 枚举当前显示设备的所有显示模式
        while (EnumDisplaySettings(DisplayDevice.DeviceName, modeIndex, &devMode)) {
            DesplayDevice.insert(Resolution(devMode.dmPelsWidth, devMode.dmPelsHeight, DeviceIndex, devMode.dmDisplayFrequency));
            modeIndex++;
        }

        std::vector<Resolution> DesplayDeviceVec(DesplayDevice.begin(), DesplayDevice.end());
        Resolutions.insert({ DisplayDevice.DeviceName, DesplayDeviceVec });

        DeviceIndex++;
    }

    // 将结果转换为 vector 并返回
    DispalyDevice = std::map<FString, std::vector<Resolution>>(Resolutions.begin(), Resolutions.end());
    return DispalyDevice;
}

void USettingItem::OutputLog()
{
}
