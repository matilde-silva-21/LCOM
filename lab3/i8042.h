#ifndef LAB3_I8042_H
#define LAB3_I8042_H

/* I/O port addresses */

#define OUT_BUF 0x60 //output buffer, leitura de valores de retorno
#define STATREG 0x64 //status register
#define IN_ARGS 0x60 //para receber argumentos
#define IN_BUF 0x64 //para receber comandos

#define DELAY_US    20000
#define ESC_BREAK_CODE 0x81
#define IRQ_KBD 1
#define MAKE_BIT BIT(7)

#define PARITYERR BIT(7)
#define TIMEOUTERR BIT(6)
#define MOUSEDATA BIT(5)
#define OBF BIT(0)
#define IBF BIT(1)

#define READ_CMBYTE 0x20
#define WRITE_CMBYTE 0x60

#define ENABLE_INT_KBD BIT(0)

#endif // LAB3_I8042_H
