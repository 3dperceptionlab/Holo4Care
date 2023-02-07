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

/// <summary>
/// Class for HTTP communication between Unreal and the API in the computing server
/// </summary>
UCLASS()
class HOLOYOLO_API AHTTPInterface : public AActor
{
	GENERATED_BODY()
	
private:	
	FString BoundaryLabel = FString();
	FString BoundaryBegin = FString();
	FString BoundaryEnd = FString();
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	int tries = 0;

protected:
	/// <summary>
	/// Called when the game starts or when spawned
	/// </summary>
	virtual void BeginPlay() override;

public:	
	/// <summary>
	/// Sets default values for this actor's properties
	/// </summary>
	AHTTPInterface();

	/// <summary>
	/// Contains JSON data from the response
	/// </summary>
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Data")
		TMap<int32, FString> JSONData;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TQueue<FString> URLs;

	/// <summary>
	/// 
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int actualIteration;

	/// <summary>
	/// 
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TArray<APredictionObject*> predictionObjects;

	/// <summary>
	/// 
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString actions;


	/// <summary>
	/// Overrides Tick function with a the new specified time. Should be called every frame.
	/// </summary>
	/// <param name="DeltaTime">Tick time</param>
	virtual void Tick(float DeltaTime) override;


	/// <summary>
	/// Sends a HTTP Post containing the serialized texture as PNG and the id to the specified endpoint
	/// </summary>
	/// <param name="TextureRenderTarget">Texture to seriealize and send to the computing server</param>
	/// <param name="URLEndpoint">Endpoint of the server</param>
	/// <param name="id">ID of the image for later retrieval</param>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable)
		int MakePost(UTextureRenderTarget2D* TextureRenderTarget, FString URLEndpoint, int id);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="newObjects"></param>
	/// <param name="allObjects"></param>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable)
		TArray<APredictionObject*> AddNewPredictionObjets(TArray<APredictionObject*> newObjects, TArray<APredictionObject*> allObjects);

	/// <summary>
	/// This function adds simple text fields to response
	/// </summary>
	/// <param name="Name"></param>
	/// <param name="Value"></param>
	/// <returns></returns>
	FString AddData(FString Name, FString Value);

	/// <summary>
	/// Callback to process response from the server
	/// </summary>
	/// <param name="pRequest"></param>
	/// <param name="pResponse"></param>
	/// <param name="connectedSuccessfully"></param>
	void OnResponseReceived(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully);
	
	/// <summary>
	/// 
	/// </summary>
	UFUNCTION(BlueprintCallable)
		void GetJSONItems();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="JSONSerialized"></param>
	/// <param name="itemPositions"></param>
	/// <param name="itemNames"></param>
	/// <param name="itemActions"></param>
	UFUNCTION(BlueprintCallable)
		static void ProcessJSON(FString JSONSerialized, TArray<FVector2D>& itemPositions, TArray<FString>& itemNames, TArray<FString>& itemActions);
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="JSONSerialized"></param>
	/// <param name="it"></param>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable)
		TArray<APredictionObject*> ProcessJSONtoObject(const FString JSONSerialized,int it);
};
