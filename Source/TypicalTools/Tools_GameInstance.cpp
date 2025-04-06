// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools_GameInstance.h"
#include "DialogWindow.h"
#include "SettingWidget.h"



UTools_GameInstance::~UTools_GameInstance()
{
    UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::~UTools_GameInstance: 析构!")), FColor::Red);
}

void UTools_GameInstance::Shutdown()
{
    Super::Shutdown();

    if (DialogMainWindow.IsValid())
    {
        DialogMainWindow->RequestDestroyWindow();
        DialogMainWindow.Reset();
    }

    UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::Shutdown: 游戏实例关闭, 窗口已销毁.")), FColor::Red);
}

void UTools_GameInstance::OnStart()
{
    Super::OnStart();

    //设置窗口大小
    UEtytl::SetWindowResolution(MainWindowWidth, MainWindowHeight, Fullscreen);
    MainWindow = UEtytl::GetWindow();

    if (MainWindow.IsValid()) {
        // 获取世界上下文
        UWorld* World = GetWorld();
        if (World)
        {
            World->GetTimerManager().SetTimer(
                TimerHandle,
                this,
                &UTools_GameInstance::OnTimer,
                MainWindowHideInterval,  // 间隔时间（秒）
                true    // 是否循环
            );
        }
    }
    
    //初始化程序数据
    if (UEtytl::CreateDirectorys(TEXT("Config"))) {
        FString ErrorMessage;
        if (!UEtytl::ReadJsonFile(*FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Config/Config.json")), ToolsConfig, ErrorMessage)) {
            if (ErrorMessage.Equals(TEXT("文件错误"))) { //没有文件
                CreateConfigFile();
            }
            else if (ErrorMessage.Equals(TEXT("非文件错误"))) { //Json格式错误
                UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::UTools_GameInstance(): Json格式错误!")));
            }
        }
        else {
            UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::UTools_GameInstance(): 配置文件读取成功!")), FColor::Yellow);
        }
    }

    //创建对话框: 文件备份
    DialogMainWindow = CreateToolDialog(MainMenuWidget, TEXT("文件备份"), DialogWindowWidth, DialogWindowHeight);
}

void UTools_GameInstance::ExitGame()
{
    UEtytl::ExitGame(GetWorld());
}

void UTools_GameInstance::OnTimer()
{
    MainWindow->HideWindow();
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
            UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::CreateConfigFile(): 写入文件[Config.json]错误!")));
        }
        else if (ErrorMessage.Equals(TEXT("非文件错误"))) { //Json格式错误
            UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::CreateConfigFile(): Json格式错误!")));
        }
    }
    else {
        UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::CreateConfigFile(): 文件创建成功!")), FColor::Yellow);
    }
}

void UTools_GameInstance::SaveConfigFile()
{
    FString ErrorMessage;
    if (!UEtytl::WriteJsonFile(*FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Config/Config.json")), ToolsConfig, ErrorMessage)) {
        if (ErrorMessage.Equals(TEXT("文件错误"))) { //没有文件
            UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::CreateConfigFile(): 写入文件[Config.json]错误!")));
        }
        else if (ErrorMessage.Equals(TEXT("非文件错误"))) { //Json格式错误
            UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::CreateConfigFile(): Json格式错误!")));
        }
    }
    else {
        UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::CreateConfigFile(): 文件创建成功!")), FColor::Yellow);
    }
}
