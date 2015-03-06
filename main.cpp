/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 /*
 Author: Chih-Wei Tang
 */
#include "mbed.h"

#define HTU21DF_I2CADDR       0x80
#define HTU21DF_READTEMP      0xE3
#define HTU21DF_READHUM       0xE5
#define HTU21DF_WRITEREG       0xE6
#define HTU21DF_READREG       0xE7
#define HTU21DF_RESET       0xFE


DigitalOut myled(LED1);
DigitalOut myled2(LED3);

I2C i2c(p30,p7);

void flashLED();
float readTemp();
Serial pc(USBTX, USBRX);

int main(){
        myled = 0;
        myled2 = 0;
        while (1) {
            float temp = readTemp();
            pc.printf("Temp: %.2f", temp);

//            flashLED();
            if(temp >=26.0){
                myled = 1;
                myled2 = 0;
            }
            else{
                myled2 = 1;
                myled = 0;    
            }
            
            
    }

}

void flashLED(){
            myled =1;
        myled2 = 0;
        wait (2);
        myled =0;
        myled2 = 1;
        wait (0.2);
}

float readTemp(){
    uint16_t t = 0;
    uint8_t checksum = 0;
    i2c.start();
    i2c.write(HTU21DF_I2CADDR);
    i2c.write(HTU21DF_READTEMP);
    
    i2c.start();
    i2c.write(HTU21DF_I2CADDR | 0x1);
    t = i2c.read(1);
    t <<= 8;
    t |= i2c.read(1);
    checksum = i2c.read(0);
    
    i2c.stop();
    
    float temp = t;
    temp *= 175.72;
    temp /= 65536;
    temp -= 46.85;
    
    checksum += 1;

    return temp;
}
