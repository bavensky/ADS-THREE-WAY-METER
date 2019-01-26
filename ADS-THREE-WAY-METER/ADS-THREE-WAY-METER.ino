/*
   Calibation 3-Way Meter using ADS1115

   ADS1115 GAIN_EIGHT // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV

                         PH          4        7       10
                         Mois        1    2     3     4     5     6     7     8
   V.1
    ADC0  Probe  เทา    = pH         14912    880      0
    ADC1  Probe  เงิน    = Moisture   0   2432  3600 5100  6500

   V.2
    ADC2  Probe  เทา    = pH        10000     768      0
    ADC3  Probe  ทอง    = Moisture   0   3000  4400  5500  6400  7000  7200

*/

#include <Adafruit_ADS1015.h>
Adafruit_ADS1115 *ads;

int soil_In1[]  = {0,  2432,   3600,   5100,   6500};
int soil_Out1[] = {1,     2,      3,      4,      5};
int soil_In2[]  = {0,  3000,   4400,   5500,   6400,   7000,   7200};
int soil_Out2[] = {1,     2,      3,      4,      5,      6,      7};

int pH_In_probe1[]  = {14912,   880,    0};
int pH_Out_probe1[] = {    4,     7,    10};
int pH_In_probe2[]  = {10000,   768,    0};
int pH_Out_probe2[] = {    4,     7,    10};

uint16_t pH_probe1, soil_probe1, pH_probe2, soil_probe2;

int multiMap(int val, int* _in, int* _out, uint8_t size)
{
  // take care the value is within range
  // val = constrain(val, _in[0], _in[size-1]);
  if (val <= _in[0]) return _out[0];
  if (val >= _in[size - 1]) return _out[size - 1];

  // search right interval
  uint8_t pos = 1;  // _in[0] allready tested
  while (val > _in[pos]) pos++;

  // this will handle all exact "points" in the _in array
  if (val == _in[pos]) return _out[pos];

  // interpolate in the right segment for the rest
  return (val - _in[pos - 1]) * (_out[pos] - _out[pos - 1]) / (_in[pos] - _in[pos - 1]) + _out[pos - 1];
}

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


  pH_probe1 = multiMap(adc0, pH_In_probe1, pH_Out_probe1, 3);

  Serial.println(adc0);
  Serial.println(adc1);
  Serial.println(adc2); // pH
  Serial.println(adc3); // MoisTure
  Serial.println(pH_probe1);

  Serial.println();
  Serial.println();


  //  Serial.printf("-200, 500, %d, %d, %d, %d\r\n", adc0, adc1, adc2, adc3);

  delay(10);
}
