// Fill out your copyright notice in the Description page of Project Settings.

#include "GameState/EWUnitState.h"
#include "AbilitySystem/AttributeSets/EWUnitAttributeSet.h"
#include "Character/EWUnitBase.h"
#include "Data/EWUnitData.h"
#include "Core/EWGameplayTags.h"
#include "Engine/World.h"
#include "TimerManager.h"

UEWUnitState::UEWUnitState()
{
	// 创建单位属性集
	UnitAttributeSet = CreateDefaultSubobject<UEWUnitAttributeSet>(TEXT("UnitAttributeSet"));

	// 设置默认Character类为单位基类
	CharacterClass = AEWUnitBase::StaticClass();

	// 初始化装备槽位（假设6个装备槽）
	EquippedItems.SetNum(6);
}

void UEWUnitState::InitializeState()
{
	Super::InitializeState();

	// 应用单位数据配置
	ApplyUnitData();

	// 初始化等级
	UpdateLevel();
}

void UEWUnitState::AddExperience(float Amount)
{
	if (Amount <= 0.0f || !IsAlive())
	{
		return;
	}

	Experience += Amount;
	UpdateLevel();
}

float UEWUnitState::GetExperiencePercentage() const
{
	if (ExperiencePerLevel <= 0.0f)
	{
		return 0.0f;
	}

	float CurrentLevelExp = Experience - ((UnitLevel - 1) * ExperiencePerLevel);
	return FMath::Clamp(CurrentLevelExp / ExperiencePerLevel, 0.0f, 1.0f);
}

float UEWUnitState::GetAttack() const
{
	// TODO: 在UEWUnitAttributeSet中添加Attack属性
	/*
	if (UnitAttributeSet)
	{
		return UnitAttributeSet->GetAttack();
	}
	*/
	return 0.0f;
}

float UEWUnitState::GetDefense() const
{
	// TODO: 在UEWUnitAttributeSet中添加Defense属性
	/*
	if (UnitAttributeSet)
	{
		return UnitAttributeSet->GetDefense();
	}
	*/
	return 0.0f;
}

float UEWUnitState::GetCriticalChance() const
{
	// TODO: 在UEWUnitAttributeSet中添加CriticalChance属性
	/*
	if (UnitAttributeSet)
	{
		return UnitAttributeSet->GetCriticalChance();
	}
	*/
	return 0.0f;
}

float UEWUnitState::GetCriticalMultiplier() const
{
	// TODO: 在UEWUnitAttributeSet中添加CriticalMultiplier属性
	/*
	if (UnitAttributeSet)
	{
		return UnitAttributeSet->GetCriticalMultiplier();
	}
	*/
	return 0.0f;
}

void UEWUnitState::SetUnitData(UEWUnitData* InUnitData)
{
	UnitData = InUnitData;
	ApplyUnitData();
}

float UEWUnitState::GetSummonCost() const
{
	float Cost = BaseSummonCost;
	
	// 根据单位等级调整消耗
	Cost += (UnitLevel - 1) * 5.0f;
	
	// 根据单位职业调整消耗
	switch (UnitClass)
	{
	case EUnitClass::Warrior:
		Cost *= 1.0f;
		break;
	case EUnitClass::Mage:
		Cost *= 1.2f;
		break;
	case EUnitClass::Archer:
		Cost *= 0.9f;
		break;
	case EUnitClass::Rogue:
		Cost *= 0.8f;
		break;
	case EUnitClass::Priest:
		Cost *= 1.1f;
		break;
	default:
		break;
	}

	return Cost;
}

bool UEWUnitState::CanBeSummoned() const
{
	return IsAlive() && !bIsReviving && !HasCharacterActor();
}

void UEWUnitState::StartRevivalCountdown(float RevivalTime)
{
	if (IsAlive() || bIsReviving)
	{
		return;
	}

	bIsReviving = true;
	
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(RevivalTimerHandle, this, &UEWUnitState::OnRevivalComplete, RevivalTime, false);
		UE_LOG(LogTemp, Log, TEXT("UEWUnitState::StartRevivalCountdown - %s revival started, time: %.1f"), *UnitName, RevivalTime);
	}
}

void UEWUnitState::Revive(float HealthPercentage)
{
	if (IsAlive())
	{
		return;
	}

	// 停止复活倒计时
	UWorld* World = GetWorld();
	if (World && RevivalTimerHandle.IsValid())
	{
		World->GetTimerManager().ClearTimer(RevivalTimerHandle);
	}

	bIsReviving = false;

	// 恢复血量
	if (UnitAttributeSet && AbilitySystemComponent)
	{
		float MaxHealth = UnitAttributeSet->GetMaxHealth();
		float NewHealth = MaxHealth * FMath::Clamp(HealthPercentage, 0.1f, 1.0f);
		
		// 通过GE恢复血量
		// TODO: 创建恢复血量的GameplayEffect
	}

	// 更新状态标签
	RemoveStateTag(FEWGameplayTags::State_Dead);
	AddStateTag(FEWGameplayTags::State_Alive);

	UE_LOG(LogTemp, Log, TEXT("UEWUnitState::Revive - %s revived with %.1f%% health"), *UnitName, HealthPercentage * 100.0f);
}

float UEWUnitState::GetRevivalTimeRemaining() const
{
	if (!bIsReviving)
	{
		return 0.0f;
	}

	UWorld* World = GetWorld();
	if (World && RevivalTimerHandle.IsValid())
	{
		return World->GetTimerManager().GetTimerRemaining(RevivalTimerHandle);
	}

	return 0.0f;
}

void UEWUnitState::EquipItem(class UObject* Item, int32 SlotIndex)
{
	if (SlotIndex >= 0 && SlotIndex < EquippedItems.Num())
	{
		EquippedItems[SlotIndex] = Item;
		
		// 这里可以添加装备效果的应用逻辑
		// 例如：属性加成、技能解锁等
	}
}

void UEWUnitState::UnequipItem(int32 SlotIndex)
{
	if (SlotIndex >= 0 && SlotIndex < EquippedItems.Num())
	{
		EquippedItems[SlotIndex] = nullptr;
		
		// 这里可以添加移除装备效果的逻辑
	}
}

AActor* UEWUnitState::SpawnCharacter(UWorld* World, const FVector& Location, const FRotator& Rotation)
{
	AActor* Character = Super::SpawnCharacter(World, Location, Rotation);
	
	if (AEWUnitBase* UnitCharacter = Cast<AEWUnitBase>(Character))
	{
		// 设置单位的基础信息
		UnitCharacter->SetFaction(Faction);
		// TODO: 在AEWUnitBase中添加SetUnitClass方法
		// UnitCharacter->SetUnitClass(UnitClass);
		
		// 应用单位数据
		if (UnitData)
		{
			// TODO: 在AEWUnitBase中添加SetUnitData方法
			// UnitCharacter->SetUnitData(UnitData);
		}
	}

	return Character;
}

void UEWUnitState::SyncStateToCharacter()
{
	Super::SyncStateToCharacter();

	if (AEWUnitBase* UnitCharacter = Cast<AEWUnitBase>(GetCharacterActor()))
	{
		// 同步单位特有的状态
		UnitCharacter->SetFaction(Faction);
		// TODO: 在AEWUnitBase中添加SetUnitClass方法
		// UnitCharacter->SetUnitClass(UnitClass);
		
		if (UnitData)
		{
			// TODO: 在AEWUnitBase中添加SetUnitData方法
			// UnitCharacter->SetUnitData(UnitData);
		}
	}
}

void UEWUnitState::SyncStateFromCharacter()
{
	Super::SyncStateFromCharacter();

	if (AEWUnitBase* UnitCharacter = Cast<AEWUnitBase>(GetCharacterActor()))
	{
		// 从单位角色同步状态
		Faction = UnitCharacter->GetFaction();
		// TODO: 在AEWUnitBase中添加GetUnitClass方法
		// UnitClass = UnitCharacter->GetUnitClass();
	}
}

void UEWUnitState::OnAttributeChangedInternal(const FOnAttributeChangeData& Data)
{
	Super::OnAttributeChangedInternal(Data);

	// 单位特有的属性变化处理
	if (Data.Attribute == UnitAttributeSet->GetHealthAttribute())
	{
		if (Data.NewValue <= 0.0f && Data.OldValue > 0.0f)
		{
			// 单位死亡
			DestroyCharacter();
			
			// 开始复活倒计时
			StartRevivalCountdown(DefaultRevivalTime);
		}
	}
}

void UEWUnitState::UpdateLevel()
{
	if (ExperiencePerLevel <= 0.0f)
	{
		return;
	}

	int32 NewLevel = FMath::FloorToInt(Experience / ExperiencePerLevel) + 1;
	
	if (NewLevel != UnitLevel)
	{
		int32 OldLevel = UnitLevel;
		UnitLevel = NewLevel;
		
		UE_LOG(LogTemp, Log, TEXT("UEWUnitState::UpdateLevel - %s level up! %d -> %d"), *UnitName, OldLevel, UnitLevel);
		
		// 这里可以添加升级奖励逻辑
		// 例如：属性增长、技能解锁等
	}
}

void UEWUnitState::OnRevivalComplete()
{
	Revive(0.3f); // 复活时恢复30%血量
}

void UEWUnitState::ApplyUnitData()
{
	if (!UnitData)
	{
		return;
	}

	// 从UnitData应用配置
	// 这里需要等UEWUnitData类实现后再完善
	
	UE_LOG(LogTemp, Log, TEXT("UEWUnitState::ApplyUnitData - Applied unit data for %s"), *UnitName);
}
