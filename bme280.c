#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <bme280.h>
#include <netinet/in.h>

#define I2C_DEV                 "/dev/i2c-2"
#define I2C_MASTER_ERROR        -1
#define I2C_MASTER_SUCCESS      0
#define MAX_BUF                 100

#define GPIO_DIRECTION_OUT      0
#define GPIO_DIRECTION_IN       1
#define GPIO_STATUS_LOW         0
#define GPIO_STATUS_HIGH        1

static struct bme280_t *p_bme280;

int i2c_write(unsigned char dev_addr, unsigned char *buf, int length);
int i2c_read(unsigned char dev_addr, unsigned char *buf, int length);
int transport_read( unsigned char *buf, int len );
int transport_write( unsigned char *buf, int len );
char msg[100];

int i2c_write(unsigned char dev_addr, unsigned char *buf, int length)
{
    int fd = 0;
    fd = open(I2C_DEV, O_RDWR); //open "/dev/i2c-2"
    //printf("command : %d,%d\nstm32 i2c slave address:%d\n",buf[0],buf[1],dev_addr);
    if (fd < 0) {
        printf("Error opening file: %s\n", strerror(errno));
        return 1;
    }

    ioctl(fd,I2C_TENBIT,0);
    if (ioctl(fd, I2C_SLAVE, dev_addr) < 0) {
        printf("ioctl error: %s\n", strerror(errno));
        return 1;
    }

    if (write(fd, buf, length) != length) {
        printf("Error writing file: %s\n", strerror(errno));
        return 1;
    }

    close(fd);
    return 0;
}

int i2c_read( unsigned char dev_addr,unsigned char *buf, int length)
{
    int fd = 0;
    fd = open(I2C_DEV, O_RDWR); //open "/dev/i2c-2"

    if (fd < 0) {
        printf("Error opening file: %s\n", strerror(errno));
        return 1;
    }

    ioctl(fd,I2C_TENBIT,0);
    if (ioctl(fd, I2C_SLAVE, dev_addr) < 0) {
        printf("ioctl error: %s\n", strerror(errno));
        return 1;
    }
    if (read(fd, buf, length) != length) {
        printf("Error reading file: %s\n", strerror(errno));
        return 1;
    }

    close(fd);
    return 0;
}

int transport_read( unsigned char *buf,  int len )
{
    return i2c_read(BME280_I2C_ADDR,buf,len);
}

int transport_write( unsigned char *buf, int len )
{
    return i2c_write(BME280_I2C_ADDR,buf,len);
}

int bme280_read_all_uncompensation_data(BME280_S32_t *u_temperature, BME280_S32_t *u_pressure, BME280_S32_t *u_humidity)
{
    unsigned char a_data_u8r[8];

    a_data_u8r[0] = BME280_PRESSURE_MSB_REG;

	if(transport_write(a_data_u8r,1))
		return 1;
	if(transport_read(a_data_u8r,8)) // get data from sensor
	    return 1;

    *u_pressure = (BME280_S32_t)((
            ((BME280_U32_t)(a_data_u8r[0]))
            << SHIFT_LEFT_12_POSITION) |
            (((BME280_U32_t)(a_data_u8r[1]))
            << SHIFT_LEFT_4_POSITION) |
            ((BME280_U32_t)a_data_u8r[2] >>
            SHIFT_RIGHT_4_POSITION));

    *u_temperature = (BME280_S32_t)(((
            (BME280_U32_t) (a_data_u8r[3]))
            << SHIFT_LEFT_12_POSITION) |
            (((BME280_U32_t)(a_data_u8r[4]))
            << SHIFT_LEFT_4_POSITION)
            | ((BME280_U32_t)a_data_u8r[5] >>
            SHIFT_RIGHT_4_POSITION));

    *u_humidity = (BME280_S32_t)(
            (((BME280_U32_t)(a_data_u8r[6]))
            << SHIFT_LEFT_8_POSITION)|
            ((BME280_U32_t)(a_data_u8r[7])));

    return 0;
}

int bme280_get_compensation_parameter(void)
{
    unsigned char a_data_u8r[26];

	a_data_u8r[0] = BME280_DIG_T1_LSB_REG;
	if(transport_write(a_data_u8r,1))
		return 1;
    if(transport_read(a_data_u8r,26)) // get data from sensor
	    return 1;

    p_bme280->cal_param.dig_T1 = (BME280_U16_t)(((
        (BME280_U16_t)((unsigned char)a_data_u8r[1])) <<
        SHIFT_LEFT_8_POSITION) | a_data_u8r[0]);

    p_bme280->cal_param.dig_T2 = (BME280_S16_t)(((
        (BME280_S16_t)((signed char)a_data_u8r[3])) <<
        SHIFT_LEFT_8_POSITION) | a_data_u8r[2]);

    p_bme280->cal_param.dig_T3 = (BME280_S16_t)(((
        (BME280_S16_t)((signed char)a_data_u8r[5])) <<
        SHIFT_LEFT_8_POSITION) | a_data_u8r[4]);

    p_bme280->cal_param.dig_P1 = (BME280_U16_t)(((
        (BME280_U16_t)((unsigned char)a_data_u8r[7])) <<
        SHIFT_LEFT_8_POSITION) | a_data_u8r[6]);

    p_bme280->cal_param.dig_P2 = (BME280_S16_t)(((
        (BME280_S16_t)((signed char)a_data_u8r[9])) <<
        SHIFT_LEFT_8_POSITION) | a_data_u8r[8]);

    p_bme280->cal_param.dig_P3 = (BME280_S16_t)(((
        (BME280_S16_t)((signed char)a_data_u8r[11])) <<
        SHIFT_LEFT_8_POSITION) | a_data_u8r[10]);

    p_bme280->cal_param.dig_P4 = (BME280_S16_t)(((
        (BME280_S16_t)((signed char)a_data_u8r[13])) <<
        SHIFT_LEFT_8_POSITION) | a_data_u8r[12]);

    p_bme280->cal_param.dig_P5 = (BME280_S16_t)(((
        (BME280_S16_t)((signed char)a_data_u8r[15])) <<
        SHIFT_LEFT_8_POSITION) | a_data_u8r[14]);

    p_bme280->cal_param.dig_P6 = (BME280_S16_t)(((
        (BME280_S16_t)((signed char)a_data_u8r[17])) <<
        SHIFT_LEFT_8_POSITION) | a_data_u8r[16]);

    p_bme280->cal_param.dig_P7 = (BME280_S16_t)(((
        (BME280_S16_t)((signed char)a_data_u8r[19])) <<
        SHIFT_LEFT_8_POSITION) | a_data_u8r[18]);

    p_bme280->cal_param.dig_P8 = (BME280_S16_t)(((
        (BME280_S16_t)((signed char)a_data_u8r[21])) <<
        SHIFT_LEFT_8_POSITION) | a_data_u8r[20]);

    p_bme280->cal_param.dig_P9 = (BME280_S16_t)(((
        (BME280_S16_t)((signed char)a_data_u8r[23])) <<
        SHIFT_LEFT_8_POSITION) | a_data_u8r[22]);

    p_bme280->cal_param.dig_H1 = a_data_u8r[25];

    a_data_u8r[0] = BME280_DIG_H2_LSB_REG;
    if(transport_write(a_data_u8r,1))
	return 1;
    if(transport_read(a_data_u8r,8)) // get data from sensor
        return 1;

    p_bme280->cal_param.dig_H2 = (BME280_S16_t)(((
        (BME280_S16_t)((signed char)a_data_u8r[1])) <<
        SHIFT_LEFT_8_POSITION) | a_data_u8r[0]);

    p_bme280->cal_param.dig_H3 = a_data_u8r[2];

    p_bme280->cal_param.dig_H4 = (BME280_S16_t)(((
        (BME280_S16_t)((signed char)a_data_u8r[3])) <<
        SHIFT_LEFT_4_POSITION) | (((unsigned char)0x0F)
        & a_data_u8r[4]));

    p_bme280->cal_param.dig_H5 = (BME280_S16_t)(((
        (BME280_S16_t)((signed char)a_data_u8r[5])) <<
        SHIFT_LEFT_4_POSITION) | (a_data_u8r[4] >>
        SHIFT_RIGHT_4_POSITION));

    p_bme280->cal_param.dig_H6 = (signed char)a_data_u8r[6];

    p_bme280->crc = a_data_u8r[7];

    return 0;
}

double bme280_compensate_T_double(BME280_S32_t adc_t)
{
    double v_x1_u32r = BME280_Zero_U8X;
    double v_x2_u32r = BME280_Zero_U8X;
    double temperature = BME280_Zero_U8X;

    v_x1_u32r  = (((double)adc_t) / 16384.0 -
    ((double)p_bme280->cal_param.dig_T1) / 1024.0) *
    ((double)p_bme280->cal_param.dig_T2);
    v_x2_u32r  = ((((double)adc_t) / 131072.0 -
    ((double)p_bme280->cal_param.dig_T1) / 8192.0) *
    (((double)adc_t) / 131072.0 -
    ((double)p_bme280->cal_param.dig_T1) / 8192.0)) *
    ((double)p_bme280->cal_param.dig_T3);
    p_bme280->cal_param.t_fine = (BME280_S32_t)(v_x1_u32r + v_x2_u32r);
    temperature  = (v_x1_u32r + v_x2_u32r) / 5120.0;

    return temperature;
}

double bme280_compensate_P_double(BME280_S32_t adc_p)
{
    double v_x1_u32r = BME280_Zero_U8X;
    double v_x2_u32r = BME280_Zero_U8X;
    double pressure = BME280_Zero_U8X;

    v_x1_u32r = ((double)p_bme280->cal_param.t_fine/2.0) - 64000.0;
    v_x2_u32r = v_x1_u32r * v_x1_u32r *
    ((double)p_bme280->cal_param.dig_P6) / 32768.0;
    v_x2_u32r = v_x2_u32r + v_x1_u32r *
    ((double)p_bme280->cal_param.dig_P5) * 2.0;
    v_x2_u32r = (v_x2_u32r / 4.0) +
    (((double)p_bme280->cal_param.dig_P4) * 65536.0);
    v_x1_u32r = (((double)p_bme280->cal_param.dig_P3) *
    v_x1_u32r * v_x1_u32r / 524288.0 +
    ((double)p_bme280->cal_param.dig_P2) * v_x1_u32r) / 524288.0;
    v_x1_u32r = (1.0 + v_x1_u32r / 32768.0) *
    ((double)p_bme280->cal_param.dig_P1);
    if (v_x1_u32r == 0.0)
        return 0;
        /* Avoid exception caused by division by zero */
    pressure = 1048576.0 - (double)adc_p;
    pressure = (pressure - (v_x2_u32r / 4096.0)) * 6250.0 / v_x1_u32r;
    v_x1_u32r = ((double)p_bme280->cal_param.dig_P9) *
    pressure * pressure / 2147483648.0;
    v_x2_u32r = pressure * ((double)p_bme280->cal_param.dig_P8) / 32768.0;
    pressure = pressure + (v_x1_u32r + v_x2_u32r +
    ((double)p_bme280->cal_param.dig_P7)) / 16.0;

    return pressure;
}

double bme280_compensate_H_double(BME280_S32_t adc_h)
{
    double var_h = BME280_Zero_U8X;
    var_h = (((double)p_bme280->cal_param.t_fine)-76800.0);
    var_h = (adc_h-(((double)p_bme280->cal_param.dig_H4)*64.0 +
    ((double)p_bme280->cal_param.dig_H5) / 16384.0 * var_h))*
    (((double)p_bme280->cal_param.dig_H2)/65536.0*(1.0 + ((double)
    p_bme280->cal_param.dig_H6)/67108864.0*var_h*(1.0+((double)
    p_bme280->cal_param.dig_H3)/67108864.0*var_h)));
    var_h = var_h * (1.0-((double)
    p_bme280->cal_param.dig_H1)*var_h/524288.0);
    if (var_h > 100.0)
        var_h = 100.0;
    else if (var_h < 0.0)
        var_h = 0.0;

	return var_h;
}

int bme280_read_all_compensation_data(double *temperature, double *pressure, double *humidity)
{
    BME280_S32_t u_temperature = 0;
    BME280_S32_t u_pressure = 0;
    BME280_S32_t u_humidity = 0;

	if(bme280_read_all_uncompensation_data(&u_temperature, &u_pressure, &u_humidity))
	{
	    printf("Fail to read raw data\n");
		return 1;
	}

	*temperature = bme280_compensate_T_double(u_temperature);
	*pressure = bme280_compensate_P_double(u_pressure);
	*humidity = bme280_compensate_H_double(u_humidity);
	return 0;
}

int bme280_get_mode(void)
{
    unsigned char a_data_u8r[1];
	a_data_u8r[0] = BME280_CTRLMEAS_REG;
	if(transport_write(a_data_u8r,1))
		return 1;
	if(transport_read(a_data_u8r,1))
		return 1;
	printf("----------------------\nMODE = %d\n----------------------\n",a_data_u8r[0]);

    return 0;
}

int bme280_set_mode(void)
{
    unsigned char a_data_u8r[2];
	a_data_u8r[0] = BME280_CTRLMEAS_REG;
	a_data_u8r[1] = 0x37;//0x93;
	if(transport_write(a_data_u8r,2))
		return 1;

    return 0;
}

int bme280_init(struct bme280_t *bme280)
{
	p_bme280 = bme280;

	unsigned char a_data_u8r[1];
	a_data_u8r[0] = BME280_CHIPID_REG;

	if(transport_write(a_data_u8r,1))
		return 1;

	if(transport_read(a_data_u8r,1))
	    return 1;

	p_bme280->chip_id = a_data_u8r[0];
	printf("----------------------\nCHIP ID = %d\n----------------------\n",a_data_u8r[0]);

	bme280_set_mode();
	bme280_get_mode();

	if(bme280_get_compensation_parameter())
	{
	    printf("Fail to get compensation parameter\n");
		return 1;
    }
	return 0;
}

int bme280_read_data()
{
	double temperature = 0;
	double pressure = 0;
	double humidity = 0;

	if(DEBUG_MODE)
	{
	    snprintf(msg, sizeof(msg), "%f\t%f\t%f", temperature, pressure, humidity);
	    return 0;
	}


	if(bme280_read_all_compensation_data(&temperature, &pressure, &humidity))
		return 1;

    bzero(&msg, sizeof(msg));
    snprintf(msg, sizeof(msg), "%f\t%f\t%f", temperature, pressure, humidity);
	//snprintf(msg, sizeof(msg), "Temperature : %f DegC\nPressure : %f Pa\nHumidity : %f rH\n", temperature, pressure, humidity);
	return 0;
}


int socket_server(void)
{
	int sockfd;
	struct sockaddr_in dest;
	//char buffer[] = "";

	/* create socket , same as client */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	/* initialize structure dest */
	bzero(&dest, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(8889);
	/* this line is different from client */
	dest.sin_addr.s_addr = INADDR_ANY;

	/* Assign a port number to socket */
	if(bind(sockfd, (struct sockaddr*)&dest, sizeof(dest)) < 0)
        return 1;
	/* make it listen to socket with max 5 connections */
	listen(sockfd, 5);

	printf("server starting...\n");

	/* infinity loop -- accepting connection from client forever */
	while(1)
	{
		int clientfd;
		struct sockaddr_in client_addr;
		int addrlen = sizeof(client_addr);

		/* Wait and Accept connection */
		clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);
        if(clientfd < 0)
		{
		    printf("accept failed\n");
			return 1;
		}

		if(!bme280_read_data())
		{
		    /* Send message */
		    if(send(clientfd, msg, sizeof(msg), 0) < 0)
			    return 1;

            printf("send:\n%s\n", msg);
		}
		/* close(client) */
		close(clientfd);
	}

	/* close(server) , but never get here because of the loop */
	close(sockfd);
	return 0;
}

int main(void)
{
	printf("bme280 test tool\n");
	struct bme280_t bme280;
	int res = 0;
	res = bme280_init(&bme280);
	while(1)
	{
		bme280_read_data();
		usleep((useconds_t)(500000));
	}
    return 0;
}
