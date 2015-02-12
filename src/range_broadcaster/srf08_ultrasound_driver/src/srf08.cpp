#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h> // Allow for sleep command
#include "srf08.h"

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <linux/i2c-dev.h>

/* Helper function which opens the I2C bus number specified by /dev/i2c-X. Will avoid opening a bus twice. */
int i2c_open_bus (int bus) {
  static int file = 0;
  // int file = 0;
  char filename[20];

  snprintf(filename, 19, "/dev/i2c-%d", bus);

  if (file > 0) {
    printf("I2C bus %d already opened.\n", bus);
  }
  else {
    file = open(filename, O_RDWR);
    if (file < 0) {
      printf("ERROR: Did not open I2C bus. Bus failed.\n");
      return -1;
    }
    else {
      printf("Successfully opened I2C bus %d.\n", bus);
    }
  }
  return file;
}


SRF08::SRF08(const int bus_num, const int i2cAddr) {
  //printf("Constructor entrance\n");
  i2cAddress = i2cAddr;
  bus_number = bus_num;

  i2c_file = i2c_open_bus(bus_number);

  //printf("Bus opened\n");
  ioctl(i2c_file, I2C_SLAVE, i2cAddress); // Change the slave address of the bus
}

// Starts ranging and returns result in cm
int SRF08::getRange() {
  ioctl(i2c_file, I2C_SLAVE, i2cAddress); // Change the slave address of the bus
  // printf("I2C Bus: %d\tI2C Address: %d\t", bus_number, i2cAddress);

  // Initiate ranging

  char command1[] = {0x00, 0x51}; // Set command to range in cm
  //printf("Attempting to write to bus...\n");
  write(i2c_file, command1, 2); // Write the command
  //printf("Write successful!\n");

  // Wait for data to be ready (default 65ms)
  usleep(70000); // Sleep for 70 000 us (70ms)

  // Get data from range register
  char command2[] = {0x02};           //Address of range register
  char response[] = {0x00, 0x00};     // Read from register 0x02 first, then register 0x03

  write(i2c_file, command2, 1);  //Send command to access range register
  // int range = i2c_smbus_read_word_data(i2c_file, 0x02);
  read(i2c_file, response, 2);     //Read 16bits result
  //int range = i2c_smbus_read_byte(i2c_file);
  int range = (response[0]<<8)+response[1]; //Shift two bytes into int
  return range;                             //Return int range
}

// Reads the light intensity
int SRF08::getLightIntensity() {
    ioctl(i2c_file, I2C_SLAVE, i2cAddress); // Change the slave address of the bus
    const char command[] = {0x01};           //Light intensity register
    char response[] = {0x00};
    write(i2cAddress, command, 1); //Send command
    read(i2cAddress, response, 1);    //Read response
    int lightIntensity = response[0];
    return lightIntensity;
}

/*
 * Function:    setRangeRegister
 * Args:        rangeVal
 * Returns:     void
 * Description: Sets the maximum range for which the module waits for an echo
 *              The range is ((rangeVal x 43mm) + 43mm)
 *              The max range is about six meters
 */
void SRF08::setRangeRegister(unsigned char rangeVal) {
    ioctl(i2c_file, I2C_SLAVE, i2cAddress); // Change the slave address of the bus
    char command[] = {0x02, rangeVal};       //Range register
    write(i2cAddress, command, 2);    //Send command
}


//Function setMaxGainRegister
void SRF08::setMaxGainRegister(unsigned char gainVal) {
    ioctl(i2c_file, I2C_SLAVE, i2cAddress); // Change the slave address of the bus
    char command[] = {0x01, gainVal};        //Max gain register
    write(i2cAddress, command, 2);    //Send command
}

/*
 * Function:    setAddress
 * Args:        address
 * Returns:     void
 * Description: Sets the address of the module on the I2C bus. The factory default address is 0x0E (224)
 *                  The address can have the following values:
 *                      E0 | E2 | E4 | E6 ... FC | FE
 */
void SRF08::setAddress(int address) {
    ioctl(i2c_file, I2C_SLAVE, i2cAddress); // Change the slave address of the bus
    //Send address change sequence
    char command[] = {0x00, 0xA0};
    write(i2cAddress, command, 2);
    command[1] = 0xAA;
    write(i2cAddress, command, 2);
    command[1] = 0xA5;
    write(i2cAddress, command, 2);
    command[1] = address;
    write(i2cAddress, command, 2);
    //Save the updated address
    i2cAddress = address;
}