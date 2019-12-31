#include "m_serialWireless.h"

bool aaaaa = true;

_CMD* CMD = new _CMD();
_WirelessReceive WirelessReceive = {0};
_WirelessRegister WirelessRegister;
SoftwareSerial m_serial(SERIAL_WIRELESS_RX, SERIAL_WIRELESS_TX);
long baudrate = 9600;


void model_setting()
{
	digitalWrite(SERIAL_WIRELESS_M0M1, HIGH);
	DEBUG_PRINTLN("model_setting");
}

void model_normal()
{
	digitalWrite(SERIAL_WIRELESS_M0M1, LOW);
	DEBUG_PRINTLN("model_normal");
}


void serialWirelessInit()
{
	pinMode(SERIAL_WIRELESS_M0M1, OUTPUT);
	model_setting();
	m_serial.begin(baudrate);
	m_serial.listen();
	DEBUG_PRINTLN("wireless software serial pin init [ok]");
}

void serialWirelessReceive()
{
	unsigned int uUch;
	while (m_serial.available())
	{
		uUch = m_serial.read();
		if (uUch == 'a')
		{
			DEBUG_PRINTLN("aaaaaaa");
			if ((unsigned int)m_serial.read() == '1')
			{
				takePhoto();
			}
		}
		else
		{	
			if (WirelessReceive.Data_RX_BUF_LENGTH < (RX_BUF_MAX_LEN - 1))
			{
				WirelessReceive.Data_RX_BUF[WirelessReceive.Data_RX_BUF_LENGTH++] = (char)uUch;
			}
		}
	}

	//for (int i = 0; i < WirelessReceive.Data_RX_BUF_LENGTH; i++)
	//{
	//	DEBUG_PRINTLN((int)WirelessReceive.Data_RX_BUF[i]);
	//}

	//if (WirelessReceive.Data_RX_BUF[0] == 'a')
	//{
		//DEBUG_PRINTLN(WirelessReceive.Data_RX_BUF_LENGTH);
	//}
	//DEBUG_PRINT("string = ");
	//DEBUG_PRINTLN(readBuffer);
}


void wirelessSend(char* str)
{
	m_serial.write(str);
}

void serialWirelessSend(unsigned int *cmd, int size)
{
	for (int i = 0; i < size; i++)
	{
		m_serial.write(cmd[i]);
	}
}


bool wirelessCmd(unsigned int *cmd, int size, char* ack)
{
	WirelessReceive.Data_RX_BUF_LENGTH = 0;
	serialWirelessSend(cmd, size);
	serialWirelessReceive();
	delay(100);
	WirelessReceive.Data_RX_BUF[WirelessReceive.Data_RX_BUF_LENGTH] = '\0';
	
	if (ack == 0)
		return true;
	if (ack != 0)
		return (bool) strstr(WirelessReceive.Data_RX_BUF, ack);
	
}


void readWirelessConfig()
{
	char count = 0;
	int cmdSize = sizeof(CMD->CMD_READ_CONFIG) / sizeof(CMD->CMD_READ_CONFIG[0]);
	while (count < 10)
	{
		if ( wirelessCmd(CMD->CMD_READ_CONFIG, cmdSize, NULL) )
		{
			WirelessRegister.ADDH = WirelessReceive.Data_RX_BUF[1];
			WirelessRegister.ADDL = WirelessReceive.Data_RX_BUF[2];
			WirelessRegister.SPEED = WirelessReceive.Data_RX_BUF[3];
			WirelessRegister.CHAN = WirelessReceive.Data_RX_BUF[4];
			WirelessRegister.OPTION = WirelessReceive.Data_RX_BUF[5];

			DEBUG_PRINTLN("%^%^%^%^%^%^%^%^%^%^%^%^%^%");
			DEBUG_PRINTLN((unsigned char)WirelessRegister.ADDH);
			DEBUG_PRINTLN((unsigned char)WirelessRegister.ADDL);
			DEBUG_PRINTLN((unsigned char)WirelessRegister.SPEED);
			DEBUG_PRINTLN((unsigned char)WirelessRegister.CHAN);
			DEBUG_PRINTLN((unsigned char)WirelessRegister.OPTION);
			DEBUG_PRINTLN("%^%^%^%^%^%^%^%^%^%^%^%^%^%");
			WirelessReceive = {0};
			return;
		}
		++count;
	}
}

void clearBuf()
{
	WirelessReceive.Data_RX_BUF_LENGTH = 0;
	for (int i = 0; i < 128; i++)
	{
		WirelessReceive.Data_RX_BUF[i] = 0;
	}
	
}


/**********************************************/
/*
设置 1.SPEED, 2.CHANL, 3.OPTION
*/


/*
* 1. SPEED
* 1.1 设置空速
*/
void setAirSpeed(Enum_AirSpeed airSpeed)
{
		char count = 0;
		byte t_a = WirelessRegister.SPEED & 0xF8;
		byte t_b = t_a |= airSpeed;
		DEBUG_PRINTLN(t_b);
		DEBUG_PRINTLN("%^%^%^%^%^%^%^%^%^%^%^%^%^%");
		unsigned int cmdList[6] = {0xC0, WirelessRegister.ADDH, WirelessRegister.ADDL, t_b, WirelessRegister.CHAN, WirelessRegister.OPTION};
	
		while (count < 10)
		{
			if (wirelessCmd(cmdList, 6, "OK"))
			{
				DEBUG_PRINTLN("Set AirApeed Success.");
				return;
			}
			DEBUG_PRINTLN("Set AirApeed Fail.");
			++count;
		}
}


/*
* 2. CHANL
* 2.1 设置信道(0 - 31)
*/
void setAirChannel(unsigned int channel)
{
		char count = 0;
		unsigned int _chan = channel;
		if (_chan < 0)
			_chan = 0;
		else if (_chan >= 31)
			_chan = 31;

		unsigned int cmdList[] = { 0xC0, WirelessRegister.ADDH, WirelessRegister.ADDL, WirelessRegister.SPEED, _chan, WirelessRegister.OPTION };
		while (count < 10)
		{
			if (wirelessCmd(cmdList, 6, "OK"))
			{
				DEBUG_PRINTLN("Set Chan Success.");
				return;
			}
			DEBUG_PRINTLN("Set Chan Fail.");
			++count;
		}
}


/*
* 3.OPTION
* 3.1 设置发射功率
*/
void setSendPower(Enum_dBm _dBm)
{
		char count = 0;
		byte t_a = WirelessRegister.OPTION & 0xFC;
		byte t_b = t_a |= _dBm;
		unsigned int cmdList[6] = { 0xC0, WirelessRegister.ADDH, WirelessRegister.ADDL, WirelessRegister.SPEED, WirelessRegister.CHAN, t_b };

		while (count < 10)
		{
			if (wirelessCmd(cmdList, 6, "OK"))
			{
				DEBUG_PRINTLN("Set dBm Success.");
				return;
			}
			DEBUG_PRINTLN("Set dBm Fail.");
			++count;
		}
}


unsigned int getAirSpeed()
{
	byte speed = WirelessRegister.SPEED & 0x07;
	return (Enum_AirSpeed)speed;
}


unsigned int getAirChannel()
{
	return (Enum_AirSpeed) WirelessRegister.CHAN;
}

unsigned int getSendPower()
{
	byte sp = WirelessRegister.OPTION & 0x03;
	return (Enum_dBm) sp;
}

