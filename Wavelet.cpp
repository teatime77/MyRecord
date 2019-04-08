#include "stdafx.h"
//#include <afxtempl.h>
#include <Windows.h>
//#include <Windef.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

using namespace System;

#include "mymath.h"

#include "Daubechies.h"
#include "Scaling.h"
#include "Wavelet.h"


#define	zero_i	0

/*

*/
double PkDB3[]	= { 
//	 0.4704672078      ,  1.141116916       , 0.6503650005      , 
	 0.4704672077841635,  1.141116915831443 , 0.6503650005262322, 
//  -0.1909344156      , -0.1208322083      , 0.04981749974 };
	-0.1909344155683274, -0.1208322083103962, 0.04981749973688368
};

//double Phi3[]	= { 0, 1.28634, -0.385837, 0.0952675, 0.00423435, 0 };

double* PkDB[]	= { 
	NULL, NULL, Daubechies_2, Daubechies_3, Daubechies_4, Daubechies_5,	
	Daubechies_6, Daubechies_7, Daubechies_8, Daubechies_9, Daubechies_10 
};

double* Phi[]	= { NULL, NULL, Phi_2, Phi_3, Phi_4, Phi_5, Phi_6, Phi_7, Phi_8, Phi_9, Phi_10 };


TWavelet	theWL;

double InP(TSeq<double>& f1, TSeq<double>& g1){
	int		st1, ed1, i1;
	double	sum1;

	st1	= max(f1.seqSt, g1.seqSt);
	ed1	= min(f1.seqEd, g1.seqEd);

	sum1	= 0;
	for(i1 = st1; i1 <= ed1; i1++){
		sum1	+= f1.seqVal[i1 - f1.seqSt] * g1.seqVal[i1 - g1.seqSt];
	}
	return sum1 / f1.seqDiv;
}

#define	SEQ_SUM(s1, s2, e1, dd)							\
	{													\
		int		ii, _ix;									\
														\
		dd	= 0;										\
		for(_ix = s1.seqSt; _ix <= s1.seqEd; _ix++){		\
			ii	= e1;									\
			if(s2.seqSt <= ii && ii <= s2.seqEd){		\
				dd	+= s1.seqAt(_ix) * s2.seqAt(ii);	\
			}											\
		}												\
	}
//				dd	+= s1.seqVal[_ix] * s2.seqVal[ii];	


/*
	Ckを求める
*/
void calcCk(TSeq<double>& f1, TSeq<double>& phi1, TSeq<double>& Ck){
	TSeq<double>	phi2;
	int				i1, st1, ed1;

	// 左端の位置
	i1	= f1.seqSt - phi1.seqEd;
	st1	= i1 / phi1.seqDiv;
	if(i1 % phi1.seqDiv < 0){
		st1--;
	}

	// 右端の位置
	i1	= f1.seqEd - phi1.seqSt;
	ed1	= i1 / phi1.seqDiv;
	if(0 < i1 % phi1.seqDiv){
		ed1++;
	}

	Ck.seqIni(st1, ed1, 1, NULL);

	phi2.seqIni(phi1.seqSt, phi1.seqEd, phi1.seqDiv, phi1.seqVal);
	for(i1 = st1 ; i1 <= ed1; i1++){

		phi2.seqSt	= phi1.seqSt + i1 * phi2.seqDiv;
		phi2.seqEd	= phi1.seqEd + i1 * phi2.seqDiv;

		Ck.seqSet(i1, InP(f1, phi2));
	}
}

/*
	線形結合
*/
void LinearCo(TSeq<double>& f1, TSeq<double>& phi1, TSeq<double>& Ck){
	TSeq<double>	phi2;
	int				i1, i2, st1, ed1;
	double			d1, c1;

	st1	= phi1.seqSt + Ck.seqSt * phi1.seqDiv;
	ed1	= phi1.seqEd + Ck.seqEd * phi1.seqDiv;
	f1.seqIni(st1, ed1, phi1.seqDiv, NULL);

	phi2.seqIni(phi1.seqSt, phi1.seqEd, phi1.seqDiv, phi1.seqVal);
	for(i1 = Ck.seqSt ; i1 <= Ck.seqEd; i1++){

		c1			= Ck.seqAt(i1);
		phi2.seqSt	= phi1.seqSt + i1 * phi2.seqDiv;
		phi2.seqEd	= phi1.seqEd + i1 * phi2.seqDiv;
		for(i2 = phi2.seqSt; i2 <= phi2.seqEd; i2++){
			d1	= f1.seqAt(i2) + c1 * phi2.seqAt(i2);
			f1.seqSet(i2, d1);
		}
	}
}

/*
	PkからQk, Gk, Hkを求める

	st <= 1 - k <= ed
	st <= - k <= ed
*/
void calcQGH(TSeq<double>& pk1, TSeq<double>& qk1, TSeq<double>& gk1, TSeq<double>& hk1){
	int	k;
	
	qk1.seqIni(1 - pk1.seqEd, 1 - pk1.seqSt, 1, NULL);
	for(k = qk1.seqSt; k <= qk1.seqEd; k++){
		qk1.seqSet(k, (k % 2 == 0 ? 1 : -1) * pk1.seqAt(1 - k));
	}

	gk1.seqIni(- pk1.seqEd, - pk1.seqSt, 1, NULL);
	for(k = gk1.seqSt; k <= gk1.seqEd; k++){
		gk1.seqSet(k, pk1.seqAt(- k));
	}

	hk1.seqIni(- qk1.seqEd, - qk1.seqSt, 1, NULL);
	for(k = hk1.seqSt; k <= hk1.seqEd; k++){
		hk1.seqSet(k, qk1.seqAt(- k));
	}
}

/*
	ψをφとQkから計算する

	ψ(x) = ΣQk * φ(2 * x - k)

	st_phi <= 2 * x - k <= ed_phi
	(st_phi + k) / 2 <= x <= (ed_phi + k) / 2
	(st_phi + st_Qk) / 2 <= x <= (ed_phi + ed_Qk) / 2
*/
void calcPsi(TSeq<double>& phi1, TSeq<double>& Qk, TSeq<double>& psi1){
	int		st1, ed1, i1;
	double	d1;

	st1	= (int)floor((phi1.seqSt + Qk.seqSt * phi1.seqDiv) / 2.0);
	ed1	= (int)ceil ((phi1.seqEd + Qk.seqEd * phi1.seqDiv) / 2.0);

	psi1.seqIni(st1, ed1, phi1.seqDiv, NULL);

	for(i1 = st1; i1 <= ed1; i1++){
		SEQ_SUM(Qk, phi1, 2 * i1 - _ix * phi1.seqDiv, d1);
		psi1.seqSet(i1, d1);
	}
}

/*
	分解

	C(j-1,k) = 1/2 * ΣG(2 * k - m) * C(j, m)
	D(j-1,k) = 1/2 * ΣH(2 * k - m) * C(j, m)

	st_G <= 2 * k - m <= ed_G
	(st_G + m) / 2 <= k <= (ed_G + m) / 2
	(st_G + st_C(j)) / 2 <= k <= (ed_G + ed_C(j)) / 2

	st_H <= 2 * k - m <= ed_H
	(st_H + m) / 2 <= k <= (ed_H + m) / 2
	(st_H + st_C(j)) / 2 <= k <= (ed_H + ed_C(j)) / 2
*/
void Decompo(TSeq<double>& C1, TSeq<double>& Gk, TSeq<double>& Hk, TSeq<double>& C2, TSeq<double>& D2){
	int		st1, ed1, k1;
	double	d1;

	// C(j)からC(j-1)を計算する
	st1	= (int)floor((Gk.seqSt + C1.seqSt) / 2.0);
	ed1	= (int)ceil ((Gk.seqEd + C1.seqEd) / 2.0);
	C2.seqIni(st1, ed1, 1, NULL);
	for(k1 = st1; k1 <= ed1; k1++){

		SEQ_SUM(C1, Gk, 2 * k1 - _ix, d1);
		C2.seqSet(k1, d1 / 2.0);
	}

	// C(j)からD(j-1)を計算する
	st1	= (int)floor((Hk.seqSt + C1.seqSt) / 2.0);
	ed1	= (int)ceil ((Hk.seqEd + C1.seqEd) / 2.0);
	D2.seqIni(st1, ed1, 1, NULL);
	for(k1 = st1; k1 <= ed1; k1++){
		SEQ_SUM(C1, Hk, 2 * k1 - _ix, d1);
		D2.seqSet(k1, d1 / 2.0);
	}
}

/*
	再構成

	C(j,k)	= Σ(P(k - 2 * m) * C(j-1,m) + Q(k - 2 * m) * D(j-1,m))

	st_P <= k - 2 * m <= ed_P
	st_P + 2 * m <= k <= ed_P + 2 * m	
	st_P + 2 * st_C <= k <= ed_P + 2 * ed_C


*/
void Recons(TSeq<double>& C1, TSeq<double>& Pk, TSeq<double>& Qk, TSeq<double>& C2, TSeq<double>& D2){
	int		st1, ed1, st2, ed2, k1;
	double	d1;

	st1	= Pk.seqSt + 2 * C2.seqSt;
	ed1	= Pk.seqEd + 2 * C2.seqEd;
	st2	= Qk.seqSt + 2 * D2.seqSt;
	ed2	= Qk.seqEd + 2 * D2.seqEd;
	C1.seqIni(min(st1,st2), max(ed1,ed2), 1, NULL);

	// C(j-1)からC(j)を計算する
	for(k1 = st1; k1 <= ed1; k1++){

		SEQ_SUM(C2, Pk, k1 - 2 * _ix, d1);
		C1.seqSet(k1, d1);
	}

	// D(j-1)からC(j)を計算する
	for(k1 = st2; k1 <= ed2; k1++){

		SEQ_SUM(D2, Qk, k1 - 2 * _ix, d1);
		C1.seqSet(k1, C1.seqAt(k1) + d1);
	}
}

/*
	f(j, x)	= Σ C(j, k) * φ((2 ^ j) * x - k) = Σ C(j, k) * φ(j, x - k/(2 ^ j))

	st_phi <= x - k / (2 ^ j) <= ed_phi
	st_phi + k    / (2 ^ j) <= x <= ed_phi + k    / (2 ^ j)
	st_phi + st_C / (2 ^ j) <= x <= ed_phi + ed_C / (2 ^ j)

*/
void DecompoFG(int j, TSeq<double>& phi, TSeq<double>& psi, TSeq<double>& C, TSeq<double>& D, TSeq<double>& f, TSeq<double>& g){
	int		st1, ed1, x1, j_2;
	double	d1;

	j_2	= (1 << j);

	// fを計算する
	st1	= phi.seqSt + C.seqSt * j_2 * phi.seqDiv;
	ed1	= phi.seqEd + C.seqEd * j_2 * phi.seqDiv;
	f.seqIni(st1, ed1, phi.seqDiv, NULL);

	for(x1 = st1; x1 <= ed1; x1++){
		SEQ_SUM(C, phi, x1 - _ix * j_2 * phi.seqDiv, d1);
		f.seqSet(x1, d1);
	}

	// gを計算する
	st1	= psi.seqSt + D.seqSt * j_2 * psi.seqDiv;
	ed1	= psi.seqEd + D.seqEd * j_2 * psi.seqDiv;
	g.seqIni(st1, ed1, psi.seqDiv, NULL);

	for(x1 = st1; x1 <= ed1; x1++){
		SEQ_SUM(D, psi, x1 - _ix * j_2 * psi.seqDiv, d1);
		g.seqSet(x1, d1);
	}
}

void dmpSeq(FILE* fp1, int n1, int st1, int ed1, TSeq<double>* vpseq[]){
	int	i1, x1;

	if(st1 == 0 && ed1 == 0){
		st1	= INT_MAX;
		ed1	= INT_MIN;
		for(i1 = 0; i1 < n1; i1++){
			st1	= min(st1, vpseq[i1]->seqSt);
			ed1	= max(ed1, vpseq[i1]->seqEd);
		}
	}

	for(x1 = st1; x1 <= ed1; x1++){
		fprintf(fp1, "%d", x1);
		for(i1 = 0; i1 < n1; i1++){
			if(vpseq[i1]->seqSt <= x1 && x1 <= vpseq[i1]->seqEd){
				fprintf(fp1, "\t%.10f", vpseq[i1]->seqAt(x1));
			}
			else{
				fprintf(fp1, "\t0");
			}
		}
		fprintf(fp1, "\n");
	}
}

void dmpSeqArray(FILE* fp1, int n1, TSeq<double>* vseq){
	TSeq<double>** vpseq;
	int	i1;

	vpseq	= new TSeq<double>*[n1];
	for(i1 = 0; i1 < n1; i1++){
		vpseq[i1]	= &vseq[i1];
	}
	dmpSeq(fp1, n1, 0, 0, vpseq);

	delete[] vpseq;
}

void TWavelet::TestWavelet(int dbc_from, int dbc_to){
	TSeq<double>	vg[WL_LVL];
	TSeq<double>	vf[WL_LVL];
	TSeq<double>	phi1;
	TSeq<double>	phi_pre;
	TSeq<double>	f0;
	TSeq<double>	f0_2;
	TSeq<double>	f0_sv;
	TSeq<double>	f1;
	TSeq<double>	Ck;
	TSeq<double>	Ck_2;
	TSeq<double>	C2;
	TSeq<double>	D2;
	TSeq<double>	ak;
	TSeq<double>	dif;
	TSeq<double>*	vpseq[100];
	FILE			*fp1;
	char			path1[MAX_PATH];
	int				dbc1, k1, k2, i1, i2, j, x1;
	double			d1;
	int				st_phi, ed_phi;

	for(dbc1 = dbc_from; dbc1 <= dbc_to; dbc1++){

		printf("Daubechies %d\n", dbc1);

		st_phi	= 0;
		ed_phi	= dbc1 * 2 - 1;

		Pk.seqIni(0, dbc1 * 2 - 1, 1, PkDB[dbc1]);

		sprintf(path1, "dat\\dbc_%d.csv", dbc1);
		fp1	= fopen(path1, "wt");
		
		// PkからQk, Gk, Hkを求める
		calcQGH(Pk, Qk, Gk, Hk);

		// φの補間
		for(i1 = 1; i1 < 5; i1++){
			if(i1 == 1){
				// 最初の場合

				phi_pre.seqIni(st_phi, ed_phi, 1, Phi[dbc1]);
			}
			else{
				// 最初でない場合

				phi_pre.seqCopy(phi1);
			}

			fprintf(fp1, "--------------------\n phi %d \n\n", i1);

			phi1.seqIni(phi_pre.seqSt * 2, phi_pre.seqEd * 2, 1 << i1, NULL);
			for(k1 = phi1.seqSt; k1 <= phi1.seqEd; k1++){

				SEQ_SUM(Pk, phi_pre, k1 - phi_pre.seqDiv *  _ix, d1);
				phi1.seqVal[k1]	= d1;

				fprintf(fp1, "%.10f\n", d1);
			}
		}
		vPhi[zero_i].seqCopy(phi1);

		// ψをφとQkから計算する
		fprintf(fp1, "--------------------\nψ\n\n");
		calcPsi(vPhi[zero_i], Qk, vPsi[zero_i]);
		for(i2 = vPsi[zero_i].seqSt; i2 <= vPsi[zero_i].seqEd; i2++){
			fprintf(fp1, "%d \t %.10f\n", i2, vPsi[zero_i].seqAt(i2));
		}

		//φjとψjを作る
		for(i1 = 1; i1 < WL_LVL; i1++){
			
			// φjを2倍の定義域で初期化する
			vPhi[i1].seqIni(vPhi[i1 - 1].seqSt * 2, vPhi[i1 - 1].seqEd * 2, vPhi[i1 - 1].seqDiv, NULL);

			for(k1 = vPhi[i1 - 1].seqSt; k1 <= vPhi[i1 - 1].seqEd; k1++){
				k2	= k1 * 2;
				vPhi[i1].seqSet(k2, vPhi[i1 - 1].seqAt(k1));
				if(k1 < vPhi[i1 - 1].seqEd){
					vPhi[i1].seqSet(k2 + 1, (vPhi[i1 - 1].seqAt(k1) + vPhi[i1 - 1].seqAt(k1 + 1)) / 2.0);
				}
			}
			vPhi[i1].seqTrim();

			// ψjを2倍の定義域で初期化する
			vPsi[i1].seqIni(vPsi[i1 - 1].seqSt * 2, vPsi[i1 - 1].seqEd * 2, vPsi[i1 - 1].seqDiv, NULL);

			for(k1 = vPsi[i1 - 1].seqSt; k1 <= vPsi[i1 - 1].seqEd; k1++){
				k2	= k1 * 2;
				vPsi[i1].seqSet(k2, vPsi[i1 - 1].seqAt(k1));
				if(k1 < vPsi[i1 - 1].seqEd){
					vPsi[i1].seqSet(k2 + 1, (vPsi[i1 - 1].seqAt(k1) + vPsi[i1 - 1].seqAt(k1 + 1)) / 2.0);
				}
			}
			vPsi[i1].seqTrim();
		}
		fprintf(fp1, "--------------------\n φj\n\n");
		dmpSeqArray(fp1, 5, vPhi);

		fprintf(fp1, "--------------------\n ψj\n\n");
		dmpSeqArray(fp1, 5, vPsi);

		// φのトゥー・スケール関係の確認
		fprintf(fp1, "--------------------\nφのトゥー・スケール関係\n\n");
		for(k1 = vPhi[zero_i].seqSt; k1 <= vPhi[zero_i].seqEd; k1++){

			SEQ_SUM(Pk, vPhi[zero_i], 2 * k1 - _ix * vPhi[zero_i].seqDiv, d1);
			fprintf(fp1, "%.10f \t %.10f\n", vPhi[zero_i].seqVal[k1], vPhi[zero_i].seqVal[k1] - d1);
		}

		// φの正規直交基底の確認
		fprintf(fp1, "--------------------\nφ正規直交基底\n\n");
		calcCk(vPhi[zero_i], vPhi[zero_i], ak);
		for(i2 = ak.seqSt; i2 <= ak.seqEd; i2++){
			fprintf(fp1, "φ \t %d \t %.10f\n", i2, ak.seqAt(i2));
		}

		// ψの正規直交基底の確認
		fprintf(fp1, "--------------------\nψ正規直交基底\n\n");
		calcCk(vPsi[zero_i], vPsi[zero_i], ak);
		for(i2 = ak.seqSt; i2 <= ak.seqEd; i2++){
			fprintf(fp1, "ψ \t %d \t %.10f\n", i2, ak.seqAt(i2));
		}

		// テストデータを作る
		fprintf(fp1, "--------------------\n f0\n\n");
		f0.seqIni(-10 * vPhi[zero_i].seqDiv, 10 * vPhi[zero_i].seqDiv, vPhi[zero_i].seqDiv, NULL);
		for(i2 = f0.seqSt; i2 <= f0.seqEd; i2++){
			d1	= sin(2 * PI / (f0.seqEd - f0.seqSt) * i2  );
			f0.seqSet(i2, d1);
			fprintf(fp1, "%d \t %.10f\n", i2, d1);
		}
		f0.seqTrim();

		// Ckを計算する
		fprintf(fp1, "--------------------\n calkCk\n\n");
		calcCk(f0, vPhi[zero_i], Ck);
		for(i2 = Ck.seqSt; i2 <= Ck.seqEd; i2++){
			fprintf(fp1, "%d \t %.10f\n", i2, Ck.seqAt(i2));
		}

		// Ckの線形結合を計算する
		fprintf(fp1, "--------------------\n Ckの線形結合\n\n");
		LinearCo(f0_2, vPhi[zero_i], Ck);
		for(i2 = f0_2.seqSt; i2 <= f0_2.seqEd; i2++){
			fprintf(fp1, "%d \t %.10f\n", i2, f0_2.seqAt(i2));
		}

		// 分解
		Decompo(Ck, Gk, Hk, C2, D2);
		fprintf(fp1, "--------------------\n C2\n\n");
		for(i2 = C2.seqSt; i2 <= C2.seqEd; i2++){
			fprintf(fp1, "%d \t %.10f\n", i2, C2.seqAt(i2));
		}
		fprintf(fp1, "--------------------\n D2\n\n");
		for(i2 = D2.seqSt; i2 <= D2.seqEd; i2++){
			fprintf(fp1, "%d \t %.10f\n", i2, D2.seqAt(i2));
		}

		// 再構成
		Recons(Ck_2, Pk, Qk, C2, D2);
		fprintf(fp1, "--------------------\n Ck_2\n\n");
		for(i2 = Ck_2.seqSt; i2 <= Ck_2.seqEd; i2++){
			fprintf(fp1, "%d \t %.10f\n", i2, Ck_2.seqAt(i2));
		}

		// f0 = f1 + g1
		f0_sv.seqCopy(f0);
		vpseq[0]	= &f0_sv;
		for(j = 1; j < WL_LVL; j++){
			if(1 < j){
				Ck.seqCopy(C2);
				f0.seqCopy(f1);
			}

			// 分解
			Decompo(Ck, Gk, Hk, C2, D2);

			// 再構成
			Recons(Ck_2, Pk, Qk, C2, D2);
			fprintf(fp1, "--------------------\n 再構成の検証\n\n");
			for(i2 = Ck.seqSt; i2 <= Ck.seqEd; i2++){
				fprintf(fp1, "%d \t %.10f\n", i2, Ck.seqAt(i2) - Ck_2.seqAt(i2));
			}

			// f0 = f1 + g1
			DecompoFG(j, vPhi[j], vPsi[j], C2, D2, f1, vg[j]);
			f1.seqTrim();
			vg[j].seqTrim();

			vf[j].seqCopy(f1);

			vpseq[j]	= &vg[j];
		}
		vpseq[WL_LVL]	= &f1;

		fprintf(fp1, "--------------------\n f0 = f1 + g1\n\n");
		dmpSeq(fp1, WL_LVL+1, f0_sv.seqSt, f0_sv.seqEd, vpseq);


		fprintf(fp1, "--------------------\n fi-1 = fi + gi\n\n");
		// f0 - (g1 + ... + gn + fn)
//		dif.seqIni(f0_sv.seqSt, f0_sv.seqEd, f0_sv.seqDiv, NULL);
		vf[0].seqCopy(f0_sv);
		for(x1 = f0_sv.seqSt; x1 <= f0_sv.seqEd; x1++){
			fprintf(fp1, "%d\t %.10f", x1, f0_sv.seqAt(x1));
			d1	= 0;
			for(i1 = 1; i1 < WL_LVL; i1++){
				fprintf(fp1, "\t %.10f", vf[i1-1].seqAt(x1) - (vf[i1].seqAt(x1) + vg[i1].seqAt(x1)));
				d1	+= vg[i1].seqAt(x1);
			}
			d1	+= vf[WL_LVL - 1].seqAt(x1);
			fprintf(fp1, "\t %.10f\n", d1);
		}

		/*
			φ(n)	= φ0(n) = φ1(2*n) = φ2(4*n) = φ3(8*n)
			φ(n/2) = φ1(n) = φ2(2*n) = φ3(4*n)
			φ(n/4) = φ2(n) = φ3(2*n)
			φ(n/8) = φ3(n) 
		*/


		fclose(fp1);
	}
}


void TWavelet::CalcWavelet(short* pshort1, int cnt1, int tt){
	TSeq<double>	Ck;
	TSeq<double>	C2;
	TSeq<double>	D2;
	TSeq<double>	vg[WL_LVL];
	TSeq<double>	vf[WL_LVL];
	TSeq<double>	f0;
	TSeq<double>	f0_sv;
	TSeq<double>	f1;
	TSeq<double>	dif;
	TSeq<double>*	vpseq[100];
	short			vsh1[4 * 1024];
	FILE			*fp1;
	int				j, i1, i2, i3, x1, st_i, ed_i;
	short			s1;
	double			d1, d2;

	fp1	= fopen("dat\\sound_wv.csv", "wt");

	// 中央付近で最小値を探す
	i3	= cnt1 / 2 + 300;
	s1	= SHRT_MAX;
	st_i	= 0;
	for(i1 = cnt1 / 2 - 300; i1 <= i3; i1++){
		if(pshort1[i1] < s1){
			s1		= pshort1[i1];
			st_i	= i1;
		}
	}

	// st_i + 150±100で最小値を探す
	i3	= st_i + 150 + 100;
	s1	= SHRT_MAX;
	ed_i	= 0;
	for(i1 = st_i + 150 - 100; i1 <= i3; i1++){
		if(pshort1[i1] < s1){
			s1		= pshort1[i1];
			ed_i	= i1;
		}
	}

	ed_i	= st_i + min(tt, 300);

	for(i1 = st_i; i1 <= ed_i; i1++){
		d1	= pshort1[i1];
		d2	= (pshort1[i1+1] - pshort1[i1]) / 4;
		for(i2 = 0; i2 < 4; i2++){
			vsh1[(i1 - st_i) * 4 + i2]	= (short)(d1 + d2 * i2);
		}
	}
	f0.seqIniShort(0, (ed_i - st_i) * 4, 16, vsh1);
/*
	f0.seqIni(0, 320, vPhi[zero_i].seqDiv, NULL);
	for(i2 = f0.seqSt; i2 <= f0.seqEd; i2++){
		d2	= 2 * PI / (f0.seqEd - f0.seqSt) * i2;
		d1	= sin(d2*5);
		f0.seqSet(i2, d1);
	}
	f0.seqTrim();
*/

	// Ckを計算する
	fprintf(fp1, "--------------------\n calkCk\n\n");
	calcCk(f0, vPhi[zero_i], Ck);

	// f0 = f1 + g1
	f0_sv.seqCopy(f0);
	vpseq[0]	= &f0_sv;
	for(j = 1; j < WL_LVL; j++){
		if(1 < j){
			Ck.seqCopy(C2);
			f0.seqCopy(f1);
		}

		// 分解
		Decompo(Ck, Gk, Hk, C2, D2);

		// f0 = f1 + g1
		DecompoFG(j, vPhi[j], vPsi[j], C2, D2, f1, vg[j]);
		f1.seqTrim();
		vg[j].seqTrim();

		vf[j].seqCopy(f1);

		vpseq[j]	= &vg[j];
	}
	vpseq[WL_LVL]	= &f1;

	dif.seqIni(f0_sv.seqSt, f0_sv.seqEd, f0_sv.seqDiv, NULL);
	for(x1 = dif.seqSt; x1 <= dif.seqEd; x1++){
		d1	= 0;
		for(i1 = 1; i1 < WL_LVL; i1++){
			d1	+= vg[i1].seqAt(x1);
		}
		d1	+= vf[WL_LVL - 1].seqAt(x1);
		dif.seqSet(x1, f0_sv.seqAt(x1) - d1);
	}
	vpseq[WL_LVL+1]	= &dif;

	fprintf(fp1, "--------------------\n f0 = f1 + g1\n\n");
	dmpSeq(fp1, WL_LVL+1+1, f0_sv.seqSt, f0_sv.seqEd, vpseq);

	fclose(fp1);
}
