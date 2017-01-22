
#ifndef _MOTOR_H
#define _MOTOR_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "stm32f4xx.h"
#include "timer.h"
/*********************************************************************
 * TYPEDEFS
 */
/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * CONSTANTS
 */
//�����ǿ��
#define MOROT_INTENSITY_0    0    //���ֹͣ��
#define MOROT_INTENSITY_1    300
#define MOROT_INTENSITY_2    400
#define MOROT_INTENSITY_3    500  //���ȫ����
	
//ͨ���ı�TIM2->CCR3��ֵ���ı�ռ�ձȣ��Ӷ�����MOTOR������
#define MOTOR_PWM_VAL TIM2_PWM_VAL 

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 *PUBLIC FUNCTIONS DECLARE
 */

void Motor_init(void);
void Motor_ON(u16 intensity,u8 times);


/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _MOTOR_H */
