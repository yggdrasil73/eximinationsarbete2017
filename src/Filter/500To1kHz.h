/*
 * oneToThreekHz.h
 *

 */ 


#ifndef ONETOTHREEKHZ_H_
#define ONETOTHREEKHZ_H_

#define N 3
#define M 3

static int32_t xbuffsection1[M+1]={0};//Section 1
static int32_t xbuffsection2[M+1]={0};//Section 2
static int32_t xbuffsection3[M+1]={0};//Section 3

//Numerator
static int32_t bsection1[M+1] ={//Section 1
	1000,0,-1000
}; //koefficienterna * 1000
static int32_t bsection2[M+1] ={//Section 2
	1000,0,-1000
}; //koefficienterna * 1000
static int32_t bsection3[M+1] ={//Section 3
	1000,0,-1000
}; //koefficienterna * 1000

static int32_t ysection1[N+1]={0};//Section 1
static int32_t ysection2[N+1]={0};//Section 2
static int32_t ysection3[N+1]={0};//Section 3

//Denominator
static int32_t asection1[N+1]={//Section 1

	-1000,1817,-903
	
}; //koefficienterna * 1000
static int32_t asection2[N+1]={//Section 2

	-1000,1920,-946

}; //koefficienterna * 1000
static int32_t asection3[N+1]={//Section 3

	-1000,1808,-854
	
}; //koefficienterna * 1000
uint32_t fiveHToOneKhz(uint32_t invalue);


#endif /* ONETOTHREEKHZ_H_ */