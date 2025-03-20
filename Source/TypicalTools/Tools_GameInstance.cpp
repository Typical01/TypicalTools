// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools_GameInstance.h"
#include "DialogWindow.h"
#include "TrayManager.h"
#include "SettingWidget.h"
#include "TrayManager.h"



UTools_GameInstance::~UTools_GameInstance()
{
}

void UTools_GameInstance::OnStart()
{
    Super::OnStart();

    //UEtytl::WindowShowAndRender(false, 0); //隐藏窗口, 暂停渲染

    UEtytl::SetWindowResolution(200, 80, EWindowMode::ConvertIntToWindowMode(2));

    if (MainMenuWidget) {
        DialogWindowMain = CreateToolDialog(MainMenuWidget, DialogTitleName, WindowWidth, WindowHeight);
        USettingWidget* SettingWidget = Cast<USettingWidget>(MainMenuWidget);
        SettingWidget->Tools_GameInstance = this;
    }
    else {
        UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::OnStart(): 无效的 UMG!")));
    }


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
    }

    CreateTrayIcon();
}


void UTools_GameInstance::HideDialogWindow()
{
    if (DialogWindowMain) {
        DialogWindowMain->HideWindow();
    }
}

void UTools_GameInstance::DestroyDialogWindow()
{
    if (DialogWindowMain) {
        DestroyWindow(DialogWindowMain.ToSharedRef());
    }
    // 重置 Slate 指针以释放资源
    if (DialogWindowMain.IsValid())
    {
        DialogWindowMain.Reset();
    }

    MainMenuWidget = nullptr;
    if (TrayManager) {
        TrayManager->RemoveTrayIcon();
    }
    TrayManager = nullptr;

    UEtytl::ExitGame(GetWorld());
}

void UTools_GameInstance::CreateConfigFile()
{
    ToolsConfig = MakeShareable(new FJsonObject());

    //创建对象: 基本设置
    TSharedPtr<FJsonObject> ItemObject_BaseSetting = MakeShareable(new FJsonObject());
    ItemObject_BaseSetting->SetBoolField(TEXT("注册表开机自启动"), false);
    ItemObject_BaseSetting->SetNumberField(TEXT("初始屏幕分辨率宽"), 1920);
    ItemObject_BaseSetting->SetNumberField(TEXT("初始屏幕分辨率高"), 1080);
    ItemObject_BaseSetting->SetNumberField(TEXT("目标屏幕分辨率宽"), 1280);
    ItemObject_BaseSetting->SetNumberField(TEXT("目标屏幕分辨率高"), 960);

    //创建对象: 笔记本键盘[关]
    TSharedPtr<FJsonObject> ItemObject_NoteBookKey_Off = MakeShareable(new FJsonObject());
    ItemObject_NoteBookKey_Off->SetStringField(TEXT("文件"), TEXT("cmd"));
    ItemObject_NoteBookKey_Off->SetStringField(TEXT("参数"), TEXT("/k sc config i8042prt start= disabled"));
    ItemObject_NoteBookKey_Off->SetStringField(TEXT("模式"), TEXT("管理员运行"));
    ItemObject_NoteBookKey_Off->SetBoolField(TEXT("显示窗口"), true);
    ItemObject_NoteBookKey_Off->SetBoolField(TEXT("菜单按键"), true);
    //创建对象: 笔记本键盘[开]
    TSharedPtr<FJsonObject> ItemObject_NoteBookKey_On = MakeShareable(new FJsonObject());
    ItemObject_NoteBookKey_On->SetStringField(TEXT("文件"), TEXT("cmd"));
    ItemObject_NoteBookKey_On->SetStringField(TEXT("参数"), TEXT("/k sc config i8042prt start= auto"));
    ItemObject_NoteBookKey_On->SetStringField(TEXT("模式"), TEXT("管理员运行"));
    ItemObject_NoteBookKey_On->SetBoolField(TEXT("显示窗口"), true);
    ItemObject_NoteBookKey_On->SetBoolField(TEXT("菜单按键"), true);

    //添加对象到 ToolsConfig: 基本设置, 其他
    ToolsConfig->SetObjectField(TEXT("基本设置"), ItemObject_BaseSetting);
    ToolsConfig->SetObjectField(TEXT("笔记本键盘[关]"), ItemObject_NoteBookKey_Off);
    ToolsConfig->SetObjectField(TEXT("笔记本键盘[开]"), ItemObject_NoteBookKey_On);

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


void UTools_GameInstance::CreateTrayIcon()
{
    TrayManager = NewObject<UTrayManager>();
    TrayManager->SettingWidget = Cast<USettingWidget>(MainMenuWidget);
    TrayManager->Tools_GameInstance = this;

    TrayManager->CreateTrayIcon();
    UEtytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::CreateTrayIcon(): CreateTrayIcon完成!")));

    // 获取窗口句柄
//#if IS_EDITOR
//    // 仅在编辑器模式下执行的代码
//    TSharedPtr<SWindow> EditorWindow = GEditor->GetEditorWorldContext().World()->GetGameViewport()->GetWindow();
//    if (EditorWindow.IsValid()) {
//        hWnd = (HWND)EditorWindow->GetNativeWindow()->GetOSWindowHandle();
//    }
//    else {
//
//    }
//#else
//    UGameViewportClient* GameViewportClient = GEngine->GameViewport;
//    if (GameViewportClient && GameViewportClient->GetWindow().IsValid()) {
//        TSharedPtr<SWindow> Window = GameViewportClient->GetWindow();
//        if (Window.IsValid()) {
//            TSharedPtr<FGenericWindow> GenericWindow = Window->GetNativeWindow();
//            if (GenericWindow.IsValid()) {
//                void* WindowHandle = GenericWindow->GetOSWindowHandle();
//                // 将 WindowHandle 转换为 HWND（仅在 Windows 平台上适用）
//                hWnd = (HWND)WindowHandle;
//            }
//        }
//    }
//#endif

    ////定义托盘图标数据结构
    //FMemory::Memzero(&nid, sizeof(nid));
    //nid.cbSize = sizeof(nid);
    //nid.hWnd = hWnd;
    //nid.uID = 1;
    //nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    //nid.uCallbackMessage = WM_USER + 1;
    //nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    //FCString::Strcpy(nid.szTip, TEXT("Typical01 Tools v2.0"));

    ////添加托盘图标
    //Shell_NotifyIcon(NIM_ADD, &nid);

    //// 替换窗口消息处理函数
    //SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
}