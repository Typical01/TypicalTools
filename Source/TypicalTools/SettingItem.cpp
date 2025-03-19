// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingItem.h"
#include "TypicalTool/Public/Tools.h"

void USettingItem::OutputLog()
{
    tytl::DebugLog(FString::Printf(TEXT("USettingItem::OutputLog: OriginalResolutionWidth[%s]"), *this->OriginalResolutionWidth));
    tytl::DebugLog(FString::Printf(TEXT("USettingItem::OutputLog: OriginalResolutionHeight[%s]"), *this->OriginalResolutionHeight));
    tytl::DebugLog(FString::Printf(TEXT("USettingItem::OutputLog: TargetResolutionWidth[%s]"), *this->TargetResolutionWidth));
    tytl::DebugLog(FString::Printf(TEXT("USettingItem::OutputLog: TargetResolutionHeight[%s]"), *this->TargetResolutionHeight));
}
