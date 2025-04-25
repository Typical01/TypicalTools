// Fill out your copyright notice in the Description page of Project Settings.


#include "TextTips.h"



UTextTips::UTextTips()
{
	Text = TEXT("");
	Range = 0.f;
}

void UTextTips::SetRange(float NewRange)
{
	Range = NewRange;
	OnProgressChanged.Broadcast(Range);
}
