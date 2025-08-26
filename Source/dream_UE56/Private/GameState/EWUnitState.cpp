// Fill out your copyright notice in the Description page of Project Settings.

#include "GameState/EWUnitState.h"
#include "AbilitySystem/AttributeSets/EWBaseAttributeSet.h"
#include "AbilitySystem/AttributeSets/EWUnitAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

AEWUnitState::AEWUnitState()
{
	SetNetUpdateFrequency(100.f);
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	// 配置AbilitySystemComponent
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	// 初始化属性集
	BaseAttributeSet = CreateDefaultSubobject<UEWBaseAttributeSet>("BaseAttributeSet");
	UnitAttributeSet = CreateDefaultSubobject<UEWUnitAttributeSet>("UnitAttributeSet");
	
	// 初始化装备槽位（默认6个装备槽）
	EquippedItems.SetNum(6);
}

void AEWUnitState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 等级经验
	DOREPLIFETIME(AEWUnitState, Level);
	DOREPLIFETIME(AEWUnitState, Experience);

	// 基础信息
	DOREPLIFETIME(AEWUnitState, UnitName);
	DOREPLIFETIME(AEWUnitState, Faction);
	DOREPLIFETIME(AEWUnitState, UnitClass);

	// 召唤数据
	DOREPLIFETIME(AEWUnitState, BaseSummonCost);

	// 复活数据
	DOREPLIFETIME(AEWUnitState, bIsReviving);
	DOREPLIFETIME(AEWUnitState, DefaultRevivalTime);

	// 装备数据
	DOREPLIFETIME(AEWUnitState, EquippedItems);
}

UAbilitySystemComponent* AEWUnitState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// ===================== 等级经验接口实现 =====================

int32 AEWUnitState::GetLevel_Implementation() const
{
	return Level;
}

void AEWUnitState::SetLevel_Implementation(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level, true);
}

void AEWUnitState::AddToLevel_Implementation(int32 InLevel)
{
	SetLevel_Implementation(Level + InLevel);
}

int32 AEWUnitState::GetExperience_Implementation() const
{
	return Experience;
}

void AEWUnitState::SetExperience_Implementation(int32 InExperience)
{
	Experience = InExperience;
	OnExperienceChangedDelegate.Broadcast(Experience);
}

void AEWUnitState::AddToExperience_Implementation(int32 InExperience)
{
	SetExperience_Implementation(Experience + InExperience);
}

void AEWUnitState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level, true);
}

void AEWUnitState::OnRep_Experience(int32 OldExperience)
{
	OnExperienceChangedDelegate.Broadcast(Experience);
}

// ===================== 基础信息 Get/Set =====================

void AEWUnitState::SetUnitName(const FString& Name)
{
	UnitName = Name;
}

void AEWUnitState::SetFaction(EUnitFaction InFaction)
{
	Faction = InFaction;
}

void AEWUnitState::SetUnitClass(EUnitClass InClass)
{
	UnitClass = InClass;
}

// ===================== 召唤相关 Get/Set =====================

void AEWUnitState::SetBaseSummonCost(float InCost)
{
	if (InCost >= 0.0f)
	{
		BaseSummonCost = InCost;
	}
}

// ===================== 复活相关 Get/Set =====================

void AEWUnitState::SetReviving(bool bInReviving)
{
	bIsReviving = bInReviving;
}

void AEWUnitState::SetDefaultRevivalTime(float InTime)
{
	if (InTime > 0.0f)
	{
		DefaultRevivalTime = InTime;
	}
}

// ===================== 装备系统 Get/Set =====================

UObject* AEWUnitState::GetEquippedItem(int32 SlotIndex) const
{
	if (SlotIndex >= 0 && SlotIndex < EquippedItems.Num())
	{
		return EquippedItems[SlotIndex];
	}
	return nullptr;
}

void AEWUnitState::SetEquippedItem(int32 SlotIndex, UObject* Item)
{
	if (SlotIndex >= 0 && SlotIndex < EquippedItems.Num())
	{
		EquippedItems[SlotIndex] = Item;
	}
}

void AEWUnitState::SetEquipmentSlotCount(int32 SlotCount)
{
	if (SlotCount >= 0)
	{
		EquippedItems.SetNum(SlotCount);
	}
}
