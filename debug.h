

#define  DEBUG_TX_DDR     DDRD
#define  DEBUG_TX_PORT    PORTD
#define  DEBUG_TX_PIN     PIND
#define  DEBUG_TX_bp      1U


/*Must edit DEBUG_RX_XXXX definition*/
#define  DEBUG_RX_DDRD
#define  DEBUG_RX_DDR     DDRD
#define  DEBUG_RX_PORT    PORTD
#define  DEBUG_RX_PIN     PIND
#define  DEBUG_RX_bp      0U


/*Only for Debugging purpose*/
//#define  DEBUG_GPIO_SWITCH_DEBUGGING
#define  DEBUG_TEST_DDR   DDRD
#define  DEBUG_TEST_PORT  PORTD
#define  DEBUG_TEST_PIN   PIND
#define  DEBUG_TEST_bp    1U


#define  DEBUG_BAUD_RATE_38400


//#define  DEBUG_USE_DELAY
//#define  DEBUG_USE_TIMER0
//#define  DEBUG_USE_TIMER1
#define  DEBUG_USE_TIMER2

#define  DEBUG_RX_BUF_SIZE 32


void     Debug_Struct_Init(void);
void     Debug_Timings_Init(void);
void     Debug_Timer_Start(void);
void     Debug_Timer_Stop(void);
void     Debug_GPIO_Init(void);
void     Debug_Tx_High(void);
void     Debug_Tx_Low(void);
void     Debug_Tx_Set(uint8_t pin_state);
uint8_t  Debug_rx_Get(void);

void     Debug_Delay(uint16_t val);
void     Debug_Delay_index_0(void);
void     Debug_Delay_index_1(void);
void     Debug_Delay_index_2(void);

void     Debug_Tx_Byte(uint8_t val);
uint8_t  Debug_rx_Byte(void);
uint8_t  Debug_rx_Byte_Get(void);
void     Debug_rx_Byte_clear(void);
void     Debug_rx_Byte_Set(uint8_t val);

void     Debug_Tx_Hex(uint32_t val);
void     Debug_Tx_Bin(uint32_t val);

void     Debug_Tx_NL(void);
void     Debug_Tx_SP(void);
void     Debug_Tx_CM(void);

void     Debug_Tx_Text(char *str);
void     Debug_Tx_Text_NL(char *str);
void     Debug_Tx_Text_SP(char *str);
void     Debug_Tx_Text_CM(char *str);

void     Debug_Determine_Digit_Numbers(uint32_t num);
void     Debug_Tx_Number_Digits(void);

void     Debug_Tx_Number(int32_t num);
void     Debug_Tx_Number_NL(int32_t num);
void     Debug_Tx_Number_SP(int32_t num);
void     Debug_Tx_Number_CM(int32_t num);

void     Debug_Tx_Number_Hex_NL(int32_t num);
void     Debug_Tx_Number_Hex_SP(int32_t num);
void     Debug_Tx_Number_Hex_CM(int32_t num);

void     Debug_Tx_Parameter_NL(char *name, int32_t num);
void     Debug_Tx_Parameter_SP(char *name, int32_t num);
void     Debug_Tx_Parameter_CM(char *name, int32_t num);

void     Debug_Tx_Parameter_Hex_NL(char *name, int32_t num);
void     Debug_Tx_Parameter_Hex_SP(char *name, int32_t num);
void     Debug_Tx_Parameter_Hex_CM(char *name, int32_t num);

uint8_t  Debug_Buf_Get(uint8_t index);
uint8_t  Debug_Buf_Index_Get(void);
void     Debug_Flush_Buf(void);
uint8_t  Debug_Data_Busy_Get(void);
uint8_t  Debug_Rx_Buf_Data_Available(void);
uint8_t  Debug_Rx_Buf_Available_Bytes(void);

void     Debug_Init(void);