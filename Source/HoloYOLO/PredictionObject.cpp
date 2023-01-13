// Fill out your copyright notice in the Description page of Project Settings.

#include "PredictionObject.h"

#pragma warning(disable:4668)  
#include <DirectXMath.h>
#pragma warning(default:4668)
#include "MixedRealityInterop.h"
//#include "WindowsMixedRealityInteropUtility.h"
#include "ARBlueprintLibrary.h"

#include "C:\Program Files\Epic Games\UE_4.27\Engine\Plugins\Runtime\WindowsMixedReality\Source\HoloLensAR\Public\HoloLensARFunctionLibrary.h"
#include "C:\Program Files\Epic Games\UE_4.27\Engine\Plugins\Runtime\WindowsMixedReality\Source\HoloLensAR\Public\HoloLensARSystem.h"
//#include "HoloLensARSystem.h"

// Sets default values
APredictionObject::APredictionObject()
{
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ParentNode"));
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	x, xmax, xmin, y, ymin, ymax = 0;
	className = "Default";
	static ConstructorHelpers::FObjectFinder<UStaticMesh> sphere(TEXT("StaticMesh'/Engine/VREditor/TransformGizmo/RotationHandleFull.RotationHandleFull'"));
	std::string nameStr = std::string(TCHAR_TO_UTF8(*className));
	node = CreateDefaultSubobject<UStaticMeshComponent>(nameStr.c_str());
	node->SetStaticMesh(sphere.Object);
	node->SetupAttachment(SceneRoot);

	text = CreateAbstractDefaultSubobject<UTextRenderComponent>("text");
	text->SetText(nameStr.c_str());
	text->SetTextRenderColor(FColor(150, 150, 150));
	text->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	text->SetRelativeLocation(FVector(0, 0, 10));
	text->SetRelativeScale3D(FVector(0.2,0.2,0.2));
	text->SetupAttachment(SceneRoot);

}

APredictionObject::~APredictionObject() {
	/*if (node != nullptr) {
		node->DestroyComponent();
	}
	if (text != nullptr) {
		text->DestroyComponent();
	}*/
	//Destroy();

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
	node->SetRelativeScale3D(FVector(0.06, 0.06, 0.06));

	std::string nameStr = std::string(TCHAR_TO_UTF8(*className));
	text->SetText(nameStr.c_str());

	
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


FVector APredictionObject::GetWorldSpaceRayFromCameraPoint(FVector2D PixelCoordinate, FTransform transform){

	CameraImageCapture& CameraCapture = CameraImageCapture::Get();

	DirectX::XMFLOAT2 CameraPoint = DirectX::XMFLOAT2(PixelCoordinate.X, PixelCoordinate.Y);
	DirectX::XMFLOAT2 UnprojectedPointAtUnitDepth = CameraCapture.UnprojectPVCamPointAtUnitDepth(CameraPoint);

	/*FVector Ray = WindowsMixedReality::WMRUtility::FromMixedRealityVector(
		DirectX::XMFLOAT3(
			UnprojectedPointAtUnitDepth.x,
			UnprojectedPointAtUnitDepth.y,
			-1.0f // Unprojection happened at 1 meter
		)
	) * 100.0f;*/

	FVector Ray = FVector(
		-1.0f * -1.0f,
		UnprojectedPointAtUnitDepth.x,
		UnprojectedPointAtUnitDepth.y) * 100.0f;

	Ray.Normalize();

	//FCriticalSection PVCamToWorldLock;
	//FScopeLock sl(&PVCamToWorldLock);
	return transform.TransformVector(Ray);
}

