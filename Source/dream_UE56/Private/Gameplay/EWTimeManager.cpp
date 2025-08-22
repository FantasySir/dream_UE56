// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/EWTimeManager.h"
#include "Character/EWCharacterBase.h"
#include "Character/EWUnitBase.h"
#include "Components/EWTimePauseImmuneComponent.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "EngineUtils.h"

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

	// 将时间暂停发起者添加到免疫列表
	ImmuneActors.AddUnique(Instigator);

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

	// 清理免疫列表中的时间暂停发起者
	if (TimePauseInstigator)
	{
		ImmuneActors.Remove(TimePauseInstigator);
		TimePauseInstigator = nullptr;
	}

	// 广播时间恢复事件
	OnTimePausedChanged.Broadcast(false);

	UE_LOG(LogTemp, Log, TEXT("Time resumed"));
}

void UEWTimeManager::RegisterTimeSensitiveActor(AActor* Actor)
{
	if (!Actor)
	{
		return;
	}

	TimeSensitiveActors.AddUnique(Actor);

	// 如果当前时间被暂停，立即应用暂停效果到新注册的Actor
	if (bIsTimePaused && !IsActorImmuneToTimePause(Actor))
	{
		// 保存原始时间缩放
		OriginalTimeDilations.Add(Actor, GetActorTimeDilation(Actor));
		SetActorTimeDilation(Actor, 0.0f);
	}
}

void UEWTimeManager::UnregisterTimeSensitiveActor(AActor* Actor)
{
	if (!Actor)
	{
		return;
	}

	TimeSensitiveActors.Remove(Actor);
	OriginalTimeDilations.Remove(Actor);
}

bool UEWTimeManager::IsActorImmuneToTimePause(AActor* Actor) const
{
	if (!Actor)
	{
		return false;
	}

	// 检查是否在免疫列表中
	for (const TWeakObjectPtr<AActor>& ImmuneActor : ImmuneActors)
	{
		if (ImmuneActor.IsValid() && ImmuneActor.Get() == Actor)
		{
			return true;
		}
	}

	// 检查是否有时间暂停免疫组件
	if (UEWTimePauseImmuneComponent* ImmuneComponent = Actor->FindComponentByClass<UEWTimePauseImmuneComponent>())
	{
		if (ImmuneComponent->IsTimePauseImmune())
		{
			return true;
		}
	}

	// 检查是否是玩家角色（默认免疫）
	if (Cast<AEWCharacterBase>(Actor))
	{
		return true;
	}

	return false;
}

void UEWTimeManager::ApplyTimePauseToActors()
{
	CleanupInvalidReferences();

	for (TWeakObjectPtr<AActor>& ActorPtr : TimeSensitiveActors)
	{
		if (ActorPtr.IsValid())
		{
			AActor* Actor = ActorPtr.Get();
			
			// 跳过免疫的Actor
			if (IsActorImmuneToTimePause(Actor))
			{
				continue;
			}

			// 保存原始时间缩放
			float OriginalDilation = GetActorTimeDilation(Actor);
			OriginalTimeDilations.Add(Actor, OriginalDilation);

			// 应用时间暂停
			SetActorTimeDilation(Actor, 0.0f);
		}
	}
}

void UEWTimeManager::RestoreTimeDilationToActors()
{
	CleanupInvalidReferences();

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
	// 清理时间敏感Actor列表中的无效引用
	TimeSensitiveActors.RemoveAll([](const TWeakObjectPtr<AActor>& ActorPtr)
	{
		return !ActorPtr.IsValid();
	});

	// 清理免疫Actor列表中的无效引用
	ImmuneActors.RemoveAll([](const TWeakObjectPtr<AActor>& ActorPtr)
	{
		return !ActorPtr.IsValid();
	});

	// 清理原始时间缩放映射中的无效引用
	for (auto It = OriginalTimeDilations.CreateIterator(); It; ++It)
	{
		if (!It.Key().IsValid())
		{
			It.RemoveCurrent();
		}
	}
}

void UEWTimeManager::OnWorldTick(UWorld* World, ELevelTick TickType, float DeltaSeconds)
{
	if (World != GetWorld())
	{
		return;
	}

	// 定期清理无效引用
	static float CleanupTimer = 0.0f;
	CleanupTimer += DeltaSeconds;
	if (CleanupTimer >= 5.0f) // 每5秒清理一次
	{
		CleanupInvalidReferences();
		CleanupTimer = 0.0f;
	}

	// 自动注册新的单位
	if (bIsTimePaused)
	{
		for (TActorIterator<AEWUnitBase> ActorItr(World); ActorItr; ++ActorItr)
		{
			AEWUnitBase* Unit = *ActorItr;
			if (Unit && !TimeSensitiveActors.Contains(Unit))
			{
				RegisterTimeSensitiveActor(Unit);
			}
		}
	}
}
