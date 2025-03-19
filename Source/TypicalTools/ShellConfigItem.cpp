// Fill out your copyright notice in the Description page of Project Settings.


#include "ShellConfigItem.h"
#include "TypicalTool/Public/Tools.h"

void UShellConfigItem::Initialize(FString _ItemName, FString _File, FString _Argument, int32 _Mode, bool _ShowWindow, bool _MenuButton)
{
    this->ItemName = _ItemName;
    this->File = _File;
    this->Argument = _Argument;
    this->Mode = _Mode;
    this->ShowWindow = _ShowWindow;
    this->MenuButton = _MenuButton;
}

void UShellConfigItem::Defualut()
{
    this->ItemName = TEXT("NULL");
    this->File = TEXT("NULL");
    this->Argument = TEXT("NULL");
    this->Mode = 0;
    this->ShowWindow = true;
    this->MenuButton = true;
    tytl::DebugLog(FString::Printf(TEXT("UShellConfigItem::Defualut: NULL!")), FColor::Yellow);
}

FString UShellConfigItem::ConvertIntToShellMode(int32 _ShellMode)
{
    switch (_ShellMode) {
    case 0: {
        return FString(TEXT("打开文件"));
    }
    case 1: {
        return FString(TEXT("打开文件夹"));
    }
    case 2: {
        return FString(TEXT("管理员运行"));
    }
    }
    return FString(TEXT("NULL"));
}

int32 UShellConfigItem::ConvertShellModeToInt(FString _ShellMode)
{
    if (_ShellMode.Equals(FString(TEXT("打开文件")))) {
        return 0;
    }
    else if (_ShellMode.Equals(FString(TEXT("打开文件夹")))) {
        return 1;
    }
    else if (_ShellMode.Equals(FString(TEXT("管理员运行")))) {
        return 2;
    }
    else {
        return 0;
    }
}

void UShellConfigItem::OutputLog()
{
    tytl::DebugLog(FString::Printf(TEXT("UShellConfigItem::OutputLog: [%s]"), *ItemName));
    tytl::DebugLog(FString::Printf(TEXT("UShellConfigItem::OutputLog: [%s]"), *File));
    tytl::DebugLog(FString::Printf(TEXT("UShellConfigItem::OutputLog: [%s]"), *Argument));
    tytl::DebugLog(FString::Printf(TEXT("UShellConfigItem::OutputLog: [%s]"), *ConvertIntToShellMode(Mode)));
    tytl::DebugLog(FString::Printf(TEXT("UShellConfigItem::OutputLog: [%d]"), ShowWindow));
    tytl::DebugLog(FString::Printf(TEXT("UShellConfigItem::OutputLog: [%d]"), MenuButton));
}
