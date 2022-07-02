/*
 * Microstepping demo
 *
 * This requires that microstep control pins be connected in addition to STEP,DIR
 *
 * Copyright (C)2015 Laurentiu Badea
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 */
#include <Arduino.h>
#include "Motor.h"

void setup()
{
    Serial.begin(115200);
    Serial.println("OK");
    pinMode(LED, OUTPUT);
    digitalWrite(LED, 1);
    InitMotores();
}

void SerialLoop()
{
    if (Serial.available())
    {
        char c = Serial.read();
        if (c == 0x1B)
            return;
        if (c == 0x5B)
            return;

       // Serial.print(c, 16);
        switch (c)
        {
        case 'a':
        case 0x41:
            Avanzar();
            break;
        case 'p':
        case 0x30:
            Parar();
            break;
        case 'g':
        case 0x43:
            GirarDerecha();
            break;
        case 0x44:
            GirarIzquierda();
            break;
        case 'f':
            Frenar();
            break;
        case 'r':
        case 0x42:
            Retroceder();
            break;
        }
    }
}

void loop()
{
    runner.execute();
    SerialLoop();
}
