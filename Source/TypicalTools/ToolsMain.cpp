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
            UEtytl::DebugLog(Tx("UToolsMain::SelfStarting(): 注册开机自启动[否]"));

            if (Win::SetSelfStarting((Tstr)Tx("Typical_Tools"), Format(Tx("\"%\""), ExePathName).str(), false)) {
                lgc(Tx("注册开机自启动: 删除成功!"), Tip);
            }
            else {
                lgcr(Tx("注册开机自启动: 删除失败!"), Err);
            }
        }
        else {
            UEtytl::DebugLog(Tx("UToolsMain::SelfStarting(): 注册开机自启动[是]"));
            if (Win::SetSelfStarting((Tstr)Tx("Typical_Tools"), Format(Tx("\"%\""), ExePathName).str(), true)) {
                UEtytl::DebugLog(Tx("UToolsMain::SelfStarting(): 注册开机自启动: 添加成功"));
            }
            else {
                UEtytl::DebugLog(Tx("UToolsMain::SelfStarting(): 注册开机自启动: 添加失败"));
            }
        }
    }
}

void UToolsMain::LoadShellConfig()
{
    if (!ShellConfigVec.empty()) { //修改配置后
        UEtytl::DebugLog(Tx("UToolsMain::LoadShellConfig(): ReLoad"));
        ShellConfigVec.clear(); //清空Shell配置
        WindowShell.Clear(); //清空 程序启动项/菜单项
    }
    else {
        UEtytl::DebugLog(Tx("UToolsMain::LoadShellConfig(): Load"));
    }

    if (SettingWidget) {
        for (auto tempShellConfigItem : SettingWidget->ListViewShellConfig->GetListItems()) {
            auto* ShellConfigItem = Cast<UShellConfigItem>(tempShellConfigItem);

            Tstr Config = ShellConfigItem->ItemName.GetCharArray().GetData();

            //执行Shell配置
            if (Config != Tx("基本设置")) {
                UEtytl::DebugLog(Tx("UToolsMain::LoadShellConfig(): Load"));
                lgc(Format(Tx("ShellConfig: [%]"), Config));

                Tstr OperateMode;
                Tstr File;
                Tstr Args;
                Tstr ShowWindow;
                Tstr MenuButton;

                //读取配置信息
                OperateMode = ShellConfigItem->Mode.GetCharArray().GetData();
                //lgc(Format(Tx("  操作模式: [%]"), ShellConfigItem->Mode));

                File = ShellConfigItem->File.GetCharArray().GetData();
                //lgc(Format(Tx("  文件: %"), File));

                Args = ShellConfigItem->Argument.GetCharArray().GetData();
                //lgc(Format(Tx("  参数: %"), Args));

                ShowWindow = ShellConfigItem->ShowWindow ? TEXT("是") : TEXT("否");
                //lgc(Format(Tx("  显示窗口: [%]"), ShowWindow));

                MenuButton = ShellConfigItem->MenuButton ? TEXT("是") : TEXT("否");
                //lgc(Format(Tx("  菜单按键: [%]"), MenuButton));

                ShellConfigVec.push_back(Win::ShellConfig(Config, OperateMode, File, Args, ShowWindow, MenuButton));
            }
            else {
                lgc(Format(Tx("OtherConfig: [%]"), Config));
            }
        }
    }
}

void UToolsMain::LoadToolsMenu(HMENU Menu)
{
    int MenuItemCount = GetMenuItemCount(Menu);
    if (MenuItemCount != 0) { //已有选项时: 重新加载配置
        lgc(Tip, Tx("LoadToolsMenu() ReLoad"));
        for (int i = MenuItemCount - 1; i >= 0; i--) { //向前遍历: 避免删除后继续使用改动后的数组而越界
            DeleteMenu(Menu, i, MF_BYPOSITION);
        }
    }
    else {
        lgc(Tx("LoadToolsMenu()"));
    }

    //为Menu添加选项  

    //Shell插入位置: Menu项总数 - 4
    WindowShell.ShellOperate(Menu, ShellConfigVec);
    WindowShell.ExeRunItemShell();
}