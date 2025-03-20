// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "TypicalTool/Public/Tools.h"
#include "libTypical/Tool.h"

#include "ToolsMain.generated.h"

/**
 * 
 */
UCLASS()
class TYPICALTOOLS_API UToolsMain : public UObject
{
	GENERATED_BODY()
	
public:
	class USettingWidget* SettingWidget;

public:
	std::vector<tytl::Win::ShellConfig> ShellConfigVec; //[Shell] 配置项
	tytl::Win::WindowShell WindowShell; //[Shell] 配置项

public:

	void SelfStarting(bool bAutoStarting);
	void LoadShellConfig();
	void LoadToolsMenu(HMENU Menu);
};
