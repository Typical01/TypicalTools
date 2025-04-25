// Fill out your copyright notice in the Description page of Project Settings.


#include "ListViewBaseDataManage.h"



void UListViewBaseDataManage::Add(UObject* Item)
{
    ViewDataArray.Add(Item);
    SetList();
}

int32 UListViewBaseDataManage::Find(UObject* Item)
{
    return ViewDataArray.Find(Item);
}

UObject* UListViewBaseDataManage::FindIndex(int32 Index)
{
    if (ViewDataArray.Num() <= Index) {
        return nullptr;
    }

    return ViewDataArray[Index];
}

int32 UListViewBaseDataManage::Remove(UObject* Item)
{
    int32 temp = ViewDataArray.Remove(Item);
    SetList();
    return temp;
}

bool UListViewBaseDataManage::RemoveAt(int32 Index, int32 Count)
{
    if (ViewDataArray.Num() <= Index) {
        return false;
    }

    ViewDataArray.RemoveAt(Index, Count);
    SetList();
    return true;
}

int32 UListViewBaseDataManage::Num()
{
    return ViewDataArray.Num();
}

bool UListViewBaseDataManage::IsEmpty()
{
    return ViewDataArray.IsEmpty();
}

void UListViewBaseDataManage::Empty()
{
    ViewDataArray.Empty();
}




//List
void UListViewDataManage::SetView(UListViewBase* InView)
{
    ListView = Cast<UListView>(InView);
}

void UListViewDataManage::SetList()
{
    checkf(ListView != nullptr, TEXT("UListViewDataManage::SetList: ListView is InValid!"));
    if (!IsValid(ListView)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UListViewDataManage::SetList: ListView is InValid!")), FColor::Red);
        return;
    }
    AsyncTask(ENamedThreads::GameThread, [this]()
        {
            ListView->SetListItems(ViewDataArray);
            RegenerateAllEntries();
        });
}

void UListViewDataManage::SetListAt(TArray<UObject*> OtherViewDataArray)
{
    checkf(ListView != nullptr, TEXT("UListViewDataManage::SetListAt: ListView is InValid!"));
    if (!IsValid(ListView)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UListViewDataManage::SetListAt: ListView is InValid!")), FColor::Red);
        return;
    }
    AsyncTask(ENamedThreads::GameThread, [this, &OtherViewDataArray]()
        {
            ListView->SetListItems(OtherViewDataArray);
            RegenerateAllEntries();
        });
}

void UListViewDataManage::RegenerateAllEntries()
{
    checkf(ListView != nullptr, TEXT("UListViewDataManage::RegenerateAllEntries: ListView is InValid!"));
    if (!IsValid(ListView)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UListViewDataManage::RegenerateAllEntries: ListView is InValid!")), FColor::Red);
        return;
    }
    ListView->RegenerateAllEntries();
}




//Tree
void UTreeViewDataManage::SetView(UListViewBase* InView)
{
    TreeView = Cast<UTreeView>(InView);
}

void UTreeViewDataManage::SetList()
{
    checkf(TreeView != nullptr, TEXT("UTreeViewDataManage::SetList: TreeView is InValid!"));
    if (!IsValid(TreeView)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UTreeViewDataManage::SetList: TreeView is InValid!")), FColor::Red);
        return;
    }
    AsyncTask(ENamedThreads::GameThread, [this]()
        {
            TreeView->SetListItems(ViewDataArray);
            RegenerateAllEntries();
        });
}

void UTreeViewDataManage::SetListAt(TArray<UObject*> OtherViewDataArray)
{
    checkf(TreeView != nullptr, TEXT("UTreeViewDataManage::SetListAt: TreeView is InValid!"));
    if (!IsValid(TreeView)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UTreeViewDataManage::SetListAt: TreeView is InValid!")), FColor::Red);
        return;
    }
    AsyncTask(ENamedThreads::GameThread, [this, &OtherViewDataArray]()
        {
            TreeView->SetListItems(OtherViewDataArray);
            RegenerateAllEntries();
        });
}

void UTreeViewDataManage::RegenerateAllEntries()
{
    checkf(TreeView != nullptr, TEXT("UTreeViewDataManage::RegenerateAllEntries: TreeView is InValid!"));
    if (!IsValid(TreeView)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UTreeViewDataManage::RegenerateAllEntries: TreeView is InValid!")), FColor::Red);
        return;
    }
    TreeView->RegenerateAllEntries();
}




//Tile
void UTileViewDataManage::SetView(UListViewBase* InView)
{
    TileView = Cast<UTileView>(InView);
}

void UTileViewDataManage::SetList()
{
    checkf(TileView != nullptr, TEXT("UTileViewDataManage::SetList: TileView is InValid!"));
    if (!IsValid(TileView)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UTileViewDataManage::SetList: TileView is InValid!")), FColor::Red);
        return;
    }
    AsyncTask(ENamedThreads::GameThread, [this]()
        {
            TileView->SetListItems(ViewDataArray);
            RegenerateAllEntries();
        });
}

void UTileViewDataManage::SetListAt(TArray<UObject*> OtherViewDataArray)
{
    checkf(TileView != nullptr, TEXT("UTileViewDataManage::SetListAt: TileView is InValid!"));
    if (!IsValid(TileView)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UTileViewDataManage::SetListAt: TileView is InValid!")), FColor::Red);
        return;
    }
    AsyncTask(ENamedThreads::GameThread, [this, &OtherViewDataArray]()
        {
            TileView->SetListItems(OtherViewDataArray);
            RegenerateAllEntries();
        });
}

void UTileViewDataManage::RegenerateAllEntries()
{
    checkf(TileView != nullptr, TEXT("UTileViewDataManage::RegenerateAllEntries: TileView is InValid!"));
    if (!IsValid(TileView)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UTileViewDataManage::RegenerateAllEntries: TileView is InValid!")), FColor::Red);
        return;
    }
    TileView->RegenerateAllEntries();
}
