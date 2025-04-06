// Fill out your copyright notice in the Description page of Project Settings.


#include "EntryWidget.h"
#include "TypicalTool/Public/Tools.h"


UEntryWidget::~UEntryWidget()
{
}

void UEntryWidget::OutputLog()
{
    UEtytl::DebugLog(FString::Printf(TEXT("UEntryWidget::OutputLog: ")));
}

void UEntryWidget::Init(UUserWidget* _SettingWidget)
{
    USettingWidget* tempSettingWidget = Cast<USettingWidget>(_SettingWidget);
    if (tempSettingWidget) {
        SettingWidget = tempSettingWidget;
        int32 tempShellConfigItemIndex = tempSettingWidget->GetListViewConfigItemEnd();
        UEtytl::DebugLog(FString::Printf(TEXT("UEntryWidget::Init(): 当前数组大小[%d]!"), tempSettingWidget->ListViewShellConfig->GetNumItems()));
        UEtytl::DebugLog(FString::Printf(TEXT("UEntryWidget::Init(): 索引赋值[%d]!"), tempShellConfigItemIndex));

        auto tempShellConfigItem = tempSettingWidget->FindListViewIndex(tempShellConfigItemIndex);
        if (tempShellConfigItem != nullptr) {
            ShellConfigItem = Cast<UShellConfigItem>(tempShellConfigItem);
            ShellConfigItem->ShellConfigItemIndex = tempShellConfigItemIndex;
        }
        else {
            UEtytl::DebugLog(FString::Printf(TEXT("UEntryWidget::Init(): 索引默认赋值失败!")), FColor::Red);
        }
    }
}

void UEntryWidget::OnBackupButton()
{
    MainTaskProgressBarShow(true);

    if (SettingWidget) {
        SettingWidget->StartBackup(ShellConfigItem);
    }

    MainTaskProgressBarShow(false);
}

void UEntryWidget::MainTaskProgressBarShow(bool bShow)
{
    if (ProgressBarItem) {
        if (bShow) {
            ProgressBarItem->SetVisibility(ESlateVisibility::Visible);
        }
        else {
            ProgressBarItem->SetVisibility(ESlateVisibility::Hidden);
            ShellConfigItem->Progress = -1.f;
        }
    }
    else {
        UEtytl::DebugLog(FString::Printf(TEXT("UEntryWidget::MainTaskProgressBarShow: ProgressBarItem 无效!")), FColor::Red);
    }
}