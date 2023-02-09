// Fill out your copyright notice in the Description page of Project Settings.


#include "QRManager.h"

AQRObject* UQRManager::CreateQRObject(const UObject* WorldContextObject, FString qr_text, FGuid qr_guid, int32 qr_version, UStaticMesh* mesh)
{
	UWorld* world = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	FActorSpawnParameters SpawnInfo = FActorSpawnParameters();
	/*UWorld* world = nullptr;

	if (GEngine->GetCurrentPlayWorld()) {
		world = GEngine->GetCurrentPlayWorld();
	}*/
	//else if (GetWorld()) {
	//	world = GetWorld();
	//}

	if (world == nullptr)
		return nullptr;

	AQRObject* qr_object = world->SpawnActor<AQRObject>(SpawnInfo);
	qr_object->qr_guid = qr_guid;
	qr_object->qr_text = qr_text;
	qr_object->qr_version = qr_version;
	qr_object->node->SetStaticMesh(mesh);

	return qr_object;
}
