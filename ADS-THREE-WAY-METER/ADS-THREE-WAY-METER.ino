/*
 * Calibation 3-Way Meter using ADS1115
 * 
 * ADS1115 GAIN_EIGHT // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
 * 
 *                       PH          4        7       10
 *                       Mois        1    2     3     4     5     6     7     8
 * V.1                      
 *  ADC0  Probe  เทา    = pH         14912    880      0
 *  ADC1  Probe  เงิน    = Moisture   0   2432  3600 5100  6500                      
 * 
 * V.2
 *  ADC2  Probe  เทา    = pH        10000     768      0                                       
 *  ADC3  Probe  ทอง    = Moisture   0   3000  4400  5500  6400  7000  7200 
 * 
 */

#include <Adafruit_ADS1015.h>
Adafruit_ADS1115 *ads;

void setup() {
  Wire.begin(4, 5);
  ads = new Adafruit_ADS1115(0x48);
  ads->setGain(GAIN_EIGHT);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  ads->begin();
  Serial.begin(115200);
}

void loop() {
  int16_t adc0, adc1, adc2, adc3;
  adc0 = ads->readADC_SingleEnded(0);
  adc1 = ads->readADC_SingleEnded(1);
  adc2 = ads->readADC_SingleEnded(2);
  adc3 = ads->readADC_SingleEnded(3);

//  if (adc0 < 10000) adc0 = 0;
//  if (adc1 < 10000) adc1 = 0;
//  if (adc2 < 10000) adc2 = 0;
//  if (adc3 < 10000) adc3 = 0;

  Serial.println(adc0);
  Serial.println(adc1);
  Serial.println(adc2); // pH
  Serial.println(adc3); // MoisTure

  Serial.println();
  Serial.println();


  //  Serial.printf("-200, 500, %d, %d, %d, %d\r\n", adc0, adc1, adc2, adc3);

  delay(10);
}
