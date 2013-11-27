#ifndef SERIO_H
#define SERIO_H

#define SERIO_BUFFER_SIZE 64

enum {
    UART_1,
    UART_2,
    UART_3,
    UART_4,
    UART_COUNT
};

enum {
    SERIO_NOPARITY,
    SERIO_ODD,
    SERIO_EVEN
};

enum {
    SERIO_1STOPBIT,
    SERIO_2STOPBIT
};

typedef struct uart_buffer_info_s {
    uint16_t size;
    uint16_t count;
    uint16_t in;
    uint16_t out;
} UART_BUFFER_INFO;

typedef struct uart_s {
    uint8_t tx_buffer[SERIO_BUFFER_SIZE];
    uint8_t rx_buffer[SERIO_BUFFER_SIZE];
    UART_BUFFER_INFO tx_buffer_info;
    UART_BUFFER_INFO rx_buffer_info;
    uint32_t baud_rate;
    uint16_t stop_bits;
    uint16_t parity;
    uint16_t uart_num;
} UART_INFO;

extern UART_INFO * UartInit(uint8_t uart_num, uint32_t baud_rate,
                            uint16_t stop_bits, uint16_t parity);
extern UART_INFO * UartGetInfo(uint8_t uart_num);
extern uint16_t UartReset(UART_INFO * uart);
extern uint16_t UartWriteData(UART_INFO * uart, uint8_t * data, uint8_t length);
extern uint16_t UartReadData(UART_INFO * uart, uint8_t * dest, uint8_t length);
extern uint16_t UartRxDataAvailable(UART_INFO * uart);

#endif
