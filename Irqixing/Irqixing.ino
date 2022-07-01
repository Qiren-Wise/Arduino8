/*

 http://shop34791330.taobao.com/ //德飞莱旗舰店
 http://qixingchong.tmall.com/   //七星虫旗舰店
 http://www.doflye.net //德飞莱技术论坛
 http://www.doflye.net/forum-98-1.html //德飞莱论坛arduino版块
 http://www.langyidianzi.com //朗译科技
 http://qixingchong.top  //七星虫商城
 
 */
#include<math.h>
#include <IRremote.h>
#include <Servo.h>
//定义引脚
#define MOTORADJUSTMENT
int RECV_PIN = 2;

#define IR_CODE_FORWARD 0x00FF02FD
#define IR_CODE_BACKWARDS 0x00FF9867
#define IR_CODE_TURN_LEFT 0x00FFE01F
#define IR_CODE_TURN_RIGHT 0x00FF906F
#define IR_CODE_CONTINUE -1

Servo myservo;  //
int ServoPin = 10;
int pos = 0;    // 用于存储舵机位置的变量

int MotorAdjustmengPin = A1;

const int DIR1_RIGHT = 12;  //2路直流电机方向控制引脚
const int DIR2_RIGHT = 11;
const int DIR1_LEFT = 8;
const int DIR2_LEFT = 9;

const int PWM_LEFT = 6;  //2路电机调速引脚
const int PWM_RIGHT = 5;

IRrecv irrecv(RECV_PIN);
decode_results results;

boolean isActing=false;  //布尔
long timer;
const long TIME_OUT=150;


//初始化端口
void setup()
{
  // 初始化led引脚为输出
  pinMode(DIR1_RIGHT, OUTPUT);
  pinMode(DIR2_RIGHT, OUTPUT);
  pinMode(DIR1_LEFT, OUTPUT);
  pinMode(DIR2_LEFT, OUTPUT);

  pinMode(PWM_LEFT, OUTPUT);
  pinMode(PWM_RIGHT, OUTPUT);

  myservo.attach(ServoPin);  // 舵机控制信号引脚
  myservo.write(90);

  irrecv.enableIRIn();//开启红外接收
  Serial.begin(9600); //调试使用，可以去掉

}
//电机控制子程序
void motorsWrite(int speedLeft,int speedRight)
{
  float motorAdjustment = MotorAdjustment();
  if(motorAdjustment<0)
  {
    speedRight*=(1+motorAdjustment);
  }
  else
  {
    speedLeft*=(1-motorAdjustment);
  }
  if(speedRight>0) //如果PWM数值小于0，表示运行方向变化
  {
    digitalWrite(DIR1_RIGHT, 0);
    digitalWrite(DIR2_RIGHT, 1);
  }
  else
  {
    digitalWrite(DIR1_RIGHT, 1);
    digitalWrite(DIR2_RIGHT, 0);
  }
  analogWrite(PWM_RIGHT, abs(speedRight));

  if(speedLeft > 0)
  {
    digitalWrite(DIR1_LEFT,0);
    digitalWrite(DIR2_LEFT,1);
  }
  else
  {
    digitalWrite(DIR1_LEFT,1);
    digitalWrite(DIR2_LEFT,0);
  }
  analogWrite(PWM_LEFT, abs(speedLeft));
}
//主循环
void loop()
{

  if (irrecv.decode(&results))
  {
    Serial.println(results.value, HEX);
    processResult(results.value);
    irrecv.resume(); // 接收下一个数值
  }
  //运行时间，如果没有接到其他指令，停止运行
  if(isActing && (millis()-timer>=TIME_OUT))
  {
    stopMotor();
    isActing=false;
  }
}
//停止电机
void stopMotor()
{
  motorsWrite(0,0);
}

void processResult(unsigned long res)
{
  switch(res)
  {
  case IR_CODE_FORWARD:
    changeAction(0.4,0.4);  //前进
    break;
  case IR_CODE_BACKWARDS:
    changeAction(-0.3,-0.3);  //后退
    break;
  case IR_CODE_TURN_LEFT:
    changeAction(-0.25,0.25);  //左转
    break;
  case IR_CODE_TURN_RIGHT:
    changeAction(0.25,-0.25);  //右转
    break;
  case IR_CODE_CONTINUE:
    timer=millis();  //执行最后一次动作，然后清零时间
    break;
  }
}
//改变运行状态
void changeAction(float directionLeft, float directionRight)
{
  motorsWrite(180*directionLeft, 255*directionRight);
  timer=millis();
  isActing=true;
}
//轮子校正程序，当2个电机在同等条件下转速不同时需要校正，使小车可以走直线，如果没有预定义校正功能，返回0，表示没有校正
float  MotorAdjustment()
{
#ifdef MOTORADJUSTMENT
  float motorAdjustment=map(analogRead(MotorAdjustmengPin),0,1023,-30,30)/100.0;
  return motorAdjustment;
#else
  return 0;
#endif
}
