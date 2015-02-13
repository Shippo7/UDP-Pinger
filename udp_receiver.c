//
//  udp_receiver.c
//  
//
//  Created by XueFei Yang on 2015-02-11.
//
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSIZE 100

int main(int argc, char **argv){
    struct sockaddr_in myaddr;	//my address
    struct sockaddr_in remaddr;	//remote address
    socklen_t addrlen = sizeof(remaddr);
    int fd; //my socket
    int port; //port number
    char buf[BUFSIZE]; //receive buffer
    
    /* Get port number */
    if(argc==2){
        port = atoi(argv[1]);
    }
    else{
        printf("Usage: %s <port_number>\n", argv[0]);
        exit(1);
    }
    
    
    /* Create a UDP socket */
    
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("cannot create socket\n");
        exit(1);
    }
    
    memset((char *)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(port);
    
    if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
        perror("bind failed");
        exit(1);
    }

    /* Receive data and send back to the sender */
    for (;;) {
        printf("\nListening on port %d ...\n", port);
        int recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
        if (recvlen >= 0) {
            buf[recvlen] = 0;
            printf("\nPing Message Received\n");
        }
        else{
            printf("\nMessage Receive Failed\n");
            printf("\n---------------\n");
            continue;
        }
        if (sendto(fd, buf, strlen(buf), 0, (struct sockaddr *)&remaddr, addrlen) < 0){
            perror("\nMessage Send Failed\n");
        }
        else{
            printf("\nPong Message Send\n");
        }
        printf("\n---------------\n");
    }
    
    return 0;
}
