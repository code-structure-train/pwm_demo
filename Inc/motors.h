/*首先，我们写好构架*/
#ifndef __MOTORS_H
#define __MOTORS_H

#include <stm32l4xx_hal.h>

typedef struct stm32_motor
{
  /*调节转向所需资源*/
  GPIO_TypeDef    *dir_port;
  uint16_t        pin_1;      // l298n in1
  uint16_t        pin_2;      // l298n in2
  
  /*调节转速所需资源*/
  TIM_HandleTypeDef  *tim;    // 定时器
  uint8_t            channel; // 定时器的通道
} stm32_motor; //这样写，我们就定义了一个叫“stm32_motor”的结构体类型

typedef enum{
  MOTORS_STOP = 0,
  GO_FRONT,
  GO_BACK,
  TURN_LEFT,
  TURN_RIGHT
}MOTOR_STATUS;

/*这两个函数要通过 main() 调用，因此放在这里声明*/
void Motors_Init();
void update_motors(MOTOR_STATUS *status);
#endif
