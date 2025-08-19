// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "EWCharacterBase.generated.h"

class UAbilitySystemComponent;
class UEWUnitAttributeSet;
class AEWUnitBase;
class UGameplayAbility;
class UGameplayEffect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChanged, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionPointChanged, float, NewActionPoint);

UCLASS()
class DREAM_UE56_API AEWCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEWCharacterBase();

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// 获取属性集
	UFUNCTION(BlueprintCallable, Category = "Ability System")
	class UEWUnitAttributeSet* GetAttributeSet() const { return AttributeSet; }

	// 生命值相关
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthPercentage() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsAlive() const;

	// 魔法值相关
	UFUNCTION(BlueprintCallable, Category = "Mana")
	float GetMana() const;

	UFUNCTION(BlueprintCallable, Category = "Mana")
	float GetMaxMana() const;

	// 行动值相关
	UFUNCTION(BlueprintCallable, Category = "ActionPoint")
	float GetActionPoint() const;

	UFUNCTION(BlueprintCallable, Category = "ActionPoint")
	float GetMaxActionPoint() const;

	// 时间控制相关
	UFUNCTION(BlueprintCallable, Category = "Time Control")
	bool CanPauseTime() const;

	UFUNCTION(BlueprintCallable, Category = "Time Control")
	void PauseTime();

	UFUNCTION(BlueprintCallable, Category = "Time Control")
	void ResumeTime();

	UFUNCTION(BlueprintCallable, Category = "Time Control")
	bool IsTimePaused() const { return bIsTimePaused; }

	// 锁定系统
	UFUNCTION(BlueprintCallable, Category = "Lock System")
	void LockTarget(AEWUnitBase* Target);

	UFUNCTION(BlueprintCallable, Category = "Lock System")
	void UnlockTarget();

	UFUNCTION(BlueprintCallable, Category = "Lock System")
	AEWUnitBase* GetLockedTarget() const { return LockedTarget; }

	// 召唤单位
	UFUNCTION(BlueprintCallable, Category = "Summoning")
	bool CanSummonUnit(TSubclassOf<AEWUnitBase> UnitClass) const;

	UFUNCTION(BlueprintCallable, Category = "Summoning")
	AEWUnitBase* SummonUnit(TSubclassOf<AEWUnitBase> UnitClass, FVector SpawnLocation);

	// 获取控制的单位列表
	UFUNCTION(BlueprintCallable, Category = "Summoning")
	TArray<AEWUnitBase*> GetControlledUnits() const { return ControlledUnits; }

	// 获取单位管理器
	UFUNCTION(BlueprintCallable, Category = "Unit Management")
	class UEWUnitManager* GetUnitManager() const { return UnitManager; }

	// 委托事件
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnManaChanged OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnActionPointChanged OnActionPointChanged;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 能力系统组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System")
	class UAbilitySystemComponent* AbilitySystemComponent;

	// 属性集
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System")
	class UEWUnitAttributeSet* AttributeSet;

	// 初始化能力系统
	virtual void InitializeAbilitySystem();

	// 默认属性效果
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability System")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	// 起始能力
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability System")
	TArray<TSubclassOf<class UGameplayAbility>> StartupAbilities;

	// 时间暂停相关
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Time Control")
	float PauseTimeCost = 20.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Time Control")
	float PauseTimeCooldown = 5.0f;

	UPROPERTY()
	bool bIsTimePaused = false;

	UPROPERTY()
	float LastPauseTime = 0.0f;

	// 锁定系统
	UPROPERTY()
	AEWUnitBase* LockedTarget = nullptr;

	// 控制的单位列表
	UPROPERTY()
	TArray<AEWUnitBase*> ControlledUnits;

	// 单位管理器
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit Management")
	class UEWUnitManager* UnitManager;

	// 单位死亡时的回调
	UFUNCTION()
	void OnControlledUnitDeath(AEWUnitBase* DeadUnit);

	// 行动值回复
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ActionPoint")
	float ActionPointRegenRate = 5.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
