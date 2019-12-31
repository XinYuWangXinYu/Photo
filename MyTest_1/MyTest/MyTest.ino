/*
 Name:		MyTest.ino
 Created:	2019/11/12 下午 04:46:20
 Author:	mc
*/

#include "m_debug.h"
#include <GroveEncoder.h>
#include "m_menu.h"
#include "m_serialWireless.h"
#include "m_photo.h"


int menu_num = 0;
int* p_menu_num = &menu_num;


/***************************************************/
int oldEncodeValue = 0, currentEncoedValue;
void c_EncodeValue(int newValue) {
	currentEncoedValue = newValue;
}

#define ENCODE_SW_PIN	4
void encodeKeyInit()
{
	pinMode(ENCODE_SW_PIN, INPUT_PULLUP);
	DEBUG_PRINTLN("encodeKey init [ok]");
}

Enum_Button getEncodeValue()
{
	int value = 0, t = 0;
	if (digitalRead(ENCODE_SW_PIN) == LOW)
	{
		while (digitalRead(ENCODE_SW_PIN) == LOW)
		{
			t++;
			delay(3);
			if (digitalRead(ENCODE_SW_PIN) == HIGH)
				break;
			if (t >= 500)
				return BUTTON_BACK;
		}
		return BUTTON_OK;
	}
		
	if (oldEncodeValue != currentEncoedValue)
	{
		if ((currentEncoedValue - oldEncodeValue) > 0)
			value = BUTTON_UP;
		else if ((currentEncoedValue - oldEncodeValue) < 0)
			value = BUTTON_DOWN;
		oldEncodeValue = currentEncoedValue;
	}
	return (Enum_Button) value;
}
/***************************************************/


void setup() {
#ifdef DEBUG
	Serial.begin(115200);
#endif // DEBUG

	DEBUG_PRINTLN("----------STEP----------");
	photoInit();
	GroveEncoder m_encodeKey(2, &c_EncodeValue);
	encodeKeyInit();
	serialWirelessInit();

	for (int i = 0; i < 3; i++)
	{
		readWirelessConfig();
		delay(200);
	}
	delay(1000);
	clearBuf();

	menuInit();
	menu_0();
	model_normal();
	model_normal();
	delay(500);
}

void loop() {
	
	int val = getEncodeValue();
	if (val != 0)
	{
		if (val == BUTTON_OK)
			menu_status(0);
		else if (val == BUTTON_UP)
			(*p_menu_num)++;
		else if (val == BUTTON_DOWN)
			(*p_menu_num)--;
		else if (val == BUTTON_BACK)
			menu_status(1);

		menu_loop(p_menu_num);

		/*DEBUG_PRINT("key = ");
		DEBUG_PRINTLN(val);*/
	}

	serialWirelessReceive();
	keyScan();
	//delay(1000);
}


