// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/EWCharacter.h"
#include "AbilitySystem/AttributeSets/EWBaseAttributeSet.h"
#include "AbilitySystem/AttributeSets/EWPlayerAttributeSet.h"
#include "Character/EWUnitBase.h"
#include "GameState/EWPlayerState.h"
#include "Player/EWPlayerController.h"
#include "Player/EWUnitManager.h"
#include "Gameplay/EWTimeManager.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values
AEWCharacter::AEWCharacter()
{
}

// Called when the game starts or when spawned
void AEWCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	SetupPerceptionStimuli();

	// 创建单位管理器
	UnitManager = CreateDefaultSubobject<UEWUnitManager>("UnitManager");
	
	// 当被控制器占有时初始化能力系统Actor信息
	InitAbilityActorInfo();

	if (UnitManager)
	{
		UnitManager->Initialize(12, 4); // 默认12个总槽位，4个战斗槽位
	}

	// 注册时间管理器
	TimeManagerRegistered();
}

void AEWCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
}

void AEWCharacter::InitAbilityActorInfo()
{
	AEWPlayerState* EWPlayerState = GetPlayerState<AEWPlayerState>();
	check(EWPlayerState);
	EWPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(EWPlayerState, this);

	AbilitySystemComponent = EWPlayerState->GetAbilitySystemComponent();
	BaseAttributeSet = EWPlayerState->GetBaseAttributeSet();
	PlayerAttributeSet = EWPlayerState->GetPlayerAttributeSet();

}