// Fill out your copyright notice in the Description page of Project Settings.

#include "PythonAPI.h"
#include <string>
// Public DependencyModuleNames -> "PythonScriptPlugin" && "Python"
//#include "../Plugins/Experimental/PythonScriptPlugin/Source/PythonScriptPlugin/Private/PythonScriptPlugin.h"

using namespace std;



FString UPythonAPI::ExecuteSystemCommand(FString Command, FString Params) {
	const TCHAR* command = *Command;
	const TCHAR* params = *Params;
	int32* returnCode = new int32(-1);
	FString* outStr = new FString();
	FString* outStrErr = new FString();


	// ********************* OTHER POSIBLE SOLUTION ****************************************
	//void* readPipe;
	//void* writePipe; 
	//FPlatformProcess::CreatePipe(readPipe, writePipe);
	//FPythonScriptPlugin::Get()->ExecPythonCommand(*PythonScript);
	//FProcHandle handle = FPlatformProcess::CreateProc(command, params, false, false, false, nullptr, 0, nullptr, writePipe, readPipe);
	//out += FPlatformProcess::ReadPipe(readPipe)
	// *************************************************************************************

	FPlatformProcess::ExecProcess(command, params, returnCode, outStr, outStrErr);
	FString out = "";

	int aux = *returnCode;
	out += *outStr;
	return out;
}



TArray<uint8> UPythonAPI::LoadFile(APawn* pawn){
	UE_LOG(LogTemp, Warning, TEXT("Trying to load file"));

	TArray<uint8> dataLoaded;
	/*FILE* picture = fopen("C:\\Users\\porygon2\\Desktop\\HoloYOLOGit\\HoloYOLO\\Executable\\test.png", "rb");
	fseek(picture, 0, SEEK_END);
	int size = ftell(picture);
	fseek(picture, 0, SEEK_SET);
	//fread(dataLoaded, sizeof(char), size, picture);*/
	// Abre el archivo para lectura
	FString FilePath = "C:\\Users\\porygon2\\Desktop\\HoloYOLOGit\\HoloYOLO\\Executable\\test.png";
	//std::string path = std::string(TCHAR_TO_UTF8(*FilePath));
	if (FFileHelper::LoadFileToArray(dataLoaded, *FilePath)){
		//UE_LOG(LogTemp, Warning, TEXT("File %s Opened"), path.c_str());
		UE_LOG(LogTemp, Warning, TEXT("File Opened"));

	}
	else{
		//UE_LOG(LogTemp, Warning, TEXT("ERROR opening File %s"), path.c_str());
		UE_LOG(LogTemp, Warning, TEXT("ERROR opening File"));
		dataLoaded.Add(5);

	}

	return dataLoaded;
	
	}

bool UPythonAPI::SendDataToServer(uint8 arrayData , APawn* pawn) {
	
	if (pawn != nullptr && pawn->GetNetConnection() != nullptr) {
		UNetConnection* netConnection = pawn->GetNetConnection();
		if (netConnection->State == EConnectionState::USOCK_Open) {
			//FOutBunch* bunch = new FOutBunch();
			//texture->Serialize(*bunch);
			//const TArray<uint8> *arrayData = bunch->GetBuffer();
			//arrayData->ToString()

			/*UE_LOG(LogTemp, Warning, TEXT("Channels:"));
			for (int c = 0; c < netConnection->Channels.Num(); c++) {
				std::string nombre = std::string(TCHAR_TO_UTF8(&netConnection->Channels[c]->ChName));
				UE_LOG(LogTemp, Warning, TEXT("Channel: %s OpenAcked:%d"), nombre.c_str() , netConnection->Channels[c]->OpenAcked);
			}*/

			//netConnection->SendRawBunch(*bunch, true);
			//UChannel* chanel = netConnection->CreateChannelByName("TextureNETChannel", EChannelCreateFlags::None, 0);
			
			//chanel->SendBunch(bunch, 0);
			
			return true;
		}
		//netConnection->SendBunch(&Writer, 0);
	}

	return false;

 }

UTexture2D* UPythonAPI::ReceiveDataFromClient(APawn*pawn ) {
	if (pawn != nullptr && pawn->GetNetConnection() != nullptr) {
		UNetConnection* netConnection = pawn->GetNetConnection();

		if (netConnection->State == EConnectionState::USOCK_Open) {
			UTexture2D* texture = UTexture2D::CreateTransient(1270, 800);
			FArchive Ar;
			Ar << texture;
			
			//FBitReader* Reader = new FBitReader();
			//FInBunch* bunch = new FInBunch(netConnection);

			/*UE_LOG(LogTemp, Warning, TEXT("Channels:"));
			for (int c = 0; c < netConnection->Channels.Num(); c++) {
				std::string nombre = std::string(TCHAR_TO_UTF8(&netConnection->Channels[c]->ChName));
				UE_LOG(LogTemp, Warning, TEXT("Channel: %s OpenAcked:%d"), nombre.c_str(), netConnection->Channels[c]->OpenAcked);
			}*/
			
			//UChannel* chanel = netConnection->CreateChannelByName("TextureNETChannel", EChannelCreateFlags::None, 0);
			//bunch = chanel->InPartialBunch;
			//chanel->ReceivedBunch(*bunch);
			//Reader = bunch->GetData();
			//netConnection->ReceivedPacket(*bunch,false);
			//texture->Serialize(*bunch);
			return texture;
		}
	}
	//netConnection->SendBunch(&Writer, 0);

	return nullptr;

}


bool UPythonAPI::SaveTextureToArchive(FArchive& DataArchive, UTexture2D* Texture)
{
	int32 TextureWidth = Texture->GetSizeX();
	int32 TextureHeight = Texture->GetSizeY();
	int8 TextureFormat = Texture->GetPixelFormat();
	int8 CompressionSettings = Texture->CompressionSettings;
	//int8 MipGenSettings = Texture->MipGenSettings;
	uint8 IsSRGB = Texture->SRGB;
	//FByteBulkData* BulkDataToSave = &Texture->PlatformData->Mips[0].BulkData;
	auto textureMip0 = Texture->PlatformData->Mips[0];
	//FUntypedBulkData* BuldData2;
	DataArchive << TextureWidth;
	DataArchive << TextureHeight;
	DataArchive << TextureFormat;
	DataArchive << CompressionSettings;
	//DataArchive << MipGenSettings;
	DataArchive << IsSRGB;
	//BulkDataToSave->Serialize(DataArchive, nullptr);
	DataArchive.AttachBulkData(nullptr, reinterpret_cast<FUntypedBulkData *>( &textureMip0.BulkData));
	
	return true;
}


UTexture2D* UPythonAPI::LoadTextureFromArchive(FArchive& DataArchive) {

	int32			TextureWidth, TextureHeight;
	int8			TextureFormat, CompressionSettings;// , MipGenSettings;
	uint8			IsSRGB;
	FByteBulkData* BulkDataToFill;

	// Get texture info from archive
	DataArchive << TextureWidth;
	DataArchive << TextureHeight;
	DataArchive << TextureFormat;
	DataArchive << CompressionSettings;
	//DataArchive << MipGenSettings;
	DataArchive << IsSRGB;

	// Create new texture and assign the needed settings to it
	UTexture2D* Texture = UTexture2D::CreateTransient(TextureWidth, TextureHeight, (EPixelFormat)TextureFormat);
	Texture->CompressionSettings = (TextureCompressionSettings)CompressionSettings;
	//Texture->MipGenSettings = (TextureMipGenSettings)MipGenSettings;
	Texture->SRGB = IsSRGB;
	BulkDataToFill = &Texture->PlatformData->Mips[0].BulkData;
	BulkDataToFill->Serialize(DataArchive, nullptr);

	Texture->UpdateResource();
	return Texture;
}

TArray<uint8> UPythonAPI::SaveTextureToFile( UTexture2D* Texture)
{
	FBufferArchive DataToSaveToBinary;
	FMemoryWriter DataSavingArchive(DataToSaveToBinary, true);
	SaveTextureToArchive(DataSavingArchive, Texture);
	TArray<uint8>a;
	if (DataToSaveToBinary.Num() <= 0) return a;

	return DataToSaveToBinary;
		
	/*if (FFileHelper::SaveArrayToFile(DataToSaveToBinary, *FullFilePath)) {
		DataToSaveToBinary.FlushCache();
		DataToSaveToBinary.Empty();
		return true;
	}
	else {
		DataToSaveToBinary.FlushCache();
		DataToSaveToBinary.Empty();
		return false;
	}*/

}

