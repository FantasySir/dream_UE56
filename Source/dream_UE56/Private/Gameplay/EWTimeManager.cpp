// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/EWTimeManager.h"
#include "Character/EWCharacter.h"
#include "Character/EWCharacterBase.h"
#include "Character/EWUnitBase.h"
#include "Components/EWTimePauseImmuneComponent.h"
#include "Interfaces/TimePauseInterface.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "EngineUtils.h"

// FTimePauseActorGroup 实现
void FTimePauseActorGroup::CleanupInvalidReferences()
{
	// 清理免疫玩家列表
	ImmunePlayers.RemoveAll([](const TWeakObjectPtr<AEWCharacterBase>& ActorPtr)
	{
		return !ActorPtr.IsValid();
	});

	// 清理敏感单位列表
	SensitiveUnits.RemoveAll([](const TWeakObjectPtr<AEWUnitBase>& ActorPtr)
	{
		return !ActorPtr.IsValid();
	});

	// 清理免疫单位列表
	ImmuneUnits.RemoveAll([](const TWeakObjectPtr<AEWUnitBase>& ActorPtr)
	{
		return !ActorPtr.IsValid();
	});

	// 清理敏感Actor列表
	SensitiveActors.RemoveAll([](const TWeakObjectPtr<AActor>& ActorPtr)
	{
		return !ActorPtr.IsValid();
	});

	// 清理免疫Actor列表
	ImmuneActors.RemoveAll([](const TWeakObjectPtr<AActor>& ActorPtr)
	{
		return !ActorPtr.IsValid();
	});
}

int32 FTimePauseActorGroup::GetSensitiveActorCount() const
{
	return SensitiveUnits.Num() + SensitiveActors.Num();
}

int32 FTimePauseActorGroup::GetImmuneActorCount() const
{
	return ImmunePlayers.Num() + ImmuneUnits.Num() + ImmuneActors.Num();
}

UEWTimeManager::UEWTimeManager()
{
	bIsTimePaused = false;
	TimePauseInstigator = nullptr;
}

void UEWTimeManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// 绑定世界Tick事件
	if (UWorld* World = GetWorld())
	{
		TickDelegateHandle = FWorldDelegates::OnWorldTickStart.AddUObject(this, &UEWTimeManager::OnWorldTick);
	}
}

void UEWTimeManager::Deinitialize()
{
	// 解绑世界Tick事件
	if (TickDelegateHandle.IsValid())
	{
		FWorldDelegates::OnWorldTickStart.Remove(TickDelegateHandle);
		TickDelegateHandle.Reset();
	}

	// 如果时间被暂停，恢复时间
	if (bIsTimePaused)
	{
		ResumeTime();
	}

	Super::Deinitialize();
}

void UEWTimeManager::PauseTime(AEWCharacterBase* Instigator)
{
	if (bIsTimePaused || !Instigator)
	{
		return;
	}

	bIsTimePaused = true;
	TimePauseInstigator = Instigator;

	// 应用时间暂停效果
	ApplyTimePauseToActors();

	// 广播时间暂停事件
	OnTimePausedChanged.Broadcast(true);

	UE_LOG(LogTemp, Log, TEXT("Time paused by %s"), *Instigator->GetName());
}

void UEWTimeManager::ResumeTime()
{
	if (!bIsTimePaused)
	{
		return;
	}

	bIsTimePaused = false;

	// 恢复所有Actor的时间缩放
	RestoreTimeDilationToActors();

	// 广播时间恢复事件
	OnTimePausedChanged.Broadcast(false);

	UE_LOG(LogTemp, Log, TEXT("Time resumed"));
}

void UEWTimeManager::RegisterTimeAffectedActor(AActor* Actor)
{
	if (!Actor)
	{
		return;
	}

	// 根据类型分类注册
	if (AEWCharacterBase* Character = Cast<AEWCharacterBase>(Actor))
	{
		RegisterCharacterBase(Character);
	}
	else
	{
		RegisterOtherActors(Actor);
	}
}

void UEWTimeManager::RegisterCharacterBase(AEWCharacterBase* Character)
{
	if (!Character)
	{
		return;
	}

	bool bIsImmune = false;

	// 优先检查是否实现了TimePauseInterface接口
	if (ITimePauseInterface* TimePauseInterface = Cast<ITimePauseInterface>(Character))
	{
		bIsImmune = TimePauseInterface->IsImmuneToTimePause_Implementation();
	}
	else
	{
		// 如果没有接口，使用Character自身的属性(兜底逻辑，理论上不应该走到这里，因为所有Character都实现了接口，输出一个Log来记录问题)
		UE_LOG(LogTemp, Warning, TEXT("Character %s does not implement ITimePauseInterface, using default immune status."), *Character->GetName());
		bIsImmune = Character->GetTimePauseImmune();
	}

	// 根据Character类型和免疫状态分类
	if (AEWUnitBase* Unit = Cast<AEWUnitBase>(Character))
	{
		// 这是一个Unit
		if (bIsImmune)
		{
			ActorGroups.ImmuneUnits.AddUnique(Unit);
		}
		else
		{
			ActorGroups.SensitiveUnits.AddUnique(Unit);
			
			// 如果当前时间被暂停，立即应用暂停效果
			if (bIsTimePaused)
			{
				OriginalTimeDilations.Add(Unit, GetActorTimeDilation(Unit));
				SetActorTimeDilation(Unit, 0.0f);
			}
		}
	}
	else
	{
		// 这是一个普通Character(Player)
		if (bIsImmune)
		{
			ActorGroups.ImmunePlayers.AddUnique(Character);
		}
		else
		{
			// 理论上玩家不应该被时间暂停，但提供选项
			ActorGroups.SensitiveActors.AddUnique(Character);
			
			if (bIsTimePaused)
			{
				OriginalTimeDilations.Add(Character, GetActorTimeDilation(Character));
				SetActorTimeDilation(Character, 0.0f);
			}
		}
	}
}

void UEWTimeManager::RegisterOtherActors(AActor* Actor)
{
	if (!Actor)
	{
		return;
	}

	bool bIsImmune = false;

	// 检查是否实现了TimePauseInterface接口
	if (ITimePauseInterface* TimePauseInterface = Cast<ITimePauseInterface>(Actor))
	{
		bIsImmune = TimePauseInterface->IsImmuneToTimePause_Implementation();
	}
	else
	{
		// 如果没有接口，检查是否有免疫组件
		if (UEWTimePauseImmuneComponent* ImmuneComponent = Actor->FindComponentByClass<UEWTimePauseImmuneComponent>())
		{
			bIsImmune = ImmuneComponent->IsTimePauseImmune();
		}
		else
		{
			// 没有接口也没有组件，默认不免疫（不注册）
			return;
		}
	}

	// 根据免疫状态分类
	if (bIsImmune)
	{
		ActorGroups.ImmuneActors.AddUnique(Actor);
	}
	else
	{
		ActorGroups.SensitiveActors.AddUnique(Actor);
		
		// 如果当前时间被暂停，立即应用暂停效果
		if (bIsTimePaused)
		{
			OriginalTimeDilations.Add(Actor, GetActorTimeDilation(Actor));
			SetActorTimeDilation(Actor, 0.0f);
		}
	}
}

void UEWTimeManager::UnregisterTimeAffectedActor(AActor* Actor)
{
	if(!Actor)
	{
		return;
	}

	// 从新的分类列表中移除
	if(AEWCharacterBase* Character = Cast<AEWCharacterBase>(Actor))
	{
		if(ITimePauseInterface* TimePauseInterface = Cast<ITimePauseInterface>(Character))
		{
			if(TimePauseInterface->IsImmuneToTimePause_Implementation())
			{
				ActorGroups.ImmunePlayers.Remove(Character);
			}
			else
			{
				ActorGroups.SensitiveActors.Remove(Character);
			}
		}
		else
		{
			// 兜底逻辑，理论上不应该走到这里，因为所有Character都实现了接口，输出一个Log来记录问题
			UE_LOG(LogTemp, Warning, TEXT("Character %s does not implement ITimePauseInterface, removing from sensitive list by default."), *Character->GetName());
			ActorGroups.SensitiveActors.Remove(Character);
		}
	}
	else if(AEWUnitBase* Unit = Cast<AEWUnitBase>(Actor))
	{
		if(ITimePauseInterface* TimePauseInterface = Cast<ITimePauseInterface>(Unit))
		{
			if(TimePauseInterface->IsImmuneToTimePause_Implementation())
			{
				ActorGroups.ImmuneUnits.Remove(Unit);
			}
			else
			{
				ActorGroups.SensitiveUnits.Remove(Unit);
			}
		}
		else
		{
			// 兜底逻辑，理论上不应该走到这里，因为Unit都实现了接口，输出一个Log来记录问题
			UE_LOG(LogTemp, Warning, TEXT("Unit %s does not implement ITimePauseInterface, removing from sensitive list by default."), *Unit->GetName());
			ActorGroups.SensitiveUnits.Remove(Unit);
		}
	}
	else
	{
		if(ITimePauseInterface* TimePauseInterface = Cast<ITimePauseInterface>(Actor))
		{
			if(TimePauseInterface->IsImmuneToTimePause_Implementation())
			{
				ActorGroups.ImmuneActors.Remove(Actor);
			}
			else
			{
				ActorGroups.SensitiveActors.Remove(Actor);
			}
		}
		else
		{
			// 兜底逻辑，理论上不应该走到这里，因为其他Actor要么实现接口要么有组件，输出一个Log来记录问题
			UE_LOG(LogTemp, Warning, TEXT("Actor %s does not implement ITimePauseInterface and has no immune component, removing from sensitive list by default."), *Actor->GetName());
			ActorGroups.SensitiveActors.Remove(Actor);
		}
	}
	OriginalTimeDilations.Remove(Actor);
}

void UEWTimeManager::ApplyTimePauseToActors()
{

	// 处理敏感单位
	for (TWeakObjectPtr<AEWUnitBase>& UnitPtr : ActorGroups.SensitiveUnits)
	{
		if (UnitPtr.IsValid())
		{
			AEWUnitBase* Unit = UnitPtr.Get();
			if(ITimePauseInterface* TimePauseInterface = Cast<ITimePauseInterface>(Unit))
			{
				// 使用接口方法检查免疫状态
				if (!TimePauseInterface->IsImmuneToTimePause_Implementation())
				{
					float OriginalDilation = GetActorTimeDilation(Unit);
					OriginalTimeDilations.Add(Unit, OriginalDilation);
					SetActorTimeDilation(Unit, 0.0f);
				}
			}
			else
			{
				// 兜底逻辑，理论上不应该走到这里，因为Unit都实现了接口，输出一个Log来记录问题
				UE_LOG(LogTemp, Warning, TEXT("Unit %s does not implement ITimePauseInterface during ApplyTimePauseToActors, skipping. FUNC::UEWTimeManager::ApplyTimePauseToActors"), *Unit->GetName());
				continue;
			}
		}
	}

	// 处理敏感Actor
	for (TWeakObjectPtr<AActor>& ActorPtr : ActorGroups.SensitiveActors)
	{
		if (ActorPtr.IsValid())
		{
			AActor* Actor = ActorPtr.Get();
			if(ITimePauseInterface* TimePauseInterface = Cast<ITimePauseInterface>(Actor))
			{
				// 使用接口方法检查免疫状态
				if (!TimePauseInterface->IsImmuneToTimePause_Implementation())
				{
					float OriginalDilation = GetActorTimeDilation(Actor);
					OriginalTimeDilations.Add(Actor, OriginalDilation);
					SetActorTimeDilation(Actor, 0.0f);
				}
			}
			else
			{
				// 兜底逻辑，理论上不应该走到这里输出一个Log来记录问题
				UE_LOG(LogTemp, Warning, TEXT("Actor %s does not implement ITimePauseInterface during ApplyTimePauseToActors, skipping. FUNC::UEWTimeManager::ApplyTimePauseToActors"), *Actor->GetName());
				continue;
			}
		}
	}
}

void UEWTimeManager::RestoreTimeDilationToActors()
{

	for (auto& Pair : OriginalTimeDilations)
	{
		if (Pair.Key.IsValid())
		{
			AActor* Actor = Pair.Key.Get();
			float OriginalDilation = Pair.Value;
			
			SetActorTimeDilation(Actor, OriginalDilation);
		}
	}

	OriginalTimeDilations.Empty();
}

void UEWTimeManager::SetActorTimeDilation(AActor* Actor, float TimeDilation)
{
	if (!Actor)
	{
		return;
	}

	// 设置Actor的时间缩放
	Actor->CustomTimeDilation = TimeDilation;

	// 如果是Character，还需要设置其组件的时间缩放
	if (ACharacter* Character = Cast<ACharacter>(Actor))
	{
		if (USkeletalMeshComponent* Mesh = Character->GetMesh())
		{
			Mesh->bPauseAnims = (TimeDilation == 0.0f);
		}
	}

	// 暂停/恢复Actor的Tick
	Actor->SetActorTickEnabled(TimeDilation > 0.0f);

	// 如果Actor实现了时间暂停接口，调用相应的方法
	if (ITimePauseInterface* TimePauseInterface = Cast<ITimePauseInterface>(Actor))
	{
		if (TimeDilation == 0.0f)
		{
			TimePauseInterface->OnTimePaused_Implementation();
		}
		else
		{
			TimePauseInterface->OnTimeResumed_Implementation();
		}
	}
}

float UEWTimeManager::GetActorTimeDilation(AActor* Actor) const
{
	if (!Actor)
	{
		return 1.0f;
	}

	return Actor->CustomTimeDilation;
}

void UEWTimeManager::CleanupInvalidReferences()
{
	// 清理新的分类列表
	ActorGroups.CleanupInvalidReferences();

	// 清理原始时间缩放映射中的无效引用
	for (auto It = OriginalTimeDilations.CreateIterator(); It; ++It)
	{
		if (!It.Key().IsValid())
		{
			It.RemoveCurrent();
		}
	}
}

void UEWTimeManager::OnActorTimePauseImmuneStatusChanged(AActor* Actor, bool bIsImmune)
{
	if (!Actor)
	{
		return;
	}

	if (bIsImmune)
	{
		// 从敏感列表移除，添加到免疫列表
		if (AEWCharacterBase* Character = Cast<AEWCharacterBase>(Actor))
		{
			// 玩家角色通常不会在敏感列表中，但为了完整性处理
			ActorGroups.SensitiveActors.Remove(Character);
			ActorGroups.ImmunePlayers.AddUnique(Character);
		}
		else if (AEWUnitBase* Unit = Cast<AEWUnitBase>(Actor))
		{
			ActorGroups.SensitiveUnits.Remove(Unit);
			ActorGroups.ImmuneUnits.AddUnique(Unit);
		}
		else
		{
			ActorGroups.SensitiveActors.Remove(Actor);
			ActorGroups.ImmuneActors.AddUnique(Actor);
		}
		
		// 如果当前时间被暂停，恢复此Actor的时间
		if (bIsTimePaused)
		{
			if (OriginalTimeDilations.Contains(Actor))
			{
				float OriginalDilation = OriginalTimeDilations[Actor];
				SetActorTimeDilation(Actor, OriginalDilation);
				OriginalTimeDilations.Remove(Actor);
			}
		}
	}
	else
	{
		// 从免疫列表移除，添加到敏感列表
		if (AEWCharacterBase* Character = Cast<AEWCharacterBase>(Actor))
		{
			ActorGroups.ImmunePlayers.Remove(Character);
			ActorGroups.SensitiveActors.AddUnique(Character); // 特殊情况：玩家变为敏感
		}
		else if (AEWUnitBase* Unit = Cast<AEWUnitBase>(Actor))
		{
			ActorGroups.ImmuneUnits.Remove(Unit);
			ActorGroups.SensitiveUnits.AddUnique(Unit);
		}
		else
		{
			ActorGroups.ImmuneActors.Remove(Actor);
			ActorGroups.SensitiveActors.AddUnique(Actor);
		}
		
		// 如果当前时间被暂停，立即暂停此Actor
		if (bIsTimePaused)
		{
			OriginalTimeDilations.Add(Actor, GetActorTimeDilation(Actor));
			SetActorTimeDilation(Actor, 0.0f);
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("Actor %s time pause immune status changed to %s"), 
		*Actor->GetName(), bIsImmune ? TEXT("immune") : TEXT("sensitive"));
}

int32 UEWTimeManager::GetTotalSensitiveActorCount() const
{
	return ActorGroups.GetSensitiveActorCount();
}

int32 UEWTimeManager::GetTotalImmuneActorCount() const
{
	return ActorGroups.GetImmuneActorCount();
}


void UEWTimeManager::OnWorldTick(UWorld* World, ELevelTick TickType, float DeltaSeconds)
{
	if (World != GetWorld())
	{
		return;
	}
}