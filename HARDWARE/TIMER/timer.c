#include "timer.h"
#include "led.h"
#include "Motor.h"
#include "PWM.h"
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��

u32 guc_10mscnt, guc_timejishi, guc_timejishi1; //guc_10mscnt��ʱ���������������в����õĺ������ж�



void TIM2_INIT(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	TIM_DeInit(TIM2);
	
	TIM_TimeBaseStructure.TIM_Period = arr;						//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler = psc;					//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);				//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//����жϱ�־�������ֱ�ӽ���һ���ж�

	TIM_ITConfig( TIM2, TIM_IT_Update, ENABLE );
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;			  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);							  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM2, ENABLE); //ʹ��TIMx��
	
}


////////////////////////////////////////////////////////////////////
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		
		decel_cnt2++;
		
		if(2==fangwei_xch_dir)
		{
					
				if ((fangwei_yundong_daowei_flag == 0) && (gul_Targettimeout1_temp > 0)) //��λ������ݸ�����ķ����˶�
				{
					gl_currentPos1 +=  (ACCEL_TIME_COUNT_VALUE/t5_pwm_value)*MM_PER_STEP;
					
				}
				else if ((fangwei_yundong_daowei_flag == 0) && (gul_Targettimeout1_temp < 0))
				{
					gl_currentPos1 -=  (ACCEL_TIME_COUNT_VALUE/t5_pwm_value)*MM_PER_STEP; //������
				}			
				
				fangwei_dec_num++;//���ټ������𽥼�С��PWM�趨����װֵ�������󣬴Ӷ��ٶȱ���
				t5_pwm_value = MOTOR_PWM_ARR*(1+fangwei_dec_num/ACCEL_NUM);//ʵ����N�κ�����ֻ���ٵ�һ���ٶ�
				
				update_accel_flag =1;//����PWM��־			
				TIM5_PWM_Init( (t5_pwm_value-1) , 0 , (t5_pwm_value-1)/2);//�ٶȵݼ���PWMֵ����	
				
				if( decel_cnt2 >= ACCEL_NUM)//���ټ��ٴ���
				{
					decel_cnt2 = 0;
//					TIM_Cmd(TIM1, 0);  //�رն�ʱ��
					TIM_Cmd(TIM2, DISABLE);  //�رն�ʱ��2
					TIM_Cmd(TIM5, DISABLE);  //�رն�ʱ��5

					update_accel_flag = 0;
					
					fangwei_yundong_daowei_flag = 0;//
					gb_SHUN_NI1 = 2;
					last_gl_currentPos1 = gl_currentPos1;
					fangwei_shezhi_guocheng_flag = 0;

					if(fangwei_xch_dir == 2)//��������
					{
						fangwei_xch_dir=0;
						Yuntai_fangwei_shezhi_chenggong_flag = 1;
						Yuntai_jiaodushezhi_flag = 1;
						TIM5_PWM_Init( MOTOR_PWM_ARR-1 , 0 , (MOTOR_PWM_ARR-1)/2);//ֱ�����趨�ٶ�����	
					}
					
					TIM_Cmd(TIM2, DISABLE);  //�رն�ʱ��2
					
					return;
				}		
				
		}
		
		
	}
	
}

/////////////////////////////////////////////////////////
void TIM1_UP_IRQHandler(void)
{ 
		TIM1->SR&=~(1<<0);
	
		if (guc_10mscnt < 15)
		{
			guc_10mscnt++;
		}	
		
		if ((fuyang_yundong_daowei_flag == 0) && (gul_Targettimeout_temp > 0))
		{
			gl_currentPos += ONE_MS_TIME_COUNT_VALUE*FUYANG_MM_PER_STEP;

		}
		else if ((fuyang_yundong_daowei_flag == 0) && (gul_Targettimeout_temp < 0))
		{
			gl_currentPos -=  ONE_MS_TIME_COUNT_VALUE*FUYANG_MM_PER_STEP;

		}
		
		if ((fangwei_yundong_daowei_flag == 0) && (gul_Targettimeout1_temp > 0)) //��λ������ݸ�����ķ����˶�
		{
			gl_currentPos1 +=  ONE_MS_TIME_COUNT_VALUE*MM_PER_STEP;
			
		}
		else if ((fangwei_yundong_daowei_flag == 0) && (gul_Targettimeout1_temp < 0))
		{
			gl_currentPos1 -=  ONE_MS_TIME_COUNT_VALUE*MM_PER_STEP;
		}
	

	//////////////////////��������˶���ʵʱλ�ø���///////////////////////
		if ((In_place_flag == 1) && (fuyang_shezhi_guocheng_flag == 1))//�жϵ�ǰ���Ǹ�λ�˶�
		{
			if ((guc_timejishi >= gul_Targettimeout) && (In_place_flag == 1) && (Init_first_flag == 1)) //�����ڸ�λ��ɺ�ִ��
			{
				TIM_Cmd(TIM4, DISABLE);

				if (gul_Targettimeout_temp == 0)
				{
					gl_currentPos = fuyang_TargetPos;
				}
				fuyang_yundong_daowei_flag = 1;//
				guc_timejishi = 0;
				gb_SHUN_NI = 2;
				last_gl_currentPos = gl_currentPos;
				fuyang_shezhi_guocheng_flag = 0;
		
			}
			
			guc_timejishi++;
			
		}
/////////////////////��λ����˶���ʵʱλ�ø���////////////////////////
		if ((In_place_flag1 == 1) && (fangwei_shezhi_guocheng_flag == 1))
		{
			if ((guc_timejishi1 >= gul_Targettimeout1) && (Init1_first_flag == 1)) //�����ڸ�λ��ɺ�ִ��
			{
				TIM_Cmd(TIM5, DISABLE);

			  if (gul_Targettimeout1_temp == 0)
				{
					gl_currentPos1 = fangwei_TargetPos;
				}
				
				guc_timejishi1 = 0;
				gb_SHUN_NI1 = 2;
				last_gl_currentPos1 = gl_currentPos1;
				fangwei_yundong_daowei_flag = 1; //��λ�����
				fangwei_shezhi_guocheng_flag = 0;			
				
			}
			
			guc_timejishi1++;
			
		}
		
		
		
}
