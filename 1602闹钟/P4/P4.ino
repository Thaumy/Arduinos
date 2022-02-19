
#include <stdio.h>
#include <string.h>
#include <DS1302.h>
#include <LiquidCrystal.h>

uint8_t RST_PIN   = 5;
uint8_t IO_PIN   = 6;
uint8_t SCLK_PIN = 7;

char buf[50];
char day[10];

char weekAndYear[50];
char times[50];
/* 串口数据缓存 */
String comdata = "";
int numdata[7] ={0}, j = 0, mark = 0;
/* 创建 DS1302 对象 */

DS1302 rtc(RST_PIN, IO_PIN, SCLK_PIN);

LiquidCrystal lcd(12, 11, 10, 9, 8, 13);
int t1,t2,t3,t4,st;
void setup()
{
  lcd.begin(16,2);
  lcd.clear(); //清屏
  Serial.begin(9600);
  rtc.write_protect(false);
  rtc.halt(false);
}
void timmer(){
  st=millis()/1000;
  
  t1=st%10;
  t2=st/10%10;
  if(t2==6){t2=0;}
  t3=st/60%10;
  if(t3==6){t3=0;}
  t4=st/3600%10;
}
void loop ()
{  
  /* 当串口有数据的时候，将数据拼接到变量comdata */
    while (Serial.available() > 0)
    {
        comdata += char(Serial.read());
        delay(2);
        mark = 1;
    }
    /* 以逗号分隔分解comdata的字符串，分解结果变成转换成数字到numdata[]数组 */
    if(mark == 1)
    {
        Serial.print("You inputed : ");
        Serial.println(comdata);
        for(int i = 0; i < comdata.length() ; i++)
        {
            if(comdata[i] == ',' || comdata[i] == 0x10 || comdata[i] == 0x13)
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
        rtc.time(t);
        mark = 0;j=0;
        /* 清空 comdata 变量，以便等待下一次输入 */
        comdata = String("");
        /* 清空 numdata */
        for(int i = 0; i < 7 ; i++) numdata[i]=0;
    }
    
    /* 打印当前时间 */
    print_time();
    delay(1000);
    
     timmer();
     lcd.setCursor(0, 0) ;
     lcd.print(weekAndYear);

     lcd.setCursor(0, 1) ;
     lcd.print(times);

     //2017,12,09,17,34,00,6
}

void print_time()
{
    /* 从 DS1302 获取当前时间 */
    Time t = rtc.time();
    /* 将星期从数字转换为名称 */
    memset(day, 0, sizeof(day));
    switch (t.day)
    {
    case 1: strcpy(day, "Sunday"); break;
    case 2: strcpy(day, "Monday"); break;
    case 3: strcpy(day, "Tuesday"); break;
    case 4: strcpy(day, "Wednesday"); break;
    case 5: strcpy(day, "Thursday"); break;
    case 6: strcpy(day, "Friday"); break;
    case 7: strcpy(day, "Saturday"); break;
    }
    /* 将日期代码格式化凑成buf等待输出 */
    snprintf(buf, sizeof(buf), "%s %04d-%02d-%02d %02d:%02d:%02d", day, t.yr, t.mon, t.date, t.hr, t.min, t.sec);
    snprintf(weekAndYear, sizeof(weekAndYear), "%s %04d", day, t.yr);
    snprintf(times, sizeof(times), "%02d-%02d %02d:%02d:%02d", t.mon, t.date, t.hr, t.min, t.sec);
    /* 输出日期到串口 */
    Serial.println(buf);
}


