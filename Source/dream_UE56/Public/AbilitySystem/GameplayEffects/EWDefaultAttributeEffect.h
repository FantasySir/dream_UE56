// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "EWDefaultAttributeEffect.generated.h"

/**
 * 默认属性效果，用于设置单位的初始属性值
 */
UCLASS()
class DREAM_UE56_API UEWDefaultAttributeEffect : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UEWDefaultAttributeEffect();
};
