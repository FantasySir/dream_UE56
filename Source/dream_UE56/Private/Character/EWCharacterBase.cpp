// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/EWCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Perception/AISense_Sight.h"
#include "AbilitySystem/AttributeSets/EWBaseAttributeSet.h"



// Sets default values
AEWCharacterBase::AEWCharacterBase()
{
 	PrimaryActorTick.bCanEverTick = true;
	//初始化刺激源组件
	StimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("StimuliSourceComponent");
}

void AEWCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEWCharacterBase, bIsStunned);
	DOREPLIFETIME(AEWCharacterBase, bIsSilenced);
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