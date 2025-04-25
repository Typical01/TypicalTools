// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TextTips.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProgressChanged, float, NewRange);

/**
 * 
 */
UCLASS(Blueprintable)
class TYPICALTOOLS_API UTextTips : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Text = TEXT("");

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnProgressChanged OnProgressChanged;

private:
	float Range = 0.f;

public:
	UTextTips();

	UFUNCTION(BlueprintCallable, Category = "Tips")
	void SetRange(float NewRange);
	UFUNCTION(BlueprintCallable, Category = "Tips")
	float GetRange() { return Range; }
};
