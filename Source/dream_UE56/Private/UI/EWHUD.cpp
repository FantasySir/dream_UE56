// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/EWHUD.h"
#include "UI/EWUserWidget.h"
#include "UI/EWWidgetController.h"
#include "Character/EWCharacterBase.h"
#include "AbilitySystem/AttributeSets/EWBaseAttributeSet.h"
#include "AbilitySystem/AttributeSets/EWPlayerAttributeSet.h"
#include "AbilitySystem/AttributeSets/EWCombatAttributeSet.h"
#include "Player/EWUnitManager.h"
#include "AttributeSet.h"
#include "GameFramework/PlayerState.h"

void AEWHUD::BeginPlay()
{
	Super::BeginPlay();
}

void AEWHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, 
	UEWBaseAttributeSet* BaseAS, UEWPlayerAttributeSet* PlayerAS, UEWCombatAttributeSet* CombatAS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_HUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_HUD"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UEWUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, BaseAS, PlayerAS, CombatAS);
	UEWOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	
	Widget->AddToViewport();
}

UEWOverlayWidgetController* AEWHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UEWOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UEWAttributeMenuWidgetController* AEWHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UEWAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

UEWUnitManagementWidgetController* AEWHUD::GetUnitManagementWidgetController(const FWidgetControllerParams& WCParams)
{
	if (UnitManagementWidgetController == nullptr)
	{
		UnitManagementWidgetController = NewObject<UEWUnitManagementWidgetController>(this, UnitManagementWidgetControllerClass);
		UnitManagementWidgetController->SetWidgetControllerParams(WCParams);
		
		// 设置单位管理器
		if (AEWCharacterBase* Character = Cast<AEWCharacterBase>(WCParams.PlayerController->GetPawn()))
		{
			UEWUnitManager* UnitManager = Character->GetUnitManager();
			UnitManagementWidgetController->SetUnitManager(UnitManager);
		}
	}
	return UnitManagementWidgetController;
}

void AEWHUD::ShowAttributeMenu()
{
	if (!AttributeMenuWidget && AttributeMenuWidgetClass)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), AttributeMenuWidgetClass);
		AttributeMenuWidget = Cast<UEWUserWidget>(Widget);
		
		if (AttributeMenuWidget)
		{
			// 获取Widget Controller参数
			APlayerController* PC = GetOwningPlayerController();
			APlayerState* PS = PC->GetPlayerState<APlayerState>();
			
			if (AEWCharacterBase* Character = Cast<AEWCharacterBase>(PC->GetPawn()))
			{
				UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
				UEWBaseAttributeSet* BaseAS = Character->GetBaseAttributeSet();
				UEWPlayerAttributeSet* PlayerAS = Character->GetPlayerAttributeSet();
				UEWCombatAttributeSet* CombatAS = Character->GetCombatAttributeSet();
				
				const FWidgetControllerParams WCParams(PC, PS, ASC, BaseAS, PlayerAS, CombatAS);
				UEWAttributeMenuWidgetController* WidgetController = GetAttributeMenuWidgetController(WCParams);
				
				AttributeMenuWidget->SetWidgetController(WidgetController);
				WidgetController->BroadcastInitialValues();
			}
		}
	}
	
	if (AttributeMenuWidget)
	{
		AttributeMenuWidget->AddToViewport();
	}
}

void AEWHUD::HideAttributeMenu()
{
	if (AttributeMenuWidget)
	{
		AttributeMenuWidget->RemoveFromParent();
	}
}

void AEWHUD::ToggleAttributeMenu()
{
	if (AttributeMenuWidget && AttributeMenuWidget->IsInViewport())
	{
		HideAttributeMenu();
	}
	else
	{
		ShowAttributeMenu();
	}
}

void AEWHUD::ShowUnitManagement()
{
	if (!UnitManagementWidget && UnitManagementWidgetClass)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), UnitManagementWidgetClass);
		UnitManagementWidget = Cast<UEWUserWidget>(Widget);
		
		if (UnitManagementWidget)
		{
			// 获取Widget Controller参数
			APlayerController* PC = GetOwningPlayerController();
			APlayerState* PS = PC->GetPlayerState<APlayerState>();
			
			if (AEWCharacterBase* Character = Cast<AEWCharacterBase>(PC->GetPawn()))
			{
				UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
				UEWBaseAttributeSet* BaseAS = Character->GetBaseAttributeSet();
				UEWPlayerAttributeSet* PlayerAS = Character->GetPlayerAttributeSet();
				UEWCombatAttributeSet* CombatAS = Character->GetCombatAttributeSet();
				
				const FWidgetControllerParams WCParams(PC, PS, ASC, BaseAS, PlayerAS, CombatAS);
				UEWUnitManagementWidgetController* WidgetController = GetUnitManagementWidgetController(WCParams);
				
				UnitManagementWidget->SetWidgetController(WidgetController);
				WidgetController->BroadcastInitialValues();
			}
		}
	}
	
	if (UnitManagementWidget)
	{
		UnitManagementWidget->AddToViewport();
	}
}

void AEWHUD::HideUnitManagement()
{
	if (UnitManagementWidget)
	{
		UnitManagementWidget->RemoveFromParent();
	}
}

void AEWHUD::ToggleUnitManagement()
{
	if (UnitManagementWidget && UnitManagementWidget->IsInViewport())
	{
		HideUnitManagement();
	}
	else
	{
		ShowUnitManagement();
	}
}
