// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Serialization/BufferArchive.h"
#include "CoreMinimal.h"
//#include "Runtime/Online/HTTP/Public/Http.h"
#include "GenericPlatform/GenericPlatformProcess.h"
//#include "HoloLens/HoloLensPlatformProcess.h"
#include "Serialization/JsonSerializer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Misc/FileHelper.h"
#include "PythonAPI.generated.h"



UCLASS()
class HOLOYOLO_API UPythonAPI : public UBlueprintFunctionLibrary{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Unreal Python")
		static FString ExecuteSystemCommand(FString Command, FString Params);

	UFUNCTION(BlueprintCallable, Category = "Unreal Python")
		bool SendDataToServer(uint8 arrayData, APawn* pawn);

	UFUNCTION(BlueprintCallable, Category = "Unreal Python")
		static TArray<uint8> LoadFile(APawn* pawn);

		//void SendDataToServer(const TArray<uint8> arrayData, APawn* pawn);
	UFUNCTION(BlueprintCallable, Category = "Unreal Python")
		static UTexture2D* ReceiveDataFromClient(APawn* pawn);

		static bool SaveTextureToArchive(FArchive& DataArchive, UTexture2D* Texture);
	
		static UTexture2D* LoadTextureFromArchive(FArchive& DataArchive);
	UFUNCTION(BlueprintCallable, Category = "Unreal Python")
		static TArray<uint8> SaveTextureToFile( UTexture2D* Texture);
};
