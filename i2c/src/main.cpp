#include <stdio.h>
#include <unistd.h>
#include <string.h>	  /* memset */
#include <stdlib.h>	  /* system */

#include"iic_t.h"

#define DATE(name) printf("\n%s Compiled on %s %s\n\n", name, __DATE__, __TIME__);


int main(int argc, char *argv[])
{
	DATE(argv[0]);

	if (argc < 4)
	{
		printf("\n");
		printf("[%d]invalid num of argc: %d !!!\n", __LINE__, argc);
		printf("useage:\n\n");
		printf("i2ctool  [Func_char] [Bus_Num] [Dev_Addr] [Reg_Addr] [Reg_Val]\n");
		printf("\n");
		printf("*********************************************************************\n");
		printf("*   Func_id list:\n");
		printf("*   R:     Read  8/16bit register \n");
		printf("*   W:     Write 8/16bit register \n");
		printf("*   D8:    Dump  8bit    register \n");
		printf("*   D16:   Dump  8bit    register \n");
		printf("*   Probe: Scan IIC Bus or Detecting IIC device\n");
		printf("*\n");
		printf("*   eg.a Listing IIC BUS ID \n");
		printf("*   i2ctool probe -f -l\n");
		printf("*\n");
		printf("*   eg.b Detecting IIC device \n");
		printf("*   i2ctool probe -f -s [BUS_ID]\n");
		printf("*\n");
		printf("*   eg.c Dump 8/16bit device on iic-bus 1\n");
		printf("*   i2ctool D8/16 [Bus_ID] [Dev_Addr]\n");
		printf("*\n");
		printf("*   eg.d Read 8/16bit device on iic-bus 1\n");
		printf("*   i2ctool R [Bus_ID] [Dev_Addr] [Reg_Addr]  \n");
		printf("*\n");
		printf("*   eg.e Write 8/16bit device on iic-bus 1\n");
		printf("*   i2ctool W [Bus_ID] [Dev_Addr] [Reg_Addr] [Reg_Val]\n");
		printf("*********************************************************************\n");
		printf("\n");
		printf("\n");
		exit(-1);
	}

	int32_t func_ID = 0;

	IIC_PARA iic_args;
	CIIctx *pIIC = NULL;


	func_ID = pIIC->Get_FuncID(argv[1]);

	iic_args.bus_id = atoi(argv[2]);

	
	snprintf(iic_args.iic_dev, sizeof(iic_args.iic_dev), "/dev/i2c-%d",iic_args.bus_id);

	switch (func_ID)
	{
	case D8:
	case d8:
		if(argc != 4)
		{
			printf("[%d]invalid input args:%d!!!\n",__LINE__, argc);
			exit(-1);
		}
		else
		{
            iic_args.dev_addr = strtoul(argv[3], (char**)NULL, 0);
			pIIC->IIc_Dump8BIT(&iic_args);
		}
		break;
	
	case D16:
	case d16:
		if(argc != 4)
		{
			printf("[%d]invalid input args:%d!!!\n",__LINE__, argc);
			exit(-1);
		}
		else
		{
            iic_args.dev_addr = strtoul(argv[3], (char**)NULL, 0);
			pIIC->IIc_Dump16BIT(&iic_args);
		}
		break;
	
	case W:
	case w:
		if(argc != 6)
		{
			printf("[%d]invalid input args:%d!!!\n",__LINE__, argc);
			exit(-1);
		}
		else
		{
            iic_args.dev_addr = strtoul(argv[3], (char**)NULL, 0);
			iic_args.reg16_addr = strtoul(argv[4], (char**)NULL, 0);
            iic_args.reg_val = strtoul(argv[5], (char**)NULL, 0);
			if(iic_args.reg16_addr > 0xFF )
			{
				pIIC->IIc_Set16BIT(&iic_args);
			}
			else
			{
				iic_args.reg_addr = strtoul(argv[4], (char**)NULL, 0);
				pIIC->IIc_Set8BIT(&iic_args);
			}
		}
		break;

	case R:
	case r:
		if(argc != 5)
		{
			printf("[%d]invalid input args:%d!!!\n",__LINE__, argc);
			exit(-1);
		}
		else
		{
            iic_args.dev_addr = strtoul(argv[3], (char**)NULL, 0);
            iic_args.reg16_addr = strtoul(argv[4], (char**)NULL, 0);
			if(iic_args.reg16_addr > 0xFF)
			{
				pIIC->IIc_Get16BIT(&iic_args);

			}
			else
			{
				iic_args.reg_addr = strtoul(argv[4], (char**)NULL, 0);
				pIIC->IIc_Get8BIT(&iic_args);
			}
		}
		break;

	case Probe:
	case probe:
		do
		{
			int32_t err_num = 0;
			int32_t handle_id = 0;

			if ((0 == strcmp(argv[2], "-f")) && (0 == strcmp(argv[3], "-l")))
			{
				if (argc != 4)
				{
					printf("[%d]invalid input args:%d!!!\n", __LINE__, argc);
					exit(-1);
				}
				else
				{
					handle_id = 1;
					iic_args.bus_id = 0;
					iic_args.reg_addr = 0x00;

					err_num = pIIC->IIC_Probe(&iic_args, handle_id);
					if (err_num != 0)
					{
						printf("[%d]HAT_IIC_Probe Failed %d!!!\n", __LINE__, err_num);
						break;
					}
				}
			}
			
			if ((0 == strcmp(argv[2], "-f")) && (0 == strcmp(argv[3], "-s")))
			{
				if (argc != 5)
				{
					printf("[%d]invalid input args:%d!!!\n", __LINE__, argc);
					exit(-1);
				}
				else
				{
					handle_id = 2;
					iic_args.bus_id = atoi(argv[4]);
					snprintf(iic_args.iic_dev, sizeof(iic_args.iic_dev), "/dev/i2c-%d", iic_args.bus_id);

					err_num = pIIC->IIC_Probe(&iic_args, handle_id);
					if (err_num != 0)
					{
						printf("[%d]HAT_IIC_Probe Failed %d!!!\n", __LINE__, err_num);
						break;
					}
				}
			}
		} while (0);
		break;
	default:
		printf("invalid funcid!!!\n");
		break;
	}

	return 0;
}
