#include "atmel_start.h"
#include "src/vocab.h"
#include "src/blink_lab.h"

int main(void) {
    // int icount = 5;
    SystemInit();
    LED_output();
    blink_lab();
    while (1);
}
