// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/EWGameplayTags.h"
#include "GameplayTagsManager.h"

// 定义静态变量
FGameplayTag FEWGameplayTags::Ability_TimePause;
FGameplayTag FEWGameplayTags::Ability_Summon;
FGameplayTag FEWGameplayTags::Ability_Attack;
FGameplayTag FEWGameplayTags::Ability_Defense;
FGameplayTag FEWGameplayTags::Ability_Heal;

FGameplayTag FEWGameplayTags::State_Alive;
FGameplayTag FEWGameplayTags::State_Dead;
FGameplayTag FEWGameplayTags::State_Stunned;
FGameplayTag FEWGameplayTags::State_Invincible;
FGameplayTag FEWGameplayTags::State_TimePaused;

FGameplayTag FEWGameplayTags::Unit_Type_Player;
FGameplayTag FEWGameplayTags::Unit_Type_Enemy;
FGameplayTag FEWGameplayTags::Unit_Type_Neutral;

FGameplayTag FEWGameplayTags::Event_Death;
FGameplayTag FEWGameplayTags::Event_Summon;
FGameplayTag FEWGameplayTags::Event_TimePause;
FGameplayTag FEWGameplayTags::Event_TimeResume;

FGameplayTag FEWGameplayTags::Effect_Damage;
FGameplayTag FEWGameplayTags::Effect_Heal;
FGameplayTag FEWGameplayTags::Effect_ActionPointCost;

void FEWGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();

	// 能力标签
	AddTag(Ability_TimePause, "Ability.TimePause", "时间暂停能力");
	AddTag(Ability_Summon, "Ability.Summon", "召唤单位能力");
	AddTag(Ability_Attack, "Ability.Attack", "攻击能力");
	AddTag(Ability_Defense, "Ability.Defense", "防御能力");
	AddTag(Ability_Heal, "Ability.Heal", "治疗能力");

	// 状态标签
	AddTag(State_Alive, "State.Alive", "存活状态");
	AddTag(State_Dead, "State.Dead", "死亡状态");
	AddTag(State_Stunned, "State.Stunned", "眩晕状态");
	AddTag(State_Invincible, "State.Invincible", "无敌状态");
	AddTag(State_TimePaused, "State.TimePaused", "时间暂停状态");

	// 单位类型标签
	AddTag(Unit_Type_Player, "Unit.Type.Player", "玩家单位");
	AddTag(Unit_Type_Enemy, "Unit.Type.Enemy", "敌方单位");
	AddTag(Unit_Type_Neutral, "Unit.Type.Neutral", "中立单位");

	// 事件标签
	AddTag(Event_Death, "Event.Death", "死亡事件");
	AddTag(Event_Summon, "Event.Summon", "召唤事件");
	AddTag(Event_TimePause, "Event.TimePause", "时间暂停事件");
	AddTag(Event_TimeResume, "Event.TimeResume", "时间恢复事件");

	// 效果标签
	AddTag(Effect_Damage, "Effect.Damage", "伤害效果");
	AddTag(Effect_Heal, "Effect.Heal", "治疗效果");
	AddTag(Effect_ActionPointCost, "Effect.ActionPointCost", "行动点消耗效果");
}

void FEWGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}
