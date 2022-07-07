#include <iostream>
#include <unistd.h> //sleep() func

#include <wiringPi.h>
#include <wiringPiI2C.h>

using namespace std;

enum class REG_CMD : int
{
    // Config Register (R/W)
    REG_CONFIG = 0x00,
    // SHUNT VOLTAGE REGISTER (R)
    REG_SHUNTVOLTAGE = 0x01,

    // BUS VOLTAGE REGISTER (R)
    REG_BUSVOLTAGE = 0x02,

    // POWER REGISTER (R)
    REG_POWER = 0x03,

    // CURRENT REGISTER (R)
    REG_CURRENT = 0x04,

    // CALIBRATION REGISTER (R/W)
    REG_CALIBRATION = 0x05

};

enum class BusVoltageRange : int
{
    //    """Constants for ``bus_voltage_range``"""
    RANGE_16V = 0x00, // set bus voltage range to 16V
    RANGE_32V = 0x01  // set bus voltage range to 32V (default)
};

enum class Gain : int
{
    //    """Constants for ``gain``"""
    DIV_1_40MV = 0x00,  // shunt prog. gain set to  1, 40 mV range
    DIV_2_80MV = 0x01,  // shunt prog. gain set to /2, 80 mV range
    DIV_4_160MV = 0x02, // shunt prog. gain set to /4, 160 mV range
    DIV_8_320MV = 0x03  // shunt prog. gain set to /8, 320 mV range
};

enum class ADCResolution : int
{
    //   """Constants for ``bus_adc_resolution`` or ``shunt_adc_resolution``"""
    ADCRES_9BIT_1S = 0x00,   //  9bit,   1 sample,     84us
    ADCRES_10BIT_1S = 0x01,  // 10 bit,   1 sample,    148us
    ADCRES_11BIT_1S = 0x02,  // 11 bit,  1 sample,    276us
    ADCRES_12BIT_1S = 0x03,  // 12 bit,  1 sample,    532us
    ADCRES_12BIT_2S = 0x09,  // 12 bit,  2 samples,  1.06ms
    ADCRES_12BIT_4S = 0x0A,  // 12 bit,  4 samples,  2.13ms
    ADCRES_12BIT_8S = 0x0B,  // 12 bit,   8 samples,  4.26ms
    ADCRES_12BIT_16S = 0x0C, // 12 bit,  16 samples,  8.51ms
    ADCRES_12BIT_32S = 0x0D, // 12 bit,  32 samples, 17.02ms
    ADCRES_12BIT_64S = 0x0E, // 12 bit,  64 samples, 34.05ms
    ADCRES_12BIT_128S = 0x0F // 12 bit, 128 samples, 68.10ms
};

enum class Mode : int
{
    //    """Constants for ``mode``"""
    POWERDOW = 0x00,            // power down
    SVOLT_TRIGGERED = 0x01,     // shunt voltage triggered
    BVOLT_TRIGGERED = 0x02,     // bus voltage triggered
    SANDBVOLT_TRIGGERED = 0x03, // shunt and bus voltage triggered
    ADCOFF = 0x04,              // ADC off
    SVOLT_CONTINUOUS = 0x05,    // shunt voltage continuous
    BVOLT_CONTINUOUS = 0x06,    // bus voltage continuous
    SANDBVOLT_CONTINUOUS = 0x07 // shunt and bus voltage continuous
};

class INA219
{
public:
    INA219(int addr = 0x42);
    double getShuntVoltage_mV();
    double getBusVoltage_V();
    double getCurrent_mA();
    double getPower_W();

private:
    int i2c_addr;
    int i2c_fd;
    double current_lsb;
    int cal_value;
    double power_lsb;
    int bus_voltage_range;
    int gain;
    int bus_adc_resolution;
    int shunt_adc_resolution;
    int mode;
    int config;
    void set_calibration_32V_2A();
};

INA219::INA219(int addr)
{
    i2c_addr = addr;
    current_lsb = 0.0;
    cal_value = 0;
    power_lsb = 0.0;
    wiringPiSetup(); //初始化wiringPi库
    i2c_fd = wiringPiI2CSetup(i2c_addr);
    set_calibration_32V_2A();
}

void INA219::set_calibration_32V_2A()
{
    /*
    """Configures to INA219 to be able to measure up to 32V and 2A of current. Counter
    overflow occurs at 3.2A.
    ..note :: These calculations assume a 0.1 shunt ohm resistor is present
    """
    # By default we use a pretty huge range for the input voltage,
    # which probably isn't the most appropriate choice for system
    # that don't use a lot of power.  But all of the calculations
    # are shown below if you want to change the settings.  You will
    # also need to change any relevant register settings, such as
    # setting the VBUS_MAX to 16V instead of 32V, etc.

    # VBUS_MAX = 32V             (Assumes 32V, can also be set to 16V)
    # VSHUNT_MAX = 0.32          (Assumes Gain 8, 320mV, can also be 0.16, 0.08, 0.04)
    # RSHUNT = 0.1               (Resistor value in ohms)

    # 1. Determine max possible current
    # MaxPossible_I = VSHUNT_MAX / RSHUNT
    # MaxPossible_I = 3.2A

    # 2. Determine max expected current
    # MaxExpected_I = 2.0A

    # 3. Calculate possible range of LSBs (Min = 15-bit, Max = 12-bit)
    # MinimumLSB = MaxExpected_I/32767
    # MinimumLSB = 0.000061              (61uA per bit)
    # MaximumLSB = MaxExpected_I/4096
    # MaximumLSB = 0,000488              (488uA per bit)

    # 4. Choose an LSB between the min and max values
    #    (Preferrably a roundish number close to MinLSB)
    # CurrentLSB = 0.0001 (100uA per bit)
    */
    this->current_lsb = 0.1;
    /*
    # 5. Compute the calibration register
    # Cal = trunc (0.04096 / (Current_LSB * RSHUNT))
    # Cal = 4096 (0x1000)
    */
    this->cal_value = 4096;
    /*
    # 6. Calculate the power LSB
    # PowerLSB = 20 * CurrentLSB
    # PowerLSB = 0.002 (2mW per bit)
    */
    this->power_lsb = 0.002;
    /*
    # 7. Compute the maximum current and shunt voltage values before overflow
    #
    # Max_Current = Current_LSB * 32767
    # Max_Current = 3.2767A before overflow
    #
    # If Max_Current > Max_Possible_I then
    #    Max_Current_Before_Overflow = MaxPossible_I
    # Else
    #    Max_Current_Before_Overflow = Max_Current
    # End If
    #
    # Max_ShuntVoltage = Max_Current_Before_Overflow * RSHUNT
    # Max_ShuntVoltage = 0.32V
    #
    # If Max_ShuntVoltage >= VSHUNT_MAX
    #    Max_ShuntVoltage_Before_Overflow = VSHUNT_MAX
    # Else
    #    Max_ShuntVoltage_Before_Overflow = Max_ShuntVoltage
    # End If

    # 8. Compute the Maximum Power
    # MaximumPower = Max_Current_Before_Overflow * VBUS_MAX
    # MaximumPower = 3.2 * 32V
    # MaximumPower = 102.4W

    # Set Calibration register to 'Cal' calculated above
    */
    wiringPiI2CWriteReg16(i2c_fd, static_cast<int>(REG_CMD::REG_CALIBRATION), this->cal_value);
    // Set Config register to take into account the settings above
    this->bus_voltage_range = static_cast<int>(BusVoltageRange::RANGE_32V);
    this->gain = static_cast<int>(Gain::DIV_8_320MV);
    this->bus_adc_resolution = static_cast<int>(ADCResolution::ADCRES_12BIT_32S);
    this->shunt_adc_resolution = static_cast<int>(ADCResolution::ADCRES_12BIT_32S);
    this->mode = static_cast<int>(Mode::SANDBVOLT_CONTINUOUS);
    this->config = (this->bus_voltage_range << 13 |
                    this->gain << 11 |
                    this->bus_adc_resolution << 7 |
                    this->shunt_adc_resolution << 3 |
                    this->mode);

    wiringPiI2CWriteReg16(i2c_fd, static_cast<int>(REG_CMD::REG_CONFIG), this->config);
}

double INA219::getShuntVoltage_mV()
{
    int value;
    wiringPiI2CWriteReg16(i2c_fd, static_cast<int>(REG_CMD::REG_CALIBRATION), this->cal_value);
    value = wiringPiI2CReadReg16(i2c_fd, static_cast<int>(REG_CMD::REG_SHUNTVOLTAGE));
    if (value > 32767)
    {
        value -= 65535;
    }
    return (value * 0.01);
}

double INA219::getBusVoltage_V()
{
    int value;
    wiringPiI2CWriteReg16(i2c_fd, static_cast<int>(REG_CMD::REG_CALIBRATION), this->cal_value);
    value = wiringPiI2CReadReg16(i2c_fd, static_cast<int>(REG_CMD::REG_BUSVOLTAGE));
    //return ((value >> 3) * 0.004);
    return (value * 0.001);
}

double INA219::getCurrent_mA()
{
    int value;
    wiringPiI2CWriteReg16(i2c_fd, static_cast<int>(REG_CMD::REG_CALIBRATION), this->cal_value);
    value = wiringPiI2CReadReg16(i2c_fd, static_cast<int>(REG_CMD::REG_CURRENT));
    if (value > 32767)
    {
        value -= 65535;
    }
    return (value * this->current_lsb);
}

double INA219::getPower_W()
{
    int value;

    wiringPiI2CWriteReg16(i2c_fd, static_cast<int>(REG_CMD::REG_CALIBRATION), this->cal_value);
    // wiringPiI2CWriteReg16(i2c_fd, REG_CMD_CAL, this->cal_value);

    value = wiringPiI2CReadReg16(i2c_fd, static_cast<int>(REG_CMD::REG_POWER));
    if (value > 32767)
    {
        value -= 65535;
    }
    return (value * this->power_lsb);
}

int main()
{
    double bus_voltage;
    double shunt_voltage;
    double current;
    double power;
    double persent;
    // init
    INA219 ina219(0x42);
    while (1)
    {
        bus_voltage = ina219.getBusVoltage_V();             // voltage on V- (load side)
        shunt_voltage = ina219.getShuntVoltage_mV() / 1000; // voltage between V+ and V- across the shunt
        current = ina219.getCurrent_mA();                   // current in mA
        power = ina219.getPower_W();
        persent = (bus_voltage - 6) / 2.4 * 100;
        if (persent > 100.0)
        {
            persent = 100.0;
        }
        if (persent < 0.0)
        {
            persent = 0.0;
        }
        cout << "Load voltage: " << bus_voltage  << "V" << endl;
        cout << "Shunt voltage: " << shunt_voltage << "V" << endl;
        cout << "Current: " << (current / 1000.0) << "A" << endl;
        cout << "Power: " << power << "W" << endl;
        cout << "Persent: " << persent << "%" << endl;
        // 2秒后，刷新一次
        sleep(2);
    }
}
