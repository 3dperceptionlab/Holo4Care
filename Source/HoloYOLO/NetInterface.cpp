// Fill out your copyright notice in the Description page of Project Settings.


#include "NetInterface.h"

// Sets default values
ANetInterface::ANetInterface()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

int ANetInterface::CreateClient() {
    // Set connection
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary format
    if (inet_pton(AF_INET, IP_ADDR, &serv_addr.sin_addr) <= 0) {
        printf("\n ERROR: Invalid address / Address not supported \n");
        return -1;
    }
    if ((client_fd = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        printf("\n ERROR: Connection failed \n");
        return -1;
    }

    return 0;
}
int ANetInterface::CreateServer() {

    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        return -1;

    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET,
        SO_REUSEADDR | SO_REUSE_UNICASTPORT, (char*) &opt,
        sizeof(opt))) {
        perror("setsockopt");
        return -1;

    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return -1;
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        return -1;
    }

    return 0;
}

// Called when the game starts or when spawned
void ANetInterface::BeginPlay(){
	
    Super::BeginPlay(); 
    isServer = IsRunningDedicatedServer();

    if (isServer) {
        printf("creating server: %d", CreateServer());
    }else {
        printf("creating client: %d", CreateClient());

    }
	
}


int ANetInterface::SendImage(){
    char* FilePath = "C:\\Users\\porygon2\\Desktop\\HoloYOLOGit\\HoloYOLO\\Executable\\test.png";
	int n = 0,size = 0;
	std::ifstream picture;
	char buf[50];

    // Read image size
    // Read image size
    printf("Getting image size \n");
    picture.open(FilePath, std::ios::in | std::ios::binary);
 
    if (!picture.is_open()) {
        printf("ERROR: opening file");
        return -1;
    }


    picture.seekg(0, std::ios::end);
    size = picture.tellg();
    printf("Image size: %d \n", size);
    UE_LOG(LogTemp, Warning, TEXT("Image size: %d"),size);

    // Send image size
    printf("Sending picture size to server\n");
    sprintf(buf, "%d", size);
    if ((n = send(sock, buf, 50, 0)) <= 0) {
        printf("ERROR: sending size");
        return -1;
    }

    // Send image
    char* imgBuf = new char(size);
    
    printf("Sending picture as byte array\n");
    picture.seekg(0, std::ios::beg);


    /*while (!feof(picture)) { // Make sure all data is sent
        n = fread(imgBuf, sizeof(char), size, picture);
        UE_LOG("Bytes: %d", n);
        if (n > 0) { // Only send what is read
            if ((n = send(sock, imgBuf, size, 0)) <= 0) {
                printf("ERROR: sending image");
                return -1;
            }
        }
    }*/

    picture.read(imgBuf, size);
    if (picture) {
        printf("All bytes loaded");
        UE_LOG(LogTemp,Warning,TEXT("All bytes loaded"));

    }
    else {
        printf("Not all bytes read");
        UE_LOG(LogTemp, Warning, TEXT("Not all bytes read"));
    }

    if ((n = send(sock, imgBuf, size, 0)) <= 0) {
        printf("ERROR: sending image");
        UE_LOG(LogTemp, Warning, TEXT("Error sending image"));

        return -1;
    }

    delete[] imgBuf;

    return 0;


}

int ANetInterface::ReceiveImage() {
    int addrlen = sizeof(address);
    int n = 0,size = 0;
    char buf[50];

    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        return -1;
    }
    n = recv(new_socket, buf, 50, 0);
    if (n < 0) {
        perror("recv_size()");
        return -1;
    }
    printf("Size: %s\n", buf);
    size = atoi(buf);

    // char imgBuf[size];
    char* imgBuf = new char(size);
    printf("Reading image byte array\n");
    n = 0;
    int bytesRead = 0;
    while (bytesRead < size) { // Make sure all data is received
        // if ((n = recv(new_socket, imgBuf, size, 0)) <= 0){
        if ((n = recv(new_socket, imgBuf + bytesRead, size - bytesRead,0)) <= 0) {
            perror("recv_size()");
            return -1;
        }
        bytesRead += n;
    }


    printf("Converting byte array to image. \n");
    FILE* image;
    image = fopen("test_server.png", "wb");
    //fwrite((void*)imgBuf, sizeof(char), size, image);
    fwrite(imgBuf, sizeof(char), size, image);
    fclose(image);

    return 0;

}

void ANetInterface::EndPlay(EEndPlayReason::Type endPlayReason)
{
    Super::EndPlay(endPlayReason);

 printf("Done. Closing connection.\n");

 if (isServer) {
     closesocket(new_socket);
     // closing the listening socket
     shutdown(server_fd,0); // UE4: Leave open?
 }else {
     closesocket(client_fd); // UE4: Leave open?

 }

}
// Called every frame
void ANetInterface::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

