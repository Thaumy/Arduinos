//基本类库
#include <string.h>
#include <stdio.h>
//LCD类库
#include <LCD12864RSPI.h>
//LCD字符处理宏
#define _( a ) sizeof( a ) / sizeof( a[0] )
//DS1302类库
#include <DS1302.h>

//LCD12864 UI文本
const unsigned char ALM[] = "ALM";
const unsigned char off[] = "[off]";
const unsigned char on[] = " [on]";
const unsigned char longgg[] = "|";

const unsigned char al_ui1[] = "01:10";
const unsigned char al_ui2[] = "05:00";

//闹钟信息
const unsigned char al1[] = "0110";
const unsigned char al2[] = "0500";
unsigned char almTime[5];

//闹钟音乐
int bz=11;
int de=100;

//闹钟开关按钮逻辑参数
int val = 0;
int old_val = 0;
int state = 0;

//闹钟选择按钮逻辑参数
int val2 = 0;
int old_val2 = 0;
int state2 = 0;

//创建DS1302对象
DS1302 dst(5, 6, 7);//RST  IO  SCLK

//串口数据缓存
char buf[27];
char day[5];
unsigned char year[5];
unsigned char week[5];
unsigned char times[9];
unsigned char month[6];
unsigned char nowTime[5];
String comdata;
int numdata[7] = {0}, j = 0, mark = 0;

void setup() {
  Serial.begin(9600);
  dst.write_protect(false);
  dst.halt(false);

  //LCD12864初始化

  LCD.PinSet(10, 8, 9);
  LCD.Initialise();

  pinMode(4, INPUT);
  pinMode(3, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  pinMode(bz, OUTPUT);
}

void loop() {
  
  digitalWrite(12, LOW);
  
  LCD.DisplayString(0, 0, year, _(year));
  LCD.DisplayString(0, 4, longgg, _(longgg));
  LCD.DisplayString(0, 6, week, _(week));
  LCD.DisplayString(1, 2, times, _(times));
  LCD.DisplayString(2, 3, month, _(month));
  LCD.DisplayString(3, 0, ALM, _(ALM));

  LCD_light();
  AlmRang();
  AlmOpenBtn();
  AlmSettingBtn();
  TimeOutput_Serial();
  
  Time t = dst.time();
  snprintf(week, sizeof(week), "%s", day);
  snprintf(year, sizeof(year), "%04d", t.yr);
  snprintf(month, sizeof(month), "%02d/%02d", t.mon, t.date);
  snprintf(times, sizeof(times), "%02d:%02d:%02d", t.hr, t.min, t.sec);
  snprintf(nowTime, sizeof(nowTime), "%02d%02d", t.hr, t.min);
  
  TimeSetter();
}


void AlmRang()
{
  int A1=221;
  int A2=248;
  int A3=278;
  int A4=294;
  int A5=330;
  int A6=371;
  int A7=416;
  if(state == 1){
    if(strcmp(nowTime, almTime) == 0)
    {
      tone(bz,A1);
      delay(de);
      tone(bz,A2);
      delay(de);
      tone(bz,A3);
      delay(de);
      tone(bz,A4);
      delay(de);
      delay(de);
      tone(bz,A5);
      delay(de);
      tone(bz,A4);
      delay(de);
      tone(bz,A7);
      delay(de);
    }else
    {
       noTone(bz);
    }
    }else
    {
       noTone(bz);
    }
}

void AlmOpenBtn()
{
  val = digitalRead(4);
  if ((val == HIGH) && (old_val == LOW))
  {
    state = 1 - state;
  }
  old_val = val;
  if (state == 1)
  {
    LCD.DisplayString(3, 5, on, _(on));
  } else
  {
    LCD.DisplayString(3, 5, off, _(off));
  }
}

void AlmSettingBtn()
{
  val2 = digitalRead(3);
  if ((val2 == HIGH) && (old_val2 == LOW))
  {
    state2 = 1 - state2;
  }
  old_val2 = val2;
  if (state2 == 1)
  {
    LCD.DisplayString(3, 2, al_ui2, _(al_ui2));
    for(int i=0;i<5;i++)
    {
      almTime[i]=al2[i];
    }
    
  } else
  {
    LCD.DisplayString(3, 2, al_ui1, _(al_ui1));
    for(int i=0;i<5;i++)
    {
      almTime[i]=al1[i];
    }
    
  }
}

void LCD_light()
{
  if (analogRead(A2) > 0)
  {
    digitalWrite(3, HIGH);
  } else {
    digitalWrite(3, LOW);
  }
}

void TimeSetter()
{
  /* 当串口有数据的时候，将数据拼接到变量comdata */
  while (Serial.available() > 0)
  {
    comdata += char(Serial.read());
    delay(2);
    mark = 1;
  }
  /* 以逗号分隔分解comdata的字符串，分解结果变成转换成数字到numdata[]数组 */
  if (mark == 1)
  {
    Serial.print("You inputed : ");
    Serial.println(comdata);
    for (int i = 0; i < comdata.length() ; i++)
    {
      if (comdata[i] == ',' || comdata[i] == 0x10 || comdata[i] == 0x13)
      {
        j++;
      }
      else
      {
        numdata[j] = numdata[j] * 10 + (comdata[i] - '0');
      }
    }
    /* 将转换好的numdata凑成时间格式，写入DS1302 */
    Time t(numdata[0], numdata[1], numdata[2], numdata[3], numdata[4], numdata[5], numdata[6]);
    dst.time(t);
    mark = 0; j = 0;
    /* 清空 comdata 变量，以便等待下一次输入 */
    comdata = String("");
    /* 清空 numdata */
    for (int i = 0; i < 7 ; i++) numdata[i] = 0;
  }


  //2018,3,11,4,59,58,1
}

void TimeOutput_Serial()
{
  /* 从 DS1302 获取当前时间 */
  Time t = dst.time();
  /* 将星期从数字转换为名称 */
  memset(day, 0, sizeof(day));
  switch (t.day)
  {
    case 1: strcpy(day, "Sun1"); break;
    case 2: strcpy(day, "Mon2"); break;
    case 3: strcpy(day, "Tue3"); break;
    case 4: strcpy(day, "Wed4"); break;
    case 5: strcpy(day, "Thu5"); break;
    case 6: strcpy(day, "Fri6"); break;
    case 7: strcpy(day, "Sat7"); break;
  }
  /* 将日期代码格式化凑成buf等待输出 */
  snprintf(buf, sizeof(buf), "%s %04d-%02d-%02d %02d:%02d:%02d", day, t.yr, t.mon, t.date, t.hr, t.min, t.sec);
  
  /* 输出日期到串口 */
  Serial.println(buf);
}

