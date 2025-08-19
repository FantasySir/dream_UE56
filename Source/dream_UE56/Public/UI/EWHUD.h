// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/EWWidgetController.h"
#include "EWHUD.generated.h"

class UEWUserWidget;
class UEWOverlayWidgetController;
class UEWAttributeMenuWidgetController;
class UEWUnitManagementWidgetController;
class UAbilitySystemComponent;
class UAttributeSet;

/**
 * EW HUD类 - 管理所有UI元素
 */
UCLASS()
class DREAM_UE56_API AEWHUD : public AHUD
{
	GENERATED_BODY()

public:
	// 初始化Overlay
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	// 获取Overlay Widget Controller
	UEWOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	// 获取属性菜单Widget Controller
	UEWAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);

	// 获取单位管理Widget Controller
	UEWUnitManagementWidgetController* GetUnitManagementWidgetController(const FWidgetControllerParams& WCParams);

	// 显示/隐藏属性菜单
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowAttributeMenu();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideAttributeMenu();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ToggleAttributeMenu();

	// 显示/隐藏单位管理界面
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowUnitManagement();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideUnitManagement();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ToggleUnitManagement();

protected:
	virtual void BeginPlay() override;

private:
	// Overlay UI相关
	UPROPERTY()
	TObjectPtr<UEWUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UEWUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UEWOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UEWOverlayWidgetController> OverlayWidgetControllerClass;

	// 属性菜单相关
	UPROPERTY()
	TObjectPtr<UEWUserWidget> AttributeMenuWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UEWUserWidget> AttributeMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<UEWAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UEWAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	// 单位管理相关
	UPROPERTY()
	TObjectPtr<UEWUserWidget> UnitManagementWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UEWUserWidget> UnitManagementWidgetClass;

	UPROPERTY()
	TObjectPtr<UEWUnitManagementWidgetController> UnitManagementWidgetController;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UEWUnitManagementWidgetController> UnitManagementWidgetControllerClass;
};
