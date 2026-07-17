#pragma once

#define BIT_BAND(addr, n) (0x42000000 + (addr % 0x40000000) * 32 + n * 4)
#define BIT_ADDR(addr)    (*(volatile unsigned long *)(addr))
#define MEM_BIT(addr, n)  BIT_ADDR(BIT_BAND(addr, n))

#define GPIOA_ODR_ADDR    (GPIOA_BASE + 0XC)
#define GPIOB_ODR_ADDR    (GPIOB_BASE + 0XC)
#define GPIOC_ODR_ADDR    (GPIOC_BASE + 0XC)
#define GPIOD_ODR_ADDR    (GPIOD_BASE + 0XC)
#define GPIOE_ODR_ADDR    (GPIOE_BASE + 0XC)
#define GPIOF_ODR_ADDR    (GPIOF_BASE + 0XC)
#define GPIOG_ODR_ADDR    (GPIOG_BASE + 0XC)

#define GPIOA_IDR_ADDR    (GPIOA_BASE + 0X8)
#define GPIOB_IDR_ADDR    (GPIOB_BASE + 0X8)
#define GPIOC_IDR_ADDR    (GPIOC_BASE + 0X8)
#define GPIOD_IDR_ADDR    (GPIOD_BASE + 0X8)
#define GPIOE_IDR_ADDR    (GPIOE_BASE + 0X8)
#define GPIOF_IDR_ADDR    (GPIOF_BASE + 0X8)
#define GPIOG_IDR_ADDR    (GPIOG_BASE + 0X8)

#define PAout(n)          MEM_BIT(GPIOA_ODR_ADDR, n)
#define PBout(n)          MEM_BIT(GPIOB_ODR_ADDR, n)
#define PCout(n)          MEM_BIT(GPIOC_ODR_ADDR, n)
#define PDout(n)          MEM_BIT(GPIOD_ODR_ADDR, n)
#define PEout(n)          MEM_BIT(GPIOE_ODR_ADDR, n)
#define PFout(n)          MEM_BIT(GPIOF_ODR_ADDR, n)
#define PGout(n)          MEM_BIT(GPIOG_ODR_ADDR, n)

#define PAin(n)           MEM_BIT(GPIOA_IDR_ADDR, n)
#define PBin(n)           MEM_BIT(GPIOB_IDR_ADDR, n)
#define PCin(n)           MEM_BIT(GPIOC_IDR_ADDR, n)
#define PDin(n)           MEM_BIT(GPIOD_IDR_ADDR, n)
#define PEin(n)           MEM_BIT(GPIOE_IDR_ADDR, n)
#define PFin(n)           MEM_BIT(GPIOF_IDR_ADDR, n)
#define PGin(n)           MEM_BIT(GPIOG_IDR_ADDR, n)

void vGpio_Init(void);
