#include "m_menu.h"

U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

_Menu Menu;

unsigned char item_1, item_2;
unsigned int airSpeed, airChannel, airSendPower;
bool _oneTime = true;

const unsigned char MENU_1_LENGTH = 2;
const unsigned char MENU_1_1_LENGTH = 3;
const char* MENU_1_ITEM[MENU_1_LENGTH] = {"1.Wireless Setting", "2.Delay Setting"};
const char* MENU_1_1_ITEM[MENU_1_1_LENGTH] = { "1.SetAirSpeed", "2.SetAirChannel", "3.SetSendPower"};

unsigned char MENU_L, MENU_CRU;


void menuInit()
{
	Menu.status = STATUS_M0;
	u8g2.begin();
	delay(10);
	DEBUG_PRINTLN("menu init [ok]");
}


void menu_test()
{
	u8g2.firstPage();
	do {
		u8g2.drawTriangle(20, 5, 27, 50, 5, 32);
	} while (u8g2.nextPage());
}


void once(int* pot, unsigned int value)
{
	if (_oneTime)
	{
		*pot = value;
		_oneTime = false;
	}
}

const char *convert_number(unsigned int num) {
	static char buf[3];
	strcpy(buf, u8g2_u8toa((unsigned int)(num), 2));
	buf[2] = '\0';
	return buf;
}

void _drawSelectedFlag(unsigned int pot, unsigned char H_space = 0, unsigned char h_space = 12)
{
	unsigned int _step = h_space, start = 16;
	const unsigned int p_x = 6;
	const unsigned int p_y = H_space + start;

	u8g2.drawTriangle(p_x, p_y + _step*pot, p_x+15, p_y+4 + _step * pot, p_x+5, p_y+4 + _step * pot);
	u8g2.drawTriangle(p_x+5, p_y+4 + _step * pot, p_x+15, p_y+4 + _step * pot, p_x, p_y+8 + _step * pot);
}

void _drawStr(unsigned int pot, const char* str, unsigned char H_space=10, unsigned char h_space=12 , unsigned char fontSize=8)
{
	unsigned int _step = h_space, start_y = 16, start_x = 27;
	const unsigned int p_x = 0 + start_x;
	const unsigned int p_y = H_space + start_y + fontSize;
	u8g2.drawStr(p_x, p_y + _step * pot, str);
}


void _drawMenuTitle(char* title)
{
	u8g2.setFont(u8g2_font_ncenB10_tr);
	u8g2.drawStr(40, 11, title);
	u8g2.drawLine(0, 15, 127, 15);
}

void _drawMenuItemNum(int* pot, unsigned char current, unsigned char total, bool disp=true)
{
	if (disp)
	{
		if (*pot > (total - 1))
		{
			*pot = total - 1;
		}
		else if (*pot <= 0)
		{
			*pot = 0;
		}
	}
	else
	{
		u8g2.setFont(u8g2_font_ncenB08_tr);
		u8g2.drawStr(92, 10, convert_number(*pot));
		u8g2.drawStr(106, 10, "/");
		u8g2.drawStr(112, 10, convert_number(total));
	}
}



void menu_0()
{
	airSpeed = getAirSpeed();
	airChannel = getAirChannel();
	airSendPower = getSendPower();
	model_normal();
	u8g2.firstPage();
	do {
		_drawMenuTitle("Main");
		u8g2.setFont(u8g2_font_ncenB08_tr);
		u8g2.drawStr(5, 30, "a_spd: ");
		u8g2.drawStr(80, 30, convert_number(airSpeed));

		u8g2.drawStr(5, 45, "a_cha: ");
		u8g2.drawStr(80, 45, convert_number(airChannel));

		u8g2.drawStr(5, 63, "a_dBm: ");
		u8g2.drawStr(80, 63, convert_number(airSendPower));

	} while (u8g2.nextPage());
}

unsigned char menu_1(int* pot)
{
	model_setting();
	u8g2.firstPage();
	do {
		_drawMenuTitle("M1");
		_drawMenuItemNum(pot, *pot, 2);
		
		u8g2.setFont(u8g2_font_ncenB08_tr);
		for (int i = 0; i < MENU_1_LENGTH; i++)
		{
			_drawStr(i, MENU_1_ITEM[i], 10, 20, 8);
		}
		_drawSelectedFlag(*pot, 10, 20);
		_drawMenuItemNum(pot, *pot, 2, false);

	} while (u8g2.nextPage());
	//DEBUG_PRINT("*pot = ");
	//DEBUG_PRINTLN(*pot);
	return *pot;
}


unsigned char menu_2(unsigned char item, int* pot)
{
	switch (item)
	{
	case 0:
		u8g2.firstPage();
		do {
			_drawMenuTitle("M1.1");
			_drawMenuItemNum(pot, *pot, 3);
			u8g2.setFont(u8g2_font_ncenB08_tr);
			for (int i = 0; i < MENU_1_1_LENGTH; i++)
			{
				_drawStr(i, MENU_1_1_ITEM[i], 10, 14, 8);
			}
			_drawSelectedFlag(*pot, 10, 14);
			_drawMenuItemNum(pot, *pot, 3, false);
		} while (u8g2.nextPage());
		
		return *pot;

	case 1:
		u8g2.firstPage();
		do {
			_drawMenuTitle("M2.1");

		} while (u8g2.nextPage());
		break;

	default:
		break;
	}
	
}

void enum_3(unsigned char item, int* pot)
{
	switch (item)
	{
	case 0:
		once(pot, airSpeed);
		u8g2.firstPage();
		do {
			_drawMenuTitle("M1.1.1");
			_drawMenuItemNum(pot, *pot, 6);
			u8g2.setFont(u8g2_font_ncenB08_tr);
			u8g2.drawStr(10, 50, "AirSpeed:");
			u8g2.drawStr(80, 50, convert_number(*pot));
			_drawMenuItemNum(pot, *pot, 6, false);
		} while (u8g2.nextPage());
		
		break;

	case 1:
		once(pot, airChannel);
		u8g2.firstPage();
		do {
			_drawMenuTitle("M1.1.2");
			_drawMenuItemNum(pot, *pot, 32);
			u8g2.setFont(u8g2_font_ncenB08_tr);
			u8g2.drawStr(10, 50, "AirChannel:");
			u8g2.drawStr(90, 50, convert_number(*pot));
			_drawMenuItemNum(pot, *pot, 32, false);
		} while (u8g2.nextPage());
		
		break;

	case 2:
		once(pot, airSendPower);
		u8g2.firstPage();
		do {
			_drawMenuTitle("M1.1.3");
			_drawMenuItemNum(pot, *pot, 4);
			u8g2.setFont(u8g2_font_ncenB08_tr);
			u8g2.drawStr(10, 50, "SendPower:");
			u8g2.drawStr(90, 50, convert_number(*pot));
			_drawMenuItemNum(pot, *pot, 4, false);
		} while (u8g2.nextPage());
		
		break;

	default:
		break;
	}
}

void menu_save(int* pot)
{
	switch (item_2)
	{
	case 0:
		DEBUG_PRINT("AirSpeed = ");
		DEBUG_PRINTLN(*pot);
		setAirSpeed((Enum_AirSpeed)*pot);
		break;

	case 1:
		DEBUG_PRINT("SetAirChannel = ");
		DEBUG_PRINTLN(*pot);
		setAirChannel(*pot);
		break;

	case 2:
		DEBUG_PRINT("SetSendPower = ");
		DEBUG_PRINTLN(*pot);
		setSendPower((Enum_dBm)*pot);
		break;

	default:
		break;
	}

	for (int i = 0; i < 3; i++)
	{
		readWirelessConfig();
		delay(200);
	}
	delay(1000);
	clearBuf();

	airSpeed = getAirSpeed();
	airChannel = getAirChannel();
	airSendPower = getSendPower();

	_oneTime = true;
	*pot = 0;
	Menu.status = STATUS_M2;
	item_2 = menu_2(item_1, pot);
}

void menu_status(unsigned char flag)
{
	switch (flag)
	{
	case 0:
		if (Menu.status == STATUS_M0)
		{
			Menu.status = STATUS_M1;
		}
		else if (Menu.status == STATUS_M1)
		{
			Menu.status = STATUS_M2;
		}
		else if (Menu.status == STATUS_M2)
		{
			Menu.status = STATUS_M3;
		}
		else if (Menu.status == STATUS_M3)
		{
			Menu.status = STATUS_M_1;
		}
		break;

	case 1:
		if (Menu.status == STATUS_M3)
		{
			Menu.status = STATUS_M2;
		}
		else if (Menu.status == STATUS_M2)
		{
			Menu.status = STATUS_M1;
		}
		else if (Menu.status == STATUS_M1)
		{
			Menu.status = STATUS_M0;
		}
		break;

	default:
		break;
	}

}


void menu_loop(int* pot)
{
	switch (Menu.status)
	{
	case STATUS_M0:
		menu_0();
		break;

	case STATUS_M1:
		item_1 = menu_1(pot);
		break;

	case STATUS_M2:
		item_2 = menu_2(item_1, pot);
		_oneTime = true;
		break;

	case STATUS_M3:
		enum_3(item_2, pot);
		break;

	case STATUS_M_1:
		menu_save(pot);
		break;

	default:
		break;
	}
}