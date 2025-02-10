#include "misc.h"
#include "rv003usb.h"
#define LED_COLOR(r, g, b) b | (r << 8) | (g << 16);
#define KEY_DOWN '6'
#define KEY_UP '9'

void key_setup()
{
	/*A (labeled as 'UP', color purple) is PA2 (A1)
	B (labeled as 'DOWN', color blue) is PA1 (A0)*/
    funGpioInitAll();
	funPinMode(PC0, FUN_OUTPUT);
	funPinMode(PA1, FUN_INPUT);
	funPinMode(PA2, FUN_INPUT);
}

int main()
{
	SystemInit();
	key_setup();
	Delay_Us(3); // Ensures USB re-enumeration after bootloader or reset; Spec demand >2.5µs ( TDDIS )
	usb_setup();
	led_blink();
	while (1)
	{
#if RV003USB_EVENT_DEBUGGING
		uint32_t* ue = GetUEvent();
		if (ue)
		{
			printf("%lu %lx %lx %lx\n", ue[0], ue[1], ue[2], ue[3]);
		}
#endif
	}
}

void usb_handle_user_in_request(struct usb_endpoint* e, uint8_t* scratchpad, int endp, uint32_t sendtok, struct rv003usb_internal* ist)
{
	if (endp)
	{
		// Send key
		uint8_t data[8] = { 0x00 };	
		if (funDigitalRead(PA2) == FUN_LOW)
		{
			data[2] = convert_ascii_to_keycode(KEY_UP);
		}
		if (funDigitalRead(PA1) == FUN_LOW)
		{
			data[3] = convert_ascii_to_keycode(KEY_DOWN);
		}
		usb_send_data(data, 8, 0, sendtok);
	}
	else
	{
		usb_send_empty(sendtok);
	}
}
