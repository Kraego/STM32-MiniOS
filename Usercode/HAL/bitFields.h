/*
 * bitFields.h
 *
 *  Created on: 25 Nov 2020
 *      Author: tkrax
 */

#ifndef BITFIELDS_H_
#define BITFIELDS_H_

typedef struct Reg32_1b_t {
	uint32_t val0 :1;
	uint32_t val1 :1;
	uint32_t val2 :1;
	uint32_t val3 :1;
	uint32_t val4 :1;
	uint32_t val5 :1;
	uint32_t val6 :1;
	uint32_t val7 :1;
	uint32_t val8 :1;
	uint32_t val9 :1;
	uint32_t val10 :1;
	uint32_t val11 :1;
	uint32_t val12 :1;
	uint32_t val13 :1;
	uint32_t val14 :1;
	uint32_t val15 :1;
	uint32_t val16 :1;
	uint32_t val17 :1;
	uint32_t val18 :1;
	uint32_t val19 :1;
	uint32_t val20 :1;
	uint32_t val21 :1;
	uint32_t val22 :1;
	uint32_t val24 :1;
	uint32_t val25 :1;
	uint32_t val26 :1;
	uint32_t val27 :1;
	uint32_t val28 :1;
	uint32_t val29 :1;
	uint32_t val30 :1;
	uint32_t val31 :1;
} Reg32_1b_t;

typedef struct Reg16_1b_t {
	uint32_t val0 :1;
	uint32_t val1 :1;
	uint32_t val2 :1;
	uint32_t val3 :1;
	uint32_t val4 :1;
	uint32_t val5 :1;
	uint32_t val6 :1;
	uint32_t val7 :1;
	uint32_t val8 :1;
	uint32_t val9 :1;
	uint32_t val10 :1;
	uint32_t val11 :1;
	uint32_t val12 :1;
	uint32_t val13 :1;
	uint32_t val14 :1;
	uint32_t val15 :1;
} Reg16_1b_t;

typedef struct Reg32_2b_t {
	uint32_t val0 :2;
	uint32_t val1 :2;
	uint32_t val2 :2;
	uint32_t val3 :2;
	uint32_t val4 :2;
	uint32_t val5 :2;
	uint32_t val6 :2;
	uint32_t val7 :2;
	uint32_t val8 :2;
	uint32_t val9 :2;
	uint32_t val10 :2;
	uint32_t val11 :2;
	uint32_t val12 :2;
	uint32_t val13 :2;
	uint32_t val14 :2;
	uint32_t val15 :2;
} Reg32_2b_t;

#endif /* BITFIELDS_H_ */
