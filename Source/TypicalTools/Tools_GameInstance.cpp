// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools_GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "TypicalTool/Public/Tools.h"
#include "libTypical/Tool.h"
#include "DialogWindow.h"


using tytl::WindowsSystem::WindowHost;
using tytl::WindowsSystem::WindowShell;

void UTools_GameInstance::OnStart()
{
    Super::OnStart();

    //tytl::WindowShowAndRender(false, 0); //隐藏窗口, 暂停渲染

    tytl::SetWindowResolution(200, 80, EWindowMode::ConvertIntToWindowMode(2));

    if (MainMenuWidget) {
        DialogWindowMain = CreateToolDialog(MainMenuWidget, DialogTitleName, WindowWidth, WindowHeight);
        // 设置窗口关闭时的回调
        //DialogWindowMain->SetOnWindowClosed(FOnWindowClosed::CreateLambda([&](const TSharedRef<SWindow>& Window)
        //    {
        //        UE_LOG(LogTemp, Warning, TEXT("DialogWindow: Window Closed!"));
        //        // 在这里处理窗口关闭后的逻辑

        //        tytl::ExitGame();
        //    })
        //);
    }
    else {
        tytl::DebugLog(FString::Printf(TEXT("UTools_GameInstance::OnStart(): 无效的 UMG!")));
    }
}

void UTools_GameInstance::DestroyDialogWindow()
{
    DestroyWindow(DialogWindowMain.ToSharedRef());
    tytl::ExitGame();
}
