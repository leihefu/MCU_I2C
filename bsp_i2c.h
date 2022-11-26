#ifndef __BSP_I2C_H
#define __BSP_I2C_H

#include "sys.h"

//IO方向设置
#define SDA_IN()  	{GPIOB->CRL &= 0X0FFFFFFF; GPIOB->CRL |= (uint32_t)8 << 28;}
#define SDA_OUT() 	{GPIOB->CRL &= 0X0FFFFFFF; GPIOB->CRL |= (uint32_t)3 << 28;}

//IO操作函数	 
#define I2C_SCL    	PBout(6) //SCL
#define I2C_SDA    	PBout(7) //SDA	 
#define READ_SDA   	PBin(7)  //输入SDA 

//IIC所有操作函数
void i2c_init(void);              		 
void i2c_start(void);			
void i2c_stop(void);	
uint8_t i2c_wait_ack(void);
void i2c_ack(void);					
void i2c_nack(void);
void i2c_send_byte(uint8_t byte);			
uint8_t i2c_read_byte(unsigned char ack);			
  
#endif



