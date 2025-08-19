// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayEffect.h"
#include "EWUnitData.generated.h"

/**
 * 单位数据资产，包含单位的基础配置
 */
UCLASS(BlueprintType)
class DREAM_UE56_API UEWUnitData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// 单位名称
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Unit Info")
	FText UnitName;

	// 单位描述
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Unit Info")
	FText UnitDescription;

	// 基础最大血量
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes", meta = (ClampMin = "1.0"))
	float BaseMaxHealth = 100.0f;

	// 初始血量百分比
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float InitialHealthPercentage = 1.0f;

	// 默认属性效果类
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffectClass;

	// 起始能力
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> StartupAbilities;
};
