// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "EWCharacterBase.generated.h"

class UAbilitySystemComponent;
class UEWBaseAttributeSet;
class UEWPlayerAttributeSet;
class UEWCombatAttributeSet;
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

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// 获取属性集
	UFUNCTION(BlueprintCallable, Category = "Ability System")
	UEWBaseAttributeSet* GetBaseAttributeSet() const { return BaseAttributeSet; }

	UFUNCTION(BlueprintCallable, Category = "Ability System")
	UEWPlayerAttributeSet* GetPlayerAttributeSet() const { return PlayerAttributeSet; }

	UFUNCTION(BlueprintCallable, Category = "Ability System")
	UEWCombatAttributeSet* GetCombatAttributeSet() const { return CombatAttributeSet; }

	// BaseAttribute相关
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthPercentage() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetManaPercentage() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsAlive() const;

	// 时间控制相关
	UFUNCTION(BlueprintCallable, Category = "Time Control")
	bool CanPauseTime() const;

	UFUNCTION(BlueprintCallable, Category = "Time Control")
	void PauseTime();

	UFUNCTION(BlueprintCallable, Category = "Time Control")
	void ResumeTime();

	UFUNCTION(BlueprintCallable, Category = "Time Control")
	bool IsTimePaused() const;

	// 获取时间管理器
	UFUNCTION(BlueprintCallable, Category = "Time Control")
	class UEWTimeManager* GetTimeManager() const;

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
	// APawn interface
	virtual void PossessedBy(AController* NewController) override;

	// 初始化能力系统Actor信息
	virtual void InitAbilityActorInfo();

	// 能力系统组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// 属性集
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System")
	class UEWBaseAttributeSet* BaseAttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System")
	class UEWPlayerAttributeSet* PlayerAttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System")
	class UEWCombatAttributeSet* CombatAttributeSet;

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
	float LastPauseTime = 0.0f;

	// 锁定系统
	UPROPERTY()
	AEWUnitBase* LockedTarget = nullptr;

	// 单位管理器
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit Management")
	class UEWUnitManager* UnitManager;

	// 行动值回复
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ActionPoint")
	float ActionPointRegenRate = 5.0f;

private:
	// 初始化能力系统
	virtual void InitializeAbilitySystem();
};
