// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/EWCharacterBase.h"
#include "Interfaces/PlayerInterface.h"
#include "EWCharacter.generated.h"  

class UEWUnitManager;
class UEWPlayerAttributeSet;

UCLASS()
class DREAM_UE56_API AEWCharacter : public AEWCharacterBase
{
    GENERATED_BODY()

public:
    AEWCharacter();
    virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	UEWPlayerAttributeSet* GetPlayerAttributeSet() const { return PlayerAttributeSet; }
protected:
	UPROPERTY()
	TObjectPtr<UEWUnitManager> UnitManager;

	UPROPERTY()
	TObjectPtr<UEWPlayerAttributeSet> PlayerAttributeSet;

	virtual void InitAbilityActorInfo() override;

};

/*
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
	**/

    /**
	// 能力系统组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// 属性集
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System")
	class UEWBaseAttributeSet* BaseAttributeSet;

	

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
	*/