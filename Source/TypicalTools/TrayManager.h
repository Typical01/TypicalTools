// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "TypicalTool/Public/Tools.h"
#include "libTypical/Tool.h"

#include "SettingWidget.h"
#include "Tools_GameInstance.h"

// 引入Windows API
//#include "Windows/AllowWindowsPlatformTypes.h"
//#include <shellapi.h>
//#include "Windows/HideWindowsPlatformTypes.h"


#include "TrayManager.generated.h"



/**
 * 
 */
UCLASS()
class TYPICALTOOLS_API UTrayManager : public UObject
{
	GENERATED_BODY()

public:
    static USettingWidget* SettingWidget;
    static UTools_GameInstance* Tools_GameInstance;
    static class UToolsMain* ToolsMain;

public:
    // 创建托盘图标
    static void CreateTrayIcon();
    // 移除托盘图标
    static void RemoveTrayIcon();
    // 选择右键菜单
    static void SelectMenu(int MenuItemID);
    // 显示右键菜单
    static void ShowContextMenu(HWND hWnd);
    // 处理托盘图标事件
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
    static NOTIFYICONDATA nid; // 托盘图标数据
    static HWND hWnd;  // 窗口句柄
    static HMENU hMenu;  // 窗口句柄
    static const UINT TRAY_ICON_ID = 1; // 图标ID
    static const UINT WM_TRAYICON = WM_USER + 1; // 自定义消息ID

public:

    static tytl::Win::WindowShell WindowShell;
    static tytl::Win::WindowHost WindowHost;
    //菜单句柄
    static int ID_Quit; //退出
    static int ID_ToolsConfig; //工具箱配置
    static int ID_SetScreenResolution; //修改屏幕分辨率

    static bool bSetScreenResolution; //修改屏幕分辨率
};
