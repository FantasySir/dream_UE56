// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/EWUserWidget.h"
#include "Player/EWUnitManager.h"
#include "Character/EWUnitBase.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Blueprint/DragDropOperation.h"

//========================================
// UEWUserWidget
//========================================

void UEWUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}

//========================================
// UEWAttributeProgressBar
//========================================

void UEWAttributeProgressBar::SetProgress(float Current, float Max)
{
	CurrentValue = Current;
	MaxValue = Max;
	Percentage = (MaxValue > 0.0f) ? (CurrentValue / MaxValue) : 0.0f;
	
	OnProgressUpdated();
}

void UEWAttributeProgressBar::SetProgressPercent(float Percent)
{
	Percentage = FMath::Clamp(Percent, 0.0f, 1.0f);
	CurrentValue = MaxValue * Percentage;
	
	OnProgressUpdated();
}

void UEWAttributeProgressBar::SetDisplayText(const FText& Text)
{
	DisplayText = Text;
	OnTextUpdated();
}

void UEWAttributeProgressBar::SetBarColor(FLinearColor Color)
{
	BarColor = Color;
	OnColorUpdated();
}

//========================================
// UEWUnitSlotWidget
//========================================

void UEWUnitSlotWidget::SetUnitSlotData(const FUnitSlotData& SlotData)
{
	UnitData = SlotData;
	OnUnitDataUpdated();
}

void UEWUnitSlotWidget::SetSlotIndex(int32 Index)
{
	SlotIndex = Index;
}

void UEWUnitSlotWidget::SetSelected(bool bSelected)
{
	if (bIsSelected != bSelected)
	{
		bIsSelected = bSelected;
		OnSelectedStateChanged();
	}
}

void UEWUnitSlotWidget::SetDraggable(bool bDraggable)
{
	bIsDraggable = bDraggable;
}

void UEWUnitSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	
	if (bIsDraggable && UnitData.UnitClass)
	{
		// 创建拖拽操作
		UDragDropOperation* DragDropOp = NewObject<UDragDropOperation>();
		if (DragDropOp)
		{
			DragDropOp->Payload = this;
			DragDropOp->DefaultDragVisual = this;
			OutOperation = DragDropOp;
			
			OnDragStarted();
		}
	}
}

bool UEWUnitSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (InOperation && InOperation->Payload)
	{
		UEWUnitSlotWidget* SourceWidget = Cast<UEWUnitSlotWidget>(InOperation->Payload);
		if (SourceWidget && SourceWidget != this)
		{
			// 处理拖拽放置逻辑
			// 这里可以交换单位或者其他操作
			OnDragEnded();
			return true;
		}
	}
	
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UEWUnitSlotWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
	// 可以在这里添加高亮效果
}

void UEWUnitSlotWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	// 可以在这里移除高亮效果
}

//========================================
// UEWBattleSlotWidget
//========================================

void UEWBattleSlotWidget::SetBattleSlotIndex(int32 Index)
{
	BattleSlotIndex = Index;
	OnBattleSlotIndexChanged();
}

void UEWBattleSlotWidget::ClearSlot()
{
	UnitData = FUnitSlotData();
	OnSlotCleared();
}

bool UEWBattleSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (InOperation && InOperation->Payload)
	{
		UEWUnitSlotWidget* SourceWidget = Cast<UEWUnitSlotWidget>(InOperation->Payload);
		if (SourceWidget && SourceWidget != this)
		{
			// 检查是否是从普通单位槽拖拽到战斗槽
			UEWBattleSlotWidget* SourceBattleWidget = Cast<UEWBattleSlotWidget>(SourceWidget);
			
			if (SourceBattleWidget)
			{
				// 如果是战斗槽之间的拖拽，交换位置
				FUnitSlotData TempData = UnitData;
				SetUnitSlotData(SourceBattleWidget->GetUnitData());
				SourceBattleWidget->SetUnitSlotData(TempData);
			}
			else
			{
				// 如果是从普通槽拖拽到战斗槽，设置单位
				SetUnitSlotData(SourceWidget->GetUnitData());
			}
			
			OnDragEnded();
			return true;
		}
	}
	
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

//========================================
// UEWAttributeDisplayWidget
//========================================

void UEWAttributeDisplayWidget::SetAttributeValue(float Value)
{
	AttributeValue = Value;
	OnAttributeValueUpdated();
}

void UEWAttributeDisplayWidget::SetAttributeName(const FText& Name)
{
	AttributeName = Name;
	OnAttributeNameUpdated();
}

void UEWAttributeDisplayWidget::SetAttributeIcon(UTexture2D* Icon)
{
	AttributeIcon = Icon;
	OnAttributeIconUpdated();
}

//========================================
// UEWMessageWidget
//========================================

void UEWMessageWidget::ShowMessage(const FText& Message, float Duration)
{
	MessageText = Message;
	DisplayDuration = Duration;
	
	OnShowMessage();
	
	// 设置自动隐藏定时器
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(HideMessageTimer);
		World->GetTimerManager().SetTimer(HideMessageTimer, this, &UEWMessageWidget::HideMessage, Duration, false);
	}
}

void UEWMessageWidget::HideMessage()
{
	OnHideMessage();
	
	// 清除定时器
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(HideMessageTimer);
	}
}

void UEWMessageWidget::SetMessageType(int32 Type)
{
	MessageType = Type;
}
