#include "stdafx.h"
//#include <afxtempl.h>
#include <windows.h>
#include <math.h>
#include "io.h"
#include "mmsystem.h"
//#include "graph.h"

HWAVEIN			hWI;
WAVEHDR			WaveHdr[2];
HWAVEOUT		hWaveOut = NULL;
WAVEFORMATEX	WaveFmt;

void IniWaveFormat(WAVEFORMATEX& fmt1, int channels, int samples, int bits){
	memset(&fmt1, 0, sizeof(fmt1));

	fmt1.wFormatTag			= WAVE_FORMAT_PCM;
	fmt1.nChannels			= channels;
	fmt1.nSamplesPerSec		= samples;
	fmt1.wBitsPerSample		= bits;
	fmt1.cbSize				= 0;
	fmt1.nAvgBytesPerSec	= (fmt1.wBitsPerSample / 8) * fmt1.nSamplesPerSec * fmt1.nChannels;
	fmt1.nBlockAlign		= (fmt1.wBitsPerSample / 8);
}

void PlayWave(HWND hwnd1, WAVEFORMATEX& fmt1, void* data, DWORD size1){
	static	WAVEHDR		hdr1;
	MMRESULT		res1;

	if(hWaveOut != NULL){
//		waveOutClose(hWaveOut);
		hWaveOut	= NULL;
	}

	WaveFmt	= fmt1;
	res1	= waveOutOpen(&hWaveOut, WAVE_MAPPER, &WaveFmt, (UINT)hwnd1, 0L, CALLBACK_WINDOW);
	if(res1 != MMSYSERR_NOERROR){
		MessageBox(NULL, L"Can not open wave in", L"wave", MB_OK);
		return;
	}

	memset(&hdr1, 0, sizeof(hdr1));
	hdr1.lpData			= (char*)data;
	hdr1.dwBufferLength	= size1;

	res1	= waveOutPrepareHeader(hWaveOut, &hdr1, sizeof(hdr1));
	if(res1 != MMSYSERR_NOERROR){
		MessageBox(NULL, L"Can not prepare wave out", L"wave", MB_OK);
		return;
	}

	res1	= waveOutWrite(hWaveOut, &hdr1, sizeof(hdr1));
	if(res1 != MMSYSERR_NOERROR){
		waveOutUnprepareHeader(hWaveOut, &hdr1, sizeof(hdr1));
		free(hdr1.lpData);
		MessageBox(NULL, L"Can not write wave out", L"wave", MB_OK);
		return;
	}
}