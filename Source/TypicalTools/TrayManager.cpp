// Fill out your copyright notice in the Description page of Project Settings.


#include "TrayManager.h"
#include "Engine/GameViewportClient.h"
#include "Framework/Application/SlateApplication.h"

#include "resource.h"
#include "Runtime/Launch/Resources/Windows/resource.h"

#include "TypicalTool/Public/Tools.h"



USettingWidget* UTrayManager::SettingWidget = nullptr;
UTools_GameInstance* UTrayManager::Tools_GameInstance = nullptr;
NOTIFYICONDATA UTrayManager::nid = {};
HWND UTrayManager::hWnd = nullptr;

int UTrayManager::ID_Quit = tytl::Win::WindowHost::GetHMENU(); //退出
int UTrayManager::ID_ToolsConfig = tytl::Win::WindowHost::GetHMENU(); //工具箱配置
int UTrayManager::ID_SetScreenResolution = tytl::Win::WindowHost::GetHMENU(); //修改屏幕分辨率

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

    if (Tools_GameInstance) {
        Tools_GameInstance->DialogWindowMain->HideWindow();
    }
}

void UTrayManager::RemoveTrayIcon()
{
    // 移除托盘图标
    Shell_NotifyIcon(NIM_DELETE, &nid);
    SettingWidget = nullptr;
    Tools_GameInstance = nullptr;
}

void UTrayManager::SelectMenu(int MenuItemID)
{
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
//    if (MenuItemID == ID_ToolsConfig)
//    {
//        WindowShell.ExecuteAnalyze(Tx("打开配置文件"), Tx("打开文件"), ConfigFile.GetConfigFilePath());
//        lgr(Tip, Tx("配置格式示例:\n\t[Shell命令名]\n\
//\t参数=/k ping -t www.baidu.com\n\t文件=cmd\n\t显示窗口=是 (是/否)\n\
//\t模式=打开文件 (打开文件/打开文件夹/管理员运行)\n\t菜单按键=是 (是/否: 托盘菜单中添加/程序启动时运行)"));
//        int Result = MessageBox(NULL, Tx("修改完成后, 点击[确定]\n即可更新托盘菜单的选项"), Tx("提示"), MB_OK | MB_ICONQUESTION);
//        if (Result == IDOK) { //用户确定修改完成
//            LoadBaseConfig(true);
//            LoadShellConfig();
//            LoadToolsMenu(hMenu);
//        }
//    }
//    else if (MenuItemID == ID_SetScreenResolution)
//    {
//        if (SetScreenResolution)
//        {
//            //SetScreenResolution
//            SetDisplaySize(
//                std::stoi(BaseConfigItem.find(Tx("默认的屏幕分辨率宽"))->second),
//                std::stoi(BaseConfigItem.find(Tx("默认的屏幕分辨率高"))->second)
//            );
//            SetScreenResolution = false;
//
//            lgc(Tip, Format(Tx("屏幕分辨率修改成功:\n[%] x [%]"),
//                BaseConfigItem.find(Tx("默认的屏幕分辨率宽"))->second, BaseConfigItem.find(Tx("默认的屏幕分辨率高"))->second));
//        }
//        else
//        {
//            //SetScreenResolution
//            SetDisplaySize(
//                std::stoi(BaseConfigItem.find(Tx("修改的屏幕分辨率宽"))->second),
//                std::stoi(BaseConfigItem.find(Tx("修改的屏幕分辨率高"))->second)
//            );
//            SetScreenResolution = true;
//
//            UEtytl::DebugLog(FString::Printf(TEXT("屏幕分辨率修改成功:\n[%] x [%]"),
//                BaseConfigItem.find(Tx("修改的屏幕分辨率宽"))->second, BaseConfigItem.find(Tx("修改的屏幕分辨率高"))->second));
//        }
//    }
//    else if (MenuItemID == ID_Quit)
//    {
//        PostQuitMessage(0);
//    }
//    else {
//        WindowShell.ExeMenuItemShell(MenuItemID);
//    }
}

void UTrayManager::ShowContextMenu(HWND _hWnd)
{
    // 创建右键菜单
    HMENU hMenu = CreatePopupMenu();
    //为Menu添加选项
    AppendMenu(hMenu, MF_STRING, ID_SetScreenResolution, TEXT("修改屏幕分辨率"));
    AppendMenu(hMenu, MF_SEPARATOR, NULL, TEXT("分割线"));
    AppendMenu(hMenu, MF_SEPARATOR, NULL, TEXT("分割线"));
    AppendMenu(hMenu, MF_STRING, ID_ToolsConfig, TEXT("设置"));
    AppendMenu(hMenu, MF_STRING, ID_Quit, TEXT("退出"));

    // 获取鼠标位置
    POINT pt;
    GetCursorPos(&pt);
    // 显示菜单
    SetForegroundWindow(_hWnd);
    //选择的菜单ID
    SelectMenu(TrackPopupMenu(hMenu, TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, 0, _hWnd, NULL));

    DestroyMenu(hMenu);
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