#ifndef SRF08_H
#define SRF08_H

#define SRF08_DEFAULT_ADDR 112

#define SRF08_COMMAND_REG 0x00
#define SRF08_GAIN_REG 0x01
#define SRF08_RANGE_REG 0x02

#define SRF08_RANGE_LOW_BYTE 0x3

#define SRF08_MAX_REGISTER_NUMBER 35
#define SRF08_MAX_ECHO_NUMBER 17
#define SRF08_MAX_RANGE_6m 0x8C
#define SRF08_MAX_GAIN 0x25

#define SRF08_MODE_INCH = 0x50
#define SRF08_MODE_CM = 0x51
#define SRF08_MODE_MICRO_SEC = 0x52
#define SRF08_ANN_MODE_INCH = 0x53
#define SRF08_ANN_MODE_CM = 0x54
#define SRF08_ANN_MODE_MICRO_SEC = 0x55


/* Opens the I2C bus number specified by /dev/i2c-X. Will avoid opening a bus twice. */
int i2c_open_bus (int bus);

class SRF08 {

    protected:
        int i2cAddress;
        int i2c_file;
        int bus_number;
 
    public:
     
        /**
         * Create a SRF08 object connected to the specified I2C pins and address
         *
         * @param SDA I2C SDA pin to connect to
         * @param SCL I2C SCL pin to connect to
         * @param i2cAddress Address of WSRF08 on I2C bus
         */
        SRF08(const int bus_number, const int i2cAddress);

        /**
         * Gets the measured range from the module
         *
         * @param returns Integer range in centimetre
         */
        int getRange();
        
        /**
         * Gets the measured light intensity from the module
         *
         * @param returns A normalised number 0-255 representing dark to light
         */
         int getLightIntensity();
         
        /**
         * Sets the range register of the SRF08 for faster ranging.
         * 
         * The max range is ((rangeVal x 43mm) + 43mm). The sensors maximum range
         *  is about six metres
         *
         * @param rangeVal The value written to the range register of the SRF08
         */
        void setRangeRegister(unsigned char rangeVal);
         
        /**
         * Sets the max gain register of the SRF08.
         * 
         * @param gainVal The value written to the max gain register of the SRF08
         */
        void setMaxGainRegister(unsigned char gainVal);
        
        /**
         * Changes the I2C address of the SRF08.
         *
         * The factory default address is 0x0E (224)
         *  The address can have the following values:
         *   E0 | E2 | E4 | E6 ... FC | FE
         * 
         * @param i2cAddress The new I2C address for the SRF08. 
         */
        void setAddress(int i2cAddress);
         
};
 
#endif