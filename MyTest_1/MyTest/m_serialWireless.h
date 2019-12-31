//#pragma once
#ifndef __M_SERIAL_WIRELESS_H__
#define __M_SERIAL_WIRELESS_H__
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "m_debug.h"
#include "m_photo.h"


#define		SERIAL_WIRELESS_RX		7
#define		SERIAL_WIRELESS_TX		8
#define		SERIAL_WIRELESS_M0M1	9
#define		RX_BUF_MAX_LEN			128


class _CMD
{
public:
	unsigned int CMD_READ_CONFIG[3] = {0xC1, 0xC1, 0xC1};
	unsigned int cmd_read_version[3] = {0xC3, 0xC3, 0xC3};
};


typedef struct
{
	char Data_RX_BUF[RX_BUF_MAX_LEN];
	int Data_RX_BUF_LENGTH;
}_WirelessReceive;


enum Enum_Baudrate
{
	B_1200 = 0x00,
	B_2400 = 0x01,
	B_4800 = 0x02,
	B_9600 = 0x03,
	B_19200 = 0x04,
	B_38400 = 0x05,
	B_57600 = 0x06,
	B_115200 = 0x07
};

enum Enum_AirSpeed
{
	AirSpeed_0_3K = 0x00,
	AirSpeed_1_2K = 0x01,
	AirSpeed_2_4K = 0x02,
	AirSpeed_4_8K = 0x03,
	AirSpeed_9_6K = 0x04,
	AirSpeed_19_2K = 0x05
};

enum Enum_SendPower
{
	SendPower_20dBm = 0x00,
	SendPower_17dBm = 0x01,
	SendPower_14dBm = 0x02,
	SendPower_11dBm = 0x03
};

enum Enum_dBm
{
	dBm_20 = 0x00,
	dBm_17 = 0x01,
	dBm_14 = 0x02,
	dBm_11 = 0x03
};

typedef struct
{
	byte ADDH;
	byte ADDL;
	byte SPEED;
	byte CHAN;
	byte OPTION;
} _WirelessRegister;



void serialWirelessInit();
void setAirSpeed(Enum_AirSpeed airSpeed);
void setAirChannel(unsigned int channel);
void setSendPower(Enum_dBm _dBm);
void readWirelessConfig();
void wirelessSend(char* str);
void serialWirelessReceive();
unsigned int getAirSpeed();
unsigned int getAirChannel();
unsigned int getSendPower();
void model_setting();
void model_normal();
void clearBuf();

#endif // !__SERIAL_WIRELESS_H__

