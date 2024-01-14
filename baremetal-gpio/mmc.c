/*
 * Copyright 2017 Google, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include "mmc.h"
#include "timer.h"
#include "uart.h"

int dwmci_wait_reset( unsigned int value )
{
    *DWMCI_CTRL = value;
    while( *DWMCI_CTRL & DWMCI_RESET_ALL );
    return 0;
}

int dwmci_init( void )
{
    *DWMCI_PWREN = 1;
    dwmci_wait_reset( DWMCI_RESET_ALL );
    dwmci_setup_bus( 400000 );
    *DWMCI_RINTSTS = 0xFFFFFFFF;
    *DWMCI_INTMASK = 0;
    *DWMCI_TMOUT = 0xFFFFFF40;  // was 0xFFFFFFFF
    *DWMCI_IDINTEN = 0;
    *DWMCI_BMOD = 1;
    fifo_size = ((*DWMCI_FIFOTH & RX_WMARK_MASK) >> RX_WMARK_SHIFT) + 1;
    fifoth_val = MSIZE(0x2) | RX_WMARK(fifo_size / 2 - 1) | TX_WMARK(fifo_size / 2);
    *DWMCI_FIFOTH = fifoth_val;
    *DWMCI_CLKENA = 0;
    *DWMCI_CLKSRC = 0;
    return 0;
}

void rk3399_mmc_set_clk( int freq )
{
    int src_clk_div;
    int mux;

    if ( freq > 400000 ) {
        src_clk_div = 9;                  // 800 / 8 = 100 MHz
        *CRU_CLKSEL_CON16 = (0x77f << 16) | (0x1 << 8) | (src_clk_div - 1);
    } else {
        src_clk_div = 31;                 // 24000 / 30 = 800 KHz
        *CRU_CLKSEL_CON16 = (0x77f << 16) | (0x5 << 8) | (src_clk_div - 1);
    }
}

int dwmci_setup_bus( int freq )
{
    rk3399_mmc_set_clk( freq );
    *DWMCI_CLKENA = 0;
    *DWMCI_CLKSRC = 0;
    *DWMCI_CLKDIV = 0;  // bypass mode
    *DWMCI_CMD = DWMCI_CMD_PRV_DAT_WAIT | DWMCI_CMD_UPD_CLK | DWMCI_CMD_START;
    while( *DWMCI_CMD & DWMCI_CMD_START );
    *DWMCI_CLKENA = DWMCI_CLKEN_ENABLE;
    *DWMCI_CMD = DWMCI_CMD_PRV_DAT_WAIT | DWMCI_CMD_UPD_CLK | DWMCI_CMD_START;
    while( *DWMCI_CMD & DWMCI_CMD_START );
    return 0;
}

int dwmci_set_ios( int bus_width, int freq )
{
    dwmci_setup_bus( freq );               // 50 MHz, 400 KHz
    if ( bus_width == 1 ) {
        *DWMCI_CTYPE = DWMCI_CTYPE_1BIT;   // 1BIT
    } else if ( bus_width == 4 ) {
        *DWMCI_CTYPE = DWMCI_CTYPE_4BIT;   // 4BIT
    }
    return 0;
}

int dwmci_data_transfer( struct mmc_data *data )
{
    int ret;
    int i;
    int size;
    unsigned long long start;
    unsigned long long tmp2;
    unsigned long long timebase_h;
    unsigned long long timebase_l;

    unsigned int timeout = 240000;
    unsigned int fifo_depth;
    unsigned int ctrl;
    unsigned int mask;
    unsigned int len;
    unsigned int crc_ok;
    unsigned int *buf = NULL;

    fifo_depth = (((fifoth_val & RX_WMARK_MASK) >> RX_WMARK_SHIFT) + 1) <<1;
    size = 0;
    for ( i=0; i < data->blocks; i++ ) {
        size += data->blocksize;
    }
    size = (size >> 2);
    if ( data->flags == MMC_DATA_READ )
        buf = (unsigned int *)data->dest;
    else
        buf = (unsigned int *)data->src;

    timebase_l = *TIMER5_CURR_VALUE0;
    timebase_h = *TIMER5_CURR_VALUE1;
    start = (timebase_h << 32) | timebase_l;
    for (;;) {
        mask = *DWMCI_RINTSTS;
        /* Error during data transfer. */
        if ( mask & (DWMCI_DATA_ERR | DWMCI_DATA_TOUT) ) {
            if ( mask & DWMCI_DATA_ERR ) {
                printf( "DATA ERROR!\r\n");
            } else if ( mask & DWMCI_DATA_TOUT ) {
                printf( "DATA TIME OUT !\r\n");
            }
            *DWMCI_RINTSTS = 0xFFFFFFFF;
            dwmci_wait_reset( DWMCI_RESET_ALL );
            *DWMCI_CMD = DWMCI_CMD_PRV_DAT_WAIT | DWMCI_CMD_UPD_CLK | DWMCI_CMD_START;
            while( *DWMCI_CMD & DWMCI_CMD_START );

            *DWMCI_BMOD |= DWMCI_BMOD_IDMAC_RESET;
            ret = 1;
            break;
        }

        /* Data arrived correctly. */

        if ( mask & DWMCI_INTMSK_DTO ) {
            ret = 0;
            break;
        }

        /* Check for timeout. */
        timebase_l = *TIMER5_CURR_VALUE0;
        timebase_h = *TIMER5_CURR_VALUE1;
        tmp2 = (timebase_h << 32) | timebase_l;
        tmp2 -= start;
        if ( tmp2 > timeout ) {
            printf( "TIME OUT !\r\n");
            ret = 1;
            break;
        }
    }

    *DWMCI_RINTSTS = mask;
    return ret;
}

int dwmci_send_cmd( int hasdata, struct mmc_data *data )
{
    struct dwmci_idmac cur_idmac;
    struct bounce_buffer bbstate;
    unsigned int flags;
    unsigned int mask;
    unsigned int ctrl;
    unsigned int desc2;
    int ret;
    int len;
    int i;
    int j;
    int cnt;
    int cnt1;
    int blk_cnt;

    ret = 0;
    // wait for not busy
    while( *DWMCI_STATUS & DWMCI_BUSY );
    *DWMCI_RINTSTS = DWMCI_INTMSK_ALL;

    if ( hasdata ) {
        len = 0;
        for ( i=0; i < data->blocks; i++ ) {
            len += data->blocksize;
        }
        if (data->flags == MMC_DATA_READ) {
            bbstate.user_buffer = (void*)data->dest;
            bbstate.bounce_buffer = (void*)data->dest;
            bbstate.len = len;
            bbstate.len_aligned = 64;
            bbstate.flags = GEN_BB_WRITE;
        } else {
            bbstate.user_buffer = (void*)data->src;
            bbstate.bounce_buffer = (void*)data->src;
            bbstate.len = len;
            bbstate.len_aligned = 64;
            bbstate.flags = GEN_BB_READ;
        }
        i = 0;
        blk_cnt = data->blocks;
        dwmci_wait_reset( DWMCI_CTRL_FIFO_RESET );
        *DWMCI_DBADDR = (unsigned long long)&cur_idmac;
    
        while( 1 ) {
            flags = DWMCI_IDMAC_OWN | DWMCI_IDMAC_CH;
            if ( i == 0 ) {
                flags |= DWMCI_IDMAC_FS;
            }
            cnt = 0;
            if (blk_cnt <= 8) {
                flags |= DWMCI_IDMAC_LD;
                for ( j=0; j < blk_cnt; j++ ) {
                    cnt += data->blocksize;
                }
            } else {
                cnt = data->blocksize << 3;
            }
            cnt1 = 0;
            for ( j=0; j < i; j++ ) {
                cnt1 += PAGE_SIZE;
            }
            desc2 = (unsigned long)bbstate.bounce_buffer + cnt1;
            cur_idmac.flags = flags;
            cur_idmac.cnt = cnt;
            cur_idmac.addr = desc2;
            cur_idmac.next_addr = (unsigned long long)&cur_idmac + sizeof(struct dwmci_idmac);
            if (blk_cnt <= 8)
                break;
            blk_cnt -= 8;
            i++;
        }
    
        *DWMCI_CTRL |= (DWMCI_IDMAC_EN | DWMCI_DMA_EN);
        *DWMCI_BMOD |= (DWMCI_BMOD_IDMAC_FB | DWMCI_BMOD_IDMAC_EN);
    
        *DWMCI_BLKSIZ = data->blocksize;
        cnt = 0;
        for ( j=0; j < data->blocks; j++ ) {
            cnt += data->blocksize;
        }
        *DWMCI_BYTCNT = cnt;
    }

    *DWMCI_CMDARG = cmd.cmdarg;

    flags = 0;
    if ( hasdata ) {
        flags = DWMCI_CMD_DATA_EXP;
        if ( data->flags == MMC_DATA_WRITE )
            flags |= DWMCI_CMD_RW;           // This makes write sdcard working
    }

    if ( (cmd.resp_type & MMC_RSP_136) && (cmd.resp_type & MMC_RSP_BUSY) ) {
        printf( "ERROR ! dwmci_send_cmd got MMC_RSP_BUSY\r\n");
        return -1;
    }

    if ( cmd.cmdidx == MMC_CMD_STOP_TRANSMISSION )
        flags |= DWMCI_CMD_ABORT_STOP;
    else
        flags |= DWMCI_CMD_PRV_DAT_WAIT;

    if ( cmd.resp_type & MMC_RSP_PRESENT ) {
        flags |= DWMCI_CMD_RESP_EXP;
        if ( cmd.resp_type & MMC_RSP_136 )
            flags |= DWMCI_CMD_RESP_LENGTH;
    }

    if ( cmd.resp_type & MMC_RSP_CRC )
        flags |= DWMCI_CMD_CHECK_CRC;

    flags |= (cmd.cmdidx | DWMCI_CMD_START | DWMCI_CMD_USE_HOLD_REG);
    *DWMCI_CMD = flags;

    while( !(*DWMCI_RINTSTS & DWMCI_INTMSK_CDONE) );
    mask = *DWMCI_RINTSTS;
    if ( !hasdata ) {
        *DWMCI_RINTSTS = mask;
    }

    if ( mask & DWMCI_INTMSK_RTO) {
        printf( "dwmci_send_cmd response timeout !\r\n");
        return 1;
    }

    if ( cmd.resp_type & MMC_RSP_PRESENT ) {
        if ( cmd.resp_type & MMC_RSP_136 ) {
            cmd.response[0] = *DWMCI_RESP3;
            cmd.response[1] = *DWMCI_RESP2;
            cmd.response[2] = *DWMCI_RESP1;
            cmd.response[3] = *DWMCI_RESP0;
        } else {
            cmd.response[0] = *DWMCI_RESP0;
        }
    }

    if ( hasdata ) {
        ret = dwmci_data_transfer( data );

        ctrl = *DWMCI_CTRL;
        ctrl &= ~(DWMCI_DMA_EN);
        *DWMCI_CTRL = ctrl;       // stop DMA
    }

    udelay( 100 );
    return ret;
}

int mmc_set_blocklen( int len )
{
    cmd.cmdidx = MMC_CMD_SET_BLOCKLEN;
    cmd.resp_type = MMC_RSP_R1;
    cmd.cmdarg = len;

    return dwmci_send_cmd( 0, NULL );
}

int mmc_go_idle( void )
{
    udelay( 1000 );
    cmd.cmdidx = MMC_CMD_GO_IDLE_STATE;
    cmd.cmdarg = 0;
    cmd.resp_type = MMC_RSP_NONE;

    dwmci_send_cmd( 0, NULL );

    udelay( 2000 );
    return 0;
}

int mmc_send_if_cond( void )
{
    unsigned int cfgvoltages;

    cfgvoltages = MMC_VDD_32_33 | MMC_VDD_33_34 | MMC_VDD_165_195;
    cmd.cmdidx = SD_CMD_SEND_IF_COND;
    cmd.cmdarg = (1 << 8) | 0xaa;
    cmd.resp_type = MMC_RSP_R7;

    dwmci_send_cmd( 0, NULL );

    if ( (cmd.response[0] & 0xff) == 0xaa ) {
        printf( "SD_VERSION_2\r\n");
    } else {
        printf( "SD_VERSION = 0x%x\r\n", cmd.response[0]);
    }
    return 0;
}

int sd_send_op_cond( void )
{
    unsigned int cfgvoltages;

    cfgvoltages = MMC_VDD_32_33 | MMC_VDD_33_34 | MMC_VDD_165_195;

    while( 1 ) {
        cmd.cmdidx = MMC_CMD_APP_CMD;
        cmd.cmdarg = 0;
        cmd.resp_type = MMC_RSP_R1;

        dwmci_send_cmd( 0, NULL );

        cmd.cmdidx = SD_CMD_APP_SEND_OP_COND;
        cmd.resp_type = MMC_RSP_R3;
        cmd.cmdarg = (cfgvoltages & 0xff8000);
        cmd.cmdarg |= OCR_HCS;

        dwmci_send_cmd( 0, NULL );

        if ( cmd.response[0] & OCR_BUSY )
            break;
        udelay( 1000 );
    }
    if ( (cmd.response[0] & OCR_HCS) == OCR_HCS ) {
        printf( "Card is high capacity\r\n");
    }

    mmcrca = 0;
    return 0;
}

int sd_change_freq( void )
{
    struct mmc_data data;
    char scr[8] __attribute__((aligned(64)));
    char switch_status[64] __attribute__((aligned(64)));
    unsigned int scr0;
    unsigned int scr1;
    unsigned int switch_status0;
    unsigned int switch_status1;
    unsigned int switch_status2;
    unsigned int switch_status3;
    unsigned int switch_status4;
    unsigned int switch_status5;
    unsigned int switch_status6;
    unsigned int switch_status7;

    cmd.cmdidx = MMC_CMD_APP_CMD;
    cmd.resp_type = MMC_RSP_R1;
    cmd.cmdarg = mmcrca << 16;

    dwmci_send_cmd( 0, NULL );

    cmd.cmdidx = SD_CMD_APP_SEND_SCR;
    cmd.resp_type = MMC_RSP_R1;
    cmd.cmdarg = 0;
    data.dest = (char *)scr;
    data.blocksize = 8;
    data.blocks = 1;
    data.flags = MMC_DATA_READ;

    dwmci_send_cmd( 1, &data );

    scr0 = ((scr[0] & 0xff) << 24) + ((scr[1] & 0xff) << 16) + ((scr[2] & 0xff) << 8) + scr[3];
    scr1 = ((scr[4] & 0xff) << 24) + ((scr[5] & 0xff) << 16) + ((scr[6] & 0xff) << 8) + scr[7];

    cmd.cmdidx = SD_CMD_SWITCH_FUNC;
    cmd.resp_type = MMC_RSP_R1;
    cmd.cmdarg = (SD_SWITCH_CHECK << 31) | 0xffffff;
    cmd.cmdarg &= ~(0xf << (0 << 2));
    cmd.cmdarg |= 1 << (0 << 2);
    data.dest = (char *)switch_status;
    data.blocksize = 64;
    data.blocks = 1;
    data.flags = MMC_DATA_READ;

    dwmci_send_cmd( 1, &data );

    switch_status0 = ((switch_status[0] & 0xff) << 24) + ((switch_status[1] & 0xff) << 16) + ((switch_status[2] & 0xff) << 8) + switch_status[3];
    switch_status1 = ((switch_status[4] & 0xff) << 24) + ((switch_status[5] & 0xff) << 16) + ((switch_status[6] & 0xff) << 8) + switch_status[7];
    switch_status2 = ((switch_status[8] & 0xff) << 24) + ((switch_status[9] & 0xff) << 16) + ((switch_status[10] & 0xff) << 8) + switch_status[11];
    switch_status3 = ((switch_status[12] & 0xff) << 24) + ((switch_status[13] & 0xff) << 16) + ((switch_status[14] & 0xff) << 8) + switch_status[15];
    switch_status4 = ((switch_status[16] & 0xff) << 24) + ((switch_status[17] & 0xff) << 16) + ((switch_status[18] & 0xff) << 8) + switch_status[19];
    switch_status5 = ((switch_status[20] & 0xff) << 24) + ((switch_status[21] & 0xff) << 16) + ((switch_status[22] & 0xff) << 8) + switch_status[23];
    switch_status6 = ((switch_status[24] & 0xff) << 24) + ((switch_status[25] & 0xff) << 16) + ((switch_status[26] & 0xff) << 8) + switch_status[27];
    switch_status7 = ((switch_status[28] & 0xff) << 24) + ((switch_status[29] & 0xff) << 16) + ((switch_status[30] & 0xff) << 8) + switch_status[31];

    cmd.cmdidx = SD_CMD_SWITCH_FUNC;
    cmd.resp_type = MMC_RSP_R1;
    cmd.cmdarg = (SD_SWITCH_SWITCH << 31) | 0xffffff;
    cmd.cmdarg &= ~(0xf << (0 << 2));
    cmd.cmdarg |= 1 << (0 << 2);
    data.dest = (char *)switch_status;
    data.blocksize = 64;
    data.blocks = 1;
    data.flags = MMC_DATA_READ;

    dwmci_send_cmd( 1, &data );

    switch_status0 = ((switch_status[0] & 0xff) << 24) + ((switch_status[1] & 0xff) << 16) + ((switch_status[2] & 0xff) << 8) + switch_status[3];
    switch_status1 = ((switch_status[4] & 0xff) << 24) + ((switch_status[5] & 0xff) << 16) + ((switch_status[6] & 0xff) << 8) + switch_status[7];
    switch_status2 = ((switch_status[8] & 0xff) << 24) + ((switch_status[9] & 0xff) << 16) + ((switch_status[10] & 0xff) << 8) + switch_status[11];
    switch_status3 = ((switch_status[12] & 0xff) << 24) + ((switch_status[13] & 0xff) << 16) + ((switch_status[14] & 0xff) << 8) + switch_status[15];
    switch_status4 = ((switch_status[16] & 0xff) << 24) + ((switch_status[17] & 0xff) << 16) + ((switch_status[18] & 0xff) << 8) + switch_status[19];
    switch_status5 = ((switch_status[20] & 0xff) << 24) + ((switch_status[21] & 0xff) << 16) + ((switch_status[22] & 0xff) << 8) + switch_status[23];
    switch_status6 = ((switch_status[24] & 0xff) << 24) + ((switch_status[25] & 0xff) << 16) + ((switch_status[26] & 0xff) << 8) + switch_status[27];
    switch_status7 = ((switch_status[28] & 0xff) << 24) + ((switch_status[29] & 0xff) << 16) + ((switch_status[30] & 0xff) << 8) + switch_status[31];

    return 0;
}

int sd_read_ssr( void )
{
    struct mmc_data data;
    char ssr[64] __attribute__((aligned(64)));
    unsigned int ssr0;
    unsigned int ssr1;
    unsigned int ssr2;
    unsigned int ssr3;

    cmd.cmdidx = MMC_CMD_APP_CMD;
    cmd.resp_type = MMC_RSP_R1;
    cmd.cmdarg = mmcrca << 16;

    dwmci_send_cmd( 0, NULL );

    cmd.cmdidx = SD_CMD_APP_SD_STATUS;
    cmd.resp_type = MMC_RSP_R1;
    cmd.cmdarg = 0;
    data.dest = (char *)ssr;
    data.blocksize = 64;
    data.blocks = 1;
    data.flags = MMC_DATA_READ;

    dwmci_send_cmd( 1, &data );

    ssr0 = ((ssr[0] & 0xff) << 24) + ((ssr[1] & 0xff) << 16) + ((ssr[2] & 0xff) << 8) + ssr[3];
    ssr1 = ((ssr[4] & 0xff) << 24) + ((ssr[5] & 0xff) << 16) + ((ssr[6] & 0xff) << 8) + ssr[7];
    ssr2 = ((ssr[8] & 0xff) << 24) + ((ssr[9] & 0xff) << 16) + ((ssr[10] & 0xff) << 8) + ssr[11];
    ssr3 = ((ssr[12] & 0xff) << 24) + ((ssr[13] & 0xff) << 16) + ((ssr[14] & 0xff) << 8) + ssr[15];

    return 0;
}

void mmc_startup( void )
{
    int blklen;

    cmd.cmdidx = MMC_CMD_ALL_SEND_CID;
    cmd.resp_type = MMC_RSP_R2;
    cmd.cmdarg = 0;

    dwmci_send_cmd( 0, NULL );

    cmd.cmdidx = SD_CMD_SEND_RELATIVE_ADDR;
    cmd.resp_type = MMC_RSP_R6;
    cmd.cmdarg = mmcrca << 16;

    dwmci_send_cmd( 0, NULL );
    mmcrca = (cmd.response[0] >> 16) & 0xffff;

    cmd.cmdidx = MMC_CMD_SEND_CSD;
    cmd.resp_type = MMC_RSP_R2;
    cmd.cmdarg = mmcrca << 16;

    dwmci_send_cmd( 0, NULL );

    blklen = 1 << ((cmd.response[1] >> 16) & 0xf);

    cmd.cmdidx = MMC_CMD_SELECT_CARD;
    cmd.resp_type = MMC_RSP_R1;
    cmd.cmdarg = mmcrca << 16;

    dwmci_send_cmd( 0, NULL );

    sd_change_freq();

    cmd.cmdidx = MMC_CMD_APP_CMD;
    cmd.resp_type = MMC_RSP_R1;
    cmd.cmdarg = mmcrca << 16;

    dwmci_send_cmd( 0, NULL );

    cmd.cmdidx = SD_CMD_APP_SET_BUS_WIDTH;
    cmd.resp_type = MMC_RSP_R1;
    cmd.cmdarg = 2;

    dwmci_send_cmd( 0, NULL );

    dwmci_set_ios( 4, 400000 );

    sd_read_ssr();
    dwmci_set_ios( 4, 50000000 );
}

int mmc_read_blocks( void *dst, int start_lba, int blkcnt )
{
    struct mmc_data data;

    if (blkcnt > 1)
        cmd.cmdidx = MMC_CMD_READ_MULTIPLE_BLOCK;
    else
        cmd.cmdidx = MMC_CMD_READ_SINGLE_BLOCK;

    cmd.cmdarg = start_lba;
    cmd.resp_type = MMC_RSP_R1;

    data.dest = dst;
    data.blocks = blkcnt;
    data.blocksize = 512;
    data.flags = MMC_DATA_READ;

    if ( dwmci_send_cmd( 1, &data ) )
        return 0;

    if (blkcnt > 1) {
        cmd.cmdidx = MMC_CMD_STOP_TRANSMISSION;
        cmd.cmdarg = 0;
        cmd.resp_type = MMC_RSP_R1b;

        if ( dwmci_send_cmd( 0, NULL ) ) {
            printf( "mmc fail to send stop cmd\r\n");
            return 0;
        }
    } 

    return blkcnt;
}

int mmc_bread( void *dst, int start_lba, int blkcnt )
{
    mmc_set_blocklen( 512 );
    if ( mmc_read_blocks( dst, start_lba, blkcnt ) != blkcnt ) {
        printf( "Failed to read blocks\r\n");
        return 0;
    }

    return blkcnt;
}

void mmc_send_status( void )
{
    cmd.cmdidx = MMC_CMD_SEND_STATUS;
    cmd.resp_type = MMC_RSP_R1;
    cmd.cmdarg = mmcrca << 16;

    while( 1 ) {
        dwmci_send_cmd( 0, NULL );
        if ( (cmd.response[0] & MMC_STATUS_RDY_FOR_DATA) && ((cmd.response[0] & MMC_STATUS_CURR_STATE) != MMC_STATE_PRG) )
            break;
        else if ( cmd.response[0] & MMC_STATUS_MASK ) {
            printf( "Status Error !\r\n");
            break;
        } 
        udelay( 1000 );
    } 
}

void mmc_erase_blocks( int start_lba, int blkcnt )
{
    cmd.cmdidx = SD_CMD_ERASE_WR_BLK_START;
    cmd.cmdarg = start_lba;
    cmd.resp_type = MMC_RSP_R1;

    dwmci_send_cmd( 0, NULL );

    cmd.cmdidx = SD_CMD_ERASE_WR_BLK_END;
    cmd.cmdarg = start_lba + blkcnt - 1; 
    cmd.resp_type = MMC_RSP_R1;

    dwmci_send_cmd( 0, NULL );

    cmd.cmdidx = MMC_CMD_ERASE;
    cmd.cmdarg = MMC_ERASE_ARG;
    cmd.resp_type = MMC_RSP_R1b;

    if ( dwmci_send_cmd( 0, NULL ) )
        printf( "mmc erase failed\r\n");
}

void mmc_berase( int start_lba, int blkcnt )
{
    mmc_erase_blocks( start_lba, blkcnt );
    mmc_send_status();
}

void mmc_write_blocks( void *src, int start_lba, int blkcnt )
{
    struct mmc_data data;

    if (blkcnt > 1)
        cmd.cmdidx = MMC_CMD_WRITE_MULTIPLE_BLOCK;
    else
        cmd.cmdidx = MMC_CMD_WRITE_SINGLE_BLOCK;

    cmd.cmdarg = start_lba;
    cmd.resp_type = MMC_RSP_R1;

    data.dest = src;
    data.blocks = blkcnt;
    data.blocksize = 512;
    data.flags = MMC_DATA_WRITE;

    dwmci_send_cmd( 1, &data );

    if (blkcnt > 1) {
        cmd.cmdidx = MMC_CMD_STOP_TRANSMISSION;
        cmd.cmdarg = 0;
        cmd.resp_type = MMC_RSP_R1b;

        dwmci_send_cmd( 0, NULL );
    } 

    /* Waiting for the ready status */
    mmc_send_status();
}

void mmc_write_stop( void )
{
    cmd.cmdidx = MMC_CMD_STOP_TRANSMISSION;
    cmd.resp_type = MMC_RSP_R1;
    cmd.cmdarg = mmcrca << 16;

    dwmci_send_cmd( 0, NULL );
    mmc_send_status();
}

void mmc_bwrite( void *src, int start_lba, int blkcnt )
{
    mmc_set_blocklen( 512 );
    mmc_write_blocks( src, start_lba, blkcnt );
}

void mmc_init( void )
{
    *GPIO4B = (0xfff << 16) | 0x555;
    dwmci_init();
    dwmci_set_ios( 1, 400000 );
    mmc_go_idle();

    /* Test for SD version 2 */
    mmc_send_if_cond();
    /* Now try to get the SD card's operating condition */
    sd_send_op_cond();
    mmcrca = 1;
    mmc_startup();
}
