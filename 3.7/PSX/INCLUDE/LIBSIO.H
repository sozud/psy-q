/* $PSLibId: Run-time Library Release 3.7$ */
#ifndef _LIBSIO_H_
#define _LIBSIO_H_

/*
 *  Copyright (C) 1996 Sony Computer Entertainment Inc. All Rights Reserved
 *  libsio.h: Sio Interface
 */

/* status bits */
#define SIO_CTS		0x100
#define SIO_DSR		0x80
#define SIO_FE		0x20
#define SIO_OE		0x10
#define SIO_PERROR	0x8
#define SIO_TXU		0x4
#define SIO_RXRDY	0x2
#define SIO_TXRDY	0x1


/* control bits */
#define SIO_BIT_DTR	0x1
#define SIO_BIT_RTS	0x2

/*
 * Prototypes
 */

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif
extern long AddSIO(int);
extern long DelSIO(void);
extern long _comb_control(unsigned long,unsigned long,unsigned long);
#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif
#endif /* _LIBSIO_H_ */
