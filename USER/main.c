#include "includes.h"
#include "common.h"
#include "calendar.h"
#include "settings.h"
#include "mass_mal.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "memory.h"
#include "usb_bot.h"

// Set priority for UCOS II tasks, 10 is the lowest
#define START_TASK_PRIO 10
// Set stack size and create task stack
#define START_STK_SIZE 64
__align(8) static OS_STK START_TASK_STK[START_STK_SIZE];
void start_task(void *pdata);

// Serial port settings
#define USART_TASK_PRIO 7
#define USART_STK_SIZE 64
__align(8) static OS_STK USART_TASK_STK[USART_STK_SIZE];
void usart_task(void *pdata);

// Main task settings
#define MAIN_TASK_PRIO 6
#define MAIN_STK_SIZE 256
__align(8) static OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];
void main_task(void *pdata);

// Serial listener settings
#define WATCH_TASK_PRIO 3
#define WATCH_STK_SIZE 128
__align(8) static OS_STK WATCH_TASK_STK[WATCH_STK_SIZE];
void watch_task(void *pdata);

extern u8 Max_Lun;
u8 wirelessmode = 0;
volatile u8 setmode = 0;
volatile u8 system_task_return = 0;

u8 start_with(const char* pre, const char* str) {
	u8 len_pre = strlen(pre);
	u8 len_str = strlen(str);
	return len_str < len_pre ? 0 : memcmp(pre, str, len_pre) == 0;
}

int str_to_int(const char* str) {
	int result;
	sscanf(str, "%d", &result);
	return result;
}

void set_calendar(const char* time_t) {
	char year[5], month[3], day[3], hour[3], minute[3], second[3];
	int y, m, d, h, min, s;
	memcpy(year, &time_t[0], 4);
	year[4] = '\0';
	memcpy(month, &time_t[4], 2);
	month[2] = '\0';
	memcpy(day, &time_t[6], 2);
	day[2] = '\0';
	memcpy(hour, &time_t[8], 2);
	hour[2] = '\0';
	memcpy(minute, &time_t[10], 2);
	minute[2] = '\0';
	memcpy(second, &time_t[12], 2);
	second[2] = '\0';
	y = str_to_int(year);
	m = str_to_int(month);
	d = str_to_int(day);
	h = str_to_int(hour);
	min = str_to_int(minute);
	s = str_to_int(second);
	RTC_Set(y,m,d,h,min,s);
}

void set_time(const char* str) {
	char buf[31] = {0};
	if (start_with("hour", str)) {
		memcpy(buf, &str[5], 2);
		RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date,str_to_int(buf),calendar.min,calendar.sec);
	} else if (start_with("minute", str)) {
		memcpy(buf, &str[7], 2);
		RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,str_to_int(buf),calendar.sec);
	} else if (start_with("second", str)) {
		memcpy(buf, &str[7], 2);
		RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,str_to_int(buf));
	} else if (start_with("year", str)) {
		memcpy(buf, &str[5], 4);
		RTC_Set(str_to_int(buf),calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);
	} else if (start_with("month", str)) {
		memcpy(buf, &str[6], 2);
		RTC_Set(calendar.w_year,str_to_int(buf),calendar.w_date,calendar.hour,calendar.min,calendar.sec);
	} else if (start_with("day", str)) {
		memcpy(buf, &str[4], 2);
		RTC_Set(calendar.w_year,calendar.w_month,str_to_int(buf),calendar.hour,calendar.min,calendar.sec);
	} else {
		set_calendar(str);
	}
	delay_ms(10);
	printf("Calendar time set to %d-%d-%d %d:%d:%d\r\n", calendar.w_year, calendar.w_month, calendar.w_date, calendar.hour, calendar.min, calendar.sec);
}

// Print error message
void system_error_show(u16 x, u16 y, u8 *err, u8 fsize) {
	POINT_COLOR = RED;
	while (1) {
		LCD_ShowString(x, y, lcddev.width, lcddev.height, fsize, err);
		delay_ms(400);
		LCD_Fill(x, y, lcddev.width, y + fsize, BLACK);
		delay_ms(100);
		LED0 = !LED0;
	}
}

// Delete all system files
u8 system_files_erase(u16 x, u16 y, u8 fsize) {
	u8 key;
	u8 t = 0;
	POINT_COLOR = RED;
	LCD_ShowString(x, y, lcddev.width, lcddev.height, fsize, "Erase all system files?");
	while (1) {
		t++;
		if (t == 20)
			LCD_ShowString(x, y + fsize, lcddev.width, lcddev.height, fsize, "KEY0:NO / KEY1:YES");
		if (t == 40) {
			gui_fill_rectangle(x, y + fsize, lcddev.width, fsize, BLACK);
			t = 0;
			LED0 = !LED0;
		}
		key = KEY_Scan(0);
		if (key == KEY0_PRES) { // Cancel erasing files
			gui_fill_rectangle(x, y, lcddev.width, fsize * 2, BLACK);
			POINT_COLOR = WHITE;
			LED0 = 1;
			return 0;
		}
		if (key == KEY1_PRES) { // Erase restart
			LED0 = 1;
			LCD_ShowString(x, y + fsize, lcddev.width, lcddev.height, fsize, "Erasing SPI FLASH...");
			SPI_Flash_Erase_Chip();
			LCD_ShowString(x, y + fsize, lcddev.width, lcddev.height, fsize, "Erasing SPI FLASH OK");
			delay_ms(600);
			return 1;
		}
		delay_ms(10);
	}
}

// Confirm whether to update font
u8 system_font_update_confirm(u16 x, u16 y, u8 fsize) {
	u8 key;
	u8 t = 0;
	u8 res = 0;
	POINT_COLOR = RED;
	LCD_ShowString(x, y, lcddev.width, lcddev.height, fsize, "Update font?");
	while (1) {
		t++;
		if (t == 20)
			LCD_ShowString(x, y + fsize, lcddev.width, lcddev.height, fsize, "KEY0:YES / KEY1:NO");
		if (t == 40) {
			gui_fill_rectangle(x, y + fsize, lcddev.width, fsize, BLACK);
			t = 0;
			LED0 = !LED0;
		}
		key = KEY_Scan(0);
		if (key == KEY1_PRES) // Cancel
			break;
		if (key == KEY0_PRES) { // Update
			res = 1;
			break;
		}
		delay_ms(10);
	}
	LED0 = 1;
	gui_fill_rectangle(x, y, lcddev.width, fsize * 2, BLACK);
	POINT_COLOR = WHITE;
	return res;
}

// Update system file using USB confirmation
u8 system_usb_update_confirm(u16 x, u16 y, u8 fsize)
{
	u8 key;
	u8 t = 0;
	u8 res = 0;
	u8 offline_cnt = 0;
	u8 Divece_STA = 0;
	u8 USB_STA = 0;
	POINT_COLOR = RED;
	LCD_ShowString(x, y, lcddev.width, lcddev.height, fsize, "Use USB Update SPI FLASH Files?");
	while (1) {
		t++;
		if (t == 20)
			LCD_ShowString(x, y + fsize, lcddev.width, lcddev.height, fsize, "KEY0:NO / KEY1:YES");
		if (t == 40) {
			gui_fill_rectangle(x, y + fsize, lcddev.width, fsize, BLACK);
			t = 0;
			LED0 = !LED0;
		}
		delay_ms(10);
		key = KEY_Scan(0);
		if (key == KEY0_PRES) // Cancel
			break;
		if (key == KEY1_PRES) { // Update
			res = 1;
			break;
		}
	}
	gui_fill_rectangle(x, y, lcddev.width, fsize * 2, BLACK);
	if (res) {
		LCD_ShowString(x, y, lcddev.width, lcddev.height, fsize, "Erasing SPI FLASH...");
		SPI_Flash_Erase_Chip();
		LCD_ShowString(x, y, lcddev.width, lcddev.height, fsize, "Formating SPI FLASH...");
		res = f_mkfs("1:", 1, 4096);
		if (res == 0)
			f_setlabel((const TCHAR *)"1:ALIENTEK");
		LCD_ShowString(x, y, lcddev.width, lcddev.height, fsize, "Please push USB cable to USB_SLAVE");
		Data_Buffer = (u32 *)gui_memin_malloc(BULK_MAX_PACKET_SIZE * 2 * 4);
		Bulk_Data_Buff = gui_memin_malloc(BULK_MAX_PACKET_SIZE);
		
		USB_Port_Set(0);
		delay_ms(700);
		USB_Port_Set(1);
		Max_Lun = 0;
		Mass_Memory_Size[0] = 9832 * 512;
		Mass_Block_Size[0] = 512;
		Mass_Block_Count[0] = Mass_Memory_Size[0] / Mass_Block_Size[0];
		USB_Interrupts_Config();
		Set_USBClock();
		USB_Init();
		t = 0;
		while (1) {
			t++;
			delay_ms(5);
			if (t == 40) {
				t = 0;
				if (USB_STATUS_REG & 0x10) {
					offline_cnt = 0;
					bDeviceState = CONFIGURED;
				} else {
					offline_cnt++;
					if (offline_cnt > 10)
						bDeviceState = UNCONNECTED;
				}
				USB_STATUS_REG = 0;
				LED0 = !LED0;
			}
			if (USB_STA != USB_STATUS_REG) {
				gui_fill_rectangle(x, y + fsize * 4, lcddev.width, fsize, BLACK);
				if (USB_STATUS_REG & 0x01) {
					LCD_ShowString(x, y + fsize * 4, lcddev.width, lcddev.height, fsize, "USB Writing...");
					USB_STATUS_REG &= 0xFD;
				}
				USB_STA = USB_STATUS_REG;
			}
			if (Divece_STA != bDeviceState) {
				if (bDeviceState == CONFIGURED) {
					LCD_ShowString(x, y + fsize, lcddev.width, lcddev.height, fsize, "USB Connected   ");
					LCD_ShowString(x, y + fsize * 2, lcddev.width, lcddev.height, fsize, "Copy SYSTEM folder to ALIENTEK Disk");
					LCD_ShowString(x, y + fsize * 3, lcddev.width, lcddev.height, fsize, "When File Copy finished Press KEY0!");
				} else {
					LCD_ShowString(x, y + fsize, lcddev.width, lcddev.height, fsize, "USB DisConnected ");
					gui_fill_rectangle(x, y + fsize * 2, lcddev.width, fsize * 3, BLACK);
				}
				Divece_STA = bDeviceState;
			}
			if (bDeviceState == CONFIGURED) {
				key = KEY_Scan(0);
				if (key == KEY0_PRES) {
					res = 1;
					break;
				}
			}
		}
		USB_Port_Set(0);
		gui_memin_free(Data_Buffer);
		gui_memin_free(Bulk_Data_Buff);
	}
	LED0 = 1;
	gui_fill_rectangle(x, y, lcddev.width, fsize * 2, BLACK);
	POINT_COLOR = WHITE;
	return res;
}

// Initalize the operating system
void system_init(void) {
	u16 okoffset = 162;
	u16 ypos = 0;
	u16 j = 0;
	u16 temp = 0;
	u8 res;
	u32 dtsize, dfsize;
	u8 *stastr = 0;
	u8 *version = 0;
	u8 verbuf[12];
	u8 fsize;
	u8 icowidth;

	// Initialize system
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();
	uart_init(9600);
	LCD_Init();
	LED_Init();
	KEY_Init();
	gui_init();
	Adc_Init();
	AT24CXX_Init();
	SPI_Flash_Init();
	mem_init();
	version = mymalloc(31);
	exfuns_init();
REINIT:
	LCD_Clear(BLACK);
	POINT_COLOR = WHITE;
	BACK_COLOR = BLACK;
	j = 0;

	// Print copyright information
	ypos = 2;
	ypos = 2;
	if (lcddev.width == 240) {
		fsize = 12;
		icowidth = 18;
		okoffset = 190;
		app_show_mono_icos(5, ypos, icowidth, 24, (u8 *)APP_ALIENTEK_ICO1824, YELLOW, BLACK);
	} else if (lcddev.width == 320) {
		fsize = 16;
		icowidth = 24;
		okoffset = 250;
		app_show_mono_icos(5, ypos, icowidth, 32, (u8 *)APP_ALIENTEK_ICO2432, YELLOW, BLACK);
	} else if (lcddev.width == 480) {
		fsize = 24;
		icowidth = 36;
		okoffset = 370;
		app_show_mono_icos(5, ypos, icowidth, 48, (u8 *)APP_ALIENTEK_ICO3648, YELLOW, BLACK);
	}
	LCD_ShowString(icowidth + 5 * 2, ypos + fsize * j++, lcddev.width, lcddev.height, fsize, "ALIENTEK MiniSTM32F1 V3");
	LCD_ShowString(icowidth + 5 * 2, ypos + fsize * j++, lcddev.width, lcddev.height, fsize, "Copyright (C) 2015-2025");
	app_get_version(verbuf, HARDWARE_VERSION, 2);
	strcpy((char *)version, "HARDWARE:");
	strcat((char *)version, (const char *)verbuf);
	strcat((char *)version, ", SOFTWARE:");
	app_get_version(verbuf, SOFTWARE_VERSION, 3);
	strcat((char *)version, (const char *)verbuf);
	LCD_ShowString(5, ypos + fsize * j++, lcddev.width, lcddev.height, fsize, version);
	sprintf((char *)verbuf, "LCD ID:%04X", lcddev.id);
	LCD_ShowString(5, ypos + fsize * j++, lcddev.width, lcddev.height, fsize, verbuf);

	// Start hardware initialization
	LED0 = 0;
	LED1 = 0;
	LCD_ShowString(5, ypos + fsize * j++, lcddev.width, lcddev.height, fsize, "CPU:STM32F103RCT6 72Mhz");
	LCD_ShowString(5, ypos + fsize * j++, lcddev.width, lcddev.height, fsize, "FLASH:256KB  SRAM:48KB");
	LED0 = 1;
	LED1 = 1;
	if (SPI_Flash_ReadID() != W25Q64)
		system_error_show(5, ypos + fsize * j++, "Ex Flash Error!!", fsize);
	else
		temp = 8 * 1024;
	LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize, "Ex Flash:     KB");
	LCD_ShowxNum(5 + 9 * (fsize / 2), ypos + fsize * j, temp, 5, fsize, 0);
	LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width, lcddev.height, fsize, "OK");

	// SPI flash check
	res = KEY_Scan(1);
	if (res == WKUP_PRES) {
		res = system_files_erase(5, ypos + fsize * j, fsize);
		if (res)
			goto REINIT;
	}

	// SPI file system check
	LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize, "FATFS Check...");
	f_mount(fs[0], "0:", 1);
	f_mount(fs[1], "1:", 1);
	LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width, lcddev.height, fsize, "OK");

	// SD card check
	LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize, "SD Card:     MB");
	temp = 0;
	do {
		temp++;
		res = exf_getfree("0:", &dtsize, &dfsize);
		delay_ms(200);
	} while (res && temp < 5);
	if (res == 0) {
		gui_phy.memdevflag |= 1 << 0;
		temp = dtsize >> 10;
		stastr = "OK";
	} else {
		temp = 0;
		stastr = "ERROR";
	}
	LCD_ShowxNum(5 + 8 * (fsize / 2), ypos + fsize * j, temp, 5, fsize, 0);
	LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width, lcddev.height, fsize, stastr);
	temp = 0;
	do {
		temp++;
		res = exf_getfree("1:", &dtsize, &dfsize);
		delay_ms(200);
	} while (res && temp < 20);
	if (res == 0X0D) {
		LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize, "Flash Disk Formatting...");
		res = f_mkfs("1:", 1, 4096);
		if (res == 0) {
			f_setlabel((const TCHAR *)"1:ALIENTEK");
			LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width, lcddev.height, fsize, "OK");
			res = exf_getfree("1:", &dtsize, &dfsize);
		}
	}
	if (res == 0) {
		gui_phy.memdevflag |= 1 << 1;
		LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize, "Flash Disk:     KB");
		temp = dtsize;
	} else
		system_error_show(5, ypos + fsize * (j + 1), "Flash Fat Error!", fsize);
	LCD_ShowxNum(5 + 11 * (fsize / 2), ypos + fsize * j, temp, 5, fsize, 0);
	LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width, lcddev.height, fsize, "OK");

	// RTC check and initial
	LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize, "RTC Check...");
	if (RTC_Init())
		system_error_show(5, ypos + fsize * (j + 1), "RTC Error!", fsize);
	else {
		RTC_Get();
		LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width, lcddev.height, fsize, "OK");
	}

	// 24C02 check
	LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize, "24C02 Check...");
	if (AT24CXX_Check())
		system_error_show(5, ypos + fsize * (j + 1), "24C02 Error!", fsize);
	else
		LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width, lcddev.height, fsize, "OK");

	// Font check and initial
	LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize, "Font Check...");
	res = KEY_Scan(1); // If you want to update font, press key 1
	if (res == KEY1_PRES)
		res = system_font_update_confirm(5, ypos + fsize * (j + 1), fsize);
	else
		res = 0;
	while (font_init() || res == 1) {
		res = 0;
		if (update_font(5, ypos + fsize * j, fsize, "0:") != 0) {
			if (update_font(5, ypos + fsize * j, fsize, "1:") != 0) {
				res = system_usb_update_confirm(5, ypos + fsize * (j + 1), fsize);
				if (res)
					goto REINIT;
				system_error_show(5, ypos + fsize * (j + 1), "Font Error!", fsize);
			}
		}
		LCD_Fill(5, ypos + fsize * j, lcddev.width, ypos + fsize * (j + 1), BLACK);
		LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize, "Font Check...");
	}
	LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width, lcddev.height, fsize, "OK");

	// Touch screen check
	LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize, "Touch Check...");
	res = KEY_Scan(1);
	if (TP_Init() || res == 1) {
		if (res == 1)
			TP_Adjust();
		res = 0;
		goto REINIT;
	}
	LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width, lcddev.height, fsize, "OK");

	// Load system parameters
	LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize, "SYSTEM Parameter Load...");
	if (app_system_parameter_init())
		system_error_show(5, ypos + fsize * (j + 1), "Parameter Load Error!", fsize);
	else
		LCD_ShowString(5 + okoffset, ypos + fsize * j++, lcddev.width, lcddev.height, fsize, "OK");
	LCD_ShowString(5, ypos + fsize * j, lcddev.width, lcddev.height, fsize, "SYSTEM Starting...");
	delay_ms(1500);
	myfree(version);
}

int main(void) {
	system_init();
	OSInit();
	OSTaskCreate(start_task, (void *)0, (OS_STK *)&START_TASK_STK[START_STK_SIZE - 1], START_TASK_PRIO);
	OSStart();
}

void start_task(void *pdata) {
	OS_CPU_SR cpu_sr = 0;
	pdata = pdata;
	OSStatInit();
	app_srand(OSTime);
	gui_init();
	piclib_init();
	OS_ENTER_CRITICAL();
	OSTaskCreate(main_task, (void *)0, (OS_STK *)&MAIN_TASK_STK[MAIN_STK_SIZE - 1], MAIN_TASK_PRIO);
	OSTaskCreate(usart_task, (void *)0, (OS_STK *)&USART_TASK_STK[USART_STK_SIZE - 1], USART_TASK_PRIO);
	OSTaskCreate(watch_task, (void *)0, (OS_STK *)&WATCH_TASK_STK[WATCH_STK_SIZE - 1], WATCH_TASK_PRIO);
	OSTaskSuspend(START_TASK_PRIO);
	OS_EXIT_CRITICAL();
}

void main_task(void *pdata)
{
	u8 key;
	LCD_Clear(BLACK);
	while (1) {
		system_task_return = 0;
		if (setmode) {
			sysset_play();
			LCD_Clear(BLACK);
			setmode = 0;
		}
		key = KEY_Scan(0);
		switch (key) {
		case KEY1_PRES:
			calendar_play();
			LCD_Clear(BLACK);
			break;
		case KEY0_PRES:
			sysset_play();
			LCD_Clear(BLACK);
			break;
		}
		delay_ms(10);
	}
}

void usart_task(void *pdata) {
	u8 len = 0;
	u16 alarmtimse = 0;
	pdata = pdata;
	while (1) {
		delay_ms(1000);
		if (USART_RX_STA & 0x8000) {
			len = USART_RX_STA & 0x3FFF;
			USART_RX_BUF[len] = '\0';
			printf("Receive command: %s\r\n", USART_RX_BUF);
			if (strcmp("alarm on", USART_RX_BUF) == 0) {
				alarm.ringsta |= 1 << 7;
				printf("Set alarm on\r\n");
			} else if (strcmp("alarm off", USART_RX_BUF) == 0) {
				alarm.ringsta &= ~(1 << 7);
				printf("Set alarm off\r\n");
			} else if (start_with("set", USART_RX_BUF)) {
				char buf[31];
				memcpy(buf, &USART_RX_BUF[4], 31);
				set_time(buf);
			} else if (strcmp("timer on", USART_RX_BUF) == 0) {
				timer.ringsta |= 1 << 7;
				printf("Set timer on\r\n");
			} else if (strcmp("timer off", USART_RX_BUF) == 0) {
				timer.ringsta &= ~(1 << 7);
				printf("Set timer off\r\n");
			}
		}
		USART_RX_STA = 0;
		if (alarm.ringsta & 1 << 7) {
			calendar_alarm_ring(alarm.ringsta & 0x3);
			alarmtimse++;
			if (alarmtimse > 30)
				alarm.ringsta &= ~(1 << 7);
		} else if (timer.ringsta & 1 << 7) {
			calendar_alarm_ring(timer.ringsta & 0x3);
			alarmtimse++;
			if (alarmtimse > 30)
				timer.ringsta &= ~(1 << 7);
		} else if (alarmtimse) {
			alarmtimse = 0;
			LED1 = 1;
		}
	}
}

void watch_task(void *pdata) {
	u8 t = 0;
	u8 key;
	pdata = pdata;
	KEY_Init();
	while (1) {
		if ((alarm.ringsta & 1 << 7) || (timer.ringsta & 1 << 7))
			calendar_alarm_msg((lcddev.width - 44) / 2, (lcddev.height - 20) / 2);
		if (t == 200) {
			LED0 = 0;
			t = 0;
		}
		if (t == 8)
			LED0 = 1;
		t++;
		key = KEY_Scan(0);
		if (key == WKUP_PRES)
			system_task_return = 1;
		delay_ms(10);
	}
}
