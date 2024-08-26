

#define  DEBUG_ENABLE_TX
//#define  DEBUG_ENABLE_RX
//#define  DEBUG_ENABLE_DBG

#define  DEBUG_TX_DDR     DDRD
#define  DEBUG_TX_PORT    PORTD
#define  DEBUG_TX_PIN     PIND
#define  DEBUG_TX_bp      1U


/*Must edit DEBUG_RX_XXXX definition*/
#define  DEBUG_RX_DDRD
#define  DEBUG_RX_DDR     DDRD
#define  DEBUG_RX_PORT    PORTD
#define  DEBUG_RX_PIN     PIND
#define  DEBUG_RX_bp      5U


/*Only for debugging purpose*/
#define  DEBUG_TEST_DDR   DDRD
#define  DEBUG_TEST_PORT  PORTD
#define  DEBUG_TEST_PIN   PIND
#define  DEBUG_TEST_bp    1U


#define  DEBUG_BAUD_RATE_9600


//#define  DEBUG_USE_DELAY
#define  DEBUG_USE_TIMER0
//#define  DEBUG_USE_TIMER1
//#define  DEBUG_USE_TIMER2

#define  DEBUG_RX_BUF_SIZE 32


void     Debug_Struct_Init(void);
void     Debug_Timings_Init(void);
void     Debug_Timer_Start(void);
void     Debug_Timer_Stop(void);
void     Debug_GPIO_Init(void);
void     Debug_Tx_Pin_High(void);
void     Debug_Tx_Pin_Low(void);
void     Debug_Tx_Pin_Set_State(uint8_t pin_state);
uint8_t  Debug_Rx_Pin_Get_State(void);

void     Debug_Delay(uint16_t val);
void     Debug_Delay_Tx_Bits(void);
void     Debug_Delay_Rx_Bits(void);
void     Debug_Delay_Rx_Half_Bit(void);


void     Debug_Tx_Byte(uint8_t val);
uint8_t  Debug_Rx_Byte(void);

/*
uint8_t  debug_rx_byte_get(void);
void     debug_rx_byte_clear(void);
void     debug_rx_byte_set(uint8_t val);
*/


/////////For Text & Commons////////
void     Debug_Tx_NL(void);
void     Debug_Tx_SP(void);
void     Debug_Tx_CM(void);
void     Debug_Tx_Text(char *str);
void     Debug_Tx_Text_NL(char *str);
void     Debug_Tx_Text_SP(char *str);
void     Debug_Tx_Text_CM(char *str);


/////////For Decimal Numbers////////

void     Debug_Determine_Digit_Numbers(uint32_t num);
void     Debug_Tx_Number_Digits(void);
void     Debug_Tx_Number(int32_t num);
void     Debug_Tx_Number_NL(int32_t num);
void     Debug_Tx_Number_SP(int32_t num);
void     Debug_Tx_Number_CM(int32_t num);
void     Debug_Tx_Parameter_NL(char *name, int32_t num);
void     Debug_Tx_Parameter_SP(char *name, int32_t num);
void     Debug_Tx_Parameter_CM(char *name, int32_t num);


/////////For Hexadecimal Numbers/////
void     Debug_Tx_Hex(uint32_t val);
void     Debug_Tx_Number_Hex_NL(int32_t num);
void     Debug_Tx_Number_Hex_SP(int32_t num);
void     Debug_Tx_Number_Hex_CM(int32_t num);
void     Debug_Tx_Parameter_Hex_NL(char *name, int32_t num);
void     Debug_Tx_Parameter_Hex_SP(char *name, int32_t num);
void     Debug_Tx_Parameter_Hex_CM(char *name, int32_t num);


/////////For Binary Numbers///////
void     Debug_Tx_Bin(uint32_t val);
void     Debug_Tx_Number_Bin_NL(int32_t num);
void     Debug_Tx_Number_Bin_SP(int32_t num);
void     Debug_Tx_Number_Bin_CM(int32_t num);
void     Debug_Tx_Parameter_Bin_NL(char *name, int32_t num);
void     Debug_Tx_Parameter_Bin_SP(char *name, int32_t num);
void     Debug_Tx_Parameter_Bin_CM(char *name, int32_t num);

/*
uint8_t  debug_buf_get(uint8_t index);
uint8_t  debug_buf_index_get(void);
void     debug_flush_buf(void);
uint8_t  debug_databsy_get(void);
uint8_t  debug_data_available_rx_buf(void);
uint8_t  debug_available_bytes_rx_buf(void);
*/

void     Debug_Init(void);