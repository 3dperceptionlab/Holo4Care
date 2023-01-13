// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <string>
#include "GameFramework/Actor.h"
//#include "HoloLensARFunctionLibrary.h"
//#include "ARBlueprintLibrary.h"
#include "Components/TextRenderComponent.h"
#include "PredictionObject.generated.h"



UCLASS()
class HOLOYOLO_API APredictionObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APredictionObject();
	~APredictionObject();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		FVector GetWorldSpaceRayFromCameraPoint(FVector2D PixelCoordinate, FTransform transform);

	void ConfigNode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TArray<FString> actions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString className;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float x;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float y;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float xmin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float xmax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float ymin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float ymax;

		USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		UStaticMeshComponent* node;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		UTextRenderComponent* text;

};
