// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/EWTimePauseImmuneComponent.h"
#include "Gameplay/EWTimeManager.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UEWTimePauseImmuneComponent::UEWTimePauseImmuneComponent()
{
	// Set this component to be ticked every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = false;
	
	bIsTimePauseImmune = true;
	bAutoRegisterOnBeginPlay = true;
}

// Called when the game starts
void UEWTimePauseImmuneComponent::BeginPlay()
{
	Super::BeginPlay();

	if (bAutoRegisterOnBeginPlay && bIsTimePauseImmune)
	{
		SetTimePauseImmune(true);
	}
}

void UEWTimePauseImmuneComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 从免疫列表中移除
	if (bIsTimePauseImmune)
	{
		SetTimePauseImmune(false);
	}

	Super::EndPlay(EndPlayReason);
}

void UEWTimePauseImmuneComponent::SetTimePauseImmune(bool bImmune)
{
	bIsTimePauseImmune = bImmune;

	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	UWorld* World = Owner->GetWorld();
	if (!World)
	{
		return;
	}

	UEWTimeManager* TimeManager = World->GetSubsystem<UEWTimeManager>();
	if (!TimeManager)
	{
		return;
	}

	if (bImmune)
	{
		// 注册为免疫Actor
		TimeManager->RegisterTimeSensitiveActor(Owner);
		// 这里可以添加额外的免疫逻辑
	}
	else
	{
		// 从免疫列表移除
		TimeManager->UnregisterTimeSensitiveActor(Owner);
	}
}
