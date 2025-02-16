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
//	Idle UMETA(DisplayName = "Idle"),            // ����δʹ��
//	Preparing UMETA(DisplayName = "Preparing"), // ����׼���У��簴ס�����ţ�
//	Active UMETA(DisplayName = "Active"),       // ���ܼ�����
//	Cooldown UMETA(DisplayName = "Cooldown"),   // ������ȴ��
//	Disabled UMETA(DisplayName = "Disabled")    // ���ܱ����ã����ܵ��ض�Ч����
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

	FTimerHandle ProgressBarTimerHandle; // ��ʱ�����

	float TargetProgress = 0.0f;


	void UpdateUpdraftButton(ESkillState SkillState);

	void UpdateTailwindButton(ESkillState SkillState);

	void ShowBar();

	void UpdateProgressBar();

	void HideBar();

protected:
	virtual void NativeConstruct() override;
};
