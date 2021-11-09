#include <SPI.h>
#include <Kangaroo.h>
#include <Adafruit_ADS1X15.h>
#include <RoboClaw.h>

#define MAX 0
#define MIN 1
#define LIMIT 2

int dist1[2] = {0, 20000};
int dist2[2] = {0, 20000};
int dist3[2] = {0, 20000};
int dist4[2] = {0, 20000};
uint32_t adc0;
byte button = 0;
uint8_t counter = 0;
bool flag = false;

int val1 = 0;
int val2 = 0;
int val3 = 0;
int val4 = 0;

//Objects:
KangarooSerial  K(Serial1);
KangarooChannel K1(K, '1');
KangarooChannel K2(K, '2');

Adafruit_ADS1115 ads;

RoboClaw roboclaw(&Serial2, 10000);
#define address  0x80

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);
  Serial2.begin(38450);
  //  Serial3.begin(115200);
  roboclaw.begin(38450);

  pinMode(LIMIT, INPUT_PULLUP);

  K1.start();
  K1.home().wait();

  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE);
  SPI.attachInterrupt();

  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS."); while (1);
  }

  while (digitalRead(LIMIT)) {
    K1.s(100);
  }
  K1.s(0);
  long encoder_val =  K1.getP().value() - 180;
  K1.p(K1.getP().value() - 180 , 45);


  while (encoder_val !=  K1.getP().value())
  {
    adc0 = ads.readADC_SingleEnded(0);
    //    Serial.println(adc0);
    if (adc0 > 20000)
    {
      flag = false;
    }
    if (adc0 < 20000 && !flag)
    {
      flag = true;
      counter = counter + 1;
    }
    switch (counter)
    {
      case 1:
        if (adc0 > 45  && adc0 < 20000) {
          if (adc0 < dist1[MIN]) {
            dist1[MIN] = adc0;
            val1 = K1.getP().value();
            //            Serial.println(val1);
          }
          if (adc0 > dist1[MAX]) {
            dist1[MAX] = adc0;
          }
        }
        break;
      case 2: if (adc0 > 45  && adc0 < 20000) {
          if (adc0 < dist2[MIN]) {
            dist2[MIN] = adc0;
            val2 = K1.getP().value();
            //            Serial.println(val2);
          }
          if (adc0 > dist2[MAX]) {
            dist2[MAX] = adc0;
          }
        }
        break;
      case 3: if (adc0 > 45  && adc0 < 20000) {
          if (adc0 < dist3[MIN]) {
            dist3[MIN] = adc0;
            val3 = K1.getP().value();
            //            Serial.println(val3);
          }
          if (adc0 > dist3[MAX]) {
            dist3[MAX] = adc0;
          }
        }
        break;
      case 4: if (adc0 > 45  && adc0 < 20000) {
          if (adc0 < dist4[MIN]) {
            dist4[MIN] = adc0;
            val4 = K1.getP().value();
            //            Serial.println(val4);
          }
          if (adc0 > dist4[MAX]) {
            dist4[MAX] = adc0;
          }
        }
        break;
      default:
        break;
    }

//        Serial.print("C: "); Serial.print(counter);
//        Serial.print(" || "); Serial.print(dist1[MIN]);
//        Serial.print(" || "); Serial.print(dist2[MIN]);
//        Serial.print(" || "); Serial.print(dist3[MIN]);
//        Serial.print(" || "); Serial.println(dist4[MIN]);
  }
  //  Serial.print(val1); Serial.print("   ");
  //  Serial.print(val2); Serial.print("   ");
  //  Serial.print(val3); Serial.print("   ");
  //  Serial.println(val4);
  //  delay(2000);
}

ISR(SPI_STC_vect) {
  button = SPDR;
}

void loop() {
  K1.p(val1+2, 45);
  Serial.println(K1.getP().value());
  delay(3000);
  K1.p(val2+2, 45);
  Serial.println(K1.getP().value());
  delay(3000);
  K1.p(val3+2, 45);
  Serial.println(K1.getP().value());
  delay(3000);
  K1.p(val4+2, 45);
  Serial.println(K1.getP().value());
  delay(3000);
}

//void loop() {
//  if (button == 1) {
//    K1.s(-60);
//    //    roboclaw.ForwardM1(address, 70);
//  }
//  else if (button == 2 && digitalRead(LIMIT)) {
//    K1.s(60);
//    //    roboclaw.BackwardM1(address, 70);
//  }
//  else if (button == 3) {
//    roboclaw.ForwardM2(address, 70);
//  }
//  else if (button == 4) {
//    roboclaw.BackwardM2(address, 70);
//  }
//  else {
//    K1.s(0);
//    roboclaw.ForwardM1(address, 0);
//    roboclaw.BackwardM1(address, 0);
//    roboclaw.ForwardM2(address, 0);
//    roboclaw.BackwardM2(address, 0);
//  }
//}
