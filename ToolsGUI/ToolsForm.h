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
	public: System::Windows::Forms::CheckBox^ checkBoxRed;
	public: System::Windows::Forms::CheckBox^ checkBoxGreen;
	public: System::Windows::Forms::CheckBox^ checkBoxBlue;
	private: System::Windows::Forms::RadioButton^ radioButtonMoveLight;
	private: System::Windows::Forms::Button^ buttonResetLight;
	public:
	public: bool resetLightRequested = false; // True when the reset button is clicked
	private: System::Windows::Forms::Label^ labelSpecularStrength;
	public: System::Windows::Forms::TrackBar^ trackBarSpecularStrength;
	private: System::Windows::Forms::Label^ labelSpecularColor;
	private: System::Windows::Forms::Label^ labelSpecularRed;
	private: System::Windows::Forms::Label^ labelSpecularGreen;
	private: System::Windows::Forms::Label^ labelSpecularBlue;
	public: System::Windows::Forms::TrackBar^ trackBarSpecularRed;
	public: System::Windows::Forms::TrackBar^ trackBarSpecularGreen;
	public: System::Windows::Forms::TrackBar^ trackBarSpecularBlue;
	public: System::Windows::Forms::RadioButton^ radioButtonColorByPosition;
	public:
	private: System::Windows::Forms::Button^ buttonResetCube;

	public: bool resetCubeRequested = false; // True when the reset cube button is clicked
	public: System::Windows::Forms::RadioButton^ radioButtonMoveLightToCube;
	public:
	public:

	public:

	public:

	public:



	public:

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
			this->checkBoxRed = (gcnew System::Windows::Forms::CheckBox());
			this->checkBoxGreen = (gcnew System::Windows::Forms::CheckBox());
			this->checkBoxBlue = (gcnew System::Windows::Forms::CheckBox());
			this->radioButtonMoveLight = (gcnew System::Windows::Forms::RadioButton());
			this->buttonResetLight = (gcnew System::Windows::Forms::Button());
			this->labelSpecularStrength = (gcnew System::Windows::Forms::Label());
			this->trackBarSpecularStrength = (gcnew System::Windows::Forms::TrackBar());
			this->labelSpecularColor = (gcnew System::Windows::Forms::Label());
			this->labelSpecularRed = (gcnew System::Windows::Forms::Label());
			this->labelSpecularGreen = (gcnew System::Windows::Forms::Label());
			this->labelSpecularBlue = (gcnew System::Windows::Forms::Label());
			this->trackBarSpecularRed = (gcnew System::Windows::Forms::TrackBar());
			this->trackBarSpecularGreen = (gcnew System::Windows::Forms::TrackBar());
			this->trackBarSpecularBlue = (gcnew System::Windows::Forms::TrackBar());
			this->radioButtonColorByPosition = (gcnew System::Windows::Forms::RadioButton());
			this->buttonResetCube = (gcnew System::Windows::Forms::Button());
			this->radioButtonMoveLightToCube = (gcnew System::Windows::Forms::RadioButton());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBarSpecularStrength))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBarSpecularRed))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBarSpecularGreen))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBarSpecularBlue))->BeginInit();
			this->SuspendLayout();
			// 
			// checkBoxRed
			// 
			this->checkBoxRed->AutoSize = true;
			this->checkBoxRed->Location = System::Drawing::Point(726, 238);
			this->checkBoxRed->Name = L"checkBoxRed";
			this->checkBoxRed->Size = System::Drawing::Size(46, 17);
			this->checkBoxRed->TabIndex = 0;
			this->checkBoxRed->Text = L"Red";
			this->checkBoxRed->UseVisualStyleBackColor = true;
			// 
			// checkBoxGreen
			// 
			this->checkBoxGreen->AutoSize = true;
			this->checkBoxGreen->Location = System::Drawing::Point(726, 261);
			this->checkBoxGreen->Name = L"checkBoxGreen";
			this->checkBoxGreen->Size = System::Drawing::Size(55, 17);
			this->checkBoxGreen->TabIndex = 1;
			this->checkBoxGreen->Text = L"Green";
			this->checkBoxGreen->UseVisualStyleBackColor = true;
			// 
			// checkBoxBlue
			// 
			this->checkBoxBlue->AutoSize = true;
			this->checkBoxBlue->Location = System::Drawing::Point(726, 284);
			this->checkBoxBlue->Name = L"checkBoxBlue";
			this->checkBoxBlue->Size = System::Drawing::Size(47, 17);
			this->checkBoxBlue->TabIndex = 2;
			this->checkBoxBlue->Text = L"Blue";
			this->checkBoxBlue->UseVisualStyleBackColor = true;
			// 
			// radioButtonMoveLight
			// 
			this->radioButtonMoveLight->AutoSize = true;
			this->radioButtonMoveLight->Checked = true;
			this->radioButtonMoveLight->Location = System::Drawing::Point(28, 28);
			this->radioButtonMoveLight->Name = L"radioButtonMoveLight";
			this->radioButtonMoveLight->Size = System::Drawing::Size(78, 17);
			this->radioButtonMoveLight->TabIndex = 3;
			this->radioButtonMoveLight->TabStop = true;
			this->radioButtonMoveLight->Text = L"Move Light";
			this->radioButtonMoveLight->UseVisualStyleBackColor = true;
			this->radioButtonMoveLight->CheckedChanged += gcnew System::EventHandler(this, &ToolsForm::radioButtonMoveLight_CheckedChanged);
			// 
			// buttonResetLight
			// 
			this->buttonResetLight->Location = System::Drawing::Point(40, 55);
			this->buttonResetLight->Name = L"buttonResetLight";
			this->buttonResetLight->Size = System::Drawing::Size(140, 25);
			this->buttonResetLight->TabIndex = 4;
			this->buttonResetLight->Text = L"Reset Light Position";
			this->buttonResetLight->UseVisualStyleBackColor = true;
			this->buttonResetLight->Click += gcnew System::EventHandler(this, &ToolsForm::buttonResetLight_Click);
			// 
			// labelSpecularStrength
			// 
			this->labelSpecularStrength->AutoSize = true;
			this->labelSpecularStrength->Location = System::Drawing::Point(40, 95);
			this->labelSpecularStrength->Name = L"labelSpecularStrength";
			this->labelSpecularStrength->Size = System::Drawing::Size(92, 13);
			this->labelSpecularStrength->TabIndex = 5;
			this->labelSpecularStrength->Text = L"Specular Strength";
			this->labelSpecularStrength->Click += gcnew System::EventHandler(this, &ToolsForm::labelSpecularStrength_Click);
			// 
			// trackBarSpecularStrength
			// 
			this->trackBarSpecularStrength->LargeChange = 10;
			this->trackBarSpecularStrength->Location = System::Drawing::Point(145, 88);
			this->trackBarSpecularStrength->Maximum = 100;
			this->trackBarSpecularStrength->Name = L"trackBarSpecularStrength";
			this->trackBarSpecularStrength->Size = System::Drawing::Size(470, 45);
			this->trackBarSpecularStrength->TabIndex = 6;
			this->trackBarSpecularStrength->Value = 5;
			this->trackBarSpecularStrength->Scroll += gcnew System::EventHandler(this, &ToolsForm::trackBarSpecularStrength_Scroll);
			// 
			// labelSpecularColor
			// 
			this->labelSpecularColor->AutoSize = true;
			this->labelSpecularColor->Location = System::Drawing::Point(40, 145);
			this->labelSpecularColor->Name = L"labelSpecularColor";
			this->labelSpecularColor->Size = System::Drawing::Size(76, 13);
			this->labelSpecularColor->TabIndex = 7;
			this->labelSpecularColor->Text = L"Specular Color";
			// 
			// labelSpecularRed
			// 
			this->labelSpecularRed->AutoSize = true;
			this->labelSpecularRed->Location = System::Drawing::Point(125, 145);
			this->labelSpecularRed->Name = L"labelSpecularRed";
			this->labelSpecularRed->Size = System::Drawing::Size(15, 13);
			this->labelSpecularRed->TabIndex = 8;
			this->labelSpecularRed->Text = L"R";
			// 
			// labelSpecularGreen
			// 
			this->labelSpecularGreen->AutoSize = true;
			this->labelSpecularGreen->Location = System::Drawing::Point(125, 190);
			this->labelSpecularGreen->Name = L"labelSpecularGreen";
			this->labelSpecularGreen->Size = System::Drawing::Size(15, 13);
			this->labelSpecularGreen->TabIndex = 9;
			this->labelSpecularGreen->Text = L"G";
			// 
			// labelSpecularBlue
			// 
			this->labelSpecularBlue->AutoSize = true;
			this->labelSpecularBlue->Location = System::Drawing::Point(125, 235);
			this->labelSpecularBlue->Name = L"labelSpecularBlue";
			this->labelSpecularBlue->Size = System::Drawing::Size(14, 13);
			this->labelSpecularBlue->TabIndex = 10;
			this->labelSpecularBlue->Text = L"B";
			// 
			// trackBarSpecularRed
			// 
			this->trackBarSpecularRed->LargeChange = 10;
			this->trackBarSpecularRed->Location = System::Drawing::Point(145, 138);
			this->trackBarSpecularRed->Maximum = 200;
			this->trackBarSpecularRed->Name = L"trackBarSpecularRed";
			this->trackBarSpecularRed->Size = System::Drawing::Size(470, 45);
			this->trackBarSpecularRed->TabIndex = 11;
			this->trackBarSpecularRed->Value = 70;
			this->trackBarSpecularRed->Scroll += gcnew System::EventHandler(this, &ToolsForm::trackBarSpecularRed_Scroll);
			// 
			// trackBarSpecularGreen
			// 
			this->trackBarSpecularGreen->LargeChange = 10;
			this->trackBarSpecularGreen->Location = System::Drawing::Point(145, 183);
			this->trackBarSpecularGreen->Maximum = 200;
			this->trackBarSpecularGreen->Name = L"trackBarSpecularGreen";
			this->trackBarSpecularGreen->Size = System::Drawing::Size(470, 45);
			this->trackBarSpecularGreen->TabIndex = 12;
			this->trackBarSpecularGreen->Value = 70;
			this->trackBarSpecularGreen->Scroll += gcnew System::EventHandler(this, &ToolsForm::trackBarSpecularGreen_Scroll);
			// 
			// trackBarSpecularBlue
			// 
			this->trackBarSpecularBlue->LargeChange = 10;
			this->trackBarSpecularBlue->Location = System::Drawing::Point(145, 228);
			this->trackBarSpecularBlue->Maximum = 200;
			this->trackBarSpecularBlue->Name = L"trackBarSpecularBlue";
			this->trackBarSpecularBlue->Size = System::Drawing::Size(470, 45);
			this->trackBarSpecularBlue->TabIndex = 13;
			this->trackBarSpecularBlue->Value = 70;
			this->trackBarSpecularBlue->Scroll += gcnew System::EventHandler(this, &ToolsForm::trackBarSpecularBlue_Scroll);
			// 
			// radioButtonColorByPosition
			// 
			this->radioButtonColorByPosition->AutoSize = true;
			this->radioButtonColorByPosition->Location = System::Drawing::Point(28, 275);
			this->radioButtonColorByPosition->Name = L"radioButtonColorByPosition";
			this->radioButtonColorByPosition->Size = System::Drawing::Size(104, 17);
			this->radioButtonColorByPosition->TabIndex = 14;
			this->radioButtonColorByPosition->TabStop = true;
			this->radioButtonColorByPosition->Text = L"Color By Position";
			this->radioButtonColorByPosition->UseVisualStyleBackColor = true;
			this->radioButtonColorByPosition->CheckedChanged += gcnew System::EventHandler(this, &ToolsForm::radioButtonColorByPosition_CheckedChanged);
			// 
			// buttonResetCube
			// 
			this->buttonResetCube->Location = System::Drawing::Point(40, 300);
			this->buttonResetCube->Name = L"buttonResetCube";
			this->buttonResetCube->Size = System::Drawing::Size(140, 25);
			this->buttonResetCube->TabIndex = 15;
			this->buttonResetCube->Text = L"Reset Cube Position";
			this->buttonResetCube->UseVisualStyleBackColor = true;
			this->buttonResetCube->Click += gcnew System::EventHandler(this, &ToolsForm::buttonResetCube_Click);
			// 
			// radioButtonMoveLightToCube
			// 
			this->radioButtonMoveLightToCube->AutoSize = true;
			this->radioButtonMoveLightToCube->Location = System::Drawing::Point(28, 347);
			this->radioButtonMoveLightToCube->Name = L"radioButtonMoveLightToCube";
			this->radioButtonMoveLightToCube->Size = System::Drawing::Size(122, 17);
			this->radioButtonMoveLightToCube->TabIndex = 17;
			this->radioButtonMoveLightToCube->TabStop = true;
			this->radioButtonMoveLightToCube->Text = L"Move Light To Cube";
			this->radioButtonMoveLightToCube->UseVisualStyleBackColor = true;
			this->radioButtonMoveLightToCube->CheckedChanged += gcnew System::EventHandler(this, &ToolsForm::radioButtonMoveLightToCube_CheckedChanged);
			// 
			// ToolsForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(801, 391);
			this->ControlBox = false;
			this->Controls->Add(this->radioButtonMoveLightToCube);
			this->Controls->Add(this->buttonResetCube);
			this->Controls->Add(this->radioButtonColorByPosition);
			this->Controls->Add(this->trackBarSpecularBlue);
			this->Controls->Add(this->trackBarSpecularGreen);
			this->Controls->Add(this->trackBarSpecularRed);
			this->Controls->Add(this->labelSpecularBlue);
			this->Controls->Add(this->labelSpecularGreen);
			this->Controls->Add(this->labelSpecularRed);
			this->Controls->Add(this->labelSpecularColor);
			this->Controls->Add(this->trackBarSpecularStrength);
			this->Controls->Add(this->labelSpecularStrength);
			this->Controls->Add(this->buttonResetLight);
			this->Controls->Add(this->radioButtonMoveLight);
			this->Controls->Add(this->checkBoxBlue);
			this->Controls->Add(this->checkBoxGreen);
			this->Controls->Add(this->checkBoxRed);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Name = L"ToolsForm";
			this->Text = L"ToolsForm";
			this->TopMost = true;
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBarSpecularStrength))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBarSpecularRed))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBarSpecularGreen))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBarSpecularBlue))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}

	#pragma endregion
	private: System::Void radioButtonMoveLight_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {

	}

	private: System::Void buttonResetLight_Click(System::Object^ sender, System::EventArgs^ e) {
		resetLightRequested = true; // Request the light position to reset
	}



	private: System::Void labelSpecularStrength_Click(System::Object^ sender, System::EventArgs^ e) {

	}

	private: System::Void trackBarSpecularStrength_Scroll(System::Object^ sender, System::EventArgs^ e) {

	}



	private: System::Void trackBarSpecularRed_Scroll(System::Object^ sender, System::EventArgs^ e) {

	}

	private: System::Void trackBarSpecularGreen_Scroll(System::Object^ sender, System::EventArgs^ e) {

	}

	private: System::Void trackBarSpecularBlue_Scroll(System::Object^ sender, System::EventArgs^ e) {

	}


	private: System::Void radioButtonColorByPosition_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {

	}

	private: System::Void buttonResetCube_Click(System::Object^ sender, System::EventArgs^ e) {
		resetCubeRequested = true; // Request the cube position to reset
	}

	private: System::Void radioButtonMoveLightToCube_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {

	}

};
}
