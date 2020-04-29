/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		KEA128_adc
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/


#include "KEA128_adc.h"




//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC��ʼ��
//  @param      adcn_ch         ѡ��ADCͨ��
//  @param      bit             ѡ�񾫶�ADC_8bit
//  @return     void
//  @since      v2.0
//  Sample usage:               adc_init(ADC0_SE8, ADC_8bit);
//-------------------------------------------------------------------------------------------------------------------
void adc_init(ADCn_Ch adcn_ch, ADC_nbit bit)
{
    SIM->SCGC |= SIM_SCGC_ADC_MASK;         //����ADCʱ��
    
    ADC->APCTL1 |= 1<<adcn_ch;              //ʹ��ADC����
	
	ADC->SC3 = (0
                | ADC_SC3_ADIV(0)           //��Ƶϵ��
                | ADC_SC3_MODE(bit)         //�ֱ���
                | ADC_SC3_ADICLK(0)         //ʹ������ʱ����ΪADC��ʱ��Դ
                );
    
    ADC->SC2 = ADC_SC2_REFSEL(0);           //��׼��ѹѡ��
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC�ɼ�
//  @param      adcn_ch         ѡ��ADCͨ��
//  @param      times           ADC���ת���Ĵ���
//  @return     void
//  @since      v2.0
//  Sample usage:               uint16 dat = adc_once(ADC0_SE8, 32);//�ɼ�ADC0_SE8��ѹ���ɼ�32��ȡƽ��ֵ
//  @by         llm
//-------------------------------------------------------------------------------------------------------------------
uint16 adc_read(ADCn_Ch adcn_ch, uint16 times)
{
    uint16 result = 0, temp_times;
	temp_times = times;
	
    while(temp_times --)
	{
		ADC->SC1 = ADC_SC1_ADCH(adcn_ch);       //����ת��
		
		while(!(ADC->SC1 & ADC_SC1_COCO_MASK)); //�ȴ�ת�����
		result += (ADC->R & ADC_R_ADR_MASK);
//		ADC->SC1 &= ~ADC_SC1_COCO_MASK;
	}
	
    return (uint16)(result / times);       //���ؽ��
}

uint16 adc_once(ADCn_Ch adcn_ch)
{
	ADC->SC1 = ADC_SC1_ADCH(adcn_ch);       //����ת��
    
    while(!(ADC->SC1 & ADC_SC1_COCO_MASK)); //�ȴ�ת�����
    return (ADC->R & ADC_R_ADR_MASK);       //���ؽ��
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADCֹͣ�ɼ�
//  @param      adcn_ch         ѡ��ADCͨ��
//  @param      bit             ѡ�񾫶�ADC_8bit��ADC_10bit��ADC_12bit
//  @return     void
//  @since      v2.0
//  Sample usage:               adc_stop(adc0);
//-------------------------------------------------------------------------------------------------------------------
void adc_stop(void)
{
    ADC->SC1 = ADC_SC1_ADCH(0xFF); 
}
