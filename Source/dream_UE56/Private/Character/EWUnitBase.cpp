// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/EWUnitBase.h"
#include "AbilitySystem/AttributeSets/EWBaseAttributeSet.h"
#include "AbilitySystem/AttributeSets/EWCombatAttributeSet.h"
#include "Data/EWUnitData.h"
#include "GameState/EWUnitState.h"
#include "Gameplay/EWTimeManager.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayEffect.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BrainComponent.h"
#include "Engine/World.h"

// Sets default values
AEWUnitBase::AEWUnitBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEWUnitBase::BeginPlay()
{
	Super::BeginPlay();
	// 注册到时间管理系统
	if (UWorld* World = GetWorld())
	{
		if (UEWTimeManager* TimeManager = World->GetSubsystem<UEWTimeManager>())
		{
			TimeManager->RegisterTimeSensitiveActor(this);
		}
	}

	check(UnitState);
	InitAbilityActorInfo();
}

void AEWUnitBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	check(UnitState);
	StartAI();
}

// Called when the game ends or when destroyed
void AEWUnitBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 从时间管理系统注销
	if (UWorld* World = GetWorld())
	{
		if (UEWTimeManager* TimeManager = World->GetSubsystem<UEWTimeManager>())
		{
			TimeManager->UnregisterTimeSensitiveActor(this);
		}
	}

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AEWUnitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 检查魔法值是否满了，如果满了则释放技能
	/**
	if (CanCastSkill() && GetManaPercentage() >= 1.0f)
	{
		CastRandomSkill();
	}
	**/
}

// Called to bind functionality to input
void AEWUnitBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UAbilitySystemComponent* AEWUnitBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AEWUnitBase::InitAbilityActorInfo()
{
	AEWUnitState* EWUnitState = GetPlayerState<AEWUnitState>();
	check(EWUnitState);
	EWUnitState->GetAbilitySystemComponent()->InitAbilityActorInfo(EWUnitState, this);
	AbilitySystemComponent = EWUnitState->GetAbilitySystemComponent();
	BaseAttributeSet = EWUnitState->GetBaseAttributeSet();
	UnitAttributeSet = EWUnitState->GetUnitAttributeSet();

}

float AEWUnitBase::GetHealthPercentage() const
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

float AEWUnitBase::GetManaPercentage() const
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

bool AEWUnitBase::IsAlive() const
{
	if (BaseAttributeSet)
	{
		return BaseAttributeSet->GetHealth() > 0.0f && CurrentState != EUnitState::Dead;
	}
	return false;
}

//是否敌对
bool AEWUnitBase::IsHostileTo(AEWUnitBase* OtherUnit) const
{
	if (!OtherUnit)
	{
		return false;
	}
	EUnitFaction OtherFaction = OtherUnit->GetFaction();
	if (Faction == EUnitFaction::Player && OtherFaction == EUnitFaction::Enemy)
		return true;
	if (Faction == EUnitFaction::Enemy && OtherFaction == EUnitFaction::Player)
		return true;
	return false;
}

//是否友善
bool AEWUnitBase::IsFriendlyTo(AEWUnitBase* OtherUnit) const
{
	if (!OtherUnit)
	{
		return false;
	}
	EUnitFaction OtherFaction = OtherUnit->GetFaction();
	if (Faction == OtherFaction)
		return true;
	if (Faction == EUnitFaction::Neutral || OtherFaction == EUnitFaction::Neutral)
		return true;
	return false;
}

void AEWUnitBase::SetCurrentState(EUnitState NewState)
{
	if (CurrentState != NewState)
	{
		EUnitState OldState = CurrentState;
		CurrentState = NewState;
		if (BlackboardComponent)
		{
			BlackboardComponent->SetValueAsEnum(FName("UnitState"), static_cast<uint8>(NewState));
		}
		if (NewState == EUnitState::Dead)
		{
			HandleDeath();
		}
	}
}

void AEWUnitBase::StartAI()
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (BehaviorTreeAsset && BlackboardAsset)
		{
			AIController->GetBlackboardComponent()->InitializeBlackboard(*BlackboardAsset);
			AIController->RunBehaviorTree(BehaviorTreeAsset);
		}
	}
}

void AEWUnitBase::StopAI()
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->GetBrainComponent()->StopLogic(FString("Manually Stopped"));
	}
}

void AEWUnitBase::AttackTarget(AEWUnitBase* Target)
{
	if (!CanAttack(Target))
		return;
	LastAttackTime = GetWorld()->GetTimeSeconds();
	float DamageAmount = CombatAttributeSet ? CombatAttributeSet->GetPhysicalAttack() : 10.0f;
	Target->TakeDamageFromUnit(DamageAmount, this);
	if (BaseAttributeSet)
	{
		float CurrentMana = BaseAttributeSet->GetMana();
		float MaxMana = BaseAttributeSet->GetMaxMana();
		float ManaGain = 10.0f;
		float NewMana = FMath::Min(CurrentMana + ManaGain, MaxMana);
		const_cast<UEWBaseAttributeSet*>(BaseAttributeSet)->SetMana(NewMana);
	}
}

bool AEWUnitBase::CanAttack(AEWUnitBase* Target) const
{
	if (!Target || !IsAlive() || !Target->IsAlive())
		return false;
	if (!IsHostileTo(Target))
		return false;
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastAttackTime < AttackCooldown)
		return false;
	float Distance = FVector::Dist(GetActorLocation(), Target->GetActorLocation());
	if (Distance > AttackRange)
		return false;
	return true;
}

bool AEWUnitBase::CanCastSkill() const
{
	return IsAlive() && StartupAbilities.Num() > 0;
}

void AEWUnitBase::CastRandomSkill()
{
	if (!CanCastSkill() || !AbilitySystemComponent)
		return;
	int32 RandomIndex = FMath::RandRange(0, StartupAbilities.Num() - 1);
	TSubclassOf<UGameplayAbility> SelectedAbility = StartupAbilities[RandomIndex];
	if (SelectedAbility)
	{
		FGameplayAbilitySpec* AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromClass(SelectedAbility);
		if (AbilitySpec)
		{
			AbilitySystemComponent->TryActivateAbility(AbilitySpec->Handle);
			if (BaseAttributeSet)
			{
				const_cast<UEWBaseAttributeSet*>(BaseAttributeSet)->SetMana(0.0f);
			}
		}
	}
}

void AEWUnitBase::HandleDeath()
{
	StopAI();
	OnDeath.Broadcast(this);
	SetActorEnableCollision(false);
	UE_LOG(LogTemp, Warning, TEXT("Unit %s has died"), *GetName());
}

void AEWUnitBase::OnHealthAttributeChanged(const FGameplayAttribute& Attribute, float NewValue, float OldValue)
{
	OnHealthChanged.Broadcast(this, NewValue);
	if (NewValue <= 0.0f && IsAlive())
	{
		SetCurrentState(EUnitState::Dead);
	}
}

float AEWUnitBase::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	
	// 转换为我们的单位系统
	AEWUnitBase* DamageSourceUnit = Cast<AEWUnitBase>(DamageCauser);
	TakeDamageFromUnit(ActualDamage, DamageSourceUnit);
	
	return ActualDamage;
}

void AEWUnitBase::TakeDamageFromUnit(float DamageAmount, AEWUnitBase* DamageSource)
{
	if (!IsAlive() || DamageAmount <= 0.0f) return;

	// 通过GAS应用伤害
	if (AbilitySystemComponent && BaseAttributeSet)
	{
		// 这里可以创建一个伤害GameplayEffect来应用伤害
		// 暂时直接修改属性
		float NewHealth = FMath::Max(0.0f, BaseAttributeSet->GetHealth() - DamageAmount);
		
		// 触发伤害事件
		OnDamaged.Broadcast(this, DamageAmount, DamageSource);
		
		UE_LOG(LogTemp, Warning, TEXT("Unit %s took %f damage from %s"), 
			*GetName(), DamageAmount, DamageSource ? *DamageSource->GetName() : TEXT("Unknown"));
	}
}

