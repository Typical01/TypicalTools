// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingWidget.h"

#include "EntryWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"

#include "TypicalTool/Public/Tools.h"
#include "Tools_GameInstance.h"



USettingWidget::~USettingWidget()
{
    Tools_GameInstance = nullptr;
}


void USettingWidget::InitializeListView()
{
    LoadSettingConfigFile();
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

UObject* USettingWidget::FindListViewIndex(int32 _Index)
{
    if (ListViewShellConfig) {
        return ListViewShellConfig->GetItemAt(_Index);
    }
    else {
        return nullptr;
    }
}

void USettingWidget::DeleteListViewItem(UObject* _Item)
{
    if (ListViewShellConfig) {
        ListViewShellConfig->RemoveItem(_Item);
    }
    else {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::DeleteListViewItem: ListViewItem 删除失败!")));
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
        ListViewShellConfig->RegenerateAllEntries();

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

    UEtytl::DebugLog(FString::Printf(TEXT(" ")), FColor::Green);
    UEtytl::DebugLog(FString::Printf(TEXT(" ")));
    UEtytl::DebugLog(FString::Printf(TEXT(" ")));
    UEtytl::DebugLog(FString::Printf(TEXT(" ")));
    UEtytl::DebugLog(FString::Printf(TEXT(" ")));
    UEtytl::DebugLog(FString::Printf(TEXT(" ")));
    UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OutputLog: bAutoStarting[%d]"), bAutoStarting));
    if (ListViewShellConfig) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OutputLog: 列表视图ShellConfigItems")), FColor::Yellow);
        for (auto& ListItem : ListViewShellConfig->GetListItems()) {
            tempCount++;
            UShellConfigItem* tempShellConfigItem = Cast<UShellConfigItem>(ListItem);
            if (tempShellConfigItem) {
                UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OutputLog: ListItem[%d]"), tempCount), FColor::Yellow);
                tempShellConfigItem->OutputLog();
            }
        }
    }
    tempCount = 0;

    if (SettingItem) {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::OutputLog: 设置项SettingItem")), FColor::Yellow);
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
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::AddListViewConfigItem(): ListViewShellConfig 无效!")));
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::AddListViewConfigItem(): 添加列表视图项失败!")));
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
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::GetListViewConfigItemIndex: ListViewShellConfig 无效!")));
        return int32(INDEX_NONE);
    }
}

UUserWidget* USettingWidget::GetListViewConfigItemEntry(UObject* _Item)
{
    if (ListViewShellConfig) {
        return Cast<UUserWidget>(ListViewShellConfig->GetEntryWidgetFromItem(_Item));
    }
    else {
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::GetListViewConfigItemIndex: ListViewShellConfig 无效!")));
        return nullptr;
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
        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::GetListViewConfigItemIndex: ListViewShellConfig 无效!")));
        return int32(INDEX_NONE);
    }
}

void USettingWidget::SaveSettingConfigFile()
{
    if (Tools_GameInstance) {
        if (Tools_GameInstance->ToolsConfig.IsValid()) {
            for (auto& tempJsonObject : Tools_GameInstance->ToolsConfig->Values) {
                FString& Key = tempJsonObject.Key;
                TSharedPtr<FJsonValue>& Value = tempJsonObject.Value;

                if (Key.Equals(TEXT("基本设置"))) {
                    //保存到配置文件: 基本设置
                    TSharedPtr<FJsonObject> BaseSettingObject = Value->AsObject();

                    if (SettingItem) {
                        BaseSettingObject->SetBoolField(TEXT("注册表开机自启动"), bAutoStarting);
                        BaseSettingObject->SetStringField(TEXT("初始屏幕分辨率宽"), SettingItem->BeginResolutionWidth);
                        BaseSettingObject->SetStringField(TEXT("初始屏幕分辨率高"), SettingItem->BeginResolutionHeight);
                        BaseSettingObject->SetStringField(TEXT("目标屏幕分辨率宽"), SettingItem->TargetResolutionWidth);
                        BaseSettingObject->SetStringField(TEXT("目标屏幕分辨率高"), SettingItem->TargetResolutionHeight);
                    }
                    else {
                        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::InitializeListView(): SettingItem 无效!")));
                        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::InitializeListView(): 初始化分辨率失败!")));
                    }
                }
                else {
                    //保存到配置文件: ShellConfigItem
                    if (ListViewShellConfig) {
                        UShellConfigItem* ConfigItem = NewObject<UShellConfigItem>();
                        ConfigItem->ItemName = Key;

                        TSharedPtr<FJsonObject> OtherConfigItemObject = Value->AsObject();
                        OtherConfigItemObject->SetStringField(TEXT("文件"), ConfigItem->File);
                        OtherConfigItemObject->SetStringField(TEXT("参数"), ConfigItem->Argument);
                        OtherConfigItemObject->SetStringField(TEXT("模式"), UShellConfigItem::ConvertIntToShellMode(ConfigItem->Mode));
                        OtherConfigItemObject->SetBoolField(TEXT("显示窗口"), ConfigItem->ShowWindow);
                        OtherConfigItemObject->SetBoolField(TEXT("菜单按键"), ConfigItem->MenuButton);

                        ListViewShellConfig->AddItem(ConfigItem);
                    }
                    else {
                        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::InitializeListView(): ListViewShellConfig 无效!")));
                        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::InitializeListView(): 添加列表视图项失败!")));
                    }
                }
            }
        }
    }
}

void USettingWidget::LoadSettingConfigFile()
{
    if (Tools_GameInstance) {
        if (Tools_GameInstance->ToolsConfig.IsValid()) {
            for (auto& tempJsonObject : Tools_GameInstance->ToolsConfig->Values) {
                FString& Key = tempJsonObject.Key;
                TSharedPtr<FJsonValue>& Value = tempJsonObject.Value;

                //从配置文件加载: 基本设置
                if (Key.Equals(TEXT("基本设置"))) {
                    TSharedPtr<FJsonObject> BaseSettingObject = Value->AsObject();

                    if (SettingItem) {
                        bAutoStarting = BaseSettingObject->GetBoolField(TEXT("注册表开机自启动"));
                        SettingItem->BeginResolutionWidth = BaseSettingObject->GetStringField(TEXT("初始屏幕分辨率宽"));
                        SettingItem->BeginResolutionHeight = BaseSettingObject->GetStringField(TEXT("初始屏幕分辨率高"));
                        SettingItem->TargetResolutionWidth = BaseSettingObject->GetStringField(TEXT("目标屏幕分辨率宽"));
                        SettingItem->TargetResolutionHeight = BaseSettingObject->GetStringField(TEXT("目标屏幕分辨率高"));
                    }
                    else {
                        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::InitializeListView(): SettingItem 无效!")));
                        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::InitializeListView(): 初始化分辨率失败!")));
                    }
                }
                else {
                    //从配置文件加载 ShellConfigItem
                    if (ListViewShellConfig) {
                        UShellConfigItem* ConfigItem = NewObject<UShellConfigItem>();
                        ConfigItem->ItemName = Key;

                        TSharedPtr<FJsonObject> OtherConfigItemObject = Value->AsObject();
                        ConfigItem->File = OtherConfigItemObject->GetStringField(TEXT("文件"));
                        ConfigItem->Argument = OtherConfigItemObject->GetStringField(TEXT("参数"));
                        ConfigItem->Mode = UShellConfigItem::ConvertShellModeToInt(OtherConfigItemObject->GetStringField(TEXT("模式")));
                        ConfigItem->ShowWindow = OtherConfigItemObject->GetBoolField(TEXT("显示窗口"));
                        ConfigItem->MenuButton = OtherConfigItemObject->GetBoolField(TEXT("菜单按键"));

                        ListViewShellConfig->AddItem(ConfigItem);
                    }
                    else {
                        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::InitializeListView(): ListViewShellConfig 无效!")));
                        UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::InitializeListView(): 添加列表视图项失败!")));
                    }
                }
            }
        }
    }
}
