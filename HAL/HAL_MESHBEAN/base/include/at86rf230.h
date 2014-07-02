/**
 * Copyright (c) 2005-2007 LuxLabs Ltd. dba MeshNetics, All Rights Reserved.
 *
 * No changes (alteration, merge, modification, reverse engineer) can be applied
 * to the source or binary code without specific prior written permission.   
 *
 * Redistribution, sublicensing and use in source and binary forms 
 * are permitted provided that the following conditions are met:
 *
 * - Redistribution, sublicensing of this source code and its derivatives is 
 *  allowed only with expressed written permission of MeshNetics and must 
 *  retain the above copyright notice, this list of conditions and 
 *  the following disclaimer.
 *
 * - Redistribution, sublicensing of this software or its derivatives in binary
 *  form is allowed only with expressed written permission of MeshNetics and 
 *  must reproduce the above copyright notice, this list of conditions and 
 *  the following disclaimer in the documentation and/or other materials 
 *  provided with the distribution.
 *
 * Neither the name of LuxLabs Ltd., MeshNetics, or the names of contributors 
 * may be used to endorse or promote products derived from this software 
 * without specific prior written permission.
 *
 * This software is provided "AS IS," without a warranty or support of any kind.
 * ALL EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING 
 * ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
 * OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED. MESHNETICS AND ITS LICENSORS SHALL 
 * NOT BE LIABLE FOR ANY DAMAGES OR LIABILITIES SUFFERED BY LICENSEE AS A RESULT
 * OF OR RELATING TO USE, MODIFICATION OR DISTRIBUTION OF THE SOFTWARE OR ITS 
 * DERIVATIVES. IN NO EVENT WILL MESHNETICS OR ITS LICENSORS BE LIABLE 
 * FOR ANY LOST REVENUE, PROFIT OR DATA, OR FOR DIRECT, INDIRECT, SPECIAL, 
 * CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER CAUSED AND REGARDLESS 
 * OF THE THEORY OF LIABILITY, ARISING OUT OF THE USE OF OR INABILITY TO USE 
 * SOFTWARE, EVEN IF MESHNETICS HAS BEEN ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGES.
 *
 * You acknowledge that this software is not designed, licensed or intended for 
 * use in the design, construction, operation or maintenance of any nuclear 
 * facility, medical or other mission-critical system.
 **/

/********************************************************************************
  Declaration constants for Atmel AT86RF230.
********************************************************************************/

#ifndef _AT86RF230_H
#define _AT86RF230_H

enum //mask which defined supported channels
{
  AT86RF230_SUPPORTED_CHANNELS = 0x07fff800,
};

typedef enum // Registers' addresses.
{
  AT86RF230_TRX_STATUS_REG   = 0x01,
  AT86RF230_TRX_STATE_REG    = 0x02,
  AT86RF230_TRX_CTRL_0_REG   = 0x03,
  AT86RF230_PHY_TX_PWR_REG   = 0x05,
  AT86RF230_PHY_RSSI_REG     = 0x06,
  AT86RF230_PHY_ED_LEVEL_REG = 0x07,
  AT86RF230_PHY_CC_CCA_REG   = 0x08,
  AT86RF230_IRQ_MASK_REG     = 0x0E,
  AT86RF230_IRQ_STATUS_REG   = 0x0F,
  AT86RF230_VREG_CTRL_REG    = 0x10,
  AT86RF230_BATMON_REG       = 0x11,
  AT86RF230_XOSC_CTRL_REG    = 0x12,
  AT86RF230_FTN_CTRL_REG     = 0x18,
  AT86RF230_VERSION_NUM_REG  = 0x1D,
  AT86RF230_MAN_ID_0_REG     = 0x1E,
  AT86RF230_MAN_ID_1_REG     = 0x1F,
  AT86RF230_SHORT_ADDR_0_REG = 0x20,
  AT86RF230_SHORT_ADDR_1_REG = 0x21,
  AT86RF230_PAN_ID_0_REG     = 0x22,
  AT86RF230_PAN_ID_1_REG     = 0x23,
  AT86RF230_IEEE_ADDR_0_REG  = 0x24,
  AT86RF230_IEEE_ADDR_1_REG  = 0x25,
  AT86RF230_IEEE_ADDR_2_REG  = 0x26,
  AT86RF230_IEEE_ADDR_3_REG  = 0x27,
  AT86RF230_IEEE_ADDR_4_REG  = 0x28,
  AT86RF230_IEEE_ADDR_5_REG  = 0x29,
  AT86RF230_IEEE_ADDR_6_REG  = 0x2A,
  AT86RF230_IEEE_ADDR_7_REG  = 0x2B,
  AT86RF230_XAH_CTRL_REG     = 0x2C,
  AT86RF230_CSMA_SEED_0_REG  = 0x2D,
  AT86RF230_CSMA_SEED_1_REG  = 0x2E,
} AT86RF230Reg;

typedef struct
{
  uint8_t trx_status : 5;
  uint8_t reserved   : 1;
  uint8_t cca_status : 1;
  uint8_t cca_done   : 1;
} PACK RegTrxStatus;

typedef struct
{
  uint8_t trx_cmd     : 5;
  uint8_t trac_status : 3;
} PACK RegTrxState;

typedef struct
{
  uint8_t clkm_ctrl    : 3;
  uint8_t clkm_sha_sel : 1;
  uint8_t pad_io_clkm  : 2;
  uint8_t pad_io       : 2;
} PACK RegTrxCtrl0;

typedef struct
{
  uint8_t tx_pwr         : 4;
  uint8_t reserved       : 3;
  uint8_t tx_auto_crc_on : 1;
} PACK RegPhyTxPwr;

typedef struct
{
  uint8_t rssi         : 5;
  uint8_t reserved     : 2;
  uint8_t rx_crc_valid : 1;
} PACK RegPhyRssi;

typedef struct
{
  uint8_t channel     : 5;
  uint8_t cca_mode    : 2;
  uint8_t cca_request : 1;
} PACK RegCcCcaMode;

typedef struct
{
  uint8_t dvreg_trim : 2;
  uint8_t dvdd_ok    : 1;
  uint8_t dvreg_ext  : 1;
  uint8_t avreg_trim : 2;
  uint8_t avdd_ok    : 1;
  uint8_t avreg_ext  : 1;

} PACK RegVregCtrl;

typedef struct
{
  uint8_t batmon_vth : 4;
  uint8_t batmon_hr  : 5;
  uint8_t batmon_ok  : 1;
  uint8_t reserved   : 2;
} PACK RegBatmon;

typedef struct
{
  uint8_t xtal_trim : 4;
  uint8_t xtal_mode : 4;
} PACK RegXoscCtrl;

typedef struct
{
  uint8_t ftnv       : 6;
  uint8_t ftn_round  : 1;
  uint8_t ftn_start  : 1;
} PACK RegFtnCtrl;

typedef struct
{
  uint8_t slotted_operation : 1;
  uint8_t max_csma_retries  : 3;
  uint8_t max_frame_retries : 4;
} PACK RegXahCtrl;

typedef struct
{
  uint8_t csma_seed_1 : 3;
  uint8_t i_am_coord  : 1;
  uint8_t reserved    : 2;
  uint8_t min_be      : 2;
} PACK RegCsmaSeed1;


// Constants.
enum //TRX_STATUS_REG 0x01
{
  AT86RF230_CCA_DONE = 1,
  
  AT86RF230_CCA_STATUS_IDLE = 1,
  
  AT86RF230_TRX_STATUS_P_ON                = 0,
  AT86RF230_TRX_STATUS_BUSY_RX             = 1,
  AT86RF230_TRX_STATUS_BUSY_TX             = 2,
  AT86RF230_TRX_STATUS_RX_ON               = 6,
  AT86RF230_TRX_STATUS_TRX_OFF             = 8,
  AT86RF230_TRX_STATUS_PLL_ON              = 9,
  AT86RF230_TRX_STATUS_SLEEP               = 15,
  AT86RF230_TRX_STATUS_BUSY_RX_AACK        = 17,
  AT86RF230_TRX_STATUS_BUSY_TX_ARET        = 18,
  AT86RF230_TRX_STATUS_RX_AACK_ON          = 22,
  AT86RF230_TRX_STATUS_TX_ARET_ON          = 25,
  AT86RF230_TRX_STATUS_RX_ON_NOCLK         = 28,
  AT86RF230_TRX_STATUS_RX_AACK_ON_NOCLK    = 29,
  AT86RF230_TRX_STATUS_BUSY_RX_AACK_NOCLK  = 30,
};
 
enum //TRX_STATE_REG 0x02
{  
  AT86RF230_TRAC_STATUS_SUCCESS                = 0,
  AT86RF230_TRAC_STATUS_CHANNEL_ACCESS_FAILURE = 3,
  AT86RF230_TRAC_STATUS_NO_ACK                 = 5,
  
  AT86RF230_TRX_CMD_NOP            = 0,
  AT86RF230_TRX_CMD_TX_START       = 2,
  AT86RF230_TRX_CMD_FORCE_TRX_OFF  = 3,
  AT86RF230_TRX_CMD_RX_ON          = 6,
  AT86RF230_TRX_CMD_TRX_OFF        = 8,
  AT86RF230_TRX_CMD_PLL_ON         = 9,
  AT86RF230_TRX_CMD_RX_AACK_ON     = 22,
  AT86RF230_TRX_CMD_TX_ARET_ON     = 25,
};
 
enum //TRX_CTRL_0_REG 0x03
{  
  AT86RF230_PAD_IO_2MA = 0,
  AT86RF230_PAD_IO_4MA = 1,
  AT86RF230_PAD_IO_6MA = 2,
  AT86RF230_PAD_IO_8MA = 3,

  AT86RF230_PAD_IO_CLK_2MA = 0,
  AT86RF230_PAD_IO_CLK_4MA = 1,
  AT86RF230_PAD_IO_CLK_6MA = 2,
  AT86RF230_PAD_IO_CLK_8MA = 3,

  AT86RF230_CLKM_SHA_SEL_DISABLE = 0,
  AT86RF230_CLKM_SHA_SEL_ENABLE = 1,
  
  AT86RF230_CLK_CTRL_NOCLK = 0,
  AT86RF230_CLK_CTRL_1MHZ  = 1,
  AT86RF230_CLK_CTRL_2MHZ  = 2,
  AT86RF230_CLK_CTRL_4MHZ  = 3,
  AT86RF230_CLK_CTRL_8MHZ  = 4,
  AT86RF230_CLK_CTRL_16MHZ = 5,
};
 
enum //PHY_TX_PWR_REG 0x05
{  
  AT86RF230_TX_AUTO_CRC_ON_DISABLE = 0,
  AT86RF230_TX_AUTO_CRC_ON_ENABLE = 1,
  
  AT86RF230_TX_PWR_3_0DBM       = 0,
  AT86RF230_TX_PWR_2_6DBM       = 1,
  AT86RF230_TX_PWR_2_1DBM       = 2,
  AT86RF230_TX_PWR_1_6DBM       = 3,
  AT86RF230_TX_PWR_1_1DBM       = 4,
  AT86RF230_TX_PWR_0_5DBM       = 5,
  AT86RF230_TX_PWR_MIN_0_2DBM   = 6,
  AT86RF230_TX_PWR_MIN_1_2DBM   = 7,
  AT86RF230_TX_PWR_MIN_2_2DBM   = 8,
  AT86RF230_TX_PWR_MIN_3_2DBM   = 9,
  AT86RF230_TX_PWR_MIN_4_2DBM   = 10,
  AT86RF230_TX_PWR_MIN_5_2DBM   = 11,
  AT86RF230_TX_PWR_MIN_7_2DBM   = 12,
  AT86RF230_TX_PWR_MIN_9_2DBM   = 13,
  AT86RF230_TX_PWR_MIN_12_2DBM  = 14,
  AT86RF230_TX_PWR_MIN_17_2DBM  = 15,
};
 
enum //PHY_RSSI_REG 0x06
{  
  AT86RF230_RX_CRC_VALID = 1,
};
 
  //AT86RF230_RSSI_LEVEL = ,
  
enum //PHY_CC_CCA_REG 0x08
{  
  AT86RF230_CCA_REQUEST_START = 1,
  
  AT86RF230_CCA_MODE_1 = 1,
  AT86RF230_CCA_MODE_2 = 2,
  AT86RF230_CCA_MODE_3 = 3,
};
 
enum //IRQ_MASK_REG 0x0E
{  
  AT86RF230_BAT_LOW_MASK    = (1 << 7), //0x80, IRQ7
  AT86RF230_TRX_UR_MASK     = (1 << 6), //0x40, IRQ6 
  AT86RF230_TRX_END_MASK    = (1 << 3), //0x08, IRQ3
  AT86RF230_RX_START_MASK   = (1 << 2), //0x04, IRQ2
  AT86RF230_PLL_UNLOCK_MASK = (1 << 1), //0x02, IRQ1
  AT86RF230_PLL_LOCK_MASK   = (1 << 0), //0x01, IRQ0
};
 
enum //VREG_CTRL 0x10
{  
  AT86RF230_AVREG_USE_INTERNAL = 0,
  AT86RF230_AVREG_USE_EXTERNAL = 1,
  
  AT86RF230_AVDD_OK = 1,
  
  AT86RF230_AVREG_TRIM_1_80V = 0,
  AT86RF230_AVREG_TRIM_1_75V = 1,
  AT86RF230_AVREG_TRIM_1_84V = 2,
  AT86RF230_AVREG_TRIM_1_88V = 3,
  
  AT86RF230_DVREG_USE_INTERNAL = 0,
  AT86RF230_DVREG_USE_EXTERNAL = 1,
  
  AT86RF230_DVDD_OK = 1,
  
  AT86RF230_DVREG_TRIM_1_80V = 0,
  AT86RF230_DVREG_TRIM_1_75V = 1,
  AT86RF230_DVREG_TRIM_1_84V = 2,
  AT86RF230_DVREG_TRIM_1_88V = 3,
};
 
enum //BATMON_REG 0x11
{  
  AT86RF230_BATMON_OK = 1,
  //AT86RF230_BATMON_HR = ,???????
};
  
  
  
enum //XOSC_CTRL_REG 0x12
{  
  AT86RF230_XTAL_MODE_SWITCH_OFF    = 0,
  AT86RF230_XTAL_MODE_EXTERNAL_OSC  = 4,
  AT86RF230_XTAL_MODE_INTERNAL_OSC  = 15,
  
  AT86RF230_XTAL_TRIM_0_0PF = 0,
  AT86RF230_XTAL_TRIM_0_3PF = 1,
  AT86RF230_XTAL_TRIM_0_6PF = 2,
  AT86RF230_XTAL_TRIM_0_9PF = 3,
  AT86RF230_XTAL_TRIM_1_2PF = 4,
  AT86RF230_XTAL_TRIM_1_5PF = 5,
  AT86RF230_XTAL_TRIM_1_8PF = 6,
  AT86RF230_XTAL_TRIM_2_1PF = 7,
  AT86RF230_XTAL_TRIM_2_4PF = 8,
  AT86RF230_XTAL_TRIM_2_7PF = 9,
  AT86RF230_XTAL_TRIM_3_0PF = 10,
  AT86RF230_XTAL_TRIM_3_3PF = 11,
  AT86RF230_XTAL_TRIM_3_6PF = 12,
  AT86RF230_XTAL_TRIM_3_9PF = 13,
  AT86RF230_XTAL_TRIM_4_2PF = 14,
  AT86RF230_XTAL_TRIM_4_5PF = 15,
};
 
enum //FTN_CTRL_REG 0x18
{  
  AT86RF230_FTN_START = 1,
  
  AT86RF230_FTN_ROUND_DISABLE = 0,
  AT86RF230_FTN_ROUND_ENABLE  = 1,
};
  //AT86RF230_FTNV = ,
  
  
enum //XAH_CTRL_REG 0x2C
{  
  AT86RF230_UNSLOTTED_OPERATION = 0,
  AT86RF230_SLOTTED_OPERATION   = 1,
};
 
enum //CSMA_SEED_1_REG 0x2E
{  
  AT86RF230_I_AM_COORD_DISABLE  = 0,
  AT86RF230_I_AM_COORD_ENABLE   = 1,
};

#endif

// eof at86rf230.h
