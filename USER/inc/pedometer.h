/*******************************************************************************
Copyright 2008 - 2016 ��������ӯ��Ƽ����޹�˾. All rights reserved.
�ļ���:        pedometer.h
����   :       �Ʋ���Ӧ�ò���룬���漰�㷨
����   :       Jahol Fan
�汾   :       V1.0
�޸�   :   
������ڣ�     2016.9.25
*******************************************************************************/
#ifndef PEDOMETER_H
#define PEDOMETER_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "stm32f4xx.h"
//#include "includes.h"
#include "stepAlgorithm.h"
/*********************************************************************
 * TYPEDEFS
 */
/*********************************************************************
*  EXTERNAL VARIABLES
*/
extern OS_TMR   * pedometer_timer;     //�Ʋ�����ʱ��pedometer_timer
/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * MACROS
 */
extern sportsInfo_t userSportsInfo;
/*********************************************************************
 *PUBLIC FUNCTIONS DECLARE
 */

u8 Pedometer_init(void);
sportsInfo_t *Pedometer_getSportsInfo(void);
void Pedometer_startSoftTimer(void);
void Pedometer_stopSoftTimer(void);


/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* PEDOMETER_H */
