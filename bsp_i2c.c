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

//����IIC��ʼ�ź�
void i2c_start(void)
{
		SDA_OUT();   
		I2C_SDA = 1;	  	  
		I2C_SCL = 1;
		delay_us(4);
		I2C_SDA = 0;//START:when SCL is high, DATA change form high to low 
		delay_us(4);
		I2C_SCL = 0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	

//����IICֹͣ�ź�
void i2c_stop(void)
{
		SDA_OUT();
		I2C_SCL = 0;
		I2C_SDA = 0;//STOP:when SCL is high DATA change form low to high
		delay_us(4);
		I2C_SCL = 1; 
		I2C_SDA = 1;//����I2C���߽����ź�
		delay_us(4);							   	
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t i2c_wait_ack(void)
{
		uint8_t ucErrTime = 0;
		SDA_IN();      //SDA����Ϊ����  
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
		I2C_SCL = 0;//ʱ�����0 	   
		return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void i2c_send_byte(uint8_t byte)
{                        
    uint8_t i;   
		SDA_OUT(); 	    
    I2C_SCL = 0;//����ʱ�ӿ�ʼ���ݴ���
    for(i = 0; i < 8; i++)
    {              
				if((byte & 0x80) >> 7)
					I2C_SDA = 1;
				else
					I2C_SDA = 0;
				byte <<= 1; 	  
				delay_us(2);   //��TEA5767��������ʱ���Ǳ����
				I2C_SCL = 1;
				delay_us(2); 
				I2C_SCL = 0;	
				delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t i2c_read_byte(unsigned char ack)
{
		unsigned char i, read_data = 0;
		SDA_IN();//SDA����Ϊ����
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
				i2c_nack();//����nACK
		else
				i2c_ack(); //����ACK   
		return read_data;
}



