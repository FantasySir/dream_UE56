// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/EWCharacterBase.h"
#include "AbilitySystem/EWAbilitySystemLibrary.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Perception/AISense_Sight.h"
#include "AbilitySystem/AttributeSets/EWBaseAttributeSet.h"
#include "Gameplay/EWTimeManager.h"



// Sets default values
AEWCharacterBase::AEWCharacterBase()
{
 	PrimaryActorTick.bCanEverTick = true;
	//初始化刺激源组件
	StimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("StimuliSourceComponent");
	
	// 默认玩家角色免疫时间暂停
	bImmuneTimePause = true;
}

void AEWCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEWCharacterBase, bIsStunned);
	DOREPLIFETIME(AEWCharacterBase, bIsSilenced);
	DOREPLIFETIME(AEWCharacterBase, bImmuneTimePause);
}

float AEWCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float DamageTaken = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	OnDamageDelegate.Broadcast(DamageTaken);
	return DamageTaken;
}

UAbilitySystemComponent* AEWCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

bool AEWCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

void AEWCharacterBase::InitAbilityActorInfo()
{
}

void AEWCharacterBase::SetupPerceptionStimuli()
{
	if (StimuliSourceComponent)
	{
		// 将此角色注册为视觉刺激源
		StimuliSourceComponent->RegisterForSense(UAISense_Sight::StaticClass());
		StimuliSourceComponent->RegisterWithPerceptionSystem();
	}
}

void AEWCharacterBase::OnTimePaused_Implementation()
{
	// 子类可以重写此方法来处理时间暂停事件
	UE_LOG(LogTemp, Log, TEXT("%s: Time paused"), *GetName());
}

void AEWCharacterBase::OnTimeResumed_Implementation()
{
	// 子类可以重写此方法来处理时间恢复事件
	UE_LOG(LogTemp, Log, TEXT("%s: Time resumed"), *GetName());
}

void AEWCharacterBase::OnTimePauseImmuneStatusChanged_Implementation(bool bIsImmune)
{
	if (UEWTimeManager* TimeManager = UEWAbilitySystemLibrary::GetTimeManager(this))
	{
		TimeManager->OnActorTimePauseImmuneStatusChanged(this, bIsImmune);
		UE_LOG(LogTemp, Log, TEXT("%s: Time pause immune status changed to %s"), *GetName(), bIsImmune ? TEXT("true") : TEXT("false"));
	}
	return;
	}

void AEWCharacterBase::OnBeforeDestroy_Implementation()
{
	// 通知时间管理器此Actor即将被销毁
	if (UWorld* World = GetWorld())
	{
		if (UEWTimeManager* TimeManager = World->GetSubsystem<UEWTimeManager>())
		{
			TimeManager->UnregisterTimeAffectedActor(this);
		}
	}
}

void AEWCharacterBase::SetTimePauseImmune(bool bImmune)
{
	if (bImmuneTimePause != bImmune)
	{
		bImmuneTimePause = bImmune;

		// 调用接口方法
		OnTimePauseImmuneStatusChanged_Implementation(bImmuneTimePause);	
		// 广播事件
		OnTimePauseImmuneChanged.Broadcast(this, bImmuneTimePause);
	}
}

void AEWCharacterBase::TimeManagerRegistered()
{
	if (UEWTimeManager* TimeManager = UEWAbilitySystemLibrary::GetTimeManager(this))
	{
		TimeManager->RegisterTimeAffectedActor(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TimeManager not found when Character %s is created."), *GetName());
	}
	return;
}

void AEWCharacterBase::BeginDestroy()
{
	// 在销毁前调用OnBeforeDestroy来清理时间管理器引用
	OnBeforeDestroy_Implementation();
	
	Super::BeginDestroy();
}