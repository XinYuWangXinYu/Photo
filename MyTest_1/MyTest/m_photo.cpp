#include "m_photo.h"

unsigned int localDelay = 100;
unsigned int wirelessDelay = 120;

void photoInit()
{
	pinMode(TAKE_PHOTO_PHONE_PIN, INPUT);
	pinMode(TAKE_PHOTO_KEY_PIN, INPUT_PULLUP);
	DEBUG_PRINTLN("take photo key pin init [ok]");
	DEBUG_PRINTLN("photo pin init [ok]");
}


void takePhoto()
{
	pinMode(TAKE_PHOTO_PHONE_PIN, OUTPUT);
	digitalWrite(TAKE_PHOTO_PHONE_PIN, LOW);
	delay(localDelay);
	pinMode(TAKE_PHOTO_PHONE_PIN, INPUT);
	DEBUG_PRINTLN("take photo .......");
}


void keyScan()
{
	if (digitalRead(TAKE_PHOTO_KEY_PIN) == LOW)
	{
		delay(100);
		if (digitalRead(TAKE_PHOTO_KEY_PIN) == LOW)
		{
			wirelessSend("a1");
			delay(wirelessDelay); //120
			takePhoto();
			DEBUG_PRINTLN("keyScan");
		}
	}
}
