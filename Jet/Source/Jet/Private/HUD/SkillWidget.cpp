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
        // ���� Updraft ���ܵĿ���״̬
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
        // ���� Updraft ���ܵĿ���״̬
        if (TailWindProgressBar)
        {
            TailWindProgressBar->SetVisibility(ESlateVisibility::Hidden);  // ���ؽ�����
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
        // ���� Updraft ���ܵļ���״̬
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
            TailWindProgressBar->SetVisibility(ESlateVisibility::Hidden);  // ���ؽ�����
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
    // ����ʱ����½�����
    TailWindProgressBar->SetPercent(0.0f);

    // ����7.5��ļ�ʱ����ÿ�����һ�ν�����
    TargetProgress = 1.0f;  // Ŀ������䵽 100%

    // ������ʱ����ÿ�봥��һ��
    GetWorld()->GetTimerManager().SetTimer(
        ProgressBarTimerHandle,
        this,
        &USkillWidget::UpdateProgressBar,
        7.5f / 100.0f,  // ÿ�θ��µ�ʱ�����������������
        true,  // ѭ������
        0.0f   // �ӳ�0������
    );
}

void USkillWidget::UpdateProgressBar()
{
    // ��ȡ��ǰ�Ľ���
    float CurrentProgress = TailWindProgressBar->Percent;

    // �������������ֵ
    CurrentProgress += 0.01f;  // ÿ�ε���1%

    // ���½�����
    TailWindProgressBar->SetPercent(CurrentProgress);

    // ��������������ϣ�ֹͣ��ʱ��
    if (CurrentProgress >= TargetProgress)
    {
        // ֹͣ��ʱ��
        GetWorld()->GetTimerManager().ClearTimer(ProgressBarTimerHandle);
    }

}

void USkillWidget::HideBar()
{
    if (TailWindProgressBar)
    {
        TailWindProgressBar->SetPercent(0.0f);
        TailWindProgressBar->SetVisibility(ESlateVisibility::Hidden);  // ���ؽ�����
    }

}

void USkillWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // ��ȡ��ɫʵ��
    PlayerCharacter = Cast<AJetCharacter>(GetOwningPlayerPawn());

    // ������ɫ��ɫ
    FLinearColor GreenColor(0.0f, 1.0f, 0.0f, 1.0f); // RGB for Green, Alpha = 1.0f

    // ���������ð�ť��ʽ
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
