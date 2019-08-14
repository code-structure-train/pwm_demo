#include "motors.h"

extern TIM_HandleTypeDef htim4; // 这里使用 extern 是说 htim4 不在 motors.c 里定义,要到别处找它的定义

/*定义两个马达*/
stm32_motor motor_1;
stm32_motor motor_2;

uint16_t speed_1;
uint16_t speed_2;

/*通过 static 声明这些函数为静态函数,限定作用域在本文件内,防止跟其他文件有重名函数*/
static void spin(stm32_motor *motor,uint16_t speed,uint8_t inv);
static void go_front(void);
static void go_back(void);
static void turn_left(void);
static void turn_right(void);

void Motors_Init()
{
  /*第一个函数,启动定时器*/
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);
  
  motor_1 = (stm32_motor){.dir_port = GPIOB,
                          .pin_1    = GPIO_PIN_10,
                          .pin_2    = GPIO_PIN_11,
                          .tim      = &htim4,
                          .channel  = TIM_CHANNEL_3};
  motor_2 = (stm32_motor){.dir_port = GPIOC,
                          .pin_1    = GPIO_PIN_4,
                          .pin_2    = GPIO_PIN_7,
                          .tim      = &htim4,
                          .channel  = TIM_CHANNEL_4};                          
}

void spin(stm32_motor *motor,uint16_t speed,uint8_t inv)
{
  if(speed <= 30){
    HAL_GPIO_WritePin(motor->dir_port, motor->pin_1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(motor->dir_port, motor->pin_2, GPIO_PIN_RESET);
  } else if(inv == 0) {
    HAL_GPIO_WritePin(motor->dir_port, motor->pin_1, GPIO_PIN_SET);
    HAL_GPIO_WritePin(motor->dir_port, motor->pin_2, GPIO_PIN_RESET);
  } else if(inv != 0) {
    HAL_GPIO_WritePin(motor->dir_port, motor->pin_1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(motor->dir_port, motor->pin_2, GPIO_PIN_SET);
  }
  
  /*第二个函数,给 pwm 高电平时间赋值*/
  __HAL_TIM_SET_COMPARE(motor->tim,motor->channel,speed);
}

void go_front()
{
  spin(&motor_1,speed_1,1);
  spin(&motor_2,speed_2,1);
}

void go_back()
{
  spin(&motor_1,speed_1,0);
  spin(&motor_2,speed_2,0);
}

void turn_left()
{
  spin(&motor_1,speed_1,0);
  spin(&motor_2,speed_2,1);
}

void turn_right()
{
  spin(&motor_1,speed_1,1);
  spin(&motor_2,speed_2,0);
}

void update_motors(MOTOR_STATUS *status)
{
  /*为了安全,先验证指针是否为空,如果为空直接反回*/
  if (status == NULL)
    return;
  switch(*status){
    case MOTORS_STOP:
    {
      spin(&motor_1, 0, 0);
      spin(&motor_2, 0, 0);
      break;
    }
    case GO_FRONT:
    {
      go_front();
      break;
    }
    case GO_BACK:
    {
      go_back();
      break;
    }
    case TURN_LEFT:
    {
      turn_left();
      break;
    }
    case TURN_RIGHT:
    {
      turn_right();
      break;
    }
  }
}
