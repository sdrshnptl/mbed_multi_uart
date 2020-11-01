#include "mbed.h"
#include <cstdint>
#include <cstdio>
#include <cstring>

Ticker timer_ms;
uint16_t ovf_cnt_1ms = 0;
uint16_t ovf_cnt1_1ms = 0;
uint8_t tx_buff_len = 0;
uint8_t tx_buff[255];

DigitalOut led(LED1);
AnalogIn ain(A5);

BufferedSerial serial_port(USBTX, USBRX);

BufferedSerial serial1_port(A0, A1);

void timer_1ms() {
  // led = !led;
  ovf_cnt_1ms++;
  ovf_cnt1_1ms++;
}

// main() runs in its own thread in the OS
int main() {
  serial_port.set_baud(9600);
  serial1_port.set_baud(9600);
  timer_ms.attach(timer_1ms, 1ms);
  tx_buff_len = sprintf((char *)tx_buff, "\r\nOK:%s\r\n", __TIMESTAMP__);
  serial_port.write(tx_buff, tx_buff_len);

  while (true) {
    if (ovf_cnt_1ms > 999) {
      ovf_cnt_1ms = 0;
      led = !led;
      tx_buff_len = sprintf((char *)tx_buff, "LED: %s \r\n", (led) ? "ON" : "OFF");
      serial_port.write(tx_buff, tx_buff_len);
    }

    if (ovf_cnt1_1ms > 499) {
      ovf_cnt1_1ms = 0;
      tx_buff_len = sprintf((char *)tx_buff, "Ain: %d \r\n", (uint8_t)ain.read_u16());
      serial1_port.write(tx_buff, tx_buff_len);
    }
  }
}
