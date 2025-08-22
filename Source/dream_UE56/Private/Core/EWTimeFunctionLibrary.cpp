// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/EWTimeFunctionLibrary.h"
#include "Gameplay/EWTimeManager.h"
#include "Character/EWCharacterBase.h"
#include "Components/EWTimePauseImmuneComponent.h"
#include "Engine/World.h"

UEWTimeManager* UEWTimeFunctionLibrary::GetTimeManager(const UObject* WorldContextObject)
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

bool UEWTimeFunctionLibrary::PauseGameTime(const UObject* WorldContextObject, AEWCharacterBase* Instigator)
{
	UEWTimeManager* TimeManager = GetTimeManager(WorldContextObject);
	if (!TimeManager || !Instigator)
	{
		return false;
	}

	if (TimeManager->IsTimePaused())
	{
		return false;
	}

	TimeManager->PauseTime(Instigator);
	return true;
}

void UEWTimeFunctionLibrary::ResumeGameTime(const UObject* WorldContextObject)
{
	UEWTimeManager* TimeManager = GetTimeManager(WorldContextObject);
	if (TimeManager)
	{
		TimeManager->ResumeTime();
	}
}

bool UEWTimeFunctionLibrary::IsGameTimePaused(const UObject* WorldContextObject)
{
	UEWTimeManager* TimeManager = GetTimeManager(WorldContextObject);
	return TimeManager ? TimeManager->IsTimePaused() : false;
}

AEWCharacterBase* UEWTimeFunctionLibrary::GetTimePauseInstigator(const UObject* WorldContextObject)
{
	UEWTimeManager* TimeManager = GetTimeManager(WorldContextObject);
	return TimeManager ? TimeManager->GetTimePauseInstigator() : nullptr;
}

void UEWTimeFunctionLibrary::RegisterTimeSensitiveActor(const UObject* WorldContextObject, AActor* Actor)
{
	UEWTimeManager* TimeManager = GetTimeManager(WorldContextObject);
	if (TimeManager && Actor)
	{
		TimeManager->RegisterTimeSensitiveActor(Actor);
	}
}

void UEWTimeFunctionLibrary::UnregisterTimeSensitiveActor(const UObject* WorldContextObject, AActor* Actor)
{
	UEWTimeManager* TimeManager = GetTimeManager(WorldContextObject);
	if (TimeManager && Actor)
	{
		TimeManager->UnregisterTimeSensitiveActor(Actor);
	}
}

bool UEWTimeFunctionLibrary::IsActorImmuneToTimePause(const UObject* WorldContextObject, AActor* Actor)
{
	UEWTimeManager* TimeManager = GetTimeManager(WorldContextObject);
	return TimeManager ? TimeManager->IsActorImmuneToTimePause(Actor) : false;
}

void UEWTimeFunctionLibrary::SetActorTimePauseImmune(AActor* Actor, bool bImmune)
{
	if (!Actor)
	{
		return;
	}

	// 查找或创建时间暂停免疫组件
	UEWTimePauseImmuneComponent* ImmuneComponent = Actor->FindComponentByClass<UEWTimePauseImmuneComponent>();
	
	if (!ImmuneComponent && bImmune)
	{
		// 如果需要免疫但没有组件，创建一个
		ImmuneComponent = NewObject<UEWTimePauseImmuneComponent>(Actor);
		Actor->AddInstanceComponent(ImmuneComponent);
		ImmuneComponent->RegisterComponent();
	}

	if (ImmuneComponent)
	{
		ImmuneComponent->SetTimePauseImmune(bImmune);
	}
}
