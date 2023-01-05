// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h> // for linux #include <arpa/inet.h> #include <sys/socket.h> 
#include <Ws2tcpip.h>  // windows
#include "CoreMinimal.h"
#include <fstream>
#include "GameFramework/Actor.h"
#include "NetInterface.generated.h"

#define PORT 5000
#define IP_ADDR "172.16.34.101"
#define _CRT_SECURE_NO_WARNINGS

UCLASS()
class HOLOYOLO_API ANetInterface : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANetInterface();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type endPlayReason) override;
	int CreateClient();
	int CreateServer();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	int SendImage();
	UFUNCTION(BlueprintCallable)
	int ReceiveImage();


	int sock = 0, client_fd, server_fd, new_socket;
	bool isServer = false;
	struct sockaddr_in address;


};
