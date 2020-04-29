/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		KEA128��������
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/

#ifndef _KEA128_port_cfg_h
#define _KEA128_port_cfg_h




#define ON           		1
#define OFF  						0
#define SPEED_PERIOD		4

//----------------------------------  ���  ----------------------------------

#define AD_CH_L_1				ADC0_SE13
#define AD_CH_L__				ADC0_SE14
#define AD_CH_M_1				ADC0_SE9
#define AD_CH_R__				ADC0_SE15
#define AD_CH_R_1				ADC0_SE8
#define AD_CH_BAT				ADC0_SE11

//----------------------------------  ������  ----------------------------------

#define BUZZ						E6

//----------------------------------  ����  ----------------------------------

#define KU							A7
#define KD							B1
#define KC							F3
#define KL							B0
#define KR							F2

//--------------------------------  ���뿪��  --------------------------------

#define S1							A2
#define S2							A3
#define S3							D2
#define S4							D3

//----------------------------------  ������  ----------------------------------

#define CODER_DIR_L			H2
#define CODER_DIR_R			C5

#define CODER_L					ftm1
#define CODER_R					ftm0

//----------------------------------  PWM  ----------------------------------

#define PWM_CH_R_Z			ftm_ch0
#define PWM_CH_R_F			ftm_ch1
#define PWM_CH_L_Z			ftm_ch3
#define PWM_CH_L_F			ftm_ch2

//-----------------------------------  IRQ  -----------------------------------
//      ģ��ͨ��        �˿�      ��ѡ��Χ                   ����
#define IRQ_PIN         I5        //A5 I0 I1 I2 I3 I4 I5 I6  A5��Ҫ�ã��븴λ������ͻ��   

//-----------------------------------  UART  -----------------------------------

//      ģ��ͨ��        �˿�      ��ѡ��Χ                ����
//���л�����ʱ����سɶ��л���������ڵ���������Ϊһ��
#define UART0_RX_PIN    B0        //A2 B0
#define UART0_TX_PIN    B1        //A3 B1
                                       
#define UART1_RX_PIN    F2        //C6 F2
#define UART1_TX_PIN    F3        //C7 F3
                                       
#define UART2_RX_PIN    I0        //D6 I0
#define UART2_TX_PIN    I1        //D7 I1

//-----------------------------------  FTM  -----------------------------------

//      ģ��ͨ��        �˿�      ��ѡ��Χ                ����
#define FTM0_CH0_PIN    B2        //B2 A0
#define FTM0_CH1_PIN    B3        //B3 A1

//      ģ��ͨ��        �˿�      ��ѡ��Χ                ����
#define FTM1_CH0_PIN    H2        //H2 C4				  C4��Ҫ�ã���SWD_CLK��ͻ��
#define FTM1_CH1_PIN    C5        //E7 C5

//      ģ��ͨ��        �˿�      ��ѡ��Χ                ����
#define FTM2_CH0_PIN    F0        //C0  H0 F0              
#define FTM2_CH1_PIN    F1        //C1  H1 F1  
#define FTM2_CH2_PIN    D0        //C2  G4 D0  
#define FTM2_CH3_PIN    D1        //C3  G5 D1
#define FTM2_CH4_PIN    G6        //B4  G6  
#define FTM2_CH5_PIN    G7        //B5  G7              


//�ⲿ����ģ��ͨ��      �˿�      ��ѡ��Χ                ����
#define FTM0_COUNT_PIN  E0        //E7 A5  E0             A5��Ҫ�ã��븴λ������ͻ��                
                                           
#define FTM1_COUNT_PIN  E7        //E0 A5  E7             A5��Ҫ�ã��븴λ������ͻ��
                                           
#define FTM2_COUNT_PIN  A5        //E0 E7  A5             A5��Ҫ�ã��븴λ������ͻ��


//-----------------------------------  I2C  -----------------------------------
//���л�����ʱ����سɶ��л���������ڵ���������Ϊһ��
//      ģ��ͨ��        �˿�      ��ѡ��Χ                ����
#define I2C0_SCL_PIN    B7        //A3 B7
#define I2C0_SDA_PIN    B6        //A2 B6
                                       
#define I2C1_SCL_PIN    H4        //E1 H4
#define I2C1_SDA_PIN    H3        //E0 H3


//-----------------------------------  SPI  -----------------------------------
//      ģ��ͨ��        �˿�      ��ѡ��Χ              ����
//���л�����ʱ����سɶ��л���������ڵ��ĸ�����Ϊһ��
#define SPI0_SCK_PIN    B2       //E0 B2
#define SPI0_SOUT_PIN   B3       //E1 B3
#define SPI0_SIN_PIN    B4       //E2 B4                        
#define SPI0_PCS0_PIN   B5       //E3 B5
                                      
                                      
#define SPI1_SCK_PIN    G4       //G4 D0
#define SPI1_SOUT_PIN   G5       //G5 D1
#define SPI1_SIN_PIN    G6       //G6 D2
#define SPI1_PCS0_PIN   G7       //G7 D3


//-----------------------------------  CAN  -----------------------------------
//���л�����ʱ����سɶ��л���������ڵ���������Ϊһ��   H2��E7Ϊһ��
#define CAN0_TX_PIN     C7       //E7 C7
#define CAN0_RX_PIN     C6       //H2 C6



#endif
