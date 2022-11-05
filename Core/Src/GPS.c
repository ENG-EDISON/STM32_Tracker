/*
 * GPS.c
 *
 *  Created on: Nov 3, 2022
 *      Author: EDISON NGUNJIRI
 */
#include "GPS.h"
#include "SIM900.h"


extern DMA_HandleTypeDef GpsDMA;
extern UART_HandleTypeDef GpsUart;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart3_rx;

char GPSString[GpsBufferSize];
char Delimeter[2]="$";
char *Token;
int Donereading=0;
char GPSData[GpsBufferSize];
char SIM900Data[SIM900BufSize];
extern char buffer2[SIM900BufSize];

GpsInfo NEO_6M={0};

void GpsInit(UART_HandleTypeDef *huart)
{
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1,(uint8_t*)GPSString, GpsBufferSize);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart->Instance==USART1)
	{
		if(Donereading==0)
		{
			memcpy(GPSData,GPSString,strlen(GPSString));
		}
		HAL_UARTEx_ReceiveToIdle_DMA(&GpsUart,(uint8_t*)GPSString, GpsBufferSize);
		__HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);
		Donereading=1;
	}

	if(huart->Instance==USART3)
	{
		memcpy(SIM900Data,buffer2,strlen(buffer2));
		HAL_UARTEx_ReceiveToIdle_DMA(&huart3,(uint8_t*)buffer2, SIM900BufSize);
		__HAL_DMA_DISABLE_IT(&hdma_usart3_rx,DMA_IT_HT);
	}
}


void ReadGPSCordinate(GpsInfo *Gps)
{
	float latitude,longitude;
	latitude=longitude=0.0;
	double Deg_Val=0.0,Min_Val=0.0,Sec_Val=0.0;
	if(Donereading==1)
	{
		Token=strtok(GPSData,Delimeter);
		sscanf(GPSData,"$GPRMC,%f,%c,%f,%c,%f,%c",&Gps->GPSTime,&Gps->GpsStatus,&latitude,&Gps->N_S,&longitude,&Gps->E_W);
		if(Gps->GpsStatus=='A')
		{
			Deg_Val=(int)(longitude/100);
			Min_Val=(int)(longitude-(Deg_Val*100));
			Sec_Val=((longitude-(Deg_Val*100))-Min_Val)*100;
			if(Gps->E_W == 'E' && Gps->N_S=='S')
			{
				Gps->Lon=(Deg_Val+(Min_Val*Min_To_Degree)+(Sec_Val*Sec_To_Degree));
			}
			Deg_Val=(int)((latitude/100));
			Min_Val=(int)(latitude-(Deg_Val*100));
			Sec_Val=(((latitude-(Deg_Val*100))-Min_Val))*10;
			if(Gps->E_W == 'E' && Gps->N_S=='S')
			{
				Gps->Lat=(Deg_Val+(Min_Val*Min_To_Degree)+(Sec_Val*Sec_To_Degree))*-1;
			}
			memset(GPSData,'\0',strlen(GPSData));
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
			Donereading=0;
		}
		else
		{
			HAL_Delay(10000);
		}
	}
}
