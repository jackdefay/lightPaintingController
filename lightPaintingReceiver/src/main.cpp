#include <Arduino.h>
#include <SPI.h>
#include <RH_RF69.h>

#define RF69_FREQ 900.0

  #define RFM69_CS      8
  #define RFM69_INT     3
  #define RFM69_RST     4
  #define LED           13

RH_RF69 rf69(RFM69_CS, RFM69_INT);

int hsvToRgb(int hue);
void writeColor(int rgb);
void writeSpectrum(int rgb1, int rgb2, int cycleTime);

#define CYCLETIME 2000

void setup() {
    Serial.begin(115200);

    Serial.println("starting");

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
      //Serial.println("setFrequency failed");
    }
    rf69.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW
    uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                      0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    rf69.setEncryptionKey(key);

    pinMode(LED, OUTPUT);
}

void loop() {
    String word, mode, color1, color2;
    char temp[20];
    int i, modeInt, color1int, color2int;

    // delay(500);

    if (rf69.available()) {
        uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
        if (rf69.recv(buf, &len)) {
            if (!len) return;
            buf[len] = 0;
            Serial.println((char*)buf);
            for(int i = 0; i<20; i++){
                temp[i] = (char) buf[i];
            }
            word = temp;
            i = 0;

            while(word.charAt(i) != '*'){
                if(word.charAt(i) >= '0' && word.charAt(i) <= '9') mode += word.charAt(i);
                i++;
            }
            i++;
            while(word.charAt(i) != '*'){
                if(word.charAt(i) >= '0' && word.charAt(i) <= '9') color1 += word.charAt(i);
                i++;
            }
            i++;
            while(word.charAt(i) != '*'){
                if(word.charAt(i) >= '0' && word.charAt(i) <= '9') color2 += word.charAt(i);
                i++;
            }

            modeInt = mode.toInt();
            color1int = color1.toInt();
            color2int = color2.toInt();
            Serial.print(modeInt); Serial.print(", "); Serial.print(color1int); Serial.print(", "); Serial.println(color2int);

            if(modeInt == 1){
                writeColor(hsvToRgb(color1int));
            }
            else if(modeInt == 2){
                writeSpectrum(hsvToRgb(color1int), hsvToRgb(color2int), CYCLETIME);
            }
            digitalWrite(13, HIGH);
        }
        else {
          Serial.println("Receive failed");
        }
    }
    else {
       digitalWrite(13, LOW);
       Serial.println("no signal");
    }
}

int hsvToRgb(int hue){
    return hue;  //converts the hue in hsv format to an array of rgb format
}

void writeColor(int rgb){
    //write a single color to the leds
    //short delay
    Serial.println(rgb);
}

void writeSpectrum(int rgb1, int rgb2, int cycleTime){
    //two for loops spectrum cycling, divide delay so the total time = cycleTime
    Serial.print(rgb1); Serial.print(", "); Serial.println(rgb2);
}
