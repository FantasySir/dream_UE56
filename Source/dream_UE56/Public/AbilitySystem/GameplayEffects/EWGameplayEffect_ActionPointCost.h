// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "EWGameplayEffect_ActionPointCost.generated.h"

/**
 * 行动点消耗效果
 * 用于消耗玩家的行动点，通常在使用技能时应用
 */
UCLASS()
class DREAM_UE56_API UEWGameplayEffect_ActionPointCost : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UEWGameplayEffect_ActionPointCost();
};
