// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Interfaces/IHttpResponse.h"
#include "HTTPModule.h"
#include <string>
#include <chrono>
#include "ImageUtils.h"
#include "Serialization/BufferArchive.h"
#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "PredictionObject.h"
#include "GameFramework/Actor.h"

#define PSCREEN(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT(x));}
#define PSCREENE(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT(x));}

typedef struct Item {
	long long unsigned id;
	float x, y;
	std::string name, action;
	Item() {};
} Item;

#include "HTTPInterface.generated.h"

UCLASS()
class HOLOYOLO_API AHTTPInterface : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHTTPInterface();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	FString BoundaryLabel = FString();
	FString BoundaryBegin = FString();
	FString BoundaryEnd = FString();
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	int tries = 0;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Data")
		TMap<int32, FString> JSONData;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TQueue<FString> URLs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int actualIteration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TArray<APredictionObject*> predictionObjects;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	//	TDoubleLinkedList<APredictionObject*> hola;


	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Main function for upload
	UFUNCTION(BlueprintCallable)
		int MakePost(UTextureRenderTarget2D* TextureRenderTarget, FString URLEndpoint, int id);

	UFUNCTION(BlueprintCallable)
		TArray<APredictionObject*> AddNewPredictionObjets(TArray<APredictionObject*> newObjects, TArray<APredictionObject*> allObjects);

	// This is a function to add simple text fields to response
	FString AddData(FString Name, FString Value);

	// Callback to process response from the server
	void OnResponseReceived(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully);
	
	UFUNCTION(BlueprintCallable)
		void GetJSONItems();
	UFUNCTION(BlueprintCallable)
		static void ProcessJSON(FString JSONSerialized, TArray<FVector2D>& itemPositions, TArray<FString>& itemNames, TArray<FString>& itemActions);
	UFUNCTION(BlueprintCallable)
		TArray<APredictionObject*> ProcessJSONtoObject(const FString JSONSerialized,int it);


};
