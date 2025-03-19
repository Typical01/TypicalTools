// Fill out your copyright notice in the Description page of Project Settings.


#include "EntryWidget.h"
#include "TypicalTool/Public/Tools.h"


void UEntryWidget::OutputLog()
{
    tytl::DebugLog(FString::Printf(TEXT("UEntryWidget::OutputLog: ")));
}


void UEntryWidget::SetListViewConfigItemMode(FString _ShellConfigItemMode)
{
    //ShellConfig: Mode
    if (ComboBoxStringShellConfigMode) {
        ComboBoxStringShellConfigMode->SetSelectedOption(_ShellConfigItemMode);
    }
    else {
        tytl::DebugLog(FString::Printf(TEXT("UShellConfigItemWidget::SetListViewConfigItemMode: ComboBoxStringShellConfigMode 无效!")));
    }
}
