// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "EWGameplayTags.generated.h"

/**
 * 游戏标签定义
 * 集中管理项目中使用的所有GameplayTag
 */
USTRUCT(BlueprintType)
struct DREAM_UE56_API FEWGameplayTags
{
	GENERATED_BODY()

public:
	// 静态初始化所有标签
	static void InitializeNativeTags();

	// 能力标签
	static FGameplayTag Ability_TimePause;
	static FGameplayTag Ability_Summon;
	static FGameplayTag Ability_Attack;
	static FGameplayTag Ability_Defense;
	static FGameplayTag Ability_Heal;

	// 状态标签
	static FGameplayTag State_Alive;
	static FGameplayTag State_Dead;
	static FGameplayTag State_Stunned;
	static FGameplayTag State_Invincible;
	static FGameplayTag State_TimePaused;

	// 单位类型标签
	static FGameplayTag Unit_Type_Player;
	static FGameplayTag Unit_Type_Enemy;
	static FGameplayTag Unit_Type_Neutral;

	// 事件标签
	static FGameplayTag Event_Death;
	static FGameplayTag Event_Summon;
	static FGameplayTag Event_TimePause;
	static FGameplayTag Event_TimeResume;

	// 效果标签
	static FGameplayTag Effect_Damage;
	static FGameplayTag Effect_Heal;
	static FGameplayTag Effect_ActionPointCost;

private:
	// 添加原生标签的辅助函数
	static void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
};
