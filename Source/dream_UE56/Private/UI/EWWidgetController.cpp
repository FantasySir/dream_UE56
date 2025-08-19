// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/EWWidgetController.h"
#include "Character/EWUnitAttributeSet.h"
#include "Player/EWUnitManager.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"

void UEWWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UEWWidgetController::BroadcastInitialValues()
{
	// 在子类中实现
}

void UEWWidgetController::BindCallbacksToDependencies()
{
	// 在子类中实现
}

void UEWWidgetController::BroadcastMessage(FGameplayTag MessageTag)
{
	MessageWidgetRowDelegate.Broadcast(MessageTag);
}

//========================================
// UEWAttributeMenuWidgetController
//========================================

void UEWAttributeMenuWidgetController::BroadcastInitialValues()
{
	if (!AttributeSet) return;

	UEWUnitAttributeSet* EWAttributeSet = CastChecked<UEWUnitAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(EWAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(EWAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(EWAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(EWAttributeSet->GetMaxMana());
	OnActionPointChanged.Broadcast(EWAttributeSet->GetActionPoint());
	OnMaxActionPointChanged.Broadcast(EWAttributeSet->GetMaxActionPoint());
	OnPhysicalAttackChanged.Broadcast(EWAttributeSet->GetPhysicalAttack());
	OnMagicalAttackChanged.Broadcast(EWAttributeSet->GetMagicalAttack());
	OnPhysicalDefenseChanged.Broadcast(EWAttributeSet->GetPhysicalDefense());
	OnMagicalDefenseChanged.Broadcast(EWAttributeSet->GetMagicalDefense());
}

void UEWAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	if (!AbilitySystemComponent || !AttributeSet) return;

	UEWUnitAttributeSet* EWAttributeSet = CastChecked<UEWUnitAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EWAttributeSet->GetHealthAttribute()).AddUObject(this, &UEWAttributeMenuWidgetController::OnHealthChangedCallback);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EWAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UEWAttributeMenuWidgetController::OnMaxHealthChangedCallback);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EWAttributeSet->GetManaAttribute()).AddUObject(this, &UEWAttributeMenuWidgetController::OnManaChangedCallback);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EWAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UEWAttributeMenuWidgetController::OnMaxManaChangedCallback);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EWAttributeSet->GetActionPointAttribute()).AddUObject(this, &UEWAttributeMenuWidgetController::OnActionPointChangedCallback);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EWAttributeSet->GetMaxActionPointAttribute()).AddUObject(this, &UEWAttributeMenuWidgetController::OnMaxActionPointChangedCallback);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EWAttributeSet->GetPhysicalAttackAttribute()).AddUObject(this, &UEWAttributeMenuWidgetController::OnPhysicalAttackChangedCallback);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EWAttributeSet->GetMagicalAttackAttribute()).AddUObject(this, &UEWAttributeMenuWidgetController::OnMagicalAttackChangedCallback);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EWAttributeSet->GetPhysicalDefenseAttribute()).AddUObject(this, &UEWAttributeMenuWidgetController::OnPhysicalDefenseChangedCallback);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EWAttributeSet->GetMagicalDefenseAttribute()).AddUObject(this, &UEWAttributeMenuWidgetController::OnMagicalDefenseChangedCallback);
}

void UEWAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	// TODO: 实现属性升级逻辑
	// 这里可以通过GameplayEffect来升级属性
}

void UEWAttributeMenuWidgetController::OnHealthChangedCallback(const FOnAttributeChangeData& Data)
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UEWAttributeMenuWidgetController::OnMaxHealthChangedCallback(const FOnAttributeChangeData& Data)
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UEWAttributeMenuWidgetController::OnManaChangedCallback(const FOnAttributeChangeData& Data)
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UEWAttributeMenuWidgetController::OnMaxManaChangedCallback(const FOnAttributeChangeData& Data)
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}

void UEWAttributeMenuWidgetController::OnActionPointChangedCallback(const FOnAttributeChangeData& Data)
{
	OnActionPointChanged.Broadcast(Data.NewValue);
}

void UEWAttributeMenuWidgetController::OnMaxActionPointChangedCallback(const FOnAttributeChangeData& Data)
{
	OnMaxActionPointChanged.Broadcast(Data.NewValue);
}

void UEWAttributeMenuWidgetController::OnPhysicalAttackChangedCallback(const FOnAttributeChangeData& Data)
{
	OnPhysicalAttackChanged.Broadcast(Data.NewValue);
}

void UEWAttributeMenuWidgetController::OnMagicalAttackChangedCallback(const FOnAttributeChangeData& Data)
{
	OnMagicalAttackChanged.Broadcast(Data.NewValue);
}

void UEWAttributeMenuWidgetController::OnPhysicalDefenseChangedCallback(const FOnAttributeChangeData& Data)
{
	OnPhysicalDefenseChanged.Broadcast(Data.NewValue);
}

void UEWAttributeMenuWidgetController::OnMagicalDefenseChangedCallback(const FOnAttributeChangeData& Data)
{
	OnMagicalDefenseChanged.Broadcast(Data.NewValue);
}

//========================================
// UEWOverlayWidgetController
//========================================

void UEWOverlayWidgetController::BroadcastInitialValues()
{
	if (!AttributeSet) return;

	UEWUnitAttributeSet* EWAttributeSet = CastChecked<UEWUnitAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(EWAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(EWAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(EWAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(EWAttributeSet->GetMaxMana());
	OnActionPointChanged.Broadcast(EWAttributeSet->GetActionPoint());
	OnMaxActionPointChanged.Broadcast(EWAttributeSet->GetMaxActionPoint());
	OnLevelChanged.Broadcast(EWAttributeSet->GetLevel());
}

void UEWOverlayWidgetController::BindCallbacksToDependencies()
{
	if (!AbilitySystemComponent || !AttributeSet) return;

	UEWUnitAttributeSet* EWAttributeSet = CastChecked<UEWUnitAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EWAttributeSet->GetHealthAttribute()).AddUObject(this, &UEWOverlayWidgetController::OnHealthChangedCallback);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EWAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UEWOverlayWidgetController::OnMaxHealthChangedCallback);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EWAttributeSet->GetManaAttribute()).AddUObject(this, &UEWOverlayWidgetController::OnManaChangedCallback);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EWAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UEWOverlayWidgetController::OnMaxManaChangedCallback);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EWAttributeSet->GetActionPointAttribute()).AddUObject(this, &UEWOverlayWidgetController::OnActionPointChangedCallback);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EWAttributeSet->GetMaxActionPointAttribute()).AddUObject(this, &UEWOverlayWidgetController::OnMaxActionPointChangedCallback);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EWAttributeSet->GetLevelAttribute()).AddUObject(this, &UEWOverlayWidgetController::OnLevelChangedCallback);
}

void UEWOverlayWidgetController::OnHealthChangedCallback(const FOnAttributeChangeData& Data)
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UEWOverlayWidgetController::OnMaxHealthChangedCallback(const FOnAttributeChangeData& Data)
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UEWOverlayWidgetController::OnManaChangedCallback(const FOnAttributeChangeData& Data)
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UEWOverlayWidgetController::OnMaxManaChangedCallback(const FOnAttributeChangeData& Data)
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}

void UEWOverlayWidgetController::OnActionPointChangedCallback(const FOnAttributeChangeData& Data)
{
	OnActionPointChanged.Broadcast(Data.NewValue);
}

void UEWOverlayWidgetController::OnMaxActionPointChangedCallback(const FOnAttributeChangeData& Data)
{
	OnMaxActionPointChanged.Broadcast(Data.NewValue);
}

void UEWOverlayWidgetController::OnLevelChangedCallback(const FOnAttributeChangeData& Data)
{
	OnLevelChanged.Broadcast(Data.NewValue);
}

//========================================
// UEWUnitManagementWidgetController
//========================================

void UEWUnitManagementWidgetController::BroadcastInitialValues()
{
	if (UnitManager)
	{
		// 广播当前单位状态
		for (int32 i = 0; i < UnitManager->GetTotalSlots(); ++i)
		{
			FUnitSlotData SlotData = UnitManager->GetUnitSlot(i);
			OnUnitSlotChanged.Broadcast(i, SlotData);
		}

		TArray<FUnitSlotData> BattleUnits = UnitManager->GetBattleUnits();
		for (int32 i = 0; i < BattleUnits.Num(); ++i)
		{
			OnBattleSlotChanged.Broadcast(i, BattleUnits[i]);
		}

		OnSummonCostChanged.Broadcast(UnitManager->CalculateSummonCost());
	}
}

void UEWUnitManagementWidgetController::BindCallbacksToDependencies()
{
	if (UnitManager)
	{
		UnitManager->OnUnitSlotChanged.AddDynamic(this, &UEWUnitManagementWidgetController::OnUnitSlotChangedCallback);
		UnitManager->OnBattleSlotChanged.AddDynamic(this, &UEWUnitManagementWidgetController::OnBattleSlotChangedCallback);
	}
}

void UEWUnitManagementWidgetController::SetUnitManager(UEWUnitManager* Manager)
{
	UnitManager = Manager;
	BindCallbacksToDependencies();
	BroadcastInitialValues();
}

void UEWUnitManagementWidgetController::SetBattleSlot(int32 BattleSlotIndex, int32 UnitSlotIndex)
{
	if (UnitManager)
	{
		UnitManager->SetBattleSlot(BattleSlotIndex, UnitSlotIndex);
		OnSummonCostChanged.Broadcast(UnitManager->CalculateSummonCost());
	}
}

void UEWUnitManagementWidgetController::ClearBattleSlot(int32 BattleSlotIndex)
{
	if (UnitManager)
	{
		UnitManager->ClearBattleSlot(BattleSlotIndex);
		OnSummonCostChanged.Broadcast(UnitManager->CalculateSummonCost());
	}
}

void UEWUnitManagementWidgetController::SwapBattleSlots(int32 BattleSlotIndex1, int32 BattleSlotIndex2)
{
	if (UnitManager)
	{
		UnitManager->SwapBattleSlots(BattleSlotIndex1, BattleSlotIndex2);
	}
}

float UEWUnitManagementWidgetController::GetSummonCost() const
{
	return UnitManager ? UnitManager->CalculateSummonCost() : 0.0f;
}

bool UEWUnitManagementWidgetController::CanSummonAll() const
{
	if (!UnitManager || !AttributeSet) return false;
	
	UEWUnitAttributeSet* EWAttributeSet = Cast<UEWUnitAttributeSet>(AttributeSet);
	if (!EWAttributeSet) return false;

	float AvailableActionPoint = EWAttributeSet->GetActionPoint();
	return UnitManager->CanSummonAll(AvailableActionPoint);
}

void UEWUnitManagementWidgetController::OnUnitSlotChangedCallback(int32 SlotIndex, const FUnitSlotData& SlotData)
{
	OnUnitSlotChanged.Broadcast(SlotIndex, SlotData);
}

void UEWUnitManagementWidgetController::OnBattleSlotChangedCallback(int32 BattleSlotIndex, const FUnitSlotData& SlotData)
{
	OnBattleSlotChanged.Broadcast(BattleSlotIndex, SlotData);
	OnSummonCostChanged.Broadcast(GetSummonCost());
}
