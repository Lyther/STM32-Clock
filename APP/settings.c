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
	},
};	    

u8*const sysset_language_tbl[GUI_LANGUAGE_NUM]=
{
	"简体中文","繁體中文","English",
};  	    

u8*const sysset_ring_tbl[GUI_LANGUAGE_NUM][4]=
{
{"DS1闪烁1","DS1闪烁2","DS1闪烁3","DS1闪烁4",},
{"DS1閃爍1","DS1閃爍2","DS1閃爍3","DS1閃爍4",},
{"DS1 FLASH 1","DS1 FLASH 2","DS1 FLASH 3","DS1 FLASH 4",},	 
};

#define SYSSET_BTN1_WIDTH			50			//一类按键宽度(加减按钮)
#define SYSSET_BTN1_HEIGHT			40			//一类按键高度(加减按钮)
#define SYSSET_BTN2_WIDTH			60			//二类按键宽度(确认/取消按钮)
#define SYSSET_BTN2_HEIGHT			30			//二类按键高度(确认/取消按钮)
#define SYSSET_BTN3_WIDTH			100			//三类按键宽度(单个按钮的宽度)
#define SYSSET_EDIT_WIDTH			50			//文字编辑处的宽度
#define SYSSET_EDIT_HEIGHT			25			//文字编辑处的高度
 
#define SYSSET_INWIN_FONT_COLOR		0X736C		//0XAD53		//窗体内嵌字颜色
#define SYSSET_EDIT_BACK_COLOR		0XE79E		//显示内容区背景色
 
/////////////////////////////////////////////////////////////////////////////////////////////

//时间/闹钟设置
//x,y:窗口坐标(窗口尺寸已经固定了的)	 
//hour,min:时分
//caption:窗口名字				  
//返回值:0,ok;其他,取消或者错误.
u8 sysset_time_set(u16 x,u16 y,u8 *hour,u8 *min,u8*caption) 
{
	u8 rval=0,res;
	u8 i;
	_window_obj* twin=0;	//窗体
 	_btn_obj * tbtn[6];		//总共六个按钮:0,时钟加按钮;1,时钟减按钮;2,分钟加按钮;3,分钟减按钮;4,确认按钮;5,取消按钮.		  
 	twin=window_creat(x,y,150,199,0,1|1<<5,16);//创建窗口
 	tbtn[0]=btn_creat(x+20,y+42,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);			//创建按钮
	tbtn[1]=btn_creat(x+20,y+42+67,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);		//创建按钮
	tbtn[2]=btn_creat(x+20+60,y+42,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);		//创建按钮
	tbtn[3]=btn_creat(x+20+60,y+42+67,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);		//创建按钮
	tbtn[4]=btn_creat(x+10,y+42+117,SYSSET_BTN2_WIDTH,SYSSET_BTN2_HEIGHT,0,0x02);		//创建按钮
	tbtn[5]=btn_creat(x+10+70,y+42+117,SYSSET_BTN2_WIDTH,SYSSET_BTN2_HEIGHT,0,0x02);	//创建按钮
	for(i=0;i<6;i++)
	{
		if(tbtn[i]==NULL)
		{
			rval=1;
			break;
		}
		if(i<4)//加减按键
		{
			tbtn[i]->bcfucolor=BLACK;//松开时为黑色
			tbtn[i]->bcfdcolor=WHITE;//按下时为白色			
			tbtn[i]->bkctbl[0]=0X453A;//边框颜色
			tbtn[i]->bkctbl[1]=0X5DDC;//第一行的颜色				
			tbtn[i]->bkctbl[2]=0X5DDC;//上半部分颜色
			tbtn[i]->bkctbl[3]=0X453A;//下半部分颜色
		}else//确认和取消按键
		{
			tbtn[i]->bkctbl[0]=0X8452;//边框颜色
			tbtn[i]->bkctbl[1]=0XAD97;//第一行的颜色				
			tbtn[i]->bkctbl[2]=0XAD97;//上半部分颜色
			tbtn[i]->bkctbl[3]=0X8452;//下半部分颜色
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
	if(rval==0)//无错误
	{
		window_draw(twin);					//画出窗体
		for(i=0;i<6;i++)btn_draw(tbtn[i]);	//画按钮
		gui_fill_rectangle(x+20+1,y+42+41,SYSSET_EDIT_WIDTH-2,SYSSET_EDIT_HEIGHT,SYSSET_EDIT_BACK_COLOR);  		//填充时钟背景
		gui_fill_rectangle(x+20+60+1,y+42+41,SYSSET_EDIT_WIDTH-2,SYSSET_EDIT_HEIGHT,SYSSET_EDIT_BACK_COLOR);	//填充分钟背景
 		app_show_nummid(x+20,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*hour,2,16,BLACK,SYSSET_EDIT_BACK_COLOR);
		app_show_nummid(x+20+60,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*min,2,16,BLACK,SYSSET_EDIT_BACK_COLOR);  
		while(rval==0)
		{
			tp_dev.scan(0);    
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//得到按键键值   
			delay_ms(1000/OS_TICKS_PER_SEC);		//延时一个时钟节拍
 			if(system_task_return)
			{
				rval=1;			//取消设置
				break;			//TPAD返回	
			}
			for(i=0;i<6;i++)
			{
				res=btn_check(tbtn[i],&in_obj);//确认按钮检测
				if(res)
				{
					if((tbtn[i]->sta&0X80)==0)//有有效操作
					{
						switch(i)
						{
							case 0://时钟增加按钮按下了
								(*hour)++;
								if(*hour>23)*hour=0;
								break;
							case 1://时钟减少按钮按下了	  
								if(*hour)(*hour)--;
								else *hour=23;
								break;
							case 2://分钟增加按钮按下了
								(*min)++;
								if(*min>59)(*min)=0;
								break;
							case 3://分钟减少按钮按下了	  
								if(*min)(*min)--;
								else *min=59;
								break;
							case 4://确认按钮按下  
								rval=0XFF;
								break;	  
							case 5://取消按钮按下	  
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
	window_delete(twin);				//删除窗口
	for(i=0;i<6;i++)btn_delete(tbtn[i]);//删除按钮					   
	system_task_return=0;				//清除退出标志
	if(rval==0XFF)return 0;
	return rval;
}
//判断是否是闰年函数
//月份   1  2  3  4  5  6  7  8  9  10 11 12
//闰年   31 29 31 30 31 30 31 31 30 31 30 31
//非闰年 31 28 31 30 31 30 31 31 30 31 30 31
//year:年份
//返回值:该年份是不是闰年.1,是.0,不是
u8 sysset_is_leap_year(u16 year)
{			  
	if(year%4==0) //必须能被4整除
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//如果以00结尾,还要能被400整除 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}	
//日期设置
//x,y:窗口坐标(窗口尺寸已经固定了的)
//year,month,date:年月日
//caption:窗口名字
//返回值:0,ok;其他,取消或者错误.
u8 sysset_date_set(u16 x,u16 y,u16 *year,u8 *month,u8 *date,u8*caption) 
{
	u8 rval=0,res;
	u8 i;
	u8 maxdate=31;			//2月份最大的天数
	_window_obj* twin=0;	//窗体
 	_btn_obj * tbtn[8];		//总共八个按钮:0,年份加按钮;1,年份减按钮;2,月份加按钮;3月份减按钮;4,日期加按钮;5,日期减按钮;6,确认按钮;7,取消按钮		  
 	twin=window_creat(x,y,190,199,0,1|1<<5,16);//创建窗口
 	tbtn[0]=btn_creat(x+10,y+42,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);			//创建按钮
	tbtn[1]=btn_creat(x+10,y+42+67,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);		//创建按钮
	tbtn[2]=btn_creat(x+10+60,y+42,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);		//创建按钮
	tbtn[3]=btn_creat(x+10+60,y+42+67,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);		//创建按钮
	tbtn[4]=btn_creat(x+10+120,y+42,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);		//创建按钮
	tbtn[5]=btn_creat(x+10+120,y+42+67,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);	//创建按钮

	tbtn[6]=btn_creat(x+20,y+42+117,SYSSET_BTN2_WIDTH,SYSSET_BTN2_HEIGHT,0,0x02);		//创建按钮
	tbtn[7]=btn_creat(x+20+90,y+42+117,SYSSET_BTN2_WIDTH,SYSSET_BTN2_HEIGHT,0,0x02);	//创建按钮
 	for(i=0;i<8;i++)
	{
		if(tbtn[i]==NULL)
		{
			rval=1;
			break;
		}
		if(i<6)//加减按键
		{
			tbtn[i]->bcfucolor=BLACK;//松开时为黑色
			tbtn[i]->bcfdcolor=WHITE;//按下时为白色			
			tbtn[i]->bkctbl[0]=0X453A;//边框颜色
			tbtn[i]->bkctbl[1]=0X5DDC;//第一行的颜色				
			tbtn[i]->bkctbl[2]=0X5DDC;//上半部分颜色
			tbtn[i]->bkctbl[3]=0X453A;//下半部分颜色
		}else//确认和取消按键
		{
			tbtn[i]->bkctbl[0]=0X8452;//边框颜色
			tbtn[i]->bkctbl[1]=0XAD97;//第一行的颜色				
			tbtn[i]->bkctbl[2]=0XAD97;//上半部分颜色
			tbtn[i]->bkctbl[3]=0X8452;//下半部分颜色
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
	if(rval==0)//无错误
	{
		window_draw(twin);					//画出窗体
		for(i=0;i<8;i++)btn_draw(tbtn[i]);	//画按钮
		gui_fill_rectangle(x+10+1,y+42+41,SYSSET_EDIT_WIDTH-2,SYSSET_EDIT_HEIGHT,SYSSET_EDIT_BACK_COLOR);  		//填充年份背景
		gui_fill_rectangle(x+10+60+1,y+42+41,SYSSET_EDIT_WIDTH-2,SYSSET_EDIT_HEIGHT,SYSSET_EDIT_BACK_COLOR);	//填充月份背景
 		gui_fill_rectangle(x+10+120+1,y+42+41,SYSSET_EDIT_WIDTH-2,SYSSET_EDIT_HEIGHT,SYSSET_EDIT_BACK_COLOR);	//填充日期背景

 		app_show_nummid(x+10,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*year,4,16,BLACK,SYSSET_EDIT_BACK_COLOR);
		app_show_nummid(x+10+60,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*month,2,16,BLACK,SYSSET_EDIT_BACK_COLOR);  
		app_show_nummid(x+10+120,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*date,2,16,BLACK,SYSSET_EDIT_BACK_COLOR);  
		while(rval==0)
		{
			tp_dev.scan(0);    
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//得到按键键值   
			delay_ms(1000/OS_TICKS_PER_SEC);		//延时一个时钟节拍
 			if(system_task_return)
			{
				rval=1;			//取消设置
				break;			//TPAD返回	
			}
			for(i=0;i<8;i++)
			{
				res=btn_check(tbtn[i],&in_obj);//确认按钮检测
				if(res)
				{
					if((tbtn[i]->sta&0X80)==0)//有有效操作
					{
						if(*month==2)
						{
							if(sysset_is_leap_year(*year))maxdate=29;//是闰年的2月份
	 						else maxdate=28;	  
						}else maxdate=31;  
						switch(i)
						{
							case 0://年份增加按钮按下了
								(*year)++;
								if(*year>2100)*year=2000;
								break;
							case 1://年份减少按钮按下了	  
								if(*year>2000)(*year)--;
								else *year=2100;
								break;
							case 2://月份增加按钮按下了
								(*month)++;
								if(*month>12)(*month)=1;
								break;
							case 3://月份减少按钮按下了	  
								if(*month>1)(*month)--;
								else *month=12;
								break;
							case 4://日期增加按钮按下了
								(*date)++;
								if(*date>maxdate)(*date)=1;
								break;
							case 5://日期减少按钮按下了	  
								if(*date>1)(*date)--;
								else *date=maxdate;
								break;
							case 6://确认按钮按下  
								rval=0XFF;
								break;	  
							case 7://取消按钮按下  
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
	window_delete(twin);				//删除窗口
	for(i=0;i<8;i++)btn_delete(tbtn[i]);//删除按钮					   
	system_task_return=0;				//清除退出标志
	if(rval==0XFF)return 0;
	return rval;
}	  

//查找条目名字
//mcaption:主目录下的目录名(一定要有'.'字符在里面)
u8 * set_search_caption(const u8 *mcaption)
{
	while(*mcaption!='.')mcaption++;
	return (u8*)(++mcaption);
}
//系统设置数据保存在:SYSTEM_PARA_SAVE_BASE
//读取系统设置信息
//sysset:系统信息 
void sysset_read_para(_system_setings * sysset)
{
	AT24CXX_Read(SYSTEM_PARA_SAVE_BASE,(u8*)sysset,sizeof(_system_setings));
}
//写入系统设置信息
//sysset:系统信息 
void sysset_save_para(_system_setings * sysset)
{
  	OS_CPU_SR cpu_sr=0;
	OS_ENTER_CRITICAL();//进入临界区(无法被中断打断) 
	AT24CXX_Write(SYSTEM_PARA_SAVE_BASE,(u8*)sysset,sizeof(_system_setings));
	OS_EXIT_CRITICAL();	//退出临界区(可以被中断打断)
}

//系统设置 
u8 sysset_play(void)
{	
	u8 tlanguage;			//进入时语言的类型					  	   
	u8 res;
	u8 selitem=0;
	u8 topitem=0;  	 
	u8 **items;	     					   	   
 	u8 *caption;			//标题列表 
	u8 *scaption;			//子标题
	u16 temp1;
	u8 temp2,temp3;			//时间/日期暂存
	u8 savemask=0;		    //0,不需要保存任何数据;
							//[0]:systemset数据改动标记
							//[1]:vsset数据改动标记
							//[2]:alarm数据改动标记	 
  
    caption=(u8*)APP_MFUNS_CAPTION_TBL[5][gui_phy.language];	 
	items=(u8**)sysset_mmenu_tbl[gui_phy.language];
	tlanguage=gui_phy.language;							//保存刚进入是语言的种类
	while(1)
	{   						   
   	res=app_listbox_select(&selitem,&topitem,caption,items,SYS_MENU_SIZE);
 		if(system_task_return)break;
		if(res&0X80)
		{	  
			scaption=set_search_caption(items[selitem]); 	 						   
			switch(selitem)//判断选择的条目
			{
				case 0://时间设置 			    
					RTC_Get();//更新到最新的时间 
					temp1=calendar.hour;
					temp2=calendar.min;
					res=sysset_time_set((lcddev.width-150)/2,(lcddev.height-200)/2,(u8*)&temp1,(u8*)&temp2,scaption);
					if(res==0)//成功
					{
						RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date,temp1,temp2,0);//设置时间
					} 
  					break;	  
				case 1://日期设置  	    
					RTC_Get();//更新到最新的时间 
					temp1=calendar.w_year;
					temp2=calendar.w_month;	 
					temp3=calendar.w_date;
					res=sysset_date_set((lcddev.width-190)/2,(lcddev.height-200)/2,&temp1,(u8*)&temp2,(u8*)&temp3,scaption);
					if(res==0)//成功
					{
						RTC_Set(temp1,temp2,temp3,calendar.hour,calendar.min,calendar.sec);//设置时间
					} 																	  
					break;
				case 2://闹钟时间设置
					temp1=alarm.hour;
					temp2=alarm.min;
					res=sysset_time_set((lcddev.width-150)/2,(lcddev.height-200)/2,(u8*)&temp1,(u8*)&temp2,scaption);
					if(res==0)//成功,点击了确认按钮
					{
						alarm.hour=temp1;
						alarm.min=temp2;
						calendar_alarm_init(&alarm,&calendar);//重新初始化闹钟
						savemask|=1<<2;//标记闹钟数据改动了
					} 
  					break;
				case 3://闹钟开关设置
					temp1=alarm.weekmask;
			 		res=app_items_sel((lcddev.width-200)/2,(lcddev.height-302)/2,200,302,(u8**)calendar_week_table[gui_phy.language],7,(u8*)&temp1,0XB0,scaption);//多选
 					if(res==0)//设置成功
					{
						alarm.weekmask=temp1;
						calendar_alarm_init(&alarm,&calendar);//重新初始化闹钟	
						savemask|=1<<2;//标记闹钟数据改动了
					}   
					break;
				case 4://闹钟铃声类型设置
					temp1=alarm.ringsta&0X03;
			 		res=app_items_sel((lcddev.width-180)/2,(lcddev.height-232)/2,180,72+40*4,(u8**)sysset_ring_tbl[gui_phy.language],4,(u8*)&temp1,0X90,scaption);//单选
					if(res==0)//设置成功
					{
						alarm.ringsta&=0XFC;//取消最后两位原来的设置
						alarm.ringsta|=temp1;//重新设置铃声						    
						savemask|=1<<2;//标记闹钟数据改动了
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
				case 6://语言设置
					temp1=gui_phy.language;//得到之前的设置
			 		res=app_items_sel((lcddev.width-180)/2,(lcddev.height-192)/2,180,72+40*3,(u8**)sysset_language_tbl,3,(u8*)&temp1,0X90,scaption);//单选
					if(res==0)//设置成功
					{												   
						gui_phy.language=temp1;		//重新设置语言
						systemset.syslanguage=temp1;//重新设置默认语言
					    caption=(u8*)APP_MFUNS_CAPTION_TBL[5][gui_phy.language];	 
						items=(u8**)sysset_mmenu_tbl[gui_phy.language];			 
						savemask|=1<<0;//标记系统设置数据改动了
					} 
					break; 
			}
  		} else break;//有错误发生了	 			   
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
