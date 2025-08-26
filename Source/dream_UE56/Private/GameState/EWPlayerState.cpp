// Fill out your copyright notice in the Description page of Project Settings.

#include "GameState/EWPlayerState.h"
#include "GameState/EWUnitState.h"
#include "AbilitySystem/AttributeSets/EWBaseAttributeSet.h"
#include "AbilitySystem/AttributeSets/EWPlayerAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

AEWPlayerState::AEWPlayerState()
{
	SetNetUpdateFrequency(100.f);

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// 初始化单位列表大小 - 延迟到BeginPlay进行，避免构造函数阻塞
	BaseAttributeSet = CreateDefaultSubobject<UEWBaseAttributeSet>("BaseAttributeSet");
	PlayerAttributeSet = CreateDefaultSubobject<UEWPlayerAttributeSet>("PlayerAttributeSet");
	UnitList.SetNum(12); // 默认12个单位槽位
}


void AEWPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 等级经验复制
	DOREPLIFETIME(AEWPlayerState, Level);
	DOREPLIFETIME(AEWPlayerState, Experience);

	// 单位管理复制
	DOREPLIFETIME(AEWPlayerState, UnitList);
	DOREPLIFETIME(AEWPlayerState, TotalUnitCount);
	DOREPLIFETIME(AEWPlayerState, StartingUnitCount);
}

UAbilitySystemComponent* AEWPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// ===================== 等级经验接口实现 =====================

int32 AEWPlayerState::GetLevel_Implementation() const
{
	return Level;
}

void AEWPlayerState::SetLevel_Implementation(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level, true);
}

void AEWPlayerState::AddToLevel_Implementation(int32 InLevel)
{
	SetLevel_Implementation(Level + InLevel);
}

int32 AEWPlayerState::GetExperience_Implementation() const
{
	return Experience;
}

void AEWPlayerState::SetExperience_Implementation(int32 InExperience)
{
	Experience = InExperience;
	OnExperienceChangedDelegate.Broadcast(Experience);
}

void AEWPlayerState::AddToExperience_Implementation(int32 InExperience)
{
	SetExperience_Implementation(Experience + InExperience);
}

void AEWPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level, true);
}

void AEWPlayerState::OnRep_Experience(int32 OldExperience)
{
	OnExperienceChangedDelegate.Broadcast(Experience);
}

void AEWPlayerState::SetUnitList(const TArray<AEWUnitState*>& NewUnitList)
{
	UnitList = NewUnitList;
	
	// 确保列表大小符合设定
	UnitList.SetNum(TotalUnitCount);
	
	// 广播单位列表变化事件
	OnUnitListChanged.Broadcast(UnitList, EUnitListOperationType::SetList, -1);
}


TArray<AEWUnitState*> AEWPlayerState::GetStartingUnits() const
{
	// 获取首发单位列表
	TArray<AEWUnitState*> StartingUnits;
	
	for (int32 i = 0; i < FMath::Min(StartingUnitCount, UnitList.Num()); ++i)
	{
		StartingUnits.Add(UnitList[i]);
	}
	
	return StartingUnits;
}

TArray<AEWUnitState*> AEWPlayerState::GetBenchUnits() const
{
	// 获取替补单位列表
	TArray<AEWUnitState*> BenchUnits;
	
	for (int32 i = StartingUnitCount; i < UnitList.Num(); ++i)
	{
		BenchUnits.Add(UnitList[i]);
	}
	
	return BenchUnits;
}

// ===================== 只读访问函数 =====================

AEWUnitState* AEWPlayerState::GetUnitAtIndex(int32 Index) const
{
	if (Index >= 0 && Index < UnitList.Num())
	{
		return UnitList[Index];
	}
	return nullptr;
}

// ===================== 客户端请求包装器 =====================

void AEWPlayerState::RequestAddUnit(AEWUnitState* Unit, int32 Index)
{
	if (HasAuthority())
	{
		// 在服务器上直接执行
		Server_AddUnit(Unit, Index);
	}
	else
	{
		// 在客户端上发送RPC请求
		Server_AddUnit(Unit, Index);
	}
}

void AEWPlayerState::RequestRemoveUnit(int32 Index)
{
	if (HasAuthority())
	{
		// 在服务器上直接执行
		Server_RemoveUnit(Index);
	}
	else
	{
		// 在客户端上发送RPC请求
		Server_RemoveUnit(Index);
	}
}

void AEWPlayerState::RequestSwapUnits(int32 IndexA, int32 IndexB)
{
	if (HasAuthority())
	{
		// 在服务器上直接执行
		Server_SwapUnits(IndexA, IndexB);
	}
	else
	{
		// 在客户端上发送RPC请求
		Server_SwapUnits(IndexA, IndexB);
	}
}

// ===================== 服务器端RPC函数 =====================

void AEWPlayerState::Server_AddUnit_Implementation(AEWUnitState* Unit, int32 Index)
{
	// 服务器端执行内部实现
	AddUnitInternal(Unit, Index);
}

bool AEWPlayerState::Server_AddUnit_Validate(AEWUnitState* Unit, int32 Index)
{
	// 基本验证：单位不为空，索引在合理范围内
	return Unit != nullptr && Index >= -1 && Index < UnitList.Num();
}

void AEWPlayerState::Server_RemoveUnit_Implementation(int32 Index)
{
	// 服务器端执行内部实现
	RemoveUnitInternal(Index);
}

bool AEWPlayerState::Server_RemoveUnit_Validate(int32 Index)
{
	// 验证索引有效性
	return Index >= 0 && Index < UnitList.Num();
}

void AEWPlayerState::Server_SwapUnits_Implementation(int32 IndexA, int32 IndexB)
{
	// 服务器端执行内部实现
	SwapUnitsInternal(IndexA, IndexB);
}

bool AEWPlayerState::Server_SwapUnits_Validate(int32 IndexA, int32 IndexB)
{
	// 验证两个索引都有效
	return IndexA >= 0 && IndexA < UnitList.Num() && 
	       IndexB >= 0 && IndexB < UnitList.Num();
}

// ===================== 统一的智能单位操作接口 =====================

void AEWPlayerState::AddUnit(AEWUnitState* Unit, int32 Index)
{
	// 自动判断网络环境
	if (GetWorld() && GetWorld()->GetNetMode() != NM_Standalone)
	{
		// 多人游戏环境，使用网络安全版本
		if (HasAuthority())
		{
			// 服务器端直接执行
			Server_AddUnit(Unit, Index);
		}
		else
		{
			// 客户端发送RPC请求
			Server_AddUnit(Unit, Index);
		}
	}
	else
	{
		// 单机游戏，直接操作
		AddUnitInternal(Unit, Index);
	}
}

void AEWPlayerState::RemoveUnit(int32 Index)
{
	// 自动判断网络环境
	if (GetWorld() && GetWorld()->GetNetMode() != NM_Standalone)
	{
		// 多人游戏环境，使用网络安全版本
		if (HasAuthority())
		{
			// 服务器端直接执行
			Server_RemoveUnit(Index);
		}
		else
		{
			// 客户端发送RPC请求
			Server_RemoveUnit(Index);
		}
	}
	else
	{
		// 单机游戏，直接操作
		RemoveUnitInternal(Index);
	}
}

void AEWPlayerState::SwapUnits(int32 IndexA, int32 IndexB)
{
	// 自动判断网络环境
	if (GetWorld() && GetWorld()->GetNetMode() != NM_Standalone)
	{
		// 多人游戏环境，使用网络安全版本
		if (HasAuthority())
		{
			// 服务器端直接执行
			Server_SwapUnits(IndexA, IndexB);
		}
		else
		{
			// 客户端发送RPC请求
			Server_SwapUnits(IndexA, IndexB);
		}
	}
	else
	{
		// 单机游戏，直接操作
		SwapUnitsInternal(IndexA, IndexB);
	}
}

// ===================== 内部实现函数（直接操作） =====================

void AEWPlayerState::AddUnitInternal(AEWUnitState* Unit, int32 Index)
{
	if (!Unit)
	{
		return;
	}
	
	// 如果没有指定索引，找到第一个空位
	if (Index == -1)
	{
		for (int32 i = 0; i < UnitList.Num(); ++i)
		{
			if (!UnitList[i])
			{
				Index = i;
				break;
			}
		}
	}
	
	// 检查索引有效性
	if (Index < 0 || Index >= UnitList.Num())
	{
		return;
	}
	
	UnitList[Index] = Unit;
	
	// 广播添加单位事件
	OnUnitListChanged.Broadcast(UnitList, EUnitListOperationType::AddUnit, Index);
}

void AEWPlayerState::RemoveUnitInternal(int32 Index)
{
	// 检查索引有效性
	if (Index < 0 || Index >= UnitList.Num())
	{
		return;
	}
	
	UnitList[Index] = nullptr;
	
	// 广播移除单位事件
	OnUnitListChanged.Broadcast(UnitList, EUnitListOperationType::RemoveUnit, Index);
}

void AEWPlayerState::SwapUnitsInternal(int32 IndexA, int32 IndexB)
{
	// 检查索引有效性
	if (IndexA < 0 || IndexA >= UnitList.Num() || IndexB < 0 || IndexB >= UnitList.Num())
	{
		return;
	}
	
	// 交换单位
	AEWUnitState* TempUnit = UnitList[IndexA];
	UnitList[IndexA] = UnitList[IndexB];
	UnitList[IndexB] = TempUnit;
	
	// 广播交换单位事件（使用较小的索引作为操作索引）
	int32 OperationIndex = FMath::Min(IndexA, IndexB);
	OnUnitListChanged.Broadcast(UnitList, EUnitListOperationType::SwapUnit, OperationIndex);
}
