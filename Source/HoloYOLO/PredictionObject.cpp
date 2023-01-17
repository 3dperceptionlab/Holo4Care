// Fill out your copyright notice in the Description page of Project Settings.

#include "PredictionObject.h"
#include <regex>
#pragma warning(disable:4668)  
#include <DirectXMath.h>
#pragma warning(default:4668)
#include "MixedRealityInterop.h"
//#include "WindowsMixedRealityInteropUtility.h"
#include "ARBlueprintLibrary.h"

// "C:\Program Files\Epic Games\UE_4.27\Engine\Plugins\Runtime\WindowsMixedReality\Source\HoloLensAR\Public\HoloLensARFunctionLibrary.h"
#include "C:\Program Files\Epic Games\UE_4.27\Engine\Plugins\Runtime\WindowsMixedReality\Source\HoloLensAR\Public\HoloLensARSystem.h"
#include "HoloLensARSystem.h"


std::string getCleanText(std::string text) {
	text[0] = std::toupper(text[0]);
	text = std::regex_replace(text, std::regex("_"), " ");
	return text;
}



// Sets default values
APredictionObject::APredictionObject()
{
	visible = false;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ParentNode"));
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UxtPressableButton = CreateDefaultSubobject<UUxtPressableButtonComponent>("UxtButton");
	FScriptDelegate delegate;
	delegate.BindUFunction(this, FName("onButtonPressed"));
	UxtPressableButton->OnButtonPressed.AddUnique(delegate);
	FScriptDelegate delegate2;
	delegate2.BindUFunction(this, FName("onButtonBeginFocus"));
	UxtPressableButton->OnBeginFocus.AddUnique(delegate2);
	FScriptDelegate delegate3;
	delegate3.BindUFunction(this, FName("onButtonEndFocus"));
	UxtPressableButton->OnEndFocus.AddUnique(delegate3);
	UxtPressableButton->SetPushBehavior(EUxtPushBehavior::Compress);
	UxtPressableButton->SetFrontFaceCollisionFraction(1);
	UxtPressableButton->SetupAttachment(SceneRoot);


	x, xmax, xmin, y, ymin, ymax = 0;
	className = "Default";
	static ConstructorHelpers::FObjectFinder<UStaticMesh> sphere(TEXT("StaticMesh'/Game/RotationHandleFull.RotationHandleFull'"));
	std::string nameStr = std::string(TCHAR_TO_UTF8(*className));
	node = CreateDefaultSubobject<UStaticMeshComponent>(nameStr.c_str());
	node->SetStaticMesh(sphere.Object);
	node->SetupAttachment(UxtPressableButton);

	text = CreateDefaultSubobject<UTextRenderComponent>("text");
	text->SetText(nameStr.c_str());
	text->SetTextRenderColor(FColor(30, 30, 30, 0.5));
	//text->SetTextRenderColor(FColor(150, 150, 150));
	text->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	//text->SetRelativeLocation(FVector(0, 0, 0));
	text->SetRelativeScale3D(FVector(0.12,0.12,0.12));
	text->SetupAttachment(SceneRoot);

	actionsText = CreateDefaultSubobject<UTextRenderComponent>("ActionText");
	actionsText->SetTextRenderColor(FColor(25, 25, 25));
	actionsText->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	actionsText->SetRelativeLocation(FVector(0, 0, -6));
	actionsText->SetRelativeScale3D(FVector(0.10, 0.10, 0.10));
	actionsText->SetupAttachment(SceneRoot);
	actionsText->SetText(TEXT("None"));

	actionsText2 = CreateDefaultSubobject<UTextRenderComponent>("ActionText2");
	actionsText2->SetTextRenderColor(FColor(25, 25, 25));
	actionsText2->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	actionsText2->SetRelativeLocation(FVector(0, 0, -9));
	actionsText2->SetRelativeScale3D(FVector(0.10, 0.10, 0.10));
	actionsText2->SetupAttachment(SceneRoot);
	actionsText2->SetText(TEXT(""));
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

bool APredictionObject::destroy() {
	return(Destroy());
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
	text->SetText(getCleanText(nameStr).c_str());

	if (actions.Num() > 0) {
		FString actionsFText;
		for (int i = 0; i < actions.Num() && i < 3 ; i++) {
			actionsFText += actions[i] + " ";
			
		}
		actionsText->SetText(actionsFText);
		actionsFText = "";
		for (int i = 3; i < actions.Num() && i < 5; i++) {
			actionsFText += actions[i] + " ";
		}
		actionsText2->SetText(actionsFText);

	}
	actionsText->SetVisibility(visible);
	actionsText2->SetVisibility(visible);


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


void APredictionObject::onButtonPressed() {
	UE_LOG(LogTemp, Display, TEXT("Button PRESSED"));
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Button PRESSED")));
	}

	visible = !visible;
	actionsText->SetVisibility(visible);
	actionsText2->SetVisibility(visible);


}

void APredictionObject::onButtonBeginFocus(){

	node->SetVectorParameterValueOnMaterials("Color", FVector(0.1, 0.2, 0.4));
	text->SetTextRenderColor(FColor(40, 80, 150, 0.8));
	actionsText->SetTextRenderColor(FColor(25, 50, 100));
	actionsText2->SetTextRenderColor(FColor(25, 50, 100));

}

void APredictionObject::onButtonEndFocus() {

	node->SetVectorParameterValueOnMaterials("Color", FVector(0.08, 0.08, 0.08));
	text->SetTextRenderColor(FColor(30, 30, 30, 0.5));
	actionsText2->SetTextRenderColor(FColor(25, 25, 25));
	actionsText->SetTextRenderColor(FColor(25, 25, 25));


}


