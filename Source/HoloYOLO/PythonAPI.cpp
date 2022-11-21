// Fill out your copyright notice in the Description page of Project Settings.

#include "PythonAPI.h"
#include <string>
// Public DependencyModuleNames -> "PythonScriptPlugin" && "Python"
//#include "../Plugins/Experimental/PythonScriptPlugin/Source/PythonScriptPlugin/Private/PythonScriptPlugin.h"

using namespace std;

typedef struct Item {
	long long unsigned id;
	float x, y;
	string name, action;
	Item() {};
} Item;


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

void UPythonAPI::ProcessJSON(FString JSONSerialized, TArray<FVector2D>& itemPositions, TArray<FString>& itemNames, TArray<FString>& itemActions) {

	// Single json value (any of supported json types e.g.
	// object with properties, array, bool) at top level of json
	TSharedPtr<FJsonValue> JsonValue;

	// Create a reader pointer to read the json data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JSONSerialized);
	
	// Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonValue)) {
		// Get the value of the json object by field name
		auto items = JsonValue->AsObject()->GetArrayField("items");
		for (auto& item : items){
			item.Get()->AsObject()->GetStringField("name");
			itemPositions.Add(FVector2D(item.Get()->AsObject()->GetNumberField("x"), item.Get()->AsObject()->GetNumberField("y")));
			itemNames.Add(FString(item.Get()->AsObject()->GetStringField("name")));
			itemActions.Add(FString(item.Get()->AsObject()->GetStringField("action")));

		}
		
	}

}