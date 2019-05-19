/* Linker script for the STM32F303K8 chip */

/* Define memory regions. */
MEMORY
{
  rom (rx)  : ORIGIN = 0x08000000, LENGTH = 64K
  ram (rwx) : ORIGIN = 0x20000000, LENGTH = 12K
}

/* Include the common ld script. */
INCLUDE cortex-m-generic.ld
