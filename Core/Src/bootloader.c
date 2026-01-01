/*
 * bootloader.c
 *
 *  Created on: Jan 1, 2026
 *      Author: tobias
 */

#include "main.h"

void JumpToBootloader(void)
{
    uint32_t bootloaderAddress = 0x1FFF0000;

    // Function pointer to the bootloader's reset handler
    void (*SysMemBootJump)(void);

    // Deinitialize everything that might interfere
    HAL_RCC_DeInit();
    HAL_DeInit();
    __disable_irq();

    // Set the main stack pointer to the bootloader's initial value
    __set_MSP(*(__IO uint32_t*)bootloaderAddress);

    // Get the bootloader's reset handler address
    uint32_t jumpAddress = *(__IO uint32_t*)(bootloaderAddress + 4);
    SysMemBootJump = (void (*)(void))jumpAddress;

    // Jump to bootloader
    SysMemBootJump();

    // Should never return
    while (1);
}
