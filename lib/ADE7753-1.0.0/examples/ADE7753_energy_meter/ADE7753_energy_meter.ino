#include "ADE7753.h"

ADE7753 meter;                        // Default constructor
// SPIClass Hspi(HSPI);
// ADE7753 meter(Hspi, 15, 4000000);  // Constructor for non-default SPI bus and frequency

// Clock frequency and calibration coeficient
const uint32_t CLK_FREQ = 3579545;
const double AENERGY_COEF = 1.0;
const double VAENERGY_COEF = 1.0;
const double IRMS_COEF = 1.0;
const double VRMS_COEF = 1.0;

void setup() {

  Serial.begin(115200);
  meter.begin();

  /*
    Set channel 1 (current) and channel 2 (voltage) gain
    ADE7753_GAIN_1
    ADE7753_GAIN_2
    ADE7753_GAIN_4
    ADE7753_GAIN_8
    ADE7753_GAIN_16
  */
  meter.setCh1Gain(ADE7753_GAIN_8);
  meter.setCh2Gain(ADE7753_GAIN_2);

  /*
    Set channel 1 (current) full scale
    ADE7753_FS_0_5V   => 0.5 V
    ADE7753_FS_0_25V  => 0.25 V
    ADE7753_FS_0_125V => 0.125 V
  */
  meter.setCh1FullScale(ADE7753_FS_0_5V);

  Serial.println("\nAEnergy\tVAEnergy\tFrequency\tRMS Current\tRMS Voltage");
}

void loop() {

  double AEnergy = AENERGY_COEF * meter.readAEnergy();
  double VAEnergy = VAENERGY_COEF * meter.readVAEnergy();
  double Frequency = (double) (CLK_FREQ / 8.0) / meter.readPeriod();
  double IRMS = IRMS_COEF * meter.readCurrentRMS();
  double VRMS = VRMS_COEF * meter.readVoltageRMS();
  
  // Show active and apparent energy, frequency, and RMS current and voltage
  Serial.print(AEnergy);
  Serial.print("\t");
  Serial.print(VAEnergy);
  Serial.print("\t");
  Serial.print(Frequency);
  Serial.print("\t");
  Serial.print(IRMS);
  Serial.print("\t");
  Serial.println(VRMS);

  delay(1000);
}
