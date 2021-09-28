#include <Arduino.h>
#include <Wire.h>
#include <ErriezDS3231.h>

int DATA = 3;
int RCLK = 4;
int SRCLK = 5;
int temp = 0;
int flag = 0;

ErriezDS3231 ds3231;

int bi0[4]={0,0,0,0};
int bi1[4]={1,0,0,0};
int bi2[4]={0,1,0,0};
int bi3[4]={1,1,0,0};
int bi4[4]={0,0,1,0};
int bi5[4]={1,0,1,0};
int bi6[4]={0,1,1,0};
int bi7[4]={1,1,1,0};
int bi8[4]={0,0,0,1};
int bi9[4]={1,0,0,1};
int bi10[4]={0,1,0,1};

void shiftout(int arr[])
{
    for(int i = 0;i <= 3; i++)
    {
        digitalWrite(SRCLK,LOW);
        digitalWrite(DATA,arr[i]);
        digitalWrite(SRCLK,HIGH);
    }
}

void putnum(int num)
{
    switch (num)
    {
        case 0 :
            shiftout(bi0);
            break;
        case 1 :
            shiftout(bi1);
            break;
        case 2 :
            shiftout(bi2);
            break;
        case 3 :
            shiftout(bi3);
            break;
        case 4 :
            shiftout(bi4);
            break;
        case 5 :
            shiftout(bi5);
            break;
        case 6 :
            shiftout(bi6);
            break;
        case 7 :
            shiftout(bi7);
            break;
        case 8 :
            shiftout(bi8);
            break;
        case 9 :
            shiftout(bi9);
            break;
        case 10 :
            shiftout(bi10);
            break;
    }
}

void refresh1()
{
    for(int j = 0;j <= 9;j++)
    {
        for(int i = 0; i <= 3; i++)
        {
            putnum(j);
        }
        digitalWrite(RCLK,LOW);
        digitalWrite(RCLK,HIGH);
        delay(500);
    }

    for(int j = 9;j <= 0;j--)
    {
        for(int i = 0; i <= 3; i++)
        {
            putnum(j);
        }
        digitalWrite(RCLK,LOW);
        digitalWrite(RCLK,HIGH);
        delay(500);
    }
}

void refresh2()
{
    for(int i = 0;i <= 3;i++)
    {
        for(int k = 0;k <= 9;k++)
        {
source $HOME/esp-idf-v4.3.1/esp-idf-v4.3.1/export.sh
            for(int m = 0;m < i ;m++)
            {
                putnum(10);
            }
            putnum(k);
            for(int j = 0;j <= 3-i ;j++)
            {
                putnum(10);
            }
            digitalWrite(RCLK,LOW);
            digitalWrite(RCLK,HIGH);
            delay(200);
        }
    }
}

void setup() {

  pinMode(DATA,OUTPUT);
  pinMode(SRCLK,OUTPUT);
  pinMode(RCLK,OUTPUT);

  // Initialize serial port
    delay(500);
    Serial.begin(115200);

    // Initialize I2C
    Wire.begin();
    Wire.setClock(100000);
    
    // Initialize RTC
    while (!ds3231.begin()) {
        Serial.println(F("Error: DS3231 not found"));
        delay(3000);
    }

    // Set square wave out pin
    // SquareWaveDisable, SquareWave1Hz, SquareWave1024Hz, SquareWave4096Hz, SquareWave8192Hz
    ds3231.setSquareWave(SquareWaveDisable);
    
    digitalWrite(DATA,LOW);
    digitalWrite(SRCLK,LOW);
    digitalWrite(RCLK,LOW);
}

void loop() {

    int HFD;
    int HSD;
    int MFD;
    int MSD;
    int SFD;
    int SSD;

    struct tm dt;

    ds3231.read(&dt);

    HFD = dt.tm_hour / 10;
    HSD = dt.tm_hour % 10;
    MFD = dt.tm_min / 10;
    MSD = dt.tm_min % 10;
    SSD = dt.tm_sec / 10;
    SSD = dt.tm_sec % 10;

    if((MSD != temp) && (MSD == 0 || MSD == 5))
    {
        if(flag == 0)
        {
            refresh1();
            flag = 1;
        }
        else
        {
            refresh2();
            flag = 0;
        }
    }
    
    temp = MSD;
    putnum(SSD);
    putnum(SFD);
    putnum(10);
    putnum(SFD);
    putnum(SFD);
    putnum(10);
    putnum(SFD);
    putnum(SFD);

} 
