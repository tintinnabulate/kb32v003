#pragma once
#include "ch32v003fun.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

static void led_blink()
{
	funDigitalWrite(PC0, FUN_HIGH);
	Delay_Ms(25);
	funDigitalWrite(PC0, FUN_LOW);
	Delay_Ms(25);
}

static uint8_t convert_ascii_to_keycode(uint8_t ascii)
{
	if (ascii >= '0' && ascii <= '9')
	{
		// Full formula:  0x01 + (0x04 + ('z' - 'a')) + (ascii - '0');
		// 0x1E is '0'
		return (0x1E - 0x01) + (ascii - '0');
	}
	return 0x04 + (ascii - 'a');
}