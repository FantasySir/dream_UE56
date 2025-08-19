// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/EWUnitAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"

UEWUnitAttributeSet::UEWUnitAttributeSet()
{
	// 初始化默认属性值
	InitHealth(100.0f);
	InitMaxHealth(100.0f);
	InitMana(100.0f);
	InitMaxMana(100.0f);
	InitActionPoint(100.0f);
	InitMaxActionPoint(100.0f);
	InitStamina(100.0f);
	InitMaxStamina(100.0f);
	InitPhysicalAttack(10.0f);
	InitMagicalAttack(10.0f);
	InitPhysicalDefense(5.0f);
	InitMagicalDefense(5.0f);
	InitMovementSpeed(600.0f);
	InitLevel(1.0f);
	InitDamage(0.0f);
	InitHealing(0.0f);
}

void UEWUnitAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// 确保当前值不超过最大值且不为负数
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}
	else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.0f);
	}
	else if (Attribute == GetActionPointAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxActionPoint());
	}
	else if (Attribute == GetMaxActionPointAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.0f);
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
	else if (Attribute == GetMaxStaminaAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.0f);
	}
	else if (Attribute == GetPhysicalAttackAttribute() || Attribute == GetMagicalAttackAttribute() ||
			 Attribute == GetPhysicalDefenseAttribute() || Attribute == GetMagicalDefenseAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.0f);
	}
	else if (Attribute == GetMovementSpeedAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.0f);
	}
	else if (Attribute == GetLevelAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}
}

void UEWUnitAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	// 获取目标演员
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
	}

	// 处理伤害
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const float LocalDamageDone = GetDamage();
		SetDamage(0.f);

		if (LocalDamageDone > 0)
		{
			const float OldHealth = GetHealth();
			SetHealth(FMath::Clamp(OldHealth - LocalDamageDone, 0.0f, GetMaxHealth()));

			if (GetHealth() <= 0.0f)
			{
				// TODO: 处理死亡事件
				UE_LOG(LogTemp, Warning, TEXT("Unit died!"));
			}
		}
	}
	// 处理治疗
	else if (Data.EvaluatedData.Attribute == GetHealingAttribute())
	{
		const float LocalHealingDone = GetHealing();
		SetHealing(0.f);

		if (LocalHealingDone > 0)
		{
			const float OldHealth = GetHealth();
			SetHealth(FMath::Clamp(OldHealth + LocalHealingDone, 0.0f, GetMaxHealth()));
		}
	}
	// 处理各种属性变化
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, GetMaxHealth(), GetHealthAttribute());
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}
	else if (Data.EvaluatedData.Attribute == GetMaxManaAttribute())
	{
		AdjustAttributeForMaxChange(Mana, MaxMana, GetMaxMana(), GetManaAttribute());
	}
	else if (Data.EvaluatedData.Attribute == GetActionPointAttribute())
	{
		SetActionPoint(FMath::Clamp(GetActionPoint(), 0.0f, GetMaxActionPoint()));
	}
	else if (Data.EvaluatedData.Attribute == GetMaxActionPointAttribute())
	{
		AdjustAttributeForMaxChange(ActionPoint, MaxActionPoint, GetMaxActionPoint(), GetActionPointAttribute());
	}
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
	}
	else if (Data.EvaluatedData.Attribute == GetMaxStaminaAttribute())
	{
		AdjustAttributeForMaxChange(Stamina, MaxStamina, GetMaxStamina(), GetStaminaAttribute());
	}
}

void UEWUnitAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UEWUnitAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEWUnitAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEWUnitAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEWUnitAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEWUnitAttributeSet, ActionPoint, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEWUnitAttributeSet, MaxActionPoint, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEWUnitAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEWUnitAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEWUnitAttributeSet, PhysicalAttack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEWUnitAttributeSet, MagicalAttack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEWUnitAttributeSet, PhysicalDefense, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEWUnitAttributeSet, MagicalDefense, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEWUnitAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEWUnitAttributeSet, Level, COND_None, REPNOTIFY_Always);
}

// 所有属性的OnRep函数
void UEWUnitAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEWUnitAttributeSet, Health, OldHealth);
}

void UEWUnitAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEWUnitAttributeSet, MaxHealth, OldMaxHealth);
}

void UEWUnitAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEWUnitAttributeSet, Mana, OldMana);
}

void UEWUnitAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEWUnitAttributeSet, MaxMana, OldMaxMana);
}

void UEWUnitAttributeSet::OnRep_ActionPoint(const FGameplayAttributeData& OldActionPoint)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEWUnitAttributeSet, ActionPoint, OldActionPoint);
}

void UEWUnitAttributeSet::OnRep_MaxActionPoint(const FGameplayAttributeData& OldMaxActionPoint)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEWUnitAttributeSet, MaxActionPoint, OldMaxActionPoint);
}

void UEWUnitAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEWUnitAttributeSet, Stamina, OldStamina);
}

void UEWUnitAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEWUnitAttributeSet, MaxStamina, OldMaxStamina);
}

void UEWUnitAttributeSet::OnRep_PhysicalAttack(const FGameplayAttributeData& OldPhysicalAttack)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEWUnitAttributeSet, PhysicalAttack, OldPhysicalAttack);
}

void UEWUnitAttributeSet::OnRep_MagicalAttack(const FGameplayAttributeData& OldMagicalAttack)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEWUnitAttributeSet, MagicalAttack, OldMagicalAttack);
}

void UEWUnitAttributeSet::OnRep_PhysicalDefense(const FGameplayAttributeData& OldPhysicalDefense)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEWUnitAttributeSet, PhysicalDefense, OldPhysicalDefense);
}

void UEWUnitAttributeSet::OnRep_MagicalDefense(const FGameplayAttributeData& OldMagicalDefense)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEWUnitAttributeSet, MagicalDefense, OldMagicalDefense);
}

void UEWUnitAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEWUnitAttributeSet, MovementSpeed, OldMovementSpeed);
}

void UEWUnitAttributeSet::OnRep_Level(const FGameplayAttributeData& OldLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEWUnitAttributeSet, Level, OldLevel);
}

void UEWUnitAttributeSet::AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}
