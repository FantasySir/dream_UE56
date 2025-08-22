// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EWPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class AEWCharacterBase;
class AEWUnitBase;

/**
 * 玩家控制器，处理增强输入系统
 */
UCLASS()
class DREAM_UE56_API AEWPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AEWPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// 输入映射上下文
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> EWMappingContext;

	// 基础移动输入动作
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	// 战斗相关输入动作
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Combat")
	TObjectPtr<UInputAction> ToggleLockAction;

	// 时间控制输入动作
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Time")
	TObjectPtr<UInputAction> PauseTimeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Time")
	TObjectPtr<UInputAction> ResumeTimeAction;

	// 召唤系统输入动作
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Summoning")
	TObjectPtr<UInputAction> SummonUnit1Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Summoning")
	TObjectPtr<UInputAction> SummonUnit2Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Summoning")
	TObjectPtr<UInputAction> SummonUnit3Action;

	// 技能释放输入动作
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Skills")
	TObjectPtr<UInputAction> Skill1Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Skills")
	TObjectPtr<UInputAction> Skill2Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Skills")
	TObjectPtr<UInputAction> Skill3Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Skills")
	TObjectPtr<UInputAction> Skill4Action;

	// 输入处理函数
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	// 战斗相关输入处理
	void ToggleLock(const FInputActionValue& Value);
	
	// 时间控制输入处理
	void PauseTime(const FInputActionValue& Value);
	void ResumeTime(const FInputActionValue& Value);
	
	// 召唤系统输入处理
	void SummonUnit1(const FInputActionValue& Value);
	void SummonUnit2(const FInputActionValue& Value);
	void SummonUnit3(const FInputActionValue& Value);
	
	// 技能释放输入处理
	void CastSkill1(const FInputActionValue& Value);
	void CastSkill2(const FInputActionValue& Value);
	void CastSkill3(const FInputActionValue& Value);
	void CastSkill4(const FInputActionValue& Value);

	// 锁定系统相关
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lock System")
	float LockRange = 1000.0f;

	// 寻找最近的可锁定目标
	UFUNCTION(BlueprintCallable, Category = "Lock System")
	AEWUnitBase* FindNearestLockableTarget();

	// 获取控制的主角
	UFUNCTION(BlueprintCallable, Category = "Character")
	AEWCharacterBase* GetControlledCharacter();

public:
	// 锁定模式时的相机控制
	void LockLook(const FInputActionValue& Value);
};
