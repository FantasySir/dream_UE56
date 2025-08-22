// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/EWWidgetController.h"
#include "AbilitySystem/AttributeSets/EWBaseAttributeSet.h"
#include "AbilitySystem/AttributeSets/EWPlayerAttributeSet.h"
#include "AbilitySystem/AttributeSets/EWCombatAttributeSet.h"
#include "Player/EWUnitManager.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"

void UEWWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	BaseAttributeSet = WCParams.BaseAttributeSet;
	PlayerAttributeSet = WCParams.PlayerAttributeSet;
	CombatAttributeSet = WCParams.CombatAttributeSet;
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
	if (BaseAttributeSet)
	{
		OnHealthChanged.Broadcast(BaseAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(BaseAttributeSet->GetMaxHealth());
		OnManaChanged.Broadcast(BaseAttributeSet->GetMana());
		OnMaxManaChanged.Broadcast(BaseAttributeSet->GetMaxMana());
	}

	if (PlayerAttributeSet)
	{
		OnActionPointChanged.Broadcast(PlayerAttributeSet->GetActionPoint());
		OnMaxActionPointChanged.Broadcast(PlayerAttributeSet->GetMaxActionPoint());
	}

	if (CombatAttributeSet)
	{
		OnPhysicalAttackChanged.Broadcast(CombatAttributeSet->GetPhysicalAttack());
		OnMagicalAttackChanged.Broadcast(CombatAttributeSet->GetMagicalAttack());
		OnPhysicalDefenseChanged.Broadcast(CombatAttributeSet->GetPhysicalDefense());
		OnMagicalDefenseChanged.Broadcast(CombatAttributeSet->GetMagicalDefense());
		OnMovementSpeedChanged.Broadcast(CombatAttributeSet->GetMovementSpeed());
	}
}

void UEWAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	if (!AbilitySystemComponent) return;

	// 绑定基础属性变化
	if (BaseAttributeSet)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {
			OnHealthChanged.Broadcast(Data.NewValue);
		});
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {
			OnManaChanged.Broadcast(Data.NewValue);
		});
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {
			OnMaxManaChanged.Broadcast(Data.NewValue);
		});
	}

	// 绑定玩家属性变化
	if (PlayerAttributeSet)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PlayerAttributeSet->GetActionPointAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {
			OnActionPointChanged.Broadcast(Data.NewValue);
		});
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PlayerAttributeSet->GetMaxActionPointAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {
			OnMaxActionPointChanged.Broadcast(Data.NewValue);
		});
	}

	// 绑定战斗属性变化
	if (CombatAttributeSet)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CombatAttributeSet->GetPhysicalAttackAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {
			OnPhysicalAttackChanged.Broadcast(Data.NewValue);
		});
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CombatAttributeSet->GetMagicalAttackAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {
			OnMagicalAttackChanged.Broadcast(Data.NewValue);
		});
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CombatAttributeSet->GetPhysicalDefenseAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {
			OnPhysicalDefenseChanged.Broadcast(Data.NewValue);
		});
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CombatAttributeSet->GetMagicalDefenseAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {
			OnMagicalDefenseChanged.Broadcast(Data.NewValue);
		});
	}
}

void UEWAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	// TODO: 实现属性升级逻辑
	// 这里可以通过GameplayEffect来升级属性
}

//========================================
// UEWOverlayWidgetController
//========================================

void UEWOverlayWidgetController::BroadcastInitialValues()
{
	if (BaseAttributeSet)
	{
		OnHealthChanged.Broadcast(BaseAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(BaseAttributeSet->GetMaxHealth());
		OnManaChanged.Broadcast(BaseAttributeSet->GetMana());
		OnMaxManaChanged.Broadcast(BaseAttributeSet->GetMaxMana());
		OnLevelChanged.Broadcast(BaseAttributeSet->GetUnitLevel());
	}

	if (PlayerAttributeSet)
	{
		OnActionPointChanged.Broadcast(PlayerAttributeSet->GetActionPoint());
		OnMaxActionPointChanged.Broadcast(PlayerAttributeSet->GetMaxActionPoint());
	}
}

void UEWOverlayWidgetController::BindCallbacksToDependencies()
{
	if (!AbilitySystemComponent) return;

	// 绑定基础属性变化
	if (BaseAttributeSet)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {
			OnHealthChanged.Broadcast(Data.NewValue);
		});
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {
			OnManaChanged.Broadcast(Data.NewValue);
		});
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {
			OnMaxManaChanged.Broadcast(Data.NewValue);
		});
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetUnitLevelAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {
			OnLevelChanged.Broadcast(Data.NewValue);
		});
	}

	// 绑定玩家属性变化
	if (PlayerAttributeSet)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PlayerAttributeSet->GetActionPointAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {
			OnActionPointChanged.Broadcast(Data.NewValue);
		});
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PlayerAttributeSet->GetMaxActionPointAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {
			OnMaxActionPointChanged.Broadcast(Data.NewValue);
		});
	}
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
	if (!UnitManager || !PlayerAttributeSet) return false;

	float AvailableActionPoint = PlayerAttributeSet->GetActionPoint();
	return UnitManager->CanSummonAll(AvailableActionPoint);
}

void UEWUnitManagementWidgetController::OnUnitSlotChangedCallback(int32 SlotIndex, const FUnitSlotData& SlotData)
{
	OnUnitSlotChanged.Broadcast(SlotIndex, SlotData);
}

void UEWUnitManagementWidgetController::OnBattleSlotChangedCallback(int32 BattleSlotIndex, const FUnitSlotData& SlotData)
{
	OnBattleSlotChanged.Broadcast(BattleSlotIndex, SlotData);
}
