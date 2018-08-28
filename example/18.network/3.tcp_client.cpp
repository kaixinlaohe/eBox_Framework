/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"
#include "bsp_ebox.h"

#include "w5500.h"
#include "socket.h"
#include "tcp.h"


/**
	*	1	��������Ҫ����eDriveĿ¼�µ�w5500������network���
	*	2	��������ʾ�˴���һ��TCP�Ŀͻ��ˣ�����Զ�̷����������ղ���������
	*/



/* ���������������̷������� */
#define EXAMPLE_NAME	"w5500 udp example"
#define EXAMPLE_DATE	"2018-08-11"


u8 mac[6] = {0x00, 0x08, 0xdc, 0x11, 0x11, 0x11}; /*����Mac����*/
u8 ip[4] = {192, 168, 1, 119}; /*����lp����*/
u8 sub[4] = {255, 255, 255, 0}; /*����subnet����*/
u8 gw[4] = {192, 168, 1, 1}; /*����gateway����*/
u8 dns[4] = {192, 168, 1, 1}; /*��dns����*/

u8 buf[100];

W5500 w5500(&PC13, &PC14, &PC15, &spi2);

TCPClient tcp;
uint16_t len;
u8 rip[4] = {192, 168, 1, 108}; /*����lp����*/

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);
    w5500.begin(2, mac, ip, sub, gw, dns);

    attach_eth_to_socket(&w5500);

    w5500.getMAC (buf);
    UART.printf("\r\nmac : %02x.%02x.%02x.%02x.%02x.%02x\r\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
    w5500.getIP (buf);
    UART.printf("IP : %d.%d.%d.%d\r\n", buf[0], buf[1], buf[2], buf[3]);
    w5500.getSubnet(buf);
    UART.printf("mask : %d.%d.%d.%d\r\n", buf[0], buf[1], buf[2], buf[3]);
    w5500.getGateway(buf);
    UART.printf("GW : %d.%d.%d.%d\r\n", buf[0], buf[1], buf[2], buf[3]);
    UART.printf("Network is ready.\r\n");



    if(tcp.begin(SOCKET7, 3000) == 0)
        UART.printf("tcp client creat ok!\r\n");
    if(tcp.connect(rip, 8080)  == 0)
        UART.printf("udp server creat ok!listen on 3000\r\n");

}
int main(void)
{
    setup();

    while(1)
    {
        len = tcp.recv(buf);
        if(len > 0)
        {
            uart1.printf("\r\n============================");
            uart1.printf("\r\n���ض˿�:%d", tcp.local_port );
            uart1.printf("\r\n��Ϣ��Դ:%d.%d.%d.%d:%d", tcp.remote_ip[0], tcp.remote_ip[1], tcp.remote_ip[2], tcp.remote_ip[3], tcp.local_port);
            uart1.printf("\r\n���ݳ���:%d", len);
            uart1.printf("\r\n��������:");
            uart1.printf((const char *)buf);
            tcp.send(buf, len);
        }
    }


}



