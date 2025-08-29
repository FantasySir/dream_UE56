// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/EWCharacterBase.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Engine/DamageEvents.h"
#include "Interfaces/EWCombatInterface.h"
#include "Perception/AIPerceptionTypes.h"
#include "EWUnitBase.generated.h"

class UAbilitySystemComponent;
class UEWBaseAttributeSet;
class UEWCombatAttributeSet;
class UEWUnitAttributeSet;
class UBehaviorTree;
class AEWUnitState;
class AEWUnitAIController;
class UAIPerceptionComponent;
struct FAIStimulus;

// 单位阵营枚举
UENUM(BlueprintType)
enum class EUnitFaction : uint8
{
	Player			UMETA(DisplayName = "Player"),
	Enemy			UMETA(DisplayName = "Enemy"),
	Neutral			UMETA(DisplayName = "Neutral")
};

// 单位状态枚举
UENUM(BlueprintType)
enum class EUnitState : uint8
{
	Idle			UMETA(DisplayName = "Idle"),
	Patrolling		UMETA(DisplayName = "Patrolling"),
	Chasing			UMETA(DisplayName = "Chasing"),
	Combat			UMETA(DisplayName = "Combat"),
	Dead			UMETA(DisplayName = "Dead")
};

// 职业枚举
UENUM(BlueprintType)
enum class EUnitClass : uint8
{
	None			UMETA(DisplayName = "None"),
	Warrior			UMETA(DisplayName = "Warrior"),
	Mage			UMETA(DisplayName = "Mage"),
	Archer			UMETA(DisplayName = "Archer"),
	Rogue			UMETA(DisplayName = "Rogue"),
	Priest			UMETA(DisplayName = "Priest")
};


// 感知委托事件
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnemyPerceived, AEWUnitBase*, PerceivedEnemy, bool, bSuccessfullyPerceived);

UCLASS()
class DREAM_UE56_API AEWUnitBase : public AEWCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEWUnitBase();

	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

	UEWUnitAttributeSet* GetUnitAttributeSet() const { return UnitAttributeSet; }

	// 获取AI Controller
	UFUNCTION(BlueprintCallable, Category = "AI")
	class AEWUnitAIController* GetUnitAIController() const { return UnitAIController; }
	// 感知相关
	UFUNCTION(BlueprintCallable, Category = "Perception")
	class UAIPerceptionComponent* GetPerceptionComponent() const { return PerceptionComponent; }

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	TObjectPtr<UEWUnitAttributeSet> UnitAttributeSet;

	// AI
	UPROPERTY()
	TObjectPtr<AEWUnitAIController> UnitAIController;
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

	// 感知组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perception")
	TObjectPtr<UAIPerceptionComponent> PerceptionComponent;

	void InitializePerceptionComponent();
};
