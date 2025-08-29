// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EWCombatInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UEWCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 战斗接口
 * 所有可以参与战斗的对象都应该实现这个接口
 */
class DREAM_UE56_API IEWCombatInterface
{
	GENERATED_BODY()

public:
	// 获取阵营
	virtual int32 GetFactionAsInt_Implementation() const { return 0; }
	// 是否存活
	virtual bool IsAlive_Implementation() const { return true; }
	// 获取位置（用于距离计算）
	virtual FVector GetCombatLocation_Implementation() const { return FVector::ZeroVector; }
	// 接受伤害
	virtual void TakeDamageFromCombat_Implementation(float DamageAmount, AActor* DamageSource) {}
	// 是否可以被攻击
	virtual bool CanBeAttacked_Implementation() const { return true; }

};