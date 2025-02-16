// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "HealthWidget.h"
#include "BeginMenuWidget.h"
#include "EndGameWidget.h"
#include "SkillWidget.h"
#include "GameStateWidget.h"
#include "Jet/JetCharacter.h"
#include "JetHUD.generated.h"

class UHealthWidget;
class UBeginMenuWidget;

//class ESkillState;
/**
 * 
 */
UCLASS()
class JET_API AJetHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UHealthWidget> HealthWidgetClass;
	UPROPERTY()
	TObjectPtr<UHealthWidget> WidgetInstance;
	
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UBeginMenuWidget> BeginMenuWidgetClass;
	UPROPERTY()
	TObjectPtr<UBeginMenuWidget> MenuWidgetInstance;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UEndGameWidget> EndGameWidgetClass;
	UPROPERTY()
	TObjectPtr<UEndGameWidget> EndGameWidgetInstance;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<USkillWidget> SkillWidgetClass;
	UPROPERTY()
	TObjectPtr<USkillWidget> SkillWidgetInstance;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UGameStateWidget> GameStateWidgetClass;
	UPROPERTY()
	TObjectPtr<UGameStateWidget> GameStateWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair")
	UTexture2D* CrosshairTexture;

	void CreateHealthWidget();

	void UpdateHealth(float HealthPercent);

	void CreateBeginMenuWidget();

	void ShowEndGameUI(bool bVictory);
	
	void ShowGameState();

	void UpdateGameState(bool SpikePlanted, int32 time);

	void ShowSkillState();

	void UpdateUpdraftState(ESkillState Skillstate);

	void UpdateTailwindState(ESkillState Skillstate);

	virtual void DrawHUD() override;
};
