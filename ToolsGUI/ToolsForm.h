#pragma once

namespace ToolsGUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for ToolsForm
	/// </summary>
	public ref class ToolsForm : public System::Windows::Forms::Form
	{
	public:
		ToolsForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ToolsForm()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::TrackBar^ trackBarY;
	protected:
	public: System::Windows::Forms::TrackBar^ trackBarU;
	public: System::Windows::Forms::TrackBar^ trackBarV;



	protected:

	protected:


	private: System::Windows::Forms::Label^ Y;
	private: System::Windows::Forms::Label^ U;
	private: System::Windows::Forms::Label^ V;
	private: System::Windows::Forms::Label^ labelYValue;
	private: System::Windows::Forms::Label^ labelUValue;
	private: System::Windows::Forms::Label^ labelVValue;
	public: System::Windows::Forms::CheckBox^ checkBoxInvert;
	private:














	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->trackBarY = (gcnew System::Windows::Forms::TrackBar());
			this->trackBarU = (gcnew System::Windows::Forms::TrackBar());
			this->trackBarV = (gcnew System::Windows::Forms::TrackBar());
			this->Y = (gcnew System::Windows::Forms::Label());
			this->U = (gcnew System::Windows::Forms::Label());
			this->V = (gcnew System::Windows::Forms::Label());
			this->labelYValue = (gcnew System::Windows::Forms::Label());
			this->labelUValue = (gcnew System::Windows::Forms::Label());
			this->labelVValue = (gcnew System::Windows::Forms::Label());
			this->checkBoxInvert = (gcnew System::Windows::Forms::CheckBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBarY))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBarU))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBarV))->BeginInit();
			this->SuspendLayout();
			// 
			// trackBarY
			// 
			this->trackBarY->Location = System::Drawing::Point(73, 31);
			this->trackBarY->Maximum = 200;
			this->trackBarY->Name = L"trackBarY";
			this->trackBarY->Size = System::Drawing::Size(456, 45);
			this->trackBarY->TabIndex = 0;
			this->trackBarY->Value = 100;
			this->trackBarY->Scroll += gcnew System::EventHandler(this, &ToolsForm::trackBarY_Scroll);
			// 
			// trackBarU
			// 
			this->trackBarU->Location = System::Drawing::Point(73, 99);
			this->trackBarU->Maximum = 200;
			this->trackBarU->Name = L"trackBarU";
			this->trackBarU->Size = System::Drawing::Size(456, 45);
			this->trackBarU->TabIndex = 1;
			this->trackBarU->Value = 100;
			this->trackBarU->Scroll += gcnew System::EventHandler(this, &ToolsForm::trackBarU_Scroll);
			// 
			// trackBarV
			// 
			this->trackBarV->Location = System::Drawing::Point(73, 162);
			this->trackBarV->Maximum = 200;
			this->trackBarV->Name = L"trackBarV";
			this->trackBarV->Size = System::Drawing::Size(456, 45);
			this->trackBarV->TabIndex = 2;
			this->trackBarV->Value = 100;
			this->trackBarV->Scroll += gcnew System::EventHandler(this, &ToolsForm::trackBarV_Scroll);
			// 
			// Y
			// 
			this->Y->AutoSize = true;
			this->Y->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Y->Location = System::Drawing::Point(31, 31);
			this->Y->Name = L"Y";
			this->Y->Size = System::Drawing::Size(16, 16);
			this->Y->TabIndex = 3;
			this->Y->Text = L"Y";
			// 
			// U
			// 
			this->U->AutoSize = true;
			this->U->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->U->Location = System::Drawing::Point(30, 99);
			this->U->Name = L"U";
			this->U->Size = System::Drawing::Size(17, 16);
			this->U->TabIndex = 4;
			this->U->Text = L"U";
			// 
			// V
			// 
			this->V->AutoSize = true;
			this->V->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->V->Location = System::Drawing::Point(31, 162);
			this->V->Name = L"V";
			this->V->Size = System::Drawing::Size(16, 16);
			this->V->TabIndex = 5;
			this->V->Text = L"V";
			// 
			// labelYValue
			// 
			this->labelYValue->AutoSize = true;
			this->labelYValue->Location = System::Drawing::Point(549, 33);
			this->labelYValue->Name = L"labelYValue";
			this->labelYValue->Size = System::Drawing::Size(33, 13);
			this->labelYValue->TabIndex = 6;
			this->labelYValue->Text = L"100%";
			// 
			// labelUValue
			// 
			this->labelUValue->AutoSize = true;
			this->labelUValue->Location = System::Drawing::Point(549, 102);
			this->labelUValue->Name = L"labelUValue";
			this->labelUValue->Size = System::Drawing::Size(33, 13);
			this->labelUValue->TabIndex = 7;
			this->labelUValue->Text = L"100%";
			// 
			// labelVValue
			// 
			this->labelVValue->AutoSize = true;
			this->labelVValue->Location = System::Drawing::Point(549, 165);
			this->labelVValue->Name = L"labelVValue";
			this->labelVValue->Size = System::Drawing::Size(33, 13);
			this->labelVValue->TabIndex = 8;
			this->labelVValue->Text = L"100%";
			// 
			// checkBoxInvert
			// 
			this->checkBoxInvert->AutoSize = true;
			this->checkBoxInvert->Location = System::Drawing::Point(33, 213);
			this->checkBoxInvert->Name = L"checkBoxInvert";
			this->checkBoxInvert->Size = System::Drawing::Size(85, 17);
			this->checkBoxInvert->TabIndex = 9;
			this->checkBoxInvert->Text = L"Invert Colors";
			this->checkBoxInvert->UseVisualStyleBackColor = true;
			this->checkBoxInvert->CheckedChanged += gcnew System::EventHandler(this, &ToolsForm::checkBoxInvert_CheckedChanged);
			// 
			// ToolsForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(658, 261);
			this->ControlBox = false;
			this->Controls->Add(this->checkBoxInvert);
			this->Controls->Add(this->labelVValue);
			this->Controls->Add(this->labelUValue);
			this->Controls->Add(this->labelYValue);
			this->Controls->Add(this->V);
			this->Controls->Add(this->U);
			this->Controls->Add(this->Y);
			this->Controls->Add(this->trackBarV);
			this->Controls->Add(this->trackBarU);
			this->Controls->Add(this->trackBarY);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Name = L"ToolsForm";
			this->Text = L"ToolsForm";
			this->TopMost = true;
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBarY))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBarU))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBarV))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion







// Trackbar Y - Show the value of the trackbar
private: System::Void trackBarY_Scroll(System::Object^ sender, System::EventArgs^ e) {
	labelYValue->Text = trackBarY->Value.ToString() + "%";
}

// Trackbar U - Show the value of the trackbar
private: System::Void trackBarU_Scroll(System::Object^ sender, System::EventArgs^ e) {
	labelUValue->Text = trackBarU->Value.ToString() + "%";
}

// Trackbar V - Show the value of the trackbar
private: System::Void trackBarV_Scroll(System::Object^ sender, System::EventArgs^ e) {
	labelVValue->Text = trackBarV->Value.ToString() + "%";
}


// Checkbox - Invert Color
private: System::Void checkBoxInvert_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
}





};
}




