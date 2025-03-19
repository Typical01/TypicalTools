// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Widgets/SWindow.h"
#include "Tools_GameInstance.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TYPICALTOOLS_API UTools_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void OnStart();


public:
	UFUNCTION(BlueprintCallable)
	void DestroyDialogWindow();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int32 WindowWidth = 440;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int32 WindowHeight = 660;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FString DialogTitleName = { TEXT("Test") };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TEnumAsByte<EWindowMode::Type> Fullscreen = EWindowMode::Windowed;
	// UI实例
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UUserWidget* MainMenuWidget;

private:
	TSharedPtr<SWindow> DialogWindowMain;
};
