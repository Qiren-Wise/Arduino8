 #include <IRremote.h>//红外库
 int RECV_PIN = 2;//红外引脚声明，红外模块接arduino主板的数字2口
  IRrecv irrecv(RECV_PIN);//红外接收信号
  decode_results results; // 用于存储编码结果的对象
  int ledon = 0;//标志灯亮灭状态
  
  int Left_IN1_Motor_go = 6;     //左电机前进,电机驱动板上IN1接arduino数字6口
  int Left_IN2_Motor_back = 7;     //左电机后退，电机驱动板上IN2接arduino数字7口
  
  int Right_IN3_Motor_go = 9;    // 右电机前进，电机驱动板上IN3接arduino数字9口
  int Right_IN4_Motor_back = 8;    // 右电机后退，电机驱动板上IN4接arduino数字8口
  
  const int Echo = A5;  //超声波接收信号口
  const int Trig = A4;  //超声波信号发射口
  float Distance = 0;//超声波测量距离值

    void setup()
    {
      pinMode(Left_IN1_Motor_go,OUTPUT); // 定义电机输出口，具有pwm功能
      pinMode(Left_IN2_Motor_back,OUTPUT);// 定义电机输出口
      pinMode(Right_IN3_Motor_go,OUTPUT);// 定义电机输出口，具有pwm功能
      pinMode(Right_IN4_Motor_back,OUTPUT);// 定义电机输出口
      pinMode(13, OUTPUT);//定义LED灯输出口
      Serial.begin(9600);  //串口波特率设置为9600
      irrecv.enableIRIn(); // 红外信号初始化
      pinMode(Echo, INPUT);    // 定义超声波输入脚
      pinMode(Trig, OUTPUT);   // 定义超声波输出
    }


    void run()     //小车前进（左右轮同时前进）
    {
      digitalWrite(Right_IN3_Motor_go,HIGH);  // 小车右轮前进
      digitalWrite(Right_IN4_Motor_back,LOW);     
      digitalWrite(Left_IN1_Motor_go,HIGH);  // 小车左轮前进
      digitalWrite(Left_IN2_Motor_back,LOW);
    }

    void stop()         //小车停止（左右轮同时不动）
    {
      digitalWrite(Right_IN3_Motor_go,LOW);// 小车右轮不动
      digitalWrite(Right_IN4_Motor_back,LOW);
      digitalWrite(Left_IN1_Motor_go,LOW);// 小车左轮不动
      digitalWrite(Left_IN2_Motor_back,LOW);
    }

    void left()         //小车左转(左轮不动，右轮前进)
    {
      digitalWrite(Right_IN3_Motor_go,HIGH);  // 小车右轮前进
      digitalWrite(Right_IN4_Motor_back,LOW);
      digitalWrite(Left_IN1_Motor_go,LOW);   //小车左轮不动
      digitalWrite(Left_IN2_Motor_back,LOW);
    }


    void right()        //小车右转(右轮不动，左轮前进)
    {
      digitalWrite(Right_IN3_Motor_go,LOW);   //小车右轮不动
      digitalWrite(Right_IN4_Motor_back,LOW);
      digitalWrite(Left_IN1_Motor_go,HIGH); //小车左轮前进
      digitalWrite(Left_IN2_Motor_back,LOW);
    }

    void back()          //小车后退
    {
      digitalWrite(Right_IN3_Motor_go,LOW);  //小车右轮后退
      digitalWrite(Right_IN4_Motor_back,HIGH);
       digitalWrite(Left_IN1_Motor_go,LOW);  //小车左轮后退
      digitalWrite(Left_IN2_Motor_back,HIGH);  
    }
    
    void get_Distance()   // 超声波测前方障碍物距离
    {
       // 产生一个10us的高脉冲去触发TrigPin
      digitalWrite(Trig, LOW); 
      delayMicroseconds(2);//延时2ms
      digitalWrite(Trig, HIGH);  
      delayMicroseconds(10);//延时10ms
      digitalWrite(Trig, LOW);   
      Distance = pulseIn(Echo, HIGH)/58;  // 检测脉冲宽度，测量距离变量(单位：微秒)
      Serial.print(Distance);//串口打印距离值
    }  

    void loop()
    {
      get_Distance();//测量前方障碍物距离
          if(Distance < 20){//数值为碰到障碍物的距离，可以按实际情况设置，这里40单位为cm
            back();//小车后退
            delay(500);
            stop();//停车 
          }
          
           if (irrecv.decode(&results)) //是否接收到红外遥控信号
          {
            Serial.println(results.value, HEX);//串口打印红外信号编码值
            digitalWrite(13, ledon ? HIGH : LOW);//当接收到红外遥控信号时候指示灯闪烁
            if (results.value == 0xFF629D )//按键标号为 CH
             {
              run();
             }//小车前进
            if (results.value == 0xFFA857 )//按键标号为 +
              
               { back();
               } //小车后退
            if (results.value == 0xFF22DD )//按键标号为 |<<
              {
                left();
              } //小车左转
            if (results.value == 0xFFC23D )//按键标号为 >||
              {
                right();
              } //小车右转
            if (results.value == 0xFF02FD )//按键标号为 >>|
              {
                stop();
              } //小车停车    
           irrecv.resume(); // 接收下一个编码
          }
    }
