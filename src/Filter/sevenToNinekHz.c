/*
 * sevenToNinekHz.c
 *
 * Created: 2017-05-09 12:43:35
 *  Author: Yggdrasil
 */ 
 #include <asf.h>
 #include "Filter/sevenToNinekHz.h"

 #define N 3
 #define M 3

 static int32_t xbuffsection1[M+1]={0};//Section 1
 static int32_t xbuffsection2[M+1]={0};//Section 2
 static int32_t xbuffsection3[M+1]={0};//Section 3

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

 static int32_t asection1[N+1]={//Section 1

	 -1000,-1999,-999
	 
 }; //koefficienterna * 1000
 static int32_t asection2[N+1]={//Section 2

	 -1000,-1045,-477
	 
 }; //koefficienterna * 1000
 static int32_t asection3[N+1]={//Section 3

	 -1000,-1414,-414
	 
 }; //koefficienterna * 1000
 /*-------------------------------------------------*/

//7500-9999 Hz
uint32_t sevenToNinekHz(uint32_t invalue){
	int32_t sum1,sum2,sum3 = 0;
	int32_t sumy1,sumy2,sumy3= 0;
	float totsumsection1,totsumsection2,totsumsection3 = 0;
	int32_t outvaluesection1, outvaluesection2,outvaluesection3;
	
	//SECTION 1
	int i;
	for(i =M; i>0; i--){
		xbuffsection1[i]=xbuffsection1[i-1];
	}
	xbuffsection1[0]=(int)invalue;
	
	int j;
	for( j =0; j<=M;j++){
		sum1 += ((xbuffsection1[j]*bsection1[j]));
	}
	for(  j=1; j<=N;j++){
		sumy1 += ((ysection1[j]*asection1[j]));
	}
	
	for(i = N; i>1;i--){
		ysection1[i]=ysection1[i-1];
	}

	totsumsection1= (((sum1/1000)+(sumy1/1000)));
	ysection1[1] = totsumsection1;
	outvaluesection1 = totsumsection1;
	//SECTION 2
	int l;
	for(l =M; l>0; l--){
		xbuffsection2[l]=xbuffsection2[l-1];
	}
	xbuffsection2[0]=(int)outvaluesection1;
	int m;
	for( m =0; m<=M;m++){
		sum2 += ((xbuffsection2[m]*bsection2[m]));
	}
	for(  m=1; m<=N;m++){
		sumy2 += ((ysection2[m]*asection2[m]));
	}
	
	for(l = N; l>1;l--){
		ysection2[l]=ysection2[l-1];
	}
	totsumsection2= (((sum2/1000)+(sumy2/1000)));
	ysection2[1] = totsumsection2;
	outvaluesection2 = totsumsection2;

	//SECTION 3
	int o;
	for(o =M; o>0; o--){
		xbuffsection3[o]=xbuffsection3[o-1];
	}
	xbuffsection3[0]=(int)outvaluesection2;
	int p;
	for( p =0; p<=M;p++){
		sum3 += ((xbuffsection3[p]*bsection3[p]));
	}
	for(  p=1; p<=N;p++){
		sumy3 += ((ysection3[p]*asection3[p]));
	}
	
	for(o = M; o>1;o--){
		ysection3[o]=ysection3[o-1];
	}
	totsumsection3= (((sum3/1000)+(sumy3/1000))*0.10);
	ysection3[1] = totsumsection3;
	outvaluesection3 = totsumsection3;
	return outvaluesection3;
}