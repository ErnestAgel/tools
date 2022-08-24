#include "iic_t.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h> /* memset */
#include <stdlib.h> /* system */
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h> /* ioctl*/
#include <fcntl.h>
#include <stdint.h>

#define IIC_Test_BUS_NUM   10

#define MS_sleep(T) usleep(T * 1000)

#define I2C_ERR(...)                                 \
    printf("[I2C:%s]LINE: %d ", __func__, __LINE__); \
    printf(__VA_ARGS__)

typedef struct
{
    const char *funcStr;
    uint8_t func_id;
} stFunc_list;

stFunc_list IIC_Func[] =
{
    {"D8",     1}, //Dump 8BIT
    {"d8",     2},
    {"R",      3}, //Read 8BIT
    {"r",      4},
    {"W",      5}, //Write 8BIT
    {"w",      6},
    {"D16",    7}, //Dump 16BIT
    {"d16",    8},
    {"Probe",  9},
    {"probe",  10}
};

CIIctx::CIIctx(/* args */)
{

}

CIIctx::~CIIctx()
{

}

int32_t CIIctx::Get_FuncID(const char* args)
{
    int32_t FuncID = 0;
    int32_t MaxFuc = sizeof(IIC_Func) / sizeof(IIC_Func[0]);

    for (int i = 1; i < MaxFuc; i++)
    {
        if (0 == strcmp(IIC_Func[i].funcStr, args))
        {
            FuncID = IIC_Func[i].func_id;
            break;
        }
    }

    if (FuncID < 0 ||FuncID > MaxFuc )
    {
        printf("invalid opeartion!!!\n");
        exit(-1);
    }

    return FuncID;
}

void CIIctx::IIc_Dump8BIT(IIC_PARA *pIIC_para)
{
	int32_t m_i = 0;
	int32_t m_j = 0;
	int32_t m_err = 0;

	printf("------------------------devAddr 0x%x----------------\r\n", pIIC_para->dev_addr);
	printf("     ");
	for (m_i = 0; m_i < 16; m_i++)
	{
		printf("-%0x ", m_i);
	}
	printf("\r\n");
	printf("----------------------------------------------------\r\n");

	for (m_i = 0; m_i < 16; m_i++)
	{
		printf("[%x-] ", m_i);
		for (m_j = 0; m_j < 16; m_j++)
		{
			m_err = IIC_8BIT_READ(pIIC_para->iic_dev, pIIC_para->dev_addr, m_i * 16 + m_j, &pIIC_para->reg_data);
			if (0 != m_err)
			{
				printf("\nread reg:0x%04x  data:0x%04x err!!\n", (m_i * 16 + m_j), pIIC_para->reg_data);
				printf("Maybe out of dev register range !!!\n");
				goto Done;
			}
			else
			{
				printf("%02x ", pIIC_para->reg_data);
				pIIC_para->reg_data = 0x0;
			}
		}
		printf("\r\n");
	}
Done:
	printf("----------------------------------------------------\r\n");
    
}

void CIIctx::IIc_Dump16BIT(IIC_PARA *pIIC_para)
{
	int32_t m_i = 0;
	int32_t m_j = 0;
	int32_t m_err = 0;

	printf("%d\n", __LINE__);

	printf("------------------------ Dump DevAddr 0x%x----------------\r\n", pIIC_para->dev_addr);
	printf("\t");
	for (m_i = 0; m_i < 16; m_i++)
	{
		printf("-%04x ", m_i);
	}
	printf("\r\n");
	printf("----------------------------------------------------------\r\n");

	for (m_i = 0; m_i < 4096; m_i++)
	{
		printf("[%04x-] ", m_i);
		for (m_j = 0; m_j < 16; m_j++)
		{
			m_err = IIC_16BIT_READ(pIIC_para->iic_dev, pIIC_para->dev_addr, m_i * 16 + m_j, &pIIC_para->reg_data);
			if (0 != m_err)
			{
				printf("\nread reg:0x%04x  data:0x%04x err!!\n", (m_i * 16 + m_j), pIIC_para->reg_data);
				printf("Maybe out of dev register range !!!\n");
				goto Done;
			}
			else
			{
				printf(" %04x ", pIIC_para->reg_data);
				pIIC_para->reg_data = 0x0;
			}
		}
		printf("\r\n");
	}
Done:
	printf("----------------------------------------------------------\r\n");
    
}

int32_t CIIctx::IIc_Get8BIT(IIC_PARA *pIIC_para)
{
    int32_t status;

    status = IIC_8BIT_READ(pIIC_para->iic_dev, pIIC_para->dev_addr, pIIC_para->reg_addr, &pIIC_para->reg_data);
    if(status != 0)
    {
        printf("IIC_8BIT_READ ERR!!! bus_id:%d dev_addr:0x%x reg:0x%x\n",pIIC_para->bus_id, pIIC_para->dev_addr, pIIC_para->reg_addr);
    }
    else
    {
        printf("IIC_8BIT_READ Success!!! dev_addr:0x%x reg:0x%x Read:0x%x\n",pIIC_para->dev_addr, pIIC_para->reg_addr, pIIC_para->reg_data);

    }
    return status;
    
}

int32_t CIIctx::IIc_Get16BIT(IIC_PARA *pIIC_para)
{
    int32_t status;

    status = IIC_16BIT_READ(pIIC_para->iic_dev, pIIC_para->dev_addr, pIIC_para->reg16_addr, &pIIC_para->reg_data);
    if (status != 0)
    {
        printf("IIC_16BIT_READ ERR!!! bus_id:%d dev_addr:0x%x reg:0x%x\n", pIIC_para->bus_id, pIIC_para->dev_addr, pIIC_para->reg16_addr);
    }
    else
    {
        printf("IIC_16BIT_READ Success!!! dev_addr:0x%x reg:0x%x Read:0x%x\n",pIIC_para->dev_addr, pIIC_para->reg16_addr, pIIC_para->reg_data);
    }
    return status;
}

int32_t CIIctx::IIc_Set8BIT(IIC_PARA *pIIC_para)
{
    int32_t status;

    status = IIC_8BIT_WRITE(pIIC_para->iic_dev, pIIC_para->dev_addr, pIIC_para->reg_addr, pIIC_para->reg_val);
    if (status != 0)
    {
        printf("IIC_8BIT_WRITE ERR!!! bus_id:%d dev_addr:0x%x reg:0x%x\n", pIIC_para->bus_id, pIIC_para->dev_addr, pIIC_para->reg_addr);
    }
    else
    {
        printf("IIC_8BIT_WRITE Success!!! dev_addr:0x%x reg:0x%x reg_val:0x%x\n",pIIC_para->dev_addr, pIIC_para->reg_addr, pIIC_para->reg_val);

    }
    return status;
}

int32_t CIIctx::IIc_Set16BIT(IIC_PARA *pIIC_para)
{
    int32_t status;
    status = IIC_16BIT_WRITE(pIIC_para->iic_dev, pIIC_para->dev_addr, pIIC_para->reg16_addr, pIIC_para->reg_val);
    if (status != 0)
    {
        printf("IIC_16BIT_WRITE ERR!!! bus_id:%d dev_addr:0x%x reg:0x%x\n", pIIC_para->bus_id, pIIC_para->dev_addr, pIIC_para->reg16_addr);
    }
    else
    {
        printf("IIC_16BIT_WRITE Success!!! dev_addr:0x%x reg:0x%x reg_val:0x%x\n",pIIC_para->dev_addr, pIIC_para->reg16_addr, pIIC_para->reg_val);
    }

    return status;
}


int32_t CIIctx::IIC_Probe(IIC_PARA *iic_prob_arg, uint32_t handle_id)
{
	int32_t i = 0;
	int32_t j = 0;
	int32_t ret = 0;
	int32_t m_err_num = 0;
    int32_t fd = 0;
    char dev_buf[128] = {0};
	if (handle_id == 0)
	{
		ret = -1;
		printf("invalid Probe handle id:%d\n", handle_id);
		return ret;
	}
	else
	{
		if (handle_id == 1)
		{
            
			printf("----------------------------------------------------\r\n");
			printf("\n");
            
			for (i = 0; i < IIC_Test_BUS_NUM; i++)
			{
                snprintf(dev_buf,sizeof(dev_buf),"/dev/i2c-%d",i);
                fd = open(dev_buf,O_RDWR);
                if(fd < 0)
                {
                    continue;
                }
                else
                {
                    printf("IIC BUS ID:%d /dev/i2c-%d\n",i,i);
                    close(fd);
                }
                
			}
			printf("\n");

			printf("----------------------------------------------------\r\n");
			printf("\n");
		}
		else if (handle_id == 2)
		{

			printf("------------------Scanning IIC BUS %d----------------\r\n", iic_prob_arg->bus_id);
			printf("     ");
			for (i = 0; i < 16; i++)
			{
				printf("-%x ", i);
			}
			printf("\r\n");
			printf("-----------------------------------------------------\r\n");

			for (i = 0; i < 8; i++)
			{
				printf("[%x-] ", i);
				for (j = 0; j < 16; j++)
				{
					iic_prob_arg->dev_addr = (uint8_t)(0xFF & (i * 16 + j));
					m_err_num = IIC_8BIT_PRBREAD(iic_prob_arg->iic_dev, iic_prob_arg->dev_addr, iic_prob_arg->reg_addr, &iic_prob_arg->reg_data);
                    if (0 != m_err_num)
					{
						m_err_num = 0;
						printf("-- ");
						continue;
					}
					else
					{
						printf("%02x ", iic_prob_arg->dev_addr);
						iic_prob_arg->dev_addr = 0x0;
					}
				}
				printf("\r\n");
			}
			printf("----------------------------------------------------\r\n");
		}
	}

	return ret;
}

int32_t CIIctx::IIC_16BIT_WRITE(const char *dev, uint16_t dev_addr, uint16_t reg_addr, unsigned char reg_val)
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

int32_t CIIctx::IIC_16BIT_READ(const char *dev, uint16_t dev_addr, uint16_t reg_addr, unsigned char *reg_val)
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

int32_t CIIctx::IIC_8BIT_WRITE(const char *dev, unsigned char dev_addr, uint16_t reg_addr, unsigned char reg_val)
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

int32_t CIIctx::IIC_8BIT_PRBREAD(const char *dev, unsigned char dev_addr, uint16_t reg_addr, unsigned char *reg_val)
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
            // I2C_ERR("Unable to Read dev:0x%x reg:0x%x  ret:%d!!!\n", dev_addr, reg_addr, ret);
            // perror("Unbale to Read regs");
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

int32_t CIIctx::IIC_8BIT_READ(const char *dev, unsigned char dev_addr, uint16_t reg_addr, unsigned char *reg_val)
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

int32_t CIIctx::IIC_32BIT_WRITE(const char *dev, uint16_t dev_addr, uint16_t reg_addr, uint16_t reg_val)
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
    buf[0] = (unsigned char)(0xFF & (reg_addr >> 8));

    buf[2] = (unsigned char)(0xFF & (reg_val >> 8));
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

int32_t CIIctx::IIC_32BIT_READ(const char *dev, uint16_t dev_addr, uint16_t reg_addr, uint16_t *reg_val)
{
    int fd = 0;
    int ret = 0;

    uint16_t data = 0U;
    unsigned char inbuf[2] = {0};
    unsigned char outbuf[2];

    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];
    inbuf[0] = (unsigned char)(0xFF & (*reg_val >> 8));
    inbuf[1] = (unsigned char)(0xFF & (*reg_val));

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