/*
 * Copyright 2017 Google, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __REGISTERS_H__
#define __REGISTERS_H__

#define LOW     0
#define HIGH    1
#define INPUT   0
#define OUTPUT  1
#define UART_LSR_THRE   0x20          /* Xmit holding register empty */
#define UART_LSR_DRY    0x1           /* data ready */

#define BOOTROM_BASE            0xff080000
#define SRAM_BASE               0xff090000

/* DMAC PL330 */
#define DMAC_BUS_DSR_NS             (volatile unsigned int *)0xff6d0000
#define DMAC_BUS_DSR                (volatile unsigned int *)0xff6d0000
#define DMAC_BUS_DPC                (volatile unsigned int *)0xff6d0004
#define DMAC_BUS_INTEN              (volatile unsigned int *)0xff6d0020
#define DMAC_BUS_INTCLR             (volatile unsigned int *)0xff6d002c
#define DMAC_BUS_FSRD               (volatile unsigned int *)0xff6d0030
#define DMAC_BUS_FSRC               (volatile unsigned int *)0xff6d0034
#define DMAC_BUS_FTRD               (volatile unsigned int *)0xff6d0038
#define DMAC_BUS_FTR0               (volatile unsigned int *)0xff6d0040
#define DMAC_BUS_CSR0               (volatile unsigned int *)0xff6d0100
#define DMAC_BUS_CPC0               (volatile unsigned int *)0xff6d0104
#define DMAC_BUS_SAR0               (volatile unsigned int *)0xff6d0400
#define DMAC_BUS_DAR0               (volatile unsigned int *)0xff6d0404
#define DMAC_BUS_CCR0               (volatile unsigned int *)0xff6d0408
#define DMAC_BUS_LC0_0              (volatile unsigned int *)0xff6d040c
#define DMAC_BUS_DBGST              (volatile unsigned int *)0xff6d0d00
#define DMAC_BUS_DBGCMD             (volatile unsigned int *)0xff6d0d04
#define DMAC_BUS_DBGINST0           (volatile unsigned int *)0xff6d0d08
#define DMAC_BUS_DBGINST1           (volatile unsigned int *)0xff6d0d0c
#define DMAC_BUS_CR0                (volatile unsigned int *)0xff6d0e00
#define DMAC_BUS_CR1                (volatile unsigned int *)0xff6d0e04
#define DMAC_BUS_CR2                (volatile unsigned int *)0xff6d0e08
#define DMAC_BUS_CR3                (volatile unsigned int *)0xff6d0e0c
#define DMAC_BUS_CR4                (volatile unsigned int *)0xff6d0e10
#define DMAC_BUS_CDRn               (volatile unsigned int *)0xff6d0e14

/* Distributor Registers */
#define GICD_CTLR               (volatile unsigned int *)0xff811000
#define GICD_TYPER              (volatile unsigned int *)0xff811004
#define GICD_IIDR               (volatile unsigned int *)0xff811008
#define GICD_IGROUPRn           (volatile unsigned int *)0xff811080
#define GICD_IGROUPRnx                                   0xff811080
#define GICD_ISENABLERn         (volatile unsigned int *)0xff811100
#define GICD_ISENABLERnx                                 0xff811100
#define GICD_ICENABLERn         (volatile unsigned int *)0xff811180
#define GICD_ICENABLERnx                                 0xff811180
#define GICD_ISPENDRn           (volatile unsigned int *)0xff811200
#define GICD_ICPENDRn           (volatile unsigned int *)0xff811280
#define GICD_ICACTIVERn         (volatile unsigned int *)0xff811380
#define GICD_IPRIORITYRn        (volatile unsigned int *)0xff811400
#define GICD_ITARGETSRn         (volatile unsigned int *)0xff811800
#define GICD_ITARGETSRnx                                 0xff811800
#define GICD_ICFGRn             (volatile unsigned int *)0xff811c00
#define GICD_ICFGRnx                                     0xff811c00
#define GICD_SGIR               (volatile unsigned int *)0xff811f00

/* Cpu Interface Memory Mapped Registers */
#define GICC_CTLR               (volatile unsigned int *)0xff812000
#define GICC_PMR                (volatile unsigned int *)0xff812004
#define GICC_BPR                (volatile unsigned int *)0xff812008
#define GICC_IAR                (volatile unsigned int *)0xff81200C
#define GICC_EOIR               (volatile unsigned int *)0xff812010
#define GICC_RPR                (volatile unsigned int *)0xff812014
#define GICC_HPPIR              (volatile unsigned int *)0xff812018
#define GICC_ABPR               (volatile unsigned int *)0xff81201c
#define GICC_IIDR               (volatile unsigned int *)0xff8120fc

#define I2S_TXCR                (volatile unsigned int *)0xff890000
#define I2S_RXCR                (volatile unsigned int *)0xff890004
#define I2S_CKR                 (volatile unsigned int *)0xff890008
#define I2S_FIFOLR              (volatile unsigned int *)0xff89000c
#define I2S_DMACR               (volatile unsigned int *)0xff890010
#define I2S_INTCR               (volatile unsigned int *)0xff890014
#define I2S_INTSR               (volatile unsigned int *)0xff890018
#define I2S_XFER                (volatile unsigned int *)0xff89001c
#define I2S_CLR                 (volatile unsigned int *)0xff890020
#define I2S_TXDR                (volatile unsigned int *)0xff890024
#define I2S_RXDR                (volatile unsigned int *)0xff890028

#define DWMCI_CTRL              (volatile unsigned int *)0xfe320000
#define DWMCI_PWREN             (volatile unsigned int *)0xfe320004
#define DWMCI_CLKDIV            (volatile unsigned int *)0xfe320008
#define DWMCI_CLKSRC            (volatile unsigned int *)0xfe32000C
#define DWMCI_CLKENA            (volatile unsigned int *)0xfe320010
#define DWMCI_TMOUT             (volatile unsigned int *)0xfe320014
#define DWMCI_CTYPE             (volatile unsigned int *)0xfe320018
#define DWMCI_BLKSIZ            (volatile unsigned int *)0xfe32001C
#define DWMCI_BYTCNT            (volatile unsigned int *)0xfe320020
#define DWMCI_INTMASK           (volatile unsigned int *)0xfe320024
#define DWMCI_CMDARG            (volatile unsigned int *)0xfe320028
#define DWMCI_CMD               (volatile unsigned int *)0xfe32002C
#define DWMCI_RESP0             (volatile unsigned int *)0xfe320030
#define DWMCI_RESP1             (volatile unsigned int *)0xfe320034
#define DWMCI_RESP2             (volatile unsigned int *)0xfe320038
#define DWMCI_RESP3             (volatile unsigned int *)0xfe32003C
#define DWMCI_MINTSTS           (volatile unsigned int *)0xfe320040
#define DWMCI_RINTSTS           (volatile unsigned int *)0xfe320044
#define DWMCI_STATUS            (volatile unsigned int *)0xfe320048
#define DWMCI_FIFOTH            (volatile unsigned int *)0xfe32004C
#define DWMCI_CDETECT           (volatile unsigned int *)0xfe320050
#define DWMCI_WRTPRT            (volatile unsigned int *)0xfe320054
#define DWMCI_GPIO              (volatile unsigned int *)0xfe320058
#define DWMCI_TCMCNT            (volatile unsigned int *)0xfe32005C
#define DWMCI_TBBCNT            (volatile unsigned int *)0xfe320060
#define DWMCI_DEBNCE            (volatile unsigned int *)0xfe320064
#define DWMCI_USRID             (volatile unsigned int *)0xfe320068
#define DWMCI_VERID             (volatile unsigned int *)0xfe32006C
#define DWMCI_HCON              (volatile unsigned int *)0xfe320070
#define DWMCI_UHS_REG           (volatile unsigned int *)0xfe320074
#define DWMCI_BMOD              (volatile unsigned int *)0xfe320080
#define DWMCI_PLDMND            (volatile unsigned int *)0xfe320084
#define DWMCI_DBADDR            (volatile unsigned int *)0xfe320088
#define DWMCI_IDSTS             (volatile unsigned int *)0xfe32008C
#define DWMCI_IDINTEN           (volatile unsigned int *)0xfe320090
#define DWMCI_DSCADDR           (volatile unsigned int *)0xfe320094
#define DWMCI_BUFADDR           (volatile unsigned int *)0xfe320098
#define DWMCI_DATA              (volatile unsigned int *)0xfe320200

#define SPI1_CTRLR0    (volatile unsigned int *)0xff1d0000
#define SPI1_CTRLR1    (volatile unsigned int *)0xff1d0004
#define SPI1_ENR       (volatile unsigned int *)0xff1d0008
#define SPI1_SER       (volatile unsigned int *)0xff1d000c
#define SPI1_BAUDR     (volatile unsigned int *)0xff1d0010
#define SPI1_TXFTLR    (volatile unsigned int *)0xff1d0014
#define SPI1_RXFTLR    (volatile unsigned int *)0xff1d0018
#define SPI1_TXFLR     (volatile unsigned int *)0xff1d001c
#define SPI1_RXFLR     (volatile unsigned int *)0xff1d0020
#define SPI1_SR        (volatile unsigned int *)0xff1d0024
#define SPI1_IPR       (volatile unsigned int *)0xff1d0028
#define SPI1_IMR       (volatile unsigned int *)0xff1d002c
#define SPI1_ISR       (volatile unsigned int *)0xff1d0030
#define SPI1_RISR      (volatile unsigned int *)0xff1d0034
#define SPI1_ICR       (volatile unsigned int *)0xff1d0038
#define SPI1_DMACR     (volatile unsigned int *)0xff1d003c
#define SPI1_DMATDLR   (volatile unsigned int *)0xff1d0040
#define SPI1_DMARDLR   (volatile unsigned int *)0xff1d0044
#define SPI1_TXDR      (volatile unsigned int *)0xff1d0400
#define SPI1_RXDR      (volatile unsigned int *)0xff1d0800

#define I2C7_CON       (volatile unsigned int *)0xff160000
#define I2C7_CLKDIV    (volatile unsigned int *)0xff160004
#define I2C7_MRXADDR   (volatile unsigned int *)0xff160008
#define I2C7_MRXRADDR  (volatile unsigned int *)0xff16000c
#define I2C7_MTXCNT    (volatile unsigned int *)0xff160010
#define I2C7_MRXCNT    (volatile unsigned int *)0xff160014
#define I2C7_IEN       (volatile unsigned int *)0xff160018
#define I2C7_IPD       (volatile unsigned int *)0xff16001c
#define I2C7_FCNT      (volatile unsigned int *)0xff160020
#define I2C7_TXDATA    (volatile unsigned int *)0xff160100
#define I2C7_RXDATA    (volatile unsigned int *)0xff160200
#define I2C7_TXDATA_BASE (volatile unsigned int *)0xff160100
#define I2C7_RXDATA_BASE (volatile unsigned int *)0xff160200

#define UART0_THR (volatile unsigned int *)0xff180000
#define UART0_DR  (volatile unsigned int *)0xff180000
#define UART0_DLL (volatile unsigned int *)0xff180000
#define UART0_DLM (volatile unsigned int *)0xff180004
#define UART0_IER (volatile unsigned int *)0xff180004
#define UART0_FCR (volatile unsigned int *)0xff180008
#define UART0_IIR (volatile unsigned int *)0xff180008
#define UART0_LCR (volatile unsigned int *)0xff18000c
#define UART0_MCR (volatile unsigned int *)0xff180010
#define UART0_LSR (volatile unsigned int *)0xff180014
#define UART0_MSR (volatile unsigned int *)0xff180018
#define UART0_USR (volatile unsigned int *)0xff18007c
#define UART1_THR (volatile unsigned int *)0xff190000
#define UART1_DR  (volatile unsigned int *)0xff190000
#define UART1_DLL (volatile unsigned int *)0xff190000
#define UART1_DLM (volatile unsigned int *)0xff190004
#define UART1_IER (volatile unsigned int *)0xff190004
#define UART1_FCR (volatile unsigned int *)0xff190008
#define UART1_IIR (volatile unsigned int *)0xff190008
#define UART1_LCR (volatile unsigned int *)0xff19000c
#define UART1_MCR (volatile unsigned int *)0xff190010
#define UART1_LSR (volatile unsigned int *)0xff190014
#define UART1_MSR (volatile unsigned int *)0xff190018
#define UART1_USR (volatile unsigned int *)0xff19007c
#define UART2_THR (volatile unsigned int *)0xff1a0000
#define UART2_DR  (volatile unsigned int *)0xff1a0000
#define UART2_DLL (volatile unsigned int *)0xff1a0000
#define UART2_DLM (volatile unsigned int *)0xff1a0004
#define UART2_IER (volatile unsigned int *)0xff1a0004
#define UART2_FCR (volatile unsigned int *)0xff1a0008
#define UART2_IIR (volatile unsigned int *)0xff1a0008
#define UART2_LCR (volatile unsigned int *)0xff1a000c
#define UART2_MCR (volatile unsigned int *)0xff1a0010
#define UART2_LSR (volatile unsigned int *)0xff1a0014
#define UART2_MSR (volatile unsigned int *)0xff1a0018
#define UART2_USR (volatile unsigned int *)0xff1a007c
#define UART4_THR (volatile unsigned int *)0xff370000
#define UART4_DR  (volatile unsigned int *)0xff370000
#define UART4_DLL (volatile unsigned int *)0xff370000
#define UART4_DLM (volatile unsigned int *)0xff370004
#define UART4_IER (volatile unsigned int *)0xff370004
#define UART4_FCR (volatile unsigned int *)0xff370008
#define UART4_IIR (volatile unsigned int *)0xff370008
#define UART4_LCR (volatile unsigned int *)0xff37000c
#define UART4_MCR (volatile unsigned int *)0xff370010
#define UART4_LSR (volatile unsigned int *)0xff370014
#define UART4_MSR (volatile unsigned int *)0xff370018
#define UART4_USR (volatile unsigned int *)0xff37007c

#define TIMER5_LOAD_COUNT0 (volatile unsigned int *)0xff8500a0
#define TIMER5_LOAD_COUNT1 (volatile unsigned int *)0xff8500a4
#define TIMER5_CURR_VALUE0 (volatile unsigned int *)0xff8500a8
#define TIMER5_CURR_VALUE1 (volatile unsigned int *)0xff8500ac
#define TIMER5_LOAD_COUNT2 (volatile unsigned int *)0xff8500b0
#define TIMER5_LOAD_COUNT3 (volatile unsigned int *)0xff8500b4
#define TIMER5_CTRL_REG    (volatile unsigned int *)0xff8500bc
#define TIMER5_INT_STATUS  (volatile unsigned int *)0xff8500b8

#define CRU_LPLL_CON0     (volatile unsigned int *)0xff760000
#define CRU_LPLL_CON1     (volatile unsigned int *)0xff760004
#define CRU_LPLL_CON2     (volatile unsigned int *)0xff760008
#define CRU_LPLL_CON3     (volatile unsigned int *)0xff76000c
#define CRU_LPLL_CON4     (volatile unsigned int *)0xff760010
#define CRU_LPLL_CON5     (volatile unsigned int *)0xff760014
#define CRU_BPLL_CON0     (volatile unsigned int *)0xff760020
#define CRU_BPLL_CON1     (volatile unsigned int *)0xff760024
#define CRU_BPLL_CON2     (volatile unsigned int *)0xff760028
#define CRU_BPLL_CON3     (volatile unsigned int *)0xff76002c
#define CRU_BPLL_CON4     (volatile unsigned int *)0xff760030
#define CRU_BPLL_CON5     (volatile unsigned int *)0xff760034
#define CRU_DPLL_CON0     (volatile unsigned int *)0xff760040
#define CRU_DPLL_CON1     (volatile unsigned int *)0xff760044
#define CRU_DPLL_CON2     (volatile unsigned int *)0xff760048
#define CRU_DPLL_CON3     (volatile unsigned int *)0xff76004c
#define CRU_DPLL_CON4     (volatile unsigned int *)0xff760050
#define CRU_DPLL_CON5     (volatile unsigned int *)0xff760054
#define CRU_CPLL_CON0     (volatile unsigned int *)0xff760060
#define CRU_CPLL_CON1     (volatile unsigned int *)0xff760064
#define CRU_CPLL_CON2     (volatile unsigned int *)0xff760068
#define CRU_CPLL_CON3     (volatile unsigned int *)0xff76006c
#define CRU_CPLL_CON4     (volatile unsigned int *)0xff760070
#define CRU_CPLL_CON5     (volatile unsigned int *)0xff760074
#define CRU_GPLL_CON0     (volatile unsigned int *)0xff760080
#define CRU_GPLL_CON1     (volatile unsigned int *)0xff760084
#define CRU_GPLL_CON2     (volatile unsigned int *)0xff760088
#define CRU_GPLL_CON3     (volatile unsigned int *)0xff76008c
#define CRU_GPLL_CON4     (volatile unsigned int *)0xff760090
#define CRU_GPLL_CON5     (volatile unsigned int *)0xff760094
#define CRU_NPLL_CON0     (volatile unsigned int *)0xff7600a0
#define CRU_NPLL_CON1     (volatile unsigned int *)0xff7600a4
#define CRU_NPLL_CON2     (volatile unsigned int *)0xff7600a8
#define CRU_NPLL_CON3     (volatile unsigned int *)0xff7600ac
#define CRU_NPLL_CON4     (volatile unsigned int *)0xff7600b0
#define CRU_NPLL_CON5     (volatile unsigned int *)0xff7600b4

// #define CRU_MODE_CON        (volatile unsigned int *)0xff440080
#define PMUCRU_PPLL_CON0    (volatile unsigned int *)0xff750000
#define PMUCRU_PPLL_CON1    (volatile unsigned int *)0xff750004
#define PMUCRU_PPLL_CON2    (volatile unsigned int *)0xff750008
#define PMUCRU_PPLL_CON3    (volatile unsigned int *)0xff75000c
#define PMUCRU_PPLL_CON4    (volatile unsigned int *)0xff750010
#define PMUCRU_PPLL_CON5    (volatile unsigned int *)0xff750014
#define PMUCRU_CLKSEL_CON0  (volatile unsigned int *)0xff750080
#define PMUCRU_CLKSEL_CON1  (volatile unsigned int *)0xff750084
#define PMUCRU_CLKSEL_CON2  (volatile unsigned int *)0xff750088
#define PMUCRU_CLKSEL_CON3  (volatile unsigned int *)0xff75008c
#define PMUCRU_CLKSEL_CON4  (volatile unsigned int *)0xff750090
#define PMUCRU_CLKSEL_CON5  (volatile unsigned int *)0xff750094
#define PMUGRF_GPIO1B_IOMUX (volatile unsigned int *)0xff320014
#define PMUGRF_GPIO1A       (volatile unsigned int *)0xFF320010
#define PMUGRF_GPIO1B       (volatile unsigned int *)0xFF320014
#define CRU_CLKSEL_CON0    (volatile unsigned int *)0xff760100
#define CRU_CLKSEL_CON1    (volatile unsigned int *)0xff760104
#define CRU_CLKSEL_CON2    (volatile unsigned int *)0xff760108
#define CRU_CLKSEL_CON3    (volatile unsigned int *)0xff76010c
#define CRU_CLKSEL_CON12   (volatile unsigned int *)0xff760130
#define CRU_CLKSEL_CON14   (volatile unsigned int *)0xff760138
#define CRU_CLKSEL_CON16   (volatile unsigned int *)0xff760140
#define CRU_CLKSEL_CON19   (volatile unsigned int *)0xff76014c
#define CRU_CLKSEL_CON21   (volatile unsigned int *)0xff760154
#define CRU_CLKSEL_CON22   (volatile unsigned int *)0xff760158
#define CRU_CLKSEL_CON23   (volatile unsigned int *)0xff76015c
#define CRU_CLKSEL_CON25   (volatile unsigned int *)0xff760164
#define CRU_CLKSEL_CON35   (volatile unsigned int *)0xff76018c
#define CRU_CLKSEL_CON56   (volatile unsigned int *)0xff7601e0
#define CRU_CLKSEL_CON59   (volatile unsigned int *)0xff7601ec
#define CRU_CLKSEL_CON63   (volatile unsigned int *)0xff7601fc
#define GPIO3_SWPORTA_DR       (volatile unsigned int *)0xFF788000
#define GPIO3_SWPORTA_DDR      (volatile unsigned int *)0xFF788004
#define GPIO1_SWPORTA_DR       (volatile unsigned int *)0xFF730000
#define GPIO1_SWPORTA_DDR      (volatile unsigned int *)0xFF730004
#define GPIO1_EXT_PORTA        (volatile unsigned int *)0xFF730050
#define GPIO2A                 (volatile unsigned int *)0xFF77e000
#define GPIO2B                 (volatile unsigned int *)0xFF77e004
#define GPIO3C                 (volatile unsigned int *)0xFF77e018
#define GPIO4B                 (volatile unsigned int *)0xFF77e024
#define GPIO4C                 (volatile unsigned int *)0xFF77e028

#define PWM1_CTRL              (volatile unsigned int *)0xFF42001c
#define PWM1_DUTY              (volatile unsigned int *)0xFF420018
#define PWM1_PERIOD            (volatile unsigned int *)0xFF420014

void printf(const char *fmt, ...);

#endif
