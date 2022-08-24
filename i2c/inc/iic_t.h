#include<iostream>

using namespace std;

typedef enum
{
    D8 = 1, //Dump 8BIT
    d8,
    R, //Read 8BIT
    r,
    W, //Write 8BIT
    w,
    D16, //Dump 16BIT
    d16,
    Probe,
    probe
} emFunc_list;


typedef struct
{
    /* IIC BUS ID eg. 0, 1, 2.. */
    uint16_t bus_id;

    /* IIC dev */
    char iic_dev[128];

    /* Device address eg. 0x08 */
    uint16_t dev_addr;

    /* Register address for 8BIT eg. 0x00~0xFF */
    uint8_t  reg_addr;

    /* Register address for 16BIT eg. 0x00_00 ~ 0xFF_FF */
    uint16_t reg16_addr;

    /* Hex val write into register */
    uint16_t reg_val;

    /* Hex val return from register */
    unsigned char  reg_data;

}IIC_PARA;


class CIIctx
{

public:
    CIIctx(/* args */);
    ~CIIctx();

    int32_t Get_FuncID(const char* args);

    void IIc_Dump8BIT(IIC_PARA *pIIC_para);
    void IIc_Dump16BIT(IIC_PARA *pIIC_para);

    int32_t IIc_Get8BIT(IIC_PARA *pIIC_para);
    int32_t IIc_Get16BIT(IIC_PARA *pIIC_para);

    int32_t IIc_Set8BIT(IIC_PARA *pIIC_para);
    int32_t IIc_Set16BIT(IIC_PARA *pIIC_para);
    int32_t IIC_Probe(IIC_PARA *pIIC_para, uint32_t handle_id);


    int32_t IIC_16BIT_WRITE(const char *dev, uint16_t dev_addr, uint16_t reg_addr, unsigned char reg_val);
 
    int32_t IIC_16BIT_READ(const char *dev, uint16_t dev_addr, uint16_t reg_addr, unsigned char *reg_val);
 
    int32_t IIC_8BIT_WRITE(const char *dev, unsigned char dev_addr, uint16_t reg_addr, unsigned char reg_val);

    int32_t IIC_8BIT_READ(const char *dev, unsigned char dev_addr, uint16_t reg_addr, unsigned char *reg_val);
    int32_t IIC_8BIT_PRBREAD(const char *dev, unsigned char dev_addr, uint16_t reg_addr, unsigned char *reg_val);

    int32_t IIC_32BIT_WRITE(const char *dev, uint16_t dev_addr, uint16_t reg_addr, uint16_t reg_val);

    int32_t IIC_32BIT_READ(const char *dev, uint16_t dev_addr, uint16_t reg_addr, uint16_t *reg_val);


};

