/**********************************************************************************
*
* 版权信息：版权所有 (c) 2019, 杭州海康威视数字技术股份有限公司, 保留所有权利
*
* 文件名称：i2c-tools.c
* 摘    要：

* 当前版本：1.0.0
* 作    者：Ernest
* 日    期：2022年02月23日
* 备    注：1.Creat

************************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <string.h> /* memset */
#include <stdlib.h> /* system */
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h> /* ioctl*/
#include <fcntl.h>
#include <stdint.h>

#include "i2c.h"

#define MS_sleep(T)		usleep(T * 1000)

#define I2C_ERR(...)                                 \
    printf("[I2C:%s]LINE: %d ", __func__, __LINE__); \
    printf(__VA_ARGS__)


int IIC_32BIT_WRITE(const char *dev, uint16_t dev_addr, uint16_t reg_addr, uint16_t reg_val)
{
    int fd = 0;
    int ret = 0;
    unsigned char buf[4] = {0};

    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[1];

    messages[0].addr = dev_addr;
    messages[0].flags = 0;
    messages[0].len = sizeof(buf);
    messages[0].buf = buf;

    buf[1] = (unsigned char)(0xFF & reg_addr);
    buf[0] = (unsigned char)(0xFF & ( reg_addr >> 8));

    buf[2] = (unsigned char)(0xFF & ( reg_val >> 8));
    buf[3] = (unsigned char)(0xFF & reg_val);

    packets.msgs = messages;
    packets.nmsgs = 1;

    fd = open(dev, O_RDWR);
    if (fd < 0)
    {
        I2C_ERR("open :%s dev failed fd: %d\n", dev, fd);
        ret = -1;
    }
    else
    {
        ret = ioctl(fd, I2C_TIMEOUT, 1); /* timeout*/
        if (ret != 0)
        {
            I2C_ERR("config :%s dev failed fd: %d\n", dev, fd);
            return ret;
        }
        ret = ioctl(fd, I2C_RETRIES, 3); /* retry once*/
        if (ret != 0)
        {
            I2C_ERR("config :%s dev failed fd: %d\n", dev, fd);
            return ret;
        }

        ret = ioctl(fd, I2C_RDWR, (unsigned long)&packets);
        if (ret < 0)
        {
            I2C_ERR("Unable to Write dev:0x%x reg:0x%x reg_val:0x%x ret:%d!!!\n", dev_addr, reg_addr, reg_val, ret);
            perror("Unbale to Write regs");
        }
        else
        {
            ret = 0;
        }
    }
    MS_sleep(1);
    close(fd);
    return ret;
}

int IIC_32BIT_READ(const char *dev, uint16_t dev_addr, uint16_t reg_addr, uint16_t *reg_val)
{
    int fd = 0;
    int ret = 0;

    uint16_t data = 0U;
    unsigned char inbuf[2] = {0};
    unsigned char outbuf[2];
    
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];
    inbuf[0] = (unsigned char)(0xFF & ( *reg_val >> 8));
    inbuf[1] = (unsigned char)(0xFF & ( *reg_val));

    memset(&packets, 0, sizeof(struct i2c_rdwr_ioctl_data));
    memset(&messages, 0, sizeof(struct i2c_msg));

    outbuf[0] = (unsigned char)(0xFF & (reg_addr >> 8));
    outbuf[1] = (unsigned char)(0xFF & (reg_addr));

    messages[0].addr = dev_addr;
    messages[0].flags = 0;
    messages[0].len = sizeof(outbuf);
    messages[0].buf = outbuf;

    messages[1].addr = dev_addr;
    messages[1].flags = I2C_M_RD;
    messages[1].len = sizeof(inbuf);
    messages[1].buf = &inbuf[0];

    packets.msgs = messages;
    packets.nmsgs = 2;

    fd = open(dev, O_RDWR);
    if (fd < 0)
    {
        I2C_ERR("open :%s dev failed fd: %d\n", dev, fd);
        ret = -1;
    }
    else
    {
        ret = ioctl(fd, I2C_TIMEOUT, 1); /* timeout*/
        if (ret != 0)
        {
            I2C_ERR("config :%s dev failed fd: %d ret:%d\n", dev, fd, ret);
            return ret;
        }
        ret = ioctl(fd, I2C_RETRIES, 3); /* retry once*/
        if (ret != 0)
        {
            I2C_ERR("config :%s dev failed fd: %d ret:%d\n", dev, fd, ret);
            return ret;
        }

        ret = ioctl(fd, I2C_RDWR, (unsigned long)&packets);
        if (ret < 0)
        {
            I2C_ERR("Unable to Read dev:0x%x reg:0x%x  ret:%d!!!\n", dev_addr, reg_addr, ret);
            perror("Unbale to Read regs");
        }
        else
        {
            data = data | inbuf[0];
            data = data << 8;
            data = data | inbuf[1];

            *reg_val = data;
            ret = 0;
        }
    }
    MS_sleep(1);
    close(fd);
    return ret;
}

int IIC_16BIT_WRITE(const char *dev, uint16_t dev_addr, uint16_t reg_addr, unsigned char reg_val)
{
    int fd = 0;
    int ret = 0;
    unsigned char buf[3] = {0};

    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[1];

    messages[0].addr = dev_addr;
    messages[0].flags = 0;
    messages[0].len = sizeof(buf);
    messages[0].buf = buf;

    buf[1] = (unsigned char)(0xFF & reg_addr);
    buf[0] = (unsigned char)(0xFF & (reg_addr >> 8));

    buf[2] = reg_val;

    packets.msgs = messages;
    packets.nmsgs = 1;

    fd = open(dev, O_RDWR);
    if (fd < 0)
    {
        I2C_ERR("open :%s dev failed fd: %d\n", dev, fd);
        ret = -1;
    }
    else
    {
        ret = ioctl(fd, I2C_TIMEOUT, 1); /* timeout*/
        if (ret != 0)
        {
            I2C_ERR("config :%s dev failed fd: %d\n", dev, fd);
            return ret;
        }
        ret = ioctl(fd, I2C_RETRIES, 3); /* retry once*/
        if (ret != 0)
        {
            I2C_ERR("config :%s dev failed fd: %d\n", dev, fd);
            return ret;
        }

        ret = ioctl(fd, I2C_RDWR, (unsigned long)&packets);
        if (ret < 0)
        {
            I2C_ERR("Unable to Write dev:0x%x reg:0x%x reg_val:0x%x ret:%d!!!\n", dev_addr, reg_addr, reg_val, ret);
            perror("Unbale to Write regs");
        }
        else
        {
            ret = 0;
        }
    }
    MS_sleep(1);
    close(fd);
    return ret;
}


int IIC_16BIT_READ(const char *dev, uint16_t dev_addr, uint16_t reg_addr, unsigned char *reg_val)
{
    int fd = 0;
    int ret = 0;
    unsigned char inbuf;
    unsigned char outbuf[2];
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];
    
    memset(&packets, 0, sizeof(struct i2c_rdwr_ioctl_data));
    memset(&messages, 0, sizeof(struct i2c_msg));

    outbuf[1] = (unsigned char)(0xFF & reg_addr);
    outbuf[0] = (unsigned char)(0xFF & (reg_addr >> 8));

    messages[0].addr = dev_addr;
    messages[0].flags = 0;
    messages[0].len = sizeof(outbuf);
    messages[0].buf = outbuf;

    messages[1].addr = dev_addr;
    messages[1].flags = I2C_M_RD;
    messages[1].len = sizeof(inbuf);
    messages[1].buf = &inbuf;

    packets.msgs = messages;
    packets.nmsgs = 2;

    fd = open(dev, O_RDWR);
    if (fd < 0)
    {
        I2C_ERR("open :%s dev failed fd: %d\n", dev, fd);
        ret = -1;
    }
    else
    {
        ret = ioctl(fd, I2C_TIMEOUT, 1); /* timeout*/
        if (ret != 0)
        {
            I2C_ERR("config :%s dev failed fd: %d ret:%d\n", dev, fd, ret);
            return ret;
        }
        ret = ioctl(fd, I2C_RETRIES, 3); /* retry once*/
        if (ret != 0)
        {
            I2C_ERR("config :%s dev failed fd: %d ret:%d\n", dev, fd, ret);
            return ret;
        }

        ret = ioctl(fd, I2C_RDWR, (unsigned long)&packets);
        if (ret < 0)
        {
            I2C_ERR("Unable to Read dev:0x%x reg:0x%x  ret:%d!!!\n", dev_addr, reg_addr, ret);
            perror("Unbale to Read regs");
        }
        else
        {
            *reg_val = inbuf;
            ret = 0;
        }
    }
    MS_sleep(1);
    close(fd);
    return ret;
}


int IIC_8BIT_WRITE(const char *dev, unsigned char dev_addr, uint16_t reg_addr, unsigned char reg_val)
{
    int fd = 0;
    int ret = 0;
    unsigned char buf[2];

    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[1];

    memset(&packets, 0, sizeof(struct i2c_rdwr_ioctl_data));
    memset(&messages, 0, sizeof(struct i2c_msg));

    messages[0].addr = dev_addr;
    messages[0].flags = 0;
    messages[0].len = sizeof(buf);
    messages[0].buf = buf;

    buf[0] = reg_addr;
    buf[1] = reg_val;

    packets.nmsgs = 1;
    packets.msgs = messages;

    fd = open(dev, O_RDWR);
    if (fd < 0)
    {
        I2C_ERR("open :%s dev failed fd: %d\n", dev, fd);
        ret = -1;
    }
    else
    {
        ret = ioctl(fd, I2C_TIMEOUT, 1); /* timeout*/
        if (ret != 0)
        {
            I2C_ERR("config :%s dev failed fd: %d\n", dev, fd);
            return ret;
        }
        ret = ioctl(fd, I2C_RETRIES, 3); /* retry times*/
        if (ret != 0)
        {
            I2C_ERR("config :%s dev failed fd: %d\n", dev, fd);
            return ret;
        }

        ret = ioctl(fd, I2C_RDWR, (unsigned long)&packets);
        if (ret < 0)
        {
            I2C_ERR("Unable to Write dev:0x%x reg:0x%x reg_val:0x%x ret:%d!!!\n", dev_addr, reg_addr, reg_val, ret);
            perror("Unbale to Write regs");
        }
        else
        {
            ret = 0;
        }
    }

    MS_sleep(1);
    close(fd);
    return ret;
}


int IIC_8BIT_READ(const char *dev, unsigned char dev_addr, uint16_t reg_addr, unsigned char *reg_val)
{
    int fd = 0;
    int ret = 0;

    unsigned char inbuf = 0;
    unsigned char outbuf = 0;

    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];

    memset(&packets, 0, sizeof(struct i2c_rdwr_ioctl_data));
    memset(&messages, 0, sizeof(struct i2c_msg));

    messages[0].addr = dev_addr;
    messages[0].flags = 0;
    messages[0].len = sizeof(outbuf);
    messages[0].buf = &outbuf;

    messages[1].addr = dev_addr;
    messages[1].flags = I2C_M_RD;
    messages[1].len = sizeof(inbuf);
    messages[1].buf = &inbuf;

    packets.msgs = messages;
    packets.nmsgs = 2;

    fd = open(dev, O_RDWR);
    if (fd < 0)
    {
        I2C_ERR("open :%s dev failed fd: %d\n", dev, fd);
        ret = -1;
    }
    else
    {
        ret = ioctl(fd, I2C_TIMEOUT, 1); /* timeout*/
        if (ret != 0)
        {
            I2C_ERR("config :%s dev failed fd: %d ret:%d\n", dev, fd, ret);
            return ret;
        }
        ret = ioctl(fd, I2C_RETRIES, 3); /* retry times*/
        if (ret != 0)
        {
            I2C_ERR("config :%s dev failed fd: %d ret:%d\n", dev, fd, ret);
            return ret;
        }

        ret = ioctl(fd, I2C_RDWR, (unsigned long)&packets);
        if (ret < 0)
        {
            I2C_ERR("Unable to Read dev:0x%x reg:0x%x  ret:%d!!!\n", dev_addr, reg_addr, ret);
            perror("Unbale to Read regs");
        }
        else
        {
            *reg_val = inbuf;
            ret = 0;
        }
    }

    MS_sleep(1);
    close(fd);
    return ret;
}

