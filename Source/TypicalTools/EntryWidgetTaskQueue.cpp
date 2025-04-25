// Fill out your copyright notice in the Description page of Project Settings.


#include "EntryWidgetTaskQueue.h"
#include "ShellConfigItem.h"

void UEntryWidgetTaskQueue::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	Super::NativeOnListItemObjectSet(ListItemObject);

    Task = Cast<UTextTips>(ListItemObject);

    // 绑定委托
    Task->OnProgressChanged.AddDynamic(this, &Super::SetProgress);

    SetText(FText::FromString(Task->Text));
    SetProgress(Task->GetRange());
}
