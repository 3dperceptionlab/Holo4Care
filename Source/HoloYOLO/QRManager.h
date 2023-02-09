// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "QRObject.h"
#include "QRManager.generated.h"

/**
 * 
 */
UCLASS()
class HOLOYOLO_API UQRManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
		static AQRObject* CreateQRObject(const UObject* WorldContextObject, FString qr_text, FGuid qr_guid, int32 qr_version);
};
