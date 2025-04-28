#include "VexIQ.h"

VexIQ_TouchLED touchLED2;
VexIQ_TouchLED touchLED3;
VexIQ_TouchLED touchLED4;

#define PORT_LED2 VEXIQ_PORT_2
#define PORT_LED3 VEXIQ_PORT_3
#define PORT_LED4 VEXIQ_PORT_4

#define MAX_SEQUENCE_LENGTH 50
uint8_t sequence[MAX_SEQUENCE_LENGTH];
uint8_t sequenceLength = 0;

enum Color { RED, GREEN, BLUE };

void showColor(VexIQ_TouchLED& led, Color color) {
    switch (color) {
        case RED:   led.setRGB(255, 0, 0); break;
        case GREEN: led.setRGB(0, 255, 0); break;
        case BLUE:  led.setRGB(0, 0, 255); break;
    }
    delay(300); // Уменьшена задержка для удобства
    led.setRGB(0, 0, 0);
    delay(150);
}

Color getColorFromButton() {
    while (true) {
        if (touchLED2.isTouch()) {
            showColor(touchLED2, RED); // Визуальный отклик на нажатие
            return RED;
        }
        if (touchLED3.isTouch()) {
            showColor(touchLED3, GREEN);
            return GREEN;
        }
        if (touchLED4.isTouch()) {
            showColor(touchLED4, BLUE);
            return BLUE;
        }
        delay(10);
    }
}

void displaySequence() {
    for (uint8_t i = 0; i < sequenceLength; i++) {
        switch (sequence[i]) {
            case RED:   showColor(touchLED2, RED); break;
            case GREEN: showColor(touchLED3, GREEN); break;
            case BLUE:  showColor(touchLED4, BLUE); break;
        }
    }
}

bool checkPlayerInput() {
    for (uint8_t i = 0; i < sequenceLength; i++) {
        Color playerColor = getColorFromButton();
        if (playerColor != sequence[i]) {
            // Мигаем ВСЕМИ кнопками красным при ошибке
            touchLED2.setRGB(255, 0, 0);
            touchLED3.setRGB(255, 0, 0);
            touchLED4.setRGB(255, 0, 0);
            delay(1000);
            touchLED2.setRGB(0, 0, 0);
            touchLED3.setRGB(0, 0, 0);
            touchLED4.setRGB(0, 0, 0);
            return false;
        }
    }
    return true;
}

void setup() {
    VexIQ.begin();
    touchLED2.begin(PORT_LED2);
    touchLED3.begin(PORT_LED3);
    touchLED4.begin(PORT_LED4);
    touchLED2.setRGB(0, 0, 0);
    touchLED3.setRGB(0, 0, 0);
    touchLED4.setRGB(0, 0, 0);
}

void loop() {
    sequenceLength = 0;
    while (true) {
        sequence[sequenceLength] = static_cast<Color>(random(3));
        sequenceLength++;
        displaySequence();
        if (!checkPlayerInput()) break;
        delay(800); // Уменьшена задержка между раундами
    }
    delay(2000);
}