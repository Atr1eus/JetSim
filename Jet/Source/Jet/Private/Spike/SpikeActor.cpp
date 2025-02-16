// Fill out your copyright notice in the Description page of Project Settings.


#include "Spike/SpikeActor.h"
#include <Components/BoxComponent.h>


// Sets default values
ASpikeActor::ASpikeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ��ʼ����ײ���
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;  // ���ø����Ϊ��ײ���

	// ��ʼ�� Skeletal Mesh ���
	SpikeSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SpikeSkeletalMesh"));
	SpikeSkeletalMesh->SetupAttachment(RootComponent);  // �� Skeletal Mesh ���ӵ������

	// ������ײ
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // ������ײ��ѯ������ģ��
	CollisionComponent->SetCollisionObjectType(ECC_WorldDynamic);  // ������ײ����
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);  // ����������ͨ������ײ��ӦΪ Block
	CollisionComponent->SetSimulatePhysics(false);
	//Initialization
	bIsSpikeInstalled = false;
	InstallTime = 4.0f; // ��װ��Ҫ4��
	ExplosionTime = 45.0f; // ��ըʱ��Ϊ45��
	CountdownTimer = ExplosionTime;
	InstallProgress = 0.0f;
	DefuseTime = 7.0f;

}

void ASpikeActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpikeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpikeActor::Explode()
{
	// ��������������ı�ը�߼����˺�����Ч�ȣ�
	UE_LOG(LogTemp, Warning, TEXT("Spike Exploded!"));

	// ����ը��
	Destroy();
}

void ASpikeActor::InstallSpike()
{
	if (!bIsSpikeInstalled)
	{
		// ������װ����
		InstallProgress = 0.0f;
		GetWorld()->GetTimerManager().SetTimer(InstallTimerHandle, this, &ASpikeActor::OnInstallComplete, InstallTime, false);
	}
}

void ASpikeActor::DefuseSpike()
{

}

void ASpikeActor::OnInstallComplete()
{
	bIsSpikeInstalled = true;
	CountdownTimer = ExplosionTime; // ���õ���ʱ
	GetWorld()->GetTimerManager().ClearTimer(InstallTimerHandle);
	// ִ�а�װ��ɺ�Ķ���
	UE_LOG(LogTemp, Warning, TEXT("Spike installation complete"));

}

void ASpikeActor::UpdateTimer(float DeltaTime)
{
	// ���ӵ���ʱ
	if (CountdownTimer > 0)
	{
		CountdownTimer -= DeltaTime;
	}
	else
	{
		Explode();
	}
}

