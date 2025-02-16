// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"

#include "Public/Component/HealthComponent.h"
#include "Public/Interface/HealthInterface.h"
#include <Component/SpikeInstComponent.h>

#include "JetCharacter.generated.h"




class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

//����ֵ
class UHealthComponent;
class AJetHUD;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

// ͨ�ü���״̬ö��
UENUM(BlueprintType)
enum class ESkillState : uint8
{
	Idle UMETA(DisplayName = "Idle"),            // ����δʹ��
	Preparing UMETA(DisplayName = "Preparing"), // ����׼���У��簴ס�����ţ�
	Active UMETA(DisplayName = "Active"),       // ���ܼ�����
	Cooldown UMETA(DisplayName = "Cooldown"),   // ������ȴ��
	Disabled UMETA(DisplayName = "Disabled")    // ���ܱ����ã����ܵ��ض�Ч����
};

UCLASS(config=Game)
class AJetCharacter : public ACharacter, public IHealthInterface
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;
	
	//HUD Menu
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> EscAction;

	//Skills
	/** Updraft Input Action Q�������*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> UpdraftAction;

	/** Tailwind Input Action E�������*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> TailwindAction;

public:
	AJetCharacter();

	AJetHUD* JetHUD;
	//-----------------------Drift ������Ư��------------------------------
	// Ĭ����������
	float DefaultGravityScale;

	// ����ʱ����������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drift", meta = (AllowPrivateAccess = "true"))
	float DriftGravityScale;

	// ���輤���ٶ���ֵ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drift", meta = (AllowPrivateAccess = "true"))
	float DriftVelocityThreshold;

	// �Ƿ��ڻ���״̬
	bool bIsDrifting;

	// �Ƿ�������Ծ��������Ծ����
	bool bIsJumpPressed;

	// ���»���״̬
	void UpdateDrift(float DeltaTime);

	//-------------------------------Tailwind-----------------------------------
	// ����״̬
	ESkillState TailwindState;

	// ��̷������
	FVector GetDashDirection() const;

	// Tailwind ���ܲ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills", meta = (AllowPrivateAccess = "true"))
	float TailwindPrepareTime; // ׼��״̬����ʱ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills", meta = (AllowPrivateAccess = "true"))
	float TailwindWindupTime; // ����ǰҡʱ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills", meta = (AllowPrivateAccess = "true"))
	float DashRange; // ��̷�Χ��15 �ף�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills", meta = (AllowPrivateAccess = "true"))
	int32 KillsToRefresh; // ��ɱ/������ˢ�¼����������

	int32 CurrentKills; // ��ǰ��ɱ/��������

	UFUNCTION()
	void IncreaseKillCount();

	//-------------------------Updraft------------------------------
	ESkillState UpdraftState;

	// ״̬��ʱ��
	FTimerHandle TailwindPrepareTimer;
	FTimerHandle TailwindWindupTimer;

	// ��װ���̵�״̬
	// ����Ƿ��ڰ�װ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone")
	bool bIsPlayerInZone;
	bool bIsInstallingSpike;
	UPROPERTY(BlueprintReadWrite, Category = "Game State")
	bool bIsSpikeInstalled;
	float InstallProgress;

protected:
	virtual void BeginPlay() override;

	//TICK
	virtual void Tick(float DeltaTime) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	//�������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UHealthComponent> HealthComponent;

	//�������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<USpikeInstComponent> SpikeComponent;

	// Drift P Ʈ��
	void StartJump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);

	// Updarft Q ��� ʵ�ֺ���
	void PerformUpdraft(const FInputActionValue& Value);

	// Tailwind E ���
	UFUNCTION(BlueprintCallable, Category = "Skills")
	void ActivateTailwind(); // ���� E �Ĵ����߼�
	void ExecuteDash();      // ����߼�
	void CancelTailwind();   // ׼��״̬������ȡ������
	void ResetTailwindCharge(); // ��ɱ/������ˢ�¼���
	

protected:
	// APawn interface
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	TObjectPtr<USkeletalMeshComponent> GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	TObjectPtr<UCameraComponent> GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }


	// ͨ�� IHealthInterface �̳�
	void Damage_Implementation() override;

	void Death_Implementation() override;

};

