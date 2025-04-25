// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Tools.h"

#include "Components/TreeView.h"
#include "Components/TileView.h"
#include "Components/ListView.h"

#include "ListViewBaseDataManage.generated.h"



/**
 * 
 */
UCLASS()
class TYPICALTOOL_API UListViewBaseDataManage : public UObject
{
	GENERATED_BODY()
public:
    TArray<UObject*> ViewDataArray; //视图数据: 数组

public:
    UFUNCTION(BlueprintCallable, Category = "View")
	virtual void SetView(UListViewBase* InView) PURE_VIRTUAL(UListViewBaseDataManage::SetView, );

	virtual void RegenerateAllEntries() PURE_VIRTUAL(UListViewBaseDataManage::RegenerateAllEntries, );

public:
    UFUNCTION(BlueprintCallable, Category = "View")
    TArray<UObject*> GetList() { return ViewDataArray; }
	UFUNCTION(BlueprintCallable, Category = "View")
	virtual void SetList() PURE_VIRTUAL(UListViewBaseDataManage::SetList, );
    UFUNCTION(BlueprintCallable, Category = "View")
	virtual void SetListAt(TArray<UObject*> OtherViewDataArray) PURE_VIRTUAL(UListViewBaseDataManage::SetListAt, );

public:

	/** 添加对象
	* @param 需要添加的列表对象
	*/
	UFUNCTION(BlueprintCallable, Category = "View")
	void Add(UObject* Item);

	/** 查找对象在列表中的索引
	* @param 需要查找的列表对象
	* @returns 对象在列表中的索引
	*/
	UFUNCTION(BlueprintCallable, Category = "View")
	int32 Find(UObject* Item);

	/** 查找在列表中索引的对象
	* @param 需要查找的列表对象索引
	* @returns 索引的列表对象
	*/
	UFUNCTION(BlueprintCallable, Category = "View")
	UObject* FindIndex(int32 Index);

	/** 删除列表中指定的对象(可能是多个)
	* @param 需要删除的列表对象
	* @returns 删除的数量
	*/
	UFUNCTION(BlueprintCallable, Category = "View")
	int32 Remove(UObject* Item);

	/** 删除列表中从 Index开始的 Count个索引项
	* @param 需要删除的索引位置
	* @param 需要从索引位置开始删除的数量
	* @returns 列表中存在索引时, 成功删除
	*/
	UFUNCTION(BlueprintCallable, Category = "View")
	bool RemoveAt(int32 Index, int32 Count = 1);

	/** 获取列表长度
	* @returns 列表长度
	*/
	UFUNCTION(BlueprintCallable, Category = "View")
	int32 Num();

	/** 列表是否为空
	* @returns 列表为空时, 返回 true
	*/
	UFUNCTION(BlueprintCallable, Category = "View")
	bool IsEmpty();

	/** 清空列表
	*/
	UFUNCTION(BlueprintCallable, Category = "View")
	void Empty();
};


/**
 *
 */
UCLASS()
class TYPICALTOOL_API UListViewDataManage : public UListViewBaseDataManage
{
	GENERATED_BODY()
public:
	UPROPERTY()
	UListView* ListView; //列表视图

public:
	virtual void SetView(UListViewBase* InView) override;
	virtual void SetList() override;
	virtual void SetListAt(TArray<UObject*> OtherViewDataArray) override;
	UFUNCTION(BlueprintCallable, Category = "View")
	UListView* GetView() { return ListView; }

	virtual void RegenerateAllEntries();
};


/**
 *
 */
UCLASS()
class TYPICALTOOL_API UTreeViewDataManage : public UListViewBaseDataManage
{
	GENERATED_BODY()
public:
	UPROPERTY()
	UTreeView* TreeView; //树视图

public:
	virtual void SetView(UListViewBase* InView) override;
	virtual void SetList() override;
	virtual void SetListAt(TArray<UObject*> OtherViewDataArray) override;
	UFUNCTION(BlueprintCallable, Category = "View")
	UTreeView* GetView() { return TreeView; }

	virtual void RegenerateAllEntries();
};


/**
 *
 */
UCLASS()
class TYPICALTOOL_API UTileViewDataManage : public UListViewBaseDataManage
{
	GENERATED_BODY()
public:
	UPROPERTY()
	UTileView* TileView; //瓦片视图

public:
	virtual void SetView(UListViewBase* InView) override;
	virtual void SetList() override;
	virtual void SetListAt(TArray<UObject*> OtherViewDataArray) override;
	UFUNCTION(BlueprintCallable, Category = "View")
	UTileView* GetView() { return TileView; }

	virtual void RegenerateAllEntries();
};