// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/SWindow.h"

#include "TypicalTool/Public/Tools.h"

#include "Tools_GameInstance.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TYPICALTOOLS_API UTools_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// UI实例
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UUserWidget* MainMenuWidget;

public:
	TSharedPtr<SWindow> DialogWindowMain;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int32 WindowWidth = 440;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int32 WindowHeight = 660;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FString DialogTitleName = { TEXT("Test") };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TEnumAsByte<EWindowMode::Type> Fullscreen = EWindowMode::Windowed;

	TSharedPtr<FJsonObject> ToolsConfig;

	class UTrayManager* TrayManager;

public:
	~UTools_GameInstance();
	virtual void OnStart();


public:
	UFUNCTION(BlueprintCallable)
	void DestroyDialogWindow();

	UFUNCTION(BlueprintCallable)
	void HideDialogWindow();

	void CreateConfigFile();
	void SaveConfigFile();

public:
	// 创建托盘图标
	UFUNCTION(BlueprintCallable, Category = "Tray")
	void CreateTrayIcon();
};
