#ifndef __MOTOR_H__
#define __MOTOR_H__
#include "stm32f10x.h"
#include "AMIS30543.h"
////////////////////////////////////////////////////////////
#define  MAX_SPEED   120.00 //��������ٶ�120ת/���� ���ڼӼ��ٵ���Ҫ��
//ÿ���޸�PWMʱ��Ϊ�˸�׼ȷ������С���㣬��PWM�趨ֵ���Ϊ������������ת�ٵ��²�������ֵ��û��ϵ
#define  DEC_TIME_VALUE  2.0
#define  ACCEL_NUM   10.0  //�ٶȷֶμ��ٷ�Ƭ����
#define  MOTOR_PWM_ARR          1400.0  //128ϸ�֣�200����2Ȧ/S��51200������/�롣��Ӧ72000000/51200 =1406.25��Ϊ�˼����趨������С��ȡ1400
#define  ACCEL_TIME_COUNT_VALUE   (72.0*(DEC_TIME_VALUE/ACCEL_NUM)*1000000)  //����Ƶ72Mʱ0.2S����ʱ��Ƭ��PWM�������ֵ�����ں�PWM��װֵ�Ƚϣ�����֪��ʱ��Ƭ������˶���PWM����
#define  ONE_MS_TIME_COUNT_VALUE  (72.0*1000/MOTOR_PWM_ARR)  //1msʱ�䣬PWM����û�з�Ƶ����1msʱ���߹�PWM�����������MOTOR_PWM_ARR��N��
#define  MM_PER_STEP              ((1.27/200)/128.0)
#define  STEP_COUNT_PER_MM        (128.0*200.0/1.27)    //ÿmm�������������������嵱��
#define  FUYANG_STEP_COUNT_PER_MM        (128.0*200.0/1.5875)    //ÿmm�������������������嵱��
#define  FUYANG_MM_PER_STEP              ((1.5875/200)/128.0)
////////////////////////////////////////////////////////
extern  int   WipeMotorFlag ;//�������ת��Ϊ��״̬
extern  float  SetSpeed_ref ;
extern  int  update_accel_flag ;
extern  float  NowSpeed ; //��ǰ�ٶ�
extern  float  accel_value;//���ٶ�ֵ����rpm��
extern  float   accel_num ;//���ٷֶ���
extern  float   dec_num;//���ټ���
extern  float  add_value ;
extern  float  dec_value ;
extern  u32  every_accel;
extern  u16  t4_pwm_value;//pwm4����ֵ
extern  u16  t5_pwm_value;//pwm5����ֵ

extern  float  fuyang_dec_num ; //���ټ���
extern  float  fangwei_dec_num ; //���ټ���
extern  int  fuyang_xch_dir ;//��������л������־
extern  int  fangwei_xch_dir ;//��λ����л������־
/////////////////////////////////////////////////////////////
//extern  u8 fangwei_daowei_flag;
//extern  u8 fuyang_daowei_flag;
extern double gl_currentPos;					//������ǰλ��
extern double gl_currentPos1;					//��λ��ǰλ��
extern u8 fuyang_yundong_daowei_flag;
extern u8 fangwei_yundong_daowei_flag;			//��λ�˶���λ��־
extern int gul_Targettimeout1_temp;				//��λĿ��ʱ��
extern int gul_Targettimeout_temp;				//��λĿ��ʱ��
extern double last_gl_currentPos;
extern double last_gl_currentPos1;
extern double gl_currentPos;
extern AMIS30543_CR AMIS30543_CR_SPI1;
extern AMIS30543_CR AMIS30543_CR_SPI2;
extern int gul_Targettimeout1_temp;
extern double gl_currentPos1;
extern u32 guc_timejishi1;



extern char In_place_flag;						//���ڸ���������̵ı�־
extern char In_place_flag1;						//���ڷ�λ������̵ı�־
extern u8 fuyang_shezhi_guocheng_flag;			//�������óɹ����̱�־
extern u8 fangwei_shezhi_guocheng_flag;			//��λ���óɹ���־
extern char Init1_first_flag;					//�ϵ縴λǰ�ı�־
extern u32 gul_Targettimeout1;					//��λĿ��ʱ��
extern u8 gb_SHUN_NI1;							//������λ�����ת��ת��ֹͣ
extern u8 gb_SHUN_NI;
extern double last_gl_currentPos;				//�����ϴ�ֹͣʱ��λ��
extern double last_gl_currentPos1;				//��λ�ϴ�ֹͣʱ��λ��
extern double fangwei_TargetPos;				//��λĿ��λ��
extern double fuyang_TargetPos;					//����Ŀ��λ��

extern u32 gul_Targettimeout;
extern char Init_first_flag;



extern u8 Yuntai_fangwei_shezhi_chenggong_flag; //��̨��λ���óɹ���־
extern u8 Yuntai_fuyang_shezhi_chenggong_flag;
extern u8 Yuntai_jiaodushezhi_flag;
/////////////////////////////////////////////////////////////
#define  MotorTIMPeriod     (float)(150.0/(NowSpeed/200.0))
#define  MotorHalfTIMPeriod   (float)(MotorTIMPeriod/2.0)
	
#define MOTOR_EN_PORT		GPIOA
#define MOTOR_EN_PIN		GPIO_Pin_10
#define MOTOR_DIR_PORT		GPIOA
#define MOTOR_DIR_PIN		GPIO_Pin_8

#define MOTOR_EN(x)			GPIO_WriteBit(MOTOR_EN_PORT,MOTOR_EN_PIN,(BitAction)(x))//1����״̬ 0����״̬
#define MOTOR_DIR(x)		GPIO_WriteBit(MOTOR_DIR_PORT,MOTOR_DIR_PIN,(BitAction)(x))
#define MOTOR_DIR_CH    GPIO_WriteBit(MOTOR_DIR_PORT,MOTOR_DIR_PIN, !GPIO_ReadOutputDataBit(MOTOR_DIR_PORT,MOTOR_DIR_PIN));

void MOTOR_Init(void);
//void MOTOR_SetSpeed(u16 Speed);
void Motor_SetSpeed(void);
int   Mortor_Accel( u32  SetSpeed );
int   fuyang_Mortor_Decel( u32  SetSpeed );
int   fangwei_Mortor_Decel( u32  SetSpeed );
extern u16 PulsePerRound;
extern u16 RevolveSpeed;
extern u16 OldRevolveSpeed;

#endif