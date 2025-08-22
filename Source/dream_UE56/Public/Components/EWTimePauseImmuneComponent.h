// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EWTimePauseImmuneComponent.generated.h"

/**
 * 时间暂停免疫组件
 * 为Actor提供时间暂停免疫能力，可以让特定的技能效果、粒子系统等不受时间暂停影响
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DREAM_UE56_API UEWTimePauseImmuneComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEWTimePauseImmuneComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called when the component is destroyed
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// 设置是否免疫时间暂停
	UFUNCTION(BlueprintCallable, Category = "Time Control")
	void SetTimePauseImmune(bool bImmune);

	// 检查是否免疫时间暂停
	UFUNCTION(BlueprintCallable, Category = "Time Control")
	bool IsTimePauseImmune() const { return bIsTimePauseImmune; }

protected:
	// 是否免疫时间暂停
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Control")
	bool bIsTimePauseImmune = true;

	// 是否在游戏开始时自动注册为免疫
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Control")
	bool bAutoRegisterOnBeginPlay = true;
};
