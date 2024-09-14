#include <Wire.h>

// MPU6050 I2C address
const int MPU6050_ADDR = 0x68;

// MPU6050 register addresses
const int PWR_MGMT_1 = 0x6B;
const int GYRO_XOUT_H = 0x43;
const int GYRO_YOUT_H = 0x45;
const int GYRO_ZOUT_H = 0x47;

// Variables for storing gyro data
int16_t gyroX, gyroY, gyroZ;
float yaw = 0.0;
unsigned long lastTime = 0;
float deltaTime;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  // Wake up the MPU6050 from sleep mode
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(PWR_MGMT_1);
  Wire.write(0);  // Set to zero to wake up the MPU6050
  Wire.endTransmission(true);
}

void loop() {
  // Get gyro data
  readMPU6050GyroData();

  // Calculate the time difference (in seconds)
  unsigned long currentTime = millis();
  deltaTime = (currentTime - lastTime) / 1000.0;
  lastTime = currentTime;

  // Convert the raw gyroZ data to degrees/second
  float gyroZ_deg = gyroZ / 131.0;

  // Integrate the angular velocity to get the yaw angle
  yaw += gyroZ_deg * deltaTime;

  // Print the yaw angle
  Serial.print("Yaw: ");
  Serial.println(yaw);

  delay(100);  // Adjust delay as necessary
}

// Function to read raw gyro data from MPU6050
void readMPU6050GyroData() {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(GYRO_XOUT_H);  // Starting with the high byte of the gyro registers
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDR, 6, true);  // Request 6 bytes of data

  // Read the raw data
  gyroX = (Wire.read() << 8 | Wire.read());  // X-axis
  gyroY = (Wire.read() << 8 | Wire.read());  // Y-axis
  gyroZ = (Wire.read() << 8 | Wire.read());  // Z-axis
}
