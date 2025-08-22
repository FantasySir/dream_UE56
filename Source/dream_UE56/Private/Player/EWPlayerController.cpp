// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/EWPlayerController.h"
#include "Character/EWCharacterBase.h"
#include "Character/EWUnitBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

AEWPlayerController::AEWPlayerController()
{
	// 设置默认值
}

void AEWPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (EWMappingContext)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		if(Subsystem)
		{
			Subsystem->AddMappingContext(EWMappingContext, 0);
		}
	}
}

void AEWPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	// 基础移动和视角
	if (MoveAction)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEWPlayerController::Move);
	}
	if (LookAction)
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AEWPlayerController::Look);
	}
	
	// 战斗相关
	if (ToggleLockAction)
	{
		EnhancedInputComponent->BindAction(ToggleLockAction, ETriggerEvent::Triggered, this, &AEWPlayerController::ToggleLock);
	}
	
	// 时间控制
	if (PauseTimeAction)
	{
		EnhancedInputComponent->BindAction(PauseTimeAction, ETriggerEvent::Triggered, this, &AEWPlayerController::PauseTime);
	}
	if (ResumeTimeAction)
	{
		EnhancedInputComponent->BindAction(ResumeTimeAction, ETriggerEvent::Triggered, this, &AEWPlayerController::ResumeTime);
	}
	
	// 召唤系统
	if (SummonUnit1Action)
	{
		EnhancedInputComponent->BindAction(SummonUnit1Action, ETriggerEvent::Triggered, this, &AEWPlayerController::SummonUnit1);
	}
	if (SummonUnit2Action)
	{
		EnhancedInputComponent->BindAction(SummonUnit2Action, ETriggerEvent::Triggered, this, &AEWPlayerController::SummonUnit2);
	}
	if (SummonUnit3Action)
	{
		EnhancedInputComponent->BindAction(SummonUnit3Action, ETriggerEvent::Triggered, this, &AEWPlayerController::SummonUnit3);
	}
	
	// 技能系统
	if (Skill1Action)
	{
		EnhancedInputComponent->BindAction(Skill1Action, ETriggerEvent::Triggered, this, &AEWPlayerController::CastSkill1);
	}
	if (Skill2Action)
	{
		EnhancedInputComponent->BindAction(Skill2Action, ETriggerEvent::Triggered, this, &AEWPlayerController::CastSkill2);
	}
	if (Skill3Action)
	{
		EnhancedInputComponent->BindAction(Skill3Action, ETriggerEvent::Triggered, this, &AEWPlayerController::CastSkill3);
	}
	if (Skill4Action)
	{
		EnhancedInputComponent->BindAction(Skill4Action, ETriggerEvent::Triggered, this, &AEWPlayerController::CastSkill4);
	}
}

void AEWPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AEWPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn())
	{
		// 检查是否在锁定模式
		AEWCharacterBase* EWCharacter = GetControlledCharacter();
		if (EWCharacter && EWCharacter->GetLockedTarget())
		{
			LockLook(Value);
		}
		else
		{
			// 普通视角控制
			AddYawInput(LookAxisVector.X);
			AddPitchInput(LookAxisVector.Y);
		}
	}
}

void AEWPlayerController::ToggleLock(const FInputActionValue& Value)
{
	AEWCharacterBase* EWCharacter = GetControlledCharacter();
	if (!EWCharacter)
	{
		return;
	}

	// 如果已经锁定了目标，则解锁
	if (EWCharacter->GetLockedTarget())
	{
		EWCharacter->UnlockTarget();
		return;
	}

	// 寻找最近的可锁定目标
	AEWUnitBase* Target = FindNearestLockableTarget();
	if (Target)
	{
		EWCharacter->LockTarget(Target);
	}
}

void AEWPlayerController::PauseTime(const FInputActionValue& Value)
{
	AEWCharacterBase* EWCharacter = GetControlledCharacter();
	if (EWCharacter && EWCharacter->CanPauseTime())
	{
		UE_LOG(LogTemp, Warning, TEXT("Pausing time for character: %s"), *EWCharacter->GetName());
		EWCharacter->PauseTime();
	}
}

void AEWPlayerController::ResumeTime(const FInputActionValue& Value)
{
	AEWCharacterBase* EWCharacter = GetControlledCharacter();
	if (EWCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Resuming time for character: %s"), *EWCharacter->GetName());
		EWCharacter->ResumeTime();
	}
}

void AEWPlayerController::SummonUnit1(const FInputActionValue& Value)
{
	// TODO: 实现召唤单位1的逻辑
	UE_LOG(LogTemp, Warning, TEXT("SummonUnit1 triggered"));
}

void AEWPlayerController::SummonUnit2(const FInputActionValue& Value)
{
	// TODO: 实现召唤单位2的逻辑
	UE_LOG(LogTemp, Warning, TEXT("SummonUnit2 triggered"));
}

void AEWPlayerController::SummonUnit3(const FInputActionValue& Value)
{
	// TODO: 实现召唤单位3的逻辑
	UE_LOG(LogTemp, Warning, TEXT("SummonUnit3 triggered"));
}

void AEWPlayerController::CastSkill1(const FInputActionValue& Value)
{
	// TODO: 实现技能1的逻辑
	UE_LOG(LogTemp, Warning, TEXT("CastSkill1 triggered"));
}

void AEWPlayerController::CastSkill2(const FInputActionValue& Value)
{
	// TODO: 实现技能2的逻辑
	UE_LOG(LogTemp, Warning, TEXT("CastSkill2 triggered"));
}

void AEWPlayerController::CastSkill3(const FInputActionValue& Value)
{
	// TODO: 实现技能3的逻辑
	UE_LOG(LogTemp, Warning, TEXT("CastSkill3 triggered"));
}

void AEWPlayerController::CastSkill4(const FInputActionValue& Value)
{
	// TODO: 实现技能4的逻辑
	UE_LOG(LogTemp, Warning, TEXT("CastSkill4 triggered"));
}

AEWUnitBase* AEWPlayerController::FindNearestLockableTarget()
{
	AEWCharacterBase* EWCharacter = GetControlledCharacter();
	if (!EWCharacter)
	{
		return nullptr;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	FVector CharacterLocation = EWCharacter->GetActorLocation();
	AEWUnitBase* NearestTarget = nullptr;
	float NearestDistance = LockRange;

	// 遍历所有EWUnitBase类型的Actor
	for (TActorIterator<AEWUnitBase> ActorItr(World); ActorItr; ++ActorItr)
	{
		AEWUnitBase* Unit = *ActorItr;
		if (!Unit || Unit == static_cast<AActor*>(EWCharacter) || !Unit->IsAlive())
		{
			continue;
		}

		float Distance = FVector::Dist(CharacterLocation, Unit->GetActorLocation());
		if (Distance < NearestDistance)
		{
			NearestDistance = Distance;
			NearestTarget = Unit;
		}
	}

	return NearestTarget;
}

AEWCharacterBase* AEWPlayerController::GetControlledCharacter()
{
	return Cast<AEWCharacterBase>(GetPawn());
}

void AEWPlayerController::LockLook(const FInputActionValue& Value)
{
	// 锁定模式下的相机控制
	// 这里可以实现围绕锁定目标的相机控制逻辑
	AEWCharacterBase* EWCharacter = GetControlledCharacter();
	if (!EWCharacter || !EWCharacter->GetLockedTarget())
	{
		return;
	}

	FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	// 可以实现围绕目标的相机控制
	// 这里先实现基础的视角控制
	AddYawInput(LookAxisVector.X * 0.5f); // 降低锁定模式下的灵敏度
	AddPitchInput(LookAxisVector.Y * 0.5f);
}
