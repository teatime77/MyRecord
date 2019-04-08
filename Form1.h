#pragma once
using namespace System::Windows::Forms;

void OnFormLoad(void* ptr);
void OnPlayBtn();
void OnStopBtn();
void OnRecordBtn();
void OnResetBtn();
void OnSinBtn();
void OnGraphBtn();
void OnOKBtn();
void MyDefWindowProc(Message msg);

namespace MyRecord {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Form1 の概要
	///
	/// 警告: このクラスの名前を変更する場合、このクラスが依存するすべての .resx ファイルに関連付けられた
	///          マネージ リソース コンパイラ ツールに対して 'Resource File Name' プロパティを
	///          変更する必要があります。この変更を行わないと、
	///          デザイナと、このフォームに関連付けられたローカライズ済みリソースとが、
	///          正しく相互に利用できなくなります。
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: ここにコンストラクタ コードを追加します
			//
		}

	protected:
		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  btn_OK;
	private: System::Windows::Forms::Button^  btn_Play;
	private: System::Windows::Forms::Button^  btn_Stop;
	private: System::Windows::Forms::Button^  btn_Record;
	private: System::Windows::Forms::Button^  btn_Reset;
	private: System::Windows::Forms::Button^  btn_Sin;
	private: System::Windows::Forms::Button^  btn_Graph;






	protected: 

	private:
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// デザイナ サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディタで変更しないでください。
		/// </summary>
		void InitializeComponent(void)
		{
			this->btn_OK = (gcnew System::Windows::Forms::Button());
			this->btn_Play = (gcnew System::Windows::Forms::Button());
			this->btn_Stop = (gcnew System::Windows::Forms::Button());
			this->btn_Record = (gcnew System::Windows::Forms::Button());
			this->btn_Reset = (gcnew System::Windows::Forms::Button());
			this->btn_Sin = (gcnew System::Windows::Forms::Button());
			this->btn_Graph = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// btn_OK
			// 
			this->btn_OK->Location = System::Drawing::Point(514, 228);
			this->btn_OK->Name = L"btn_OK";
			this->btn_OK->Size = System::Drawing::Size(75, 23);
			this->btn_OK->TabIndex = 0;
			this->btn_OK->Text = L"OK";
			this->btn_OK->UseVisualStyleBackColor = true;
			this->btn_OK->Click += gcnew System::EventHandler(this, &Form1::btn_OK_Click);
			// 
			// btn_Play
			// 
			this->btn_Play->Location = System::Drawing::Point(28, 228);
			this->btn_Play->Name = L"btn_Play";
			this->btn_Play->Size = System::Drawing::Size(75, 23);
			this->btn_Play->TabIndex = 0;
			this->btn_Play->Text = L"Play";
			this->btn_Play->UseVisualStyleBackColor = true;
			this->btn_Play->Click += gcnew System::EventHandler(this, &Form1::btn_Play_Click);
			// 
			// btn_Stop
			// 
			this->btn_Stop->Location = System::Drawing::Point(109, 228);
			this->btn_Stop->Name = L"btn_Stop";
			this->btn_Stop->Size = System::Drawing::Size(75, 23);
			this->btn_Stop->TabIndex = 0;
			this->btn_Stop->Text = L"Stop";
			this->btn_Stop->UseVisualStyleBackColor = true;
			this->btn_Stop->Click += gcnew System::EventHandler(this, &Form1::btn_Stop_Click);
			// 
			// btn_Record
			// 
			this->btn_Record->Location = System::Drawing::Point(190, 228);
			this->btn_Record->Name = L"btn_Record";
			this->btn_Record->Size = System::Drawing::Size(75, 23);
			this->btn_Record->TabIndex = 0;
			this->btn_Record->Text = L"Record";
			this->btn_Record->UseVisualStyleBackColor = true;
			this->btn_Record->Click += gcnew System::EventHandler(this, &Form1::btn_Record_Click);
			// 
			// btn_Reset
			// 
			this->btn_Reset->Location = System::Drawing::Point(271, 228);
			this->btn_Reset->Name = L"btn_Reset";
			this->btn_Reset->Size = System::Drawing::Size(75, 23);
			this->btn_Reset->TabIndex = 0;
			this->btn_Reset->Text = L"Reset";
			this->btn_Reset->UseVisualStyleBackColor = true;
			this->btn_Reset->Click += gcnew System::EventHandler(this, &Form1::btn_Reset_Click);
			// 
			// btn_Sin
			// 
			this->btn_Sin->Location = System::Drawing::Point(352, 228);
			this->btn_Sin->Name = L"btn_Sin";
			this->btn_Sin->Size = System::Drawing::Size(75, 23);
			this->btn_Sin->TabIndex = 0;
			this->btn_Sin->Text = L"Sin";
			this->btn_Sin->UseVisualStyleBackColor = true;
			this->btn_Sin->Click += gcnew System::EventHandler(this, &Form1::btn_Sin_Click);
			// 
			// btn_Graph
			// 
			this->btn_Graph->Location = System::Drawing::Point(433, 228);
			this->btn_Graph->Name = L"btn_Graph";
			this->btn_Graph->Size = System::Drawing::Size(75, 23);
			this->btn_Graph->TabIndex = 0;
			this->btn_Graph->Text = L"Graph";
			this->btn_Graph->UseVisualStyleBackColor = true;
			this->btn_Graph->Click += gcnew System::EventHandler(this, &Form1::btn_Graph_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(620, 263);
			this->Controls->Add(this->btn_Graph);
			this->Controls->Add(this->btn_Sin);
			this->Controls->Add(this->btn_Reset);
			this->Controls->Add(this->btn_Record);
			this->Controls->Add(this->btn_Stop);
			this->Controls->Add(this->btn_Play);
			this->Controls->Add(this->btn_OK);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->ResumeLayout(false);

		}
#pragma endregion

	void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
		OnFormLoad(Handle.ToPointer());
	}
	void btn_Play_Click(System::Object^  sender, System::EventArgs^  e) {
		OnPlayBtn();
	}
	void btn_Stop_Click(System::Object^  sender, System::EventArgs^  e) {
		OnStopBtn();
	}
	void btn_Record_Click(System::Object^  sender, System::EventArgs^  e) {
		OnRecordBtn();
	}
	void btn_Reset_Click(System::Object^  sender, System::EventArgs^  e) {
		OnResetBtn();
	}
	void btn_Sin_Click(System::Object^  sender, System::EventArgs^  e) {
		OnSinBtn();
	}
	void btn_Graph_Click(System::Object^  sender, System::EventArgs^  e) {
		OnGraphBtn();
	}
	void btn_OK_Click(System::Object^  sender, System::EventArgs^  e) {
		OnOKBtn();
	}
	
	protected:
	virtual void DefWndProc (Message% m) override{
		MyDefWindowProc(m);
		Form::DefWndProc(m);
	}
};
}

