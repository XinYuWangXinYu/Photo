#pragma once
#ifndef __M_PHOTO_H__
#define __M_PHOTO_H__
#include <Arduino.h>
#include "m_debug.h"
#include "m_serialWireless.h"



// 连接耳机插孔
#define  TAKE_PHOTO_PHONE_PIN  11

// 拍照按键, 就是天使眼开关
#define  TAKE_PHOTO_KEY_PIN    12



void photoInit();
void takePhoto();
void keyScan();

#endif // !__PHOTO_H__



