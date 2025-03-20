// Fill out your copyright notice in the Description page of Project Settings.


#include "EntryWidget.h"
#include "TypicalTool/Public/Tools.h"


void UEntryWidget::OutputLog()
{
    UEtytl::DebugLog(FString::Printf(TEXT("UEntryWidget::OutputLog: ")));
}

void UEntryWidget::Init(UUserWidget* _SettingWidget)
{
    USettingWidget* tempSettingWidget = Cast<USettingWidget>(_SettingWidget);
    if (tempSettingWidget) {
        SettingWidget = tempSettingWidget;
        ListViewShellConfig = tempSettingWidget->ListViewShellConfig;
        ShellConfigItemIndex = tempSettingWidget->GetListViewConfigItemEnd();
        ShellConfigItem = Cast<UShellConfigItem>(tempSettingWidget->FindListViewIndex(ShellConfigItemIndex));
    }
}

void UEntryWidget::NativeOnListItemObjectSet(UObject* _ListItemObject)
{
    ShellConfigItem = Cast<UShellConfigItem>(_ListItemObject);
}


void UEntryWidget::SetListViewConfigItemMode(FString _ShellConfigItemMode)
{
    //ShellConfig: Mode
    if (ComboBoxStringShellConfigMode) {
        ComboBoxStringShellConfigMode->SetSelectedOption(_ShellConfigItemMode);
    }
    else {
        UEtytl::DebugLog(FString::Printf(TEXT("UShellConfigItemWidget::SetListViewConfigItemMode: ComboBoxStringShellConfigMode 无效!")));
    }
}
