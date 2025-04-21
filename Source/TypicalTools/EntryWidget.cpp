// Fill out your copyright notice in the Description page of Project Settings.


#include "EntryWidget.h"
#include "TypicalTool/Public/Tools.h"


void UEntryWidget::NativeOnListItemObjectSet(UObject* ItemObject)
{
    UShellConfigItem* DataObject = Cast<UShellConfigItem>(ItemObject);
    if (!IsValid(DataObject)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UEntryWidget::NativeOnListItemObjectSet: DataObject 无效!")), FColor::Red);
        return;
    }
    ShellConfigItem = DataObject;

    if (!IsValid(DataObject)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UEntryWidget::Init: 初始化 EntryWidget 失败!")), FColor::Red);
        return;
    }

    ShellConfigItem->EntryWidget = this;

    if (!IsValid(EditableTextOperationName)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UEntryWidget::Init: EditableTextOperationName 无效!")), FColor::Red);
        return;
    }
    EditableTextOperationName->SetText(FText::FromString(ShellConfigItem->OperationName));

    if (!IsValid(CheckBoxSetPermission)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UEntryWidget::Init: CheckBoxSetPermission 无效!")), FColor::Red);
        return;
    }
    CheckBoxSetPermission->SetIsChecked(ShellConfigItem->bSetPermissions);

    if (!IsValid(CheckBoxStartBackup)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UEntryWidget::Init: CheckBoxStartBackup 无效!")), FColor::Red);
        return;
    }
    CheckBoxStartBackup->SetIsChecked(ShellConfigItem->bStartBackup);

    if (!IsValid(MultiLineEditableTextSourceItem)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UEntryWidget::Init: MultiLineEditableTextSourceItem 无效!")), FColor::Red);
        return;
    }
    MultiLineEditableTextSourceItem->SetText(FText::FromString(ShellConfigItem->SourceFile));

    if (!IsValid(MultiLineEditableTextDestinationPath)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UEntryWidget::Init: MultiLineEditableTextDestinationPath 无效!")), FColor::Red);
        return;
    }
    MultiLineEditableTextDestinationPath->SetText(FText::FromString(ShellConfigItem->DestinationPath));
}

void UEntryWidget::OnBackupButton()
{
    if (!IsValid(SettingWidget)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UEntryWidget::OnBackupButton: SettingWidget 无效!")), FColor::Red);
        return;
    }
    if (!IsValid(ShellConfigItem)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UEntryWidget::OnBackupButton: ShellConfigItem 无效!")), FColor::Red);
        return;
    }
    if (!IsValid(ButtonBackup)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UEntryWidget::OnBackupButton: ButtonBackup 无效!")), FColor::Red);
        return;
    }

    if (!ShellConfigItem->bEntryButtonIsEnabled) { //正在备份中, 拒绝二次备份的输入
        UEtytl::DebugLog(FString::Printf(TEXT("UEntryWidget::OnBackupButton: 上一次备份未完成!")), FColor::Red);

        UTextTips* ErrorPathTips = NewObject<UTextTips>();
        ErrorPathTips->Text = FString::Printf(TEXT("上一次备份未完成[%s]"), *ShellConfigItem->OperationName);
        SettingWidget->ListViewErrorPathTipsArray.Add(ErrorPathTips);
        SettingWidget->OnErrorMessageTips();

        return;
    } 
    ShellConfigItem->bEntryButtonIsEnabled = false;

    OnBackupButtonIsEnabled(false); //等待备份完成

    SetProgress(.3f);
    SettingWidget->StartBackup(ShellConfigItem);

    SetProgress(1.f);
}

void UEntryWidget::OnBackupButtonIsEnabled(bool _bIsEnabled)
{
    //if (!IsValid(ButtonBackup)) {
    //    UEtytl::DebugLog(FString::Printf(TEXT("UEntryWidget::OnBackupButton: ButtonBackup 无效!")), FColor::Red);
    //    return;
    //}

    //AsyncTask(ENamedThreads::GameThread, [this, _bIsEnabled]()
    //    {
    //        ButtonBackup->SetIsEnabled(_bIsEnabled); //备份完成
    //    });
}

void UEntryWidget::SetProgress(float Progress)
{
    //if (!IsValid(ProgressBarItem)) {
    //    UEtytl::DebugLog(FString::Printf(TEXT("UEntryWidget::SetProgress: ProgressBarItem 无效!")), FColor::Red);
    //    return;
    //}
    //if (!IsValid(HorizontalBoxProgressBar)) {
    //    UEtytl::DebugLog(FString::Printf(TEXT("UEntryWidget::SetProgress: HorizontalBoxProgressBar 无效!")), FColor::Red);
    //    return;
    //}

    //AsyncTask(ENamedThreads::GameThread, [this, Progress]()
    //    {
    //        HorizontalBoxProgressBar->SetVisibility(ESlateVisibility::Visible); //显示
    //        float tempProgress = FMath::Clamp(Progress, 0.0f, 1.0f); //确保范围: 0-1 float
    //        ProgressBarItem->SetPercent(tempProgress);
    //        UEtytl::DebugLog(FString::Printf(TEXT("UEntryWidget::SetProgress: Progress[%f]"), tempProgress), FColor::Green);

    //        //满进度
    //        if (tempProgress >= 1.f) { //进度条清空
    //            ProgressBarItem->SetPercent(0.f);
    //            HorizontalBoxProgressBar->SetVisibility(ESlateVisibility::Collapsed); //隐藏
    //        }
    //        else {
    //            HorizontalBoxProgressBar->SetVisibility(ESlateVisibility::Visible); //显示
    //        }
    //    });
}