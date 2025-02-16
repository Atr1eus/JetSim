// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Jet/JetCharacter.h>

#include "InstZoneActor.generated.h"


UCLASS()
class JET_API AInstZoneActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInstZoneActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    // ��������
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spike")
    class UBoxComponent* TriggerBox;

    // ����Ƿ���������
    //bool bIsPlayerInZone;
    TObjectPtr<AJetCharacter> PlayerCharacter;

    // �����ҽ�������ĺ���
    UFUNCTION()
    void OnPlayerEnterZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
        const FHitResult& SweepResult);

    // ������װը���ĺ���
    UFUNCTION(BlueprintCallable, Category = "Spike")
    void TriggerInstallSpike();

    // ������ʾUI��ʾ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    bool bShowInstallPrompt;
	
};
