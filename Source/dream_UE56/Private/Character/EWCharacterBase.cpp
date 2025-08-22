// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/EWCharacterBase.h"
#include "AbilitySystem/AttributeSets/EWBaseAttributeSet.h"
#include "AbilitySystem/AttributeSets/EWPlayerAttributeSet.h"
#include "AbilitySystem/AttributeSets/EWCombatAttributeSet.h"
#include "Character/EWUnitBase.h"
#include "Player/EWUnitManager.h"
#include "Gameplay/EWTimeManager.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values
AEWCharacterBase::AEWCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 创建能力系统组件
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// 创建属性集
	BaseAttributeSet = CreateDefaultSubobject<UEWBaseAttributeSet>(TEXT("BaseAttributeSet"));
	PlayerAttributeSet = CreateDefaultSubobject<UEWPlayerAttributeSet>(TEXT("PlayerAttributeSet"));
	CombatAttributeSet = CreateDefaultSubobject<UEWCombatAttributeSet>(TEXT("CombatAttributeSet"));

	// 创建单位管理器
	UnitManager = CreateDefaultSubobject<UEWUnitManager>(TEXT("UnitManager"));
}

// Called when the game starts or when spawned
void AEWCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	// 初始化能力系统
	InitializeAbilitySystem();

	// 初始化单位管理器
	if (UnitManager)
	{
		UnitManager->Initialize(12, 4); // 默认12个总槽位，4个战斗槽位
	}
}

// Called every frame
void AEWCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 行动值自动回复
	if (AbilitySystemComponent && PlayerAttributeSet)
	{
		float CurrentActionPoint = GetActionPoint();
		float MaxActionPoint = GetMaxActionPoint();
		
		if (CurrentActionPoint < MaxActionPoint)
		{
			float NewActionPoint = FMath::Min(CurrentActionPoint + ActionPointRegenRate * DeltaTime, MaxActionPoint);
			// 这里需要通过GE来修改属性，暂时先这样处理
		}
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

float AEWCharacterBase::GetHealth() const
{
	if (BaseAttributeSet)
	{
		return BaseAttributeSet->GetHealth();
	}
	return 0.0f;
}

float AEWCharacterBase::GetMaxHealth() const
{
	if (BaseAttributeSet)
	{
		return BaseAttributeSet->GetMaxHealth();
	}
	return 0.0f;
}

float AEWCharacterBase::GetHealthPercentage() const
{
	float MaxHealth = GetMaxHealth();
	if (MaxHealth > 0)
	{
		return GetHealth() / MaxHealth;
	}
	return 0.0f;
}

bool AEWCharacterBase::IsAlive() const
{
	return GetHealth() > 0.0f;
}

float AEWCharacterBase::GetMana() const
{
	if (BaseAttributeSet)
	{
		return BaseAttributeSet->GetMana();
	}
	return 0.0f;
}

float AEWCharacterBase::GetMaxMana() const
{
	if (BaseAttributeSet)
	{
		return BaseAttributeSet->GetMaxMana();
	}
	return 0.0f;
}

float AEWCharacterBase::GetActionPoint() const
{
	if (PlayerAttributeSet)
	{
		return PlayerAttributeSet->GetActionPoint();
	}
	return 0.0f;
}

float AEWCharacterBase::GetMaxActionPoint() const
{
	if (PlayerAttributeSet)
	{
		return PlayerAttributeSet->GetMaxActionPoint();
	}
	return 0.0f;
}

bool AEWCharacterBase::CanPauseTime() const
{
	// 检查行动值是否足够
	if (GetActionPoint() < PauseTimeCost)
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
	// 这里需要根据UnitClass的具体消耗来判断
	return GetMana() >= 30.0f; // 临时数值
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

	// 生成单位
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	AEWUnitBase* SummonedUnit = World->SpawnActor<AEWUnitBase>(UnitClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
	
	if (SummonedUnit)
	{
		// 添加到控制列表
		ControlledUnits.Add(SummonedUnit);
		
		// 绑定死亡事件
		// SummonedUnit->OnDeath.AddDynamic(this, &AEWCharacterBase::OnControlledUnitDeath);
		
		// 消耗魔法值
		// TODO: 通过GE消耗魔法值
	}

	return SummonedUnit;
}

void AEWCharacterBase::OnControlledUnitDeath(AEWUnitBase* DeadUnit)
{
	if (DeadUnit)
	{
		ControlledUnits.Remove(DeadUnit);
	}
}

