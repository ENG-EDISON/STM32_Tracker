/*
 * SIM900.h
 *
 *  Created on: 16 Oct 2022
 *      Author: EDISON NGUNJIRI
 */
#include "GPS.h"
#ifndef SRC_SIM900_H_
#define SRC_SIM900_H_
#define SIM900BufSize  2000
#include "stm32f4xx_hal.h"


#define  test           "AT\r\n"
#define  SetAPN         "AT+SAPBR=3,1,\"APN\",\"www.safaricom.com\"\r\n"
#define  ActGPRS        "AT+SAPBR =1,1\r\n"
#define  GetIP         "AT+SAPBR=2,1\r\n"
#define  ConnectIP     "AT+CGATT=1\r\n"
#define  InitHTTP      "AT+HTTPINIT\r\n"
#define  EnableHTTPS   "AT+HTTPSSL=1\r\n"
#define  SetCIDPAR     "AT+HTTPPARA= \"CID\",1\r\n"
#define  Google         "www.google.com\r\n"
#define  SetURL        "AT+HTTPPARA=\"URL\",\"https://script.google.com/macros/s/AKfycbxEPoGshAFIKhv-pHxg6oy4EH_aJG_v9VWqDn5eo7jzrX6dUNS4SqAAiIXy3hnAYXZB7A/exec?"
#define  HTTPRequest   "AT+HTTPACTION=0\r\n"
#define  HTTPResponse  "AT+HTTPREAD\r\n"
#define  TERMINATEHTTP "AT+HTTPTERM\r\n"
//message AT commands
#define SmsTextMode "AT+CMGF=1\r\n"

typedef enum{
	Gsmok=0,
	GsmError,
	GsmReady
}GsmStatus;

void Sim900Init(UART_HandleTypeDef *huart);
int SendCommand(char *Command,char *response);
int SetSim900InternetConnectivity(void);
int SIM900SendSms(char *phoneNumber,char *Message);
int SIM900HttpRequest(char *UrlLink,GpsInfo * Gps);
#endif /* SRC_SIM900_H_ */
