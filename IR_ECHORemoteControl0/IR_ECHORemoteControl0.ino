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
#define IR_CODE_MODE2 0x00FF18E7
#define IR_CODE_CONTINUE -1

#define S_LIMIT_1 18 //距离判断参数预设值
#define S_LIMIT_3 8 //距离判断参数预设值
#define S_LIMIT_2 100
long timer,runtime;
const long TIME_OUT=5000;

Servo myservo;  //
int ServoPin = 10;
int pos = 0;    // 用于存储舵机位置的变量


int Bit_Front,Bit_Left,Bit_Right;

int MotorAdjustmengPin = A1;

//定义引脚
const int trig = 4;    // 触发信号
const int echo = 7;    // 反馈信号

const int DIR1_RIGHT = 12;  //2路直流电机方向控制引脚
const int DIR2_RIGHT = 11;
const int DIR1_LEFT = 8;
const int DIR2_LEFT = 9;

const int PWM_LEFT = 6;  //2路电机调速引脚
const int PWM_RIGHT = 5;

IRrecv irrecv(RECV_PIN);
decode_results results;

boolean isActing=false;  
long timer1;
const long TIME_OUT1=150;


//初始化端口
void setup()
{
  //触发端口设置为输出，反馈端口设置为输入
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);
  
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

int ranging()
{
  long IntervalTime=0; //定义一个时间变量
  digitalWrite(trig, 1);//置高电平
  delayMicroseconds(15);//延时15us
  digitalWrite(trig, 0);//设为低电平
  IntervalTime=pulseIn(echo, HIGH);//用自带的函数采样反馈的高电平的宽度，单位us
  int S=IntervalTime/58; //
  return S;
}


//主循环
void loop()
{

  if (irrecv.decode(&results))
  {
    Serial.println(results.value, HEX);
    processResult(results.value);
    zizhu();
    irrecv.resume(); // 接收下一个数值
  }
  //运行时间，如果没有接到其他指令，停止运行
  if(isActing && (millis()-timer1>=TIME_OUT1))
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

void zizhu()
{
    
      int S,SL,SR;
      int TimerFlag=0;
      myservo.write(90);//正前方位置
        while(1)
          { 
                  if((millis()-timer>=TIME_OUT))//定时到更新标志位
                  {
                      TimerFlag = 1;
                  }
                myservo.write(90);//正前方位置
                delay(1000);
                S =  ranging();
                Serial.println(S);//打印数据
                if(S<=S_LIMIT_3)
                  back(1000);
                    if(S<=S_LIMIT_1 || TimerFlag == 1)//测量距离小于预设距离或者定时到时
                    {
                      timer=millis();//标志位清零，并准备重新计时
                      TimerFlag = 0; //清零标志位
                      stopMotor();   //停止小车
                      myservo.write(45); //超声波探头右转
                      delay(1000);        //延时稳定后测量
                      int SR = ranging();//获取右边的障碍距离
                      myservo.write(145); //超声波探头左转
                      delay(1500);      //延时
                      int SL = ranging();//获取左边的障碍距离
                      myservo.write(90); //探头返回中间位置
                      delay(1000);      //延时
                      int SM  = ranging();//获取距离
                      if(SL>S_LIMIT_1 && SR>S_LIMIT_1 && SM <S_LIMIT_1)//前方障碍，左右无障碍，右转
                        turnRight(300);
                      else if(SL<=S_LIMIT_1 && SR>S_LIMIT_1)//左障碍，右转
                        turnRight(300);
                      else if(SL>S_LIMIT_1 && SR<=S_LIMIT_1)//右障碍，左转
                        turnLeft(300);
                      else if(SL<=S_LIMIT_1 && SR<=S_LIMIT_1)//左右都有障碍，转弯后退
                        back(300);
                    }
                else
                  run(60); //前方无障碍，前行
    
    
                     if (results.value==0x00FF02FD)
                     {
                      
                      break;
                      }
                  
             }
            
          
  }
  
void processResult(unsigned long res)
{
  switch(res)
  {
  case IR_CODE_FORWARD:
    changeAction(0.4,0.4);  //前进
    break;
  case IR_CODE_BACKWARDS:
    changeAction(-0.4,-0.4);  //后退
    break;
  case IR_CODE_TURN_LEFT:
    changeAction(-0.3,0.3);  //左转
    break;
  case IR_CODE_TURN_RIGHT:
    changeAction(0.3,-0.3);  //右转
    break;
  case IR_CODE_MODE2:
    zizhu();            //自主避障
    break;
  case IR_CODE_CONTINUE:
    timer=millis();  //执行最后一次动作，然后清零时间
    break;
  }
}

//直行，输入0-100数字，表示百分比
void run(int pct)
{
  motorsWritePct(pct,pct);
}
//右转，并带有时间参数，调整时间可以调节转弯角度
void turnRight(int runtime)
{
  motorsWritePct(100,0);
  delay(runtime);
}
//左转，并带有时间参数，调整时间可以调节转弯角度
void turnLeft(int runtime)
{
  motorsWritePct(0,100);
  delay(runtime);  
}
//后退，并带有时间参数，调整时间可以调节转弯角度，带转弯后退。
void back(int runtime)
{
  motorsWritePct(-80,-50);
  delay(runtime); 
}

//改变运行状态
void changeAction(float directionLeft, float directionRight)
{
  motorsWrite(180*directionLeft, 255*directionRight);
  timer=millis();
  isActing=true;
}

//小车百分比输入
void motorsWritePct(int speedLeftPct, int speedRightPct)
{
  int16_t speedLeft=180*speedLeftPct/100.0;
  int16_t speedRight=255*speedRightPct/100.0;
  motorsWrite(speedLeft,speedRight);
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
