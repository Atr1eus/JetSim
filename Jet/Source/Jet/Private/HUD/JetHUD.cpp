// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/JetHUD.h"
#include "HUD/HealthWidget.h"
#include "JetController.h"

void AJetHUD::BeginPlay()
{
	Super::BeginPlay();
	
}

void AJetHUD::CreateHealthWidget()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (HealthWidgetClass != nullptr)
	{
		WidgetInstance = CreateWidget<UHealthWidget>(PlayerController, HealthWidgetClass);
		WidgetInstance->AddToViewport();
	}
}

void AJetHUD::UpdateHealth(float HealthPercent)
{
	if (WidgetInstance != nullptr)
	{
		WidgetInstance->UpdateHealthPercent(HealthPercent);
	}
}

void AJetHUD::CreateBeginMenuWidget()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (BeginMenuWidgetClass != nullptr)
	{
		MenuWidgetInstance = CreateWidget<UBeginMenuWidget>(PlayerController, BeginMenuWidgetClass);
		MenuWidgetInstance->AddToViewport();
	}
}

void AJetHUD::ShowEndGameUI(bool bVictory)
{
	if (EndGameWidgetClass)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			PlayerController->SetPause(true);
			PlayerController->SetInputMode(FInputModeUIOnly());

			PlayerController->bShowMouseCursor = true;
		}
		
		// 创建EndGameWidget实例
		UEndGameWidget* EndGameWidget = CreateWidget<UEndGameWidget>(GetWorld(), EndGameWidgetClass);
		if (EndGameWidget)
		{
			// 设置文本和按钮事件
			EndGameWidget->SetVictoryOrDefeated(bVictory);  // 传递胜利/失败状态
			// 显示Widget
			EndGameWidget->AddToViewport();
		}
	}
}

void AJetHUD::ShowGameState()
{
	if (GameStateWidgetClass)
	{
		GameStateWidget = CreateWidget<UGameStateWidget>(GetWorld(), GameStateWidgetClass);
		if (GameStateWidget)
		{
			GameStateWidget->AddToViewport();
		}
	}
}

void AJetHUD::UpdateGameState(bool SpikePlanted, int32 time)
{
	GameStateWidget->UpdateGameState(time, SpikePlanted);
}

void AJetHUD::ShowSkillState()
{
	if (SkillWidgetClass)
	{
		SkillWidgetInstance = CreateWidget<USkillWidget>(GetWorld(), SkillWidgetClass);
		if (SkillWidgetInstance)
		{
			SkillWidgetInstance->AddToViewport();
		}
	}
}

void AJetHUD::UpdateUpdraftState(ESkillState Skillstate)
{
	SkillWidgetInstance->UpdateUpdraftButton(Skillstate);
}

void AJetHUD::UpdateTailwindState(ESkillState Skillstate)
{
	SkillWidgetInstance->UpdateTailwindButton(Skillstate);
}


void AJetHUD::DrawHUD()
{
	if (CrosshairTexture)
    {
		int32 Width, Height;
		GetWorld()->GetFirstPlayerController()->GetViewportSize(Width, Height);
		FVector2D ScreenSize(Width, Height);

        FVector2D CrosshairPosition = FVector2D(ScreenSize.X * 0.5f, ScreenSize.Y * 0.5f);
		// 绘制水平线
		DrawLine(CrosshairPosition.X - 16, CrosshairPosition.Y, CrosshairPosition.X + 16, CrosshairPosition.Y, FLinearColor::White, 1.0f);

		// 绘制垂直线
		DrawLine(CrosshairPosition.X, CrosshairPosition.Y - 16, CrosshairPosition.X, CrosshairPosition.Y + 16, FLinearColor::White, 1.0f);
    }
}
