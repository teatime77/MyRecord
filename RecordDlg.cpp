#include "stdafx.h"
//#include <afxtempl.h>
#include <windows.h>
#include <math.h>
#include "stdio.h"
#include "io.h"
#include "mmsystem.h"
// RecordDlg.cpp : implementation file
//
using namespace System;

//#include <afxtempl.h>
//#include "Record.h"
//#include "RecordDlg.h"
//#include "graph.h"
#include "mymath.h"
//#include "wave.h"
#include "Wavelet.h"

using namespace System::Collections::Generic;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::Diagnostics;

#define MY_MM_WIM_DATA	(WM_USER + 1)
#define MY_MM_WOM_DATA	(WM_USER + 2)
void IniWaveFormat(WAVEFORMATEX& fmt1, int channels, int samples, int bits);
void PlayWave(HWND hwnd1, WAVEFORMATEX& fmt1, void* data, DWORD size1);
void PrintLog(String^ msg){
	Debug::Print(msg);
}

extern TWavelet	theWL;
value class TPnt {
public:
	double	pntX;
	double	pntY;
};

ref class TGraph {
public:
	array<TPnt>	^graPnt;
	int		graCnt;

	TGraph(){
		graPnt	= nullptr;
	}

	TGraph(TPnt *dat, int cnt){
		int	i1;

		graPnt	= gcnew array<TPnt>(cnt);
		for(i1 = 0; i1 < cnt; i1++){
			graPnt[i1]	= dat[i1];
		}
		graCnt	= cnt;
	}

	~TGraph(){
		if(graPnt != nullptr){
			graPnt	= nullptr;
		}
	}
};

ref class TPic {
private:
	List<TGraph^>^ picGra;
public:
	//CWnd		*picWnd;
	//CClientDC	*picDC;
	
	Drawing::Rectangle	picRec;
	TPnt	picMin;
	TPnt	picMax;

	TPic(){
		//picWnd	= NULL;
		//picDC	= NULL;
	}

	//void initPic(CWnd *psta){
	//	picWnd	= psta;
	//	picDC	= new CClientDC(psta);

	//	picWnd->GetClientRect(&picRec);
	//}

	//TPic(CWnd *psta, double x1, double y1, double x2, double y2){
	//	picWnd	= psta;
	//	picDC	= new CClientDC(psta);

	//	picWnd->GetClientRect(&picRec);
	//	picMin.pntX	= x1;
	//	picMin.pntY	= y1;
	//	picMax.pntX	= x2;
	//	picMax.pntY	= y2;
	//}

	~TPic(){
		clearGra();

		//if(picDC == NULL) return;
		//delete picDC;
	}

	TGraph^ getGra(int n1){ return picGra[n1]; }

	void addGra(TGraph^ pgra1) { picGra->Add(pgra1); }

	void clearGra(){
		int	i1;

		for(i1 = 0; i1 < picGra->Count; i1++){
			delete picGra[i1];
		}
		picGra->Clear();
	}

	// pnt1 -> pnt2
	void getXY(TPnt& pnt1, int& x1, int& y1){
		//x1	=	int(picRec.left	  + (picRec.right  - picRec.left) * (pnt1.pntX - picMin.pntX) / (picMax.pntX - picMin.pntX));
		//y1	=	int(picRec.bottom - (picRec.bottom - picRec.top ) * (pnt1.pntY - picMin.pntY) / (picMax.pntY - picMin.pntY));
	}

	void drawLine(TPnt& pnt1, TPnt& pnt2){
		//int	x1, y1, x2, y2;

		//getXY(pnt1, x1, y1);
		//getXY(pnt2, x2, y2);
		//picDC->MoveTo(x1, y1);
		//picDC->LineTo(x2, y2);
	}

	void draw(){
		//int		n1, i1;
		//double	xmin, ymin, xmax, ymax;
		//TGraph	*pgra1;

		//if(picWnd == NULL){
		//	return;
		//}

 	//	Clear();
		//for(n1 = 0; n1 < picGra.GetSize(); n1++){
		//	pgra1	= picGra.GetAt(n1);

		//	xmin = xmax = pgra1->graPnt[0].pntX;
		//	ymin = ymax = pgra1->graPnt[0].pntY;
		//	for(i1 = 1; i1 < pgra1->graCnt; i1++){
		//		xmin	= min(xmin, pgra1->graPnt[i1].pntX);
		//		ymin	= min(ymin, pgra1->graPnt[i1].pntY);
		//		xmax	= max(xmax, pgra1->graPnt[i1].pntX);
		//		ymax	= max(ymax, pgra1->graPnt[i1].pntY);
		//	}
		//	picMin.pntX	= xmin;
		//	picMin.pntY	= ymin;
		//	picMax.pntX	= xmax;
		//	picMax.pntY	= ymax;

		//	for(i1 = 1; i1 < pgra1->graCnt; i1++){
		//		drawLine(pgra1->graPnt[i1-1], pgra1->graPnt[i1]);
		//	}
		//}
	}

	inline double trn(double d1, double min1, double max1, double min2, double max2){
		return min2 + (max2 - min2) *  (d1 - min1) / (max1 - min1);
	}

	void drawShort(short* pshort1, int cnt1, int min1, int max1){
		//int		i1, x1, x2, y1;
		//double	w1, h1, n1;

		//w1	= picRec.right  - picRec.left;
		//h1	= picRec.bottom - picRec.top;
		//n1	= max1 - (double)min1;

		//x1	= -1;
		//for(i1 = 0; i1 < cnt1; i1++){
		//	x2	= int(picRec.left + w1 * i1 / cnt1);
		//	if(x1 != x2){
		//		y1	= int(picRec.bottom - h1 * (double(pshort1[i1]) - min1) / n1);
		//		if(x1 == -1){
		//			picDC->MoveTo(x2, y1);
		//		}
		//		else{
		//			picDC->LineTo(x2, y1);
		//		}
		//		x1	= x2;
		//	}
		//}	
	}

	void drawText(int x1, int y1, LPCTSTR str1){
		//picDC->TextOut(x1, y1, str1, strlen(str1));
	}

	void Clear(){
		//if(picDC != NULL){
		//	picDC->FillSolidRect(&picRec, RGB(255,255,255));
		//}
	}
};

ref class TGlb {
public:
	static TPic^	thePic;
};
//void ttftest(CWnd* pwnd1);

typedef double (*dfnc)(double);





double integral(TFnc& fnc, double from, double to, int cnt){
	int		i1;
	double	d1, sum, x1;

	d1	= (to - from) / cnt;
	sum	= 0;
	x1	= from;
	for(i1 = 0; i1 < cnt; i1++){
		sum	+= fnc.DVal(x1);
		x1	+= d1;
	}
	return sum * d1;
}





void fourier(TFnc& fnc, double from, double to, int cnt, double va[], double vb[], int nn){
	TSinN	sin1;
	TCosN	cos1;
	TFncP	fncp1;
	TFncP	fncp2;
	int		n1;

	fncp1.fncpFnc1	= &fnc;
	fncp1.fncpFnc2	= &cos1;

	fncp2.fncpFnc1	= &fnc;
	fncp2.fncpFnc2	= &sin1;

	vb[0]	= 0;
	for(n1 = 0; n1 < nn; n1++){
		sin1.sinN	= n1;
		cos1.cosN	= n1;
		va[n1]	= (1/PI) * integral(fncp1, from, to, cnt);

		if(1 <= n1){
			vb[n1]	= (1/PI) * integral(fncp2, from, to, cnt);
		}
	}

}


double fourierS(double d1, double van[], double vbn[], int nn){
	double	sum;
	int		n1;

	sum	= van[0] / 2;
	for(n1 = 1; n1 < nn; n1++){
		sum	+= van[n1] * cos(n1 * d1) + vbn[n1] * sin(n1 * d1);
	}
	return sum;
}

//void drawGraph(TPnt vpnt1[], int cnt){
//	TGraph	^pgra1;
//
//	pgra1	= gcnew TGraph(vpnt1, cnt);
//	TGlb::thePic->addGra(pgra1);
//	thePic.draw();
//}

//void drawGraphD(CWnd *pwnd, double vd1[], int cnt){
//	TGraph	*pgra1;
//	TPnt	*vpnt1;
//	double	miny, maxy;
//	int		i1;
//
//	miny = maxy = vd1[0];
//	for(i1 = 1; i1 < cnt; i1++){
//		miny	= min(miny, vd1[i1]);
//		maxy	= max(maxy, vd1[i1]);
//	}
//
//	vpnt1	= new TPnt[cnt];
//	for(i1 = 0; i1 < cnt; i1++){
//		vpnt1[i1].pntX	= i1 / (double)cnt;
//		if(miny == maxy){
//			vpnt1[i1].pntY	= miny;
//		}
//		else{
//			vpnt1[i1].pntY	= (vd1[i1] - miny) / (maxy - miny);
//		}
//	}
//	pgra1	= new TGraph(vpnt1, cnt);
//	thePic.addGra(pgra1);
//	thePic.draw();
//	delete[] vpnt1;
//}

//void drawSin(CWnd *pwnd){
//	TPnt		vpnt1[100];
//	int			i1, cnt;
//	double		x1;
//
//	cnt	= sizeof(vpnt1)/sizeof(vpnt1[0]);
//	for(i1 = 0; i1 < cnt; i1++){
//		x1	= i1 * (2 * PI) / cnt;
//		vpnt1[i1].pntX	= x1;
//		vpnt1[i1].pntY	= sin(x1);
//	}
//	//drawGraph(pwnd, vpnt1, cnt);
//}



/////////////////////////////////////////////////////////////////////////////
// CRecordDlg message handlers

BOOL OnInitDialog(){
	
	// TODO: Add extra initialization here
	void testFFT();
	testFFT();

//	theWL.TestWavelet(2, 10);
	theWL.TestWavelet(3, 3);

	TMat<TCmp>	mat1;
	TMat<TCmp>	mat2;
	TMat<TCmp>	mat3;
	TCmp		cmp1;
	String^		str1;

	setF(mat1, 4);
	mat2.matCopy(mat1);
	conjMat(mat2);
	cmp1	= 1.0 / (double)4.0;
	matMulN(mat2, cmp1);
	matMul(mat3, mat1, mat2);

	mat3.matPrint(str1);
	//m_log.SetWindowText(str1);


	//thePic.initPic(&m_pic);


	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}




FILE	*fpRec;

char	theBuf[16][4 * 1024];
int		BufIdx, BufLen;

extern WAVEFORMATEX	WaveFmt;
extern HWAVEIN		hWI;
extern WAVEHDR		WaveHdr[2];

HWND	m_hWnd;

void MsgBox(wchar_t *msg){
	::MessageBox(NULL, msg, L"", MB_OK);
}

void OnFormLoad(void* ptr){
	m_hWnd	= (HWND)ptr;

	OnInitDialog();
}

void OnRecordBtn() 
{
	MMRESULT		res1;
	int			i1;

	if(fpRec != NULL){
		fclose(fpRec);
		fpRec	= NULL;
	}

	IniWaveFormat(WaveFmt, 1, 22050, 16);
	res1	= waveInOpen(&hWI, WAVE_MAPPER, &WaveFmt, (DWORD)m_hWnd, 0, CALLBACK_WINDOW);
	if(res1 != MMSYSERR_NOERROR){
		MsgBox(L"Can not open wave in");
		return;
	}

	memset(&WaveHdr, 0, sizeof(WaveHdr));
	for(i1 = 0; i1 < sizeof(WaveHdr)/sizeof(WaveHdr[0]); i1++){
		WaveHdr[i1].lpData			= theBuf[i1];
		WaveHdr[i1].dwBufferLength	= sizeof(theBuf[i1]);
		WaveHdr[i1].dwFlags			= 0;
		res1	= waveInPrepareHeader(hWI, &WaveHdr[i1], sizeof(WaveHdr[i1]));
		if(res1 != MMSYSERR_NOERROR){
			waveInClose(hWI);
			MsgBox(L"Can not prepare wave in");
			return;
		}

		res1	= waveInAddBuffer(hWI, &WaveHdr[i1], sizeof(WaveHdr[i1]));
		if(res1 != MMSYSERR_NOERROR){
			waveInClose(hWI);
			MsgBox(L"Can not add buffer wave in");
			return;
		}
	}

	res1	= waveInStart(hWI);
	if(res1 != MMSYSERR_NOERROR){
		MsgBox(L"Can not start wave in");
	}
}

void OnStopBtn() 
{
	MMRESULT		res1;
	int			i1;

	PrintLog(L"stop");
	res1	= waveInStop(hWI);
	if(res1 != MMSYSERR_NOERROR){
		MsgBox(L"Can not stop wave in");
	}
	//if(true) return;

	PrintLog(L"unprepare");
	for(i1 = 0; i1 < sizeof(WaveHdr)/sizeof(WaveHdr[0]); i1++){
		PrintLog( String::Format("recorded:{0}", WaveHdr[i1].dwBytesRecorded) );
		res1	= waveInUnprepareHeader(hWI, &WaveHdr[i1], sizeof(WaveHdr[i1]));
		if(res1 != MMSYSERR_NOERROR){
			MsgBox(L"Can not unprepare wave in");
		}
	}

	res1	= waveInClose(hWI);
	if(res1 != MMSYSERR_NOERROR){
		MsgBox(L"Can not close wave in");
	}
}

void CRecordDlg_drawWave(WAVEFORMATEX& fmt1, WAVEHDR& hdr1){
	//TPnt		vpnt1[100 + 2];
	//int			i1, cnt;


	//if(true) return;

	//cnt	= sizeof(vpnt1)/sizeof(vpnt1[0]);
	//for(i1 = 0; i1 < cnt; i1++){
	//	vpnt1[i1].pntX	= i1;
	//	vpnt1[i1].pntY	= ((short*)hdr1.lpData)[i1];
	//}
	//vpnt1[100].pntX	= 100;
	//vpnt1[100].pntY	= SHRT_MIN;
	//vpnt1[101].pntX	= 101;
	//vpnt1[101].pntY	= SHRT_MAX;

	//drawGraph(&m_pic, vpnt1, cnt);
}

LRESULT MyDefWindowProc2(UINT message, WPARAM wParam, LPARAM lParam) 
{
	HWAVEIN			hwi;
	HWAVEOUT		hwo;
	WAVEHDR			*phdr1;
	MMRESULT		res1;
	int				i1;

	switch(message){
	case MM_WIM_OPEN:
		PrintLog(L"wave in open");
		break;
	case MM_WIM_DATA:
		PrintLog(L"wave in data");
		hwi		= (HWAVEIN)wParam;
		phdr1	= (WAVEHDR*)lParam;

		if(phdr1->dwBytesRecorded != 0){
			if(fpRec == NULL){
				fpRec	= fopen("a.dat", "wb");
				fwrite(&WaveFmt, sizeof(WaveFmt), 1, fpRec);
			}
			fwrite(phdr1->lpData, phdr1->dwBytesRecorded, 1, fpRec);
			if(WaveFmt.nChannels == 1 && WaveFmt.wBitsPerSample == 16){
				//calcWave((short*)phdr1->lpData, phdr1->dwBytesRecorded / sizeof(short));
			}
		}
		for(i1 = 0; i1 < sizeof(WaveHdr)/sizeof(WaveHdr[0]); i1++){
			if(phdr1 == &WaveHdr[i1]){

				BufIdx	= i1;
				BufLen	= phdr1->dwBytesRecorded;

				PostMessage(m_hWnd, MY_MM_WIM_DATA, (WPARAM)i1, 0);
				break;
			}
		}
		break;
	case MY_MM_WIM_DATA:
		i1	= (int)wParam;
		waveInUnprepareHeader(hWI, &WaveHdr[i1], sizeof(WaveHdr[i1]));

		memset(&WaveHdr[i1], 0, sizeof(WaveHdr[i1]));
		WaveHdr[i1].lpData			= theBuf[i1];
		WaveHdr[i1].dwBufferLength	= sizeof(theBuf[i1]);
		WaveHdr[i1].dwFlags			= 0;
		res1	= waveInPrepareHeader(hWI, &WaveHdr[i1], sizeof(WaveHdr[i1]));
		if(res1 != MMSYSERR_NOERROR){
			waveInClose(hWI);
			PrintLog(L"Can not prepare wave in");
		}
		else{
			res1	= waveInAddBuffer(hWI, &WaveHdr[i1], sizeof(WaveHdr[i1]));
			if(res1 != MMSYSERR_NOERROR){
				waveInClose(hWI);
				PrintLog(L"Can not add buffer wave in");
			}
		}
		break;
	case MM_WIM_CLOSE:
		PrintLog(L"wave in close");
		if(fpRec != NULL){
			fclose(fpRec);
			fpRec	= NULL;
		}
	case MM_WOM_OPEN:
		PrintLog(L"wave out open");
		break;
	case MM_WOM_DONE:
		PrintLog(L"wave out open");
		PostMessage(m_hWnd, MY_MM_WOM_DATA, wParam, lParam);
		break;
	case MY_MM_WOM_DATA:
		PrintLog(L"post wave out done");
		hwo		= (HWAVEOUT)wParam;
		phdr1	= (WAVEHDR*)lParam;
		waveOutUnprepareHeader(hwo, phdr1, sizeof(*phdr1));
		waveOutClose(hwo);
		free(phdr1->lpData);
		break;
	case MM_WOM_CLOSE:
		PrintLog(L"wave out close");
		break;
	}

	return ::DefWindowProc(m_hWnd, message, wParam, lParam);
}

void MyDefWindowProc(Message msg){
	MyDefWindowProc2((UINT)msg.Msg, (WPARAM)msg.WParam.ToInt32(), (LPARAM)msg.LParam.ToInt32());
}


void OnResetBtn() 
{
	MMRESULT		res1;

	res1	= waveInReset(hWI);
	if(res1 != MMSYSERR_NOERROR){
		waveInClose(hWI);
		PrintLog(L"Can not reset wave in");
	}
	
}

void OnPlayBtn() 
{
	WAVEFORMATEX	fmt1;
	FILE		*fp1;
	long		len1;
	char		*buf1;

	if(fpRec != NULL){
		fclose(fpRec);
		fpRec	= NULL;
	}
	fp1	= fopen("a.dat", "rb");
	if(fp1 == NULL){
		MsgBox(L"Can not open wave file");
		return;
	}
	fread(&fmt1, sizeof(fmt1), 1, fp1);
	len1	= _filelength(_fileno(fp1)) - sizeof(fmt1);
	buf1	= (char*)malloc(len1);
	fread(buf1, len1, 1, fp1);
	fclose(fp1);

	PlayWave(m_hWnd, fmt1, buf1, len1);
}

void OnGraphBtn() 
{
	//TFncX_1		fncx;
	//TPnt		vpnt1[1000], vpnt2[1000];
	//double		d1, from, to;
	//int			i1, nn, npnt;
	//double		van[30], vbn[30];

	//UpdateData(true);

	//thePic.clearGra();
	//thePic.Clear();
	//
	//from	= -PI;
	//to		= PI;
	//nn		= sizeof(van)/sizeof(van[0]);
	//fourier(fncx, from, to, 100, van, vbn, nn);

	//npnt	= sizeof(vpnt1)/sizeof(vpnt1[0]);
	//for(i1 = 0; i1 < npnt; i1++){
	//	d1	= from + i1 * (to - from)/npnt;
	//	vpnt1[i1].pntX	= d1;
	//	vpnt1[i1].pntY	= fncx.DVal(d1);

	//	vpnt2[i1].pntX	= d1;
	//	vpnt2[i1].pntY	= fourierS(d1, van, vbn, nn);
	//}
	//drawGraph(&m_pic, vpnt1, npnt);
	//drawGraph(&m_pic, vpnt2, npnt);
}

void OnSinBtn() 
{
	WAVEFORMATEX	fmt1;
	short			*pshort1;
	int				size1, n1, i1;

	IniWaveFormat(fmt1, 1, 22050, 16);


	size1	= 3 * 22050 * sizeof(short);
	pshort1	= (short*)malloc(size1);
	n1	= 22050 / 200;
	for(i1 = 0; i1 < 3 * 22050; i1++){
		pshort1[i1]	= short(sin( (2 * PI/n1) * i1) * SHRT_MAX / 4);
	}
	
	PlayWave(m_hWnd, fmt1, pshort1, size1);
}

void FFTWave(short* pshort1, int cnt1){
	FILE	*fp1;
	double	*re1, *im1, *re2, *im2, *re3, *im3;
	double	d1, d2;
	int		i1, i2;

	re1	= new double[cnt1];
	im1	= new double[cnt1];
	re2	= new double[cnt1];
	im2	= new double[cnt1];
	re3	= new double[cnt1];
	im3	= new double[cnt1];

	memset(im1, 0, cnt1 * sizeof(double));
	for(i1 = 0; i1 < cnt1; i1++){
		re1[i1]	= pshort1[i1];
	}

	FFT(cnt1, re1, im1, re2, im2, true);

	// 絶対値が最大の周波数を探す
	d1	= 0;
	i2	= 0;
	for(i1 = 1; i1 < cnt1 / 2; i1++){
		d2	= fabs(re2[i1]) + fabs(im2[i1]);
		if(d1 < d2){
			d1	= d2;
			i2	= i1;
		}
	}
	for(i1 = 0; i1 < cnt1; i1++){
		if(i1 != i2){
			re2[i1]	= 0;
			im2[i1]	= 0;
		}
	}

	FFT(cnt1, re2, im2, re3, im3, false);

	fp1	= fopen("dat\\fft_wv.csv", "wt");
	fprintf(fp1, "---------- DFT \n\n");
	for(i1 = 0; i1 < cnt1; i1++){
		fprintf(fp1, "%.16f\t %.16f\n", re2[i1], im2[i1]);
	}
	fprintf(fp1, "---------- IDFT \n\n");
	for(i1 = 0; i1 < cnt1; i1++){
		fprintf(fp1, "%.16f\t %.16f\t %.16f\t %.16f\n", re1[i1], im1[i1], re3[i1], im3[i1]);
	}
	fclose(fp1);

	delete[] re1;
	delete[] im1;
	delete[] re2;
	delete[] im2;
	delete[] re3;
	delete[] im3;
}

void OnOKBtn() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	TSeq<double>	f1;
	FILE			*fp1;
	double			val1[4 * 1024];
	short			*pshort1;
	int				i1, t_i, tt;
	double			d1, d2, offset1;
	int				span1, div1, nn;
	double			van[30], vbn[30];

	fp1		= fopen("dat\\sound.csv", "wt");
	pshort1	= (short*)theBuf[BufIdx];
	for(i1 = 0; i1 < BufLen / 2; i1++){
		fprintf(fp1, "%d\n", (int)pshort1[i1]);
	}

	offset1	= 0;
	for(i1 = 0; i1 <= BufLen; i1++){
		offset1	+= pshort1[i1];
	}
	offset1	= offset1 / BufLen;

	div1	= 256;
	span1	= (2 * 1024) / (2 * div1);

	for(i1 = 0; i1 <= span1 * 2 * div1; i1++){
		val1[i1]	= pshort1[i1] - offset1;
	}
	f1.seqIni(- span1 * div1, span1 * div1, div1, val1);
	
	nn		= sizeof(van)/sizeof(van[0]);
	fourier(f1, - span1, span1, span1 * 2 * div1, van, vbn, nn);

	// an, bnの最大値を求める
	t_i	= 1;
	d1	= 0;
	for(i1 = 1; i1 < sizeof(van)/sizeof(van[0]); i1++){
		d2	= fabs(van[i1]) + fabs(vbn[i1]);
		if(d1 < d2){
			d1	= d2;
			t_i	= i1;
		}
	}
	tt	= (int)((2 * PI / t_i) * div1);

	fprintf(fp1, "--------------------\n フーリエ級数 an, bn\n\n");
	for(i1 = 0; i1 < sizeof(van)/sizeof(van[0]); i1++){
		fprintf(fp1, "%d\t %.10f\t %.10f\n", i1, van[i1], vbn[i1]);
	}


	fprintf(fp1, "--------------------\n フーリエ変換\n\n");
	for(i1 = f1.seqSt; i1 <= f1.seqEd; i1++){
		d1	= i1 / (double)f1.seqDiv;
		d2	= van[t_i] * cos(t_i * d1) + vbn[t_i] * sin(t_i * d1);
		fprintf(fp1, "%.10f\t %.10f\t %.10f\t %.10f\n", d1, f1.DVal(d1), fourierS(d1, van, vbn, nn), d2);
	}

	fclose(fp1);

	FFTWave(pshort1, BufLen / 2);

	theWL.CalcWavelet(pshort1, BufLen / 2, tt);
	
}

void calcWave(short* pshort1, int cnt1){
	String^	str1;
	double	*re1, *im1, *re2, *im2;
	double	d1, d2;
	int		i1, max_i, span1, i3, st_i;
	short	s1;

	span1	= 150;

	re1	= new double[cnt1];
	im1	= new double[cnt1];
	re2	= new double[cnt1];
	im2	= new double[cnt1];

	for(i1 = 0; i1 < cnt1; i1++){
		re1[i1]	= pshort1[i1];
	}
	memset(im1, 0, cnt1 * sizeof(double));
	FFT(cnt1, re1, im1, re2, im2, true);

	// 絶対値が最大の周波数を探す
	d1		= 0;
	max_i	= 0;
	for(i1 = 1; i1 < cnt1 / 2; i1++){
		d2	= fabs(re2[i1]) + fabs(im2[i1]);
		if(d1 < d2){
			d1		= d2;
			max_i	= i1;
		}
	}
	span1	= cnt1 / max_i;

	delete[] re1;
	delete[] im1;
	delete[] re2;
	delete[] im2;

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

	// 1周期分以降のデータをクリアする
	if(600 < span1){
		span1	= 600;
	}
	memset(pshort1 + st_i + span1, 0, (600 - span1) * sizeof(short));

	str1	= String::Format(L"間隔:{0}", span1);

	//thePic.clearGra();
	//thePic.Clear();

	//thePic.drawShort(pshort1 + st_i, 600, -600, 1200);

	//thePic.drawText(50, 50, str1);
}

