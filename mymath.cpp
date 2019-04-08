#include "stdafx.h"
//#include <afxtempl.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

using namespace System;

#include "mymath.h"



void setF(TMat<TCmp>& mat1, int dim1){
	int	r1, c1;

	mat1.matInit(dim1, dim1);
	for(r1 = 0; r1 < dim1; r1++){
		for(c1 = 0; c1 < dim1; c1++){
			mat1.matAt(r1, c1)	= omega(dim1, r1 * c1);
		}
	}
}

TCmp	theI(0, 1);

TCmp::TCmp(double re1, double im1){
	cmpR	= re1;
	cmpI	= im1;
}

TCmp& TCmp::operator=(double d1){
	cmpR	= d1;
	cmpI	= 0;

    return *this;
}

TCmp& TCmp::operator+=(TCmp& cmp1){
	cmpR	+= cmp1.cmpR;
	cmpI	+= cmp1.cmpI;
	return *this;
}

TCmp& TCmp::operator*=(TCmp& cmp1){
	*this	= (*this) * cmp1;
	return *this;
}

TCmp operator *(TCmp& cmp1, TCmp& cmp2){
	TCmp	cmp3;

	cmp3.cmpR	= cmp1.cmpR * cmp2.cmpR - cmp1.cmpI * cmp2.cmpI;
	cmp3.cmpI	= cmp1.cmpR * cmp2.cmpI + cmp1.cmpI * cmp2.cmpR;
	return cmp3;
}

void Print(String^% str1, TCmp& cmp1){
	str1	+= String::Format(L"({0:0.5}, {1:0.5}) ", cmp1.cmpR, cmp1.cmpI);
}

TCmp c_exp(TCmp& cmp2){
	TCmp	cmp1;
	double	r1;

	r1	= exp(cmp2.cmpR);
	cmp1.cmpR	= r1 * cos(cmp2.cmpI);
	cmp1.cmpI	= r1 * sin(cmp2.cmpI);
	return cmp1;
};

TCmp conj(TCmp& cmp2){
	TCmp	cmp1;

	cmp1.cmpR	= cmp2.cmpR;
	cmp1.cmpI	= - cmp2.cmpI;
	return cmp1;
};

void conjMat(TMat<TCmp>& mat1){
	int	i1, i2;

	for(i1 = 0; i1 < mat1.matRow; i1++){
		for(i2 = 0; i2 < mat1.matCol; i2++){
			mat1.matAt(i1, i2)	= conj(mat1.matAt(i1, i2));
		}
	}
}


TCmp omega(int n1, int k1){
	TCmp	cmp1;
	double	t1;

	t1	= 2 * PI / n1 * k1;
	cmp1.cmpR	= cos(t1);
	cmp1.cmpI	= sin(t1);
	return cmp1;
};

void setZero(TCmp& cmp1){ 
	cmp1.cmpR	= 0; 
	cmp1.cmpI	= 0; 
}



void FFT(int N, double re1[], double im1[], double Re[], double Im[], bool is_dft){
	int		I, J, K, M, Mm, N1, N2;
	double	*Co, *Si;
	double	Phi, Fakt, Temp1, Temp2, Rew, Imw;

	memcpy(Re, re1, N * sizeof(double));
	memcpy(Im, im1, N * sizeof(double));

	N1	= N - 1;
	N2	= N / 2;

	Phi		= PI / N2;
	Fakt	= 1.0 / N;

	if(! is_dft){
		Phi		= - Phi;
		Fakt	= 1;
	}

	Co	= new double[N2];
	Si	= new double[N2];
	for(I = 0; I <= N2 - 1; I++){
		Co[I]	=   cos(I * Phi);
		Si[I]	= - sin(I * Phi);
	}

	for(I = 0; I <= N1; I++){
		Re[I]	*= Fakt;
		Im[I]	*= Fakt;
	}

	// ビット順序逆転
	J	= 0;
	for(I = 1; I <= N1; I++){
		M	= N2;
		while(J >= M){
			J	= J - M;
			M	= M / 2;
		}
		J	= J + M;
		if(J > I){
			Temp1	= Re[I];
			Re[I]	= Re[J];
			Re[J]	= Temp1;

			Temp1	= Im[I];
			Im[I]	= Im[J];
			Im[J]	= Temp1;
		}
	}

	// バタフライ演算
	Mm	= 1;
	do{
		M	= Mm;
		Mm	= M + M;
		for(K = 0; K <= M - 1; K++){
			Rew	= Co[K * N2];
			Imw	= Si[K * N2];
			for(I = K; I <= N1; I += Mm){
				J	= I + M;
				Temp1	= Rew * Re[J] - Imw * Im[J];
				Temp2	= Imw * Re[J] + Rew * Im[J];
				Re[J]	= Re[I] - Temp1;
				Im[J]	= Im[I] - Temp2;
				Re[I]	= Re[I] + Temp1;
				Im[I]	= Im[I] + Temp2;
			}
		}
		N2	= N2 / 2;

	}while (Mm != N);

	delete[] Co;
	delete[] Si;
}

void testFFT(){
	double	re1[64], im1[64];
	double	re2[64], im2[64];
	double	re3[64], im3[64];
	FILE	*fp1;
	int		i1;

	for(i1 = 0; i1 < 64; i1++){
		re1[i1]	= sin(2 * PI / 64 * i1 * 2) + sin(2 * PI / 64 * i1 * 3) + sin(2 * PI / 64 * i1 * 4);
		im1[i1]	= 0;
	}

	FFT(64, re1, im1, re2, im2, true);
	FFT(64, re2, im2, re3, im3, false);

	fp1	= fopen("dat\\fft.csv", "wt");
	fprintf(fp1, "\"---------- DFT\" \n\n");
	for(i1 = 0; i1 < 64; i1++){
		fprintf(fp1, "%.16f, %.16f\n", re2[i1], im2[i1]);
	}
	fprintf(fp1, "\"---------- IDFT\" \n\n");
	for(i1 = 0; i1 < 64; i1++){
		fprintf(fp1, "%.16f, %.16f, %.16f, %.16f\n", re1[i1], im1[i1], re3[i1], im3[i1]);
	}
	fclose(fp1);
}