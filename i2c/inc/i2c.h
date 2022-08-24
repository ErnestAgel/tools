#include <stdio.h>
#include <unistd.h>
#include <string.h> /* memset */
#include <stdlib.h> /* system */
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h> /* ioctl*/
#include <fcntl.h>
#include <stdint.h>



/**
 * @brief I2C 16bit send
 *
 * @param dev          device_node: /dev/i2c-xxx
 * @param dev_addr     chip_address
 * @param reg_addr     register_address
 * @param reg_val      send register value to target
 * @return int         0:ok others:error
 * @note
 */
int IIC_16BIT_WRITE(const char *dev, uint16_t dev_addr, uint16_t reg_addr, unsigned char reg_val);

/**
 * @brief I2C 16bit recev
 *
 * @param dev          device_node: /dev/i2c-xxx
 * @param dev_addr     chip_address
 * @param reg_addr     register_address
 * @param reg_val      recev register value to target
 * @return int         0:ok others:error
 * @note
 */
int IIC_16BIT_READ(const char *dev, uint16_t dev_addr, uint16_t reg_addr, unsigned char *reg_val);

/**
 * @brief I2C 8bit send
 *
 * @param dev          device_node: /dev/i2c-xxx
 * @param dev_addr     chip_address
 * @param reg_addr     register_address
 * @param reg_val      send register value to target
 * @return int         0:ok others:error
 * @note
 */
int IIC_8BIT_WRITE(const char *dev, unsigned char dev_addr, uint16_t reg_addr, unsigned char reg_val);

/**
 * @brief I2C 8bit recev
 *
 * @param dev          device_node: /dev/i2c-xxx
 * @param dev_addr     chip_address
 * @param reg_addr     register_address
 * @param reg_val      recev register value to target
 * @return int         0:ok others:error
 * @note
 */
int IIC_8BIT_READ(const char *dev, unsigned char dev_addr, uint16_t reg_addr, unsigned char *reg_val);

int IIC_32BIT_WRITE(const char *dev, uint16_t dev_addr, uint16_t reg_addr, uint16_t reg_val);

int IIC_32BIT_READ(const char *dev, uint16_t dev_addr, uint16_t reg_addr, uint16_t *reg_val);

