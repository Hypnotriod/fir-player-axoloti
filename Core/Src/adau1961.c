/*
 * ADAU1961.c
 *
 *  Created on: Sep 9, 2021
 *      Author: IPikin
 *
 *  Based on: https://github.com/axoloti/axoloti/blob/master/firmware/codec_ADAU1961_SAI.c
 */

#include "adau1961.h"
#include "main.h"
#include "i2c.h"
#include <stdbool.h>

#define I2C_TIMEOUT 1000000

void ADAU1961_writeRegister(uint16_t registerAddr, uint8_t RegisterValue)
{
  uint8_t txData[3];
  txData[0] = registerAddr >> 8;
  txData[1] = registerAddr;
  txData[2] = RegisterValue;
  HAL_I2C_Master_Transmit(&hi2c3, hi2c3.Init.OwnAddress1, txData, 3, I2C_TIMEOUT);
}

void ADAU1961_writeRegister6(uint16_t registerAddr, const uint8_t * registerValues)
{
  uint8_t txData[8];
  txData[0] = registerAddr >> 8;
  txData[1] = registerAddr;
  txData[2] = registerValues[0];
  txData[3] = registerValues[1];
  txData[4] = registerValues[2];
  txData[5] = registerValues[3];
  txData[6] = registerValues[4];
  txData[7] = registerValues[5];
  HAL_I2C_Master_Transmit(&hi2c3, hi2c3.Init.OwnAddress1, txData, 8, I2C_TIMEOUT);
}

void ADAU1961_readRegister6(uint16_t registerAddr, uint8_t * pData)
{
  uint8_t txData[2];
  txData[0] = registerAddr >> 8;
  txData[1] = registerAddr;
  HAL_I2C_Master_Transmit(&hi2c3, hi2c3.Init.OwnAddress1, txData, 2, I2C_TIMEOUT);
  HAL_I2C_Master_Receive(&hi2c3, hi2c3.Init.OwnAddress1, pData, 6, I2C_TIMEOUT);
}

void ADAU1961_setupPll(uint16_t samplerate)
{
  uint8_t txData[6];
  uint8_t rxData[6];

  /*
   * 1. Power down the PLL.
   * 2. Reset the PLL control register.
   * 3. Start the PLL.
   * 4. Poll the lock bit.
   * 5. Assert the core clock enable bit after the PLL lock is acquired.
   */

  ADAU1961_writeRegister(ADAU1961_REG_R0_CLKC, 0x00);

  if (samplerate == 48000)
  {
    // MCLK Input 8MHz, PLL Output = 45.1584 MHz, PLL Control Setting = 0x007D 0012 3101
    txData[0] = 0x00;
    txData[1] = 0x7D;
    txData[2] = 0x00;
    txData[3] = 0x12;
    txData[4] = 0x31;
    txData[5] = 0x01;
  }
  else if (samplerate == 44100)
  {
    // MCLK Input 8MHz, PLL Output = 49.152, PLL Control Setting = 0x0271 0193 2901
    txData[0] = 0x02;
    txData[1] = 0x71;
    txData[2] = 0x01;
    txData[3] = 0x93;
    txData[4] = 0x29;
    txData[5] = 0x01;
  }
  else
    Error_Handler();

  ADAU1961_writeRegister6(ADAU1961_REG_R1_PLLC, txData);

  int i = 100;
  while (i)
  {
    // wait for PLL
    ADAU1961_readRegister6(ADAU1961_REG_R1_PLLC, rxData);
    if (rxData[5] & 0x02)
      break;
    HAL_Delay(1);
    i--;
  }
  if (!i)
  {
    Error_Handler();
  }

  ADAU1961_writeRegister(ADAU1961_REG_R0_CLKC, 0x09);
}

void ADAU1961_hardwareInit(uint16_t samplerate, bool isMaster)
{
  if (isMaster)
    ADAU1961_setupPll(samplerate);

// Reset next registers to default values:
  ADAU1961_writeRegister(ADAU1961_REG_R2_DMICJ, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R3_RES, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R4_RMIXL0, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R5_RMIXL1, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R6_RMIXR0, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R7_RMIXR1, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R8_LDIVOL, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R9_RDIVOL, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R10_MICBIAS, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R11_ALC0, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R12_ALC1, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R13_ALC2, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R14_ALC3, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R15_SERP0, isMaster ? 0x01 : 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R16_SERP1, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R17_CON0, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R18_CON1, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R19_ADCC, 0x10);
  ADAU1961_writeRegister(ADAU1961_REG_R20_LDVOL, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R21_RDVOL, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R22_PMIXL0, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R23_PMIXL1, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R24_PMIXR0, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R25_PMIXR1, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R26_PLRML, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R27_PLRMR, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R28_PLRMM, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R29_PHPLVOL, 0x02);
  ADAU1961_writeRegister(ADAU1961_REG_R30_PHPRVOL, 0x02);
  ADAU1961_writeRegister(ADAU1961_REG_R31_PLLVOL, 0x02);
  ADAU1961_writeRegister(ADAU1961_REG_R32_PLRVOL, 0x02);
  ADAU1961_writeRegister(ADAU1961_REG_R33_PMONO, 0x02);
  ADAU1961_writeRegister(ADAU1961_REG_R34_POPCLICK, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R35_PWRMGMT, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R36_DACC0, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R37_DACC1, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R38_DACC2, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R39_SERPP, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R40_CPORTP0, 0xAA);
  ADAU1961_writeRegister(ADAU1961_REG_R41_CPORTP1, 0xAA);
  ADAU1961_writeRegister(ADAU1961_REG_R42_JACKDETP, 0x00);

  ADAU1961_writeRegister(ADAU1961_REG_R19_ADCC, 0x03); // ADC enable
  ADAU1961_writeRegister(ADAU1961_REG_R36_DACC0, 0x03); // DAC enable

  ADAU1961_writeRegister(ADAU1961_REG_R31_PLLVOL, 0xE7); // Playback Line Output Left Volume
  ADAU1961_writeRegister(ADAU1961_REG_R32_PLRVOL, 0xE7); // Playback Right Output Left Volume

  ADAU1961_writeRegister(ADAU1961_REG_R26_PLRML, 0x05); // unmute Mixer5, 6dB gain
  ADAU1961_writeRegister(ADAU1961_REG_R27_PLRMR, 0x11); // unmute Mixer6, 6dB gain
  ADAU1961_writeRegister(ADAU1961_REG_R22_PMIXL0, 0x21); // unmute DAC, no aux mix
  ADAU1961_writeRegister(ADAU1961_REG_R24_PMIXR0, 0x41); // unmute DAC, no aux mix

  ADAU1961_writeRegister(ADAU1961_REG_R35_PWRMGMT, 0x03); //enable L&R

  ADAU1961_writeRegister(ADAU1961_REG_R4_RMIXL0, 0x01); // mixer1 enable, mute LINP and LINR
  ADAU1961_writeRegister(ADAU1961_REG_R5_RMIXL1, 0x08); // unmute PGA, aux mute, 0 dB boost
  ADAU1961_writeRegister(ADAU1961_REG_R6_RMIXR0, 0x01); // mixer2 enable, mute LINP and LINR
  ADAU1961_writeRegister(ADAU1961_REG_R7_RMIXR1, 0x08); // unmute PGA, aux mute, 0 dB boost

  ADAU1961_writeRegister(ADAU1961_REG_R8_LDIVOL, 0x43); // 0dB gain
  ADAU1961_writeRegister(ADAU1961_REG_R9_RDIVOL, 0x43); // 0dB gain

  // capless headphone config
  ADAU1961_writeRegister(ADAU1961_REG_R33_PMONO, 0x03); // MONOM+MOMODE
  ADAU1961_writeRegister(ADAU1961_REG_R28_PLRMM, 0x01); // MX7EN, COMMON MODE OUT
  ADAU1961_writeRegister(ADAU1961_REG_R29_PHPLVOL, 0xC3);
  ADAU1961_writeRegister(ADAU1961_REG_R30_PHPRVOL, 0xC3);
}

void ADAU1961_Init(uint16_t samplerate, bool isMaster)
{
  ADAU1961_hardwareInit(samplerate, isMaster);
}

void ADAU1961_MuteInput(void)
{
  ADAU1961_writeRegister(ADAU1961_REG_R20_LDVOL, 0xFF);
  ADAU1961_writeRegister(ADAU1961_REG_R21_RDVOL, 0xFF);
}

void ADAU1961_UnmuteInput(void)
{
  ADAU1961_writeRegister(ADAU1961_REG_R20_LDVOL, 0x00);
  ADAU1961_writeRegister(ADAU1961_REG_R21_RDVOL, 0x00);
}
