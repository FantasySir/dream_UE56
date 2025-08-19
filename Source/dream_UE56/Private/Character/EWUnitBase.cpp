// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/EWUnitBase.h"
#include "Character/EWUnitAttributeSet.h"
#include "Data/EWUnitData.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayEffect.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BrainComponent.h"

// Sets default values
AEWUnitBase::AEWUnitBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 创建能力系统组件
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	
	// 设置复制模式 - 对于玩家角色，通常使用 Full，对于AI/NPC使用 Mixed
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// 创建属性集
	AttributeSet = CreateDefaultSubobject<UEWUnitAttributeSet>(TEXT("AttributeSet"));
}

// Called when the game starts or when spawned
void AEWUnitBase::BeginPlay()
{
	Super::BeginPlay();
	
	// 初始化能力系统
	if (AbilitySystemComponent)
	{
		InitializeAbilitySystem();
	}
}

// Called every frame
void AEWUnitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 检查魔法值是否满了，如果满了则释放技能
	if (CanCastSkill() && GetManaPercentage() >= 1.0f)
	{
		CastRandomSkill();
	}
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

void AEWUnitBase::InitializeAbilitySystem()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	// 初始化AbilitySystemComponent与此Actor
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	// 优先使用UnitData中的配置，如果没有则使用直接设置的属性
	TSubclassOf<UGameplayEffect> EffectToApply = DefaultAttributeEffect;
	TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant = StartupAbilities;

	if (UnitData)
	{
		if (UnitData->DefaultAttributeEffectClass)
		{
			EffectToApply = UnitData->DefaultAttributeEffectClass;
		}
		if (UnitData->StartupAbilities.Num() > 0)
		{
			AbilitiesToGrant = UnitData->StartupAbilities;
		}
	}

	// 应用默认属性效果
	if (EffectToApply)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EffectToApply, 1, EffectContext);
		if (SpecHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}

	// 给予起始能力
	for (auto& StartupAbility : AbilitiesToGrant)
	{
		if (StartupAbility)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, INDEX_NONE, this));
		}
	}
}

float AEWUnitBase::GetHealth() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetHealth();
	}
	return 0.0f;
}

float AEWUnitBase::GetMaxHealth() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMaxHealth();
	}
	return 0.0f;
}

float AEWUnitBase::GetHealthPercentage() const
{
	const float MaxHealth = GetMaxHealth();
	if (MaxHealth > 0.0f)
	{
		return GetHealth() / MaxHealth;
	}
	return 0.0f;
}


bool AEWUnitBase::IsAlive() const
{
	return GetHealth() > 0.0f && CurrentState != EUnitState::Dead;
}

float AEWUnitBase::GetMana() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMana();
	}
	return 0.0f;
}

float AEWUnitBase::GetMaxMana() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMaxMana();
	}
	return 0.0f;
}

float AEWUnitBase::GetManaPercentage() const
{
	float MaxMana = GetMaxMana();
	if (MaxMana > 0)
	{
		return GetMana() / MaxMana;
	}
	return 0.0f;
}

float AEWUnitBase::GetStamina() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetStamina();
	}
	return 0.0f;
}

float AEWUnitBase::GetMaxStamina() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMaxStamina();
	}
	return 0.0f;
}

float AEWUnitBase::GetPhysicalAttack() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetPhysicalAttack();
	}
	return 0.0f;
}

float AEWUnitBase::GetMagicalAttack() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMagicalAttack();
	}
	return 0.0f;
}

float AEWUnitBase::GetPhysicalDefense() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetPhysicalDefense();
	}
	return 0.0f;
}

float AEWUnitBase::GetMagicalDefense() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMagicalDefense();
	}
	return 0.0f;
}

int32 AEWUnitBase::GetUnitLevel() const
{
	if (AttributeSet)
	{
		return FMath::RoundToInt(AttributeSet->GetUnitLevel());
	}
	return 1;
}

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
	float DamageAmount = GetPhysicalAttack();
	Target->TakeDamageFromUnit(DamageAmount, this);
	if (AttributeSet)
	{
		float CurrentMana = GetMana();
		float MaxMana = GetMaxMana();
		float ManaGain = 10.0f;
		float NewMana = FMath::Min(CurrentMana + ManaGain, MaxMana);
		const_cast<UEWUnitAttributeSet*>(AttributeSet)->SetMana(NewMana);
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
			if (AttributeSet)
			{
				const_cast<UEWUnitAttributeSet*>(AttributeSet)->SetMana(0.0f);
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
	if (AbilitySystemComponent && AttributeSet)
	{
		// 这里可以创建一个伤害GameplayEffect来应用伤害
		// 暂时直接修改属性
		float NewHealth = FMath::Max(0.0f, GetHealth() - DamageAmount);
		
		// 触发伤害事件
		OnDamaged.Broadcast(this, DamageAmount, DamageSource);
		
		UE_LOG(LogTemp, Warning, TEXT("Unit %s took %f damage from %s"), 
			*GetName(), DamageAmount, DamageSource ? *DamageSource->GetName() : TEXT("Unknown"));
	}
}

