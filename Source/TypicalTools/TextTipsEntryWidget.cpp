// Fill out your copyright notice in the Description page of Project Settings.


#include "TextTipsEntryWidget.h"

#include "TypicalTool/Public/Tools.h"



void UTextTipsEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    UTextTips* DataObject = Cast<UTextTips>(ListItemObject);
    if (!IsValid(DataObject)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UTextTipsEntryWidget::NativeOnListItemObjectSet: DataObject 无效!")), FColor::Red);
        return;
    }
    TextTips = DataObject;
    UEtytl::DebugLog(FString::Printf(TEXT("UTextTipsEntryWidget::NativeOnListItemObjectSet: TextTips[%s]!"), *TextTips->Text));

    if (!IsValid(TextBlockTips)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UTextTipsEntryWidget::NativeOnListItemObjectSet: TextBlockTips 无效!")), FColor::Red);
        return;
    }
    TextBlockTips->SetText(FText::FromString(TextTips->Text));
}