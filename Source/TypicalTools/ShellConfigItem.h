// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShellConfigItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TYPICALTOOLS_API UShellConfigItem : public UObject
{
	GENERATED_BODY()

public:
    UShellConfigItem() : Super() {};
    
    UFUNCTION(BlueprintCallable, Category = "Shell")
    void Initialize(FString _ItemName, FString _File, FString _Argument, FString _Mode, bool _ShowWindow, bool _MenuButton);
    UFUNCTION(BlueprintCallable, Category = "Shell")
    void Defualut();

    UFUNCTION(BlueprintCallable, Category = "Shell")
    static FString ConvertIntToShellMode(int32 _ShellMode);
    UFUNCTION(BlueprintCallable, Category = "Shell")
    static int32 ConvertShellModeToInt(FString _ShellMode);

    void OutputLog();

public:
    UPROPERTY(BlueprintReadWrite, Category = "ListItem")
    FString ItemName;
    UPROPERTY(BlueprintReadWrite, Category = "ListItem")
    FString File;
    UPROPERTY(BlueprintReadWrite, Category = "ListItem")
    FString Argument;
    UPROPERTY(BlueprintReadWrite, Category = "ListItem")
    FString Mode;
    UPROPERTY(BlueprintReadWrite, Category = "ListItem")
    bool ShowWindow;
    UPROPERTY(BlueprintReadWrite, Category = "ListItem")
    bool MenuButton;
};
