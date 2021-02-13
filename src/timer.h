
#pragma once

#include "../main.h"
#include <stdint.h>

/******************************************************************************/
/*                                                                            */
/*                               Timers (TIM)                                 */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for TIM_CR1 register  ********************/
#define  TIM_CR1_CEN                         0x0001            /*!<Counter enable */
#define  TIM_CR1_UDIS                        0x0002            /*!<Update disable */
#define  TIM_CR1_URS                         0x0004            /*!<Update request source */
#define  TIM_CR1_OPM                         0x0008            /*!<One pulse mode */
#define  TIM_CR1_DIR                         0x0010            /*!<Direction */

#define  TIM_CR1_CMS                         0x0060            /*!<CMS[1:0] bits (Center-aligned mode selection) */
#define  TIM_CR1_CMS_0                       0x0020            /*!<Bit 0 */
#define  TIM_CR1_CMS_1                       0x0040            /*!<Bit 1 */

#define  TIM_CR1_ARPE                        0x0080            /*!<Auto-reload preload enable */

#define  TIM_CR1_CKD                         0x0300            /*!<CKD[1:0] bits (clock division) */
#define  TIM_CR1_CKD_0                       0x0100            /*!<Bit 0 */
#define  TIM_CR1_CKD_1                       0x0200            /*!<Bit 1 */

/*******************  Bit definition for TIM_CR2 register  ********************/
#define  TIM_CR2_CCPC                        0x0001            /*!<Capture/Compare Preloaded Control */
#define  TIM_CR2_CCUS                        0x0004            /*!<Capture/Compare Control Update Selection */
#define  TIM_CR2_CCDS                        0x0008            /*!<Capture/Compare DMA Selection */

#define  TIM_CR2_MMS                         0x0070            /*!<MMS[2:0] bits (Master Mode Selection) */
#define  TIM_CR2_MMS_0                       0x0010            /*!<Bit 0 */
#define  TIM_CR2_MMS_1                       0x0020            /*!<Bit 1 */
#define  TIM_CR2_MMS_2                       0x0040            /*!<Bit 2 */

#define  TIM_CR2_TI1S                        0x0080            /*!<TI1 Selection */
#define  TIM_CR2_OIS1                        0x0100            /*!<Output Idle state 1 (OC1 output) */
#define  TIM_CR2_OIS1N                       0x0200            /*!<Output Idle state 1 (OC1N output) */
#define  TIM_CR2_OIS2                        0x0400            /*!<Output Idle state 2 (OC2 output) */
#define  TIM_CR2_OIS2N                       0x0800            /*!<Output Idle state 2 (OC2N output) */
#define  TIM_CR2_OIS3                        0x1000            /*!<Output Idle state 3 (OC3 output) */
#define  TIM_CR2_OIS3N                       0x2000            /*!<Output Idle state 3 (OC3N output) */
#define  TIM_CR2_OIS4                        0x4000            /*!<Output Idle state 4 (OC4 output) */

/*******************  Bit definition for TIM_SMCR register  *******************/
#define  TIM_SMCR_SMS                        0x0007            /*!<SMS[2:0] bits (Slave mode selection) */
#define  TIM_SMCR_SMS_0                      0x0001            /*!<Bit 0 */
#define  TIM_SMCR_SMS_1                      0x0002            /*!<Bit 1 */
#define  TIM_SMCR_SMS_2                      0x0004            /*!<Bit 2 */

#define  TIM_SMCR_OCCS                       0x0008            /*!< OCREF clear selection */

#define  TIM_SMCR_TS                         0x0070            /*!<TS[2:0] bits (Trigger selection) */
#define  TIM_SMCR_TS_0                       0x0010            /*!<Bit 0 */
#define  TIM_SMCR_TS_1                       0x0020            /*!<Bit 1 */
#define  TIM_SMCR_TS_2                       0x0040            /*!<Bit 2 */

#define  TIM_SMCR_MSM                        0x0080            /*!<Master/slave mode */

#define  TIM_SMCR_ETF                        0x0F00            /*!<ETF[3:0] bits (External trigger filter) */
#define  TIM_SMCR_ETF_0                      0x0100            /*!<Bit 0 */
#define  TIM_SMCR_ETF_1                      0x0200            /*!<Bit 1 */
#define  TIM_SMCR_ETF_2                      0x0400            /*!<Bit 2 */
#define  TIM_SMCR_ETF_3                      0x0800            /*!<Bit 3 */

#define  TIM_SMCR_ETPS                       0x3000            /*!<ETPS[1:0] bits (External trigger prescaler) */
#define  TIM_SMCR_ETPS_0                     0x1000            /*!<Bit 0 */
#define  TIM_SMCR_ETPS_1                     0x2000            /*!<Bit 1 */

#define  TIM_SMCR_ECE                        0x4000            /*!<External clock enable */
#define  TIM_SMCR_ETP                        0x8000            /*!<External trigger polarity */

/*******************  Bit definition for TIM_DIER register  *******************/
#define  TIM_DIER_UIE                        0x0001            /*!<Update interrupt enable */
#define  TIM_DIER_CC1IE                      0x0002            /*!<Capture/Compare 1 interrupt enable */
#define  TIM_DIER_CC2IE                      0x0004            /*!<Capture/Compare 2 interrupt enable */
#define  TIM_DIER_CC3IE                      0x0008            /*!<Capture/Compare 3 interrupt enable */
#define  TIM_DIER_CC4IE                      0x0010            /*!<Capture/Compare 4 interrupt enable */
#define  TIM_DIER_COMIE                      0x0020            /*!<COM interrupt enable */
#define  TIM_DIER_TIE                        0x0040            /*!<Trigger interrupt enable */
#define  TIM_DIER_BIE                        0x0080            /*!<Break interrupt enable */
#define  TIM_DIER_UDE                        0x0100            /*!<Update DMA request enable */
#define  TIM_DIER_CC1DE                      0x0200            /*!<Capture/Compare 1 DMA request enable */
#define  TIM_DIER_CC2DE                      0x0400            /*!<Capture/Compare 2 DMA request enable */
#define  TIM_DIER_CC3DE                      0x0800            /*!<Capture/Compare 3 DMA request enable */
#define  TIM_DIER_CC4DE                      0x1000            /*!<Capture/Compare 4 DMA request enable */
#define  TIM_DIER_COMDE                      0x2000            /*!<COM DMA request enable */
#define  TIM_DIER_TDE                        0x4000            /*!<Trigger DMA request enable */

/********************  Bit definition for TIM_SR register  ********************/
#define  TIM_SR_UIF                          0x0001            /*!<Update interrupt Flag */
#define  TIM_SR_CC1IF                        0x0002            /*!<Capture/Compare 1 interrupt Flag */
#define  TIM_SR_CC2IF                        0x0004            /*!<Capture/Compare 2 interrupt Flag */
#define  TIM_SR_CC3IF                        0x0008            /*!<Capture/Compare 3 interrupt Flag */
#define  TIM_SR_CC4IF                        0x0010            /*!<Capture/Compare 4 interrupt Flag */
#define  TIM_SR_COMIF                        0x0020            /*!<COM interrupt Flag */
#define  TIM_SR_TIF                          0x0040            /*!<Trigger interrupt Flag */
#define  TIM_SR_BIF                          0x0080            /*!<Break interrupt Flag */
#define  TIM_SR_CC1OF                        0x0200            /*!<Capture/Compare 1 Overcapture Flag */
#define  TIM_SR_CC2OF                        0x0400            /*!<Capture/Compare 2 Overcapture Flag */
#define  TIM_SR_CC3OF                        0x0800            /*!<Capture/Compare 3 Overcapture Flag */
#define  TIM_SR_CC4OF                        0x1000            /*!<Capture/Compare 4 Overcapture Flag */

/*******************  Bit definition for TIM_EGR register  ********************/
#define  TIM_EGR_UG                          0x01               /*!<Update Generation */
#define  TIM_EGR_CC1G                        0x02               /*!<Capture/Compare 1 Generation */
#define  TIM_EGR_CC2G                        0x04               /*!<Capture/Compare 2 Generation */
#define  TIM_EGR_CC3G                        0x08               /*!<Capture/Compare 3 Generation */
#define  TIM_EGR_CC4G                        0x10               /*!<Capture/Compare 4 Generation */
#define  TIM_EGR_COMG                        0x20               /*!<Capture/Compare Control Update Generation */
#define  TIM_EGR_TG                          0x40               /*!<Trigger Generation */
#define  TIM_EGR_BG                          0x80               /*!<Break Generation */

/******************  Bit definition for TIM_CCMR1 register  *******************/
#define  TIM_CCMR1_CC1S                      0x0003            /*!<CC1S[1:0] bits (Capture/Compare 1 Selection) */
#define  TIM_CCMR1_CC1S_0                    0x0001            /*!<Bit 0 */
#define  TIM_CCMR1_CC1S_1                    0x0002            /*!<Bit 1 */

#define  TIM_CCMR1_OC1FE                     0x0004            /*!<Output Compare 1 Fast enable */
#define  TIM_CCMR1_OC1PE                     0x0008            /*!<Output Compare 1 Preload enable */

#define  TIM_CCMR1_OC1M                      0x0070            /*!<OC1M[2:0] bits (Output Compare 1 Mode) */
#define  TIM_CCMR1_OC1M_0                    0x0010            /*!<Bit 0 */
#define  TIM_CCMR1_OC1M_1                    0x0020            /*!<Bit 1 */
#define  TIM_CCMR1_OC1M_2                    0x0040            /*!<Bit 2 */

#define  TIM_CCMR1_OC1CE                     0x0080            /*!<Output Compare 1Clear Enable */

#define  TIM_CCMR1_CC2S                      0x0300            /*!<CC2S[1:0] bits (Capture/Compare 2 Selection) */
#define  TIM_CCMR1_CC2S_0                    0x0100            /*!<Bit 0 */
#define  TIM_CCMR1_CC2S_1                    0x0200            /*!<Bit 1 */

#define  TIM_CCMR1_OC2FE                     0x0400            /*!<Output Compare 2 Fast enable */
#define  TIM_CCMR1_OC2PE                     0x0800            /*!<Output Compare 2 Preload enable */

#define  TIM_CCMR1_OC2M                      0x7000            /*!<OC2M[2:0] bits (Output Compare 2 Mode) */
#define  TIM_CCMR1_OC2M_0                    0x1000            /*!<Bit 0 */
#define  TIM_CCMR1_OC2M_1                    0x2000            /*!<Bit 1 */
#define  TIM_CCMR1_OC2M_2                    0x4000            /*!<Bit 2 */

#define  TIM_CCMR1_OC2CE                     0x8000            /*!<Output Compare 2 Clear Enable */

/*----------------------------------------------------------------------------*/

#define  TIM_CCMR1_IC1PSC                    0x000C            /*!<IC1PSC[1:0] bits (Input Capture 1 Prescaler) */
#define  TIM_CCMR1_IC1PSC_0                  0x0004            /*!<Bit 0 */
#define  TIM_CCMR1_IC1PSC_1                  0x0008            /*!<Bit 1 */

#define  TIM_CCMR1_IC1F                      0x00F0            /*!<IC1F[3:0] bits (Input Capture 1 Filter) */
#define  TIM_CCMR1_IC1F_0                    0x0010            /*!<Bit 0 */
#define  TIM_CCMR1_IC1F_1                    0x0020            /*!<Bit 1 */
#define  TIM_CCMR1_IC1F_2                    0x0040            /*!<Bit 2 */
#define  TIM_CCMR1_IC1F_3                    0x0080            /*!<Bit 3 */

#define  TIM_CCMR1_IC2PSC                    0x0C00            /*!<IC2PSC[1:0] bits (Input Capture 2 Prescaler) */
#define  TIM_CCMR1_IC2PSC_0                  0x0400            /*!<Bit 0 */
#define  TIM_CCMR1_IC2PSC_1                  0x0800            /*!<Bit 1 */

#define  TIM_CCMR1_IC2F                      0xF000            /*!<IC2F[3:0] bits (Input Capture 2 Filter) */
#define  TIM_CCMR1_IC2F_0                    0x1000            /*!<Bit 0 */
#define  TIM_CCMR1_IC2F_1                    0x2000            /*!<Bit 1 */
#define  TIM_CCMR1_IC2F_2                    0x4000            /*!<Bit 2 */
#define  TIM_CCMR1_IC2F_3                    0x8000            /*!<Bit 3 */

/******************  Bit definition for TIM_CCMR2 register  *******************/
#define  TIM_CCMR2_CC3S                      0x0003            /*!<CC3S[1:0] bits (Capture/Compare 3 Selection) */
#define  TIM_CCMR2_CC3S_0                    0x0001            /*!<Bit 0 */
#define  TIM_CCMR2_CC3S_1                    0x0002            /*!<Bit 1 */

#define  TIM_CCMR2_OC3FE                     0x0004            /*!<Output Compare 3 Fast enable */
#define  TIM_CCMR2_OC3PE                     0x0008            /*!<Output Compare 3 Preload enable */

#define  TIM_CCMR2_OC3M                      0x0070            /*!<OC3M[2:0] bits (Output Compare 3 Mode) */
#define  TIM_CCMR2_OC3M_0                    0x0010            /*!<Bit 0 */
#define  TIM_CCMR2_OC3M_1                    0x0020            /*!<Bit 1 */
#define  TIM_CCMR2_OC3M_2                    0x0040            /*!<Bit 2 */

#define  TIM_CCMR2_OC3CE                     0x0080            /*!<Output Compare 3 Clear Enable */

#define  TIM_CCMR2_CC4S                      0x0300            /*!<CC4S[1:0] bits (Capture/Compare 4 Selection) */
#define  TIM_CCMR2_CC4S_0                    0x0100            /*!<Bit 0 */
#define  TIM_CCMR2_CC4S_1                    0x0200            /*!<Bit 1 */

#define  TIM_CCMR2_OC4FE                     0x0400            /*!<Output Compare 4 Fast enable */
#define  TIM_CCMR2_OC4PE                     0x0800            /*!<Output Compare 4 Preload enable */

#define  TIM_CCMR2_OC4M                      0x7000            /*!<OC4M[2:0] bits (Output Compare 4 Mode) */
#define  TIM_CCMR2_OC4M_0                    0x1000            /*!<Bit 0 */
#define  TIM_CCMR2_OC4M_1                    0x2000            /*!<Bit 1 */
#define  TIM_CCMR2_OC4M_2                    0x4000            /*!<Bit 2 */

#define  TIM_CCMR2_OC4CE                     0x8000            /*!<Output Compare 4 Clear Enable */

/*----------------------------------------------------------------------------*/

#define  TIM_CCMR2_IC3PSC                    0x000C            /*!<IC3PSC[1:0] bits (Input Capture 3 Prescaler) */
#define  TIM_CCMR2_IC3PSC_0                  0x0004            /*!<Bit 0 */
#define  TIM_CCMR2_IC3PSC_1                  0x0008            /*!<Bit 1 */

#define  TIM_CCMR2_IC3F                      0x00F0            /*!<IC3F[3:0] bits (Input Capture 3 Filter) */
#define  TIM_CCMR2_IC3F_0                    0x0010            /*!<Bit 0 */
#define  TIM_CCMR2_IC3F_1                    0x0020            /*!<Bit 1 */
#define  TIM_CCMR2_IC3F_2                    0x0040            /*!<Bit 2 */
#define  TIM_CCMR2_IC3F_3                    0x0080            /*!<Bit 3 */

#define  TIM_CCMR2_IC4PSC                    0x0C00            /*!<IC4PSC[1:0] bits (Input Capture 4 Prescaler) */
#define  TIM_CCMR2_IC4PSC_0                  0x0400            /*!<Bit 0 */
#define  TIM_CCMR2_IC4PSC_1                  0x0800            /*!<Bit 1 */

#define  TIM_CCMR2_IC4F                      0xF000            /*!<IC4F[3:0] bits (Input Capture 4 Filter) */
#define  TIM_CCMR2_IC4F_0                    0x1000            /*!<Bit 0 */
#define  TIM_CCMR2_IC4F_1                    0x2000            /*!<Bit 1 */
#define  TIM_CCMR2_IC4F_2                    0x4000            /*!<Bit 2 */
#define  TIM_CCMR2_IC4F_3                    0x8000            /*!<Bit 3 */

/*******************  Bit definition for TIM_CCER register  *******************/
#define  TIM_CCER_CC1E                       0x0001            /*!<Capture/Compare 1 output enable */
#define  TIM_CCER_CC1P                       0x0002            /*!<Capture/Compare 1 output Polarity */
#define  TIM_CCER_CC1NE                      0x0004            /*!<Capture/Compare 1 Complementary output enable */
#define  TIM_CCER_CC1NP                      0x0008            /*!<Capture/Compare 1 Complementary output Polarity */
#define  TIM_CCER_CC2E                       0x0010            /*!<Capture/Compare 2 output enable */
#define  TIM_CCER_CC2P                       0x0020            /*!<Capture/Compare 2 output Polarity */
#define  TIM_CCER_CC2NE                      0x0040            /*!<Capture/Compare 2 Complementary output enable */
#define  TIM_CCER_CC2NP                      0x0080            /*!<Capture/Compare 2 Complementary output Polarity */
#define  TIM_CCER_CC3E                       0x0100            /*!<Capture/Compare 3 output enable */
#define  TIM_CCER_CC3P                       0x0200            /*!<Capture/Compare 3 output Polarity */
#define  TIM_CCER_CC3NE                      0x0400            /*!<Capture/Compare 3 Complementary output enable */
#define  TIM_CCER_CC3NP                      0x0800            /*!<Capture/Compare 3 Complementary output Polarity */
#define  TIM_CCER_CC4E                       0x1000            /*!<Capture/Compare 4 output enable */
#define  TIM_CCER_CC4P                       0x2000            /*!<Capture/Compare 4 output Polarity */
#define  TIM_CCER_CC4NP                      0x8000            /*!<Capture/Compare 4 Complementary output Polarity */

/*******************  Bit definition for TIM_CNT register  ********************/
#define  TIM_CNT_CNT                         0xFFFF            /*!<Counter Value */

/*******************  Bit definition for TIM_PSC register  ********************/
#define  TIM_PSC_PSC                         0xFFFF            /*!<Prescaler Value */

/*******************  Bit definition for TIM_ARR register  ********************/
#define  TIM_ARR_ARR                         0xFFFF            /*!<actual auto-reload Value */

/*******************  Bit definition for TIM_RCR register  ********************/
#define  TIM_RCR_REP                         0xFF               /*!<Repetition Counter Value */

/*******************  Bit definition for TIM_CCR1 register  *******************/
#define  TIM_CCR1_CCR1                       0xFFFF            /*!<Capture/Compare 1 Value */

/*******************  Bit definition for TIM_CCR2 register  *******************/
#define  TIM_CCR2_CCR2                       0xFFFF            /*!<Capture/Compare 2 Value */

/*******************  Bit definition for TIM_CCR3 register  *******************/
#define  TIM_CCR3_CCR3                       0xFFFF            /*!<Capture/Compare 3 Value */

/*******************  Bit definition for TIM_CCR4 register  *******************/
#define  TIM_CCR4_CCR4                       0xFFFF            /*!<Capture/Compare 4 Value */

/*******************  Bit definition for TIM_BDTR register  *******************/
#define  TIM_BDTR_DTG                        0x00FF            /*!<DTG[0:7] bits (Dead-Time Generator set-up) */
#define  TIM_BDTR_DTG_0                      0x0001            /*!<Bit 0 */
#define  TIM_BDTR_DTG_1                      0x0002            /*!<Bit 1 */
#define  TIM_BDTR_DTG_2                      0x0004            /*!<Bit 2 */
#define  TIM_BDTR_DTG_3                      0x0008            /*!<Bit 3 */
#define  TIM_BDTR_DTG_4                      0x0010            /*!<Bit 4 */
#define  TIM_BDTR_DTG_5                      0x0020            /*!<Bit 5 */
#define  TIM_BDTR_DTG_6                      0x0040            /*!<Bit 6 */
#define  TIM_BDTR_DTG_7                      0x0080            /*!<Bit 7 */

#define  TIM_BDTR_LOCK                       0x0300            /*!<LOCK[1:0] bits (Lock Configuration) */
#define  TIM_BDTR_LOCK_0                     0x0100            /*!<Bit 0 */
#define  TIM_BDTR_LOCK_1                     0x0200            /*!<Bit 1 */

#define  TIM_BDTR_OSSI                       0x0400            /*!<Off-State Selection for Idle mode */
#define  TIM_BDTR_OSSR                       0x0800            /*!<Off-State Selection for Run mode */
#define  TIM_BDTR_BKE                        0x1000            /*!<Break enable */
#define  TIM_BDTR_BKP                        0x2000            /*!<Break Polarity */
#define  TIM_BDTR_AOE                        0x4000            /*!<Automatic Output enable */
#define  TIM_BDTR_MOE                        0x8000            /*!<Main Output enable */

/*******************  Bit definition for TIM_DCR register  ********************/
#define  TIM_DCR_DBA                         0x001F            /*!<DBA[4:0] bits (DMA Base Address) */
#define  TIM_DCR_DBA_0                       0x0001            /*!<Bit 0 */
#define  TIM_DCR_DBA_1                       0x0002            /*!<Bit 1 */
#define  TIM_DCR_DBA_2                       0x0004            /*!<Bit 2 */
#define  TIM_DCR_DBA_3                       0x0008            /*!<Bit 3 */
#define  TIM_DCR_DBA_4                       0x0010            /*!<Bit 4 */

#define  TIM_DCR_DBL                         0x1F00            /*!<DBL[4:0] bits (DMA Burst Length) */
#define  TIM_DCR_DBL_0                       0x0100            /*!<Bit 0 */
#define  TIM_DCR_DBL_1                       0x0200            /*!<Bit 1 */
#define  TIM_DCR_DBL_2                       0x0400            /*!<Bit 2 */
#define  TIM_DCR_DBL_3                       0x0800            /*!<Bit 3 */
#define  TIM_DCR_DBL_4                       0x1000            /*!<Bit 4 */

/*******************  Bit definition for TIM_DMAR register  *******************/
#define  TIM_DMAR_DMAB                       0xFFFF            /*!<DMA register for burst accesses */

/*******************  Bit definition for TIM_OR register  *********************/
#define TIM14_OR_TI1_RMP                       0x0003            /*!<TI1_RMP[1:0] bits (TIM14 Input 4 remap) */
#define TIM14_OR_TI1_RMP_0                     0x0001            /*!<Bit 0 */
#define TIM14_OR_TI1_RMP_1                     0x0002            /*!<Bit 1 */
