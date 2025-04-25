// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogWindow.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/SWindow.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Input/SButton.h"
#include "Blueprint/UserWidget.h"



DialogWindow::DialogWindow()
{
}

DialogWindow::~DialogWindow()
{
}

TSharedRef<SWindow> CreateToolDialog(UUserWidget* _MainMenuWidget, FString _DialogTitleName, int32 _WindowWidth, int32 _WindowHeight)
{
    //// 创建自定义的窗口样式
    //FWindowStyle CustomWindowStyle = FCoreStyle::Get().GetWidgetStyle<FWindowStyle>("Window");
    //// 创建自定义的字体信息
    //FSlateFontInfo CustomFontInfo = FCoreStyle::Get().GetFontStyle("NormalFont");
    //CustomFontInfo.Size = 16; // 设置所需的字体大小

    //// 应用自定义的字体信息到标题文本样式
    //FTextBlockStyle CustomTitleTextStyle = CustomWindowStyle.TitleTextStyle;
    //CustomTitleTextStyle.SetFont(CustomFontInfo);
    //CustomWindowStyle.SetTitleTextStyle(CustomTitleTextStyle); 

    // 创建独立对话框窗口
    TSharedRef<SWindow> DialogWindow = SNew(SWindow)
        .Title(FText::FromString(_DialogTitleName))
        .ClientSize(FVector2D(_WindowWidth + 2, _WindowHeight + 2))
        .SupportsMaximize(false)
        .SupportsMinimize(false)
        .SizingRule(ESizingRule::FixedSize) // 固定窗口大小
        .HasCloseButton(false);              // 禁用关闭按钮
        //.Style(&CustomWindowStyle);         // 应用自定义样式
        
    TSharedPtr<SWidget> SlateWidget = _MainMenuWidget->TakeWidget();
    // 将UserWidget添加到Slate布局中
    TSharedRef<SBox> DialogContent = SNew(SBox)
        //.WidthOverride(_WindowWidth)  // 设置固定宽度
        //.HeightOverride(_WindowHeight) // 设置固定高度
        //.Padding(FMargin(2))
        [
            SNew(SVerticalBox)
                + SVerticalBox::Slot()
                .HAlign(HAlign_Center) // 水平居中对齐
                [
                    SlateWidget.ToSharedRef()
                ]
        ];

    DialogWindow->SetContent(DialogContent);
    // 添加对话框窗口到 Slate 应用程序
    FSlateApplication::Get().AddWindow(DialogWindow);

    //聚焦
    FSlateApplication::Get().SetKeyboardFocus(DialogWindow, EFocusCause::SetDirectly);
    FSlateApplication::Get().SetUserFocus(0, DialogWindow, EFocusCause::SetDirectly);

    return DialogWindow;
}

void DestroyWindow(TSharedRef<SWindow> _DialogWindow)
{
    _DialogWindow.Get().RequestDestroyWindow();
    //// 获取主窗口
    //TSharedPtr<SWindow> MainWindow = FSlateApplication::Get().GetActiveTopLevelWindow();
    //// 关闭主窗口
    //if (MainWindow.IsValid())
    //{
    //    MainWindow->RequestDestroyWindow();
    //}
}
