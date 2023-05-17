/***************************************************************************
* Example sketch for the MPU9250_WE library
*
* This sketch shows how to measure pitch and roll angles from the MPU9250. 
* I have also included the corrected angle method for comparison.  
* 
* For further information visit my blog:
*
* https://wolles-elektronikkiste.de/mpu9250-9-achsen-sensormodul-teil-1  (German)
* https://wolles-elektronikkiste.de/en/mpu9250-9-axis-sensor-module-part-1  (English)
* 
***************************************************************************/

#include <MPU9250_WE.h>
#include <Wire.h>
#include <HMC5883L.h>

HMC5883L compass;

#define MPU9250_ADDR 0x68
#define EEPROM1_I2C_address 0x50

int maxAddress = 20;
int addr = 0;

/* There are several ways to create your MPU9250 object:
 * MPU9250_WE myMPU9250 = MPU9250_WE()              -> uses Wire / I2C Address = 0x68
 * MPU9250_WE myMPU9250 = MPU9250_WE(MPU9250_ADDR)  -> uses Wire / MPU9250_ADDR
 * MPU9250_WE myMPU9250 = MPU9250_WE(&wire2)        -> uses the TwoWire object wire2 / MPU9250_ADDR
 * MPU9250_WE myMPU9250 = MPU9250_WE(&wire2, MPU9250_ADDR) -> all together
 * Successfully tested with two I2C busses on an ESP32
 */
MPU9250_WE myMPU9250 = MPU9250_WE(MPU9250_ADDR);

void setup() {
  Serial.begin(115200);
  Wire.begin();
  if(!myMPU9250.init()){
    Serial.println("MPU9250 does not respond");
  }
  else{
    Serial.println("MPU9250 is connected");
  }

  // Initialize Initialize HMC5883L
  Serial.println("Initialize HMC5883L");
  while (!compass.begin())
  {
    Serial.println("Could not find a valid HMC5883L sensor, check wiring!");
    delay(500);
  }
  /* The slope of the curve of acceleration vs measured values fits quite well to the theoretical 
   * values, e.g. 16384 units/g in the +/- 2g range. But the starting point, if you position the 
   * MPU9250 flat, is not necessarily 0g/0g/1g for x/y/z. The autoOffset function measures offset 
   * values. It assumes your MPU9250 is positioned flat with its x,y-plane. The more you deviate 
   * from this, the less accurate will be your results.
   * The function also measures the offset of the gyroscope data. The gyroscope offset does not   
   * depend on the positioning.
   * This function needs to be called at the beginning since it can overwrite your settings!
   */
  Serial.println("Position you MPU9250 flat and don't move it - calibrating...");
  delay(1000);
  myMPU9250.autoOffsets();
  Serial.println("Done!");
  
  /*  This is a more accurate method for calibration. You have to determine the minimum and maximum 
   *  raw acceleration values of the axes determined in the range +/- 2 g. 
   *  You call the function as follows: setAccOffsets(xMin,xMax,yMin,yMax,zMin,zMax);
   *  Use either autoOffset or setAccOffsets, not both.
   */
  //myMPU9250.setAccOffsets(-14240.0, 18220.0, -17280.0, 15590.0, -20930.0, 12080.0);
  
  /*  Sample rate divider divides the output rate of the gyroscope and accelerometer.
   *  Sample rate = Internal sample rate / (1 + divider) 
   *  It can only be applied if the corresponding DLPF is enabled and 0<DLPF<7!
   *  Divider is a number 0...255
   */
  myMPU9250.setSampleRateDivider(5);

  /*  MPU9250_ACC_RANGE_2G      2 g    (default)
   *  MPU9250_ACC_RANGE_4G      4 g
   *  MPU9250_ACC_RANGE_8G      8 g   
   *  MPU9250_ACC_RANGE_16G    16 g
   */
  myMPU9250.setAccRange(MPU9250_ACC_RANGE_2G);

  /*  Enable/disable the digital low pass filter for the accelerometer 
   *  If disabled the bandwidth is 1.13 kHz, delay is 0.75 ms, output rate is 4 kHz
   */
  myMPU9250.enableAccDLPF(true);


  /*  Digital low pass filter (DLPF) for the accelerometer, if enabled 
   *  MPU9250_DPLF_0, MPU9250_DPLF_2, ...... MPU9250_DPLF_7 
   *   DLPF     Bandwidth [Hz]      Delay [ms]    Output rate [kHz]
   *     0           460               1.94           1
   *     1           184               5.80           1
   *     2            92               7.80           1
   *     3            41              11.80           1
   *     4            20              19.80           1
   *     5            10              35.70           1
   *     6             5              66.96           1
   *     7           460               1.94           1
   */
  myMPU9250.setAccDLPF(MPU9250_DLPF_6);  

  // Set measurement range
  compass.setRange(HMC5883L_RANGE_1_3GA);

  // Set measurement mode
  compass.setMeasurementMode(HMC5883L_CONTINOUS);

  // Set data rate
  compass.setDataRate(HMC5883L_DATARATE_30HZ);

  // Set number of samples averaged
  compass.setSamples(HMC5883L_SAMPLES_8);

  // Set calibration offset. See HMC5883L_calibration.ino
  compass.setOffset(121, -409);
}

void loop() {
  xyzFloat angles = myMPU9250.getAngles();

/* Pitch and roll consider all axes for calculation. According to my experience
   it provides more reliable results at higher angles (>60°) */
  float pitchRaw = myMPU9250.getPitch();
  float rollRaw  = myMPU9250.getRoll();
  
  int pitch = int(abs(pitchRaw));
  int roll = int(abs(rollRaw));

  Vector norm = compass.readNormalize();

  // Calculate heading
  float heading = atan2(norm.YAxis, norm.XAxis);

  // Set declination angle on your location and fix heading
  // You can find your declination on: http://magnetic-declination.com/
  // (+) Positive or (-) for negative
  // For Bytom / Poland declination angle is 4'26E (positive)
  // Formula: (deg + (min / 60.0)) / (180 / M_PI);
  float declinationAngle = (4.0 + (26.0 / 60.0)) / (180 / M_PI);
  heading += declinationAngle;

  // Correct for heading < 0deg and heading > 360deg
  if (heading < 0)
  {
    heading += 2 * PI;
  }

  if (heading > 2 * PI)
  {
    heading -= 2 * PI;
  }

  // Convert to degrees
  float headingDegrees = heading * 180/M_PI; 

  int temp = myMPU9250.getTemperature();
  
  static bool runCode = true;
  static int counter = 0; // address 0 is pitch, 1 is roll, 2 is heading, and 3 is temp in C
  
  if (runCode) { 
    writeToEEPROM(EEPROM1_I2C_address, addr, pitch);
    delay(10);
    Serial.println(readFromEEPROM(EEPROM1_I2C_address, addr), DEC);
    delay(10);
    addr++;
    writeToEEPROM(EEPROM1_I2C_address, addr, roll);
    delay(10);
    Serial.println(readFromEEPROM(EEPROM1_I2C_address, addr), DEC);
    delay(10);
    addr++;
    writeToEEPROM(EEPROM1_I2C_address, addr, int(headingDegrees));
    delay(10);
    Serial.println(readFromEEPROM(EEPROM1_I2C_address, addr), DEC);
    delay(10);
    addr++;
    writeToEEPROM(EEPROM1_I2C_address, addr, temp); // temp in C
    delay(10);
    Serial.println(readFromEEPROM(EEPROM1_I2C_address, addr), DEC);
    delay(10);
    addr++;
    
  counter++;
  if (counter >= maxAddress) {
    runCode = false;
  }
 }
  
  delay(1000);
  
}

void writeToEEPROM(byte EEPROMAddress, byte dataAddress, byte dataValue)
{
  Wire.beginTransmission(EEPROMAddress);
  Wire.write(dataAddress);
  Wire.write(dataValue);
  Wire.endTransmission();
  delay(5);
}
//======================================================================
byte readFromEEPROM(byte EEPROMAddress, byte dataAddress)
{
  Wire.beginTransmission(EEPROMAddress);
  Wire.write(dataAddress);
  Wire.endTransmission();
  delay(5);
  Wire.requestFrom(EEPROMAddress, 1);
  if(Wire.available()) return Wire.read();
  return 0;
}
