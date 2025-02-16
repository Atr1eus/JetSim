// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/BeginMenuWidget.h"
#include "JetController.h"
#include "Components/Button.h"
#include "MediaPlayer.h"
#include "MediaSoundComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/Texture2D.h"
#include "GameFramework/PlayerController.h"

void UBeginMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 按钮绑定点击事件
	if (PlayButton != nullptr)
	{
		PlayButton->OnClicked.AddDynamic(this, &UBeginMenuWidget::OnPlayClicked);
	}

	if (ExitButton != nullptr)
	{
		ExitButton->OnClicked.AddDynamic(this, &UBeginMenuWidget::OnExitClicked);
	}

	// 设置视频背景
	SetupVideoBackground();
}

void UBeginMenuWidget::OnPlayClicked()
{
	AJetController* PlayerController = Cast<AJetController>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		//Hide
		PlayerController->DestroyTopmostWidget();
	}
	

}

void UBeginMenuWidget::OnExitClicked()
{
	// 退出游戏
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}

void UBeginMenuWidget::SetupVideoBackground()
{
	// 创建并设置 MediaPlayer
	if (MediaPlayer == nullptr)
	{
		MediaPlayer = NewObject<UMediaPlayer>(this);
	}
}
