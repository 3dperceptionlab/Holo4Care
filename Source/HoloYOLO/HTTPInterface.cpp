// Fill out your copyright notice in the Description page of Project Settings.


#include "HTTPInterface.h"

// Sets default values
AHTTPInterface::AHTTPInterface()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHTTPInterface::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHTTPInterface::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<uint8> FStringToUint8(const FString& InString)
{
	TArray<uint8> OutBytes;

	// Handle empty strings
	if (InString.Len() > 0)
	{
		FTCHARToUTF8 Converted(*InString); // Convert to UTF8
		OutBytes.Append(reinterpret_cast<const uint8*>(Converted.Get()), Converted.Length());
	}

	return OutBytes;
}

FString AHTTPInterface::AddData(FString Name, FString Value) {
	return FString(TEXT("\r\n"))
		+ BoundaryBegin
		+ FString(TEXT("Content-Disposition: form-data; name=\""))
		+ Name
		+ FString(TEXT("\"\r\n\r\n"))
		+ Value;
}


int AHTTPInterface::MakePost(UTextureRenderTarget2D* TextureRenderTarget, FString URLEndpoint, int id){

	float time = 0;
	//PSCREEN("0.Starting makePost. Time: 0");
	UE_LOG(LogTemp, Display, TEXT("0.Starting makePost. Time: 0"));

	start = std::chrono::steady_clock::now();




	FHttpModule& HttpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = HttpModule.CreateRequest();

	// We set the api URL
	//HttpRequest->SetURL("http://127.0.0.1:3000/upload");
	HttpRequest->SetURL(URLEndpoint);

	// We set verb of the request (GET/PUT/POST)
	HttpRequest->SetVerb(TEXT("POST"));


	// Create a boundary label, for the header
	BoundaryLabel = FString(TEXT("e543322540af456f9a3773049ca02529-")) + FString::FromInt(FMath::Rand());
	// boundary label for begining of every payload chunk 
	BoundaryBegin = FString(TEXT("--")) + BoundaryLabel + FString(TEXT("\r\n"));
	// boundary label for the end of payload
	BoundaryEnd = FString(TEXT("\r\n--")) + BoundaryLabel + FString(TEXT("--\r\n"));

	// Set the content-type for server to know what are we going to send
	HttpRequest->SetHeader(TEXT("Content-Type"), FString(TEXT("multipart/form-data; boundary=")) + BoundaryLabel);


	// This is binary content of the request
	TArray<uint8> CombinedContent;

	TArray<uint8> FileRawData;
	FBufferArchive Buffer;
	//FFileHelper::LoadFileToArray(FileRawData, *FullFilePath);

	auto end = std::chrono::steady_clock::now();
	time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	/*if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("1.toGetPNG Time: %f"), time));
	}
	UE_LOG(LogTemp, Display, TEXT("1.toGetPNG Time: %f"), time);
	*/
	start = std::chrono::steady_clock::now();
	FImageUtils::ExportRenderTarget2DAsPNG(TextureRenderTarget, Buffer);

	end = std::chrono::steady_clock::now();
	time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	/*if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("2.Image READED Time:%f N:%d"), time, Buffer.Num()));
	}
	UE_LOG(LogTemp, Display, TEXT("2.Image READED Time:%f N:%d"), time, Buffer.Num());
	*/
	start = std::chrono::steady_clock::now();
	// First, we add the boundary for the file, which is different from text payload
	/*FString FileBoundaryString = FString(TEXT("\r\n"))
		+ BoundaryBegin
		+ FString(TEXT("Content-Disposition: form-data; name=\"img\"; filename=\""))
		+ FileName + "\"\r\n"
		+ "Content-Type: image/png"
		+ FString(TEXT("\r\n\r\n"));
		*/
	// Notice, we convert all strings into uint8 format using FStringToUint8
	//CombinedContent.Append(FStringToUint8(FileBoundaryString));

	// Append the file data
	//CombinedContent.Append(FileRawData)
		
	CombinedContent.Append(FStringToUint8(AddData("img", FBase64::Encode(Buffer))));
	
	// Let's add couple of text values to the payload
	CombinedContent.Append(FStringToUint8(AddData("id", FString::FromInt(id))));

	// Finally, add a boundary at the end of the payload
	CombinedContent.Append(FStringToUint8(BoundaryEnd));

	// Set the request content
	HttpRequest->SetContent(CombinedContent);


	// Hook a lambda(anonymous function) to when we receive a response
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &AHTTPInterface::OnResponseReceived);

	/*HttpRequest->OnProcessRequestComplete().BindLambda(
		[this](
			FHttpRequestPtr pRequest,
			FHttpResponsePtr pResponse,
			bool connectedSuccessfully) mutable {
				AHTTPInterface::OnResponseReceived(pRequest, pResponse, connectedSuccessfully);
		});*/

	// Send the request 
	HttpRequest->ProcessRequest();

	end = std::chrono::steady_clock::now();
	time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	/*if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("3.Finish MakePOST Time:%f"), time));
	}*/
	UE_LOG(LogTemp, Display, TEXT("3.Finish MakePOST Time:%f"), time);

	start = std::chrono::steady_clock::now();

	return FileRawData.Num();

}

void AHTTPInterface::GetJSONItems() {

	if (!URLs.IsEmpty()) {
		FString url = FString();
		URLs.Peek(url);
		UE_LOG(LogTemp, Display, TEXT("Getting: %s"), *url);
		/*if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Getting: %s"), *url));
		}*/
		FHttpModule& HttpModule = FHttpModule::Get();
		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = HttpModule.CreateRequest();
		HttpRequest->SetURL(*url);
		HttpRequest->SetVerb(TEXT("GET"));
		HttpRequest->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		HttpRequest->SetHeader("Content-Type", TEXT("application/json"));
		// Hook a lambda(anonymous function) to when we receive a response
		//HttpRequest->OnProcessRequestComplete().BindUObject(this, &AHTTPInterface::OnResponseReceived);
		HttpRequest->OnProcessRequestComplete().BindLambda(
			[this](
				FHttpRequestPtr pRequest,
				FHttpResponsePtr pResponse,
				bool connectedSuccessfully) mutable {

					if (connectedSuccessfully) {
						UE_LOG(LogTemp, Display, TEXT("Response: %s"), *pResponse->GetContentAsString());
						//if (GEngine) {
						//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Response: %s"), *pResponse->GetContentAsString()));
						//}
						FJsonSerializableArray fJSONSerial;
						pResponse->GetURL().ParseIntoArray(fJSONSerial, TEXT("/"));
						int id = FCString::Atoi(*fJSONSerial.Top());

						switch (pResponse->GetResponseCode()) {
						case EHttpResponseCodes::Ok:

							predictionObjects.Empty();
							predictionObjects = ProcessJSONtoObject(pResponse->GetContentAsString(),id);
							URLs.Pop();
							if(predictionObjects.Num()==0)
								GetJSONItems();

							//JSONData.Add(20, pResponse->GetContentAsString());
							UE_LOG(LogTemp, Display, TEXT("JSON Data added"));
							break;
						case EHttpResponseCodes::NotFound:
							UE_LOG(LogTemp, Error, TEXT("ID doesn't exist"));
							PSCREENE("ID doesn't exist");
							break;
						case EHttpResponseCodes::ServiceUnavail:
							UE_LOG(LogTemp, Error, TEXT("ID processing"));
							PSCREENE("ID processing");
							break;

						default:
							UE_LOG(LogTemp, Display, TEXT("Connection."));


							//GetJSONItems(getURL); if 101 retry
						}

					}
					else {
						switch (pRequest->GetStatus()) {
						case EHttpRequestStatus::Failed_ConnectionError:
							UE_LOG(LogTemp, Error, TEXT("Connection failed."));
							PSCREENE("Connection failed.");
						default:
							UE_LOG(LogTemp, Error, TEXT("Request failed."));
							PSCREENE("Request failed.");
						}
					}
			});

		// Send the request 
		HttpRequest->ProcessRequest();

	}
}


void AHTTPInterface::OnResponseReceived(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully)
{
	UE_LOG(LogTemp, Display, TEXT("Connection."));

	if (connectedSuccessfully) {
		
		UE_LOG(LogTemp, Display, TEXT("Response: %s"), *pResponse->GetContentAsString());
		//if (GEngine) {
		//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Response: %s"), *pResponse->GetContentAsString()));
		//}
		
		TSharedPtr<FJsonValue> JsonValue;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(pResponse->GetContentAsString());
		if (FJsonSerializer::Deserialize(Reader, JsonValue)) {
			FString getURL = JsonValue->AsObject()->GetStringField("get");
			//GetJSONItems(getURL);
			URLs.Enqueue(getURL);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Failed Deserializing json."));

		}

	}
	else {
		switch (pRequest->GetStatus()) {
		case EHttpRequestStatus::Failed_ConnectionError:
			UE_LOG(LogTemp, Error, TEXT("Connection failed."));
			PSCREENE("Connection failed.");
		default:
			UE_LOG(LogTemp, Error, TEXT("Request failed."));
			PSCREENE("Request failed.");
		}

	}

	auto end = std::chrono::steady_clock::now();
	float time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("4.ResponseReceived Time:%f"), time));
	}

	UE_LOG(LogTemp, Display, TEXT("4.ResponseReceived Time:%f"), time);

	start = std::chrono::steady_clock::now();
}


void AHTTPInterface::ProcessJSON(FString JSONSerialized, TArray<FVector2D>& itemPositions, TArray<FString>& itemNames, TArray<FString>& itemActions) {

	// Single json value (any of supported json types e.g.
	// object with properties, array, bool) at top level of json
	TSharedPtr<FJsonValue> JsonValue;

	// Create a reader pointer to read the json data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JSONSerialized);

	// Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonValue)) {
		// Get the value of the json object by field name
		auto items = JsonValue->AsObject()->GetArrayField("data");
		for (auto& item : items) {
			itemPositions.Add(FVector2D((item.Get()->AsObject()->GetNumberField("xmax")+ item.Get()->AsObject()->GetNumberField("xmin"))/2, \
										(item.Get()->AsObject()->GetNumberField("ymax")+ item.Get()->AsObject()->GetNumberField("ymin"))/2));
			itemNames.Add(FString(item.Get()->AsObject()->GetStringField("class")));
			itemActions.Add(FString(item.Get()->AsObject()->GetArrayField("actions")[0]->AsString()));

		}

	}

}

TArray<APredictionObject*> AHTTPInterface::ProcessJSONtoObject(const FString JSONSerialized, int it) {
	TArray<APredictionObject*> predictionObjectsFunc;
	TSharedPtr<FJsonValue> JsonValue;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JSONSerialized);
	UE_LOG(LogTemp, Display, TEXT("ProcessJSONtoObject %d"),it);
	
	// Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonValue)) {
		// Get the value of the json object by field name
		auto items = JsonValue->AsObject()->GetArrayField("data");
		for (auto& item : items) {
			FActorSpawnParameters SpawnInfo = FActorSpawnParameters();
			UWorld* world = nullptr;
			if (GEngine->GetCurrentPlayWorld()) {
				world = GEngine->GetCurrentPlayWorld();
			}
			else if (GetWorld()) {
				world = GetWorld();
			}

			if (world != nullptr){

				APredictionObject* predictionObject = world->SpawnActor< APredictionObject>(SpawnInfo);
			

				predictionObject->xmax = item.Get()->AsObject()->GetNumberField("xmax");
				predictionObject->xmin = item.Get()->AsObject()->GetNumberField("xmin");
				predictionObject->x = (predictionObject->xmax + predictionObject->xmin) / 2;

				predictionObject->ymax = item.Get()->AsObject()->GetNumberField("ymax");
				predictionObject->ymin = item.Get()->AsObject()->GetNumberField("ymin");
				predictionObject->y = (predictionObject->ymax + predictionObject->ymin) / 2;

				predictionObject->className = FString(item.Get()->AsObject()->GetStringField("class"));

				for (auto& action : item.Get()->AsObject()->GetArrayField("actions")) {
					predictionObject->actions.Add(FString(action->AsString()));
				}
				predictionObject->ConfigNode();
				predictionObjectsFunc.Add(predictionObject);

			}else {
				UE_LOG(LogTemp, Error, TEXT("No world found"));
			//	return;
			}

			
		}
		auto actionsData = JsonValue->AsObject()->GetArrayField("actions");
		if (actionsData.Num() > 0) {
			actions = "";
			for (auto& action : actionsData) {
				actions += (action->AsString()) + "\n";
			}

		}
		
		actualIteration = it;

	}
	return predictionObjectsFunc;
}

TArray<APredictionObject*> AHTTPInterface::AddNewPredictionObjets(TArray<APredictionObject*> newObjects, TArray<APredictionObject*> allObjects) {

	//TArray<APredictionObject*> finalArray;

	for (APredictionObject* newObj : newObjects) {

		float nodeScale = ((newObj->xmax - newObj->xmin) + (newObj->ymax - newObj->ymin)) / 1050000 * newObj->camDistance;
		newObj->node->SetRelativeScale3D(FVector(0.05, 0.05 + nodeScale, 0.05 + nodeScale));
		newObj->text->SetRelativeScale3D(FVector(0.12 + nodeScale / 1.5, 0.12 + nodeScale / 1.5, 0.12 + nodeScale / 1.5));
		newObj->actionsText->SetRelativeScale3D(FVector(0.08 + nodeScale / 2, 0.08 + nodeScale / 2, 0.08 + nodeScale / 2));
		newObj->actionsText2->SetRelativeScale3D(FVector(0.08 + nodeScale / 2, 0.08 + nodeScale / 2, 0.08 + nodeScale / 2));

		if (nodeScale < 0.11) {
			float upDistance = (0.05 + nodeScale) * 120;
			//newObj->text->SetRelativeLocation(FVector(0, 0, upDistance));
			newObj->actionsText->SetRelativeLocation(FVector(0, 0, upDistance ));
			newObj->actionsText2->SetRelativeLocation(FVector(0, 0, upDistance +3));
				
		}

		float distance = 20 + ((newObj->xmax - newObj->xmin) + (newObj->ymax - newObj->ymin))/6;
		int id = -1;
		for (int i = 0; i < allObjects.Num();i++) {

			if (newObj->className == allObjects[i]->className) {
				float actualDistance = (allObjects[i]->GetActorLocation() - newObj->GetActorLocation()).Size();
				if (actualDistance < distance) {
					distance = actualDistance;
					id = i;
				}
			}
		}
		//if(false){

		if (id >= 0) {
			newObj->SetActorLocation((newObj->GetActorLocation() + allObjects[id]->GetActorLocation())/2);
			newObj->visible = allObjects[id]->visible;
			newObj->actionsText->SetVisibility(newObj->visible);
			newObj->actionsText2->SetVisibility(newObj->visible);

			//delete allObjects[id];
			if (allObjects[id]->destroy()){
				allObjects.RemoveAt(id);
			}
			
			allObjects.Add(newObj);

			//delete allObjects[id];
			//allObjects[id] == nullptr;

		}else {
			allObjects.Add(newObj);

		}

	}

	/*for (int i = 0; i < allObjects.Num(); i++) {
		if (allObjects[i] != nullptr) {
			finalArray.Add(allObjects[i]);
		}
	}*/
	/*if (allObjects.Num() > 10) {
		allObjects.RemoveAt(0, 10 - allObjects.Num(), true);
		
	}*/
	for(int i =0;i<10 && allObjects.Num() > 12;i++) {
		if (allObjects[0]->destroy()) {
			allObjects.RemoveAt(0);
		}
	}


	return allObjects;

}




