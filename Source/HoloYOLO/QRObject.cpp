// Fill out your copyright notice in the Description page of Project Settings.


#include "QRObject.h"

// Sets default values
AQRObject::AQRObject()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ParentNode"));
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> sphere(TEXT("StaticMesh'/Game/RotationHandleFull.RotationHandleFull'"));
	className = "DefaultMesh";
	std::string nameStr = std::string(TCHAR_TO_UTF8(*className));
	node = CreateDefaultSubobject<UStaticMeshComponent>(nameStr.c_str());
	node->SetStaticMesh(sphere.Object);
	//node->SetRelativeScale3D(FVector(0.05, 0.05, 0.05));
	node->SetupAttachment(SceneRoot);
}

// Called when the game starts or when spawned
void AQRObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AQRObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AQRObject::UpdateObjectPosition(FVector center, FVector extent, FRotator rotation)
{
	SetActorLocation(center);
	SetActorRotation(rotation);
	node->SetRelativeRotation(FRotator(-90, 0, 0));
	float scale = (extent.Y + extent.Z)/50;
	SetActorScale3D(FVector(scale,scale,scale));
	

}

