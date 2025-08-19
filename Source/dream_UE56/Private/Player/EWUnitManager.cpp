// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/EWUnitManager.h"
#include "Character/EWUnitBase.h"
#include "Character/EWUnitAttributeSet.h"
#include "Engine/World.h"

UEWUnitManager::UEWUnitManager()
{
	MaxTotalSlots = 12;
	MaxBattleSlots = 4;
	HealthRestoreCostPerPercent = 1.0f;
	ManaRestoreCostPerPercent = 0.5f;
}

void UEWUnitManager::Initialize(int32 TotalSlots, int32 MaxBattleSlotsCount)
{
	MaxTotalSlots = TotalSlots;
	MaxBattleSlots = MaxBattleSlotsCount;
	UnitSlots.SetNum(MaxTotalSlots);
	BattleSlots.SetNum(MaxBattleSlots);
}

bool UEWUnitManager::AddUnit(TSubclassOf<AEWUnitBase> UnitClass, int32 SlotIndex)
{
	if (SlotIndex < 0)
	{
		SlotIndex = FindEmptySlot();
	}
	if (!IsValidSlotIndex(SlotIndex) || !UnitClass)
	{
		return false;
	}
	UnitSlots[SlotIndex].UnitClass = UnitClass;
	UnitSlots[SlotIndex].HealthPercentage = 1.0f;
	UnitSlots[SlotIndex].ManaPercentage = 1.0f;
	UnitSlots[SlotIndex].SlotState = EUnitSlotState::Available;
	UnitSlots[SlotIndex].SlotIndex = SlotIndex;
	UnitSlots[SlotIndex].bUnlocked = true;
	BroadcastSlotChanged(SlotIndex);
	return true;
}

bool UEWUnitManager::RemoveUnit(int32 SlotIndex)
{
	if (!IsValidSlotIndex(SlotIndex)) return false;
	UnitSlots[SlotIndex] = FUnitSlotData();
	BroadcastSlotChanged(SlotIndex);
	return true;
}

FUnitSlotData UEWUnitManager::GetUnitSlot(int32 SlotIndex) const
{
	if (!IsValidSlotIndex(SlotIndex)) return FUnitSlotData();
	return UnitSlots[SlotIndex];
}

TArray<FUnitSlotData> UEWUnitManager::GetAvailableUnits() const
{
	TArray<FUnitSlotData> Result;
	for (const FUnitSlotData& Slot : UnitSlots)
	{
		if (Slot.SlotState == EUnitSlotState::Available && Slot.bUnlocked && Slot.UnitClass)
		{
			Result.Add(Slot);
		}
	}
	return Result;
}

bool UEWUnitManager::SetBattleSlot(int32 BattleSlotIndex, int32 UnitSlotIndex)
{
	if (!IsValidBattleSlotIndex(BattleSlotIndex) || !IsValidSlotIndex(UnitSlotIndex)) return false;
	BattleSlots[BattleSlotIndex] = UnitSlots[UnitSlotIndex];
	BattleSlots[BattleSlotIndex].SlotState = EUnitSlotState::InBattle;
	BroadcastBattleSlotChanged(BattleSlotIndex);
	return true;
}

bool UEWUnitManager::ClearBattleSlot(int32 BattleSlotIndex)
{
	if (!IsValidBattleSlotIndex(BattleSlotIndex)) return false;
	BattleSlots[BattleSlotIndex] = FUnitSlotData();
	BroadcastBattleSlotChanged(BattleSlotIndex);
	return true;
}

bool UEWUnitManager::SwapBattleSlots(int32 BattleSlotIndex1, int32 BattleSlotIndex2)
{
	if (!IsValidBattleSlotIndex(BattleSlotIndex1) || !IsValidBattleSlotIndex(BattleSlotIndex2)) return false;
	BattleSlots.Swap(BattleSlotIndex1, BattleSlotIndex2);
	BroadcastBattleSlotChanged(BattleSlotIndex1);
	BroadcastBattleSlotChanged(BattleSlotIndex2);
	return true;
}

void UEWUnitManager::ClearAllBattleSlots()
{
	for (int32 i = 0; i < BattleSlots.Num(); ++i)
	{
		BattleSlots[i] = FUnitSlotData();
		BroadcastBattleSlotChanged(i);
	}
}

float UEWUnitManager::CalculateSummonCost() const
{
	float TotalCost = 0.0f;
	for (const FUnitSlotData& Slot : BattleSlots)
	{
		if (Slot.UnitClass)
		{
			if (Slot.HealthPercentage < 1.0f)
			{
				TotalCost += (1.0f - Slot.HealthPercentage) * 100.0f * HealthRestoreCostPerPercent;
			}
			if (Slot.ManaPercentage < 1.0f)
			{
				TotalCost += (1.0f - Slot.ManaPercentage) * 100.0f * ManaRestoreCostPerPercent;
			}
		}
	}
	return TotalCost;
}

bool UEWUnitManager::CanSummonAll(float AvailableActionPoint) const
{
	return AvailableActionPoint >= CalculateSummonCost();
}

TArray<AEWUnitBase*> UEWUnitManager::SummonBattleUnits(UWorld* World, const TArray<FVector>& SpawnLocations, AActor* Owner)
{
	TArray<AEWUnitBase*> Result;
	SummonedUnits.Empty();
	for (int32 i = 0; i < BattleSlots.Num(); ++i)
	{
		const FUnitSlotData& Slot = BattleSlots[i];
		if (Slot.UnitClass && World)
		{
			FActorSpawnParameters Params;
			Params.Owner = Owner;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			FVector SpawnLoc = SpawnLocations.IsValidIndex(i) ? SpawnLocations[i] : FVector::ZeroVector;
			AEWUnitBase* Unit = World->SpawnActor<AEWUnitBase>(Slot.UnitClass, SpawnLoc, FRotator::ZeroRotator, Params);
			if (Unit)
			{
				// 设置血量和魔法值
				float MaxHealth = Unit->GetMaxHealth();
				float MaxMana = Unit->GetMaxMana();
				Unit->GetAttributeSet()->SetHealth(MaxHealth * Slot.HealthPercentage);
				Unit->GetAttributeSet()->SetMana(MaxMana * Slot.ManaPercentage);
				Result.Add(Unit);
				SummonedUnits.Add(Unit);
			}
		}
	}
	return Result;
}

void UEWUnitManager::UpdateUnitStatus(int32 SlotIndex, float HealthPercentage, float ManaPercentage)
{
	if (!IsValidSlotIndex(SlotIndex)) return;
	UnitSlots[SlotIndex].HealthPercentage = HealthPercentage;
	UnitSlots[SlotIndex].ManaPercentage = ManaPercentage;
	BroadcastSlotChanged(SlotIndex);
}

void UEWUnitManager::UpdateUnitFromActor(int32 SlotIndex, AEWUnitBase* UnitActor)
{
	if (!IsValidSlotIndex(SlotIndex) || !UnitActor) return;
	float HealthPct = UnitActor->GetHealthPercentage();
	float ManaPct = UnitActor->GetManaPercentage();
	UpdateUnitStatus(SlotIndex, HealthPct, ManaPct);
}

bool UEWUnitManager::UnlockSlot(int32 SlotIndex)
{
	if (!IsValidSlotIndex(SlotIndex)) return false;
	UnitSlots[SlotIndex].bUnlocked = true;
	BroadcastSlotChanged(SlotIndex);
	return true;
}

int32 UEWUnitManager::GetUnlockedSlotCount() const
{
	int32 Count = 0;
	for (const FUnitSlotData& Slot : UnitSlots)
	{
		if (Slot.bUnlocked)
		{
			++Count;
		}
	}
	return Count;
}

void UEWUnitManager::SaveToGameInstance() {}
void UEWUnitManager::LoadFromGameInstance() {}

TArray<AEWUnitBase*> UEWUnitManager::GetSummonedUnits() const
{
	TArray<AEWUnitBase*> ValidUnits;
	for (const TWeakObjectPtr<AEWUnitBase>& WeakUnit : SummonedUnits)
	{
		if (AEWUnitBase* Unit = WeakUnit.Get())
		{
			ValidUnits.Add(Unit);
		}
	}
	return ValidUnits;
}

bool UEWUnitManager::IsValidSlotIndex(int32 SlotIndex) const
{
	return SlotIndex >= 0 && SlotIndex < UnitSlots.Num();
}

bool UEWUnitManager::IsValidBattleSlotIndex(int32 BattleSlotIndex) const
{
	return BattleSlotIndex >= 0 && BattleSlotIndex < BattleSlots.Num();
}

int32 UEWUnitManager::FindEmptySlot() const
{
	for (int32 i = 0; i < UnitSlots.Num(); ++i)
	{
		if (!UnitSlots[i].UnitClass)
		{
			return i;
		}
	}
	return -1;
}

void UEWUnitManager::BroadcastSlotChanged(int32 SlotIndex)
{
	if (IsValidSlotIndex(SlotIndex))
	{
		OnUnitSlotChanged.Broadcast(SlotIndex, UnitSlots[SlotIndex]);
	}
}

void UEWUnitManager::BroadcastBattleSlotChanged(int32 BattleSlotIndex)
{
	if (IsValidBattleSlotIndex(BattleSlotIndex))
	{
		OnBattleSlotChanged.Broadcast(BattleSlotIndex, BattleSlots[BattleSlotIndex]);
	}
}
