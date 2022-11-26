#include "bsp_i2c.h"
#include "delay.h"
 
void i2c_init(void)
{					     
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_SetBits(GPIOB ,GPIO_Pin_6 | GPIO_Pin_7); 
}

//产生IIC起始信号
void i2c_start(void)
{
		SDA_OUT();   
		I2C_SDA = 1;	  	  
		I2C_SCL = 1;
		delay_us(4);
		I2C_SDA = 0;//START:when SCL is high, DATA change form high to low 
		delay_us(4);
		I2C_SCL = 0;//钳住I2C总线，准备发送或接收数据 
}	

//产生IIC停止信号
void i2c_stop(void)
{
		SDA_OUT();
		I2C_SCL = 0;
		I2C_SDA = 0;//STOP:when SCL is high DATA change form low to high
		delay_us(4);
		I2C_SCL = 1; 
		I2C_SDA = 1;//发送I2C总线结束信号
		delay_us(4);							   	
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t i2c_wait_ack(void)
{
		uint8_t ucErrTime = 0;
		SDA_IN();      //SDA设置为输入  
		I2C_SDA = 1;
		delay_us(1);	   
		I2C_SCL = 1;
		delay_us(1);	 
		while(READ_SDA)
		{
				ucErrTime++;
				if(ucErrTime > 250)
				{
						i2c_stop();
						return 1;
				}
		}
		I2C_SCL = 0;//时钟输出0 	   
		return 0;  
} 
//产生ACK应答
void i2c_ack(void)
{
		I2C_SCL = 0;
		SDA_OUT();
		I2C_SDA = 0;
		delay_us(2);
		I2C_SCL = 1;
		delay_us(2);
		I2C_SCL = 0;
}
//不产生ACK应答		    
void i2c_nack(void)
{
		I2C_SCL = 0;
		SDA_OUT();
		I2C_SDA = 1;
		delay_us(2);
		I2C_SCL = 1;
		delay_us(2);
		I2C_SCL = 0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void i2c_send_byte(uint8_t byte)
{                        
    uint8_t i;   
		SDA_OUT(); 	    
    I2C_SCL = 0;//拉低时钟开始数据传输
    for(i = 0; i < 8; i++)
    {              
				if((byte & 0x80) >> 7)
					I2C_SDA = 1;
				else
					I2C_SDA = 0;
				byte <<= 1; 	  
				delay_us(2);   //对TEA5767这三个延时都是必须的
				I2C_SCL = 1;
				delay_us(2); 
				I2C_SCL = 0;	
				delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t i2c_read_byte(unsigned char ack)
{
		unsigned char i, read_data = 0;
		SDA_IN();//SDA设置为输入
		for(i = 0; i < 8; i++ )
		{
				I2C_SCL = 0; 
				delay_us(2);
				I2C_SCL = 1;
				read_data <<= 1;
				if(READ_SDA)
						read_data++;   
				delay_us(1); 
		}					 
		if (!ack)
				i2c_nack();//发送nACK
		else
				i2c_ack(); //发送ACK   
		return read_data;
}



