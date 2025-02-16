// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include <Components/BoxComponent.h>
#include "SpikeActor.generated.h"


UCLASS()
class JET_API ASpikeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpikeActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere)
    USkeletalMeshComponent* SpikeSkeletalMesh;

    UPROPERTY(VisibleAnywhere)
    UBoxComponent* CollisionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
    UAnimSequence* InstallAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    USoundBase* PlantSound;

    FTimerHandle InstallTimerHandle;

    // ը���Ƿ��Ѿ���װ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spike")
    bool bIsSpikeInstalled;

    // ��װʱ��Ҫ��ʱ��
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spike")
    float InstallTime;

    // ��ըʱ��
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spike")
    float ExplosionTime;

    // ��жʱ��
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spike")
    float DefuseTime;

    // ����ʱ
    float CountdownTimer;

    // ��װ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spike")
    float InstallProgress;

    // ��ը����
    UFUNCTION(BlueprintCallable, Category = "Spike")
    void Explode();

    // ������װ�ĺ���
    UFUNCTION(BlueprintCallable, Category = "Spike")
    void InstallSpike();

    //���
    UFUNCTION(BlueprintCallable, Category = "Spike")
    void DefuseSpike();

    UFUNCTION(BlueprintCallable, Category = "Spike")
    void OnInstallComplete();

    // ��ʱ�����º���
    void UpdateTimer(float DeltaTime);
	
};
