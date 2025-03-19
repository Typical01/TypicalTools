// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingWidget.h"
#include "TypicalTool/Public/Tools.h"

void USettingWidget::InitializeLiListView()
{
    //从配置文件加载 ShellConfigItem

    //如果有, ListViewShellConfigItem 默认为第一条 ShellConfig
}

int32 USettingWidget::FindListViewItem(UObject* _Item)
{
    if (ListViewShellConfig) {
        return ListViewShellConfig->GetListItems().Find(_Item);
    }
    else {
        return int32(INDEX_NONE);
    }
}

void USettingWidget::DeleteListViewItem(UObject* _Item)
{
    if (ListViewShellConfig) {
        ListViewShellConfig->RemoveItem(_Item);
    }
    else {
        tytl::DebugLog(FString::Printf(TEXT("USettingWidget::DeleteListViewItem: ListViewItem 删除失败!")));
    }
}

void USettingWidget::OnApplyButton()
{
    OutputLog();
}

void USettingWidget::OnDestroyListViewWidget(UObject* _Item)
{
    if (ListViewShellConfig) {
        UEntryWidget* tempEntry = Cast<UEntryWidget>(ListViewShellConfig->GetEntryWidgetFromItem(_Item));
        ListViewShellConfig->RemoveItem(_Item);
        //tempEntry = nullptr;

        //for (UObject* Item : ListViewShellConfig->GetListItems())
        //{
        //    // 如果数据项无效，则移除并销毁条目控件
        //    if (!Item)
        //    {
        //        UEntryWidget* DeleteEntry = Cast<UEntryWidget>(ListViewShellConfig->GetEntryWidgetFromItem(_Item));
        //        DeleteEntry->RemoveFromParent();
        //        DeleteEntry->ConditionalBeginDestroy();
        //        ListViewShellConfig->RegenerateAllEntries();
        //    }
        //}
        //tempEntry->RemoveFromParent();
    }
}

void USettingWidget::OutputLog()
{
    int32 tempCount = 0;

    tytl::DebugLog(FString::Printf(TEXT(" ")), FColor::Green);
    tytl::DebugLog(FString::Printf(TEXT(" ")));
    tytl::DebugLog(FString::Printf(TEXT(" ")));
    tytl::DebugLog(FString::Printf(TEXT(" ")));
    tytl::DebugLog(FString::Printf(TEXT(" ")));
    tytl::DebugLog(FString::Printf(TEXT(" ")));
    tytl::DebugLog(FString::Printf(TEXT("USettingWidget::OutputLog: bAutoStarting[%d]"), bAutoStarting));
    if (ListViewShellConfig) {
        tytl::DebugLog(FString::Printf(TEXT("USettingWidget::OutputLog: 列表视图ShellConfigItems")), FColor::Yellow);
        for (auto& ListItem : ListViewShellConfig->GetListItems()) {
            tempCount++;
            UShellConfigItem* tempShellConfigItem = Cast<UShellConfigItem>(ListItem);
            if (tempShellConfigItem) {
                tytl::DebugLog(FString::Printf(TEXT("USettingWidget::OutputLog: ListItem[%d]"), tempCount), FColor::Yellow);
                tempShellConfigItem->OutputLog();
            }
        }
    }
    tempCount = 0;

    if (SettingItem) {
        tytl::DebugLog(FString::Printf(TEXT("USettingWidget::OutputLog: 设置项SettingItem")), FColor::Yellow);
        SettingItem->OutputLog();
    }
}

UShellConfigItem* USettingWidget::AddListViewConfigItem()
{
    UShellConfigItem* ConfigItem = NewObject<UShellConfigItem>();
    ConfigItem->Defualut();

    if (ListViewShellConfig) {
        ListViewShellConfig->AddItem(ConfigItem);
    }
    else {
        tytl::DebugLog(FString::Printf(TEXT("USettingWidget::AddListViewConfigItem(): ListViewShellConfig 无效!")));
        tytl::DebugLog(FString::Printf(TEXT("USettingWidget::AddListViewConfigItem(): 添加列表视图项失败!")));
    }

    return ConfigItem;
}

int32 USettingWidget::GetListViewConfigItemIndex(UObject* _Item)
{
    if (ListViewShellConfig) {
        int32 tempIndex = ListViewShellConfig->GetListItems().Find(_Item);
        if (tempIndex != INDEX_NONE) {
            return tempIndex;
        }
        else {
            return int32(INDEX_NONE);
        }
    }
    else {
        tytl::DebugLog(FString::Printf(TEXT("USettingWidget::GetListViewConfigItemIndex: ListViewShellConfig 无效!")));
        return int32(INDEX_NONE);
    }
}

UUserWidget* USettingWidget::GetListViewConfigItemEntry(UObject* _Item)
{
    if (ListViewShellConfig) {
        return Cast<UUserWidget>(ListViewShellConfig->GetEntryWidgetFromItem(_Item));
    }
    else {
        tytl::DebugLog(FString::Printf(TEXT("USettingWidget::GetListViewConfigItemIndex: ListViewShellConfig 无效!")));
        return NewObject<UUserWidget>();
    }
}

int32 USettingWidget::GetListViewConfigItemEnd()
{
    if (ListViewShellConfig) {
        int32 tempIndex = GetListViewConfigItemIndex(ListViewShellConfig->GetItemAt(ListViewShellConfig->GetNumItems() - 1));
        if (tempIndex != INDEX_NONE) {
            return tempIndex;
        }
        else {
            return int32(INDEX_NONE);
        }
    }
    else {
        tytl::DebugLog(FString::Printf(TEXT("USettingWidget::GetListViewConfigItemIndex: ListViewShellConfig 无效!")));
        return int32(INDEX_NONE);
    }
}

void USettingWidget::SaveListViewConfigItem()
{

}

void USettingWidget::LoadListViewConfigItem()
{
}
