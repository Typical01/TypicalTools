// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingItem.h"
#include "TypicalTool/Public/Tools.h"


void USettingItem::OutputLog()
{
    UEtytl::DebugLog(FString::Printf(TEXT("USettingItem::OutputLog: OriginalResolutionWidth[%s]"), *this->BeginResolutionWidth));
    UEtytl::DebugLog(FString::Printf(TEXT("USettingItem::OutputLog: OriginalResolutionHeight[%s]"), *this->BeginResolutionHeight));
    UEtytl::DebugLog(FString::Printf(TEXT("USettingItem::OutputLog: TargetResolutionWidth[%s]"), *this->TargetResolutionWidth));
    UEtytl::DebugLog(FString::Printf(TEXT("USettingItem::OutputLog: TargetResolutionHeight[%s]"), *this->TargetResolutionHeight));
}
