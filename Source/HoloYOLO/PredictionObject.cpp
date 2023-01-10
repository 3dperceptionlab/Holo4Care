// Fill out your copyright notice in the Description page of Project Settings.


#include "PredictionObject.h"

// Sets default values
APredictionObject::APredictionObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	x, xmax, xmin, y, ymin, ymax = 0;
	className = "Default";
	static ConstructorHelpers::FObjectFinder<UStaticMesh> sphere(TEXT("StaticMesh'/Engine/VREditor/TransformGizmo/RotationHandleFull.RotationHandleFull'"));
	std::string nameStr = std::string(TCHAR_TO_UTF8(*className));
	node = CreateDefaultSubobject<UStaticMeshComponent>(nameStr.c_str());
	node->SetStaticMesh(sphere.Object);


}

// Called when the game starts or when spawned
void APredictionObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APredictionObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APredictionObject::ConfigNode() {

	//GetWorldSpaceRayFromCameraPoint(x, y);

	//SetActorLocation(FVector(0, 0, 0));
	node->SetRelativeLocation(FVector(0, 0, 0));
	node->SetRelativeScale3D(FVector(0.1, 0.1, 0.1));
	//text->SetText(className);
	//text->SetRelativeLocation(FVector(0, 0, 20));
	//node->SetupAttachment(NodeScene);
	
	/*	if (UWorld* World = GetWorld()){

		APlayerCameraManager* PlayerCamera = World->GetFirstPlayerController()->PlayerCameraManager;
		auto camLoc = PlayerCamera->GetCameraLocation();

		for (int n = 0; n < nodes.size(); n++) {

			FRotator newRot = UKismetMathLibrary::FindLookAtRotation(nodes[n]->GetComponentLocation(), camLoc);
			nodes[n]->SetWorldRotation(newRot);
		}

	}*/

}

