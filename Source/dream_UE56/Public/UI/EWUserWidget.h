// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EWUserWidget.generated.h"

class UEWWidgetController;

/**
 * EW用户控件基类 - 所有UI控件的基类
 */
UCLASS()
class DREAM_UE56_API UEWUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 设置Widget Controller
	UFUNCTION(BlueprintCallable, Category = "Widget Controller")
	void SetWidgetController(UObject* InWidgetController);

protected:
	// Widget Controller引用
	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<UObject> WidgetController;

	// 当Widget Controller设置时调用
	UFUNCTION(BlueprintImplementableEvent, Category = "Widget Controller")
	void OnWidgetControllerSet();
};

/**
 * 属性进度条控件 - 用于显示血量、魔法值等属性
 */
UCLASS()
class DREAM_UE56_API UEWAttributeProgressBar : public UEWUserWidget
{
	GENERATED_BODY()

public:
	// 设置进度条值
	UFUNCTION(BlueprintCallable, Category = "Progress Bar")
	void SetProgress(float Current, float Max);

	UFUNCTION(BlueprintCallable, Category = "Progress Bar")
	void SetProgressPercent(float Percent);

	// 设置显示文本
	UFUNCTION(BlueprintCallable, Category = "Progress Bar")
	void SetDisplayText(const FText& Text);

	// 设置颜色
	UFUNCTION(BlueprintCallable, Category = "Progress Bar")
	void SetBarColor(FLinearColor Color);

protected:
	// 当前值
	UPROPERTY(BlueprintReadOnly, Category = "Progress Bar")
	float CurrentValue = 0.0f;

	// 最大值
	UPROPERTY(BlueprintReadOnly, Category = "Progress Bar")
	float MaxValue = 100.0f;

	// 百分比
	UPROPERTY(BlueprintReadOnly, Category = "Progress Bar")
	float Percentage = 0.0f;

	// 显示文本
	UPROPERTY(BlueprintReadOnly, Category = "Progress Bar")
	FText DisplayText;

	// 进度条颜色
	UPROPERTY(BlueprintReadOnly, Category = "Progress Bar")
	FLinearColor BarColor = FLinearColor::Red;

	// 当进度更新时调用（蓝图实现）
	UFUNCTION(BlueprintImplementableEvent, Category = "Progress Bar")
	void OnProgressUpdated();

	// 当文本更新时调用（蓝图实现）
	UFUNCTION(BlueprintImplementableEvent, Category = "Progress Bar")
	void OnTextUpdated();

	// 当颜色更新时调用（蓝图实现）
	UFUNCTION(BlueprintImplementableEvent, Category = "Progress Bar")
	void OnColorUpdated();
};

/**
 * 单位槽位控件 - 用于显示单位信息
 */
UCLASS()
class DREAM_UE56_API UEWUnitSlotWidget : public UEWUserWidget
{
	GENERATED_BODY()

public:
	// 设置单位数据
	UFUNCTION(BlueprintCallable, Category = "Unit Slot")
	void SetUnitSlotData(const struct FUnitSlotData& SlotData);

	// 设置槽位索引
	UFUNCTION(BlueprintCallable, Category = "Unit Slot")
	void SetSlotIndex(int32 Index);

	// 获取槽位索引
	UFUNCTION(BlueprintCallable, Category = "Unit Slot")
	int32 GetSlotIndex() const { return SlotIndex; }

	// 设置选中状态
	UFUNCTION(BlueprintCallable, Category = "Unit Slot")
	void SetSelected(bool bSelected);

	// 设置可拖拽状态
	UFUNCTION(BlueprintCallable, Category = "Unit Slot")
	void SetDraggable(bool bDraggable);

protected:
	// 槽位索引
	UPROPERTY(BlueprintReadOnly, Category = "Unit Slot")
	int32 SlotIndex = -1;

	// 单位数据
	UPROPERTY(BlueprintReadOnly, Category = "Unit Slot")
	struct FUnitSlotData UnitData;

	// 是否选中
	UPROPERTY(BlueprintReadOnly, Category = "Unit Slot")
	bool bIsSelected = false;

	// 是否可拖拽
	UPROPERTY(BlueprintReadOnly, Category = "Unit Slot")
	bool bIsDraggable = true;

	// 当单位数据更新时调用（蓝图实现）
	UFUNCTION(BlueprintImplementableEvent, Category = "Unit Slot")
	void OnUnitDataUpdated();

	// 当选中状态改变时调用（蓝图实现）
	UFUNCTION(BlueprintImplementableEvent, Category = "Unit Slot")
	void OnSelectedStateChanged();

	// 点击事件
	UFUNCTION(BlueprintImplementableEvent, Category = "Unit Slot")
	void OnSlotClicked();

	// 拖拽开始事件
	UFUNCTION(BlueprintImplementableEvent, Category = "Unit Slot")
	void OnDragStarted();

	// 拖拽结束事件
	UFUNCTION(BlueprintImplementableEvent, Category = "Unit Slot")
	void OnDragEnded();

public:
	// 重写拖拽相关函数
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};

/**
 * 战斗槽位控件 - 用于显示战斗配置
 */
UCLASS()
class DREAM_UE56_API UEWBattleSlotWidget : public UEWUnitSlotWidget
{
	GENERATED_BODY()

public:
	// 设置战斗槽位索引
	UFUNCTION(BlueprintCallable, Category = "Battle Slot")
	void SetBattleSlotIndex(int32 Index);

	// 获取战斗槽位索引
	UFUNCTION(BlueprintCallable, Category = "Battle Slot")
	int32 GetBattleSlotIndex() const { return BattleSlotIndex; }

	// 清空槽位
	UFUNCTION(BlueprintCallable, Category = "Battle Slot")
	void ClearSlot();

protected:
	// 战斗槽位索引
	UPROPERTY(BlueprintReadOnly, Category = "Battle Slot")
	int32 BattleSlotIndex = -1;

	// 当战斗槽位索引改变时调用（蓝图实现）
	UFUNCTION(BlueprintImplementableEvent, Category = "Battle Slot")
	void OnBattleSlotIndexChanged();

	// 当槽位被清空时调用（蓝图实现）
	UFUNCTION(BlueprintImplementableEvent, Category = "Battle Slot")
	void OnSlotCleared();
};

/**
 * 属性显示控件 - 用于显示单个属性值
 */
UCLASS()
class DREAM_UE56_API UEWAttributeDisplayWidget : public UEWUserWidget
{
	GENERATED_BODY()

public:
	// 设置属性值
	UFUNCTION(BlueprintCallable, Category = "Attribute Display")
	void SetAttributeValue(float Value);

	// 设置属性名称
	UFUNCTION(BlueprintCallable, Category = "Attribute Display")
	void SetAttributeName(const FText& Name);

	// 设置属性图标
	UFUNCTION(BlueprintCallable, Category = "Attribute Display")
	void SetAttributeIcon(UTexture2D* Icon);

protected:
	// 属性值
	UPROPERTY(BlueprintReadOnly, Category = "Attribute Display")
	float AttributeValue = 0.0f;

	// 属性名称
	UPROPERTY(BlueprintReadOnly, Category = "Attribute Display")
	FText AttributeName;

	// 属性图标
	UPROPERTY(BlueprintReadOnly, Category = "Attribute Display")
	TObjectPtr<UTexture2D> AttributeIcon;

	// 当属性值更新时调用（蓝图实现）
	UFUNCTION(BlueprintImplementableEvent, Category = "Attribute Display")
	void OnAttributeValueUpdated();

	// 当属性名称更新时调用（蓝图实现）
	UFUNCTION(BlueprintImplementableEvent, Category = "Attribute Display")
	void OnAttributeNameUpdated();

	// 当属性图标更新时调用（蓝图实现）
	UFUNCTION(BlueprintImplementableEvent, Category = "Attribute Display")
	void OnAttributeIconUpdated();
};

/**
 * 消息提示控件 - 用于显示游戏消息
 */
UCLASS()
class DREAM_UE56_API UEWMessageWidget : public UEWUserWidget
{
	GENERATED_BODY()

public:
	// 显示消息
	UFUNCTION(BlueprintCallable, Category = "Message")
	void ShowMessage(const FText& Message, float Duration = 3.0f);

	// 隐藏消息
	UFUNCTION(BlueprintCallable, Category = "Message")
	void HideMessage();

	// 设置消息类型
	UFUNCTION(BlueprintCallable, Category = "Message")
	void SetMessageType(int32 Type);

protected:
	// 消息文本
	UPROPERTY(BlueprintReadOnly, Category = "Message")
	FText MessageText;

	// 显示时长
	UPROPERTY(BlueprintReadOnly, Category = "Message")
	float DisplayDuration = 3.0f;

	// 消息类型（0=普通，1=警告，2=错误，3=成功）
	UPROPERTY(BlueprintReadOnly, Category = "Message")
	int32 MessageType = 0;

	// 当显示消息时调用（蓝图实现）
	UFUNCTION(BlueprintImplementableEvent, Category = "Message")
	void OnShowMessage();

	// 当隐藏消息时调用（蓝图实现）
	UFUNCTION(BlueprintImplementableEvent, Category = "Message")
	void OnHideMessage();

private:
	// 隐藏消息的定时器
	FTimerHandle HideMessageTimer;
};
