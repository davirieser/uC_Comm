.syntax unified
.cpu cortex-m0
@ .fpu softvfp
.thumb

.global g_pfnVectors
.global Default_Handler
.global _start

g_pfnVectors:
    @ Non-configurable
    @ 0x00
    .word _estack
    .word Reset_Handler
    .word NMI_Handler
    .word HardFault_Handler
    @ 0x10
    .word hang
    .word hang
    .word hang
    .word hang
    @ 0x20
    .word hang
    .word hang
    .word hang
    .word SVC_Handler
    @ 0x30
    .word hang
    .word hang
    .word PendSV_Handler
    .word SysTick_Handler
    @ Start configurable Interrupts
    @ 0x40
    .word WWDG_IRQHandler
    .word hang
    .word RTC_IRQHandler
    .word FLASH_IRQHandler
    @ 0x50
    .word RCC_IRQHandler
    .word EXTI0_1_IRQHandler
    .word EXTI2_3_IRQHandler
    .word EXTI4_15_IRQHandler
    @ 0x60
    .word hang
    .word DMA1_Channel1_IRQHandler
    .word DMA1_Channel2_3_IRQHandler
    .word DMA1_Channel4_5_IRQHandler
    @ 0x70
    .word ADC1_IRQHandler
    .word TIM1_BRK_UP_TRG_COM_IRQHandler
    .word TIM1_CC_IRQHandler
    .word hang
    @ 0x80
    .word TIM3_IRQHandler
    .word TIM6_DAC_IRQHandler
    .word hang
    .word TIM14_IRQHandler
    @ 0x90
    .word TIM15_IRQHandler
    .word TIM16_IRQHandler
    .word TIM17_IRQHandler
    .word I2C1_IRQHandler
    @ 0xA0
    .word I2C2_IRQHandler
    .word SPI1_IRQHandler
    .word SPI2_IRQHandler
    .word USART1_IRQHandler
    @ 0xB0
    .word USART2_IRQHandler
    .word USART3_IRQHandler
    .word hang
    .word USB_IRQHandler

.section .text.Reset_Handler
.weak Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
    ldr   r0, =_estack
    mov   sp, r0          /* set stack pointer */

    movs r3, #0
    bl not_main
    b hang

.size Reset_Handler, .-Reset_Handler

.section .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
    b Infinite_Loop
    .size Default_Handler, .-Default_Handler

_start:
    bl Reset_Handler

/******************************************************************************/

.thumb_func
hang:
    b .
;@-----------------------
.align
;@-----------------------
.thumb_func
.globl dummy
dummy:
    bx lr
;@-----------------------
.thumb_func
.globl INTERRUPTS
INTERRUPTS:
    CPSIE IF
    BX LR
;@-----------------------
.thumb_func
.globl NO_INTERRUPTS
NO_INTERRUPTS:
    CPSID IF
    BX LR
;@-----------------------

.align

.thumb_func
.globl PUT16
PUT16:
    strh r1,[r0]
    bx lr

.thumb_func
.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.thumb_func
.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

.thumb_func
.globl GET16
GET16:
    ldrh r0,[r0]
    bx lr

.globl PUT8
PUT8:
    strb r1,[r0]
    bx lr

.globl BRANCHTO
BRANCHTO:
    bx r0

/******************************************************************************/

/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler.
* As they are weak aliases, any function with the same name will override
* this definition.
*
*******************************************************************************/

.weak NMI_Handler
    .thumb_set NMI_Handler,Default_Handler

.weak HardFault_Handler
    .thumb_set HardFault_Handler,Default_Handler

.weak SVC_Handler
    .thumb_set SVC_Handler,Default_Handler

.weak PendSV_Handler
    .thumb_set PendSV_Handler,Default_Handler

.weak SysTick_Handler
    .thumb_set SysTick_Handler,Default_Handler

.weak WWDG_IRQHandler
    .thumb_set WWDG_IRQHandler,Default_Handler

.weak PVD_IRQHandler
    .thumb_set PVD_IRQHandler,Default_Handler

.weak RTC_IRQHandler
    .thumb_set RTC_IRQHandler,Default_Handler

.weak FLASH_IRQHandler
    .thumb_set FLASH_IRQHandler,Default_Handler

.weak RCC_IRQHandler
    .thumb_set RCC_IRQHandler,Default_Handler

.weak EXTI0_1_IRQHandler
    .thumb_set EXTI0_1_IRQHandler,Default_Handler

.weak EXTI2_3_IRQHandler
    .thumb_set EXTI2_3_IRQHandler,Default_Handler

.weak EXTI4_15_IRQHandler
    .thumb_set EXTI4_15_IRQHandler,Default_Handler

.weak TS_IRQHandler
    .thumb_set TS_IRQHandler,Default_Handler

.weak DMA1_Channel1_IRQHandler
    .thumb_set DMA1_Channel1_IRQHandler,Default_Handler

.weak DMA1_Channel2_3_IRQHandler
    .thumb_set DMA1_Channel2_3_IRQHandler,Default_Handler

.weak DMA1_Channel4_5_IRQHandler
    .thumb_set DMA1_Channel4_5_IRQHandler,Default_Handler

.weak ADC1_IRQHandler
    .thumb_set ADC1_IRQHandler,Default_Handler

.weak TIM1_BRK_UP_TRG_COM_IRQHandler
    .thumb_set TIM1_BRK_UP_TRG_COM_IRQHandler,Default_Handler

.weak TIM1_CC_IRQHandler
    .thumb_set TIM1_CC_IRQHandler,Default_Handler

.weak TIM2_IRQHandler
    .thumb_set TIM2_IRQHandler,Default_Handler

.weak TIM3_IRQHandler
    .thumb_set TIM3_IRQHandler,Default_Handler

.weak TIM6_DAC_IRQHandler
    .thumb_set TIM6_DAC_IRQHandler,Default_Handler

.weak TIM14_IRQHandler
    .thumb_set TIM14_IRQHandler,Default_Handler

.weak TIM15_IRQHandler
    .thumb_set TIM15_IRQHandler,Default_Handler

.weak TIM16_IRQHandler
    .thumb_set TIM16_IRQHandler,Default_Handler

.weak TIM17_IRQHandler
    .thumb_set TIM17_IRQHandler,Default_Handler

.weak I2C1_IRQHandler
    .thumb_set I2C1_IRQHandler,Default_Handler

.weak I2C2_IRQHandler
    .thumb_set I2C2_IRQHandler,Default_Handler

.weak SPI1_IRQHandler
    .thumb_set SPI1_IRQHandler,Default_Handler

.weak SPI2_IRQHandler
    .thumb_set SPI2_IRQHandler,Default_Handler

.weak hang
    .thumb_set hang,Default_Handler

.weak USART1_IRQHandler
    .thumb_set USART1_IRQHandler,Default_Handler

.weak USART2_IRQHandler
    .thumb_set USART2_IRQHandler,Default_Handler

.weak USART3_IRQHandler
    .thumb_set USART3_IRQHandler,Default_Handler

.weak CEC_IRQHandler
    .thumb_set CEC_IRQHandler,Default_Handler

.weak USB_IRQHandler
    .thumb_set USB_IRQHandler,Default_Handler

.end
