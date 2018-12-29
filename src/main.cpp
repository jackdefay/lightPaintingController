#include <Arduino.h>

int button1 = 12;  //sets light to single color mode
int button2 = 11;
int button3 = 10;
int button4 = 6;
int switch1 = 5;

void singleColor(int color);
void twoColorSpectrum(int color1, int color2);

void setup() {
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
        else if(digitalRead(button2) == HIGH){
            buttonState2 == true;
        }
        else if(digitalRead(button3) == HIGH){
            buttonState3 == true;
        }
        else if(digitalRead(button4) == HIGH){
            buttonState4 == true;
        }

        if(buttonState1 == true && digitalRead(button1) == LOW){
            singleColor(int 0);  //hsv green
            buttonState1 == false;
        }
        else if(buttonState2 == true && digitalRead(button2) == LOW){
            singleColor(int 0);  //hsv yellow
            buttonState2 == false;
        }
        else if(buttonState3 == true && digitalRead(button3) == LOW){
            twoColorSpectrum(int 0, int 0);  //hsv red blue
            buttonState3 == false;
        }
        else if(buttonState4 == true && digitalRead(button4) == LOW){
            twoColorSpectrum(int 0, int 0);  //hsv green blue
            buttonState4 == false;
        }
    }
}

void singleColor(int color){
    //send mode and color
}

void twoColorSpectrum(int color1, int color2){
    //send mode and two colors
}
