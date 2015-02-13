# UDP-Pinger
Exchanging an UDP message between client and server, and show the Round Trip Time (RTT).

# Description
There is a sender program and a receiver program. The sender program will send “ping” message with UDP socket to the receiver program. If the receiver got the “ping” message, it will return a “pong” message to the sender. After that, the sender will show the RTT of this message exchange. The time out is be set by 1 second. If the sender is not received the “pong” message in 1 second, it will send the next message.

# Usage
Udp_sender: ./udp_sender &lt;hostname> &lt;port> &lt;message_number><br />
e.g. Sending 10 messages to host name: ./udp_sender example.com 18300 10 <br />
e.g. Sending 5 messages to ip address: ./udp_sender 127.0.0.1 18300 5<br />
Udp_receiver: ./udp_receiver &lt;port><br />
e.g. Listening port 18300: ./udp_receiver 18300<br />

# Compile the source code
A Makefile for compile is privided.<br />
Use “make” command to compile the programs <br />
Use “make clean” to delete the executive files <br />