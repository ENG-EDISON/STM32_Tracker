/*
 * SIM900.c
 *
 *  Created on: 16 Oct 2022
 *      Author: EDISON NGUNJIRI
 */
#include "SIM900.h"
#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern IWDG_HandleTypeDef hiwdg;


extern int Timer_count;
extern int n;
char buffer2[SIM900BufSize];
unsigned int MessageFlag=0;

void Sim900Init(UART_HandleTypeDef *huart)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_UARTEx_ReceiveToIdle_DMA(huart,(uint8_t*)buffer2, SIM900BufSize);
	HAL_IWDG_Refresh(&hiwdg);
}

int SendCommand(char *Command,char *response)
{
	char *P={0};
	char *err={0};
	char *GsmRead={0};
	char *NewMsg={0};
	HAL_UART_Transmit_IT(&huart3,(uint8_t*)Command,strlen(Command));
	while(*P!=response[0])
	{
		P=strstr(buffer2,response);
		err=strstr(buffer2,"ERROR\r\n");
		GsmRead=strstr(buffer2,"PSUTTZ");
		NewMsg=strstr(buffer2,"+CMTI");
		if(*NewMsg=='+')
		{
			MessageFlag=1;
		}
		if(*err=='E')
		{
			memset(buffer2,'\0',strlen(buffer2));
			return GsmError;
		}
		if(*GsmRead=='P')
		{
			return GsmReady;
		}
	}
	HAL_IWDG_Refresh(&hiwdg);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer2, strlen(buffer2), HAL_MAX_DELAY);
	memset(buffer2,'\0',strlen(buffer2));
	return Gsmok;
}

int SetSim900InternetConnectivity(void)
{
	while(SendCommand(test,"OK\r\n")!=Gsmok);
	SendCommand(SetAPN,"OK\r\n");//Set APN to your network provider
	SendCommand(ActGPRS,"OK\r\n");
	SendCommand(GetIP,"OK\r\n");
	SendCommand(ConnectIP,"OK\r\n");
	return Gsmok;
}

int SIM900HttpRequest(char *UrlLink,GpsInfo * Gps)
{

	char RQSTLink[250]={0};
	if(Gps->GpsStatus =='A')
	{
	sprintf(RQSTLink,"%slon=%.8f&lat=%.8f\"\r\n",SetURL,Gps->Lon,Gps->Lat);
	SendCommand(InitHTTP,"OK\r\n");
	SendCommand(EnableHTTPS,"OK\r\n");
	SendCommand(SetCIDPAR,"OK\r\n");
	SendCommand(RQSTLink,"OK\r\n");
	SendCommand(HTTPRequest,"+HTTPACTION:");
	SendCommand(HTTPResponse,"OK\r\n");
	SendCommand(TERMINATEHTTP,"OK\r\n");
	}
	else
	{
		return GsmError;
	}

	return Gsmok;
}

int SIM900SendSms(char *phoneNumber,char *Message)
{
	char SetPara[40]={0};
	char ch[1]={0};
	int n=26;
	ch[0]=n;
	strcat(SetPara,"AT+CMGS=\"");
	strcat(SetPara,phoneNumber);
	strcat(SetPara,"\"\r\n");
	SendCommand(SmsTextMode,"OK\r\n");
	SendCommand(SetPara,">");
	SendCommand(Message,">");
	SendCommand(ch,"OK\r\n");
	return Gsmok;
}


