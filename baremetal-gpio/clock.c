/*
 * Copyright 2017 Google, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include "clock.h"
#include "timer.h"

void rkclk_set_pll( int pll_id )
{
    unsigned int fbdiv;
    unsigned int postdiv2;
    unsigned int postdiv1;
    unsigned int refdiv;

    if ( pll_id == APLLL_ID ) {
        fbdiv = 68;
        postdiv2 = 1;
        postdiv1 = 2;
        refdiv = 1;
        // force PLL into slow mode
        *CRU_LPLL_CON3 = (PLL_MODE_MASK << 16) | (PLL_MODE_SLOW << PLL_MODE_SHIFT);
        // use integer mode
        *CRU_LPLL_CON3 = (PLL_DSMPD_MASK << 16) | (PLL_INTEGER_MODE << PLL_DSMPD_SHIFT);
        *CRU_LPLL_CON0 = (PLL_FBDIV_MASK << 16) | (fbdiv << PLL_FBDIV_SHIFT);
        *CRU_LPLL_CON1 = ((PLL_POSTDIV2_MASK | PLL_POSTDIV1_MASK | PLL_REFDIV_MASK | PLL_REFDIV_SHIFT) << 16) | (postdiv2 << PLL_POSTDIV2_SHIFT | postdiv1 << PLL_POSTDIV1_SHIFT | refdiv << PLL_REFDIV_SHIFT);
        // waiting for pll lock
        while (!(*CRU_LPLL_CON2 & (1 << PLL_LOCK_STATUS_SHIFT)))
            udelay( 1 );
        // pll enter normal mode
        *CRU_LPLL_CON3 = (PLL_MODE_MASK << 16) | (PLL_MODE_NORM << PLL_MODE_SHIFT);
    } else if ( pll_id == NPLL_ID ) {
        fbdiv = 75;
        postdiv2 = 1;
        postdiv1 = 3;
        refdiv = 1;
        // force PLL into slow mode
        *CRU_NPLL_CON3 = (PLL_MODE_MASK << 16) | (PLL_MODE_SLOW << PLL_MODE_SHIFT);
        // use integer mode
        *CRU_NPLL_CON3 = (PLL_DSMPD_MASK << 16) | (PLL_INTEGER_MODE << PLL_DSMPD_SHIFT);
        *CRU_NPLL_CON0 = (PLL_FBDIV_MASK << 16) | (fbdiv << PLL_FBDIV_SHIFT);
        *CRU_NPLL_CON1 = ((PLL_POSTDIV2_MASK | PLL_POSTDIV1_MASK | PLL_REFDIV_MASK | PLL_REFDIV_SHIFT) << 16) | (postdiv2 << PLL_POSTDIV2_SHIFT | postdiv1 << PLL_POSTDIV1_SHIFT | refdiv << PLL_REFDIV_SHIFT);
        // waiting for pll lock
        while (!(*CRU_NPLL_CON2 & (1 << PLL_LOCK_STATUS_SHIFT)))
            udelay( 1 );
        // pll enter normal mode
        *CRU_NPLL_CON3 = (PLL_MODE_MASK << 16) | (PLL_MODE_NORM << PLL_MODE_SHIFT);
    } else if ( pll_id == GPLL_ID ) {
        fbdiv = 100;
        postdiv2 = 1;
        postdiv1 = 3;
        refdiv = 1;
        // force PLL into slow mode
        *CRU_GPLL_CON3 = (PLL_MODE_MASK << 16) | (PLL_MODE_SLOW << PLL_MODE_SHIFT);
        // use integer mode
        *CRU_GPLL_CON3 = (PLL_DSMPD_MASK << 16) | (PLL_INTEGER_MODE << PLL_DSMPD_SHIFT);
        *CRU_GPLL_CON0 = (PLL_FBDIV_MASK << 16) | (fbdiv << PLL_FBDIV_SHIFT);
        *CRU_GPLL_CON1 = ((PLL_POSTDIV2_MASK | PLL_POSTDIV1_MASK | PLL_REFDIV_MASK | PLL_REFDIV_SHIFT) << 16) | (postdiv2 << PLL_POSTDIV2_SHIFT | postdiv1 << PLL_POSTDIV1_SHIFT | refdiv << PLL_REFDIV_SHIFT);
        // waiting for pll lock
        while (!(*CRU_GPLL_CON2 & (1 << PLL_LOCK_STATUS_SHIFT)))
            udelay( 1 );
        // pll enter normal mode
        *CRU_GPLL_CON3 = (PLL_MODE_MASK << 16) | (PLL_MODE_NORM << PLL_MODE_SHIFT);
    } else if ( pll_id == PPLL_ID ) {
        fbdiv = 112;
        postdiv2 = 1;
        postdiv1 = 2;
        refdiv = 2;
        // force PLL into slow mode
        *PMUCRU_PPLL_CON3 = (PLL_MODE_MASK << 16) | (PLL_MODE_SLOW << PLL_MODE_SHIFT);
        // use integer mode
        *PMUCRU_PPLL_CON3 = (PLL_DSMPD_MASK << 16) | (PLL_INTEGER_MODE << PLL_DSMPD_SHIFT);
        *PMUCRU_PPLL_CON0 = (PLL_FBDIV_MASK << 16) | (fbdiv << PLL_FBDIV_SHIFT);
        *PMUCRU_PPLL_CON1 = ((PLL_POSTDIV2_MASK | PLL_POSTDIV1_MASK | PLL_REFDIV_MASK | PLL_REFDIV_SHIFT) << 16) | (postdiv2 << PLL_POSTDIV2_SHIFT | postdiv1 << PLL_POSTDIV1_SHIFT | refdiv << PLL_REFDIV_SHIFT);
        // waiting for pll lock
        while (!(*PMUCRU_PPLL_CON2 & (1 << PLL_LOCK_STATUS_SHIFT)))
            udelay( 1 );
        // pll enter normal mode
        *PMUCRU_PPLL_CON3 = (PLL_MODE_MASK << 16) | (PLL_MODE_NORM << PLL_MODE_SHIFT);
    }
}

void rk3399_configure_cpu( void )
{
    unsigned int aclkm_div;
    unsigned int pclk_dbg_div;
    unsigned int atclk_div;
    unsigned int apll_hz;

    printf("In rk3399_configure_cpu\r\n");
    rkclk_set_pll( APLLL_ID );

    apll_hz = 816;
    aclkm_div = 1;
    pclk_dbg_div = 7;
    atclk_div = 1;
    *CRU_CLKSEL_CON0 = ((ACLKM_CORE_DIV_CON_MASK | CLK_CORE_PLL_SEL_MASK | CLK_CORE_DIV_MASK) << 16) | (aclkm_div << ACLKM_CORE_DIV_CON_SHIFT | CLK_CORE_PLL_SEL_ALPLL << CLK_CORE_PLL_SEL_SHIFT);
    *CRU_CLKSEL_CON1 = ((PCLK_DBG_DIV_MASK | ATCLK_CORE_DIV_MASK) << 16) | (pclk_dbg_div << PCLK_DBG_DIV_SHIFT | atclk_div << ATCLK_CORE_DIV_SHIFT);
}

void pmuclk_init( void )
{
    unsigned int pclk_div;

    printf("In pmuclk_init\r\n");
    rkclk_set_pll( PPLL_ID );
    
    pclk_div = 13;
    *PMUCRU_CLKSEL_CON0 = (PMU_PCLK_DIV_CON_MASK << 16) | (pclk_div << PMU_PCLK_DIV_CON_SHIFT);
}

void rkclk_init( void )
{
    unsigned int aclk_div;
    unsigned int hclk_div;
    unsigned int pclk_div;

    printf("In rkclk_init\r\n");
    rk3399_configure_cpu();

    rkclk_set_pll( NPLL_ID );
    *CRU_CLKSEL_CON12 = (0xffff << 16) | 0x4101;
    *CRU_CLKSEL_CON19 = (0xffff << 16) | 0x033f;
    *CRU_CLKSEL_CON56 = (0x0003 << 16) | 0x0003;
    aclk_div = 5;
    hclk_div = 1;
    pclk_div = 3;
    *CRU_CLKSEL_CON14 = ((PCLK_PERIHP_DIV_CON_MASK | HCLK_PERIHP_DIV_CON_MASK | ACLK_PERIHP_PLL_SEL_MASK | ACLK_PERIHP_DIV_CON_MASK) << 16) | (pclk_div << PCLK_PERIHP_DIV_CON_SHIFT | hclk_div << HCLK_PERIHP_DIV_CON_SHIFT | ACLK_PERIHP_PLL_SEL_GPLL << ACLK_PERIHP_PLL_SEL_SHIFT | aclk_div << ACLK_PERIHP_DIV_CON_SHIFT);
    aclk_div = 2;
    hclk_div = 2;
    pclk_div = 5;
    *CRU_CLKSEL_CON23 = ((PCLK_PERILP0_DIV_CON_MASK | HCLK_PERILP0_DIV_CON_MASK | ACLK_PERILP0_PLL_SEL_MASK | ACLK_PERILP0_DIV_CON_MASK) << 16) | (pclk_div << PCLK_PERILP0_DIV_CON_SHIFT | hclk_div << HCLK_PERILP0_DIV_CON_SHIFT | ACLK_PERILP0_PLL_SEL_GPLL << ACLK_PERILP0_PLL_SEL_SHIFT | aclk_div << ACLK_PERILP0_DIV_CON_SHIFT);
    hclk_div = 7;
    pclk_div = 1;
    *CRU_CLKSEL_CON25 = ((PCLK_PERILP1_DIV_CON_MASK | HCLK_PERILP1_DIV_CON_MASK | HCLK_PERILP1_PLL_SEL_MASK) << 16) | (pclk_div << PCLK_PERILP1_DIV_CON_SHIFT | hclk_div << HCLK_PERILP1_DIV_CON_SHIFT | HCLK_PERILP1_PLL_SEL_GPLL << HCLK_PERILP1_PLL_SEL_SHIFT);
    *CRU_CLKSEL_CON21 = ((ACLK_EMMC_PLL_SEL_MASK | ACLK_EMMC_DIV_CON_MASK) << 16) | (ACLK_EMMC_PLL_SEL_GPLL << ACLK_EMMC_PLL_SEL_SHIFT | (4 - 1) << ACLK_EMMC_DIV_CON_SHIFT);
    *CRU_CLKSEL_CON22 = ((0x3f << 0) << 16) | (7 << 0);
    rkclk_set_pll( GPLL_ID );
}
