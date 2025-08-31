// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/EWUnitBase.h"
#include "AbilitySystem/AttributeSets/EWBaseAttributeSet.h"
#include "AbilitySystem/AttributeSets/EWUnitAttributeSet.h"
#include "AI/EWUnitAIController.h"
#include "AbilitySystem/EWAbilitySystemLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayEffect.h"
#include "AttributeSet.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEWUnitBase::AEWUnitBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	BaseAttributeSet = CreateDefaultSubobject<UEWBaseAttributeSet>("BaseAttributeSet");
	UnitAttributeSet = CreateDefaultSubobject<UEWUnitAttributeSet>("UnitAttributeSet");
	// 初始化感知组件
	InitializePerceptionComponent();
	// 默认单位不免疫时间暂停
	bImmuneTimePause = false;
	bDead = false;
	// 注册
	TimeManagerRegistered();
}

void AEWUnitBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
		
	// 将Controller转换为我们的AI Controller

	UnitAIController = Cast<AEWUnitAIController>(NewController);
	if (UnitAIController && BehaviorTreeAsset)
	{
		UnitAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTreeAsset->BlackboardAsset);
		UnitAIController->RunBehaviorTree(BehaviorTreeAsset);
	}
}

// Called when the game starts or when spawned
void AEWUnitBase::BeginPlay()
{
	Super::BeginPlay();
	// 注册到时间管理系统
	InitAbilityActorInfo();
	// 设置感知刺激源
	SetupPerceptionStimuli();
}

void AEWUnitBase::InitAbilityActorInfo()
{	
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void AEWUnitBase::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor || bDead)
	{
		return;
	}

	//TODO:检查感知到的目标是否是敌对单位
}


void AEWUnitBase::InitializePerceptionComponent()
{// 初始化感知组件
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
	// 初始化刺激源组件
	if (PerceptionComponent)
	{
		// 创建并配置视觉感知
		UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
		if (SightConfig)
		{
			SightConfig->SightRadius = 1500.0f; // 视野范围
			SightConfig->LoseSightRadius = 1800.0f; // 失去视野范围
			SightConfig->PeripheralVisionAngleDegrees = 90.0f; // 周边视角
			SightConfig->SetMaxAge(5.0f); // 感知信息的最大存活时间
			SightConfig->DetectionByAffiliation.bDetectEnemies = true;
			SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
			SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

			PerceptionComponent->ConfigureSense(*SightConfig);
			PerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
		}

		// 绑定感知更新事件
		PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEWUnitBase::OnTargetPerceptionUpdated);
	}
}