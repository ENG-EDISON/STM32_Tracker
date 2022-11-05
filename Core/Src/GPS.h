/*
 * GPS.h
 *
 *  Created on: Nov 3, 2022
 *      Author: EDISON NGUNJIRI
 */
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define Min_To_Degree  0.0166666667
#define Sec_To_Degree	 0.0002777778
#define GpsBufferSize 700
#define GpsUart  huart1
#define GpsUSARTPeripheral USART1
#define GpsDMA hdma_usart1_rx


#ifndef SRC_GPS_H_
#define SRC_GPS_H_

typedef struct GpsInfo
{
	char N_S;
	char E_W;
	float GPSTime;
	float Lat;
	float Lon;
	int GPSDate;
	char GpsStatus;

}GpsInfo;

void GpsInit(UART_HandleTypeDef *huart);
void ReadGPSCordinate(GpsInfo *Gps);

#endif /* SRC_GPS_H_ */
