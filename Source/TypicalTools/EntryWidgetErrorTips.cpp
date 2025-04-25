// Fill out your copyright notice in the Description page of Project Settings.


#include "EntryWidgetErrorTips.h"
#include "TypicalTool/Public/Tools.h"

#include "TextTips.h"

void UEntryWidgetErrorTips::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    Super::NativeOnListItemObjectSet(ListItemObject);

    ErrorTips = Cast<UTextTips>(ListItemObject);

    SetText(FText::FromString(ErrorTips->Text));
}
