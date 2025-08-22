// Fill out your copyright notice in the Description page of Project Settings.

#include "dream_UE56.h"
#include "Core/EWGameplayTags.h"
#include "Modules/ModuleManager.h"

class FDreamUE56Module : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override
	{
		FDefaultGameModuleImpl::StartupModule();
		
		// 初始化GameplayTags
		FEWGameplayTags::InitializeNativeTags();
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE( FDreamUE56Module, dream_UE56, "dream_UE56" );
