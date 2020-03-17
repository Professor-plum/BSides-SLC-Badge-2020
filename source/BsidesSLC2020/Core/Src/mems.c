#include "mems.h"
#include <stdio.h>

LIS2DH12_Object_t lis;

bool MEMS_Init() {
	 uint32_t ret=0;
	 LIS2DH12_IO_t io;
	 io.Address = LIS2DH12_I2C_ADD_H;
	 io.BusType = LIS2DH12_I2C_BUS;
	 io.Init = BSP_I2C1_Init;
	 io.DeInit = BSP_I2C1_DeInit;
	 io.GetTick = BSP_GetTick;
	 io.ReadReg = BSP_I2C1_ReadReg;
	 io.WriteReg = BSP_I2C1_WriteReg;

	 if (LIS2DH12_RegisterBusIO(&lis, &io) != 0) {
		 puts("Unable to register LIS2DH12");
		 return false;
	 }
	 if (LIS2DH12_Init(&lis) != 0) {
		 puts("Lis init failed!");
		 return false;
	 }

	 if (LIS2DH12_Enable(&lis)){
		 puts("Lis enable failed");
	 	 return false;
	 }

	 ret |= LIS2DH12_Write_Reg(&lis, LIS2DH12_CTRL_REG0, 0x10);
	 ret |= LIS2DH12_Write_Reg(&lis, LIS2DH12_CTRL_REG1, 0x77);
	 ret |= LIS2DH12_Write_Reg(&lis, LIS2DH12_CTRL_REG2, 0x00); //default
	 ret |= LIS2DH12_Write_Reg(&lis, LIS2DH12_CTRL_REG3, 0x80); //enable CLICK on INT1
	 ret |= LIS2DH12_Write_Reg(&lis, LIS2DH12_CTRL_REG4, 0x88); //enable High res & BDU
	 ret |= LIS2DH12_Write_Reg(&lis, LIS2DH12_CTRL_REG5, 0x08); //enable latch on INT1
	 ret |= LIS2DH12_Write_Reg(&lis, LIS2DH12_CTRL_REG6, 0x0A); //enable INT2 on ACT, Reverse Polarity
	 ret |= LIS2DH12_Write_Reg(&lis, LIS2DH12_CTRL_REG6, 0x02); //Reverse Polarity
	 ret |= LIS2DH12_Write_Reg(&lis, LIS2DH12_TEMP_CFG_REG, 0x00); //disabled (default)
	 ret |= LIS2DH12_Write_Reg(&lis, LIS2DH12_FIFO_CTRL_REG, 0x00); //disable FIFO

	 ret |= LIS2DH12_Write_Reg(&lis, LIS2DH12_INT1_CFG, 0x08); //not used, 0x08 for high Y event
	 ret |= LIS2DH12_Write_Reg(&lis, LIS2DH12_INT1_THS, 0x7f); // 0x7f (2g)
	 ret |= LIS2DH12_Write_Reg(&lis, LIS2DH12_INT1_DURATION, 0x80);

	 ret |= LIS2DH12_Write_Reg(&lis, LIS2DH12_INT2_CFG, 0x00); //not used 0xA0 (and high Z)
	 ret |= LIS2DH12_Write_Reg(&lis, LIS2DH12_INT2_THS, 0); // 0x1F
	 ret |= LIS2DH12_Write_Reg(&lis, LIS2DH12_INT2_DURATION, 0);

	 ret |= LIS2DH12_Write_Reg(&lis, LIS2DH12_CLICK_CFG, 0x3F); //enable CLICK
	 ret |= LIS2DH12_Write_Reg(&lis, LIS2DH12_CLICK_THS, 40);
	 ret |= LIS2DH12_Write_Reg(&lis, LIS2DH12_TIME_LIMIT, 0x33);
	 ret |= LIS2DH12_Write_Reg(&lis, LIS2DH12_TIME_LATENCY, 0x20);
	 ret |= LIS2DH12_Write_Reg(&lis, LIS2DH12_TIME_WINDOW, 0xff);

	 ret |= LIS2DH12_Write_Reg(&lis, LIS2DH12_ACT_THS, 5);
	 ret |= LIS2DH12_Write_Reg(&lis, LIS2DH12_ACT_DUR, 0xFF);

	 //LIS2DH12_Read_Reg(&lis, LIS2DH12_REFERENCE, &dat);
	 //LIS2DH12_SetOutputDataRate(&lis, LIS2DH12_ODR_400Hz);


	 //LIS2DH12_Read_Reg(&lis, uint8_t reg, &dat);
	 //LIS2DH12_Write_Reg(&lis, uint8_t reg, dat);

	 /*for (uint8_t i=0x1e; i<0x40; ++i){
		 LIS2DH12_Read_Reg(&lis, i, &dat);
		 printf("mems %02x: %02x\n", i, dat);
	 }*/


	 uint8_t id;
	 if (LIS2DH12_ReadID(&lis, &id) !=0){
		 puts("Lis read id failed");
		return false;
	 }
	 //printf("Lis id:%x\r\n", id);

	 return ret == 0;
}


uint8_t MEMS_Process() {
	LIS2DH12_Axes_t accel;

	if (LIS2DH12_OK == LIS2DH12_GetAxes(&lis, &accel)) {
		//printf("%ld, %ld, %ld\r\n", accel.x, accel.y, accel.z);
	} else {
		printf("Error reading mems values!");
		return 0;
	}
	lis2dh12_click_src_t dat;
	lis2dh12_int1_src_t dat2;
	lis2dh12_int2_src_t dat3;

	LIS2DH12_Read_Reg(&lis, LIS2DH12_CLICK_SRC, (uint8_t*)&dat);
	LIS2DH12_Read_Reg(&lis, LIS2DH12_INT1_SRC, (uint8_t*)&dat2);
	LIS2DH12_Read_Reg(&lis, LIS2DH12_INT2_SRC, (uint8_t*)&dat3);
	if (dat.ia) {
		//printf("C:%d, DC:%d, %x\r\n", dat.sclick, dat.dclick, dat);
	}
	if (dat2.ia){
		printf("Movement %x\r\n", dat2);
		return 2;
	}
	if (dat3.ia){
		printf("Movement 2 %x\r\n", dat3);
	}
	else {
		//puts("No event?");
	}
	//if (accel.y < -1300)
	//		return 2; // fast punch
	if (dat.dclick && (accel.z < -800))
		return 1; //lock box open
	return 0;
}

void MEMS_PowerDown() {
	//LIS2DH12_Write_Reg(&lis, LIS2DH12_CTRL_REG3, 0x90); //enable INT1
	//LIS2DH12_Write_Reg(&lis, LIS2DH12_CTRL_REG3, 0x90); //enable INT1
	LIS2DH12_DeInit(&lis);
}

void MEMS_Click() {
  lis2dh12_click_src_t dat;
  LIS2DH12_Read_Reg(&lis, LIS2DH12_CLICK_SRC, (uint8_t*)&dat);
  if (*(uint8_t*)&dat) {
		//printf("C:%d, DC:%d, %x\r\n", dat.sclick, dat.dclick, dat);
  }
}

void MEMS_Clear() {
	lis2dh12_click_src_t dat;
	lis2dh12_int1_src_t dat2;
	lis2dh12_int2_src_t dat3;

	LIS2DH12_Read_Reg(&lis, LIS2DH12_CLICK_SRC, (uint8_t*)&dat);
	LIS2DH12_Read_Reg(&lis, LIS2DH12_INT1_SRC, (uint8_t*)&dat2);
	LIS2DH12_Read_Reg(&lis, LIS2DH12_INT2_SRC, (uint8_t*)&dat3);
}
