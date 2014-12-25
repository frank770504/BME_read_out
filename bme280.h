#define BME280_U8_t unsigned char
#define BME280_S8_t signed char
#define BME280_U16_t unsigned short
#define BME280_S16_t signed short
#define BME280_U32_t unsigned int
#define BME280_S32_t signed int

#define DEBUG_MODE                           0

#define SHIFT_RIGHT_4_POSITION               4
#define SHIFT_LEFT_2_POSITION                2
#define SHIFT_LEFT_4_POSITION                4
#define SHIFT_LEFT_5_POSITION                5
#define SHIFT_LEFT_8_POSITION                8
#define SHIFT_LEFT_12_POSITION               12
#define SHIFT_LEFT_16_POSITION               16
#define BME280_Four_U8X                      4
#define BME280_Zero_U8X                      0
#define BME280_Eight_U8X                     8

#define BME280_I2C_ADDR1                     0x76
#define BME280_I2C_ADDR2                     0x77
#define BME280_I2C_ADDR                      BME280_I2C_ADDR1

/* Sensor Specific constants */
#define BME280_SLEEP_MODE                    0x00
#define BME280_FORCED_MODE                   0x01
#define BME280_NORMAL_MODE                   0x03
#define BME280_SOFT_RESET_CODE               0xB6

#define BME280_STANDBYTIME_1_MS              0x00
#define BME280_STANDBYTIME_63_MS             0x01
#define BME280_STANDBYTIME_125_MS	         0x02
#define BME280_STANDBYTIME_250_MS            0x03
#define BME280_STANDBYTIME_500_MS            0x04
#define BME280_STANDBYTIME_1000_MS           0x05
#define BME280_STANDBYTIME_10_MS             0x06
#define BME280_STANDBYTIME_20_MS             0x07

#define BME280_OVERSAMPLING_SKIPPED          0x00
#define BME280_OVERSAMPLING_1X               0x01
#define BME280_OVERSAMPLING_2X               0x02
#define BME280_OVERSAMPLING_4X               0x03
#define BME280_OVERSAMPLING_8X               0x04
#define BME280_OVERSAMPLING_16X              0x05

/*calibration parameters */
#define BME280_DIG_T1_LSB_REG                0x88
#define BME280_DIG_T1_MSB_REG                0x89
#define BME280_DIG_T2_LSB_REG                0x8A
#define BME280_DIG_T2_MSB_REG                0x8B
#define BME280_DIG_T3_LSB_REG                0x8C
#define BME280_DIG_T3_MSB_REG                0x8D
#define BME280_DIG_P1_LSB_REG                0x8E
#define BME280_DIG_P1_MSB_REG                0x8F
#define BME280_DIG_P2_LSB_REG                0x90
#define BME280_DIG_P2_MSB_REG                0x91
#define BME280_DIG_P3_LSB_REG                0x92
#define BME280_DIG_P3_MSB_REG                0x93
#define BME280_DIG_P4_LSB_REG                0x94
#define BME280_DIG_P4_MSB_REG                0x95
#define BME280_DIG_P5_LSB_REG                0x96
#define BME280_DIG_P5_MSB_REG                0x97
#define BME280_DIG_P6_LSB_REG                0x98
#define BME280_DIG_P6_MSB_REG                0x99
#define BME280_DIG_P7_LSB_REG                0x9A
#define BME280_DIG_P7_MSB_REG                0x9B
#define BME280_DIG_P8_LSB_REG                0x9C
#define BME280_DIG_P8_MSB_REG                0x9D
#define BME280_DIG_P9_LSB_REG                0x9E
#define BME280_DIG_P9_MSB_REG                0x9F

#define BME280_DIG_H1_REG                    0xA1

#define BME280_DIG_H2_LSB_REG                0xE1
#define BME280_DIG_H2_MSB_REG                0xE2
#define BME280_DIG_H3_REG                    0xE3
#define BME280_DIG_H4_MSB_REG                0xE4
#define BME280_DIG_H5_LSB_H4_LSB_REG         0xE5
#define BME280_DIG_H5_MSB_REG                0xE6
#define BME280_DIG_H6_REG                    0xE7


#define BME280_CRC_REG                       0xE8

#define BME280_CHIPID_REG                    0xD0  /*Chip ID Register */
#define BME280_RESET_REG                     0xE0  /*Softreset Register */
#define BME280_STATUS_REG                    0xF3  /*Status Register */
#define BME280_CTRLMEAS_REG                  0xF4  /*Ctrl Measure Register */
#define BME280_CTRLHUM_REG                   0xF2  /*Ctrl Humidity Register*/
#define BME280_CONFIG_REG                    0xF5  /*Configuration Register */
#define BME280_PRESSURE_MSB_REG              0xF7  /*Pressure MSB Register */
#define BME280_PRESSURE_LSB_REG              0xF8  /*Pressure LSB Register */
#define BME280_PRESSURE_XLSB_REG             0xF9  /*Pressure XLSB Register */
#define BME280_TEMPERATURE_MSB_REG           0xFA  /*Temperature MSB Reg */
#define BME280_TEMPERATURE_LSB_REG           0xFB  /*Temperature LSB Reg */
#define BME280_TEMPERATURE_XLSB_REG          0xFC  /*Temperature XLSB Reg */
#define BME280_HUMIDITY_MSB_REG              0xFD  /*Humidity MSB Reg */
#define BME280_HUMIDITY_LSB_REG              0xFE  /*Humidity LSB Reg */

struct bme280_calibration_param_t {
	BME280_U16_t dig_T1;
	BME280_S16_t dig_T2;
	BME280_S16_t dig_T3;
	BME280_U16_t dig_P1;
	BME280_S16_t dig_P2;
	BME280_S16_t dig_P3;
	BME280_S16_t dig_P4;
	BME280_S16_t dig_P5;
	BME280_S16_t dig_P6;
	BME280_S16_t dig_P7;
	BME280_S16_t dig_P8;
	BME280_S16_t dig_P9;

	BME280_U8_t  dig_H1;
	BME280_S16_t dig_H2;
	BME280_U8_t  dig_H3;
	BME280_S16_t dig_H4;
	BME280_S16_t dig_H5;
	BME280_S8_t  dig_H6;

	BME280_S32_t t_fine;
};

struct bme280_t {
	struct bme280_calibration_param_t cal_param;

	unsigned char chip_id;
	unsigned char dev_addr;

	unsigned char osrs_t;
	unsigned char osrs_p;
	unsigned char osrs_h;
	unsigned char crc;
};