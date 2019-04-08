#define PI	3.14159265358979323846264338327950288419716939937510

class TCmp {
public:
	double	cmpR;
	double	cmpI;

	TCmp(double re1, double im1);
	TCmp(){};

	TCmp& operator=(double d1);
	TCmp& operator+=(TCmp& cmp1);
	TCmp& operator*=(TCmp& cmp1);
};

TCmp operator *(TCmp& cmp1, TCmp& cmp2);

class TFnc{
public:
	virtual double DVal(double d1) = 0;
};


class TSinN : public TFnc {
public:
	int	sinN;

	double DVal(double d1){ return sin(sinN * d1); }
};

class TCosN : public TFnc {
public:
	int	cosN;

	double DVal(double d1){ return cos(cosN * d1); }
};

class TFncP : public TFnc {
public:
	TFnc	*fncpFnc1;
	TFnc	*fncpFnc2;
	double DVal(double d1){ return fncpFnc1->DVal(d1) * fncpFnc2->DVal(d1); }
};

class TFncX_1 : public TFnc {
public:
	double DVal(double d1){ return ((- PI/2 < d1 && d1 < PI/2) ? 1 : 0); }
//	double DVal(double d1){ return d1 * d1 - 0.5; }
};



extern	TCmp theI;

template<class T> class TMat {
public:
	int	matRow;
	int	matCol;
	T**	matDat;

	TMat();
	~TMat();

	void matInit(int row1, int col1);
	void matClear();
	void matCopy(TMat<T>& mat1);
	inline T& matAt(int row1, int col1){ return matDat[col1][row1]; }
	void matPrint(String^% str1);

};

template<class T> void matMul(TMat<T>& mat1, TMat<T>& mat2, TMat<T>& mat3);
template<class T> void matMulN(TMat<T>& mat1, T t1);

void setZero(TCmp& cmp1);

TCmp c_exp(TCmp& cmp2);
TCmp conj(TCmp& cmp2);
void conjMat(TMat<TCmp>& mat1);
TCmp omega(int n1, int k1);
void Print(String^% str1, TCmp& cmp1);
void setF(TMat<TCmp>& mat1, int dim1);
void initFFT(int N);
void FFT(int N, double re1[], double im1[], double Re[], double Im[], bool is_dft);

/*
	TMat implementation
*/

template<class T> TMat<T>::TMat(){
	matCol	= 0;
	matRow	= 0;
	matDat	= NULL;
}

template<class T> TMat<T>::~TMat(){
	if(matDat != NULL){
		matClear();
	}
}

template<class T> void TMat<T>::matClear(){
	int	i1;

	for(i1 = 0; i1 < matCol; i1++){
		delete[] matDat[i1];
	}
	delete[] matDat;
	matDat	= NULL;
}

template<class T> void TMat<T>::matInit(int row1, int col1){
	int	i1;

	if(matDat != NULL){
		matClear();
	}

	matCol	= col1;
	matRow	= row1;
	matDat	= new T*[matCol];
	for(i1 = 0; i1 < matCol; i1++){
		matDat[i1]	= new T[matRow];
	}
}

template<class T> void TMat<T>::matCopy(TMat<T>& mat1){
	int	i1, i2;

	matInit(mat1.matRow, mat1.matCol);

	for(i1 = 0; i1 < matRow; i1++){
		for(i2 = 0; i2 < matCol; i2++){
			matAt(i1, i2)	= mat1.matAt(i1, i2);
		}
	}
}

template<class T> void matMul(TMat<T>& mat1, TMat<T>& mat2, TMat<T>& mat3){
	int	i1, i2, i3;
	T	d1;

	if(mat2.matCol != mat3.matRow){
		::MessageBox(NULL, L"mat mul", L"dmat error", MB_OK);
		return;
	}

	mat1.matClear();
	mat1.matInit(mat2.matRow, mat3.matCol);

	for(i1 = 0; i1 < mat1.matCol; i1++){
		for(i2 = 0; i2 < mat1.matRow; i2++){
			d1	= 0;
			for(i3 = 0; i3 < mat2.matCol; i3++){
				d1	+= mat2.matAt(i2, i3) * mat3.matAt(i3, i1);
			}
			mat1.matAt(i2, i1)	= d1;
		}
	}
}

template<class T> void matMulN(TMat<T>& mat1, T t1){
	int	i1, i2;

	for(i1 = 0; i1 < mat1.matCol; i1++){
		for(i2 = 0; i2 < mat1.matRow; i2++){
			mat1.matAt(i2, i1)	*= t1;
		}
	}
}

template<class T> void TMat<T>::matPrint(String^% str1){
	int	i1, i2;

	for(i1 = 0; i1 < matRow; i1++){
		str1	+= "(";
		for(i2 = 0; i2 < matCol; i2++){
			Print(str1,	matAt(i1, i2));
		}
		str1	+= ")\r\n";
	}
}
