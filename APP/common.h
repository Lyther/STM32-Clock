#ifndef __COMMON_H
#define __COMMON_H
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "wdg.h"
#include "timer.h"
#include "lcd.h"
#include "usmart.h"
#include "rtc.h"
#include "wkup.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "24cxx.h"
#include "flash.h"
#include "touch.h"
#include "24l01.h"
#include "remote.h"
#include "ds18b20.h"
#include "ps2.h"
#include "mouse.h"
#include "stmflash.h"
#include "malloc.h"
#include "string.h"
#include "mmc_sd.h"
#include "ff.h"
#include "exfuns.h"
#include "fontupd.h"
#include "text.h"
#include "piclib.h"
#include "t9input.h"
#include "includes.h"
#include "ff.h"
#include "piclib.h"
#include "gui.h"

#define HARDWARE_VERSION 30
#define SOFTWARE_VERSION 230

#define SYSTEM_PARA_SAVE_BASE 100

extern volatile u8 system_task_return;
extern volatile u8 setmode;

extern u8*const  APP_OK_PIC;
extern u8*const  APP_CANCEL_PIC;
extern u8*const  APP_UNSELECT_PIC;
extern u8*const  APP_SELECT_PIC;
extern u8*const  APP_ASCII_60;
extern u8*const  APP_ASCII_28;
extern u8*const  APP_VOL_PIC;

extern u8*const APP_ASCII_S6030;
extern u8*const APP_ASCII_5427;
extern u8*const APP_ASCII_3618;
extern u8*const APP_ASCII_2814;

extern u8* asc2_s6030;
extern u8* asc2_5427;
extern u8* asc2_3618;
extern u8* asc2_2814;

extern const u8 APP_ALIENTEK_ICO1824[];
extern const u8 APP_ALIENTEK_ICO2432[];
extern const u8 APP_ALIENTEK_ICO3648[];

#define APP_FUNS_NUM 9

extern u8*const  APP_MFUNS_CAPTION_TBL[APP_FUNS_NUM][GUI_LANGUAGE_NUM];
extern u8*const  APP_MODESEL_CAPTION_TBL[GUI_LANGUAGE_NUM];
extern u8*const  APP_REMIND_CAPTION_TBL[GUI_LANGUAGE_NUM];
extern u8*const  APP_SAVE_CAPTION_TBL[GUI_LANGUAGE_NUM];
extern u8*const  APP_CREAT_ERR_MSG_TBL[GUI_LANGUAGE_NUM];

#define WIN_SMOOTH_LINE_SEC	0XB1FFC4
#define WIN_SMOOTH_LINE_MC	0XF5D81B


#define APP_ITEM_BTN1_WIDTH 60
#define APP_ITEM_BTN2_WIDTH 100
#define APP_ITEM_BTN_HEIGHT 30
#define APP_ITEM_ICO_SIZE 32

#define APP_ITEM_SEL_BKCOLOR 0XFF00
#define APP_WIN_BACK_COLOR 0XC618

#define APP_FB_TOPBAR_HEIGHT 20
#define APP_FB_BTMBAR_HEIGHT 20
#define APP_TEST_TOPBAR_HEIGHT 20

#define	app_pi 3.1415926535897932384626433832795

u32  app_get_rand(u32 max);
void app_srand(u32 seed);
void app_set_lcdsize(u8 mode);
void app_read_bkcolor(u16 x,u16 y,u16 width,u16 height,u16 *ctbl);
void app_recover_bkcolor(u16 x,u16 y,u16 width,u16 height,u16 *ctbl);
void app_gui_tcbar(u16 x,u16 y,u16 width,u16 height,u8 mode);
u8 app_get_numlen(long long num,u8 dir);
void app_show_float(u16 x,u16 y,long long num,u8 flen,u8 clen,u8 font,u16 color,u16 bkcolor);
void app_filebrower(u8 *topname,u8 mode);
void app_showbigchar(u8 *fontbase,u16 x,u16 y,u8 chr,u8 size,u16 color,u16 bkcolor);
void app_showbigstring(u8 *fontbase,u16 x,u16 y,const u8 *p,u8 size,u16 color,u16 bkcolor);
void app_showbignum(u8 *fontbase,u16 x,u16 y,u32 num,u8 len,u8 size,u16 color,u16 bkcolor);
void app_showbig2num(u8 *fontbase,u16 x,u16 y,u8 num,u8 size,u16 color,u16 bkcolor);
void app_show_nummid(u16 x,u16 y,u16 width,u16 height,u32 num,u8 len,u8 size,u16 ptcolor,u16 bkcolor);
void app_draw_smooth_line(u16 x,u16 y,u16 width,u16 height,u32 sergb,u32 mrgb);
void app_show_mono_icos(u16 x,u16 y,u8 width,u8 height,u8 *icosbase,u16 color,u16 bkcolor);

u8 app_tp_is_in_area(_m_tp_dev *tp,u16 x,u16 y,u16 width,u16 height);
void app_show_items(u16 x,u16 y,u16 itemwidth,u16 itemheight,u8*name,u8*icopath,u16 color,u16 bkcolor);
u8 * app_get_icopath(u8 mode,u8 *selpath,u8 *unselpath,u8 selx,u8 index);
u8 app_items_sel(u16 x,u16 y,u16 width,u16 height,u8 *items[],u8 itemsize,u8 *selx,u8 mode,u8*caption);
u8 app_listbox_select(u8 *sel,u8 *top,u8 * caption,u8 *items[],u8 itemsize);
void app_show_mono_icos(u16 x,u16 y,u8 width,u8 height,u8 *icosbase,u16 color,u16 bkcolor);

u8 app_system_file_check(u8* diskx);
u8 app_boot_cpdmsg(u8*pname,u8 pct,u8 mode);
void app_boot_cpdmsg_set(u16 x,u16 y,u8 fsize);
u8 app_system_update(u8(*fcpymsg)(u8*pname,u8 pct,u8 mode));
void app_getstm32_sn(u32 *sn0,u32 *sn1,u32 *sn2);
void app_get_version(u8*buf,u32 ver,u8 len);

void app_usmart_getsn(void);
u8 app_system_parameter_init(void);
#endif
