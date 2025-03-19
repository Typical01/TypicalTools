// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SWindow.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"

/**
 * 
 */
class TYPICALTOOLS_API DialogWindow : public SCompoundWidget
{
public:
    DialogWindow();
    ~DialogWindow();

public:
    SLATE_BEGIN_ARGS(DialogWindow) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs)
    {
        ChildSlot
            [
                SNew(SVerticalBox)
                    + SVerticalBox::Slot()
                    .AutoHeight()
                    .Padding(10)
                    [
                        SNew(STextBlock)
                            .Text(FText::FromString("This is a modal dialog!"))
                    ]
                    + SVerticalBox::Slot()
                    .AutoHeight()
                    .Padding(10)
                    [
                        SNew(SButton)
                            .Text(FText::FromString("Close"))
                            .OnClicked(this, &DialogWindow::OnCloseButtonClicked)
                    ]
            ];
    }

private:
    FReply OnCloseButtonClicked()
    {
        // 关闭对话框
        if (DialogWindowMain.IsValid())
        {
            DialogWindowMain.Pin()->RequestDestroyWindow();
        }
        return FReply::Handled();
    }

    TWeakPtr<SWindow> DialogWindowMain;
public:
    void SetDialogWindow(TSharedRef<SWindow> InWindow)
    {
        DialogWindowMain = InWindow;
    }
};

TSharedRef<SWindow> CreateToolDialog(UUserWidget* _MainMenuWidget, FString _DialogTitleName, int32 _WindowWidth, int32 _WindowHeight);
void DestroyWindow(TSharedRef<SWindow> _DialogWindow);