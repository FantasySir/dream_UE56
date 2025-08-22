// Fill out your copyright notice in the Description page of Project Settings.

#include "GameState/EWBaseState.h"
#include "AbilitySystem/AttributeSets/EWBaseAttributeSet.h"
#include "AbilitySystem/AttributeSets/EWCombatAttributeSet.h"
#include "Character/EWCharacterBase.h"
#include "Character/EWUnitBase.h"
#include "Core/EWGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/World.h"

UEWBaseState::UEWBaseState()
{
	// 创建能力系统组件
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	
	// 创建属性集
	BaseAttributeSet = CreateDefaultSubobject<UEWBaseAttributeSet>(TEXT("BaseAttributeSet"));
	CombatAttributeSet = CreateDefaultSubobject<UEWCombatAttributeSet>(TEXT("CombatAttributeSet"));

	// 初始化状态标签
	StateTags.AddTag(FEWGameplayTags::State_Alive);
}

UAbilitySystemComponent* UEWBaseState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void UEWBaseState::InitializeState()
{
	if (AbilitySystemComponent)
	{
		InitializeAbilitySystem();
	}
}

AActor* UEWBaseState::SpawnCharacter(UWorld* World, const FVector& Location, const FRotator& Rotation)
{
	if (!World || !CharacterClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("UEWBaseState::SpawnCharacter - Invalid World or CharacterClass"));
		return nullptr;
	}

	// 如果已经有Character实例，先销毁
	if (CharacterActor.IsValid())
	{
		DestroyCharacter();
	}

	// 生成新的Character
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* NewCharacter = World->SpawnActor<AActor>(CharacterClass, Location, Rotation, SpawnParams);
	if (NewCharacter)
	{
		CharacterActor = NewCharacter;
		
		// 绑定事件
		BindCharacterEvents(NewCharacter);
		
		// 同步状态到Character
		SyncStateToCharacter();
		
		// 广播事件
		OnCharacterSpawned.Broadcast(NewCharacter);
		
		UE_LOG(LogTemp, Log, TEXT("UEWBaseState::SpawnCharacter - Successfully spawned %s"), *NewCharacter->GetName());
	}

	return NewCharacter;
}

void UEWBaseState::DestroyCharacter()
{
	if (CharacterActor.IsValid())
	{
		AActor* Character = CharacterActor.Get();
		
		// 从Character同步状态
		SyncStateFromCharacter();
		
		// 解绑事件
		UnbindCharacterEvents(Character);
		
		// 销毁Character
		Character->Destroy();
		CharacterActor.Reset();
		
		// 广播事件
		OnCharacterDestroyed.Broadcast(this);
		
		UE_LOG(LogTemp, Log, TEXT("UEWBaseState::DestroyCharacter - Character destroyed"));
	}
}

float UEWBaseState::GetHealth() const
{
	if (BaseAttributeSet)
	{
		return BaseAttributeSet->GetHealth();
	}
	return 0.0f;
}

float UEWBaseState::GetMaxHealth() const
{
	if (BaseAttributeSet)
	{
		return BaseAttributeSet->GetMaxHealth();
	}
	return 0.0f;
}

float UEWBaseState::GetMana() const
{
	if (BaseAttributeSet)
	{
		return BaseAttributeSet->GetMana();
	}
	return 0.0f;
}

float UEWBaseState::GetMaxMana() const
{
	if (BaseAttributeSet)
	{
		return BaseAttributeSet->GetMaxMana();
	}
	return 0.0f;
}

bool UEWBaseState::IsAlive() const
{
	return GetHealth() > 0.0f && HasStateTag(FEWGameplayTags::State_Alive);
}

void UEWBaseState::AddStateTag(FGameplayTag Tag)
{
	StateTags.AddTag(Tag);
}

void UEWBaseState::RemoveStateTag(FGameplayTag Tag)
{
	StateTags.RemoveTag(Tag);
}

bool UEWBaseState::HasStateTag(FGameplayTag Tag) const
{
	return StateTags.HasTag(Tag);
}

void UEWBaseState::InitializeAbilitySystem()
{
	if (AbilitySystemComponent)
	{
		// 暂时跳过Actor初始化，等角色生成后再处理
		// AbilitySystemComponent->InitAbilityActorInfo(this, this);

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
			if (StartupAbility)
			{
				AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, INDEX_NONE, this));
			}
		}

		// 绑定属性变化事件
		if (BaseAttributeSet)
		{
			// TODO: 修复属性变化回调的委托签名
			// AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetHealthAttribute()).AddUObject(this, &UEWBaseState::OnAttributeChangedInternal);
			// AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetManaAttribute()).AddUObject(this, &UEWBaseState::OnAttributeChangedInternal);
		}
	}
}

void UEWBaseState::SyncStateToCharacter()
{
	if (!CharacterActor.IsValid())
	{
		return;
	}

	// 如果Character有AbilitySystemComponent，同步属性
	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(CharacterActor.Get()))
	{
		if (UAbilitySystemComponent* CharacterASC = ASI->GetAbilitySystemComponent())
		{
			// 这里可以添加具体的属性同步逻辑
			// 例如：同步当前血量、魔法值等
		}
	}
}

void UEWBaseState::SyncStateFromCharacter()
{
	if (!CharacterActor.IsValid())
	{
		return;
	}

	// 从Character同步状态数据
	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(CharacterActor.Get()))
	{
		if (UAbilitySystemComponent* CharacterASC = ASI->GetAbilitySystemComponent())
		{
			// 这里可以添加具体的状态同步逻辑
		}
	}
}

void UEWBaseState::OnAttributeChangedInternal(const FOnAttributeChangeData& Data)
{
	// 更新状态标签
	if (Data.Attribute == BaseAttributeSet->GetHealthAttribute())
	{
		if (Data.NewValue <= 0.0f && HasStateTag(FEWGameplayTags::State_Alive))
		{
			RemoveStateTag(FEWGameplayTags::State_Alive);
			AddStateTag(FEWGameplayTags::State_Dead);
		}
		else if (Data.NewValue > 0.0f && HasStateTag(FEWGameplayTags::State_Dead))
		{
			RemoveStateTag(FEWGameplayTags::State_Dead);
			AddStateTag(FEWGameplayTags::State_Alive);
		}
	}

	// 广播属性变化事件
	OnAttributeChanged.Broadcast(this, Data.Attribute);
}

void UEWBaseState::BindCharacterEvents(AActor* Character)
{
	if (!Character)
	{
		return;
	}

	// 这里可以绑定Character的特定事件
	// 例如：死亡事件、受伤事件等
}

void UEWBaseState::UnbindCharacterEvents(AActor* Character)
{
	if (!Character)
	{
		return;
	}

	// 解绑Character的事件
}
