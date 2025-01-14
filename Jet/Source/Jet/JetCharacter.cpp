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

	//--------------Drfit初始化参数---------------
	// 初始化默认重力比例
	DefaultGravityScale = 1.5f;

	// 滑翔时的重力比例
	DriftGravityScale = 0.05f;

	// 滑翔激活速度阈值
	DriftVelocityThreshold = 0.0f;

	// 初始状态
	bIsDrifting = false;
	bIsJumpPressed = false;

	//-------------Tailwind----------------
	TailwindState = ESkillState::Idle;

	// 技能参数初始化
	TailwindPrepareTime = 7.5f; // 准备状态持续时间
	TailwindWindupTime = 1.0f;  // 激活前摇时间
	DashRange = 1500.0f;        // 冲刺范围
	KillsToRefresh = 2;         // 击杀/助攻后刷新次数
	CurrentKills = 0;           // 初始击杀次数
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
		// Updraft Q 凌空
		EnhancedInputComponent->BindAction(UpdraftAction, ETriggerEvent::Triggered, this, &AJetCharacter::PerformUpdraft);

		// Tailwind E 逐风
		EnhancedInputComponent->BindAction(TailwindAction, ETriggerEvent::Started, this, &AJetCharacter::ActivateTailwind);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AJetCharacter::UpdateDrift(float DeltaTime)
{
	// 检查是否正在跳跃且角色在空中
	if (bIsJumpPressed && GetCharacterMovement()->IsFalling())
	{
		// 获取当前角色速度
		FVector Velocity = GetVelocity();

		// 如果速度低于滑翔阈值，进入滑翔状态
		if (Velocity.Z < DriftVelocityThreshold)
		{
			if (!bIsDrifting)
			{
				bIsDrifting = true;
				GetCharacterMovement()->GravityScale = DriftGravityScale; // 降低重力
				UE_LOG(LogTemp, Log, TEXT("Drift activated!"));
			}
		}
		else
		{
			// 未达到滑翔条件，保持默认重力
			GetCharacterMovement()->GravityScale = DefaultGravityScale;
			bIsDrifting = false;
		}
	}
	else
	{
		// 不满足滑翔条件，重置状态
		GetCharacterMovement()->GravityScale = DefaultGravityScale;
		bIsDrifting = false;
	}
}

void AJetCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 更新滑翔状态
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

	// 退出滑翔状态，恢复默认重力
	GetCharacterMovement()->GravityScale = DefaultGravityScale;
	bIsDrifting = false;
}

void AJetCharacter::PerformUpdraft(const FInputActionValue& Value)
{
	// 给角色一个向上的冲量
	FVector UpwardForce = FVector(0, 0, 1000.0f); // 1000 是向上的力度，可以根据需要调整
	LaunchCharacter(UpwardForce, true, true);

	// 打印日志
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
		// 进入准备状态
		TailwindState = ESkillState::Preparing;

		// 添加前摇计时器
		GetWorld()->GetTimerManager().SetTimer(TailwindWindupTimer, [this]()
			{
				TailwindState = ESkillState::Active;
				UE_LOG(LogTemp, Log, TEXT("Tailwind is ready!"));
			}, TailwindWindupTime, false);

		// 添加准备状态持续时间计时器
		GetWorld()->GetTimerManager().SetTimer(TailwindPrepareTimer, this, &AJetCharacter::CancelTailwind, TailwindPrepareTime, false);

		UE_LOG(LogTemp, Log, TEXT("Tailwind preparing..."));
		break;

	case ESkillState::Active:
		// 在准备状态下执行冲刺
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

	// 获取冲刺方向
	FVector DashDirection = GetDashDirection();

	if (DashDirection.IsZero())
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid dash direction!"));
		return;
	}

	// 冲刺（向 DashDirection 方向移动 DashRange 的距离）
	LaunchCharacter(DashDirection * DashRange, true, true);

	// 设置技能为冷却状态
	//TailwindState = ESkillState::Cooldown;

	// 清除准备状态计时器
	GetWorld()->GetTimerManager().ClearTimer(TailwindPrepareTimer);

	// 技能冷却逻辑（）
	

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
