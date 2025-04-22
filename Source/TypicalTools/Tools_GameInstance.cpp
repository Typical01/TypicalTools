// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools_GameInstance.h"
#include "DialogWindow.h"
#include "SettingWidget.h"

#include <Shcore.h> //WindowDPI
#include "TypicalTool/Public/Tools.h"



void UTools_GameInstance::Shutdown()
{
    UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::Shutdown: 游戏实例关闭开始!")));

    if (!DialogMainWindow.IsValid()) {
        UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::Shutdown: DialogMainWindow 无效!")), FColor::Red);
    }
    else {
        DialogMainWindow->RequestDestroyWindow();
        DialogMainWindow.Reset();
    }

    if (!MainWindow.IsValid()) {
        UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::Shutdown: MainWindow 无效!")), FColor::Red);
    }
    else {
        MainWindow.Reset();
    }

    if (!ToolsConfig.IsValid()) {
        UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::Shutdown: ToolsConfig 无效!")), FColor::Red);
    }
    else {
        ToolsConfig.Reset();
    }

    if (!TimerHandle.IsValid()) {
        UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::Shutdown: TimerHandle 无效!")), FColor::Red);
    }
    else {
        this->GetTimerManager().ClearTimer(TimerHandle);
    }

    UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::Shutdown: 游戏实例已关闭!")));
    Super::Shutdown();
}

void UTools_GameInstance::OnStart()
{
    Super::OnStart();

    tytl::Win::WindowDPI();

    UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::OnStart: ")));
    UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::OnStart: ")));
    UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::OnStart: ")));
    UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::OnStart: ")));
    UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::OnStart: ")));
    UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::OnStart: ")));
    UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::OnStart: ")));
    UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::OnStart: ")));
    UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::OnStart: ")));
    UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::OnStart: ")));
    UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::OnStart: 游戏开始!")));

    //设置窗口大小
    MainWindow = UEtytl::GetWindow();
    UEtytl::SetWindowResolution(MainWindowWidth, MainWindowHeight, Fullscreen);

    if (!MainWindow.IsValid()) {
        UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::OnStart: MainWindow 无效!")));
        return;
    }

    this->GetTimerManager().SetTimer(
        TimerHandle,
        this,
        &UTools_GameInstance::OnTimer,
        MainWindowHideInterval,  // 间隔时间（秒）
        true                     // 是否循环
    );

    //初始化程序数据
    if (!UEtytl::CreateDirectory(TEXT("Config"))) {
        UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::OnStart: 文件夹创建失败/已创建!")), FColor::Red);
    }

    FString ErrorMessage;
    if (!UEtytl::ReadJsonFile(*FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Config/Config.json")), ToolsConfig, ErrorMessage)) {
        if (ErrorMessage.Equals(TEXT("文件错误"))) { //没有文件
            CreateConfigFile();
        }
        else if (ErrorMessage.Equals(TEXT("非文件错误"))) { //Json格式错误
            UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::OnStart: Json格式错误!")));
            
            return;
        }
    }
    UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::OnStart: 配置文件读取成功!")), FColor::Yellow);

    //创建对话框: 文件备份
    DialogMainWindow = CreateToolDialog(MainMenuWidget, TEXT("文件备份"), DialogWindowWidth, DialogWindowHeight);
    USettingWidget* temp = Cast<USettingWidget>(MainMenuWidget);
    if (!IsValid(temp)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::OnStart: MainMenuWidget 转换 USettingWidget后 无效!")));
        return;
    }
    temp->InitializeListView();
}

void UTools_GameInstance::QuitGame()
{
    UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::QuitGame: 退出游戏!")), FColor::Red);
    UKismetSystemLibrary::QuitGame(GetWorld(), GetFirstLocalPlayerController(), EQuitPreference::Quit, false);
}

void UTools_GameInstance::OnTimer()
{
    if (!bShowMainWindow) {
        MainWindow->HideWindow();
    }
}

void UTools_GameInstance::CreateConfigFile()
{
    ToolsConfig = MakeShareable(new FJsonObject());

    //创建对象: 基本设置
    TSharedPtr<FJsonObject> ItemObject_BaseSetting = MakeShareable(new FJsonObject());
    ItemObject_BaseSetting->SetBoolField(TEXT("注册表开机自启动"), false);

    //添加对象到 ToolsConfig: 基本设置, 其他
    ToolsConfig->SetObjectField(TEXT("基本设置"), ItemObject_BaseSetting);

    FString ErrorMessage;
    if (!UEtytl::WriteJsonFile(*FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Config/Config.json")), ToolsConfig, ErrorMessage)) {
        if (ErrorMessage.Equals(TEXT("文件错误"))) { //没有文件
            UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::CreateConfigFile: 写入文件[Config.json]错误!")));
        }
        else if (ErrorMessage.Equals(TEXT("非文件错误"))) { //Json格式错误
            UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::CreateConfigFile: Json格式错误!")));
        }
        return;
    }

    UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::CreateConfigFile: 文件创建成功!")), FColor::Yellow);
}

void UTools_GameInstance::SaveConfigFile()
{
    FString ErrorMessage;
    if (!UEtytl::WriteJsonFile(*FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Config/Config.json")), ToolsConfig, ErrorMessage)) {
        if (ErrorMessage.Equals(TEXT("文件错误"))) { //没有文件
            UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::CreateConfigFile: 写入文件[Config.json]错误!")));
        }
        else if (ErrorMessage.Equals(TEXT("非文件错误"))) { //Json格式错误
            UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::CreateConfigFile: Json格式错误!")));
        }

        return;
    }
    UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::CreateConfigFile: 文件创建成功!")), FColor::Yellow);
}
