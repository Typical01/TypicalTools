// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/SWindow.h"

//#include "TypicalTool/Public/Tools.h"

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
	TSharedPtr<SWindow> MainWindow;
	TSharedPtr<SWindow> DialogMainWindow;
	FTimerHandle TimerHandle;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int32 MainWindowWidth = 300;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int32 MainWindowHeight = 189;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int32 DialogWindowWidth = 500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int32 DialogWindowHeight = 900;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	float MainWindowHideInterval = 0.1f; //主窗口的隐藏间隔
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool bShowMainWindow = true; //主窗口 隐藏/显示
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TEnumAsByte<EWindowMode::Type> Fullscreen = EWindowMode::Windowed;

	TSharedPtr<FJsonObject> ToolsConfig;

public:
	virtual void OnStart() override;
	virtual void Shutdown() override;

public:
	UFUNCTION(BlueprintCallable)
	void QuitGame();
	void OnTimer();

	void CreateConfigFile();
	void SaveConfigFile();

};
