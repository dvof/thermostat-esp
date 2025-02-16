#ifndef WEMOS_D1_MINI_PINS_H
#define WEMOS_D1_MINI_PINS_H

// UART
#define TX      TX   // TXD (Hardware Serial)
#define RX      RX   // RXD (Hardware Serial)

// Analog Input
#define A0      A0   // Analog input (Max 3.2V)

// Digital I/O
#define D0      16   // GPIO16
#define D1      5    // GPIO5 (SCL)
#define D2      4    // GPIO4 (SDA)
#define D3      0    // GPIO0 (10k Pull-up)
#define D4      2    // GPIO2 (10k Pull-up, BUILTIN_LED)
#define D5      14   // GPIO14 (SCK)
#define D6      12   // GPIO12 (MISO)
#define D7      13   // GPIO13 (MOSI)
#define D8      15   // GPIO15 (10k Pull-down, SS)

// Power & Reset
#define GND     GND  // Ground
#define VCC     3.3  // 3.3V
#define RST     RST  // Reset

#endif // WEMOS_D1_MINI_PINS_H
