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

void sendInstructions(int mode, int color1, int color2);

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

    pinMode(LED, OUTPUT);

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
    static int numPresses2 = 0;
    static int color = 0;
    static int color1 = 0;
    static int color2 = 0;

    // delay(500);

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
            numPresses1+=1;
            if(numPresses1 > 6){
                numPresses1 = 1;
            }

            if(numPresses1 == 1) color = 120;  //hsv green
            else if(numPresses1 == 2) color = 60;  //hsv yellow
            else if(numPresses1 == 3) color = 240;  //hsv blue
            else if(numPresses1 == 4) color = 0;  //hsv red
            else if(numPresses1 == 5) color = 300;  //hsv pink/purple
            else if(numPresses1 == 6) color = 180;  //hsv aqua

            sendInstructions(1, color, -1);
            buttonState1 = false;
        }
        if(buttonState2 == true && digitalRead(button2) == LOW){
            numPresses2+=1;
            if(numPresses2 > 5){
                numPresses2 = 1;
            }

            if(numPresses2 == 1){
                color1 = 0;  //hsv red
                color2 = 240;  //hsv blue
            }
            else if(numPresses2 == 2) {
                color1 = 120;  //hsv green
                color2 = 240;  //hsv blue
            }
            else if(numPresses2 == 3) {
                color1 = 300;  //hsv pink
                color2 = 60;  //hsv yellow
            }
            else if(numPresses2 == 4) {
                color1 = 60;  //hsv yellow
                color2 = 120;  //hsv green
            }
            else if(numPresses2 == 5) {
                color1 = 0;  //hsv red
                color2 = 60;  //hsv yellow
            }

            sendInstructions(2, color1, color2);
            buttonState2 = false;
        }
        if(buttonState3 == true && digitalRead(button3) == LOW){
            sendInstructions(4, 0, 240); //hsv green blue
            buttonState3 = false;
        }
        if(buttonState4 == true && digitalRead(button4) == LOW){
            sendInstructions(3, 240, 300); //hsv blue yellow
            buttonState4 = false;
        }
    }

    else{
        sendInstructions(0, -1, -1);
    }
}

void sendInstructions(int mode, int color1, int color2){
    char radiopacket[20];
    char temp[5];
    String tempWord = "";

    itoa((int) mode, temp, 10);
    tempWord = temp;
    tempWord += "* ";

    if(color1 >= 0){
        itoa((int) color1, temp, 10);
        tempWord += temp;
    }
    tempWord += "* ";

    if(color2 >= 0){
        itoa((int) color2, temp, 10);
        tempWord += temp;
    }
    tempWord += "* ";
    tempWord.toCharArray(radiopacket, 20);

    Serial.println(radiopacket);

    rf69.send((uint8_t *)radiopacket, strlen(radiopacket));
    rf69.waitPacketSent();
}
