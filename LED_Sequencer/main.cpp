/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include <cstring>

#define WAIT_TIME_MS 500
#define BUF_SIZE 21
#define MAX_SEQ 10
#define SHORT_PRESS 200.0e-3f

InterruptIn button(BUTTON1);
Timer timer_buttonPressed;
Timer timer_ledState;

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
BufferedSerial pc(USBTX, USBRX);

float freq = 0.5f;
float value;
int currentState = 0;
int currentSequence = 0;
char* sequences[MAX_SEQ];
int seqCount;

char buf[BUF_SIZE];
char s[BUF_SIZE+1];
int buf_count = 0;
char message[BUF_SIZE+1];
int action;
bool new_message_received = false;

void on_serial_activity();
int char_to_bin(char c);
void led_on(DigitalOut led, char c);

int main() {
    seqCount = 3;
    sequences[0] = "10204030605070";
    sequences[1] = "102040";
    sequences[2] = "70";

    for(int i=0; i<strlen(sequences[0]);i++){
        led_on(led1, sequences[i][0]); // Access character by index
    }

    while (true) {
        pc.sigio(&on_serial_activity);
        if (new_message_received) {
            if(message[0] == '+'){ //if "+" character is at the beginning a new sequency is added
                char ch;
                int len = strlen(message);
                printf("Received a length %d message: %s\n", len, message);

                for (int i = 0; i < strlen(message); i++) {
                    ch = message[i];
                    led_on(led1, ch);
                }
            } else if(message[0]=='*'){ // if "*" character is at the beginning the frequency is changed (freq)
                freq=atof(message); //converts char to float
            }

            new_message_received = false;
        }

        /* Loop through the current sequence and control the LED based on the binary representation */
        for (int i = 0; i < strlen(sequences[currentSequence]); i++) {
            char ch = sequences[currentSequence][i];
            int binary_value = char_to_bin(ch);

            // Store the previous bit for comparison
            static int prev_bit = 0; // Initialize static variable for persistence

            for (int j = 0; j < 8; j++) {
                int bit = (binary_value >> j) & 1;

                if (bit == 1 && prev_bit == 0) {
                    // Turn on the LED
                    led1 = 1;
                    thread_sleep_for(WAIT_TIME_MS);
                } else if (bit == 0 && prev_bit == 1) {
                    // Turn off the LED
                    led1 = 0;
                    thread_sleep_for(WAIT_TIME_MS);
                }

                // Update the previous bit for the next iteration
                prev_bit = bit;
            }
        }

        thread_sleep_for(freq);
    }
}

char *strnchr(char *s, size_t count, char c)
{
    char *pos = s;
    while (pos - s < count && *pos != c) {
        pos++;
    }
    if (pos - s == count) {
        pos = nullptr;
    }

    return pos;
}

void on_serial_activity()
{
    if (pc.readable()) {
        int len = pc.read(buf + buf_count, BUF_SIZE - buf_count);
        if (len > 0) {
            char *pos = strnchr(buf + buf_count, len, '\r');
            buf_count += len;
            if (pos) {
                strncpy(message, buf, pos - buf);
                
