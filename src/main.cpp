#include <Arduino.h>

int button1 = 12;  //sets light to single color mode
int button2 = 11;
int button3 = 10;
int button4 = 6;
int switch1 = 5;

// void singleColor(int color);

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

    delay(100);

    Serial.println(digitalRead(button1));

    if(digitalRead(switch1) == HIGH){

        // if(digitalRead(button2) == HIGH){
        //     color++;
        //     if(color > 360){
        //         color = 0;
        //     }
        // }

        // if(buttonState1 == false && digitalRead(button1) == HIGH){
        //     singleColor(color);
        //     buttonState1 = true;
        // }


    }

    // if(/*buttonState1 == true &&*/ digitalRead(button1) == LOW){
    //     buttonState1 = false;
    //     digitalWrite(13, HIGH);
    //
    // }

    // if(numPresses1 > 0){
    //     numPresses1 = 0;
    // }



    delay(100);
    digitalWrite(13, LOW);
}

void singleColor(int color){
    //send mode and color
    Serial.println(color);
}
