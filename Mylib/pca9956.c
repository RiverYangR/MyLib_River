#include "pca9956.h"


/**
  * @brief   write a byte data to slave from hardware i2c
  * @param1: dev_address:the address of slave device 
  * @param2: register: the address of internal register
  * @param3: data: the data of will write to register
  * @retval  None
  * @edit    River
  */
void WriteByteData(uint8_t dev_address,uint8_t reg_address,uint8_t data)
{   
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));  //be sure I2c is not busy
    I2C_GenerateSTART(I2C1,ENABLE);
    //check the start signal is success
    while( ! I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, dev_address, I2C_Direction_Transmitter);     //send address
    while( ! I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));  //wait ack
    I2C_SendData(I2C1,reg_address);      //send register adress select register
    while( ! I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));  //wait ack   
    I2C_SendData(I2C1,data);         //write data to regiser
    while( ! I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));  //wait ack   
    I2C_GenerateSTOP(I2C1, ENABLE);    
}

/**
  * @brief   read a byte data from slave fro hardware i2c
  * @param1: dev_address:the address of slave device 
  * @param2: register: the address of internal register
  * @retval  the data of recieved
  * @edit    River
  */
 uint8_t ReadByteData(uint8_t dev_address)//,uint8_t reg_address)
{
    uint8_t read_tmp = 0;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));  //be sure I2c is not busy
    I2C_GenerateSTART(I2C1,ENABLE);
    //check the start signal is success
    while( ! I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, dev_address, I2C_Direction_Receiver);     //send address
    while( ! I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));  //wait ack
    
    /* Disable Acknowledgement */
    I2C_AcknowledgeConfig(I2C1, DISABLE); 

	/*为了在收到最后一个字节后产生一个NACK脉冲，在读倒数第二个数据字节之后(在倒数第二个RxNE事件之后)必须清除ACK位。
       为了产生一个停止/重起始条件，软件必须在读倒数第二个数据字节之后(在倒数第二个RxNE事件之后)设置STOP/START位。
       只接收一个字节时，刚好在EV6之后(EV6_1时，清除ADDR之后)要关闭应答和停止条件的产生位。*/ 
		
    while( ! I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED));  //have data need recieve
    read_tmp = I2C_ReceiveData(I2C1);
    
    I2C_GenerateSTOP(I2C1, ENABLE);  
    /* Enable Acknowledgement to be ready for another reception */
    I2C_AcknowledgeConfig(I2C1, ENABLE);

    return read_tmp;
}
/**
  * @brief   software reset register of all device
  * @param:  None
  * @retval  None
  * @edit    River
  *****
  * @Note:the minimum wait time after software reset is 1ms.
  */
void Device_ResetRegister(void)
{    
    I2C_GenerateSTART(I2C1,ENABLE);    
    while( ! I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));//check the start signal is success
    I2C_SendData(I2C1,0x00);      //send general call address
    while( ! I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));  //wait ack
    I2C_SendData(I2C1,0x06);      //send reset commod
    while( ! I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));  //wait ack   
    I2C_GenerateSTOP(I2C1, ENABLE);   
}

/**
  * @brief   Config Mode1 and Mode2 Registers
  * @param1: dev_address:the address of slave device 
  * @param2: ModeStruct: the point of Mode parameter
  * @retval  None
  * @edit    River
  */
void ModeConfig(uint8_t dev_address,tMODE* ModeStruct)
{
    uint8_t Mode1temp = 0;
    uint8_t Mode2temp = 0;
    Mode1temp = ModeStruct->AutoIcrement << 5 |
                ModeStruct->Sleep << 4        |
                ModeStruct->Address ;
    Mode2temp = ModeStruct->Dmblink << 5  |
                ModeStruct->Clear << 4    |
                ModeStruct->Och << 3 ;
    WriteByteData(dev_address,MODE1,Mode1temp);
    WriteByteData(dev_address,MODE2,Mode2temp);
}
/**
  * @brief   the device of pca9956b initialization
  * @param   None
  * @retval  None
  * @edit    River
  */
void PCA9956B_Init(void)//(tPCA9956B_Param* Pca9956Init)
{
    tMODE ModeStructer;
    Device_ResetRegister();     //software reset
    delay_ms(5);

    //configuration Mode
    ModeStructer.AutoIcrement = NoIncrement;
    ModeStructer.Sleep = DISABLE;
    ModeStructer.Address = SUB1 | ALL_CALL;
    ModeStructer.Dmblink = DISABLE;
    ModeStructer.Dmblink = Dimming;
    ModeStructer.Clear = ENABLE;
    ModeStructer.Och = RESET;
    ModeConfig(ADDR_ALL,&ModeStructer);
	
    //config offset,this param is very improtant,if not config,it will take a lot of peak current.
    WriteByteData(ADDR_ALL,OFFSET,DELAY11);

    //config output for group driver
    uint8_t LedOutData = 0;
    LedOutData = (LDRx_BRIGHTNESS << 6) |
                 (LDRx_BRIGHTNESS << 4) |
                 (LDRx_BRIGHTNESS << 2) |
                 LDRx_BRIGHTNESS;
    WriteByteData(ADDR_ALL,LEDOUT0,LedOutData);
    WriteByteData(ADDR_ALL,LEDOUT1,LedOutData);
    WriteByteData(ADDR_ALL,LEDOUT2,LedOutData);
    WriteByteData(ADDR_ALL,LEDOUT3,LedOutData);
    WriteByteData(ADDR_ALL,LEDOUT4,LedOutData);
    WriteByteData(ADDR_ALL,LEDOUT5,LedOutData);

    //config current value
    WriteByteData(ADDR_ALL,IREFALL,0x7F);
}
/**
  * @brief   control led dimming
  * @param1: group:the group of different color 
  * @param2: value: the value of led each group
  * @retval  None
  * @edit    River
  */
void LedControl(void)
{
    
//    //config all pwm
//    WriteByteData(ADDR_ALL,PWMALL,255);  

    //config pwm
    GroupControlDimming(RED,0xFF);//CD
    GroupControlDimming(GREEN,0xFF);//7F
    GroupControlDimming(BLUE,0xFF);//32
	
    for(uint8_t i = 0;i<255;i+=1)
    {
        WriteByteData(ADDR_ALL,IREFALL,i);//0x7F
        delay_ms(100);
    }
   
}

/**
  * @brief   config led dimming with three group for different color
  * @param1: group:the group of different color 
  * @param2: value: the value of led each group
  * @retval  None
  * @edit    River
  */
void GroupControlDimming(uint8_t group,uint8_t value)
{
    if(group == GREEN)
    {
        WriteByteData(ADDR_ALL,PWM0,value);
        WriteByteData(ADDR_ALL,PWM3,value);
        WriteByteData(ADDR_ALL,PWM6,value);
        WriteByteData(ADDR_ALL,PWM9,value);
        WriteByteData(ADDR_ALL,PWM12,value);
        WriteByteData(ADDR_ALL,PWM15,value);
        WriteByteData(ADDR_ALL,PWM18,value);
        WriteByteData(ADDR_ALL,PWM21,value);
    }
    else if( group == RED)
    {
        WriteByteData(ADDR_ALL,PWM1,value);
        WriteByteData(ADDR_ALL,PWM4,value);
        WriteByteData(ADDR_ALL,PWM7,value);
        WriteByteData(ADDR_ALL,PWM10,value);
        WriteByteData(ADDR_ALL,PWM13,value);
        WriteByteData(ADDR_ALL,PWM16,value);
        WriteByteData(ADDR_ALL,PWM19,value);
        WriteByteData(ADDR_ALL,PWM22,value);
    }
    else if(group == BLUE)
    {
        WriteByteData(ADDR_ALL,PWM2,value);
        WriteByteData(ADDR_ALL,PWM5,value);
        WriteByteData(ADDR_ALL,PWM8,value);
        WriteByteData(ADDR_ALL,PWM11,value);
        WriteByteData(ADDR_ALL,PWM14,value);
        WriteByteData(ADDR_ALL,PWM17,value);
        WriteByteData(ADDR_ALL,PWM20,value);
        WriteByteData(ADDR_ALL,PWM23,value);
    }
}

//void config_current(uint8_t value)
//{
//    WriteByteData(ADDR_ALL,IREF0,value);
//    WriteByteData(ADDR_ALL,IREF1,value);
//    WriteByteData(ADDR_ALL,IREF2,value);
//    WriteByteData(ADDR_ALL,IREF3,value);
//    WriteByteData(ADDR_ALL,IREF4,value);
//    WriteByteData(ADDR_ALL,IREF5,value);
//    WriteByteData(ADDR_ALL,IREF6,value);
//    WriteByteData(ADDR_ALL,IREF7,value);
//    WriteByteData(ADDR_ALL,IREF8,value);
//    WriteByteData(ADDR_ALL,IREF9,value);
//    WriteByteData(ADDR_ALL,IREF10,value);
//    WriteByteData(ADDR_ALL,IREF11,value);
//    WriteByteData(ADDR_ALL,IREF12,value);
//    WriteByteData(ADDR_ALL,IREF13,value);
//    WriteByteData(ADDR_ALL,IREF14,value);
//    WriteByteData(ADDR_ALL,IREF15,value);
//    WriteByteData(ADDR_ALL,IREF16,value);
//    WriteByteData(ADDR_ALL,IREF17,value);
//    WriteByteData(ADDR_ALL,IREF18,value);
//    WriteByteData(ADDR_ALL,IREF19,value);
//    WriteByteData(ADDR_ALL,IREF20,value);
//    WriteByteData(ADDR_ALL,IREF21,value);
//    WriteByteData(ADDR_ALL,IREF22,value);
//    WriteByteData(ADDR_ALL,IREF23,value);
//}


/******************************* end of pca9959.c **************************/


