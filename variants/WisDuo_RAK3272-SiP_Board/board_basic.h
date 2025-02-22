#ifndef __BOARD_BASIC_H__
#define __BOARD_BASIC_H__

#include <stdbool.h>
#include <stdint.h>
#include "pin_define.h"
#ifdef SUPPORT_LORA
#include "RegionNvm.h"
#include "service_lora.h"
#include "service_lora_multicast.h"
#endif
#include "service_mode.h"
#include "udrv_serial.h"

#define DEFAULT_SERIAL_CONSOLE		SERIAL_UART2

#define DEFAULT_SERIAL_UART0_MODE       SERVICE_MODE_TYPE_CUSTOM
#define DEFAULT_SERIAL_UART1_MODE       SERVICE_MODE_TYPE_CUSTOM
#define DEFAULT_SERIAL_UART2_MODE       SERVICE_MODE_TYPE_CLI

#define DEFAULT_SERIAL_BAUDRATE		115200

#define DEFAULT_SPIMST_PORT         UDRV_SPIMST_0

#ifdef SUPPORT_LORA
typedef struct _lorap2p_param{
    /*!
     * Frequency in Hz
     */
    uint32_t Frequency;
    /*!
     * Spreading factor
     * [6, 7, 8, 9, 10, 11, 12]
     */
    uint8_t  Spreadfact;	
    /*!
     * Bandwidth
     * [0: 125 kHz, 1: 250 kHz, 2: 500 kHz]                              
     */
    uint16_t  Bandwidth; 
    /*!
     * Coding Rate
     * [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
     */
    uint8_t  Codingrate; 
    /*!
     * Preamble Length
     * [5-65535]
     */
    uint16_t  Preamlen; 
    /*!
     * Power dbm
     * [5-20]
     */
    uint8_t  Powerdbm;	
    /*!
     * p2p workmode
     * [1-receiver 2-sender]
     */
    uint8_t  p2p_workmode;
    bool crypt_enable;
    uint8_t crypt_key[8];
    /* FSK 600 b/s up 300 kb/s */
    uint32_t bitrate;
    /* 2*Fdev + BR) < BW */
    uint32_t deviation;
    uint32_t fsk_rxbw;
}S_LORAP2P_PARAM;

typedef struct {
    SERVICE_LORA_BAND region;
    uint8_t dev_eui[8];
    uint8_t app_eui[8];
    uint8_t app_key[16];
    uint8_t app_skey[16];
    uint8_t dev_addr[4];
    uint8_t nwk_id[4];
    uint8_t nwk_skey[16];
    uint32_t multi_dev_addr;
    uint8_t multi_nwks_key[16];
    uint8_t multi_apps_key[16];
    bool MulticastEnable;
#if defined( REGION_CN470 ) || defined( REGION_US915 ) || \
    defined( REGION_AU915 )
    uint16_t ch_mask[REGION_NVM_CHANNELS_MASK_SIZE];
#endif
    SERVICE_LORA_JOIN_MODE join_mode;
    SERVICE_LORA_CLASS device_class;
    uint8_t confirm;
    uint8_t retry;
    SERVICE_LORA_DATA_RATE dr;
    SERVICE_LORA_DATA_RATE rx2dr;
    bool adr;
    uint8_t tx_power;
    uint8_t DutycycleEnable;
    uint32_t jn1dl;
    uint32_t jn2dl;
    uint32_t rx1dl;
    uint32_t rx2dl;
    uint32_t rx2fq;
    bool pub_nwk_mode;
    uint8_t linkcheck_mode;
    uint8_t ping_slot_periodicity;
    bool join_start;
    bool auto_join;
    uint32_t auto_join_period;
    uint32_t auto_join_max_cnt;
    McSession_t McSession_group[4];
    uint32_t chs;
    uint8_t tp_port[SERIAL_MAX];
} lora_cfg_t;
#endif

typedef struct {
        uint8_t server_ip[20];
        uint8_t server_port[20];    	
        uint8_t operator_long_data[20]; 
        uint8_t operator_short_data[20]; 
        uint8_t operator_apn_data[20];
        uint8_t operator_net_data[20];
        uint8_t hologram_card_num[20];
} cellular_cfg_t;


typedef struct{
    uint8_t work_mode;  // 0:ble peripheral  1:ble central  2:ble observer 
    uint8_t long_range_enable;
    uint8_t reserve[14];
}ble_central_cfg_t;

typedef struct{
    uint32_t seconds;
    uint32_t subseconds;
}rtc_delta_t;

typedef struct {
    uint32_t crc_verify;
#ifdef SUPPORT_LORA
    uint8_t lora_work_mode;
    S_LORAP2P_PARAM g_lora_p2p_cfg_t;
#endif
#ifdef SUPPORT_BLE
    ble_central_cfg_t g_ble_cfg_t;
#endif
#ifdef SUPPORT_LORA
    lora_cfg_t g_lora_cfg_t;
#endif
    rtc_delta_t g_rtc_delta_t;
    SERVICE_MODE_TYPE mode_type[SERIAL_MAX];
    SERIAL_WLOCK_STATE serial_lock_status[SERIAL_MAX];
    uint32_t baudrate;
    uint8_t atcmd_echo;
    uint8_t serial_passwd[9];
    uint32_t auto_sleep_time;
    uint8_t sn[18];
    uint8_t alias[16];
} rui_cfg_t;

uint32_t get_batt_table_size(void);
#endif
