


//舵机调整180
#define SERVOLEFTNULL 2000  //数值减小，顺时针旋转，加大则逆时针旋转  
#define SERVORIGHTNULL 1000  //数值减小，顺时针旋转，加大则逆时针旋转 


//舵机调整90
#define SERVOFAKTORLEFT 630 //数值加大，顺时针旋转，减小则逆时针旋转//630
#define SERVOFAKTORRIGHT 605  //数值减小，顺时针旋转，加大则逆时针旋转//605


//升举舵机的3个角度 
#define LIFT0 1850 //落笔写字 on drawing surface
#define LIFT1 2000  //写字时抬臂动作 between numbers
#define LIFT2 2300  //高抬笔架  going towards sweeper 


//笔擦的坐标位置
int  rubberx=74 ,rubbery=43 ;   

//三只舵机的接口号
#define SERVOPINLIFT  2 //抬臂舵机
#define SERVOPINLEFT  3 //左臂舵机
#define SERVOPINRIGHT 4 //右臂舵机

// 速度  数字越小越慢（1000~200）
#define LIFTSPEED 1300 

// 悬臂的长度，根据图纸测量
#define L1 35
#define L2 57.2
#define L3 14.2

// 左右舵机轴心的位置，根据图纸测量
#define O1X 22
#define O1Y -25
#define O2X 47
#define O2Y -25


//需要的库函数 
#include <DS3231.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 and 0x3f for a 16 chars and 2 line display
//时钟模块
DS3231 Clock;  
bool h12;
bool PM;


int servoLift = LIFT2;

Servo servo1;  // 
Servo servo2;  // 
Servo servo3;  // 

volatile double lastX = rubberx;
volatile double lastY = rubbery;

int last_min = 0;

void setup() 
{ 
   Wire.begin();  
   lcd.init();                      // initialize the lcd 
   lcd.backlight();
   lcd.setCursor(0,0); 
   lcd.print("time:");
   
  //第一次运行 请设置时间
  Clock.setMinute(9);//设置分钟 
  Clock.setHour(16);  //设置小时 
 
  
  // 设置一个模拟时间，（小时，分钟，后面全填0）

  servo1.attach(SERVOPINLIFT);  //初始化抬臂舵机  lifting servo
  servo2.attach(SERVOPINLEFT);  //初始化左臂舵机  left servo
  servo3.attach(SERVOPINRIGHT); //初始化右臂舵机  right servo
  lift(2); //高抬笔
  drawTo(rubberx, rubbery); //停留在笔擦位置
  delay(1000);
} 

void loop() 
{ 
  int minute,hour; 
  minute=Clock.getMinute();
  hour=Clock.getHour(h12, PM);//读取时间
  lcd.setCursor(5,0); 
  lcd.print(hour);
  lcd.print(':');
  lcd.print(minute); 
  int i = 0;
//两种功能函数
    if (last_min != minute)  //每分钟书写一次时间
  //if  (1)                  //反复不间断的擦写模式 
  
  if (last_min != minute)
  {

    if (!servo1.attached()) servo1.attach(SERVOPINLIFT);
    if (!servo2.attached()) servo2.attach(SERVOPINLEFT);
    if (!servo3.attached()) servo3.attach(SERVOPINRIGHT);
    lift(0);   
    while ((i+1)*10 <= hour)
    {
      i++;
    }

    number(0, 3, 111, 1);
    number(2, 25, i, 0.9);  
    number(14, 25, (hour-i*10), 0.9);
    number(25, 25, 11, 0.9);
    i=0;
    while ((i+1)*10 <= minute)
    {
      i++;
    }
    number(32, 25, i, 0.9);
    number(43, 25, (minute-i*10), 0.9);

    lift(2);
    drawTo(rubberx, rubbery);
    lift(1);
    last_min = minute;

    servo1.detach();
    servo2.detach();
    servo3.detach();
  }
//    lift(0); //落笔写字 
//    number(0, 3, 111, 1);   // 檫黑板  
//    number(0, 25, 5, 1); 
//    number(14, 25, 5,1);
//    number(25, 25, 11,1);  // 打点
//    number(32, 25, 5, 1);
//    number(43, 25, 5, 1);
//    lift(2); //高抬笔
//    drawTo(rubberx, rubbery); //【此数值可能需要调节】  默认为70  44
//    lift(1);//抬笔
//    servo1.detach();
//    servo2.detach();
//    servo3.detach();
//    while(1);
} 

// Writing numeral with bx by being the bottom left originpoint. Scale 1 equals a 20 mm high font.
// The structure follows this principle: move to first startpoint of the numeral, lift down, draw numeral, lift up
//这写字函数
//自定义函数
void number(float bx, float by, int num, float scale) {

  switch (num) {

  case 0:
    drawTo(bx + 12 * scale, by + 6 * scale);
    lift(0);
    bogenGZS(bx + 7 * scale, by + 10 * scale, 10 * scale, -0.8, 6.7, 0.5);
    lift(1);
    break;
  case 1:

    drawTo(bx + 3 * scale, by + 15 * scale); //
    lift(0);
    drawTo(bx + 5 * scale, by + 20 * scale);// drawTo(bx + 10 * scale, by + 20 * scale);
    drawTo(bx + 5 * scale, by + 0 * scale);
    lift(1);
    break;
  case 2:
    drawTo(bx + 2 * scale, by + 12 * scale);
    lift(0);
    bogenUZS(bx + 8 * scale, by + 14 * scale, 6 * scale, 3, -0.8, 1);
    drawTo(bx + 1 * scale, by + 0 * scale);
    drawTo(bx + 12 * scale, by + 2 * scale);//    drawTo(bx + 12 * scale, by + 0 * scale);
    lift(1);
    break;
  case 3:
    drawTo(bx + 2 * scale, by + 17 * scale);
    lift(0);
    bogenUZS(bx + 5 * scale, by + 15 * scale, 5 * scale, 3, -2, 1);
    bogenUZS(bx + 5 * scale, by + 5 * scale, 5 * scale, 1.57, -3, 1);
    lift(1);
    break;
  case 4:
    drawTo(bx + 10 * scale, by + 0 * scale);
    lift(0);
    drawTo(bx + 10 * scale, by + 20 * scale);
    drawTo(bx + 4 * scale, by + 2 * scale); //   drawTo(bx + 2 * scale, by + 6 * scale);
    drawTo(bx + 15 * scale, by + 6 * scale);//    drawTo(bx + 12 * scale, by + 6 * scale);
    lift(1);
    break;
  case 5:
    drawTo(bx + 2 * scale, by + 5 * scale);
    lift(0);
    bogenGZS(bx + 5 * scale, by + 6 * scale, 6 * scale, -2.5, 2, 1);
    drawTo(bx + 5 * scale, by + 20 * scale);
    drawTo(bx + 12 * scale, by + 20 * scale);
    lift(1);
    break;
  case 6:
    drawTo(bx + 2 * scale, by + 10 * scale);
    lift(0);
    bogenUZS(bx + 7 * scale, by + 6 * scale, 6 * scale, 2, -4.4, 1);
    drawTo(bx + 11 * scale, by + 20 * scale);
    lift(1);
    break;
  case 7:
    drawTo(bx + 2 * scale, by + 20 * scale);
    lift(0);
    drawTo(bx + 12 * scale, by + 20 * scale);
    drawTo(bx + 2 * scale, by + 0);
    lift(1);
    break;
  case 8:
    drawTo(bx + 5 * scale, by + 10 * scale);
    lift(0);
    bogenUZS(bx + 5 * scale, by + 15 * scale, 5 * scale, 4.7, -1.6, 1);
    bogenGZS(bx + 5 * scale, by + 5 * scale, 5 * scale, -4.7, 2, 1);
    lift(1);
    break;

  case 9:
    drawTo(bx + 9 * scale, by + 11 * scale);
    lift(0);
    bogenUZS(bx + 7 * scale, by + 15 * scale, 5 * scale, 4, -0.5, 1);
    drawTo(bx + 5 * scale, by + 0);
    lift(1);
    break;

  case 111:

    lift(0);
    drawTo(rubberx, rubbery);
    drawTo(58, 42);

    drawTo(58, 45); 
    drawTo(0, 45);
    drawTo(0, 41);
    drawTo(58, 41);
    drawTo(60, 37);

    drawTo(0, 37);
    drawTo(0, 33);
    drawTo(60, 33);
    drawTo(60, 29);

    drawTo(0, 29);
    drawTo(0, 25);
    drawTo(60, 25);
    drawTo(60, 20);

    drawTo(0, 20);
    
    drawTo(60, rubbery-3);
    drawTo(rubberx+6 , rubbery-3);
    lift(2);
    drawTo(rubberx , rubbery);
    break;
  case 11:
    drawTo(bx + 5 * scale, by + 15 * scale);
    lift(0);
    bogenGZS(bx + 5 * scale, by + 15 * scale, 0.1 * scale, 1, -1, 1);
    lift(1);
    drawTo(bx + 5 * scale, by + 5 * scale);
    lift(0);
    bogenGZS(bx + 5 * scale, by + 5 * scale, 0.1 * scale, 1, -1, 1);
    lift(1);
    break;
  }
}
//抬函数，不同的摆臂高度
void lift(char lift) {
  switch (lift) {
    // room to optimize  !
  case 0: 
      if (servoLift >= LIFT0) {
      while (servoLift >= LIFT0) 
      {
        servoLift--;
        servo1.writeMicroseconds(servoLift);				
        delayMicroseconds(LIFTSPEED);
      }
    } 
    else {
      while (servoLift <= LIFT0) {
        servoLift++;
        servo1.writeMicroseconds(servoLift);
        delayMicroseconds(LIFTSPEED);
      }
    }
    break;
  case 1: 
    if (servoLift >= LIFT1) {
      while (servoLift >= LIFT1) {
        servoLift--;
        servo1.writeMicroseconds(servoLift);
        delayMicroseconds(LIFTSPEED);
      }
    } 
    else {
      while (servoLift <= LIFT1) {
        servoLift++;
        servo1.writeMicroseconds(servoLift);
        delayMicroseconds(LIFTSPEED);
      }
    }
    break;
  case 2:
    if (servoLift >= LIFT2) {
      while (servoLift >= LIFT2) {
        servoLift--;
        servo1.writeMicroseconds(servoLift);
        delayMicroseconds(LIFTSPEED);
      }
    } 
    else {
      while (servoLift <= LIFT2) {
        servoLift++;
        servo1.writeMicroseconds(servoLift);				
        delayMicroseconds(LIFTSPEED);
      }
    }
    break;
  }
}

//勾股定理
void bogenUZS(float bx, float by, float radius, int start, int ende, float sqee) {
  float inkr = -0.05;
  float count = 0;
  do {
    drawTo(sqee * radius * cos(start + count) + bx,
    radius * sin(start + count) + by);
    count += inkr;
  } 
  while ((start + count) > ende);
}

//cos？sin？
void bogenGZS(float bx, float by, float radius, int start, int ende, float sqee) {
  float inkr = 0.05;
  float count = 0;
  do {
    drawTo(sqee * radius * cos(start + count) + bx,
    radius * sin(start + count) + by);
    count += inkr;
  } 
  while ((start + count) <= ende);
}


void drawTo(double pX, double pY) {
  double dx, dy, c;
  int i;

  // dx dy of new point
  dx = pX - lastX;
  dy = pY - lastY;
  //path lenght in mm, times 4 equals 4 steps per mm
  c = floor(4 * sqrt(dx * dx + dy * dy));

  if (c < 1) c = 1;

  for (i = 0; i <= c; i++) {
    // draw line point by point
    set_XY(lastX + (i * dx / c), lastY + (i * dy / c));
  }
  lastX = pX;
  lastY = pY;
}

double return_angle(double a, double b, double c) {
  // cosine rule for angle between c and a
  return acos((a * a + c * c - b * b) / (2 * a * c));
}

//  三角函数转换舵机角度
//Plotclock by joo - Thingiverse
//http://www.thingiverse.com/thing:248009/
void set_XY(double Tx, double Ty) 
{
  delay(1);
  double dx, dy, c, a1, a2, Hx, Hy;

  // calculate triangle between pen, servoLeft and arm joint
  // cartesian dx/dy
  dx = Tx - O1X;
  dy = Ty - O1Y;

  // polar lemgth (c) and angle (a1)
  c = sqrt(dx * dx + dy * dy); // 
  a1 = atan2(dy, dx); //
  a2 = return_angle(L1, L2, c);

  servo2.writeMicroseconds(floor(((a2 + a1 - M_PI) * SERVOFAKTORLEFT) + SERVOLEFTNULL));

  // calculate joinr arm point for triangle of the right servo arm
  a2 = return_angle(L2, L1, c);
  Hx = Tx + L3 * cos((a1 - a2 + 0.621) + M_PI); //36,5掳
  Hy = Ty + L3 * sin((a1 - a2 + 0.621) + M_PI);

  // calculate triangle between pen joint, servoRight and arm joint
  dx = Hx - O2X;
  dy = Hy - O2Y;

  c = sqrt(dx * dx + dy * dy);
  a1 = atan2(dy, dx);
  a2 = return_angle(L1, (L2 - L3), c);
  servo3.writeMicroseconds(floor(((a1 - a2) * SERVOFAKTORRIGHT) + SERVORIGHTNULL));
}






