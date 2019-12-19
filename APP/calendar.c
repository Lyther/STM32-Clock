#include "calendar.h"
#include "stdio.h"
#include "settings.h"
#include "ds18b20.h"
#include "24cxx.h"
#include "math.h"
#include "rtc.h"
#include "led.h"
#include "key.h"

u8 is_alarm_set = 0;
u8 is_timer_set = 0;
_alarm_obj alarm;
_calendar_obj calendar;
_timer_obj timer;

static u16 TIME_TOPY;
static u16 OTHER_TOPY;

u8*const calendar_week_table[GUI_LANGUAGE_NUM][7]=
{
{"星期天","星期一","星期二","星期三","星期四","星期五","星期六"},
{"星期天","星期一","星期二","星期三","星期四","星期五","星期六"},
{"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"}
};

u8*const calendar_alarm_caption_table[GUI_LANGUAGE_NUM]=
{
"闹钟","鬧鐘","ALARM"
};

u8*const calendar_loading_str[GUI_LANGUAGE_NUM][3]=
{
{
	"欢迎使用电子时钟",
	"闹钟已设定！",
	"正在读取当前时间..."
},
{
	"歡迎使用電子時鐘",
	"鬧鐘已設定！",
	"正在讀取當前時間..."
},
{
	"Welcome to STM32 Clock",
	"Alarm is already set!",
	"Getting RTC Time..."
}
};

void calendar_alarm_init(_alarm_obj *alarmx,_calendar_obj *calendarx) {
	u8 temp;
	RTC_Get();
	if (calendarx->week == 7) temp = 1;
	else temp = 1 << calendarx->week;
	if (alarmx->weekmask & temp) {
		printf("alarm:%d-%d-%d %d:%d\r\n", calendarx->w_year, calendarx->w_month, calendarx->w_date, alarmx->hour, alarmx->min);
		RTC_Alarm_Set(calendarx->w_year, calendarx->w_month, calendarx->w_date, alarmx->hour, alarmx->min, 0);
		is_alarm_set = 1;
	} else is_alarm_set = 0;
}

void calendar_alarm_ring(u8 type) {
	u8 i;
	for (i = 0; i < type + 1; i++) {
		LED1 = 0;
		delay_ms(100);
		LED1 = 1;
		delay_ms(100);
	}
}

void calendar_timer_init(_alarm_obj *alarmx, _timer_obj *timerx) {
	timerx->ringsta = alarmx->ringsta & ~(1 << 7);
	is_timer_set = 1;
}

void calendar_date_refresh(void) {
 	u8 weekn;
	u16 offx=(lcddev.width-240)/2;
	// Display date
	POINT_COLOR=GRED;
	BACK_COLOR=BLACK;
	LCD_ShowxNum(offx+5,OTHER_TOPY+9,(calendar.w_year/100)%100,2,16,0);
	LCD_ShowxNum(offx+21,OTHER_TOPY+9,calendar.w_year%100,2,16,0);
	LCD_ShowString(offx+37,OTHER_TOPY+9,lcddev.width,lcddev.height,16,"-");
	LCD_ShowxNum(offx+45,OTHER_TOPY+9,calendar.w_month,2,16,0X80);
	LCD_ShowString(offx+61,OTHER_TOPY+9,lcddev.width,lcddev.height,16,"-");
	LCD_ShowxNum(offx+69,OTHER_TOPY+9,calendar.w_date,2,16,0X80);
	// Display week
	POINT_COLOR=MAGENTA;
  weekn=calendar.week;
	Show_Str(5+offx,OTHER_TOPY+35,lcddev.width,lcddev.height,(u8 *)calendar_week_table[gui_phy.language][weekn],16,0);
}

void calendar_read_para(_alarm_obj * alarm) {
	AT24CXX_Read(SYSTEM_PARA_SAVE_BASE+sizeof(_system_setings),(u8*)alarm,sizeof(_alarm_obj));
}

void calendar_save_para(_alarm_obj * alarm) {
  OS_CPU_SR cpu_sr=0;
	alarm->ringsta&=0X7F;
	OS_ENTER_CRITICAL();
	AT24CXX_Write(SYSTEM_PARA_SAVE_BASE+sizeof(_system_setings),(u8*)alarm,sizeof(_alarm_obj));
	OS_EXIT_CRITICAL();
}

void timer_read_para(_timer_obj* timer) {
	AT24CXX_Read(SYSTEM_PARA_SAVE_BASE+sizeof(_system_setings)+sizeof(_alarm_obj),(u8*)timer,sizeof(_timer_obj));
}

void timer_save_para(_timer_obj* timer) {
	OS_CPU_SR cpu_sr = 0;
	timer->ringsta &= 0X7F;
	OS_ENTER_CRITICAL();
	AT24CXX_Write(SYSTEM_PARA_SAVE_BASE+sizeof(_system_setings)+sizeof(_alarm_obj),(u8*)timer,sizeof(_timer_obj));
	OS_EXIT_CRITICAL();
}

u8 calendar_alarm_msg(u16 x,u16 y) {
	u8 rval = 0;
  u16 *lcdbuf = 0;
  lcdbuf = (u16*) gui_memin_malloc(44*20*2);
 	if (lcdbuf) {
		app_read_bkcolor(x, y, 44, 20, lcdbuf);
		gui_fill_rectangle(x, y, 44, 20, WHITE);
		gui_draw_rectangle(x, y, 44, 20, BLUE);
		gui_show_num(x+2, y+2, 2, RED, 16, calendar.hour, 0X81);
 		gui_show_ptchar(x+2+16, y+2, x+2+16+8, y+2+16, 0, RED, 16, ':', 1);
		gui_show_num(x+2+24, y+2, 2, RED, 16, calendar.min, 0X81);
  	OSTaskSuspend(6);
		while (rval == 0) {
			tp_dev.scan(0);
			if (tp_dev.sta & TP_PRES_DOWN) {
				if (app_tp_is_in_area(&tp_dev, x, y, 44, 20)) {
					rval = 0XFF;
					break;
				}
			 }
			delay_ms(5);
    	if (system_task_return) break;
		}
		app_recover_bkcolor(x, y, 44, 20, lcdbuf);
	} else rval = 1;
	system_task_return = 0;
	alarm.ringsta &= ~(1<<7);
	timer.ringsta &= ~(1<<7);
	calendar_alarm_init((_alarm_obj*) &alarm, &calendar);
	gui_memin_free(lcdbuf);
	OSTaskResume(6);
	system_task_return = 0;
	return rval;
}

void calendar_circle_clock_drawpanel(u16 x,u16 y,u16 size,u16 d)
{
	u16 r=size/2;
	u16 sx=x-r;
	u16 sy=y-r;
	u16 px0,px1;
	u16 py0,py1;
	u16 i;
	gui_fill_circle(x,y,r,YELLOW);
	gui_fill_circle(x,y,r-4,BLACK);
	for(i=0;i<60;i++)
	{
		px0=sx+r+(r-4)*sin((app_pi/30)*i);
		py0=sy+r-(r-4)*cos((app_pi/30)*i);
		px1=sx+r+(r-d)*sin((app_pi/30)*i);
		py1=sy+r-(r-d)*cos((app_pi/30)*i);
		gui_draw_bline1(px0,py0,px1,py1,0,WHITE);
	}
	for(i=0;i<12;i++)
	{
		px0=sx+r+(r-5)*sin((app_pi/6)*i);
		py0=sy+r-(r-5)*cos((app_pi/6)*i);
		px1=sx+r+(r-d)*sin((app_pi/6)*i);
		py1=sy+r-(r-d)*cos((app_pi/6)*i);
		gui_draw_bline1(px0,py0,px1,py1,2,RED);
	}
	for(i=0;i<4;i++)
	{
		px0=sx+r+(r-5)*sin((app_pi/2)*i);
		py0=sy+r-(r-5)*cos((app_pi/2)*i);
		px1=sx+r+(r-d-3)*sin((app_pi/2)*i);
		py1=sy+r-(r-d-3)*cos((app_pi/2)*i);
		gui_draw_bline1(px0,py0,px1,py1,2,RED);
	}
	gui_fill_circle(x,y,d/2,WHITE);
}

void calendar_circle_clock_showtime(u16 x,u16 y,u16 size,u16 d,u8 hour,u8 min,u8 sec)
{
	static u8 oldhour=0;
	static u8 oldmin=0;
	static u8 oldsec=0;
	float temp;
	u16 r=size/2;
	u16 sx=x-r;
	u16 sy=y-r;
	u16 px0,px1;
	u16 py0,py1;
	u8 r1;
	if(hour>11)hour-=12;

	r1=d/2+4;
	temp=(float)oldmin/60;
	temp+=oldhour;
	px0=sx+r+(r-3*d-7)*sin((app_pi/6)*temp);
	py0=sy+r-(r-3*d-7)*cos((app_pi/6)*temp);
	px1=sx+r+r1*sin((app_pi/6)*temp);
	py1=sy+r-r1*cos((app_pi/6)*temp);
	gui_draw_bline1(px0,py0,px1,py1,2,BLACK);

	r1=d/2+3;
	temp=(float)oldsec/60;
	temp+=oldmin;

	px0=sx+r+(r-2*d-7)*sin((app_pi/30)*temp);
	py0=sy+r-(r-2*d-7)*cos((app_pi/30)*temp);
	px1=sx+r+r1*sin((app_pi/30)*temp);
	py1=sy+r-r1*cos((app_pi/30)*temp);
	gui_draw_bline1(px0,py0,px1,py1,1,BLACK);

	r1=d/2+3;

	px0=sx+r+(r-d-7)*sin((app_pi/30)*oldsec);
	py0=sy+r-(r-d-7)*cos((app_pi/30)*oldsec);
	px1=sx+r+r1*sin((app_pi/30)*oldsec);
	py1=sy+r-r1*cos((app_pi/30)*oldsec);
	gui_draw_bline1(px0,py0,px1,py1,0,BLACK);


	r1=d/2+4;

	temp=(float)min/60;
	temp+=hour;
	px0=sx+r+(r-3*d-7)*sin((app_pi/6)*temp);
	py0=sy+r-(r-3*d-7)*cos((app_pi/6)*temp);
	px1=sx+r+r1*sin((app_pi/6)*temp);
	py1=sy+r-r1*cos((app_pi/6)*temp);
	gui_draw_bline1(px0,py0,px1,py1,2,RED);

	r1=d/2+3;
	temp=(float)sec/60;
	temp+=min;

	px0=sx+r+(r-2*d-7)*sin((app_pi/30)*temp);
	py0=sy+r-(r-2*d-7)*cos((app_pi/30)*temp);
	px1=sx+r+r1*sin((app_pi/30)*temp);
	py1=sy+r-r1*cos((app_pi/30)*temp);
	gui_draw_bline1(px0,py0,px1,py1,1,YELLOW);

	r1=d/2+3;

	px0=sx+r+(r-d-7)*sin((app_pi/30)*sec);
	py0=sy+r-(r-d-7)*cos((app_pi/30)*sec);
	px1=sx+r+r1*sin((app_pi/30)*sec);
	py1=sy+r-r1*cos((app_pi/30)*sec);
	gui_draw_bline1(px0,py0,px1,py1,0,WHITE);
	oldhour=hour;
	oldmin=min;
	oldsec=sec;
}

u8 calendar_play(void)
{
	u8 second=0;
	u8 tempdate=0;
	u8 rval=0;
	u8 res;
	u8 key;
	u16 xoff=0;
	u16 yoff=0;
	u16 r=0;
	u8 d=0;
	FIL* f_calendar=0;

  f_calendar=(FIL *)gui_memin_malloc(sizeof(FIL));
	if(f_calendar==NULL)rval=1;
	else
	{
		res=f_open(f_calendar,(const TCHAR*)APP_ASCII_S6030,FA_READ);
		if(res==FR_OK)
		{
			asc2_s6030=(u8*)gui_memex_malloc(f_calendar->fsize);
			if(asc2_s6030==0)rval=1;
			else
			{
				res=f_read(f_calendar,asc2_s6030,f_calendar->fsize,(UINT*)&br);
 			}
			f_close(f_calendar);
		}
		if(res)rval=res;
	}
	if(rval==0)
	{
 		LCD_Clear(BLACK);
		second=calendar.sec;
		POINT_COLOR=WHITE;
		Show_Str(48,60,lcddev.width,lcddev.height,(u8*)calendar_loading_str[gui_phy.language][0],16,0x01);
		if(DS18B20_Init())
		{
			Show_Str(48,76,lcddev.width,lcddev.height,(u8*)calendar_loading_str[gui_phy.language][1],16,0x01);
			delay_ms(500);
			Show_Str(48,92,lcddev.width,lcddev.height,(u8*)calendar_loading_str[gui_phy.language][2],16,0x01);
		}
		delay_ms(1100);
		BACK_COLOR= BLACK;
		LCD_Clear(BLACK);
		if(lcddev.width==240)
		{
			r=80;
			d=7;
		}else if(lcddev.width==320)
		{
			r=120;
			d=9;
		}else if(lcddev.width==480)
		{
			r=160;
			d=12;
		}
		yoff=(lcddev.height-r*2-140)/2;
		TIME_TOPY=yoff+r*2+10;
		OTHER_TOPY=TIME_TOPY+60+10;
		xoff=(lcddev.width-240)/2;
		calendar_circle_clock_drawpanel(lcddev.width/2,yoff+r,r*2,d);
		calendar_date_refresh();
		tempdate=calendar.w_date;
		gui_phy.back_color=BLACK;
		gui_show_ptchar(xoff+70-4,TIME_TOPY,lcddev.width,lcddev.height,0,GREEN,60,':',0);
		gui_show_ptchar(xoff+150-4,TIME_TOPY,lcddev.width,lcddev.height,0,GREEN,60,':',0);
	}
	KEY_Init();
	calendar_alarm_init(&alarm,&calendar);
  while(rval==0) {
		RTC_Get();
		key = KEY_Scan(0);
		if (key == KEY1_PRES) {
			setmode = 1;
			break;
		} else if (key == WKUP_PRES) system_task_return = 1;
		if(system_task_return) break;
 		if(second!=calendar.sec) {
  		second=calendar.sec;
			if (is_timer_set) { // Counting down timer
				timer.second--;
				if (timer.second == 0) {
					if (timer.minute > 0) {
						timer.minute--;
						timer.second = 59;
					} else {
						timer.ringsta |= 1 << 7;
						is_timer_set = 0;
					}
				}
			}

			calendar_circle_clock_showtime(lcddev.width/2,yoff+r,r*2,d,calendar.hour,calendar.min,calendar.sec);
			gui_phy.back_color=BLACK;
			gui_show_num(xoff+10,TIME_TOPY,2,GREEN,60,calendar.hour,0X80);
			gui_show_num(xoff+90,TIME_TOPY,2,GREEN,60,calendar.min,0X80);
			gui_show_num(xoff+170,TIME_TOPY,2,GREEN,60,calendar.sec,0X80);

			if (is_timer_set) POINT_COLOR = RED;
			else POINT_COLOR = GRAY;
			gui_show_num(xoff+90,OTHER_TOPY,2,POINT_COLOR,60,timer.minute,0X80);
			gui_show_ptchar(xoff+150,OTHER_TOPY,lcddev.width,lcddev.height,0,POINT_COLOR,60,':',0);
			gui_show_num(xoff+170,OTHER_TOPY,2,POINT_COLOR,60,timer.second,0X80);

			if (calendar.w_date != tempdate) {
				calendar_date_refresh();
				tempdate = calendar.w_date;
			}
			if (is_alarm_set && calendar.hour == alarm.hour && calendar.min == alarm.min && calendar.sec == 0) // Process alarm
				alarm.ringsta |= 1 << 7;
			if (is_timer_set && timer.minute == 0 && timer.second == 0) // Process timer
				timer.ringsta |= 1 << 7;
 		}
		delay_ms(10);
 	};
 	while(tp_dev.sta&TP_PRES_DOWN)tp_dev.scan(0);
 	gui_memex_free(asc2_s6030);
	asc2_s6030=0;
	gui_memin_free(f_calendar);
	POINT_COLOR=BLUE;
	BACK_COLOR=WHITE;
	return rval;
}
