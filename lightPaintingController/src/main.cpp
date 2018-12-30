#include <Arduino.h>
#include <SPI.h>
#include <RH_RF69.h>

#define RF69_FREQ 900.0

  #define RFM69_CS      8
  #define RFM69_INT     3
  #define RFM69_RST     4
  #define LED           13

RH_RF69 rf69(RFM69_CS, RFM69_INT);

int button1 = 12;  //sets light to single color mode
int button2 = 11;
int button3 = 10;
int button4 = 6;
int switch1 = 5;

void singleColor(int color);
void twoColorSpectrum(int color1, int color2);
void turnOff();

void setup() {
    pinMode(LED, OUTPUT);
    pinMode(RFM69_RST, OUTPUT);
    digitalWrite(RFM69_RST, LOW);

      // manual reset
    digitalWrite(RFM69_RST, HIGH);
    delay(10);
    digitalWrite(RFM69_RST, LOW);
    delay(10);

    if (!rf69.init()) {
      Serial.println("RFM69 radio init failed");
      while (1);
    }
    Serial.println("RFM69 radio init OK!");
    if (!rf69.setFrequency(RF69_FREQ)) {
      Serial.println("setFrequency failed");
    }
    rf69.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW
    uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                      0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    rf69.setEncryptionKey(key);

    pinMode(button1, INPUT);
    pinMode(button2, INPUT);
    pinMode(button3, INPUT);
    pinMode(button4, INPUT);
    pinMode(switch1, INPUT);

    Serial.begin(115200);
}

void loop() {
    static bool buttonState1 = false;
    static bool buttonState2 = false;
    static bool buttonState3 = false;
    static bool buttonState4 = false;
    static int numPresses1 = 0;
    static int color = 0;

    if(digitalRead(switch1) == HIGH){

        if(digitalRead(button1) == HIGH){
            buttonState1 = true;
        }
        if(digitalRead(button2) == HIGH){
            buttonState2 = true;
        }
        if(digitalRead(button3) == HIGH){
            buttonState3 = true;
        }
        if(digitalRead(button4) == HIGH){
            buttonState4 = true;
        }

        if(buttonState1 == true && digitalRead(button1) == LOW){
            singleColor(120);  //hsv green
            buttonState1 = false;
        }
        if(buttonState2 == true && digitalRead(button2) == LOW){
            singleColor(60);  //hsv yellow
            buttonState2 = false;
        }
        if(buttonState3 == true && digitalRead(button3) == LOW){
            twoColorSpectrum(0, 240);  //hsv red blue
            buttonState3 = false;
        }
        if(buttonState4 == true && digitalRead(button4) == LOW){
            twoColorSpectrum(120, 240);  //hsv green blue
            buttonState4 = false;
        }
    }

    else{
        turnOff();
    }
}

void singleColor(int color){
    char radiopacket[20];
    char temp[5];
    String tempWord = "";
    int mode = 1;

    itoa((int) mode, temp, 10);
    tempWord += temp;
    tempWord += "* ";

    itoa((int) color, temp, 10);
    tempWord += temp;
    tempWord += "* ";

    tempWord += "* ";
    tempWord.toCharArray(radiopacket, 20);

    Serial.println(radiopacket);

    rf69.send((uint8_t *)radiopacket, strlen(radiopacket));
    rf69.waitPacketSent();
}

void twoColorSpectrum(int color1, int color2){
    char radiopacket[20];
    char temp[5];
    String tempWord = "";
    int mode = 2;

    itoa((int) mode, temp, 10);
    tempWord = temp;
    tempWord += "* ";

    itoa((int) color1, temp, 10);
    tempWord += temp;
    tempWord += "* ";

    itoa((int) color2, temp, 10);
    tempWord += temp;
    tempWord += "* ";
    tempWord.toCharArray(radiopacket, 20);

    Serial.println(radiopacket);

    rf69.send((uint8_t *)radiopacket, strlen(radiopacket));
    rf69.waitPacketSent();
}

void turnOff(){
    char radiopacket[20];
    char temp[5];
    String tempWord = "";
    int mode = 1;

    itoa((int) 0, temp, 10);
    tempWord += temp;
    tempWord += "* ";

    tempWord += "* ";
    tempWord += "* ";
    tempWord.toCharArray(radiopacket, 20);

    Serial.println(radiopacket);

    rf69.send((uint8_t *)radiopacket, strlen(radiopacket));
    rf69.waitPacketSent();
}
