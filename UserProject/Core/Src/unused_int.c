


















//Try to avoid making this not line 20
volatile int debug_var;

void isr_break( int number )
{
	debug_var = number;//so that this can be line 25
}

//NMI_Handler( void )
//{
//	isr_break(1);
//}

void voidHardFault_Handler( void )
{
	while(1);
	isr_break(2);
}

//void MemManage_Handler( void )
//{
//	isr_break(3);
//}

//void BusFault_Handler( void )
//{
//	isr_break(4);
//}

//void UsageFault_Handler( void )
//{
//	isr_break(5);
//}

//void SVC_Handler( void )
//{
//	isr_break(6);
//}

//void DebugMon_Handler( void )
//{
//	isr_break(7);
//}

//void PendSV_Handler( void )
//{
//	isr_break(8);
//}

//void SysTick_Handler( void )
//{
//	isr_break(9);
//}

void WWDG_IRQHandler( void )
{
	isr_break(10);
	while(1);
}

void PVD_IRQHandler( void )
{
	isr_break(11);
}

void TAMP_STAMP_IRQHandler( void )
{
	isr_break(12);
}

void RTC_WKUP_IRQHandler( void )
{
	isr_break(13);
}

void FLASH_IRQHandler( void )
{
	isr_break(14);
}

void RCC_IRQHandler( void )
{
	isr_break(15);
}

void EXTI0_IRQHandler( void )
{
	isr_break(16);
}

void EXTI1_IRQHandler( void )
{
	isr_break(17);
}

void EXTI2_IRQHandler( void )
{
	isr_break(18);
}

void EXTI3_IRQHandler( void )
{
	isr_break(19);
}

void EXTI4_IRQHandler( void )
{
	isr_break(20);
}

void DMA1_Stream0_IRQHandler( void )
{
	isr_break(21);
}

void DMA1_Stream1_IRQHandler( void )
{
	isr_break(22);
}

void DMA1_Stream2_IRQHandler( void )
{
	isr_break(23);
}

void DMA1_Stream3_IRQHandler( void )
{
	isr_break(24);
}

void DMA1_Stream4_IRQHandler( void )
{
	isr_break(25);
}

void DMA1_Stream5_IRQHandler( void )
{
	isr_break(26);
}

//void DMA1_Stream6_IRQHandler( void )
//{
//	isr_break(27);
//}

//void ADC_IRQHandler( void )
//{
//	isr_break(28);
//}

void CAN1_TX_IRQHandler( void )
{
	isr_break(29);
}

void CAN1_RX0_IRQHandler( void )
{
	isr_break(30);
}

void CAN1_RX1_IRQHandler( void )
{
	isr_break(31);
}

void CAN1_SCE_IRQHandler( void )
{
	isr_break(32);
}

void EXTI9_5_IRQHandler( void )
{
	isr_break(33);
}

void TIM1_BRK_TIM9_IRQHandler( void )
{
	isr_break(34);
}

//void TIM1_UP_TIM10_IRQHandler( void )
//{
//	isr_break(35);
//}

void TIM1_TRG_COM_TIM11_IRQHandler( void )
{
	isr_break(36);
}

void TIM1_CC_IRQHandler( void )
{
	isr_break(37);
}

//void TIM2_IRQHandler( void )
//{
//	isr_break(38);
//}

//void TIM3_IRQHandler( void )
//{
//	isr_break(39);
//}

void TIM4_IRQHandler( void )
{
	isr_break(40);
}

void I2C1_EV_IRQHandler( void )
{
	isr_break(41);
}

void I2C1_ER_IRQHandler( void )
{
	isr_break(42);
}

void I2C2_EV_IRQHandler( void )
{
	isr_break(43);
}

void I2C2_ER_IRQHandler( void )
{
	isr_break(44);
}

//void SPI1_IRQHandler( void )
//{
//	isr_break(45);
//}

void SPI2_IRQHandler( void )
{
	isr_break(46);
}

void USART1_IRQHandler( void )
{
	isr_break(47);
}

//void USART2_IRQHandler( void )
//{
//	isr_break(48);
//}

void USART3_IRQHandler( void )
{
	isr_break(49);
}

void EXTI15_10_IRQHandler( void )
{
	isr_break(50);
}

void RTC_Alarm_IRQHandler( void )
{
	isr_break(51);
}

void OTG_FS_WKUP_IRQHandler( void )
{
	isr_break(52);
}

void TIM8_BRK_TIM12_IRQHandler( void )
{
	isr_break(53);
}

void TIM8_UP_TIM13_IRQHandler( void )
{
	isr_break(54);
}

void TIM8_TRG_COM_TIM14_IRQHandler( void )
{
	isr_break(55);
}

void TIM8_CC_IRQHandler( void )
{
	isr_break(56);
}

void DMA1_Stream7_IRQHandler( void )
{
	isr_break(57);
}

void FMC_IRQHandler( void )
{
	isr_break(58);
}

void SDMMC1_IRQHandler( void )
{
	isr_break(59);
}

void TIM5_IRQHandler( void )
{
	isr_break(60);
}

void SPI3_IRQHandler( void )
{
	isr_break(61);
}

void UART4_IRQHandler( void )
{
	isr_break(62);
}

void UART5_IRQHandler( void )
{
	isr_break(63);
}

void TIM6_DAC_IRQHandler( void )
{
	isr_break(64);
}

void TIM7_IRQHandler( void )
{
	isr_break(65);
}

void DMA2_Stream0_IRQHandler( void )
{
	isr_break(66);
}

void DMA2_Stream1_IRQHandler( void )
{
	isr_break(67);
}

void DMA2_Stream2_IRQHandler( void )
{
	isr_break(68);
}

//void DMA2_Stream3_IRQHandler( void )
//{
//	isr_break(69);
//}

void DMA2_Stream4_IRQHandler( void )
{
	isr_break(70);
}

//71 is free

void ETH_IRQHandler( void )
{
	isr_break(72);
}

void ETH_WKUP_IRQHandler( void )
{
	isr_break(73);
}

void CAN2_TX_IRQHandler( void )
{
	isr_break(74);
}

void CAN2_RX0_IRQHandler( void )
{
	isr_break(75);
}

void CAN2_RX1_IRQHandler( void )
{
	isr_break(76);
}

void CAN2_SCE_IRQHandler( void )
{
	isr_break(77);
}

void OTG_FS_IRQHandler( void )
{
	isr_break(78);
}

void DMA2_Stream5_IRQHandler( void )
{
	isr_break(79);
}

//void DMA2_Stream6_IRQHandler( void )
//{
//	isr_break(80);
//}

void DMA2_Stream7_IRQHandler( void )
{
	isr_break(81);
}

//void USART6_IRQHandler( void )
//{
//	isr_break(82);
//}

void I2C3_EV_IRQHandler( void )
{
	isr_break(83);
}

void I2C3_ER_IRQHandler( void )
{
	isr_break(84);
}

void OTG_HS_EP1_OUT_IRQHandler( void )
{
	isr_break(85);
}

void OTG_HS_EP1_IN_IRQHandler( void )
{
	isr_break(86);
}

void OTG_HS_WKUP_IRQHandler( void )
{
	isr_break(87);
}

void OTG_HS_IRQHandler( void )
{
	isr_break(88);
}

void DCMI_IRQHandler( void )
{
	isr_break(89);
}

void RNG_IRQHandler( void )
{
	isr_break(90);
}

void FPU_IRQHandler( void )
{
	isr_break(91);
}

void UART7_IRQHandler( void )
{
	isr_break(92);
}

void UART8_IRQHandler( void )
{
	isr_break(93);
}

void SPI4_IRQHandler( void )
{
	isr_break(94);
}

void SPI5_IRQHandler( void )
{
	isr_break(95);
}

void SPI6_IRQHandler( void )
{
	isr_break(96);
}

void SAI1_IRQHandler( void )
{
	isr_break(97);
}

void LTDC_IRQHandler( void )
{
	isr_break(98);
}

void LTDC_ER_IRQHandler( void )
{
	isr_break(99);
}

void DMA2D_IRQHandler( void )
{
	isr_break(100);
}

void SAI2_IRQHandler( void )
{
	isr_break(101);
}

void QUADSPI_IRQHandler( void )
{
	isr_break(102);
}

void LPTIM1_IRQHandler( void )
{
	isr_break(103);
}

void CEC_IRQHandler( void )
{
	isr_break(104);
}

void I2C4_EV_IRQHandler( void )
{
	isr_break(105);
}

void I2C4_ER_IRQHandler( void )
{
	isr_break(106);
}

void SPDIF_RX_IRQHandler( void )
{
	isr_break(107);
}

void DSI_IRQHandler( void )
{
	isr_break(108);
}

void DFSDM1_FLT0_IRQHandler( void )
{
	isr_break(109);
}

void DFSDM1_FLT1_IRQHandler( void )
{
	isr_break(110);
}

void DFSDM1_FLT2_IRQHandler( void )
{
	isr_break(111);
}

void DFSDM1_FLT3_IRQHandler( void )
{
	isr_break(112);
}

void SDMMC2_IRQHandler( void )
{
	isr_break(113);
}

void CAN3_TX_IRQHandler( void )
{
	isr_break(114);
}

void CAN3_RX0_IRQHandler( void )
{
	isr_break(115);
}

void CAN3_RX1_IRQHandler( void )
{
	isr_break(116);
}

void CAN3_SCE_IRQHandler( void )
{
	isr_break(117);
}

void JPEG_IRQHandler( void )
{
	isr_break(118);
}

void MDIOS_IRQHandler( void )
{
	isr_break(119);
}
