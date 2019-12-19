#ifndef __CALENDAR_H
#define __CALENDAR_H
#include "sys.h"
#include "includes.h"
#include "common.h"
#include "rtc.h"

__packed typedef struct
{
	u8  weekmask;
	u8  ringsta;
	u8  hour;
	u8  min;
	u8 saveflag;
}_alarm_obj;

__packed typedef struct {
	u8 minute;
	u8 second;
	u8 ringsta;
} _timer_obj;

extern _alarm_obj alarm;
extern _timer_obj timer;
extern u8*const calendar_week_table[GUI_LANGUAGE_NUM][7];

void calendar_alarm_init(_alarm_obj *alarmx,_calendar_obj *calendarx);
void calendar_timer_init(_alarm_obj *alarmx, _timer_obj *timerx);
void calendar_alarm_ring(u8 type);
void calendar_get_time(_calendar_obj *calendarx);
void calendar_get_date(_calendar_obj *calendarx);
void calendar_date_refresh(void);
void calendar_read_para(_alarm_obj * alarm);
void calendar_save_para(_alarm_obj * alarm);
void timer_read_para(_timer_obj* timer);
void timer_save_para(_timer_obj* timer);
u8 calendar_alarm_msg(u16 x,u16 y);
void calendar_circle_clock_drawpanel(u16 x,u16 y,u16 size,u16 d);
void calendar_circle_clock_showtime(u16 x,u16 y,u16 size,u16 d,u8 hour,u8 min,u8 sec);
u8 calendar_play(void);

#endif
