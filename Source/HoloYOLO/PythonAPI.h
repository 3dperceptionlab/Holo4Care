// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Runtime/Online/HTTP/Public/Http.h"
#include "GenericPlatform/GenericPlatformProcess.h"
//#include "HoloLens/HoloLensPlatformProcess.h"
#include "Serialization/JsonSerializer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PythonAPI.generated.h"



UCLASS()
class HOLOYOLO_API UPythonAPI : public UBlueprintFunctionLibrary{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Unreal Python")
		static FString ExecuteSystemCommand(FString Command, FString Params);
	UFUNCTION(BlueprintCallable, Category = "Unreal Python")
		static void ProcessJSON(FString JSONSerialized, TArray<FVector2D>& itemPositions, TArray<FString>& itemNames, TArray<FString>& itemActions);
};
