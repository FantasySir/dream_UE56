// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EWUnitAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

/**
 * 单位AI控制器
 * 负责控制AI单位的行为和决策
 */
UCLASS()
class DREAM_UE56_API AEWUnitAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEWUnitAIController();

protected:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

};
