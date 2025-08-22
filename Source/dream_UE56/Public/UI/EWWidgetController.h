// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "Engine/DataTable.h"
#include "EWWidgetController.generated.h"

class UAbilitySystemComponent;
class UEWBaseAttributeSet;
class UEWPlayerAttributeSet;
class UEWCombatAttributeSet;
class APlayerController;
class APawn;
class APlayerState;
struct FOnAttributeChangeData;

// Widget Controller 参数结构体
USTRUCT(BlueprintType)
struct DREAM_UE56_API FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, 
		UEWBaseAttributeSet* BaseAS, UEWPlayerAttributeSet* PlayerAS, UEWCombatAttributeSet* CombatAS)
		: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), 
		  BaseAttributeSet(BaseAS), PlayerAttributeSet(PlayerAS), CombatAttributeSet(CombatAS) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UEWBaseAttributeSet> BaseAttributeSet = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UEWPlayerAttributeSet> PlayerAttributeSet = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UEWCombatAttributeSet> CombatAttributeSet = nullptr;
};

// 属性值改变委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChangedWithTagSignature, FGameplayTag, AttributeTag, float, NewValue);

// UI消息委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FGameplayTag, MessageTag);

/**
 * Widget Controller 基类 - 作为UI和游戏逻辑之间的中间层
 */
UCLASS(BlueprintType, Blueprintable)
class DREAM_UE56_API UEWWidgetController : public UObject
{
	GENERATED_BODY()

public:
	// 设置Widget Controller参数
	UFUNCTION(BlueprintCallable, Category = "Widget Controller")
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	// 广播初始值
	UFUNCTION(BlueprintCallable, Category = "Widget Controller")
	virtual void BroadcastInitialValues();

	// 绑定依赖回调
	UFUNCTION(BlueprintCallable, Category = "Widget Controller")
	virtual void BindCallbacksToDependencies();

	// 广播消息
	UFUNCTION(BlueprintCallable, Category = "Widget Controller")
	void BroadcastMessage(FGameplayTag MessageTag);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<UEWBaseAttributeSet> BaseAttributeSet;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<UEWPlayerAttributeSet> PlayerAttributeSet;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<UEWCombatAttributeSet> CombatAttributeSet;

public:
	// 通用属性变化委托
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedWithTagSignature OnAttributeChanged;

	// 消息委托
	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;
};

/**
 * 属性菜单Widget Controller - 处理属性面板相关逻辑
 */
UCLASS(BlueprintType, Blueprintable)
class DREAM_UE56_API UEWAttributeMenuWidgetController : public UEWWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	// 属性点分配
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	// 属性升级委托
	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChangedSignature OnActionPointChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChangedSignature OnMaxActionPointChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChangedSignature OnPhysicalAttackChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChangedSignature OnMagicalAttackChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChangedSignature OnPhysicalDefenseChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChangedSignature OnMagicalDefenseChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChangedSignature OnMovementSpeedChanged;

private:
	void OnHealthChangedCallback(const FOnAttributeChangeData& Data);
	void OnMaxHealthChangedCallback(const FOnAttributeChangeData& Data);
	void OnManaChangedCallback(const FOnAttributeChangeData& Data);
	void OnMaxManaChangedCallback(const FOnAttributeChangeData& Data);
	void OnActionPointChangedCallback(const FOnAttributeChangeData& Data);
	void OnMaxActionPointChangedCallback(const FOnAttributeChangeData& Data);
	void OnPhysicalAttackChangedCallback(const FOnAttributeChangeData& Data);
	void OnMagicalAttackChangedCallback(const FOnAttributeChangeData& Data);
	void OnPhysicalDefenseChangedCallback(const FOnAttributeChangeData& Data);
	void OnMagicalDefenseChangedCallback(const FOnAttributeChangeData& Data);
};

/**
 * 重叠Widget Controller - 处理HUD overlay相关逻辑
 */
UCLASS(BlueprintType, Blueprintable)
class DREAM_UE56_API UEWOverlayWidgetController : public UEWWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	// 血量相关委托
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	// 魔法值相关委托
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	// 行动值相关委托
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnActionPointChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxActionPointChanged;

	// 等级相关委托
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnLevelChanged;
};

/**
 * 单位管理Widget Controller - 处理单位管理UI相关逻辑
 */
UCLASS(BlueprintType, Blueprintable)
class DREAM_UE56_API UEWUnitManagementWidgetController : public UEWWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	// 设置单位管理器
	UFUNCTION(BlueprintCallable, Category = "Unit Management")
	void SetUnitManager(class UEWUnitManager* Manager);

	// 单位操作
	UFUNCTION(BlueprintCallable, Category = "Unit Management")
	void SetBattleSlot(int32 BattleSlotIndex, int32 UnitSlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Unit Management")
	void ClearBattleSlot(int32 BattleSlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Unit Management")
	void SwapBattleSlots(int32 BattleSlotIndex1, int32 BattleSlotIndex2);

	UFUNCTION(BlueprintCallable, Category = "Unit Management")
	float GetSummonCost() const;

	UFUNCTION(BlueprintCallable, Category = "Unit Management")
	bool CanSummonAll() const;

	// 单位管理相关委托
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUnitSlotChanged, int32, SlotIndex, const struct FUnitSlotData&, SlotData);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBattleSlotChanged, int32, BattleSlotIndex, const struct FUnitSlotData&, SlotData);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSummonCostChanged, float, NewCost);

	UPROPERTY(BlueprintAssignable, Category = "Unit Management")
	FOnUnitSlotChanged OnUnitSlotChanged;

	UPROPERTY(BlueprintAssignable, Category = "Unit Management")
	FOnBattleSlotChanged OnBattleSlotChanged;

	UPROPERTY(BlueprintAssignable, Category = "Unit Management")
	FOnSummonCostChanged OnSummonCostChanged;

protected:
	UPROPERTY()
	TObjectPtr<class UEWUnitManager> UnitManager;

	// 回调函数
	UFUNCTION()
	void OnUnitSlotChangedCallback(int32 SlotIndex, const struct FUnitSlotData& SlotData);

	UFUNCTION()
	void OnBattleSlotChangedCallback(int32 BattleSlotIndex, const struct FUnitSlotData& SlotData);
};
