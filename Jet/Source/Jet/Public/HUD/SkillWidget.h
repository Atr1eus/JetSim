// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Jet/JetCharacter.h"
#include "SkillWidget.generated.h"



class UButton;
class UImage;
class UProgressBar;
class AJetCharacter;

//UENUM(BlueprintType)
//enum class ESkillState : uint8
//{
//	Idle UMETA(DisplayName = "Idle"),            // 技能未使用
//	Preparing UMETA(DisplayName = "Preparing"), // 技能准备中（如按住键不放）
//	Active UMETA(DisplayName = "Active"),       // 技能激活中
//	Cooldown UMETA(DisplayName = "Cooldown"),   // 技能冷却中
//	Disabled UMETA(DisplayName = "Disabled")    // 技能被禁用（如受到特定效果）
//};
/**
 * 
 */
UCLASS()
class JET_API USkillWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloudBurstButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> UpdraftButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> TailWindButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StormBladeButton;

	UPROPERTY(meta = (BindWidget))
	UImage* CloudBurstIcon;

	UPROPERTY(meta = (BindWidget))
	UImage* UpdraftIcon;

	UPROPERTY(meta = (BindWidget))
	UImage* TailWindIcon;

	UPROPERTY(meta = (BindWidget))
	UImage* StormBladeIcon;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* TailWindProgressBar;

	AJetCharacter* PlayerCharacter;

	FTimerHandle ProgressBarTimerHandle; // 定时器句柄

	float TargetProgress = 0.0f;


	void UpdateUpdraftButton(ESkillState SkillState);

	void UpdateTailwindButton(ESkillState SkillState);

	void ShowBar();

	void UpdateProgressBar();

	void HideBar();

protected:
	virtual void NativeConstruct() override;
};
