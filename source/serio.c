#include <stdint.h>
#include <stddef.h>
#include <serio.h>

UART_INFO uarts[UART_COUNT];

extern void UartHardwareInit(uint8_t uart_num, uint32_t baud_rate, uint16_t stop_bits, uint16_t parity);
extern void UartStartTx(uint8_t uart_num);

UART_INFO * UartInit(uint8_t uart_num, uint32_t baud_rate, uint16_t stop_bits, uint16_t parity)
{
    UART_INFO * uart;
    uint16_t result;

    if (uart_num > UART_COUNT - 1) {
        return NULL;
    }

    uart = &uarts[uart_num];

    uart->baud_rate = baud_rate;
    uart->parity = parity;
    uart->stop_bits = stop_bits;
    uart->uart_num = uart_num;

    result = UartReset(uart);
    if (result != 0) {
        return NULL;
    }

    /* Initialize UART hardware registers */
    UartHardwareInit(uart_num, baud_rate, stop_bits, parity);
    return uart;

}

UART_INFO * UartGetInfo(uint8_t uart_num)
{
    if (uart_num > UART_COUNT - 1) {
        return NULL;
    }
    return &uarts[uart_num];
}

uint16_t UartReset(UART_INFO * uart)
{
    if (uart != NULL) {
        /* Reset buffers and pointers*/
        uart->tx_buffer_info.count = 0;
        uart->tx_buffer_info.size = SERIO_BUFFER_SIZE;
        uart->tx_buffer_info.in = 0;
        uart->tx_buffer_info.out = 0;
        uart->rx_buffer_info.count = 0;
        uart->rx_buffer_info.size = SERIO_BUFFER_SIZE;
        uart->rx_buffer_info.in = 0;
        uart->rx_buffer_info.out = 0;
        return 0;
    }
    return -1;
}

uint16_t UartWriteData(UART_INFO * uart, uint8_t * data, uint8_t length)
{
    uint16_t write_count;

    if (uart == NULL) {
        return 0;
    }

    if (data == NULL) {
        return 0;
    }

    write_count = 0;
    while (length--) {
        uart->tx_buffer[uart->tx_buffer_info.in++] = *data++;
        if (uart->tx_buffer_info.in > uart->tx_buffer_info.size) {
            uart->tx_buffer_info.in = 0;
        }
        uart->tx_buffer_info.count++;
        write_count++;
        if (uart->tx_buffer_info.count == uart->tx_buffer_info.size) {
            break;
        }
    }
    if (uart->tx_buffer_info.count != 0) {
        /* Start tx */
        UartStartTx(uart->uart_num);
    }
    return write_count;
}
uint16_t UartReadData(UART_INFO * uart, uint8_t * dest, uint8_t length);

uint16_t UartRxDataAvailable(UART_INFO * uart)
{
    if (uart == NULL) {
        return 0;
    }
    return uart->rx_buffer_info.count;
}

uint8_t UartTxBufferIsEmpty(UART_INFO * uart)
{
    if (uart == NULL) {
        return 0;
    }
    return uart->tx_buffer_info.count;
}

uint8_t UartTxBufferGetByte(UART_INFO * uart)
{
    uint8_t c;

    if (uart == NULL) {
        return 0;
    }
    c = uart->tx_buffer[uart->tx_buffer_info.out++];
    if (uart->tx_buffer_info.out == uart->tx_buffer_info.size) {
        uart->tx_buffer_info.out = 0;
    }
    if (uart->tx_buffer_info.count > 0) {
        uart->tx_buffer_info.count--;
    }
    return c;
}

uint8_t UartRxBufferIsFull(UART_INFO * uart)
{
    if (uart == NULL) {
        return 0;
    }
    return uart->rx_buffer_info.size - uart->rx_buffer_info.count;
}

uint8_t UartRxBufferPutByte(UART_INFO * uart, uint8_t c)
{
    if (uart == NULL) {
        return 0;
    }
    uart->rx_buffer[uart->rx_buffer_info.in] = c;
    uart->rx_buffer_info.in++;
    if (uart->rx_buffer_info.in == uart->rx_buffer_info.size) {
        uart->rx_buffer_info.in = 0;
    }
    uart->rx_buffer_info.count++;
    return 1;
}


