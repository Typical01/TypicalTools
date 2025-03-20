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
	HMENU hMenu;  // 窗口句柄
    //菜单句柄
	int ID_Quit; //退出
	int ID_ToolsConfig; //工具箱配置
	int ID_SetScreenResolution; //修改屏幕分辨率

public:
	std::vector<tytl::Win::ShellConfig> ShellConfigVec; //[Shell] 配置项
	tytl::Win::WindowShell WindowShell; //[Shell] 配置项

public:

	void SelfStarting(bool bAutoStarting);
	void LoadShellConfig();
	void LoadToolsMenu();
};
