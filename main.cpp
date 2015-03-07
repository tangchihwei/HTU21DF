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
DigitalOut myled2(LED2);

I2C i2c(p30,p7);

void flashLED();
float readTemp();
uint16_t read(uint8_t, uint8_t, uint8_t);
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

    //    i2c.start();
    //     i2c.write(HTU21DF_I2CADDR);
    //     i2c.write(HTU21DF_READTEMP);
    //
    //     i2c.start();
    //     i2c.write(HTU21DF_I2CADDR | 0x1);
    //     t = i2c.read(1);
    //     t <<= 8;
    //     t |= i2c.read(1);
    //
    //     checksum = i2c.read(0);
    //
    // i2c.stop();
    t = read(HTU21DF_I2CADDR, HTU21DF_READTEMP, 3);
    float temp = t;
    temp *= 175.72;
    temp /= 65536;
    temp -= 46.85;


    return temp;
}

float readHum(){
	uint16_t h = 0;
	h = read(HTU21DF_I2CADDR, HTU21DF_READHUM, 3);
	float hum = h;
	hum *= 125;
	hum /=65536;
	hum -= 6;
	
	return hum
}

uint16_t read(uint8_t i2c_address, uint8_t register_address, uint8_t length){
    uint16_t val = 0;
	uint8_t checksum = 0;
    i2c.start();
    i2c.write(i2c_address);
    i2c.write(register_address);
    i2c.start();
    i2c.write(i2c_address | 0x1);
    // for(int i = 0; i++; i< length-1){
    //  val <<= 8;
    //  val |= i2c.read(1);
    // }
    val = i2c.read(1);
    val <<= 8;
    val |= i2c.read(1);
	checksum = i2c.read(0);
	
	/*
		check the checksum, if incorrect, read again
		http://www.barrgroup.com/Embedded-Systems/How-To/CRC-Calculation-C-Code
	*/
    i2c.stop();
    return val;
}
