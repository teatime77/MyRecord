template <class T> class TSeq : public TFnc {
public:
	int		seqSt;
	int		seqEd;
	int		seqDiv;
	T*		seqVal;

	TSeq(){
		seqSt	= 0;
		seqEd	= 0;
		seqVal	= NULL;
	}

	TSeq(int st1, int ed1, int div1, T* val1){
		seqVal	= NULL;
		seqIni(st1, ed1, div1, val1);
	}

	void seqCopy(const TSeq<T>& s1){
		seqIni(s1.seqSt, s1.seqEd, s1.seqDiv, s1.seqVal);
	}

	void seqClear(){
		if(seqVal != NULL){
			delete[] seqVal;
			seqVal	= NULL;
		}
	}

	~TSeq(){
		seqClear();
	}

	void seqIni(int st1, int ed1, int div1, T* val1){
		int	cnt1;

		seqSt	= st1;
		seqEd	= ed1;
		seqDiv	= div1;
		cnt1	= ed1 - st1 + 1;
		if(seqVal != NULL){
			delete[] seqVal;
		}
		seqVal	= new T[cnt1];
		if(val1 != NULL){
			// 初期化データがある場合

			memcpy(seqVal, val1, cnt1 * sizeof(T));
		}
		else{
			// 初期化データがない場合

			memset(seqVal, 0, cnt1 * sizeof(T));
		}
	}
	
	void seqIniShort(int st1, int ed1, int div1, short* pshort1){
		T*	val1;
		int	cnt1, i1;

		cnt1	= ed1 - st1 + 1;
		val1	= new T[cnt1];
		for(i1 = 0; i1 < cnt1; i1++){
			val1[i1]	= (T)pshort1[i1];
		}
		seqIni(st1, ed1, div1, val1);
		delete[] val1;
	}

	inline T seqAt(int i1){
		if(seqSt <= i1 && i1 <= seqEd){
			return seqVal[i1 - seqSt];
		}
		else{
			return (T)0;
		}
	}

	inline void seqSet(int i1, T val1){
		seqVal[i1 - seqSt]	= val1;
	}

	/*
		両端の不要な0を取り除く
	*/
	void seqTrim(){
		int	st1, ed1, cnt1;
		T	*ptr1;

		for(st1 = seqSt; st1 <= seqEd; st1++){
			if(seqAt(st1) != 0){
				if(seqSt < st1){
					st1--;
				}
				break;
			}
		}
		for(ed1 = seqEd; st1 < ed1; ed1--){
			if(seqAt(ed1) != 0){
				if(ed1 < seqEd){
					ed1++;
				}
				break;
			}
		}
		if(st1 <= seqEd && (st1 != seqSt || ed1 != seqEd)){
			// 0以外の値がある場合

			cnt1	= ed1 - st1 + 1;
			ptr1	= new T[cnt1];
			memcpy(ptr1, seqVal + (st1 - seqSt), cnt1 * sizeof(T));

			seqSt	= st1;
			seqEd	= ed1;		
			delete[] seqVal;
			seqVal	= ptr1;
		}
	}

	virtual double DVal(double d1){
		int	i1;

		i1	= (int)(d1 * seqDiv);
		if(seqSt <= i1 && i1 <= seqEd){
			return seqAt(i1);
		}
		else{
			return 0;
		}
	}
};

#define	WL_LVL	10

class TWavelet {
public:
	TSeq<double>	vPhi[WL_LVL];
	TSeq<double>	vPsi[WL_LVL];
	TSeq<double>	Pk;
	TSeq<double>	Qk;
	TSeq<double>	Gk;
	TSeq<double>	Hk;

	~TWavelet(){
		int	i1;

		for(i1 = 0; i1 < sizeof(vPhi)/sizeof(vPhi[0]); i1++){
			vPhi[i1].seqClear();
		}

		for(i1 = 0; i1 < sizeof(vPsi)/sizeof(vPsi[0]); i1++){
			vPsi[i1].seqClear();
		}
	}

	void TestWavelet(int dbc_from, int dbc_to);
	void CalcWavelet(short* pshort1, int cnt1, int tt);
};


