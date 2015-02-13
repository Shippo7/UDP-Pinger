all: sender receiver

sender: udp_sender.c
	gcc -std=c99 -D_BSD_SOURCE -o udp_sender udp_sender.c

receiver: udp_receiver.c
	gcc -o udp_receiver udp_receiver.c

clean: 
	-rm udp_sender udp_receiver

