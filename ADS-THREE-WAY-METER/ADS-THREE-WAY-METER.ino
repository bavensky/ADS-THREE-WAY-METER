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

//// Kadyai 001 GAIN_EIGHT
uint32_t soil_In_probe1[]  = {0,  3500,   6500,   8500,   12500,   17800};
uint32_t soil_In_probe2[]  = {0,  1500,   3000,   6000,   8000,   12000};
uint32_t pH_In_probe1[]  = {10000,   1300,  650};
uint32_t pH_In_probe2[]  = {19000,   3000,  1000};

//// Kadyai 002 GAIN_SIXTEEN
//uint32_t soil_In_probe1[]  = {0,  1200,   1600,   2000,   2500,   2500};
//uint32_t soil_In_probe2[]  = {0,  900,   1300,   1700,   3000,   3000};
//uint32_t pH_In_probe1[]  = {20000,   1850,  800};
//uint32_t pH_In_probe2[]  = {20000,   1500,  1000};


// Meter out
int soil_Out[]     = {    1,     2,     3,     4,     5,     6};
int pH_Out_probe[] = {    4,     7,    10};

//// Kadyai 003 GAIN_SIXTEEN
//uint32_t soil_In_probe1[]  = {0,  600,   1000,   1300,   3500,   7000};
//uint32_t soil_In_probe2[]  = {0,  1000,   1200,   1500,   3000,   6000};
//uint32_t pH_In_probe1[]  = {6000,   3000,  1000};
//uint32_t pH_In_probe2[]  = {2000,   1500,  800};
//// Meter out
//int soil_Out[]     = {    1,     2,     3,     4,     5,     6};
//int pH_Out_probe[] = {    4,     5,    8};

//// Kadyai 004 GAIN_SIXTEEN
//uint32_t soil_In_probe1[]  = {0,  2000,   3000,   3500,   4600,   6000};
//uint32_t soil_In_probe2[]  = {0,  600,   3200,   5500,   8300,   10000};
//uint32_t pH_In_probe1[]  = {3000,   1200,  400};
//uint32_t pH_In_probe2[]  = {6000,   3700,  2400};
//// Meter out
//int soil_Out[]     = {    1,     2,     3,     4,     5,     6};
//int pH_Out_probe[] = {    4,     5,    7};

//// Kadyai 005 GAIN_SIXTEEN
//uint32_t soil_In_probe1[]  = {0,  1200,   2000,   3000,   5000,   6000};
//uint32_t soil_In_probe2[]  = {0,  400,   4100,   7600,   9000,   10000};
//uint32_t pH_In_probe1[]  = {20000,   2200,  1000};
//uint32_t pH_In_probe2[]  = {14000,   7000,  4000};
//// Meter out
//int soil_Out[]     = {    1,     2,     3,     4,     5,     6};
//int pH_Out_probe[] = {    4,     5,    7};

uint32_t read_soil1, read_soil2, read_pH1, read_pH2;
uint16_t pH_probe1, soil_probe1, pH_probe2, soil_probe2;

uint32_t multiMap(uint32_t val, uint32_t* _in, int* _out, uint8_t size)
{
  if (val <= _in[0]) return _out[0];
  if (val >= _in[size - 1]) return _out[size - 1];

  uint32_t pos = 1;  // _in[0] allready tested
  while (val > _in[pos]) pos++;
  if (val == _in[pos]) return _out[pos];

  return (val - _in[pos - 1]) * (_out[pos] - _out[pos - 1]) / (_in[pos] - _in[pos - 1]) + _out[pos - 1];
}

// ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
// ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
// ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
// ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
// ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
// ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV


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

  if (adc0 < 100) adc0 = 0;
  if (adc1 < 100) adc1 = 0;
  if (adc2 < 100) adc2 = 0;
  if (adc3 < 100) adc3 = 0;

  read_soil1 = adc1;
  read_soil2 = adc3;
  read_pH1 = adc0;
  read_pH2 = adc2;

  // Kadyai 001
  soil_probe1 = multiMap(read_soil1, soil_In_probe1, soil_Out, 6);
  soil_probe2 = multiMap(read_soil2, soil_In_probe2, soil_Out, 6);
  pH_probe1   = multiMap(read_pH1, pH_In_probe1, pH_Out_probe, 3);
  pH_probe2   = multiMap(read_pH2, pH_In_probe2, pH_Out_probe, 3);


  // Kadyai 002
  //  soil_probe1 = multiMap(adc1, soil_In_probe1, soil_Out, 6);
  //  soil_probe2 = multiMap(adc3, soil_In_probe2, soil_Out, 6);
  //  pH_probe1   = multiMap(adc0, pH_In_probe1, pH_Out_probe, 3);
  //  pH_probe2   = multiMap(adc2, pH_In_probe2, pH_Out_probe, 3);


  //  Serial.println(adc0); // pH 1
  //  Serial.println(adc1); // MoisTure 1
  //  Serial.println(adc2); // pH 2
  //  Serial.println(adc3); // MoisTure 2

  //  Serial.println(pH_probe1);


  Serial.printf("0, 20000, %d, %d, %d, %d\r\n", adc0, adc1, adc2, adc3);
  Serial.printf("0, 20000, %d, %d, %d, %d\r\n", soil_probe1, pH_probe1, soil_probe2, pH_probe2);
  Serial.println();
  Serial.println();

  delay(100);
}
