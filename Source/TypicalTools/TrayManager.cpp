// Fill out your copyright notice in the Description page of Project Settings.


#include "TrayManager.h"
#include "Engine/GameViewportClient.h"
#include "Framework/Application/SlateApplication.h"

#include "resource.h"
#include "Runtime/Launch/Resources/Windows/resource.h"

#include "TypicalTool/Public/Tools.h"
#include "ToolsMain.h"


using namespace tytl;

USettingWidget* UTrayManager::SettingWidget = nullptr;
UTools_GameInstance* UTrayManager::Tools_GameInstance = nullptr;
UToolsMain* UTrayManager::ToolsMain = nullptr;
NOTIFYICONDATA UTrayManager::nid = {};
HWND UTrayManager::hWnd = nullptr;
HMENU UTrayManager::hMenu = nullptr;

int UTrayManager::ID_Quit = tytl::Win::WindowHost::GetHMENU(); //退出
int UTrayManager::ID_ToolsConfig = tytl::Win::WindowHost::GetHMENU(); //工具箱配置
int UTrayManager::ID_SetScreenResolution = tytl::Win::WindowHost::GetHMENU(); //修改屏幕分辨率
bool UTrayManager::bSetScreenResolution = false; //修改屏幕分辨率


void UTrayManager::CreateTrayIcon()
{
    // 获取窗口句柄
    UGameViewportClient* GameViewportClient = GEngine->GameViewport;
    if (GameViewportClient && GameViewportClient->GetWindow().IsValid()) {
        TSharedPtr<SWindow> Window = GameViewportClient->GetWindow();
        if (Window.IsValid()) {
            TSharedPtr<FGenericWindow> GenericWindow = Window->GetNativeWindow();
            if (GenericWindow.IsValid()) {
                void* WindowHandle = GenericWindow->GetOSWindowHandle();
                // 将 WindowHandle 转换为 HWND（仅在 Windows 平台上适用）
                hWnd = (HWND)WindowHandle;
            }
        }
    }

    // 设置托盘图标数据
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hWnd;
    nid.uID = TRAY_ICON_ID;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;

    HICON hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDICON_UEGame)); //加载UE默认图标
    if (hIcon == nullptr) {
        DWORD error = GetLastError();
        // 输出错误信息
        UEtytl::DebugLog(FString::Printf(TEXT("UTrayManager::CreateTrayIcon: 图标加载错误 [%d]"), error));
    }
    nid.hIcon = hIcon;
    lstrcpy(nid.szTip, TEXT("Typical01 Tools v2.0")); // 提示信息

    // 添加托盘图标
    Shell_NotifyIcon(NIM_ADD, &nid);

    // 替换窗口消息处理函数
    SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)&WndProc);

    ToolsMain = NewObject<UToolsMain>();

    if (Tools_GameInstance) {
        Tools_GameInstance->DialogWindowMain->HideWindow();
        ToolsMain->SettingWidget = Cast<USettingWidget>(Tools_GameInstance->MainMenuWidget);
    }
    else {
        UEtytl::DebugLog(FString::Printf(TEXT("UTrayManager::CreateTrayIcon: Tools_GameInstance 无效!")));
    }

    if (SettingWidget) {
        SettingWidget->ToolsMain = ToolsMain;
    }
    else {
        UEtytl::DebugLog(FString::Printf(TEXT("UTrayManager::CreateTrayIcon: SettingWidget 无效!")));
    }


    if (ToolsMain) {
        ToolsMain->LoadShellConfig();

        // 创建右键菜单
        hMenu = CreatePopupMenu();
        ToolsMain->hMenu = hMenu;
        ToolsMain->ID_Quit = ID_Quit;
        ToolsMain->ID_ToolsConfig = ID_ToolsConfig;
        ToolsMain->ID_SetScreenResolution = ID_SetScreenResolution;

        //为Menu添加选项
        ToolsMain->LoadToolsMenu();

        ToolsMain->WindowShell.ExeRunItemShell();
    }
    else {
        UEtytl::DebugLog(FString::Printf(TEXT("UTrayManager::CreateTrayIcon: Shell 配置加载错误!")));
    }
}

void UTrayManager::RemoveTrayIcon()
{
    // 移除托盘图标
    Shell_NotifyIcon(NIM_DELETE, &nid);
    DestroyMenu(hMenu);
    SettingWidget = nullptr;
    Tools_GameInstance = nullptr;
}

void UTrayManager::SelectMenu(int MenuItemID)
{
    UEtytl::DebugLog(FString::Printf(TEXT("UTrayManager::SelectMenu(): ")));
    UEtytl::DebugLog(FString::Printf(TEXT("UTrayManager::SelectMenu(): ")));
    UEtytl::DebugLog(FString::Printf(TEXT("UTrayManager::SelectMenu(): ")));
    UEtytl::DebugLog(FString::Printf(TEXT("UTrayManager::SelectMenu(): ")));
    UEtytl::DebugLog(FString::Printf(TEXT("UTrayManager::SelectMenu(): 选择[%d]"), MenuItemID));
    UEtytl::DebugLog(FString::Printf(TEXT("UTrayManager::SelectMenu(): 退出[%d]"), ID_Quit));
    UEtytl::DebugLog(FString::Printf(TEXT("UTrayManager::SelectMenu(): 设置[%d]"), ID_ToolsConfig));
    UEtytl::DebugLog(FString::Printf(TEXT("UTrayManager::SelectMenu(): 分辨率[%d]"), ID_SetScreenResolution));

    if (MenuItemID == ID_Quit) {
        UEtytl::DebugLog(FString::Printf(TEXT("UTrayManager::SelectMenu(): 退出程序!")));

        Tools_GameInstance->DestroyDialogWindow();
        RemoveTrayIcon();
        PostQuitMessage(0);
    }
    else if (MenuItemID == ID_ToolsConfig) {
        if (Tools_GameInstance) {
            Tools_GameInstance->DialogWindowMain->ShowWindow();
        }
    }
    else if (MenuItemID == ID_SetScreenResolution)
    {
        if (bSetScreenResolution)
        {
            USettingItem* SettingItem = SettingWidget->SettingItem;
            if (SettingItem) {
                int32 BeginResolutionWidth = FCString::Atoi(*SettingItem->BeginResolutionWidth);
                int32 BeginResolutionHeight = FCString::Atoi(*SettingItem->BeginResolutionHeight);
                Win::SetDisplaySize(BeginResolutionWidth, BeginResolutionHeight);
                bSetScreenResolution = false;

                UEtytl::DebugLog(FString::Printf(TEXT("UTrayManager::SelectMenu: 屏幕分辨率修改成功: [%d] x [%d]"), BeginResolutionWidth, BeginResolutionHeight));
            }
        }
        else
        {
            USettingItem* SettingItem = SettingWidget->SettingItem;
            if (SettingItem) {
                int32 TargetResolutionWidth = FCString::Atoi(*SettingItem->TargetResolutionWidth);
                int32 TargetResolutionHeight = FCString::Atoi(*SettingItem->TargetResolutionHeight);
                Win::SetDisplaySize(TargetResolutionWidth, TargetResolutionHeight);
                bSetScreenResolution = true;

                UEtytl::DebugLog(FString::Printf(TEXT("UTrayManager::SelectMenu: 屏幕分辨率修改成功: [%d] x [%d]"), TargetResolutionWidth, TargetResolutionHeight));
            }
        }
    }
    else {
        ToolsMain->WindowShell.ExeMenuItemShell(MenuItemID);
    }
}

void UTrayManager::ShowContextMenu(HWND _hWnd)
{
    // 获取鼠标位置
    POINT pt;
    GetCursorPos(&pt);
    // 显示菜单
    SetForegroundWindow(_hWnd);
    //选择的菜单ID
    SelectMenu(TrackPopupMenu(hMenu, TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, 0, _hWnd, NULL)); //获取用户选择的菜单项的标识符
}

LRESULT CALLBACK UTrayManager::WndProc(HWND _hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (Tools_GameInstance) {
        // 处理托盘图标事件
        if (message == WM_TRAYICON)
        {
            if (lParam == WM_RBUTTONDOWN)
            {
                // 显示右键菜单
                ShowContextMenu(_hWnd);
            }
            else if (lParam == WM_LBUTTONDOWN)
            {
                // 处理左键点击事件
                //FPlatformMisc::MessageBoxExt(EAppMsgType::Ok, TEXT("Tray Icon Clicked!"), TEXT("Info"));
            }
        }
    }
    // 调用默认窗口过程
    return DefWindowProc(_hWnd, message, wParam, lParam);
}