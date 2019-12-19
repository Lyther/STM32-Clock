#include "settings.h"
#include "rtc.h"
#include "calendar.h"

_system_setings systemset;
#define SYS_MENU_SIZE 7

u8*const sysset_mmenu_tbl[GUI_LANGUAGE_NUM][SYS_MENU_SIZE]=
{
	{
	"1.时间设置",
	"2.日期设置",
	"3.闹钟时间设置",
	"4.闹钟开关设置",
	"5.闹钟提示设置",
	"6.计时器设置",
	"7.语言设置"
	},
	{
	"1.時間設置",
	"2.日期設置",
	"3.鬧鐘時間設置",
	"4.鬧鐘開關設置",
	"5.鬧鐘提示設置",
	"6.計時器設置",
	"7.語言設置"
	},
	{
	"1.TIME SET",
	"2.DATE SET",
	"3.ALARM TIME SET",
	"4.ALARM ON/OFF SET",
	"5.ALARM RING SET",
	"6.TIMER SET",
	"7.LANGUAGE SET"
	}
};

u8*const sysset_language_tbl[GUI_LANGUAGE_NUM]=
{
	"简体中文","繁體中文","English"
};

u8*const sysset_ring_tbl[GUI_LANGUAGE_NUM][4]=
{
{"DS1闪烁1","DS1闪烁2","DS1闪烁3","DS1闪烁4"},
{"DS1閃爍1","DS1閃爍2","DS1閃爍3","DS1閃爍4"},
{"DS1 FLASH 1","DS1 FLASH 2","DS1 FLASH 3","DS1 FLASH 4"}
};

#define SYSSET_BTN1_WIDTH 50
#define SYSSET_BTN1_HEIGHT 40
#define SYSSET_BTN2_WIDTH 60
#define SYSSET_BTN2_HEIGHT 30
#define SYSSET_BTN3_WIDTH 100
#define SYSSET_EDIT_WIDTH 50
#define SYSSET_EDIT_HEIGHT 25

#define SYSSET_INWIN_FONT_COLOR		0X736C
#define SYSSET_EDIT_BACK_COLOR		0XE79E

u8 sysset_time_set(u16 x,u16 y,u8 *hour,u8 *min,u8*caption)
{
	u8 rval=0,res;
	u8 i;
	_window_obj* twin=0;
 	_btn_obj * tbtn[6];
 	twin=window_creat(x,y,150,199,0,1|1<<5,16);
 	tbtn[0]=btn_creat(x+20,y+42,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);
	tbtn[1]=btn_creat(x+20,y+42+67,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);
	tbtn[2]=btn_creat(x+20+60,y+42,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);
	tbtn[3]=btn_creat(x+20+60,y+42+67,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);
	tbtn[4]=btn_creat(x+10,y+42+117,SYSSET_BTN2_WIDTH,SYSSET_BTN2_HEIGHT,0,0x02);
	tbtn[5]=btn_creat(x+10+70,y+42+117,SYSSET_BTN2_WIDTH,SYSSET_BTN2_HEIGHT,0,0x02);
	for(i=0;i<6;i++)
	{
		if(tbtn[i]==NULL)
		{
			rval=1;
			break;
		}
		if(i<4)
		{
			tbtn[i]->bcfucolor=BLACK;
			tbtn[i]->bcfdcolor=WHITE;
			tbtn[i]->bkctbl[0]=0X453A;
			tbtn[i]->bkctbl[1]=0X5DDC;
			tbtn[i]->bkctbl[2]=0X5DDC;
			tbtn[i]->bkctbl[3]=0X453A;
		}else
		{
			tbtn[i]->bkctbl[0]=0X8452;
			tbtn[i]->bkctbl[1]=0XAD97;
			tbtn[i]->bkctbl[2]=0XAD97;
			tbtn[i]->bkctbl[3]=0X8452;
		}
		if(i==0||i==2)tbtn[i]->caption="＋";
		if(i==1||i==3)tbtn[i]->caption="－";
		if(i==4)tbtn[i]->caption=(u8*)GUI_OK_CAPTION_TBL[gui_phy.language];
		if(i==5)tbtn[i]->caption=(u8*)GUI_CANCEL_CAPTION_TBL[gui_phy.language];
	}
    if(twin==NULL)rval=1;
	else
	{
		twin->caption=caption;
		twin->windowbkc=APP_WIN_BACK_COLOR;
	}
	if(rval==0)
	{
		window_draw(twin);
		for(i=0;i<6;i++)btn_draw(tbtn[i]);
		gui_fill_rectangle(x+20+1,y+42+41,SYSSET_EDIT_WIDTH-2,SYSSET_EDIT_HEIGHT,SYSSET_EDIT_BACK_COLOR);
		gui_fill_rectangle(x+20+60+1,y+42+41,SYSSET_EDIT_WIDTH-2,SYSSET_EDIT_HEIGHT,SYSSET_EDIT_BACK_COLOR);
 		app_show_nummid(x+20,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*hour,2,16,BLACK,SYSSET_EDIT_BACK_COLOR);
		app_show_nummid(x+20+60,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*min,2,16,BLACK,SYSSET_EDIT_BACK_COLOR);
		while(rval==0)
		{
			tp_dev.scan(0);
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);
			delay_ms(1000/OS_TICKS_PER_SEC);
 			if(system_task_return)
			{
				rval=1;
				break;
			}
			for(i=0;i<6;i++)
			{
				res=btn_check(tbtn[i],&in_obj);
				if(res)
				{
					if((tbtn[i]->sta&0X80)==0)
					{
						switch(i)
						{
							case 0:
								(*hour)++;
								if(*hour>23)*hour=0;
								break;
							case 1:
								if(*hour)(*hour)--;
								else *hour=23;
								break;
							case 2:
								(*min)++;
								if(*min>59)(*min)=0;
								break;
							case 3:
								if(*min)(*min)--;
								else *min=59;
								break;
							case 4:
								rval=0XFF;
								break;
							case 5:
								rval=1;
								break;
 						}
					}
					app_show_nummid(x+20,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*hour,2,16,BLACK,SYSSET_EDIT_BACK_COLOR);
					app_show_nummid(x+20+60,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*min,2,16,BLACK,SYSSET_EDIT_BACK_COLOR);
				}
			}
		}
 	}
	window_delete(twin);
	for(i=0;i<6;i++)btn_delete(tbtn[i]);
	system_task_return=0;
	if(rval==0XFF)return 0;
	return rval;
}

u8 sysset_is_leap_year(u16 year)
{
	if(year%4==0)
	{
		if(year%100==0)
		{
			if(year%400==0)return 1;
			else return 0;
		}else return 1;
	}else return 0;
}

u8 sysset_date_set(u16 x,u16 y,u16 *year,u8 *month,u8 *date,u8*caption)
{
	u8 rval=0,res;
	u8 i;
	u8 maxdate=31;
	_window_obj* twin=0;
 	_btn_obj * tbtn[8];
 	twin=window_creat(x,y,190,199,0,1|1<<5,16);
 	tbtn[0]=btn_creat(x+10,y+42,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);
	tbtn[1]=btn_creat(x+10,y+42+67,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);
	tbtn[2]=btn_creat(x+10+60,y+42,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);
	tbtn[3]=btn_creat(x+10+60,y+42+67,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);
	tbtn[4]=btn_creat(x+10+120,y+42,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);
	tbtn[5]=btn_creat(x+10+120,y+42+67,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);

	tbtn[6]=btn_creat(x+20,y+42+117,SYSSET_BTN2_WIDTH,SYSSET_BTN2_HEIGHT,0,0x02);
	tbtn[7]=btn_creat(x+20+90,y+42+117,SYSSET_BTN2_WIDTH,SYSSET_BTN2_HEIGHT,0,0x02);
 	for(i=0;i<8;i++)
	{
		if(tbtn[i]==NULL)
		{
			rval=1;
			break;
		}
		if(i<6)
		{
			tbtn[i]->bcfucolor=BLACK;
			tbtn[i]->bcfdcolor=WHITE;
			tbtn[i]->bkctbl[0]=0X453A;
			tbtn[i]->bkctbl[1]=0X5DDC;
			tbtn[i]->bkctbl[2]=0X5DDC;
			tbtn[i]->bkctbl[3]=0X453A;
		}else
		{
			tbtn[i]->bkctbl[0]=0X8452;
			tbtn[i]->bkctbl[1]=0XAD97;
			tbtn[i]->bkctbl[2]=0XAD97;
			tbtn[i]->bkctbl[3]=0X8452;
		}
		if(i==0||i==2||i==4)tbtn[i]->caption="＋";
		if(i==1||i==3||i==5)tbtn[i]->caption="－";
		if(i==6)tbtn[i]->caption=(u8*)GUI_OK_CAPTION_TBL[gui_phy.language];;
		if(i==7)tbtn[i]->caption=(u8*)GUI_CANCEL_CAPTION_TBL[gui_phy.language];;
	}
    if(twin==NULL)rval=1;
	else
	{
		twin->caption=caption;
		twin->windowbkc=APP_WIN_BACK_COLOR;
	}
	if(rval==0)
	{
		window_draw(twin);
		for(i=0;i<8;i++)btn_draw(tbtn[i]);
		gui_fill_rectangle(x+10+1,y+42+41,SYSSET_EDIT_WIDTH-2,SYSSET_EDIT_HEIGHT,SYSSET_EDIT_BACK_COLOR);
		gui_fill_rectangle(x+10+60+1,y+42+41,SYSSET_EDIT_WIDTH-2,SYSSET_EDIT_HEIGHT,SYSSET_EDIT_BACK_COLOR);
 		gui_fill_rectangle(x+10+120+1,y+42+41,SYSSET_EDIT_WIDTH-2,SYSSET_EDIT_HEIGHT,SYSSET_EDIT_BACK_COLOR);

 		app_show_nummid(x+10,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*year,4,16,BLACK,SYSSET_EDIT_BACK_COLOR);
		app_show_nummid(x+10+60,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*month,2,16,BLACK,SYSSET_EDIT_BACK_COLOR);
		app_show_nummid(x+10+120,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*date,2,16,BLACK,SYSSET_EDIT_BACK_COLOR);
		while(rval==0)
		{
			tp_dev.scan(0);
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);
			delay_ms(1000/OS_TICKS_PER_SEC);
 			if(system_task_return)
			{
				rval=1;
				break;
			}
			for(i=0;i<8;i++)
			{
				res=btn_check(tbtn[i],&in_obj);
				if(res)
				{
					if((tbtn[i]->sta&0X80)==0)
					{
						if(*month==2)
						{
							if(sysset_is_leap_year(*year))maxdate=29;
	 						else maxdate=28;
						}else maxdate=31;
						switch(i)
						{
							case 0:
								(*year)++;
								if(*year>2100)*year=2000;
								break;
							case 1:
								if(*year>2000)(*year)--;
								else *year=2100;
								break;
							case 2:
								(*month)++;
								if(*month>12)(*month)=1;
								break;
							case 3:
								if(*month>1)(*month)--;
								else *month=12;
								break;
							case 4:
								(*date)++;
								if(*date>maxdate)(*date)=1;
								break;
							case 5:
								if(*date>1)(*date)--;
								else *date=maxdate;
								break;
							case 6:
								rval=0XFF;
								break;
							case 7:
								rval=1;
								break;
 						}
					}
			 		app_show_nummid(x+10,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*year,4,16,BLACK,SYSSET_EDIT_BACK_COLOR);
					app_show_nummid(x+10+60,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*month,2,16,BLACK,SYSSET_EDIT_BACK_COLOR);
					app_show_nummid(x+10+120,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*date,2,16,BLACK,SYSSET_EDIT_BACK_COLOR);
				}
			}

		}
 	}
	window_delete(twin);
	for(i=0;i<8;i++)btn_delete(tbtn[i]);
	system_task_return=0;
	if(rval==0XFF)return 0;
	return rval;
}

u8 * set_search_caption(const u8 *mcaption)
{
	while(*mcaption!='.')mcaption++;
	return (u8*)(++mcaption);
}

void sysset_read_para(_system_setings * sysset)
{
	AT24CXX_Read(SYSTEM_PARA_SAVE_BASE,(u8*)sysset,sizeof(_system_setings));
}

void sysset_save_para(_system_setings * sysset)
{
  OS_CPU_SR cpu_sr=0;
	OS_ENTER_CRITICAL();
	AT24CXX_Write(SYSTEM_PARA_SAVE_BASE,(u8*)sysset,sizeof(_system_setings));
	OS_EXIT_CRITICAL();
}

u8 sysset_play(void)
{
	u8 tlanguage;
	u8 res;
	u8 selitem=0;
	u8 topitem=0;
	u8 **items;
 	u8 *caption;
	u8 *scaption;
	u16 temp1;
	u8 temp2,temp3;
	u8 savemask=0;
  caption=(u8*)APP_MFUNS_CAPTION_TBL[5][gui_phy.language];
	items=(u8**)sysset_mmenu_tbl[gui_phy.language];
	tlanguage=gui_phy.language;
	while(1)
	{
   	res=app_listbox_select(&selitem,&topitem,caption,items,SYS_MENU_SIZE);
 		if(system_task_return)break;
		if(res&0X80)
		{
			scaption=set_search_caption(items[selitem]);
			switch(selitem)
			{
				case 0:
					RTC_Get();
					temp1=calendar.hour;
					temp2=calendar.min;
					res=sysset_time_set((lcddev.width-150)/2,(lcddev.height-200)/2,(u8*)&temp1,(u8*)&temp2,scaption);
					if(res==0)
					{
						RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date,temp1,temp2,0);
					}
  					break;
				case 1:
					RTC_Get();
					temp1=calendar.w_year;
					temp2=calendar.w_month;
					temp3=calendar.w_date;
					res=sysset_date_set((lcddev.width-190)/2,(lcddev.height-200)/2,&temp1,(u8*)&temp2,(u8*)&temp3,scaption);
					if(res==0)
					{
						RTC_Set(temp1,temp2,temp3,calendar.hour,calendar.min,calendar.sec);
					}
					break;
				case 2:
					temp1=alarm.hour;
					temp2=alarm.min;
					res=sysset_time_set((lcddev.width-150)/2,(lcddev.height-200)/2,(u8*)&temp1,(u8*)&temp2,scaption);
					if(res==0)
					{
						alarm.hour=temp1;
						alarm.min=temp2;
						calendar_alarm_init(&alarm,&calendar);
						savemask|=1<<2;
					}
  					break;
				case 3:
					temp1=alarm.weekmask;
			 		res=app_items_sel((lcddev.width-200)/2,(lcddev.height-302)/2,200,302,(u8**)calendar_week_table[gui_phy.language],7,(u8*)&temp1,0XB0,scaption);
 					if(res==0)
					{
						alarm.weekmask=temp1;
						calendar_alarm_init(&alarm,&calendar);
						savemask|=1<<2;
					}
					break;
				case 4:
					temp1=alarm.ringsta&0X03;
			 		res=app_items_sel((lcddev.width-180)/2,(lcddev.height-232)/2,180,72+40*4,(u8**)sysset_ring_tbl[gui_phy.language],4,(u8*)&temp1,0X90,scaption);
					if(res==0)
					{
						alarm.ringsta&=0XFC;
						alarm.ringsta|=temp1;
						savemask|=1<<2;
					}
					break;
				case 5:
					temp1 = timer.minute;
					temp2 = timer.second;
					res = sysset_time_set((lcddev.width-150)/2,(lcddev.height-200)/2, (u8*)&temp1, (u8*)&temp2, scaption);
					if (res == 0) {
						timer.minute = temp1;
						timer.second = temp2;
						calendar_timer_init(&alarm, &timer);
						savemask |= 1 << 3;
					}
					break;
				case 6:
					temp1=gui_phy.language;
			 		res=app_items_sel((lcddev.width-180)/2,(lcddev.height-192)/2,180,72+40*3,(u8**)sysset_language_tbl,3,(u8*)&temp1,0X90,scaption);
					if(res==0)
					{
						gui_phy.language=temp1;
						systemset.syslanguage=temp1;
					    caption=(u8*)APP_MFUNS_CAPTION_TBL[5][gui_phy.language];
						items=(u8**)sysset_mmenu_tbl[gui_phy.language];
						savemask|=1<<0;
					}
					break;
			}
  		} else break;
	}
	if(savemask & 1 << 0) {
		printf("sysset save!\r\n");
		sysset_save_para(&systemset);
	}
	if(savemask & 1 << 2) {
		printf("alarm save!\r\n");
		calendar_save_para(&alarm);
	}
	if (savemask & 1 << 3) {
		printf("timer save!\r\n");
		timer_save_para(&timer);
	}
	if(tlanguage!=gui_phy.language) {
		LCD_Clear(BLACK);
	}
	return res;
}
