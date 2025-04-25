// Fill out your copyright notice in the Description page of Project Settings.


#include "TextTipsEntryWidget.h"

#include "TypicalTool/Public/Tools.h"



void UTextTipsEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    if (!IsValid(ListItemObject)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UTextTipsEntryWidget::NativeOnListItemObjectSet: ListItemObject 无效!")), FColor::Red);
        return;
    }
}

void UTextTipsEntryWidget::SetProgress(float Progress)
{
    if (!IsValid(ProgressBarTask)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UTextTipsEntryWidget::SetProgress: ProgressBarTask 无效!")), FColor::Red);
        return;
    }
    Progress = FMath::Clamp(Progress, 0.f, 1.f);

    AsyncTask(ENamedThreads::GameThread, [this, Progress]()
        {
            UEtytl::DebugLog(FString::Printf(TEXT("UTextTipsEntryWidget::SetProgress: Progress[%f]"), Progress));
            ProgressBarTask->SetPercent(Progress);
        });
}

void UTextTipsEntryWidget::SetText(FText Text)
{
    if (!IsValid(TextBlockTips)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UTextTipsEntryWidget::SetText: TextBlockTips 无效!")), FColor::Red);
        return;
    }
    AsyncTask(ENamedThreads::GameThread, [this, Text]()
        {
            UEtytl::DebugLog(FString::Printf(TEXT("UTextTipsEntryWidget::SetText: Text[%s]"), *Text.ToString()));
            TextBlockTips->SetText(Text);
        });
}