// Fill out your copyright notice in the Description page of Project Settings.


#include "ToolsMain.h"

#include "SettingWidget.h"
#include "ShellConfigItem.h"


using namespace tytl;



void UToolsMain::SelfStarting(bool bAutoStarting)
{
    Tstr ExePathName;
    if (Win::GetExePathName(ExePathName)) {
        if (!bAutoStarting) {
            UEtytl::DebugLog(TEXT("UToolsMain::SelfStarting(): 注册开机自启动[否]"));

            if (Win::SetSelfStarting((Tstr)TEXT("TypicalTools"), Format(TEXT("\"%\""), ExePathName).str(), false)) {
                lgc(TEXT("注册开机自启动: 删除成功!"), Tip);
            }
            else {
                lgcr(TEXT("注册开机自启动: 删除失败!"), Err);
            }
        }
        else {
            UEtytl::DebugLog(TEXT("UToolsMain::SelfStarting(): 注册开机自启动[是]"));
            if (Win::SetSelfStarting((Tstr)TEXT("TypicalTools"), Format(TEXT("\"%\""), ExePathName).str(), true)) {
                UEtytl::DebugLog(TEXT("UToolsMain::SelfStarting(): 注册开机自启动: 添加成功"));
            }
            else {
                UEtytl::DebugLog(TEXT("UToolsMain::SelfStarting(): 注册开机自启动: 添加失败"));
            }
        }
    }
}

void UToolsMain::LoadShellConfig()
{
    if (!ShellConfigVec.empty()) { //修改配置后
        UEtytl::DebugLog(TEXT("UToolsMain::LoadShellConfig(): ReLoad"));
        ShellConfigVec.clear(); //清空Shell配置
        WindowShell.Clear(); //清空 程序启动项/菜单项
    }
    else {
        UEtytl::DebugLog(TEXT("UToolsMain::LoadShellConfig(): Load"));
    }

    if (SettingWidget) {
        for (auto tempShellConfigItem : SettingWidget->ListViewShellConfig->GetListItems()) {
            auto* ShellConfigItem = Cast<UShellConfigItem>(tempShellConfigItem);

            //执行Shell配置
            Tstr Config(*ShellConfigItem->ItemName);
            if (Config != TEXT("基本设置")) {
                UEtytl::DebugLog(TEXT("UToolsMain::LoadShellConfig(): Other Config Load"));
                UEtytl::DebugLog(FString::Printf(TEXT("UToolsMain::LoadShellConfig(): Shell配置: [%s]"), Config.c_str()));

                Tstr OperateMode(*ShellConfigItem->Mode);
                Tstr File(*ShellConfigItem->File);
                Tstr Args(*ShellConfigItem->Argument);
                Tstr ShowWindow = ShellConfigItem->ShowWindow ? TEXT("是") : TEXT("否");
                Tstr MenuButton = ShellConfigItem->MenuButton ? TEXT("是") : TEXT("否");

                //读取配置信息
                UEtytl::DebugLog(FString::Printf(TEXT("UToolsMain::LoadShellConfig(): 操作模式: [%s]"), OperateMode.c_str()));
                UEtytl::DebugLog(FString::Printf(TEXT("UToolsMain::LoadShellConfig(): 文件: [%s]"), File.c_str()));
                UEtytl::DebugLog(FString::Printf(TEXT("UToolsMain::LoadShellConfig(): 参数: [%s]"), Args.c_str()));
                UEtytl::DebugLog(FString::Printf(TEXT("UToolsMain::LoadShellConfig(): 显示窗口: [%s]"), ShowWindow.c_str()));
                UEtytl::DebugLog(FString::Printf(TEXT("UToolsMain::LoadShellConfig(): 菜单按键: [%s]"), MenuButton.c_str()));
                
                ShellConfigVec.push_back(Win::ShellConfig(Config, OperateMode, File, Args, ShowWindow, MenuButton));
            }
            else {
                UEtytl::DebugLog(TEXT("UToolsMain::LoadShellConfig(): 基本设置"));
            }
        }
    }
}

void UToolsMain::LoadToolsMenu()
{
    int MenuItemCount = GetMenuItemCount(hMenu);
    if (MenuItemCount > 0) { //已有选项时: 重新加载配置
        lgc(Tip, TEXT("LoadToolsMenu() ReLoad"));
        for (int i = MenuItemCount - 1; i >= 0; i--) { //向前遍历: 避免删除后继续使用改动后的数组而越界
            DeleteMenu(hMenu, i, MF_BYPOSITION);
        }
    }
    else {
        lgc(TEXT("LoadToolsMenu()"));
    }

    //为Menu添加选项

    //Shell插入位置
    WindowShell.ShellOperate(hMenu, ShellConfigVec);
    AppendMenu(hMenu, MF_SEPARATOR, NULL, TEXT("分割线"));
    AppendMenu(hMenu, MF_STRING, ID_SetScreenResolution, TEXT("修改屏幕分辨率"));
    AppendMenu(hMenu, MF_SEPARATOR, NULL, TEXT("分割线"));
    AppendMenu(hMenu, MF_STRING, ID_ToolsConfig, TEXT("设置"));
    AppendMenu(hMenu, MF_STRING, ID_Quit, TEXT("退出"));
}