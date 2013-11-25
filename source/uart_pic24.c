#include <stdint.h>
#define USE_AND_OR
#include <p24Fxxxx.h>
#include <uart.h>
#include <HardwareConfig.h>
#include <serio.h>

extern uint8_t UartTxBufferIsEmpty(UART_INFO * uart);
extern uint8_t UartTxBufferGetByte(UART_INFO * uart);
extern uint8_t UartRxBufferIsFull(UART_INFO * uart);
extern uint8_t UartRxBufferPutByte(UART_INFO * uart, uint8_t c);

static void Uart1Init(uint32_t baud_rate, uint16_t stop_bits, uint16_t parity);
static void Uart2Init(uint32_t baud_rate, uint16_t stop_bits, uint16_t parity);
static void Uart3Init(uint32_t baud_rate, uint16_t stop_bits, uint16_t parity);
static void Uart4Init(uint32_t baud_rate, uint16_t stop_bits, uint16_t parity);

static uint16_t get_brg(uint32_t baud_rate, BOOL is_high_speed);
static uint16_t get_parity(uint16_t parity);
static uint16_t get_stopbit(uint16_t stop_bits);

void UartHardwareInit(uint8_t uart_num, uint32_t baud_rate, uint16_t stop_bits, uint16_t parity)
{
    switch (uart_num) {
    case 0: Uart1Init(baud_rate, stop_bits, parity);
        break;
    case 1: Uart2Init(baud_rate, stop_bits, parity);
        break;
    case 2: Uart3Init(baud_rate, stop_bits, parity);
        break;
    case 3: Uart4Init(baud_rate, stop_bits, parity);
        break;
    }
}

void UartStartTx(uint8_t uart_num)
{
    switch(uart_num){
    case 0: IFS0bits.U1TXIF = 1; break;
    case 1: IFS1bits.U2TXIF = 1; break;
    case 2: IFS5bits.U3TXIF = 1; break;
    case 3: IFS5bits.U4TXIF = 1; break;
    }
}

void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void)
{
    UART_INFO * uart;
    U1TX_Clear_Intr_Status_Bit;
    uart = UartGetInfo(UART_1);
    while(U1STAbits.UTXBF == 0 && UartTxBufferIsEmpty(uart) > 0){
        U1TXREG = UartTxBufferGetByte(uart);
    }
}

void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void)
{
    UART_INFO * uart;
    U1RX_Clear_Intr_Status_Bit;
    uart = UartGetInfo(UART_1);
    while(U1STAbits.URXDA == 1 && UartRxBufferIsFull(uart) > 0){
        UartRxBufferPutByte(uart,U1RXREG);
    }
}

void __attribute__((interrupt, no_auto_psv)) _U2TXInterrupt(void)
{
    UART_INFO * uart;
    U2TX_Clear_Intr_Status_Bit;
    uart = UartGetInfo(UART_2);
    while(U2STAbits.UTXBF == 0 && UartTxBufferIsEmpty(uart) > 0){
        U2TXREG = UartTxBufferGetByte(uart);
    }
}

void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void)
{
    UART_INFO * uart;
    U2RX_Clear_Intr_Status_Bit;
    uart = UartGetInfo(UART_2);
    while(U2STAbits.URXDA == 1 && UartRxBufferIsFull(uart) > 0){
        UartRxBufferPutByte(uart,U2RXREG);
    }
}

void __attribute__((interrupt, no_auto_psv)) _U3TXInterrupt(void)
{
    UART_INFO * uart;
    U3TX_Clear_Intr_Status_Bit;
    uart = UartGetInfo(UART_3);
    while(U3STAbits.UTXBF == 0 && UartTxBufferIsEmpty(uart) > 0){
        U3TXREG = UartTxBufferGetByte(uart);
    }
}

void __attribute__((interrupt, no_auto_psv)) _U3RXInterrupt(void)
{
    UART_INFO * uart;
    U3RX_Clear_Intr_Status_Bit;
    uart = UartGetInfo(UART_3);
    while(U3STAbits.URXDA == 1 && UartRxBufferIsFull(uart) > 0){
        UartRxBufferPutByte(uart,U3RXREG);
    }
}

void __attribute__((interrupt, no_auto_psv)) _U4TXInterrupt(void)
{
    UART_INFO * uart;
    U4TX_Clear_Intr_Status_Bit;
    uart = UartGetInfo(UART_4);
    while(U4STAbits.UTXBF == 0 && UartTxBufferIsEmpty(uart) > 0){
        U4TXREG = UartTxBufferGetByte(uart);
    }
}

void __attribute__((interrupt, no_auto_psv)) _U4RXInterrupt(void)
{
    UART_INFO * uart;
    U4RX_Clear_Intr_Status_Bit;
    uart = UartGetInfo(UART_4);
    while(U4STAbits.URXDA == 1 && UartRxBufferIsFull(uart) > 0){
        UartRxBufferPutByte(uart,U4RXREG);
    }
}

void Uart1Init(uint32_t baud_rate, uint16_t stop_bits, uint16_t parity)
{
    uint16_t parity_config;
    uint16_t stopbit_config;
    uint16_t brg_config;
    uint16_t brg_value;

    /* default brgh to high speed */
    brg_config = UART_BRGH_FOUR;
    parity_config = get_parity(parity);
    stopbit_config = get_stopbit(stop_bits);
    brg_value = get_brg(baud_rate, brg_config);

    CloseUART1();
    ConfigIntUART1(UART_RX_INT_EN | UART_RX_INT_PR6 | UART_TX_INT_EN | UART_TX_INT_PR6);
    OpenUART1(UART_EN | parity_config | stopbit_config | brg_config, UART_TX_ENABLE, brg_value);
}

void Uart2Init(uint32_t baud_rate, uint16_t stop_bits, uint16_t parity)
{
    uint16_t parity_config;
    uint16_t stopbit_config;
    uint16_t brg_config;
    uint16_t brg_value;

    /* default brgh to high speed */
    brg_config = UART_BRGH_FOUR;
    parity_config = get_parity(parity);
    stopbit_config = get_stopbit(stop_bits);
    brg_value = get_brg(baud_rate, brg_config);

    CloseUART2();
    ConfigIntUART2(UART_RX_INT_EN | UART_RX_INT_PR6 | UART_TX_INT_EN | UART_TX_INT_PR6);
    OpenUART2(UART_EN | parity_config | stopbit_config | brg_config, UART_TX_ENABLE, brg_value);
}

void Uart3Init(uint32_t baud_rate, uint16_t stop_bits, uint16_t parity)
{
    uint16_t parity_config;
    uint16_t stopbit_config;
    uint16_t brg_config;
    uint16_t brg_value;

    /* default brgh to high speed */
    brg_config = UART_BRGH_FOUR;
    parity_config = get_parity(parity);
    stopbit_config = get_stopbit(stop_bits);
    brg_value = get_brg(baud_rate, brg_config);

    CloseUART3();
    ConfigIntUART3(UART_RX_INT_EN | UART_RX_INT_PR6 | UART_TX_INT_EN | UART_TX_INT_PR6);
    OpenUART3(UART_EN | parity_config | stopbit_config | brg_config, UART_TX_ENABLE, brg_value);
}

void Uart4Init(uint32_t baud_rate, uint16_t stop_bits, uint16_t parity)
{
    uint16_t parity_config;
    uint16_t stopbit_config;
    uint16_t brg_config;
    uint16_t brg_value;

    /* default brgh to high speed */
    brg_config = UART_BRGH_FOUR;
    parity_config = get_parity(parity);
    stopbit_config = get_stopbit(stop_bits);
    brg_value = get_brg(baud_rate, brg_config);

    CloseUART4();
    ConfigIntUART4(UART_RX_INT_EN | UART_RX_INT_PR6 | UART_TX_INT_EN | UART_TX_INT_PR6);
    OpenUART4(UART_EN | parity_config | stopbit_config | brg_config, UART_TX_ENABLE, brg_value);
}

uint16_t get_brg(uint32_t baud_rate, BOOL is_high_speed)
{
    uint32_t div;
    uint32_t brg;

    switch (is_high_speed) {
    case 0: div = 16UL;
        break;
    case 1: div = 4UL;
        break;
    default: div = 16UL;
        break;
    }

    brg = INSTRUCTION_CLOCK / div;
    brg = brg / baud_rate;
    brg = brg - 1UL;

    return (uint16_t) brg;
}

uint16_t get_parity(uint16_t parity)
{
    switch (parity) {
    case SERIO_NOPARITY: return UART_NO_PAR_8BIT;
    case SERIO_ODD: return UART_ODD_PAR_8BIT;
    case SERIO_EVEN: return UART_EVEN_PAR_8BIT;
    }
    return 0;
}

uint16_t get_stopbit(uint16_t stop_bits)
{
    switch (stop_bits) {
    case SERIO_1STOPBIT: return UART_1STOPBIT;
    case SERIO_2STOPBIT: return UART_2STOPBITS;
    }
    return 0;
}
