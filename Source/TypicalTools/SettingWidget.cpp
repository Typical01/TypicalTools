// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingWidget.h"

#include "EntryWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"

#include "TypicalTool/Public/Tools.h"
#include "Tools_GameInstance.h"
#include "SettingItem.h"



USettingWidget::~USettingWidget()
{
    Tools_GameInstance = nullptr;
}


void USettingWidget::InitializeListView()
{
    if (SettingItem) {
        SettingItem->ResolutionVec = GetAllSupportedResolutions();
    }

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

    SaveSettingConfigFile();

    //if (SettingItem) {
    //    //创建对象: 基本设置
    //    TSharedPtr<FJsonObject> ItemObject_BaseSetting = MakeShareable(new FJsonObject());
    //    ItemObject_BaseSetting->SetBoolField(TEXT("注册表开机自启动"), bAutoStarting);
    //    ItemObject_BaseSetting->SetNumberField(TEXT("初始屏幕分辨率宽"), FCString::Atoi(*SettingItem->BeginResolutionWidth));
    //    ItemObject_BaseSetting->SetNumberField(TEXT("初始屏幕分辨率高"), FCString::Atoi(*SettingItem->BeginResolutionHeight));
    //    ItemObject_BaseSetting->SetNumberField(TEXT("目标屏幕分辨率宽"), FCString::Atoi(*SettingItem->TargetResolutionWidth));
    //    ItemObject_BaseSetting->SetNumberField(TEXT("目标屏幕分辨率高"), FCString::Atoi(*SettingItem->TargetResolutionHeight));

    //    Tools_GameInstance->ToolsConfig->SetObjectField(TEXT("基本设置"), ItemObject_BaseSetting);
    //}

    //if (ListViewShellConfig) {
    //    for (auto tempShellConfigItem : ListViewShellConfig->GetListItems()) {
    //        auto* ShellConfigItem = Cast<UShellConfigItem>(tempShellConfigItem);

    //        //创建对象: ShellConfig
    //        TSharedPtr<FJsonObject> ItemObject_ShellConfig = MakeShareable(new FJsonObject());
    //        ItemObject_ShellConfig->SetStringField(TEXT("文件"), ShellConfigItem->File);
    //        ItemObject_ShellConfig->SetStringField(TEXT("参数"), ShellConfigItem->Argument);
    //        ItemObject_ShellConfig->SetStringField(TEXT("模式"), FString::FromInt(ShellConfigItem->Mode));
    //        ItemObject_ShellConfig->SetBoolField(TEXT("显示窗口"), ShellConfigItem->ShowWindow);
    //        ItemObject_ShellConfig->SetBoolField(TEXT("菜单按键"), ShellConfigItem->MenuButton);

    //        Tools_GameInstance->ToolsConfig->SetObjectField(ShellConfigItem->ItemName, ItemObject_ShellConfig);
    //    }
    //}
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
            if (ListViewShellConfig) {
                //保存到配置文件: ShellConfigItem
                for (auto tempJsonObject : ListViewShellConfig->GetListItems()) {
                    UShellConfigItem* ShellConfigItem = Cast<UShellConfigItem>(tempJsonObject);

                    TSharedPtr<FJsonObject> ShellConfigItemObject = MakeShareable(new FJsonObject());
                    ShellConfigItemObject->SetStringField(TEXT("文件"), ShellConfigItem->File);
                    ShellConfigItemObject->SetStringField(TEXT("参数"), ShellConfigItem->Argument);
                    ShellConfigItemObject->SetStringField(TEXT("模式"), ShellConfigItem->Mode);
                    ShellConfigItemObject->SetBoolField(TEXT("显示窗口"), ShellConfigItem->ShowWindow);
                    ShellConfigItemObject->SetBoolField(TEXT("菜单按键"), ShellConfigItem->MenuButton);

                    Tools_GameInstance->ToolsConfig->SetObjectField(ShellConfigItem->ItemName, ShellConfigItemObject);
                }
            }
            else {
                UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::SaveSettingConfigFile(): ListViewShellConfig 无效!")));
                UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::SaveSettingConfigFile(): 添加列表视图项失败!")));
            }

            if (SettingItem) {
                //保存到配置文件: 基本设置
                TSharedPtr<FJsonObject> BaseSettingObject = MakeShareable(new FJsonObject());

                BaseSettingObject->SetBoolField(TEXT("注册表开机自启动"), bAutoStarting);
                BaseSettingObject->SetStringField(TEXT("初始屏幕分辨率宽"), SettingItem->BeginResolutionWidth);
                BaseSettingObject->SetStringField(TEXT("初始屏幕分辨率高"), SettingItem->BeginResolutionHeight);
                BaseSettingObject->SetStringField(TEXT("目标屏幕分辨率宽"), SettingItem->TargetResolutionWidth);
                BaseSettingObject->SetStringField(TEXT("目标屏幕分辨率高"), SettingItem->TargetResolutionHeight);

                Tools_GameInstance->ToolsConfig->SetObjectField(TEXT("基本设置"), BaseSettingObject);
            }
            else {
                UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::InitializeListView(): SettingItem 无效!")));
                UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::InitializeListView(): 初始化分辨率失败!")));
            }
        }
    }
}

void USettingWidget::LoadSettingConfigFile()
{
    //组合框: 字符串, 初始化
    //ComboBoxStringBeginResolutionWidth->ClearOptions();
    //ComboBoxStringBeginResolutionHeight->ClearOptions();
    //ComboBoxStringTargetResolutionWidth->ClearOptions();
    //ComboBoxStringTargetResolutionHeight->ClearOptions();



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
                        ConfigItem->Mode = OtherConfigItemObject->GetStringField(TEXT("模式"));
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
