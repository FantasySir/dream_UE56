// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/EWPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

AEWPlayerController::AEWPlayerController()
{
	// 设置默认值
}

void AEWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 获取Enhanced Input本地玩家子系统
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// 添加输入映射上下文
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AEWPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// 设置Enhanced Input组件
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// 绑定移动输入
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEWPlayerController::Move);
		}

		// 绑定视角输入
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AEWPlayerController::Look);
		}
	}
}

void AEWPlayerController::Move(const FInputActionValue& Value)
{
	// 获取输入向量
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn())
	{
		// 获取控制器的前向和右向向量
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 获取前向方向
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// 获取右向方向
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// 添加移动输入
		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AEWPlayerController::Look(const FInputActionValue& Value)
{
	// 获取鼠标输入向量
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (GetPawn())
	{
		// 添加偏航和俯仰输入
		AddYawInput(LookAxisVector.X);
		AddPitchInput(LookAxisVector.Y);
	}
}

