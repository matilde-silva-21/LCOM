#ifndef LAB3_I8042_H
#define LAB3_I8042_H

/* I/O port addresses */

#define OUT_BUF 0x60 //output buffer
#define STATREG 0x64 //statues register

#define DELAY_US    20000
#define ESC_BREAK_CODE 0x81
#define IRQ_KBD 1
#define MAKE_BIT BIT(7)

#define PARITYERR BIT(7)
#define TIMEOUTERR BIT(6)
#define MOUSEDATA BIT(5)
#define OBF BIT(0)

#endif // LAB3_I8042_H
