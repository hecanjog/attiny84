#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

double terry[] = {
    1.0,         // M1 0
    16.0 / 15.0, // m2 1
    10.0 / 9.0,  // M2 2
    6.0 / 5.0,   // m3 3
    5.0 / 4.0,   // M3 4
    4.0 / 3.0,   // P4 5
    64.0 / 45.0, // TT 6
    3.0 / 2.0,   // P5 7
    8.0 / 5.0,   // m6 8
    27.0 / 16.0, // M6 9
    16.0 / 9.0,  // m7 10
    15.0 / 8.0   // M7 11
};

double seed = 3.99;
double lval = 0.555;

double logistic(double low, double high) {
    lval = seed * lval * (1.0 - lval);
    return lval * (high - low) + low;
}

void delay(double ms) { 
    int frames = (int)(ms * 44.1);
    while(frames--) { 
        _delay_ms(0.022675736961451247); 
    } 
} 

void play(double freq, double length, int amp) {
    if(amp == 0) {
        delay(length);
        return;
    }

    double pw = logistic(0.1, 1.0);
    double plen = length * pw;
    double slen = length - plen;

    int numcycles; // length / cycle length
    double cycle_length; // ms

    double pulse_length, pause_length;
    double pulsewidth = logistic(0.05, 0.9);

    cycle_length = 1000.0 / freq;
    numcycles = (int)(plen / cycle_length);
    pulse_length = cycle_length * pulsewidth;
    pause_length = cycle_length - pulse_length;

    while(numcycles--) {
        // Set pin high and wait
        PORTB = (1 << PB1);
        delay(pulse_length);

        // Set pin low and wait
        PORTB &= ~(1 << PB1);
        delay(pause_length);
    }

    delay(slen);

}

int main (void) {
    // Set pin 3 to output
    DDRB = (1 << PB1);

    double freq, length;
    int count = 0;
    double root = 220.0;
    length = 150.0;
    int scale[] = {0, 4, 5, 7, 0};
    int octaves[] = {0, 1, 2};
    root = 80.0;

    while(1) {
        if(count % 4 == 0) {
            length = logistic(100.0, 200.0);
        }

        freq = root * terry[scale[count % 5]] * (2 << octaves[count % 3]);
        play(freq, length, 1);

        if(count % 11 == 0) {
            play(freq, length, 0);
        }

        count = (count + 1) % 1024;
    }

    return 1;
}
