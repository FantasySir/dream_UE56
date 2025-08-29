// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "Character/EWUnitBase.h"
#include "EWAbilitySystemLibrary.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
struct FGameplayEffectContextHandle;

/**
 * 能力系统蓝图函数库
 * 提供全局静态函数用于能力系统相关操作
 */
UCLASS()
class DREAM_UE56_API UEWAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/*
	 * Ability System Class Defaults
	 */
    UFUNCTION(BlueprintCallable, Category="EWAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes();

};
