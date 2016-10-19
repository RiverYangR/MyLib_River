/**
  ***************************************************************
  * @file       oled.c 
  * @author     River
  * @version    v1.0
  * @date       2016-4-10
  * @brief      the dirver of oled,communication using 4-wire simulation spi protocol
  Pin conection:
    OLED_D1_SDIN(PB4)：serial data pin
    OLED_D0_SCL(PB5)：serial clock pin
    OLED_DC(PB6): data/command# control pin,but in iic mode,this pin act as SA0(Slave Address Bit0).
    OLED_RST(PB7)：reset contol,act as low status.
  **************************************************************/
#include "oled.h"
#include "oled_lib.h"

#define LCD_RST_1  GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define LCD_RST_0  GPIO_ResetBits(GPIOB,GPIO_Pin_7)

#define LCD_DC_1   GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define LCD_DC_0   GPIO_ResetBits(GPIOB,GPIO_Pin_6)

#define LCD_SCL_1  GPIO_SetBits(GPIOB,GPIO_Pin_5)
#define LCD_SCL_0  GPIO_ResetBits(GPIOB,GPIO_Pin_5)

#define LCD_SDIN_1  GPIO_SetBits(GPIOB,GPIO_Pin_4)
#define LCD_SDIN_0  GPIO_ResetBits(GPIOB,GPIO_Pin_4)

static void ScrollCmd(FunctionalState NewState);

/**
  * @brief  simulation spi write data to gram of oled        
  * @param  data:write data
  * @retval None
  * @Edit   River
  */
void LCD_WrDat(unsigned char data)
{
	unsigned char i=8;
	LCD_DC_1;
    LCD_SCL_0;
    delay_us(1);
    while(i--)
     {
       if(data&0x80) LCD_SDIN_1;
       else          LCD_SDIN_0;
       LCD_SCL_1; 
       delay_us(1);         
       LCD_SCL_0;;    
       data<<=1;
     }
}
/**
  * @brief  simulation spi write comand to oled register.        
  * @param  cmd:wrte cmd
  * @retval None
  * @Edit   River
  */
void LCD_WrCmd(unsigned char cmd)
{
	unsigned char i=8;
	LCD_DC_0;
    LCD_SCL_0;
    delay_us(1);
    while(i--)
     {
       if(cmd&0x80)	 LCD_SDIN_1;
       else          LCD_SDIN_0;
       LCD_SCL_1;
       delay_us(1);
       LCD_SCL_0;
       cmd<<=1;
     } 	
}

/**
  * @brief  Set disply start coordinate,this comand ohly use for page addressing mode.        
  * @param  x: columns for 0~127(7F)
            y: page address for 0~7
  * @retval None
  * @Edit   River
  */
void LCD_Set_Pos(unsigned char x, unsigned char y)
{ 
  	LCD_WrCmd(0xb0+y);
  	LCD_WrCmd(((x&0xf0)>>4)|0x10);
  	LCD_WrCmd(x&0x0f); 
} 

void OLED_Clear(void)
{
	unsigned char y,x;	
	for(y=0;y<8;y++)
	{
		LCD_WrCmd(0xb0+y);
		LCD_WrCmd(0x01);
		LCD_WrCmd(0x10); 
		for(x=0;x<128;x++)
			LCD_WrDat(0);
	}
}
/**
  * @brief  set oled scroll horizontal        
  * @param  direction :@eOledScrollDirection,
            start_page:0~7,
            end_page:0~7,must be larger or equal to start_page,
            gear_frame: time intervall between each scroll step in term of frame frenquency
  * @retval None
  * @Edit   River
  */
void SetHorizontalCcrolling(uint8_t direction,uint8_t start_page,uint8_t end_page,uint8_t gear_frame)
{
    LCD_WrCmd(0x26|direction);
    LCD_WrCmd(0x00);        //dummy byte
    LCD_WrCmd(start_page);
    LCD_WrCmd(gear_frame);
    LCD_WrCmd(end_page);
    LCD_WrCmd(0x00);        //dummy byte
    LCD_WrCmd(0xFF);        //dummy byte
    ScrollCmd(ENABLE);      //active scroll
}
/***************************************************************/
static void ScrollCmd(FunctionalState NewState)
{
    if(NewState == ENABLE)
    {
        LCD_WrCmd(0x2F);
    }
    else
    {
        LCD_WrCmd(0x2E);
    }
}
///***************************************************************/
//static void SetStartColumn(unsigned char d)
//{
//	LCD_WrCmd(0x00+d%16);   // Set Lower Column Start Address for Page Addressing Mode
//						    // Default => 0x00
//	LCD_WrCmd(0x10+d/16);   // Set Higher Column Start Address for Page Addressing Mode
//						    // Default => 0x10
//}
/***************************************************************/
static void SetAddressingMode(unsigned char d)
{
	LCD_WrCmd(0x20);	// Set Memory Addressing Mode
	LCD_WrCmd(d);	    // Default => 0x02
						// 0x00 => Horizontal Addressing Mode
						// 0x01 => Vertical Addressing Mode
						// 0x02 => Page Addressing Mode
}
///***************************************************************/
//static void SetColumnAddress(unsigned char a, unsigned char b)
//{
//	LCD_WrCmd(0x21);	// Set Column Address,this command is only for horizontal and vertical addressing mode.
//	LCD_WrCmd(a);	    // Default => 0x00 (Column Start Address)
//	LCD_WrCmd(b);	    // Default => 0x7F (Column End Address)
//}
/***************************************************************/
//static void SetPageAddress(unsigned char a, unsigned char b)
//{
//	LCD_WrCmd(0x22);	//Set Page Address,this command is only for horizontal and vertical addressing mode.
//	LCD_WrCmd(a);		//Default => 0x00 (Page Start Address)
//	LCD_WrCmd(b);	    //Default => 0x07 (Page End Address)
//}
/***************************************************************/
static void SetStartLine(unsigned char d)
{
	LCD_WrCmd(0x40|d);	// Set Display Start Line(row),d is 0~3F
						// Default => 0x40 (0x00)
}
/***************************************************************/
static void SetContrastControl(unsigned char d)
{
	LCD_WrCmd(0x81);	//Set Contrast Control，d is 0~FF
	LCD_WrCmd(d);		//Default => 0x7F
}
/***************************************************************/
static void Set_Charge_Pump(unsigned char d)
{
	LCD_WrCmd(0x8D);	//Set Charge Pump
	LCD_WrCmd(0x10|d);	//Default => 0x10
						//0x10 (0x00) => Disable Charge Pump
						//0x14 (0x04) => Enable Charge Pump
}
/***************************************************************/
static void Set_Segment_Remap(unsigned char d)
{
	LCD_WrCmd(0xA0|d);  //Set Segment Re-Map,can display the effect of horizontal mirroring
						//Default => 0xA0
						//0xA0 (0x00) => Column Address 0 Mapped to SEG0(Normal)
						//0xA1 (0x01) => Column Address 0 Mapped to SEG127(Mirroring)
}
/***************************************************************/
static void Set_Entire_Display(unsigned char d)
{
	LCD_WrCmd(0xA4|d);  //Set Entire Display On / Off
						//Default => 0xA4
						//0xA4 (0x00) => Normal Display
						//0xA5 (0x01) => Entire Display On
}
/***************************************************************/
static void Set_Inverse_Display(unsigned char d)
{
	LCD_WrCmd(0xA6|d);  //Set Inverse Display On/Off
						//Default => 0xA6
						//0xA6 (0x00) => Normal Display
						//0xA7 (0x01) => Inverse Display On
}
/***************************************************************/
static void Set_Multiplex_Ratio(unsigned char d)
{
	LCD_WrCmd(0xA8);	//Set Multiplex Ratio
	LCD_WrCmd(d);	    //Default => 0x3F (1/64 Duty)
}
/***************************************************************/
/**
  * @brief  Set oled on or off,switch sleep or normal mode,
           in sleep mode the power consumption is less than 10uA        
  * @param  None
  * @retval None
  * @Edit   River
  */
static void Set_Display_On_Off(unsigned char d)
{
    if(d == 1)
    {Set_Charge_Pump(0x04);}//open charge pump
    else if(d == 0)
    {Set_Charge_Pump(0x00);}//close charge pump
	LCD_WrCmd(0xAE|d);  //Set Display On/Off
						//Default => 0xAE
						//0xAE (0x00) => Display Off(sleep mode)
						//0xAF (0x01) => Display On in mormal mode
}
/***************************************************************/
//static void SetStartPage(unsigned char d)
//{
//	LCD_WrCmd(0xB0|d);	//Set Page Start Address for Page Addressing Mode
//						//Default => 0xB0 (0x00)
//}
/***************************************************************/
static void Set_Com_Scan_Direction(unsigned char d)
{
	LCD_WrCmd(0xC0|d);	//Set COM Output Scan Direction
						//Default => 0xC0
						//0xC0 (0x00) => Scan from COM0 to 63
						//0xC8 (0x08) => Scan from COM63 to 0
}
/***************************************************************/
static void Set_Display_Offset(unsigned char d)
{
	LCD_WrCmd(0xD3);    //Set Display Offset
	LCD_WrCmd(d);	    //Default => 0x00
}
/***************************************************************/
static void Set_Display_Clock(unsigned char d)
{
	LCD_WrCmd(0xD5);	//Set Display Clock Divide Ratio / Oscillator Frequency
	LCD_WrCmd(d);	    //Default => 0x80
						//D[3:0] => Display Clock Divider
						//D[7:4] => Oscillator Frequency
}
/***************************************************************/
static void Set_Precharge_Period(unsigned char d)
{
	LCD_WrCmd(0xD9);	//Set Pre-Charge Period
	LCD_WrCmd(d);		//Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
						//D[3:0] => Phase 1 Period in 1~15 Display Clocks
						//D[7:4] => Phase 2 Period in 1~15 Display Clocks
}
/***************************************************************/
static void Set_ComPinHard_Config(unsigned char d)
{
	LCD_WrCmd(0xDA);	//Set COM Pins Hardware Configuration
	LCD_WrCmd(0x02|d);	//Default => 0x12 (0x10)
						//Alternative COM Pin Configuration
						//Disable COM Left/Right Re-Map
}
/***************************************************************/
static void Set_VCOMH(unsigned char d)
{
	LCD_WrCmd(0xDB);	//Set VCOMH Deselect Level
	LCD_WrCmd(d);	    //Default => 0x20 (0.77*VCC)
}
/***************************************************************/
//static void Set_NOP(void)
//{
//	LCD_WrCmd(0xE3);	// Command for No Operation
//}
/**
  * @brief  display a char of 8*16(16 is high) typeface       
  * @param  x : x columns for 0~127.
            y : y pages for 0~7.
            data:char of display
            mode : dispay mode @eOLED_DISPLAY_MODE.
  * @retval None
  * @Edit   River
  */
static void LCD_P8x16Str(unsigned int x,unsigned char y,char data,uint8_t mode)
{
    uint8_t c;
    uint8_t dis_data;
    c = data - 32;           //字符在字库中的下标
    
    LCD_Set_Pos(x,y);
    for(uint8_t i = 0;i<8;i++)
    {
        if(mode == OLED_DISPLAY_NORMAL)
        {
            dis_data = F8X16[c*16+i];
        }
        else
        {
            dis_data = ~F8X16[c*16+i];
        }
        LCD_WrDat(dis_data);  
    }
    LCD_Set_Pos(x,y+1);
    for(uint8_t i = 0;i<8;i++)
    {
        if(mode == OLED_DISPLAY_NORMAL)
        {
            dis_data = F8X16[c*16+8+i];
        }
        else
        {
            dis_data = ~F8X16[c*16+8+i];
        }
        LCD_WrDat(dis_data); 
    }  
}
/**
  * @brief  Printf char string to screen.        
  * @param  x : x columns for 0~127.
            y : y pages for 0~7.            
            mode : dispay mode @eOLED_DISPLAY_MODE.
            *ch:pointer of char string.
  * @retval None
  * @Edit   River
  */
void OLED_Print_CharString(unsigned int x, unsigned char y, char* ch,uint8_t mode)
{
	for(uint8_t i = 0;ch[i] != '\0';i++)
    {        
        LCD_P8x16Str(x,y,ch[i],mode);
        x+=8;
        if(x>=128)
        {
            x =  0;
            y+=2;
        }
    }
    if(mode == OLED_DISPLAY_ROW_SELECT)
    {
        LCD_Set_Pos(x,y);
        for(uint8_t i=x;i<128;i++)
        {
            LCD_WrDat(0xff);
        }
        LCD_Set_Pos(x,y+1);
        for(uint8_t i=x;i<128;i++)
        {
            LCD_WrDat(0xff);
        }
    }
} 
  

/**
  * @brief  Oled_printf    128*64
  * @param  x : x columns for 0~127.
            y : y pages for 0~7.            
            mode : dispay mode @eOLED_DISPLAY_MODE.
            *fmt:pointer of format char string.
            ... ：vairable list.
  * @retval void
  */
void OLED_Printf(unsigned char x,unsigned char y,uint8_t mode,const char *fmt,...)
{
	static unsigned char last_len[8]={0,0,0,0,0,0,0,0};	
	static char LCD_BUF[128]={0}; 
	unsigned char len;
	unsigned char i;
	va_list ap;
	
	va_start(ap,fmt);
    /*使va_list 类型的ap指向可变参数表的第一个参数，传入第二个参数为省略号前
        最后一个有名字的参数，可以用于可变参数的定位。*/    
	vsprintf((char *)LCD_BUF,fmt,ap);
    /*　函数名: vsprintf
        返回值: 正常情况下返回生成字串的长度(除去\0),错误情况返回负值
        用 法: int vsprintf(char *string, char *format, va_list param);
        将param 按格式format写入字符串string中*/
	va_end(ap);
    /*释放va_list*/
	
	len=strlen((char *)LCD_BUF);
	for(i=len;i<last_len[y];i++)
	{
		LCD_BUF[i]=' ';
	}
	LCD_BUF[i]='\0';
	OLED_Print_CharString(x,y,LCD_BUF,mode);
	last_len[y]=len;
}
/**
  * @brief  Oled gpio pin config        
  * @param  None
  * @retval None
  * @Edit   River
  */
static void OLED_GPIO_Config(void)
{
    GPIO_InitTypeDef gpio;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
    
    //调试引脚，默认为NJTRST功能，需要重映射才为PB4,先开复用时钟，再配置重映射才能成功！
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

    gpio.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio); 
}

void OLED_Configuration(void)        
{       
    OLED_GPIO_Config();  
    
    LCD_SCL_1;
    //复位 
    LCD_RST_0;
    delay_ms(200);//上电序列必须的等待时间100ms
    LCD_RST_1;

    Set_Display_On_Off(0x00);		// Display Off (0x00/0x01)
    Set_Display_Clock(0x80);		// Set Clock as 100 Frames/Sec
    Set_Multiplex_Ratio(0x3F);		// 1/64 Duty (0x0F~0x3F)
    Set_Display_Offset(0x00);		// Shift Mapping RAM Counter (0x00~0x3F)
    SetStartLine(0x00);			    // Set Mapping RAM Display Start Line (0x00~0x3F)
    Set_Charge_Pump(0x04);		    // Enable Embedded DC/DC Converter (0x00/0x04)
    SetAddressingMode(0x02);		// Set Page Addressing Mode (0x00/0x01/0x02)
    Set_Segment_Remap(0x01);		// Set SEG/Column Mapping     
    Set_Com_Scan_Direction(0x08);   // Set COM/Row Scan Direction 
    Set_ComPinHard_Config(0x10);    // Set Sequential Configuration (0x00/0x10)//不懂
    SetContrastControl(0xCF);	    // Set SEG Output Current
    Set_Precharge_Period(0xF1);		// Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    Set_VCOMH(0x40);			    // Set VCOM Deselect Level
    Set_Entire_Display(0x00);		// Disable Entire Display On (0x00/0x01)
    Set_Inverse_Display(0x00);		// Disable Inverse Display On (0x00/0x01)  
    Set_Display_On_Off(0x01);		// Display On (0x00/0x01)
    OLED_Clear();
	LCD_Set_Pos(0,0); 
}
/**
  * @brief  display BMP picture,if the buffer is NULL,use this function can 
            also be used to clear or fill the specified area.         
  * @param  bmp_info : bmp information struct @tBMP_Info
  * @retval None
  * @Edit   River
  */
void OledPrintfBMP(tBMP_Info* bmp_info)
{
    uint16_t buf_index=0;
    uint8_t temp_dis_element;
    for(uint8_t width = 0;width<bmp_info->bmpWidth/8;width++)
    {
        LCD_Set_Pos(bmp_info->x,bmp_info->y+width);
        for(uint8_t lenth = 0;lenth<bmp_info->bmpLength;lenth++)
        {
            if(bmp_info->displayMode == OLED_DISPLAY_NORMAL)
            {
                if(bmp_info->pbuf == NULL) 
                    LCD_WrDat(0);
                else
                    LCD_WrDat(bmp_info->pbuf[buf_index++]); 
            }
            else
            {
                temp_dis_element = bmp_info->pbuf[buf_index++];
                temp_dis_element = ~temp_dis_element;
                if(bmp_info->pbuf == NULL)
                    LCD_WrDat(0xff);
                else
                    LCD_WrDat(temp_dis_element);
            }
        }
    }
    
}

/************************************************ end of oled.c *****************************************/

