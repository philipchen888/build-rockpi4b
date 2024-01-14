/*
 * Copyright 2017 Google, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef _RK_MMC_H_
#define _RK_MMC_H_

#include "registers.h"

/* Interrupt Mask register */
#define DWMCI_INTMSK_ALL        0xffffffff
#define DWMCI_INTMSK_RE         (1 << 1)
#define DWMCI_INTMSK_CDONE      (1 << 2)
#define DWMCI_INTMSK_DTO        (1 << 3)
#define DWMCI_INTMSK_TXDR       (1 << 4)
#define DWMCI_INTMSK_RXDR       (1 << 5)
#define DWMCI_INTMSK_DCRC       (1 << 7)
#define DWMCI_INTMSK_RTO        (1 << 8)
#define DWMCI_INTMSK_DRTO       (1 << 9)
#define DWMCI_INTMSK_HTO        (1 << 10)
#define DWMCI_INTMSK_FRUN       (1 << 11)
#define DWMCI_INTMSK_HLE        (1 << 12)
#define DWMCI_INTMSK_SBE        (1 << 13)
#define DWMCI_INTMSK_ACD        (1 << 14)
#define DWMCI_INTMSK_EBE        (1 << 15)

/* Raw interrupt Regsiter */
#define DWMCI_DATA_ERR  (DWMCI_INTMSK_EBE | DWMCI_INTMSK_SBE | DWMCI_INTMSK_HLE | DWMCI_INTMSK_FRUN | DWMCI_INTMSK_EBE | DWMCI_INTMSK_DCRC)
#define DWMCI_DATA_TOUT (DWMCI_INTMSK_HTO | DWMCI_INTMSK_DRTO)

/* CTRL register */
#define DWMCI_CTRL_RESET        (1 << 0)
#define DWMCI_CTRL_FIFO_RESET   (1 << 1)
#define DWMCI_CTRL_DMA_RESET    (1 << 2)
#define DWMCI_DMA_EN            (1 << 5)
#define DWMCI_CTRL_SEND_AS_CCSD (1 << 10)
#define DWMCI_IDMAC_EN          (1 << 25)
#define DWMCI_RESET_ALL         (DWMCI_CTRL_RESET | DWMCI_CTRL_FIFO_RESET | DWMCI_CTRL_DMA_RESET)

/* CMD register */
#define DWMCI_CMD_RESP_EXP      (1 << 6)
#define DWMCI_CMD_RESP_LENGTH   (1 << 7)
#define DWMCI_CMD_CHECK_CRC     (1 << 8)
#define DWMCI_CMD_DATA_EXP      (1 << 9)
#define DWMCI_CMD_RW            (1 << 10)
#define DWMCI_CMD_SEND_STOP     (1 << 12)
#define DWMCI_CMD_ABORT_STOP    (1 << 14)
#define DWMCI_CMD_PRV_DAT_WAIT  (1 << 13)
#define DWMCI_CMD_UPD_CLK       (1 << 21)
#define DWMCI_CMD_USE_HOLD_REG  (1 << 29)
#define DWMCI_CMD_START         (1 << 31)

/* CLKENA register */
#define DWMCI_CLKEN_ENABLE      (1 << 0)
#define DWMCI_CLKEN_LOW_PWR     (1 << 16)

/* Card-type registe */
#define DWMCI_CTYPE_1BIT        0
#define DWMCI_CTYPE_4BIT        (1 << 0)
#define DWMCI_CTYPE_8BIT        (1 << 16)

/* Status Register */
#define DWMCI_STATUS_FIFO_EMPTY	(1 << 2)
#define DWMCI_STATUS_FIFO_FULL	(1 << 3)
#define DWMCI_BUSY              (1 << 9)
#define DWMCI_FIFO_MASK         0x1fff
#define DWMCI_FIFO_SHIFT        17

/* FIFOTH Register */
#define MSIZE(x)                ((x) << 28)
#define RX_WMARK(x)             ((x) << 16)
#define TX_WMARK(x)             (x)
#define RX_WMARK_SHIFT          16
#define RX_WMARK_MASK           (0xfff << RX_WMARK_SHIFT)

#define DWMCI_IDMAC_OWN         (1 << 31)
#define DWMCI_IDMAC_CH          (1 << 4)
#define DWMCI_IDMAC_FS          (1 << 3)
#define DWMCI_IDMAC_LD          (1 << 2)

/*  Bus Mode Register */
#define DWMCI_BMOD_IDMAC_RESET  (1 << 0)
#define DWMCI_BMOD_IDMAC_FB     (1 << 1)
#define DWMCI_BMOD_IDMAC_EN     (1 << 7)

/* UHS register */
#define DWMCI_DDR_MODE  (1 << 16)

#define NULL	0
#define SD_CMD_APP_SET_BUS_WIDTH        6
#define SD_CMD_SEND_RELATIVE_ADDR       3
#define SD_CMD_SWITCH_FUNC              6
#define SD_CMD_SEND_IF_COND             8
#define SD_CMD_APP_SD_STATUS            13
#define SD_CMD_ERASE_WR_BLK_START       32
#define SD_CMD_ERASE_WR_BLK_END         33
#define SD_CMD_APP_SEND_OP_COND         41
#define SD_CMD_APP_SEND_SCR             51
#define MMC_DATA_READ                   1
#define MMC_DATA_WRITE                  2

#define MMC_CMD_GO_IDLE_STATE           0
#define MMC_CMD_SEND_OP_COND            1
#define MMC_CMD_ALL_SEND_CID            2
#define MMC_CMD_SET_RELATIVE_ADDR       3
#define MMC_CMD_SET_DSR                 4
#define MMC_CMD_SWITCH                  6
#define MMC_CMD_SELECT_CARD             7
#define MMC_CMD_SEND_EXT_CSD            8
#define MMC_CMD_SEND_CSD                9
#define MMC_CMD_SEND_CID                10
#define MMC_CMD_STOP_TRANSMISSION       12
#define MMC_CMD_SEND_STATUS             13
#define MMC_CMD_SET_BLOCKLEN            16
#define MMC_CMD_READ_SINGLE_BLOCK       17
#define MMC_CMD_READ_MULTIPLE_BLOCK     18
#define MMC_SEND_TUNING_BLOCK           19
#define MMC_SEND_TUNING_BLOCK_HS200     21
#define MMC_CMD_SET_BLOCK_COUNT         23
#define MMC_CMD_WRITE_SINGLE_BLOCK      24
#define MMC_CMD_WRITE_MULTIPLE_BLOCK    25
#define MMC_CMD_ERASE_GROUP_START       35
#define MMC_CMD_ERASE_GROUP_END         36
#define MMC_CMD_ERASE                   38
#define MMC_CMD_APP_CMD                 55
#define MMC_CMD_SPI_READ_OCR            58
#define MMC_CMD_SPI_CRC_ON_OFF          59
#define MMC_CMD_RES_MAN                 62

#define MMC_RSP_PRESENT (1 << 0)
#define MMC_RSP_136     (1 << 1)                /* 136 bit response */
#define MMC_RSP_CRC     (1 << 2)                /* expect valid crc */
#define MMC_RSP_BUSY    (1 << 3)                /* card may send busy */
#define MMC_RSP_OPCODE  (1 << 4)                /* response contains opcode */

#define MMC_RSP_NONE    (0)
#define MMC_RSP_R1      (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R1b     (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE|MMC_RSP_BUSY)
#define MMC_RSP_R2      (MMC_RSP_PRESENT|MMC_RSP_136|MMC_RSP_CRC)
#define MMC_RSP_R3      (MMC_RSP_PRESENT)
#define MMC_RSP_R4      (MMC_RSP_PRESENT)
#define MMC_RSP_R5      (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R6      (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R7      (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)

#define MMC_ERASE_ARG           0x00000000

#define MMC_STATUS_MASK         (~0x0206BF7F)
#define MMC_STATUS_SWITCH_ERROR (1 << 7)
#define MMC_STATUS_RDY_FOR_DATA (1 << 8)
#define MMC_STATUS_CURR_STATE   (0xf << 9)
#define MMC_STATUS_ERROR        (1 << 19)

#define MMC_STATE_PRG           (7 << 9)
#define MMC_STATE_RCV           (6 << 9)

#define MMC_VDD_165_195         0x00000080      /* VDD voltage 1.65 - 1.95 */
#define MMC_VDD_32_33           0x00100000      /* VDD voltage 3.2 ~ 3.3 */
#define MMC_VDD_33_34           0x00200000      /* VDD voltage 3.3 ~ 3.4 */
#define OCR_BUSY                0x80000000
#define OCR_HCS                 0x40000000

#define GEN_BB_READ             (1 << 0)
#define GEN_BB_WRITE            (1 << 1)
#define GEN_BB_RW               (GEN_BB_READ | GEN_BB_WRITE)
#define PAGE_SIZE               4096
#define SD_SWITCH_CHECK         0
#define SD_SWITCH_SWITCH        1
#define SD_HIGHSPEED_BUSY	0x00020000

struct mmc_cmd {
        unsigned short cmdidx;
        unsigned int resp_type;
        unsigned int cmdarg;
        unsigned int response[4];
};

struct mmc_data {
        union {
                char *dest;
                const char *src; /* src buffers don't get written to */
        };
        unsigned int flags;
        unsigned int blocks;
        unsigned int blocksize;
} __attribute__((aligned(64)));

struct dwmci_idmac {
        unsigned int flags;
        unsigned int cnt;
        unsigned int addr;
        unsigned int next_addr;
} __attribute__((aligned(64)));

struct bounce_buffer {
        /* Copy of data parameter passed to start() */
        void *user_buffer;
        /*
         * DMA-aligned buffer. This field is always set to the value that
         * should be used for DMA; either equal to .user_buffer, or to a
         * freshly allocated aligned buffer.
         */
        void *bounce_buffer;
        /* Copy of len parameter passed to start() */
        unsigned int len;
        /* DMA-aligned buffer length */
        unsigned int len_aligned;
        /* Copy of flags parameter passed to start() */
        unsigned int flags;
} __attribute__((aligned(64)));

extern unsigned int Get_PC();

int dwmci_wait_reset( unsigned int value );
int dwmci_init( void );
void rk_mmc_set_clk( int freq );
int dwmci_setup_bus( int freq );
int dwmci_set_ios( int bus_width, int freq );
int dwmci_data_transfer(  struct mmc_data *data );
int dwmci_send_cmd( int hasdata, struct mmc_data *data );
int mmc_set_blocklen( int len );
int mmc_go_idle( void );
int mmc_send_if_cond( void );
int sd_send_op_cond( void );
int sd_change_freq( void );
int sd_read_ssr( void );
void mmc_startup( void );
int mmc_read_blocks( void *dst, int start_lba, int blkcnt );
int mmc_bread( void *dst, int start_lba, int blkcnt );
void mmc_send_status( void );
void mmc_erase_blocks( int start_lba, int blkcnt );
void mmc_berase( int start_lba, int blkcnt );
void mmc_write_blocks( void *src, int start_lba, int blkcnt );
void mmc_bwrite( void *src, int start_lba, int blkcnt );
void mmc_init( void );

unsigned short mmcrca;
int fifo_size;
int fifoth_val;

struct mmc_cmd cmd;
struct dwmci_idmac cur_idmac[10];

#endif
