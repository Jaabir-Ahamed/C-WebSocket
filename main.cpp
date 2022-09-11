//
//  main.cpp
//  JASocket
//
//  Created by Jaabir Saleem on 9/7/22.
//

#include <iostream>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <string>

#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <sstream>

#define PORT 5000


int main(int argc, const char * argv[]) {
    
    //SERVER
    struct sockaddr_in saddr {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(PORT)
    };
    
    int option = 1;
    int saddersize = sizeof(saddr);
    int socketServer = socket(AF_INET, SOCK_STREAM, 0); //SOCK_DGRAM
    setsockopt(socketServer, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option));
    if(socketServer == -1) {
        std::cout << "Not able to establish socket! Exiting" << std::endl;
        return  -1;
    }
    
    
    
    //CLIENT
    struct sockaddr_in cadder;
    socklen_t caddersize = sizeof(cadder);
    int socketClient;
    
    
    //Binding our Socket to the ip+port
    bind(socketServer, (struct sockaddr*)&saddr, sizeof(saddr));
    
    //Listening
    listen(socketServer, SOMAXCONN);
    //Print
    std::stringstream ss;
    ss << PORT;
    std::cout << "[Server] Listening on port " << ss.str() << std::endl;
    
    char buff[4096];
    int sizeInBytesOfRecivedData;
    
    //WHILE waiting for client
    while (true) {
        //Accept connections from clients
        socketClient = accept(socketServer, (struct sockaddr*)&cadder, (socklen_t*)&caddersize);
        std::cout << "[Server] Client succesuflly connected. " << std::endl;
        
        // Try to find out who is the client
        char hostClient[NI_MAXHOST];
        char portClient[NI_MAXHOST];
        memset(hostClient, 0, NI_MAXHOST);
        memset(portClient, 0, NI_MAXHOST);
        if (getnameinfo((sockaddr*)&cadder, sizeof(cadder), hostClient, NI_MAXHOST, portClient, NI_MAXHOST, 0) == 0) {
            std::cout << " --> " << hostClient << " port connection succesful " << portClient << std::endl;
        } else {
            inet_ntop(AF_INET, &cadder.sin_addr, hostClient, NI_MAXHOST);
            std::cout << " --> " << hostClient << " connected to the port " << ntohs(cadder.sin_port) << std::endl;
        }
        
        
        //Recive our data
        sizeInBytesOfRecivedData = recv(socketClient, buff, 4096, 0);
        if (sizeInBytesOfRecivedData == -1) {
            std::cerr << "Error reciving message. Terminating";
            break;
        } else if (sizeInBytesOfRecivedData == 0){
            std::cout << "Client Disconected" << std::endl;
            break;
        }
        send(socketClient, buff, sizeInBytesOfRecivedData + 1, 0);
        
        
        
        std::cout << std::string(buff, 0, sizeInBytesOfRecivedData) << std::endl;
        
        close(socketClient);
        
    }
    
    return 0;
}
