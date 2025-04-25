// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerTools.h"
#include "DialogWindow.h"
#include "SettingWidget.h"

#include <Shcore.h> //WindowDPI
#include "TypicalTool/Public/Tools.h"



void APlayerControllerTools::EndPlay(const EEndPlayReason::Type)
{
    UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::Shutdown: 游戏实例关闭开始!")));

    if (!DialogMainWindow.IsValid()) {
        UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::Shutdown: DialogMainWindow 无效!")), FColor::Red);
    }
    else {
        DialogMainWindow->RequestDestroyWindow();
        DialogMainWindow.Reset();
    }

    if (!MainWindow.IsValid()) {
        UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::Shutdown: MainWindow 无效!")), FColor::Red);
    }
    else {
        MainWindow.Reset();
    }

    if (!ToolsConfig.IsValid()) {
        UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::Shutdown: ToolsConfig 无效!")), FColor::Red);
    }
    else {
        ToolsConfig.Reset();
    }

    if (!TimerHandle.IsValid()) {
        UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::Shutdown: TimerHandle 无效!")), FColor::Red);
    }
    else {
        World->GetTimerManager().ClearTimer(TimerHandle);
    }

    UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::Shutdown: 游戏实例已关闭!")));
}

void APlayerControllerTools::BeginPlay()
{
    World = GetWorld();
    if (!IsValid(World)) {
        UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: World 无效!")), FColor::Red);
    }

    tytl::Win::WindowDPI();

    UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: ")));
    UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: ")));
    UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: ")));
    UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: ")));
    UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: ")));
    UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: ")));
    UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: ")));
    UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: ")));
    UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: ")));
    UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: ")));
    UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: 游戏开始!")));

    //设置窗口大小
    MainWindow = UEtytl::GetWindow();
    UEtytl::SetWindowResolution(MainWindowWidth, MainWindowHeight, Fullscreen);

    if (!MainWindow.IsValid()) {
        UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: MainWindow 无效!")), FColor::Red);
        return;
    }

    World->GetTimerManager().SetTimer(
        TimerHandle,
        this,
        &APlayerControllerTools::OnTimer,
        MainWindowHideInterval,  // 间隔时间（秒）
        true                     // 是否循环
    );

    //初始化程序数据
    if (!UEtytl::CreateDirectory(TEXT("Config"))) {
        UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: 文件夹创建失败/已创建!")), FColor::Red);
    }

    FString ErrorMessage;
    if (!UEtytl::ReadJsonFile(*FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Config/Config.json")), ToolsConfig, ErrorMessage)) {
        if (ErrorMessage.Equals(TEXT("文件错误"))) { //没有文件
            CreateConfigFile();
        }
        else if (ErrorMessage.Equals(TEXT("非文件错误"))) { //Json格式错误
            UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: Json格式错误!")));

            return;
        }
    }
    UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: 配置文件读取成功!")), FColor::Yellow);

    //创建对话框: 文件备份
    if (!IsValid(MainMenuWidget)) {
        UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: MainMenuWidget 无效!")), FColor::Red);
        return;
    }

    DialogMainWindow = CreateToolDialog(MainMenuWidget, TEXT("文件备份"), DialogWindowWidth, DialogWindowHeight);
    USettingWidget* temp = Cast<USettingWidget>(MainMenuWidget);
    if (!IsValid(temp)) {
        UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: MainMenuWidget 转换 USettingWidget后 无效!")), FColor::Red);
        return;
    }
    temp->InitializeListView();
}

void APlayerControllerTools::QuitGame()
{
    UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::QuitGame: 退出游戏!")), FColor::Red); 
    
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!IsValid(PlayerController)) {
        UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::QuitGame: PlayerController 无效!")), FColor::Red);
    }
    else {
        UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
    }
}

void APlayerControllerTools::OnTimer()
{
    if (!bShowMainWindow) {
        MainWindow->HideWindow();
    }
}

void APlayerControllerTools::CreateConfigFile()
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
            UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::CreateConfigFile: 写入文件[Config.json]错误!")), FColor::Red);
        }
        else if (ErrorMessage.Equals(TEXT("非文件错误"))) { //Json格式错误
            UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::CreateConfigFile: Json格式错误!")), FColor::Red);
        }
        return;
    }

    UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::CreateConfigFile: 文件创建成功!")), FColor::Yellow);
}

void APlayerControllerTools::SaveConfigFile()
{
    FString ErrorMessage;
    if (!UEtytl::WriteJsonFile(*FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Config/Config.json")), ToolsConfig, ErrorMessage)) {
        if (ErrorMessage.Equals(TEXT("文件错误"))) { //没有文件
            UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::CreateConfigFile: 写入文件[Config.json]错误!")), FColor::Red);
        }
        else if (ErrorMessage.Equals(TEXT("非文件错误"))) { //Json格式错误
            UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::CreateConfigFile: Json格式错误!")), FColor::Red);
        }

        return;
    }
    UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::CreateConfigFile: 文件创建成功!")), FColor::Yellow);
}
