#ifndef _CCMRAM_H_
#define _CCMRAM_H_

/**
Enable CCMRAM with:
__CCMDATARAMEN_CLK_ENABLE();


Scatter file example to use ccmram

LR_IROM1 0x08010000 0x000F0000  {    ; load region size_region
  ER_IROM1 0x08010000 0x000F0000  {  ; load address = execution address
   *.o (RESET, +First)
   *(InRoot$$Sections)
   .ANY (+RO)
  }
  RW_IRAM1 0x20000000 0x00020000  {  ; RW data (128K)
   .ANY (+RW +ZI)
  }
  RW_IRAM2 0x10000000 0x00010000  {  ; CCM data (64K)
    startup_stm32f40xx.o (STACK)
	startup_stm32f40xx.o (HEAP)
    .ANY (ccmram)
  }
}
*/

#define CCM_RAM_SECTION	__attribute__((section("ccmram")))

#endif
