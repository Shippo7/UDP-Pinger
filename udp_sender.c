//
//  udp_sender.c
//  
//
//  Created by XueFei Yang on 2015-02-11.
//
//

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define TIMEOUT 2
#define BUFSIZE 100

int main(int argc, char **argv){
    struct sockaddr_in myaddr;	//my address
    struct sockaddr_in remaddr;	//remote address
    socklen_t slen=sizeof(remaddr);
    int fd; //my socket
    int port; //port number
    char buf[BUFSIZE]; //receive buffer
    int times; //number of messages send
    char host_to_contact[50];
    struct hostent *hp, *gethostbyname();
    char ip[100];
    struct hostent *he;
    struct in_addr **addr_list;
    struct timeval starttime, endtime;//init the clock
    struct timeval timeout={TIMEOUT,0}; //set timeout
    
    /* Get host name, port number, loop times */
    if(argc==4){
        strncpy(host_to_contact, argv[1], sizeof(host_to_contact));
        port=atoi(argv[2]);
        times = atoi(argv[3]);
    }
    else{
        printf("Usage: %s <host name> <port number> <number of messages>\n", argv[0]);
        exit(1);
    }
    
    /* Create a UDP socket */
    
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("cannot create socket\n");
        exit(1);
    }
    
    /* Get IP address from Hostname */
    
    he = gethostbyname(host_to_contact);
    
    addr_list = (struct in_addr **) he->h_addr_list;
    
    for(int i = 0; addr_list[i] != NULL; i++)
    {
        strcpy(ip , inet_ntoa(*addr_list[i]) );
    }
    
    memset((char *)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(0);
    
    if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
        perror("bind failed");
        return 0;
    }
    
    memset((char *) &remaddr, 0, sizeof(remaddr));
    remaddr.sin_family = AF_INET;
    remaddr.sin_port = htons(port);
    if (inet_aton(ip, &remaddr.sin_addr)==0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
    
    /* Set receive UDP message timeout */
    
    setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(struct timeval));
    
    /* Send the messages and calculate RTT */
    
    for (int i=0; i < times; i++) {
        char send_buf[] = "ping";
        if (sendto(fd, send_buf, strlen(buf), 0, (struct sockaddr *)&remaddr, slen)==-1) {
            perror("sendto");
            printf("\n---------------\n");
            continue;
        }
        gettimeofday(&starttime,0);
        printf("Ping packet send to %s port %d\n", host_to_contact, port);
        
        /* Waiting message come back */
        int recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &slen);
        if (recvlen >= 0) {
            buf[recvlen] = 0;
            printf("\nPong Message Received\n");
            gettimeofday(&endtime,0);
            double timeuse = 1000000*(endtime.tv_sec - starttime.tv_sec) + endtime.tv_usec - starttime.tv_usec;
            timeuse /=1000;
            printf("RTT: %.3f Seconds\n", timeuse);
        }
        else{
            printf("\nMessage Receive Timeout\n");
            printf("\n---------------\n");
            continue;
        }
    }
    close(fd);


    return 0;
}
