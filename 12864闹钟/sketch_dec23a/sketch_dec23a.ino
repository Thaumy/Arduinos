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
unsigned char ALM[] = "ALM";
unsigned char off[] = "[off]";
unsigned char on[] = " [on]";
unsigned char a0500[] = "05:00";
unsigned char a0530[] = "05:30";
unsigned char longgg[] = "|";



//闹钟开关按钮逻辑参数
bool almState = false;
int val = 0;
int old_val = 0;
int state = 0;

//闹钟选择按钮逻辑参数
int val2 = 0;
int old_val2 = 0;
int state2 = 0;
int almTime;

//创建DS1302对象
DS1302 dst(5, 6, 7);//RST  IO  SCLK

//串口数据缓存
char buf[27];
char day[5];
unsigned char year[5];
unsigned char week[5];
unsigned char times[9];
unsigned char month[5];
String comdata;
int numdata[7] = {0}, j = 0;

void setup() {
  Serial.begin(9600);
  dst.write_protect(false);
  dst.halt(false);

  //LCD12864初始化

  LCD.PinSet(10, 8, 9);
  LCD.Initialise();

  pinMode(4, INPUT);
  pinMode(3, OUTPUT);
  pinMode(2, INPUT);
}

void loop() {

  LCD.DisplayString(0, 1, year, _(year));

  LCD.DisplayString(0, 5, week, _(week));
  LCD.DisplayString(1, 2, times, _(times));
  LCD.DisplayString(2, 3, month, _(month));
  LCD.DisplayString(3, 0, ALM, _(ALM));
  
  LCD_light();
  AlmOpenBtn();
  AlmSettingBtn();
  TimeOutput_Serial();
  TimeSetter();
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
    almState = true;
  } else
  {
    LCD.DisplayString(3, 5, off, _(off));
    almState = false;
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

void AlmSettingBtn()
{
  val2 = digitalRead(2);
  if ((val2 == HIGH) && (old_val2 == LOW))
  {
    state2 = 1 - state2;
  }
  old_val2 = val2;
  if (state2 == 1)
  {
    LCD.DisplayString(3, 2, a0500, _(a0500));
  } else
  {
    LCD.DisplayString(3, 2, a0530, _(a0530));
  }
}


void TimeSetter()
{
  /* 当串口有数据的时候，将数据拼接到变量comdata */
  while (Serial.available() > 0)
  {
    comdata += char(Serial.read());

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
    dst.time(t); j = 0;
    /* 清空 comdata 变量，以便等待下一次输入 */
    comdata = String("");
    /* 清空 numdata */
    for (int i = 0; i < 7 ; i++) numdata[i] = 0;
  }

  
  //2017,12,31,20,00,00,1
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
  snprintf(week, sizeof(week), "%s", day);
  snprintf(year, sizeof(year), "%04d", t.yr);
  snprintf(month, sizeof(month), "%02d-%02d", t.mon, t.date);
  snprintf(times, sizeof(times), "%02d:%02d:%02d", t.hr, t.min, t.sec);
  /* 输出日期到串口 */
  Serial.println(buf);
}

