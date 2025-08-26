// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/EWCharacterBase.h"
#include "AbilitySystem/AttributeSets/EWBaseAttributeSet.h"
#include "AbilitySystem/AttributeSets/EWPlayerAttributeSet.h"
#include "AbilitySystem/AttributeSets/EWCombatAttributeSet.h"
#include "Character/EWUnitBase.h"
#include "GameState/EWPlayerState.h"
#include "Player/EWPlayerController.h"
#include "Player/EWUnitManager.h"
#include "Gameplay/EWTimeManager.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values
AEWCharacterBase::AEWCharacterBase()
{
 	PrimaryActorTick.bCanEverTick = true;

	// 创建单位管理器
	UnitManager = CreateDefaultSubobject<UEWUnitManager>("UnitManager");
}

void AEWCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 当被控制器占有时初始化能力系统Actor信息
	InitAbilityActorInfo();

	if (UnitManager)
	{
		UnitManager->Initialize(12, 4); // 默认12个总槽位，4个战斗槽位
	}
}

void AEWCharacterBase::InitAbilityActorInfo()
{
	AEWPlayerState* EWPlayerState = GetPlayerState<AEWPlayerState>();
	check(EWPlayerState);
	EWPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(EWPlayerState, this);
	//Cast<AbilitySystemComponent>(EWPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = EWPlayerState->GetAbilitySystemComponent();
	BaseAttributeSet = EWPlayerState->GetBaseAttributeSet();
	PlayerAttributeSet = EWPlayerState->GetPlayerAttributeSet();
	//OnAscRegistered.Broadcast(AbilitySystemComponent);
	//AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraCharacter::StunTagChanged);

	/**
	if (AEWPlayerController* EWPlayerController = Cast<AEWPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	**/
}

// Called every frame
void AEWCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 行动值自动回复
	if (AbilitySystemComponent && PlayerAttributeSet)
	{
		// 这里可以添加行动值自动回复逻辑
		// 需要通过GameplayEffect来实现属性修改
	}
}

// Called to bind functionality to input
void AEWCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UAbilitySystemComponent* AEWCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

//TODO: 百分比计算可以考虑后续移到UI中
float AEWCharacterBase::GetHealthPercentage() const
{
	if (BaseAttributeSet)
	{
		float MaxHealth = BaseAttributeSet->GetMaxHealth();
		if (MaxHealth > 0)
		{
			return BaseAttributeSet->GetHealth() / MaxHealth;
		}
	}
	return 0.0f;
}

float AEWCharacterBase::GetManaPercentage() const
{
	if (BaseAttributeSet)
	{
		float MaxMana = BaseAttributeSet->GetMaxMana();
		if (MaxMana > 0)
		{
			return BaseAttributeSet->GetMana() / MaxMana;
		}
	}
	return 0.0f;
}

bool AEWCharacterBase::IsAlive() const
{
	if (BaseAttributeSet)
	{
		return BaseAttributeSet->GetHealth() > 0.0f;
	}
	return false;
}

bool AEWCharacterBase::CanPauseTime() const
{
	// 检查行动值是否足够
	if (PlayerAttributeSet && PlayerAttributeSet->GetActionPoint() < PauseTimeCost)
	{
		return false;
	}

	// 检查冷却时间
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastPauseTime < PauseTimeCooldown)
	{
		return false;
	}

	return true;
}

void AEWCharacterBase::PauseTime()
{
	if (!CanPauseTime())
	{
		return;
	}

	LastPauseTime = GetWorld()->GetTimeSeconds();

	// 使用时间管理器暂停时间
	if (UEWTimeManager* TimeManager = GetTimeManager())
	{
		TimeManager->PauseTime(this);
	}

	// 这里需要消耗行动值，通过GE实现
	// TODO: 实现行动值消耗的GameplayEffect
}

void AEWCharacterBase::ResumeTime()
{
	// 使用时间管理器恢复时间
	if (UEWTimeManager* TimeManager = GetTimeManager())
	{
		TimeManager->ResumeTime();
	}
}

bool AEWCharacterBase::IsTimePaused() const
{
	if (UEWTimeManager* TimeManager = GetTimeManager())
	{
		return TimeManager->IsTimePaused();
	}
	return false;
}

UEWTimeManager* AEWCharacterBase::GetTimeManager() const
{
	if (UWorld* World = GetWorld())
	{
		return World->GetSubsystem<UEWTimeManager>();
	}
	return nullptr;
}

void AEWCharacterBase::LockTarget(AEWUnitBase* Target)
{
	if (Target && Target != static_cast<AActor*>(this))
	{
		LockedTarget = Target;
		// 这里可以添加锁定UI或相机行为
	}
}

void AEWCharacterBase::UnlockTarget()
{
	LockedTarget = nullptr;
	// 这里可以添加解除锁定的UI更新
}

bool AEWCharacterBase::CanSummonUnit(TSubclassOf<AEWUnitBase> UnitClass) const
{
	// 检查魔法值是否足够
	if (BaseAttributeSet)
	{
		return BaseAttributeSet->GetMana() >= 30.0f; // 临时数值，应该根据UnitClass来确定
	}
	return false;
}

AEWUnitBase* AEWCharacterBase::SummonUnit(TSubclassOf<AEWUnitBase> UnitClass, FVector SpawnLocation)
{
	if (!CanSummonUnit(UnitClass))
	{
		return nullptr;
	}

	UWorld* World = GetWorld();
	if (!World || !UnitClass)
	{
		return nullptr;
	}

	//TODO 1. 先创建 UnitState (数据层)
	// 2. 生成单位 (表现层)
	
	return nullptr;
}

//TODO: 这个函数基本上是废弃的，但是它赋予默认属性效果的实现值得参考，先放在这里
void AEWCharacterBase::InitializeAbilitySystem()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		// 应用默认属性效果
		if (DefaultAttributeEffect)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);
			if (SpecHandle.IsValid())
			{
				AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}

		// 给予起始能力
		for (TSubclassOf<UGameplayAbility>& StartupAbility : StartupAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, INDEX_NONE, this));
		}
	}
}