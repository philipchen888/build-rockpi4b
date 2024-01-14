/*
 * Copyright 2017 Google, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "registers.h"

enum {
        APLLL_ID = 0,
        APLLB_ID,
        DPLL_ID,
        CPLL_ID,
        GPLL_ID,
        NPLL_ID,
        VPLL_ID,
        PPLL_ID
};

enum {
        /* PLL_CON0 */
        PLL_FBDIV_MASK                  = 0xfff,
        PLL_FBDIV_SHIFT                 = 0,

        /* PLL_CON1 */
        PLL_POSTDIV2_SHIFT              = 12,
        PLL_POSTDIV2_MASK               = 0x7 << PLL_POSTDIV2_SHIFT,
        PLL_POSTDIV1_SHIFT              = 8,
        PLL_POSTDIV1_MASK               = 0x7 << PLL_POSTDIV1_SHIFT,
        PLL_REFDIV_MASK                 = 0x3f,
        PLL_REFDIV_SHIFT                = 0,

        /* PLL_CON2 */
        PLL_LOCK_STATUS_SHIFT           = 31,
        PLL_LOCK_STATUS_MASK            = 1 << PLL_LOCK_STATUS_SHIFT,
        PLL_FRACDIV_MASK                = 0xffffff,
        PLL_FRACDIV_SHIFT               = 0,

        /* PLL_CON3 */
        PLL_MODE_SHIFT                  = 8,
        PLL_MODE_MASK                   = 3 << PLL_MODE_SHIFT,
        PLL_MODE_SLOW                   = 0,
        PLL_MODE_NORM,
        PLL_MODE_DEEP,
        PLL_DSMPD_SHIFT                 = 3,
        PLL_DSMPD_MASK                  = 1 << PLL_DSMPD_SHIFT,
        PLL_INTEGER_MODE                = 1,

        /* PMUCRU_CLKSEL_CON0 */
        PMU_PCLK_DIV_CON_MASK           = 0x1f,
        PMU_PCLK_DIV_CON_SHIFT          = 0,

        /* PMUCRU_CLKSEL_CON2 */
        I2C_DIV_CON_MASK                = 0x7f,
        CLK_I2C8_DIV_CON_SHIFT          = 8,
        CLK_I2C0_DIV_CON_SHIFT          = 0,

        /* CLKSEL_CON1 / CLKSEL_CON3 */
        PCLK_DBG_DIV_MASK               = 0x1f,
        PCLK_DBG_DIV_SHIFT              = 0x8,
        ATCLK_CORE_DIV_MASK             = 0x1f,
        ATCLK_CORE_DIV_SHIFT            = 0,

        /* CLKSEL_CON14 */
        PCLK_PERIHP_DIV_CON_SHIFT       = 12,
        PCLK_PERIHP_DIV_CON_MASK        = 0x7 << PCLK_PERIHP_DIV_CON_SHIFT,
        HCLK_PERIHP_DIV_CON_SHIFT       = 8,
        HCLK_PERIHP_DIV_CON_MASK        = 3 << HCLK_PERIHP_DIV_CON_SHIFT,
        ACLK_PERIHP_PLL_SEL_SHIFT       = 7,
        ACLK_PERIHP_PLL_SEL_MASK        = 1 << ACLK_PERIHP_PLL_SEL_SHIFT,
        ACLK_PERIHP_PLL_SEL_CPLL        = 0,
        ACLK_PERIHP_PLL_SEL_GPLL        = 1,
        ACLK_PERIHP_DIV_CON_SHIFT       = 0,
        ACLK_PERIHP_DIV_CON_MASK        = 0x1f,

        /* CLKSEL_CON21 */
        ACLK_EMMC_PLL_SEL_SHIFT         = 7,
        ACLK_EMMC_PLL_SEL_MASK          = 0x1 << ACLK_EMMC_PLL_SEL_SHIFT,
        ACLK_EMMC_PLL_SEL_GPLL          = 0x1,
        ACLK_EMMC_DIV_CON_SHIFT         = 0,
        ACLK_EMMC_DIV_CON_MASK          = 0x1f,

        /* CLKSEL_CON22 */
        CLK_EMMC_PLL_SHIFT              = 8,
        CLK_EMMC_PLL_MASK               = 0x7 << CLK_EMMC_PLL_SHIFT,
        CLK_EMMC_PLL_SEL_GPLL           = 0x1,
        CLK_EMMC_PLL_SEL_24M            = 0x5,
        CLK_EMMC_DIV_CON_SHIFT          = 0,
        CLK_EMMC_DIV_CON_MASK           = 0x7f << CLK_EMMC_DIV_CON_SHIFT,

        /* CLKSEL_CON23 */
        PCLK_PERILP0_DIV_CON_SHIFT      = 12,
        PCLK_PERILP0_DIV_CON_MASK       = 0x7 << PCLK_PERILP0_DIV_CON_SHIFT,
        HCLK_PERILP0_DIV_CON_SHIFT      = 8,
        HCLK_PERILP0_DIV_CON_MASK       = 3 << HCLK_PERILP0_DIV_CON_SHIFT,
        ACLK_PERILP0_PLL_SEL_SHIFT      = 7,
        ACLK_PERILP0_PLL_SEL_MASK       = 1 << ACLK_PERILP0_PLL_SEL_SHIFT,
        ACLK_PERILP0_PLL_SEL_CPLL       = 0,
        ACLK_PERILP0_PLL_SEL_GPLL       = 1,
        ACLK_PERILP0_DIV_CON_SHIFT      = 0,
        ACLK_PERILP0_DIV_CON_MASK       = 0x1f,

        /* CRU_CLK_SEL24_CON */
        CRYPTO0_PLL_SEL_SHIFT           = 6,
        CRYPTO0_PLL_SEL_MASK            = 3 << CRYPTO0_PLL_SEL_SHIFT,
        CRYPTO_PLL_SEL_CPLL             = 0,
        CRYPTO_PLL_SEL_GPLL,
        CRYPTO_PLL_SEL_PPLL             = 0,
        CRYPTO0_DIV_SHIFT               = 0,
        CRYPTO0_DIV_MASK                = 0x1f << CRYPTO0_DIV_SHIFT,

        /* CLKSEL_CON25 */
        PCLK_PERILP1_DIV_CON_SHIFT      = 8,
        PCLK_PERILP1_DIV_CON_MASK       = 0x7 << PCLK_PERILP1_DIV_CON_SHIFT,
        HCLK_PERILP1_PLL_SEL_SHIFT      = 7,
        HCLK_PERILP1_PLL_SEL_MASK       = 1 << HCLK_PERILP1_PLL_SEL_SHIFT,
        HCLK_PERILP1_PLL_SEL_CPLL       = 0,
        HCLK_PERILP1_PLL_SEL_GPLL       = 1,
        HCLK_PERILP1_DIV_CON_SHIFT      = 0,
        HCLK_PERILP1_DIV_CON_MASK       = 0x1f,

        /* CLKSEL_CON0 / CLKSEL_CON2 */
        ACLKM_CORE_DIV_CON_MASK = 0x1f,
        ACLKM_CORE_DIV_CON_SHIFT        = 8,
        CLK_CORE_PLL_SEL_MASK           = 3,
        CLK_CORE_PLL_SEL_SHIFT          = 6,
        CLK_CORE_PLL_SEL_ALPLL          = 0x0,
        CLK_CORE_PLL_SEL_ABPLL          = 0x1,
        CLK_CORE_PLL_SEL_DPLL           = 0x10,
        CLK_CORE_PLL_SEL_GPLL           = 0x11,
        CLK_CORE_DIV_MASK               = 0x1f,
        CLK_CORE_DIV_SHIFT              = 0,

};

void rkclk_init( void );
void pmuclk_init( void );

#endif
