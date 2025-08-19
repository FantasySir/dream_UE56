// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Engine/DamageEvents.h"
#include "EWUnitBase.generated.h"

class UAbilitySystemComponent;
class UEWUnitAttributeSet;
class UEWUnitData;
class UBehaviorTreeComponent;
class UBlackboardComponent;
class UBehaviorTree;

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

// 单位死亡委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitDeath, AEWUnitBase*, DeadUnit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUnitHealthChanged, AEWUnitBase*, Unit, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUnitDamaged, AEWUnitBase*, Unit, float, DamageAmount, AEWUnitBase*, DamageSource);

UCLASS()
class DREAM_UE56_API AEWUnitBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEWUnitBase();

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// 获取属性集
	UFUNCTION(BlueprintCallable, Category = "Ability System")
	class UEWUnitAttributeSet* GetAttributeSet() const { return AttributeSet; }

	// 血量相关的蓝图可调用函数
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthPercentage() const;

	// 是否已死亡
	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsAlive() const;

	// 魔法值相关
	UFUNCTION(BlueprintCallable, Category = "Mana")
	float GetMana() const;

	UFUNCTION(BlueprintCallable, Category = "Mana")
	float GetMaxMana() const;

	UFUNCTION(BlueprintCallable, Category = "Mana")
	float GetManaPercentage() const;

	// 体力值相关
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	float GetMaxStamina() const;

	// 战斗属性
	UFUNCTION(BlueprintCallable, Category = "Combat")
	float GetPhysicalAttack() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	float GetMagicalAttack() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	float GetPhysicalDefense() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	float GetMagicalDefense() const;

	// 等级相关
	UFUNCTION(BlueprintCallable, Category = "UnitLevel")
	int32 GetUnitLevel() const;

	// 阵营相关
	UFUNCTION(BlueprintCallable, Category = "Faction")
	EUnitFaction GetFaction() const { return Faction; }

	UFUNCTION(BlueprintCallable, Category = "Faction")
	void SetFaction(EUnitFaction NewFaction) { Faction = NewFaction; }

	UFUNCTION(BlueprintCallable, Category = "Faction")
	bool IsHostileTo(AEWUnitBase* OtherUnit) const;

	UFUNCTION(BlueprintCallable, Category = "Faction")
	bool IsFriendlyTo(AEWUnitBase* OtherUnit) const;

	// 状态相关
	UFUNCTION(BlueprintCallable, Category = "State")
	EUnitState GetCurrentState() const { return CurrentState; }

	UFUNCTION(BlueprintCallable, Category = "State")
	void SetCurrentState(EUnitState NewState);

	// 职业相关
	UFUNCTION(BlueprintCallable, Category = "Class")
	EUnitClass GetPrimaryClass() const { return PrimaryClass; }

	UFUNCTION(BlueprintCallable, Category = "Class")
	EUnitClass GetSecondaryClass() const { return SecondaryClass; }

	UFUNCTION(BlueprintCallable, Category = "Class")
	void SetPrimaryClass(EUnitClass NewClass) { PrimaryClass = NewClass; }

	UFUNCTION(BlueprintCallable, Category = "Class")
	void SetSecondaryClass(EUnitClass NewClass) { SecondaryClass = NewClass; }

	// AI相关
	UFUNCTION(BlueprintCallable, Category = "AI")
	void StartAI();

	UFUNCTION(BlueprintCallable, Category = "AI")
	void StopAI();

	// 战斗相关
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual float TakeDamage(float Damage, const struct FDamageEvent& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void TakeDamageFromUnit(float DamageAmount, AEWUnitBase* DamageSource);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void AttackTarget(AEWUnitBase* Target);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool CanAttack(AEWUnitBase* Target) const;

	// 技能相关
	UFUNCTION(BlueprintCallable, Category = "Skills")
	bool CanCastSkill() const;

	UFUNCTION(BlueprintCallable, Category = "Skills")
	void CastRandomSkill();

	// 委托事件
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnUnitDeath OnDeath;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnUnitHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnUnitDamaged OnDamaged;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 能力系统组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System")
	class UAbilitySystemComponent* AbilitySystemComponent;

	// 属性集
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System")
	class UEWUnitAttributeSet* AttributeSet;

	// AI组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UBlackboardComponent* BlackboardComponent;

	// 初始化能力系统
	virtual void InitializeAbilitySystem();

	// 单位数据资产
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Unit Data")
	class UEWUnitData* UnitData;

	// 默认属性效果（用于设置初始属性值）
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability System")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	// 起始能力
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability System")
	TArray<TSubclassOf<class UGameplayAbility>> StartupAbilities;

	// 行为树资产
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTreeAsset;

	// 黑板资产
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AI")
	class UBlackboardData* BlackboardAsset;

	// 单位属性
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Unit Properties")
	EUnitFaction Faction = EUnitFaction::Neutral;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Unit Properties")
	EUnitState CurrentState = EUnitState::Idle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Unit Properties")
	EUnitClass PrimaryClass = EUnitClass::None;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Unit Properties")
	EUnitClass SecondaryClass = EUnitClass::None;

	// 战斗相关属性
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat")
	float AttackRange = 200.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat")
	float AttackCooldown = 1.5f;

	UPROPERTY()
	float LastAttackTime = 0.0f;

	// 当单位死亡时调用
	virtual void HandleDeath();

	// 处理属性变化
	UFUNCTION()
	virtual void OnHealthAttributeChanged(const FGameplayAttribute& Attribute, float NewValue, float OldValue);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
