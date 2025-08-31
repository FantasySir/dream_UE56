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
	// 从时间管理器中移除此Actor
	AActor* Owner = GetOwner();
	if (Owner)
	{
		if (UWorld* World = Owner->GetWorld())
		{
			if (UEWTimeManager* TimeManager = World->GetSubsystem<UEWTimeManager>())
			{
				TimeManager->UnregisterTimeAffectedActor(Owner);
			}
		}
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
		// 注册为时间影响Actor（TimeManager会根据免疫状态自动分类）
		TimeManager->RegisterTimeAffectedActor(Owner);
	}
	else
	{
		// 从管理列表中移除
		TimeManager->UnregisterTimeAffectedActor(Owner);
	}
}
