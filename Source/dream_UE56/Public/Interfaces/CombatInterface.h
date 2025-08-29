#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"


class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegistered, UAbilitySystemComponent*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, AActor*, DeadActor);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDamageSignature, float /*DamageAmount*/);


UINTERFACE(MinimalAPI, Blueprintable)
class UCombatInterface : public UInterface
{
    GENERATED_BODY()
};

class DREAM_UE56_API ICombatInterface
{
    GENERATED_BODY()

public:
    virtual FOnASCRegistered& GetOnASCRegisteredDelegate() = 0;
    virtual FOnDeathSignature& GetOnDeathDelegate() = 0;
    virtual FOnDamageSignature& GetOnDamageSignature() = 0; 
    
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;
};