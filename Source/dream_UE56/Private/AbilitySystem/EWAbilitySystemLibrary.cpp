// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/EWAbilitySystemLibrary.h"
#include "AbilitySystemComponent.h"
#include "Character/EWUnitBase.h"
#include "Character/EWCharacterBase.h"
#include "Gameplay/EWTimeManager.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

void UEWAbilitySystemLibrary::InitializeDefaultAttributes()
{
    return;

	// TODO: 这里将来可以根据UnitClass和Level来初始化不同的属性
	// 例如：
	// - Warrior: 高血量，高物理攻击
	// - Mage: 高魔法值，高魔法攻击
	// - Archer: 中等血量，高敏捷
	// - Rogue: 低血量，高暴击
	// - Priest: 中等血量，高治疗能力
}

UEWTimeManager* UEWAbilitySystemLibrary::GetTimeManager(const UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return nullptr;
	}

	return World->GetSubsystem<UEWTimeManager>();
}

bool UEWAbilitySystemLibrary::IsGameTimePaused(const UObject* WorldContextObject)
{
	UEWTimeManager* TimeManager = GetTimeManager(WorldContextObject);
	return TimeManager ? TimeManager->IsTimePaused() : false;
}

void UEWAbilitySystemLibrary::PauseGameTime(const UObject* WorldContextObject, AEWCharacterBase* Instigator)
{
	UEWTimeManager* TimeManager = GetTimeManager(WorldContextObject);
	if (!TimeManager || !Instigator)
	{
		return;
	}
	TimeManager->PauseTime(Instigator);
	return;
}

void UEWAbilitySystemLibrary::ResumeGameTime(const UObject* WorldContextObject)
{
	UEWTimeManager* TimeManager = GetTimeManager(WorldContextObject);
	if (TimeManager)
	{
		TimeManager->ResumeTime();
	}
}
