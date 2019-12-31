#pragma once
#ifndef __M_MENU_H__
#define __M_MENU_H__
#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>
#include "m_debug.h"
#include <U8g2lib.h>
#include "m_serialWireless.h"


#define SCL 5
#define SDA 6

typedef struct
{
	int status;

} _Menu;

enum Enum_Menu_Status
{
	STATUS_M_1 = 0,
	STATUS_M0 = 1,
	STATUS_M1 = 2,
	STATUS_M2 = 3,
	STATUS_M3 = 4
};


enum Enum_Button
{
	BUTTON_NULL,
	BUTTON_OK,
	BUTTON_BACK,
	BUTTON_UP,
	BUTTON_DOWN
};


void menuInit();
void menu_test();


void menu_status(unsigned char flag);
void menu_0();
void menu_loop(int* pot);


#endif // !__M_MENU_H__

