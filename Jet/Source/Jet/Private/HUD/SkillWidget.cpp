// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SkillWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Jet/JetCharacter.h"

void USkillWidget::UpdateUpdraftButton(ESkillState SkillState)
{
    switch (SkillState)
    {
    case ESkillState::Idle:
        // 处理 Updraft 技能的空闲状态
        if (UpdraftButton)
        {
            FLinearColor ButtonColor = FLinearColor(0, 1, 0, 1) ;
            FButtonStyle NewStyle = UpdraftButton->WidgetStyle;
            NewStyle.Normal.TintColor = FSlateColor(ButtonColor);
            UpdraftButton->SetStyle(NewStyle);
        }
        break;
    default:
        if (UpdraftButton)
        {
            FLinearColor ButtonColor = FLinearColor(0.5, 0.5, 0.5, 1);
            FButtonStyle NewStyle = UpdraftButton->WidgetStyle;
            NewStyle.Normal.TintColor = FSlateColor(ButtonColor);
            UpdraftButton->SetStyle(NewStyle);
        }
        break;
    }
}

void USkillWidget::UpdateTailwindButton(ESkillState SkillState)
{
    switch (SkillState)
    {
    case ESkillState::Idle:
        // 处理 Updraft 技能的空闲状态
        if (TailWindProgressBar)
        {
            TailWindProgressBar->SetVisibility(ESlateVisibility::Hidden);  // 隐藏进度条
        }

        if (TailWindButton)
        {
            FLinearColor ButtonColor = FLinearColor(0, 1, 0, 1);
            FButtonStyle NewStyle = TailWindButton->WidgetStyle;
            NewStyle.Normal.TintColor = FSlateColor(ButtonColor);
            TailWindButton->SetStyle(NewStyle);
        }
        break;
    case ESkillState::Active:
        // 处理 Updraft 技能的激活状态
        if (TailWindButton)
        {
            FLinearColor ButtonColor = FLinearColor(0.5, 0.5, 0.5, 1);
            FButtonStyle NewStyle = TailWindButton->WidgetStyle;
            NewStyle.Normal.TintColor = FSlateColor(ButtonColor);
            TailWindButton->SetStyle(NewStyle);

            ShowBar();
        }
        break;
    default:
        if (TailWindProgressBar)
        {
            TailWindProgressBar->SetVisibility(ESlateVisibility::Hidden);  // 隐藏进度条
        }

        if (TailWindButton)
        {
            FLinearColor ButtonColor = FLinearColor(0.5, 0.5, 0.5, 1);
            FButtonStyle NewStyle = TailWindButton->WidgetStyle;
            NewStyle.Normal.TintColor = FSlateColor(ButtonColor);
            TailWindButton->SetStyle(NewStyle);
        }
        break;
    }
}

void USkillWidget::ShowBar()
{
    TailWindProgressBar->SetVisibility(ESlateVisibility::Visible);
    // 根据时间更新进度条
    TailWindProgressBar->SetPercent(0.0f);

    // 启动7.5秒的计时器，每秒更新一次进度条
    TargetProgress = 1.0f;  // 目标是填充到 100%

    // 启动定时器，每秒触发一次
    GetWorld()->GetTimerManager().SetTimer(
        ProgressBarTimerHandle,
        this,
        &USkillWidget::UpdateProgressBar,
        7.5f / 100.0f,  // 每次更新的时间间隔，控制匀速填充
        true,  // 循环调用
        0.0f   // 延迟0秒启动
    );
}

void USkillWidget::UpdateProgressBar()
{
    // 获取当前的进度
    float CurrentProgress = TailWindProgressBar->Percent;

    // 计算进度条更新值
    CurrentProgress += 0.01f;  // 每次递增1%

    // 更新进度条
    TailWindProgressBar->SetPercent(CurrentProgress);

    // 如果进度条填充完毕，停止定时器
    if (CurrentProgress >= TargetProgress)
    {
        // 停止定时器
        GetWorld()->GetTimerManager().ClearTimer(ProgressBarTimerHandle);
    }

}

void USkillWidget::HideBar()
{
    if (TailWindProgressBar)
    {
        TailWindProgressBar->SetPercent(0.0f);
        TailWindProgressBar->SetVisibility(ESlateVisibility::Hidden);  // 隐藏进度条
    }

}

void USkillWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 获取角色实例
    PlayerCharacter = Cast<AJetCharacter>(GetOwningPlayerPawn());

    // 定义绿色颜色
    FLinearColor GreenColor(0.0f, 1.0f, 0.0f, 1.0f); // RGB for Green, Alpha = 1.0f

    // 创建并设置按钮样式
    if (CloudBurstButton)
    {
        FButtonStyle NewStyle = CloudBurstButton->WidgetStyle;
        NewStyle.Normal.TintColor = FSlateColor(GreenColor);
        CloudBurstButton->SetStyle(NewStyle);
    }

    if (UpdraftButton)
    {
        FButtonStyle NewStyle = UpdraftButton->WidgetStyle;
        NewStyle.Normal.TintColor = FSlateColor(GreenColor);
        UpdraftButton->SetStyle(NewStyle);
    }

    if (TailWindButton)
    {
        FButtonStyle NewStyle = TailWindButton->WidgetStyle;
        NewStyle.Normal.TintColor = FSlateColor(GreenColor);
        TailWindButton->SetStyle(NewStyle);
    }

    if (StormBladeButton)
    {
        FButtonStyle NewStyle = StormBladeButton->WidgetStyle;
        NewStyle.Normal.TintColor = FSlateColor(GreenColor);
        StormBladeButton->SetStyle(NewStyle);
    }
}
