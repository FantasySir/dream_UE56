// Fill out your copyright notice in the Description page of Project Settings.

#include "GameState/EWPlayerState.h"
#include "GameState/EWUnitState.h"
#include "AbilitySystem/AttributeSets/EWPlayerAttributeSet.h"
#include "Character/EWCharacterBase.h"
#include "Character/EWUnitBase.h"
#include "Player/EWUnitManager.h"
#include "Core/EWGameplayTags.h"
#include "Engine/World.h"
#include "TimerManager.h"

UEWPlayerState::UEWPlayerState()
{
	// 创建玩家属性集
	PlayerAttributeSet = CreateDefaultSubobject<UEWPlayerAttributeSet>(TEXT("PlayerAttributeSet"));
	
	// 创建单位管理器
	UnitManager = CreateDefaultSubobject<UEWUnitManager>(TEXT("UnitManager"));

	// 设置默认Character类为玩家角色
	CharacterClass = AEWCharacterBase::StaticClass();

	// 初始化参战单位槽位（默认4个槽位）
	ActiveUnits.SetNum(4);
}

void UEWPlayerState::InitializeState()
{
	Super::InitializeState();

	// 初始化单位管理器
	if (UnitManager)
	{
		UnitManager->Initialize(12, 4); // 12个总槽位，4个战斗槽位
	}

	// 初始化等级
	UpdateLevel();
}

float UEWPlayerState::GetActionPoint() const
{
	if (PlayerAttributeSet)
	{
		return PlayerAttributeSet->GetActionPoint();
	}
	return 0.0f;
}

float UEWPlayerState::GetMaxActionPoint() const
{
	if (PlayerAttributeSet)
	{
		return PlayerAttributeSet->GetMaxActionPoint();
	}
	return 0.0f;
}

void UEWPlayerState::AddOwnedUnit(UEWUnitState* UnitState)
{
	if (UnitState && !OwnedUnits.Contains(UnitState))
	{
		OwnedUnits.Add(UnitState);
		
		// 绑定单位死亡事件
		UnitState->OnCharacterDestroyed.AddDynamic(this, &UEWPlayerState::OnUnitDeath);
	}
}

void UEWPlayerState::RemoveOwnedUnit(UEWUnitState* UnitState)
{
	if (UnitState)
	{
		OwnedUnits.Remove(UnitState);
		
		// 从参战列表中移除
		for (int32 i = 0; i < ActiveUnits.Num(); ++i)
		{
			if (ActiveUnits[i] == UnitState)
			{
				ActiveUnits[i] = nullptr;
			}
		}
		
		// 解绑事件
		UnitState->OnCharacterDestroyed.RemoveDynamic(this, &UEWPlayerState::OnUnitDeath);
	}
}

bool UEWPlayerState::SetActiveUnit(int32 SlotIndex, UEWUnitState* UnitState)
{
	if (SlotIndex < 0 || SlotIndex >= ActiveUnits.Num())
	{
		return false;
	}

	// 检查单位是否属于玩家
	if (UnitState && !OwnedUnits.Contains(UnitState))
	{
		return false;
	}

	ActiveUnits[SlotIndex] = UnitState;
	return true;
}

UEWUnitState* UEWPlayerState::GetActiveUnitAtSlot(int32 SlotIndex) const
{
	if (SlotIndex >= 0 && SlotIndex < ActiveUnits.Num())
	{
		return ActiveUnits[SlotIndex];
	}
	return nullptr;
}

AEWUnitBase* UEWPlayerState::SummonUnit(int32 SlotIndex, const FVector& Location, const FRotator& Rotation)
{
	UEWUnitState* UnitState = GetActiveUnitAtSlot(SlotIndex);
	if (!UnitState || !UnitState->IsAlive())
	{
		return nullptr;
	}

	// 检查是否有足够的行动点或魔法值
	// 这里可以添加召唤消耗的逻辑

	UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	// 通过UnitState生成Character
	AActor* SpawnedActor = UnitState->SpawnCharacter(World, Location, Rotation);
	AEWUnitBase* SummonedUnit = Cast<AEWUnitBase>(SpawnedActor);

	if (SummonedUnit)
	{
		UE_LOG(LogTemp, Log, TEXT("UEWPlayerState::SummonUnit - Successfully summoned unit at slot %d"), SlotIndex);
	}

	return SummonedUnit;
}

void UEWPlayerState::RecallUnit(int32 SlotIndex)
{
	UEWUnitState* UnitState = GetActiveUnitAtSlot(SlotIndex);
	if (UnitState)
	{
		UnitState->DestroyCharacter();
		UE_LOG(LogTemp, Log, TEXT("UEWPlayerState::RecallUnit - Recalled unit at slot %d"), SlotIndex);
	}
}

void UEWPlayerState::RecallAllUnits()
{
	for (int32 i = 0; i < ActiveUnits.Num(); ++i)
	{
		RecallUnit(i);
	}
}

void UEWPlayerState::AddExperience(float Amount)
{
	if (Amount <= 0.0f)
	{
		return;
	}

	Experience += Amount;
	UpdateLevel();
}

bool UEWPlayerState::CanPauseTime() const
{
	// 检查行动值是否足够
	if (GetActionPoint() < PauseTimeCost)
	{
		return false;
	}

	// 检查冷却时间
	UWorld* World = GetWorld();
	if (World)
	{
		float CurrentTime = World->GetTimeSeconds();
		if (CurrentTime - LastPauseTime < PauseTimeCooldown)
		{
			return false;
		}
	}

	return true;
}

AActor* UEWPlayerState::SpawnCharacter(UWorld* World, const FVector& Location, const FRotator& Rotation)
{
	AActor* Character = Super::SpawnCharacter(World, Location, Rotation);
	
	if (AEWCharacterBase* PlayerCharacter = Cast<AEWCharacterBase>(Character))
	{
		// 设置玩家状态的引用
		// 这里可以添加玩家角色的特殊初始化逻辑
	}

	return Character;
}

void UEWPlayerState::SyncStateToCharacter()
{
	Super::SyncStateToCharacter();

	if (AEWCharacterBase* PlayerCharacter = Cast<AEWCharacterBase>(GetCharacterActor()))
	{
		// 同步玩家特有的状态
		// 例如：单位管理器的引用等
		if (UEWUnitManager* CharacterUnitManager = PlayerCharacter->GetUnitManager())
		{
			// 同步单位管理器状态
		}
	}
}

void UEWPlayerState::SyncStateFromCharacter()
{
	Super::SyncStateFromCharacter();

	if (AEWCharacterBase* PlayerCharacter = Cast<AEWCharacterBase>(GetCharacterActor()))
	{
		// 从玩家角色同步状态
		// 例如：最后暂停时间等
		LastPauseTime = 0.0f; // 重置暂停时间
	}
}

void UEWPlayerState::UpdateLevel()
{
	if (ExperiencePerLevel <= 0.0f)
	{
		return;
	}

	int32 NewLevel = FMath::FloorToInt(Experience / ExperiencePerLevel) + 1;
	
	if (NewLevel != Level)
	{
		int32 OldLevel = Level;
		Level = NewLevel;
		
		UE_LOG(LogTemp, Log, TEXT("UEWPlayerState::UpdateLevel - Level up! %d -> %d"), OldLevel, Level);
		
		// 这里可以添加升级奖励逻辑
	}
}

void UEWPlayerState::OnUnitDeath(UEWBaseState* State)
{
	if (State)
	{
		UE_LOG(LogTemp, Log, TEXT("UEWPlayerState::OnUnitDeath - Unit died"));
		
		// 这里可以添加单位死亡的处理逻辑
		// 例如：经验奖励、复活倒计时等
	}
}
