#include <Arduino.h>
#include <SPI.h>
#include <RH_RF69.h>
#include <math.h>

#define RF69_FREQ 900.0

  #define RFM69_CS      8
  #define RFM69_INT     3
  #define RFM69_RST     4
  #define LED           13

RH_RF69 rf69(RFM69_CS, RFM69_INT);

void hsvToRgb(int hue, int rgb[]);
void writeColor(int rgb);
void writeSpectrum(int rgb1, int rgb2, int cycleTime);
void turnOff();

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

    delay(500);

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
                writeColor(color1int);
            }
            else if(modeInt == 2){
                writeSpectrum(color1int, color2int, CYCLETIME);
            }
            // digitalWrite(13, HIGH);
        }
        else {
          Serial.println("Receive failed");
        }
    }
    else {
       // digitalWrite(13, LOW);
       Serial.println("no signal");
    }
}

void hsvToRgb(int hue, int rgb[]){

    double chroma = 1;  //saturation * value, since I assume these are both 1, chroma is 1
    double huePrime = hue/60;
    double X = chroma * (1-abs(fmod(huePrime, 2.0) - 1));

    int roundedHuePrime = floor(huePrime+0.5);
    int chromaInt = 255*round(chroma);
    int XInt = 255*floor(X+0.5);

    switch(roundedHuePrime){
        case 0:
            rgb[0] = chromaInt;
            rgb[1] = XInt;
            rgb[2] = 0;
            break;
        case 1:
            rgb[0] = chromaInt;
            rgb[1] = XInt;
            rgb[2] = 0;
            break;
        case 2:
            rgb[0] = XInt;
            rgb[1] = chromaInt;
            rgb[2] = 0;
            break;
        case 3:
            rgb[0] = 0;
            rgb[1] = chromaInt;
            rgb[2] = XInt;
            break;
        case 4:
            rgb[0] = 0;
            rgb[1] = XInt;
            rgb[2] = chromaInt;
            break;
        case 5:
            rgb[0] = XInt;
            rgb[1] = 0;
            rgb[2] = chromaInt;
            break;
        case 6:
            rgb[0] = chromaInt;
            rgb[1] = 0;
            rgb[2] = XInt;
            break;
        default:
            //do nothing
            break;

    //don't have to add m because V-C is 0
    //https://en.wikipedia.org/wiki/HSL_and_HSV, see convert from HSV section
    }

    //converts the hue in hsv format to an array of rgb format
}

void writeColor(int hue){
    int rgb[3];
    hsvToRgb(hue, rgb);  //now use rgb array for assigning the colors
    
    //write a single color to the leds
    //short delay
    // for(int i = 0; i<3; i++){
    //     Serial.print(rgb[i]);
    //     if(i!=2) Serial.print(", ");
    // }
    // Serial.println();
}

void writeSpectrum(int hue1, int hue2, int cycleTime){
    int rgb1[3], rgb2[3];
    hsvToRgb(hue1, rgb1);
    hsvToRgb(hue2, rgb2);
    //two for loops spectrum cycling, divide delay so the total time = cycleTime

    // for(int i = 0; i<3; i++){
    //     Serial.print(rgb1[i]);
    //     if(i!=2) Serial.print(", ");
    // }
    // Serial.println();
    // for(int i = 0; i<3; i++){
    //     Serial.print(rgb2[i]);
    //     if(i!=2) Serial.print(", ");
    // }
    // Serial.println();
}

void turnOff(){

}
