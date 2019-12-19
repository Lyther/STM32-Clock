#include "common.h"
#include "settings.h"
#include "calendar.h"

u8* const APP_MODESEL_CAPTION_TBL[GUI_LANGUAGE_NUM]=
{
"模式选择","模式選擇","Mode Select",
};

u8*const APP_REMIND_CAPTION_TBL[GUI_LANGUAGE_NUM]=
{
"提示信息","提示信息","Remind",
};

 u8*const APP_SAVE_CAPTION_TBL[GUI_LANGUAGE_NUM]=
{
"保存文件?","保存文件?","Save?",
};

u8*const APP_CREAT_ERR_MSG_TBL[GUI_LANGUAGE_NUM]=
{
"保存失败,检查SD卡!","創建失敗,檢查SD卡!","Failed,Check SD!",
};

u8*const APP_MFUNS_CAPTION_TBL[APP_FUNS_NUM][GUI_LANGUAGE_NUM]={{"","",""},{"","",""},{"","",""},{"","",""},{"","",""},{"","",""},{"","",""},{"","",""},{"","",""}};

u8*const  APP_OK_PIC="1:/SYSTEM/APP/COMMON/ok.bmp";
u8*const  APP_CANCEL_PIC="1:/SYSTEM/APP/COMMON/cancel.bmp";
u8*const  APP_UNSELECT_PIC="1:/SYSTEM/APP/COMMON/unselect.bmp";
u8*const  APP_SELECT_PIC="1:/SYSTEM/APP/COMMON/select.bmp";

u8*const APP_ASCII_S6030="1:/SYSTEM/APP/COMMON/fonts60.fon";
u8*const APP_ASCII_5427="1:/SYSTEM/APP/COMMON/font54.fon";
u8*const APP_ASCII_3618="1:/SYSTEM/APP/COMMON/font36.fon";
u8*const APP_ASCII_2814="1:/SYSTEM/APP/COMMON/font28.fon";

u8* asc2_s6030=0;
u8* asc2_5427=0;
u8* asc2_3618=0;
u8* asc2_2814=0;

const u8 APP_ALIENTEK_ICO1824[]=
{
0x03,0xF0,0x00,0x0F,0xF8,0x00,0x1F,0xFC,0x00,0x3F,0xFE,0x00,0x7F,0xFF,0x00,0x7F,
0xFF,0x00,0xFF,0xFF,0x80,0xFF,0xFF,0x80,0xFF,0xFF,0x80,0xFF,0xFF,0x80,0xFF,0xFF,
0x80,0xFF,0xFF,0x80,0xCF,0xF9,0x80,0xC7,0xF1,0x80,0x43,0xE1,0x00,0x61,0xC3,0x00,
0x31,0xC6,0x00,0x39,0xCE,0x00,0x1F,0xFC,0x00,0x0F,0xF8,0x00,0x07,0xF0,0x00,0x03,
0xE0,0x00,0x01,0xC0,0x00,0x00,0x80,0x00
};


const u8 APP_ALIENTEK_ICO2432[]=
{
0x00,0x7F,0x00,0x01,0xFF,0xC0,0x03,0xFF,0xE0,0x07,0xFF,0xF0,0x0F,0xFF,0xF8,0x1F,
0xFF,0xFC,0x1F,0xFF,0xFC,0x3F,0xFF,0xFE,0x3F,0xFF,0xFE,0x7F,0xFF,0xFF,0x7F,0xFF,
0xFF,0x7F,0xFF,0xFF,0x7F,0xFF,0xFF,0x7F,0xFF,0xFF,0x7F,0xFF,0xFF,0x7F,0xFF,0xFF,
0x73,0xFF,0xE7,0x70,0xFF,0x87,0x30,0xFF,0x86,0x30,0x7F,0x06,0x38,0x3E,0x0E,0x1C,
0x3E,0x1C,0x1E,0x1C,0x3C,0x0F,0x1C,0x78,0x07,0xFF,0xF0,0x07,0xFF,0xF0,0x03,0xFF,
0xE0,0x01,0xFF,0xC0,0x00,0xFF,0x80,0x00,0x7F,0x00,0x00,0x3E,0x00,0x00,0x1C,0x00,
};


const u8 APP_ALIENTEK_ICO3648[]=
{
0x00,0x01,0xFC,0x00,0x00,0x00,0x0F,0xFF,0x80,0x00,0x00,0x1F,0xFF,0xC0,0x00,0x00,
0x7F,0xFF,0xF0,0x00,0x00,0xFF,0xFF,0xF8,0x00,0x01,0xFF,0xFF,0xFC,0x00,0x03,0xFF,
0xFF,0xFE,0x00,0x07,0xFF,0xFF,0xFF,0x00,0x07,0xFF,0xFF,0xFF,0x80,0x0F,0xFF,0xFF,
0xFF,0x80,0x0F,0xFF,0xFF,0xFF,0xC0,0x1F,0xFF,0xFF,0xFF,0xC0,0x1F,0xFF,0xFF,0xFF,
0xC0,0x1F,0xFF,0xFF,0xFF,0xC0,0x1F,0xFF,0xFF,0xFF,0xE0,0x1F,0xFF,0xFF,0xFF,0xE0,
0x3F,0xFF,0xFF,0xFF,0xE0,0x3F,0xFF,0xFF,0xFF,0xE0,0x3F,0xFF,0xFF,0xFF,0xE0,0x3F,
0xFF,0xFF,0xFF,0xE0,0x3F,0xFF,0xFF,0xFF,0xE0,0x3F,0xFF,0xFF,0xFF,0xE0,0x3F,0xFF,
0xFF,0xFF,0xE0,0x3F,0xFF,0xFF,0xFF,0xE0,0x3C,0xFF,0xFF,0xFD,0xE0,0x38,0x3F,0xFF,
0xF0,0xE0,0x18,0x1F,0xFF,0xC0,0xE0,0x1C,0x0F,0xFF,0x80,0xC0,0x1C,0x07,0xFF,0x01,
0xC0,0x0C,0x03,0xFF,0x01,0xC0,0x0E,0x03,0xFE,0x03,0x80,0x07,0x01,0xFC,0x07,0x80,
0x07,0x81,0xFC,0x07,0x00,0x03,0xC0,0xFC,0x0F,0x00,0x03,0xE0,0xF8,0x3E,0x00,0x01,
0xF0,0xF8,0x7E,0x00,0x01,0xFE,0xFD,0xFC,0x00,0x00,0xFF,0xFF,0xFC,0x00,0x00,0x7F,
0xFF,0xF8,0x00,0x00,0x3F,0xFF,0xF0,0x00,0x00,0x3F,0xFF,0xE0,0x00,0x00,0x1F,0xFF,
0xC0,0x00,0x00,0x0F,0xFF,0x80,0x00,0x00,0x07,0xFF,0x00,0x00,0x00,0x03,0xFE,0x00,
0x00,0x00,0x01,0xFC,0x00,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

u32 random_seed=1;
void app_srand(u32 seed)
{
	random_seed=seed;
}

u32 app_get_rand(u32 max)
{
	random_seed=random_seed*22695477+1;
	return (random_seed)%max;
}

void app_set_lcdsize(u8 mode)
{
	if(mode)
	{
		LCD_Clear(BLACK);
		if(lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X5510)
		{
			lcddev.width=240;
			lcddev.height=320;
			LCD_Scan_Dir(L2R_U2D);
		}
	}else
	{
		if(lcddev.id==0X6804||lcddev.id==0X5310)
		{
			lcddev.width=320;
			lcddev.height=480;
		}else if(lcddev.id==0X5510)
		{
			lcddev.width=480;
			lcddev.height=800;
		}else
		{
			lcddev.width=240;
			lcddev.height=320;
		}
		LCD_Scan_Dir(L2R_U2D);
	}
	gui_phy.lcdwidth=lcddev.width;
	gui_phy.lcdheight=lcddev.height;
}

void app_read_bkcolor(u16 x,u16 y,u16 width,u16 height,u16 *ctbl)
{
	u16 x0,y0,ccnt;
	ccnt=0;
	for(y0=y;y0<y+height;y0++)
	{
		for(x0=x;x0<x+width;x0++)
		{
			ctbl[ccnt]=gui_phy.read_point(x0,y0);
			ccnt++;
		}
	}
}

void app_recover_bkcolor(u16 x,u16 y,u16 width,u16 height,u16 *ctbl)
{
	u16 x0,y0,ccnt;
	ccnt=0;
	for(y0=y;y0<y+height;y0++)
	{
		for(x0=x;x0<x+width;x0++)
		{
			gui_phy.draw_point(x0,y0,ctbl[ccnt]);
			ccnt++;
		}
	}
}

void app_gui_tcbar(u16 x,u16 y,u16 width,u16 height,u8 mode)
{
 	u16 halfheight=height/2;
 	gui_fill_rectangle(x,y,width,halfheight,LIGHTBLUE);
 	gui_fill_rectangle(x,y+halfheight,width,halfheight,GRAYBLUE);
	if(mode&0x01)gui_draw_hline(x,y,width,DARKBLUE);
	if(mode&0x02)gui_draw_hline(x,y+height-1,width,DARKBLUE);
	if(mode&0x04)gui_draw_vline(x,y,height,DARKBLUE);
	if(mode&0x08)gui_draw_vline(x+width-1,y,width,DARKBLUE);
}

u8 app_get_numlen(long long num,u8 dir)
{
#define MAX_NUM_LENTH		10
	u8 i=0,j;
	u8 temp=0;
	if(dir==0)
	{
		i=MAX_NUM_LENTH-1;
		while(1)
		{
			temp=(num/gui_pow(10,i))%10;
			if(temp||i==0)break;
			i--;
		}
	}else
	{
		j=0;
 		while(1)
		{
			if(num%10)
			{
				i=app_get_numlen(num,0);
				return i;
			}
			if(j==(MAX_NUM_LENTH-1))break;
			num/=10;
			j++;
		}
	}
	if(i)return i+1;
	else if(temp)return 1;
	else return 0;
}

void app_show_mono_icos(u16 x,u16 y,u8 width,u8 height,u8 *icosbase,u16 color,u16 bkcolor)
{
	u16 rsize;
	u8 i,j;
	u8 temp;
	u8 t=0;
	u16 x0=x;
	rsize=width/8+((width%8)?1:0);
 	for(i=0;i<rsize*height;i++)
	{
		temp=icosbase[i];
		for(j=0;j<8;j++)
		{
	        if(temp&0x80)gui_phy.draw_point(x,y,color);
			else gui_phy.draw_point(x,y,bkcolor);
			temp<<=1;
			x++;
			t++;
			if(t==width)
			{
				t=0;
				x=x0;
				y++;
				break;
			}
		}
	}
}

void app_show_float(u16 x,u16 y,long long num,u8 flen,u8 clen,u8 font,u16 color,u16 bkcolor)
{
	u8 offpos=0;
	u8 ilen=0;
	u8 negative=0;
	u16 maxlen=(u16)clen*(font/2);
	gui_fill_rectangle(x-maxlen,y,maxlen,font,bkcolor);
	if(num<0)
	{
		num=-num;
  		negative=1;
	}
	ilen=app_get_numlen(num,0);
	gui_phy.back_color=bkcolor;
	if(flen)
	{
 		gui_show_num(x-(font/2)*flen,y,flen,color,font,num,0X80);
		gui_show_ptchar(x-(font/2)*(flen+1),y,gui_phy.lcdwidth,gui_phy.lcdheight,0,color,font,'.',0);
		offpos=2+flen;
	}
	if(ilen<=flen)ilen=1;
 	else
	{
		offpos=ilen+1;
		ilen=ilen-flen;
	}
	num=num/gui_pow(10,flen);
	gui_show_num(x-(font/2)*offpos,y,ilen,color,font,num,0X80);
	if(negative)gui_show_ptchar(x-(font/2)*(offpos+1),y,gui_phy.lcdwidth,gui_phy.lcdheight,0,color,font,'-',0);
}

void app_filebrower(u8 *topname,u8 mode)
{
  if(mode&0X01)app_gui_tcbar(0,0,lcddev.width,gui_phy.tbheight,0x02);
	if(mode&0X02)app_gui_tcbar(0,lcddev.height-gui_phy.tbheight,lcddev.width,gui_phy.tbheight,0x01);
	if(mode&0X04)gui_show_strmid(0,0,lcddev.width,gui_phy.tbheight,WHITE,gui_phy.tbfsize,topname);
}

void app_show_nummid(u16 x,u16 y,u16 width,u16 height,u32 num,u8 len,u8 size,u16 ptcolor,u16 bkcolor)
{
	u16 numlen;
	u8 xoff,yoff;
	numlen=(size/2)*len;
	if(numlen>width||size>height)return;
	xoff=(width-numlen)/2;
	yoff=(height-size)/2;
	POINT_COLOR=ptcolor;
	BACK_COLOR=bkcolor;
	LCD_ShowxNum(x+xoff,y+yoff,num,len,size,0X80);
}

void app_draw_smooth_line(u16 x,u16 y,u16 width,u16 height,u32 sergb,u32 mrgb)
{
	gui_draw_smooth_rectangle(x,y,width/2,height,sergb,mrgb);
	gui_draw_smooth_rectangle(x+width/2,y,width/2,height,mrgb,sergb);
}

u8 app_tp_is_in_area(_m_tp_dev *tp,u16 x,u16 y,u16 width,u16 height)
{
	if(tp->x[0]<=(x+width)&&tp->x[0]>=x&&tp->y[0]<=(y+height)&&tp->y[0]>=y)return 1;
	else return 0;
}

void app_show_items(u16 x,u16 y,u16 itemwidth,u16 itemheight,u8*name,u8*icopath,u16 color,u16 bkcolor)
{
  gui_fill_rectangle(x,y,itemwidth,itemheight,bkcolor);
	gui_show_ptstr(x+5,y+(itemheight-16)/2,x+itemwidth-10-APP_ITEM_ICO_SIZE-5,y+itemheight,0,color,16,name,1);
	if(icopath)minibmp_decode(icopath,x+itemwidth-10-APP_ITEM_ICO_SIZE,y+(itemheight-APP_ITEM_ICO_SIZE)/2,APP_ITEM_ICO_SIZE,APP_ITEM_ICO_SIZE,0,0);
}

u8 * app_get_icopath(u8 mode,u8 *selpath,u8 *unselpath,u8 selx,u8 index)
{
	u8 *icopath=0;
	if(mode)
	{
		if(selx&(1<<index))icopath=selpath;
		else icopath=unselpath;
	}else
	{
		if(selx==index)icopath=selpath;
		else icopath=unselpath;
	}
	return icopath;
}

u8 app_items_sel(u16 x,u16 y,u16 width,u16 height,u8 *items[],u8 itemsize,u8 *selx,u8 mode,u8*caption)
{
	u8 rval=0,res;
	u8 selsta=0;
	u16 i;
	u8 temp;
	u16 itemheight=0;
	u16 itemwidth=0;
	u8* unselpath=0;
	u8* selpath=0;
	u8* icopath=0;
 	_window_obj* twin=0;
 	_btn_obj * okbtn=0;
 	_btn_obj * cancelbtn=0;

	if(itemsize>8||itemsize<1)return 0xff;
	if(width<150||height<72)return 0xff;

	itemheight=(height-72)/itemsize-1;
	itemwidth=width-10;

 	twin=window_creat(x,y,width,height,0,1|(1<<5)|((1<<6)&mode),16);
  	if(mode&(1<<7))
	{
   		temp=(width-APP_ITEM_BTN1_WIDTH*2)/3;
		okbtn=btn_creat(x+temp,y+height-APP_ITEM_BTN_HEIGHT-5,APP_ITEM_BTN1_WIDTH,APP_ITEM_BTN_HEIGHT,0,0x02);
		cancelbtn=btn_creat(x+APP_ITEM_BTN1_WIDTH+temp*2,y+height-APP_ITEM_BTN_HEIGHT-5,APP_ITEM_BTN1_WIDTH,APP_ITEM_BTN_HEIGHT,0,0x02);
		if(twin==NULL||okbtn==NULL||cancelbtn==NULL)rval=1;
		else
		{
	 		okbtn->caption=(u8*)GUI_OK_CAPTION_TBL[gui_phy.language];
			okbtn->bkctbl[0]=0X8452;
			okbtn->bkctbl[1]=0XAD97;
			okbtn->bkctbl[2]=0XAD97;
			okbtn->bkctbl[3]=0X8452;
		}
	}else
	{
   		temp=(width-APP_ITEM_BTN2_WIDTH)/2;
		cancelbtn=btn_creat(x+temp,y+height-APP_ITEM_BTN_HEIGHT-5,APP_ITEM_BTN2_WIDTH,APP_ITEM_BTN_HEIGHT,0,0x02);
		if(twin==NULL||cancelbtn==NULL)rval=1;
	}
 	if(rval==0)
	{
 		twin->caption=caption;
		twin->windowbkc=APP_WIN_BACK_COLOR;
 		cancelbtn->caption=(u8*)GUI_CANCEL_CAPTION_TBL[gui_phy.language];
 		cancelbtn->bkctbl[0]=0X8452;
		cancelbtn->bkctbl[1]=0XAD97;
		cancelbtn->bkctbl[2]=0XAD97;
		cancelbtn->bkctbl[3]=0X8452;

		if(mode&(1<<4))
		{
  			if(mode&(1<<5))
			{
				unselpath=(u8*)APP_CANCEL_PIC;
				selpath=(u8*)APP_OK_PIC;
			}else
			{
				unselpath=(u8*)APP_UNSELECT_PIC;
				selpath=(u8*)APP_SELECT_PIC;
			}
		}
		window_draw(twin);
		btn_draw(cancelbtn);
	    if(mode&(1<<7))btn_draw(okbtn);
		for(i=0;i<itemsize;i++)
		{
			icopath=app_get_icopath(mode&(1<<5),selpath,unselpath,*selx,i);
			app_show_items(x+5,y+32+i*(itemheight+1),itemwidth,itemheight,items[i],icopath,BLACK,twin->windowbkc);
			if((i+1)!=itemsize)app_draw_smooth_line(x+5,y+32+(i+1)*(itemheight+1)-1,itemwidth,1,0Xb1ffc4,0X1600b1);
 		}
		while(rval==0)
		{
			tp_dev.scan(0);
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);
			delay_ms(1000/OS_TICKS_PER_SEC);
			if(system_task_return){rval=1;break;};
			if(mode&(1<<7))
			{
				res=btn_check(okbtn,&in_obj);
				if(res)
				{
					if((okbtn->sta&0X80)==0)
					{
						rval=0XFF;
						break;
					}
				}
			}
			res=btn_check(cancelbtn,&in_obj);
			if(res)
			{
				if((cancelbtn->sta&0X80)==0)
				{
					rval=1;
					break;
				}
			}
			temp=0XFF;
			for(i=0;i<itemsize;i++)
			{
				if(tp_dev.sta&TP_PRES_DOWN)
				{
				 	if(app_tp_is_in_area(&tp_dev,x+5,y+32+i*(itemheight+1),itemwidth,itemheight))
					{
						if((selsta&0X80)==0)
						{
							icopath=app_get_icopath(mode&(1<<5),selpath,unselpath,*selx,i);
							app_show_items(x+5,y+32+i*(itemheight+1),itemwidth,itemheight,items[i],icopath,BLACK,APP_ITEM_SEL_BKCOLOR);
							selsta=i;
							selsta|=0X80;
						}
						break;
					}
				}else
				{
				 	if(app_tp_is_in_area(&tp_dev,x+5,y+32+i*(itemheight+1),itemwidth,itemheight))
					{
						temp=i;
						break;
					}
				}
			}
			if((selsta&0X80)&&(tp_dev.sta&TP_PRES_DOWN)==0)
			{
				if((selsta&0X0F)==temp)
				{
					if(mode&(1<<5))
					{
						if((*selx)&(1<<temp))*selx&=~(1<<temp);
						else *selx|=1<<temp;
					}else
					{
						app_show_items(x+5,y+32+(*selx)*(itemheight+1),itemwidth,itemheight,items[*selx],unselpath,BLACK,twin->windowbkc);
						*selx=temp;
					}
				}else temp=selsta&0X0F;
 				icopath=app_get_icopath(mode&(1<<5),selpath,unselpath,*selx,temp);
				app_show_items(x+5,y+32+temp*(itemheight+1),itemwidth,itemheight,items[temp],icopath,BLACK,twin->windowbkc);
				selsta=0;
			}
 		}
 	}
	window_delete(twin);
	btn_delete(okbtn);
	btn_delete(cancelbtn);
	system_task_return=0;
	if(rval==0XFF)return 0;
	return rval;
}

u8 app_listbox_select(u8 *sel,u8 *top,u8 * caption,u8 *items[],u8 itemsize)
{
	u8  res;
	u8 rval=0;
  	u16 i;

 	_btn_obj* rbtn=0;
 	_btn_obj* okbtn=0;
	_listbox_obj * tlistbox;

	if(*sel>=itemsize||*top>=itemsize)return 2;
 	app_filebrower(caption,0X07);
   	tlistbox=listbox_creat(0,gui_phy.tbheight,lcddev.width,lcddev.height-gui_phy.tbheight*2,1,gui_phy.tbfsize);
	if(tlistbox==NULL)rval=1;
	else
	{
		for(i=0;i<itemsize;i++)
		{
			res=listbox_addlist(tlistbox,items[i]);
			if(res)
			{
				rval=1;
				break;
			}
		}
	}
	if(rval==0)
	{
		tlistbox->scbv->topitem=*top;
		tlistbox->selindex=*sel;
		listbox_draw_listbox(tlistbox);
		rbtn=btn_creat(lcddev.width-2*gui_phy.tbfsize-8-1,lcddev.height-gui_phy.tbheight,2*gui_phy.tbfsize+8,gui_phy.tbheight-1,0,0x03);
 		okbtn=btn_creat(0,lcddev.height-gui_phy.tbheight,2*gui_phy.tbfsize+8,gui_phy.tbheight-1,0,0x03);
		if(rbtn==NULL||okbtn==NULL)rval=1;
		else
		{
		 	rbtn->caption=(u8*)GUI_BACK_CAPTION_TBL[gui_phy.language];
			rbtn->font=gui_phy.tbfsize;
			rbtn->bcfdcolor=WHITE;
			rbtn->bcfucolor=WHITE;
			btn_draw(rbtn);


		 	okbtn->caption=(u8*)GUI_OK_CAPTION_TBL[gui_phy.language];
		 	okbtn->font=gui_phy.tbfsize;
			okbtn->bcfdcolor=WHITE;
			okbtn->bcfucolor=WHITE;
			btn_draw(okbtn);
		}
 	}
   	while(rval==0)
	{
		tp_dev.scan(0);
		in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);
		delay_ms(1000/OS_TICKS_PER_SEC);
 		if(system_task_return)break;
		res=btn_check(rbtn,&in_obj);
		if(res)if(((rbtn->sta&0X80)==0))break;
		res=btn_check(okbtn,&in_obj);
		if(res)
		{
			if(((okbtn->sta&0X80)==0))
			{
				*top=tlistbox->scbv->topitem;
				*sel=tlistbox->selindex;
				rval|=1<<7;
  			}
		}
		listbox_check(tlistbox,&in_obj);
		if(tlistbox->dbclick==0X80)
		{
			*top=tlistbox->scbv->topitem;
			*sel=tlistbox->selindex;
			rval|=1<<7;
		}
	}
	listbox_delete(tlistbox);
	btn_delete(okbtn);
	btn_delete(rbtn);
	return rval;
}

void app_system_file_chgdisk(u8 *des,u8 *diskx,u8 *src)
{

	strcpy((char*)des,(const char*)src);
	des[0]=diskx[0];

}

u8 app_system_file_check(u8* diskx)
{
	u8 i;
	u8 rval=0;
	u8 res=0;
	FIL *f_check;
	u8 *path;
	f_check=(FIL *)gui_memin_malloc(sizeof(FIL));
	path=gui_memin_malloc(200);
	if(!f_check||!path)rval=0XFF;
	while(rval==0)
	{
		rval+=i;
		if(i<9){rval++;break;}

		app_system_file_chgdisk(path,diskx,APP_OK_PIC);
		res=f_open(f_check,(const TCHAR*)path,FA_READ);
		if(res)break;
    	f_close(f_check);
		rval++;
		app_system_file_chgdisk(path,diskx,APP_CANCEL_PIC);
		res=f_open(f_check,(const TCHAR*)path,FA_READ);
		if(res)break;
    	f_close(f_check);
		rval++;
		app_system_file_chgdisk(path,diskx,APP_UNSELECT_PIC);
		res=f_open(f_check,(const TCHAR*)path,FA_READ);
		if(res)break;
    	f_close(f_check);
		rval++;
		app_system_file_chgdisk(path,diskx,APP_SELECT_PIC);
		res=f_open(f_check,(const TCHAR*)path,FA_READ);
		if(res)break;
    	f_close(f_check);
		rval++;
		app_system_file_chgdisk(path,diskx,APP_ASCII_S6030);
		res=f_open(f_check,(const TCHAR*)path,FA_READ);
		if(res)break;
    	f_close(f_check);
		rval++;
		app_system_file_chgdisk(path,diskx,APP_ASCII_5427);
		res=f_open(f_check,(const TCHAR*)path,FA_READ);
		if(res)break;
    	f_close(f_check);
		rval++;
		app_system_file_chgdisk(path,diskx,APP_ASCII_3618);
		res=f_open(f_check,(const TCHAR*)path,FA_READ);
		if(res)break;
    	f_close(f_check);
		rval++;
		app_system_file_chgdisk(path,diskx,APP_ASCII_2814);
		res=f_open(f_check,(const TCHAR*)path,FA_READ);
		if(res)break;
    	f_close(f_check);
		rval++;
		rval+=i;
		if(i<16)break;
		printf("\r\ntotal system files:%d\r\n",rval);
		rval=0;
		break;
	}
	gui_memin_free(f_check);
	gui_memin_free(path);
	return rval;
}

u8*const  APP_SYSTEM_APPPATH="0:/SYSTEM/APP";
u8*const  APP_SYSTEM_FONTPATH="0:/SYSTEM/FONT";
u8*const  APP_SYSTEM_SYSICOPATH="0:/SYSTEM/SYSICO";
u8*const  APP_SYSTEM_DSTPATH="1:/SYSTEM";
static u16 cpdmsg_x;
static u16 cpdmsg_y;
static u8  cpdmsg_fsize;

u8 app_boot_cpdmsg(u8*pname,u8 pct,u8 mode)
{
	if(mode&0X01)
	{
		LCD_Fill(cpdmsg_x+9*(cpdmsg_fsize/2),cpdmsg_y,cpdmsg_x+(9+16)*(cpdmsg_fsize/2),cpdmsg_y+cpdmsg_fsize,BLACK);
		LCD_ShowString(cpdmsg_x+9*(cpdmsg_fsize/2),cpdmsg_y,16*(cpdmsg_fsize/2),cpdmsg_fsize,cpdmsg_fsize,pname);
		printf("\r\nCopy File:%s\r\n",pname);
	}
	if(mode&0X04)printf("Copy Folder:%s\r\n",pname);
	if(mode&0X02)
	{
		LCD_ShowString(cpdmsg_x+(9+16+1+3)*(cpdmsg_fsize/2),cpdmsg_y,lcddev.width,lcddev.height,cpdmsg_fsize,"%");
 		LCD_ShowNum(cpdmsg_x+(9+16+1)*(cpdmsg_fsize/2),cpdmsg_y,pct,3,cpdmsg_fsize);
		printf("File Copyed:%d\r\n",pct);
	}
	return 0;
}

void app_boot_cpdmsg_set(u16 x,u16 y,u8 fsize)
{
	cpdmsg_x=x;
	cpdmsg_y=y;
	cpdmsg_fsize=fsize;
}

u8 app_system_update(u8(*fcpymsg)(u8*pname,u8 pct,u8 mode))
{
	u32 totsize=0;
	u32 cpdsize=0;
	u8 res=0;
	totsize+=exf_fdsize((u8*)APP_SYSTEM_APPPATH);
	totsize+=exf_fdsize((u8*)APP_SYSTEM_FONTPATH);
	totsize+=exf_fdsize((u8*)APP_SYSTEM_SYSICOPATH);
	printf("totsize:%d\r\n",totsize);
	f_mkdir((const TCHAR *)APP_SYSTEM_DSTPATH);
 	res=exf_fdcopy(fcpymsg,(u8*)APP_SYSTEM_APPPATH,(u8*)APP_SYSTEM_DSTPATH,&totsize,&cpdsize,1);
	if(res)
	{
		if(res!=0xff)return 1;
		else return 0XFF;
	}
	res=exf_fdcopy(fcpymsg,(u8*)APP_SYSTEM_FONTPATH,(u8*)APP_SYSTEM_DSTPATH,&totsize,&cpdsize,1);
	if(res)
	{
		if(res!=0xff)return 1;
		else return 0XFF;
	}
 	res=exf_fdcopy(fcpymsg,(u8*)APP_SYSTEM_SYSICOPATH,(u8*)APP_SYSTEM_DSTPATH,&totsize,&cpdsize,1);
	if(res)
	{
		if(res!=0xff)return 1;
		else return 0XFF;
	}
	return 0;
}

void app_getstm32_sn(u32 *sn0,u32 *sn1,u32 *sn2)
{
	*sn0=*(vu32*)(0x1FFFF7E8);
	*sn1=*(vu32*)(0x1FFFF7EC);
	*sn2=*(vu32*)(0x1FFFF7F0);
}

void app_usmart_getsn(void)
{
	u32 sn0,sn1,sn2;
	app_getstm32_sn(&sn0,&sn1,&sn2);
	printf("\r\nSerial Number:%X%X%X\r\n",sn0,sn1,sn2);
}

void app_get_version(u8*buf,u32 ver,u8 len)
{
	u8 i=0;
	buf[i++]='V';
	if(len==1)
	{
		buf[i++]=ver%10+'0';
	}else if(len==2)
	{
		buf[i++]=(ver/10)%10+'0';
		buf[i++]='.';
		buf[i++]=ver%10+'0';
	}else
	{
		buf[i++]=(ver/100)%10+'0';
		buf[i++]='.';
		buf[i++]=(ver/10)%10+'0';
		buf[i++]=ver%10+'0';
	}
	buf[i]=0;
}

u8 app_system_parameter_init(void)
{
	sysset_read_para(&systemset);
	calendar_read_para(&alarm);

	if(systemset.saveflag!=0X0A&&systemset.syslanguage>2)
	{
		systemset.syslanguage=0;
		systemset.picmode=0;
		systemset.saveflag=0X0A;
		sysset_save_para(&systemset);
 	}
	if(systemset.picmode>1)
	{
		systemset.picmode=0;
		sysset_save_para(&systemset);
	}
	if(alarm.saveflag!=0X0A)
	{
		alarm.weekmask=0;
		alarm.ringsta=0;
		alarm.hour=0;
		alarm.min=0;
 		alarm.saveflag=0X0A;
		calendar_save_para(&alarm);
 	}
	f_mkdir("0:TEXT");
   	f_mkdir("0:PAINT");
 	gui_phy.language=systemset.syslanguage;
 	return 0;
}
