// Copyright Epic Games, Inc. All Rights Reserved.

#include "JetCharacter.h"
#include "JetProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"

#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AJetCharacter

AJetCharacter::AJetCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	//--------------Drfit��ʼ������---------------
	// ��ʼ��Ĭ����������
	DefaultGravityScale = 1.5f;

	// ����ʱ����������
	DriftGravityScale = 0.05f;

	// ���輤���ٶ���ֵ
	DriftVelocityThreshold = 0.0f;

	// ��ʼ״̬
	bIsDrifting = false;
	bIsJumpPressed = false;

	//-------------Tailwind----------------
	TailwindState = ESkillState::Idle;

	// ���ܲ�����ʼ��
	TailwindPrepareTime = 7.5f; // ׼��״̬����ʱ��
	TailwindWindupTime = 1.0f;  // ����ǰҡʱ��
	DashRange = 1500.0f;        // ��̷�Χ
	KillsToRefresh = 2;         // ��ɱ/������ˢ�´���
	CurrentKills = 0;           // ��ʼ��ɱ����
}

//////////////////////////////////////////////////////////////////////////// Input

void AJetCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AJetCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AJetCharacter::StartJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AJetCharacter::StopJump);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AJetCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AJetCharacter::Look);

		// Skills
		// Updraft Q ���
		EnhancedInputComponent->BindAction(UpdraftAction, ETriggerEvent::Triggered, this, &AJetCharacter::PerformUpdraft);

		// Tailwind E ���
		EnhancedInputComponent->BindAction(TailwindAction, ETriggerEvent::Started, this, &AJetCharacter::ActivateTailwind);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AJetCharacter::UpdateDrift(float DeltaTime)
{
	// ����Ƿ�������Ծ�ҽ�ɫ�ڿ���
	if (bIsJumpPressed && GetCharacterMovement()->IsFalling())
	{
		// ��ȡ��ǰ��ɫ�ٶ�
		FVector Velocity = GetVelocity();

		// ����ٶȵ��ڻ�����ֵ�����뻬��״̬
		if (Velocity.Z < DriftVelocityThreshold)
		{
			if (!bIsDrifting)
			{
				bIsDrifting = true;
				GetCharacterMovement()->GravityScale = DriftGravityScale; // ��������
				UE_LOG(LogTemp, Log, TEXT("Drift activated!"));
			}
		}
		else
		{
			// δ�ﵽ��������������Ĭ������
			GetCharacterMovement()->GravityScale = DefaultGravityScale;
			bIsDrifting = false;
		}
	}
	else
	{
		// �����㻬������������״̬
		GetCharacterMovement()->GravityScale = DefaultGravityScale;
		bIsDrifting = false;
	}
}

void AJetCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ���»���״̬
	UpdateDrift(DeltaTime);
}

void AJetCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AJetCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AJetCharacter::StartJump(const FInputActionValue& Value)
{
	bIsJumpPressed = true;
	Jump();
}

void AJetCharacter::StopJump(const FInputActionValue& Value)
{
	bIsJumpPressed = false;

	StopJumping();

	// �˳�����״̬���ָ�Ĭ������
	GetCharacterMovement()->GravityScale = DefaultGravityScale;
	bIsDrifting = false;
}

void AJetCharacter::PerformUpdraft(const FInputActionValue& Value)
{
	// ����ɫһ�����ϵĳ���
	FVector UpwardForce = FVector(0, 0, 1000.0f); // 1000 �����ϵ����ȣ����Ը�����Ҫ����
	LaunchCharacter(UpwardForce, true, true);

	// ��ӡ��־
	UE_LOG(LogTemplateCharacter, Log, TEXT("Updraft activated!"));
}

FVector AJetCharacter::GetDashDirection() const
{
	FVector ForwardDirection = GetActorForwardVector();
	FVector RightDirection = GetActorRightVector();

	float ForwardInput = InputComponent->GetAxisValue(TEXT("MoveForward"));
	float RightInput = InputComponent->GetAxisValue(TEXT("MoveRight"));

	FVector DashDirection = (ForwardDirection * ForwardInput) + (RightDirection * RightInput);
	DashDirection.Normalize();

	return DashDirection.IsZero() ? ForwardDirection : DashDirection;
}

void AJetCharacter::ActivateTailwind()
{
	switch (TailwindState)
	{
	case ESkillState::Idle:
		// ����׼��״̬
		TailwindState = ESkillState::Preparing;

		// ���ǰҡ��ʱ��
		GetWorld()->GetTimerManager().SetTimer(TailwindWindupTimer, [this]()
			{
				TailwindState = ESkillState::Active;
				UE_LOG(LogTemp, Log, TEXT("Tailwind is ready!"));
			}, TailwindWindupTime, false);

		// ���׼��״̬����ʱ���ʱ��
		GetWorld()->GetTimerManager().SetTimer(TailwindPrepareTimer, this, &AJetCharacter::CancelTailwind, TailwindPrepareTime, false);

		UE_LOG(LogTemp, Log, TEXT("Tailwind preparing..."));
		break;

	case ESkillState::Active:
		// ��׼��״̬��ִ�г��
		ExecuteDash();
		break;

	case ESkillState::Cooldown:
		UE_LOG(LogTemp, Warning, TEXT("Tailwind is on cooldown!"));
		break;

	default:
		break;
	}
}

void AJetCharacter::ExecuteDash()
{
	if (TailwindState != ESkillState::Active)
		return;

	// ��ȡ��̷���
	FVector DashDirection = GetDashDirection();

	if (DashDirection.IsZero())
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid dash direction!"));
		return;
	}

	// ��̣��� DashDirection �����ƶ� DashRange �ľ��룩
	LaunchCharacter(DashDirection * DashRange, true, true);

	// ���ü���Ϊ��ȴ״̬
	//TailwindState = ESkillState::Cooldown;

	// ���׼��״̬��ʱ��
	GetWorld()->GetTimerManager().ClearTimer(TailwindPrepareTimer);

	// ������ȴ�߼�����
	

	UE_LOG(LogTemp, Log, TEXT("Tailwind executed!"));
}

void AJetCharacter::CancelTailwind()
{
	if (TailwindState == ESkillState::Preparing || TailwindState == ESkillState::Active)
	{
		TailwindState = ESkillState::Idle;
		UE_LOG(LogTemp, Log, TEXT("Tailwind cancelled."));
	}
}
