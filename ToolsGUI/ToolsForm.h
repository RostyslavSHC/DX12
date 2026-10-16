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
			this->SuspendLayout();
			// 
			// checkBoxRed
			// 
			this->checkBoxRed->AutoSize = true;
			this->checkBoxRed->Location = System::Drawing::Point(578, 126);
			this->checkBoxRed->Name = L"checkBoxRed";
			this->checkBoxRed->Size = System::Drawing::Size(46, 17);
			this->checkBoxRed->TabIndex = 0;
			this->checkBoxRed->Text = L"Red";
			this->checkBoxRed->UseVisualStyleBackColor = true;
			// 
			// checkBoxGreen
			// 
			this->checkBoxGreen->AutoSize = true;
			this->checkBoxGreen->Location = System::Drawing::Point(578, 149);
			this->checkBoxGreen->Name = L"checkBoxGreen";
			this->checkBoxGreen->Size = System::Drawing::Size(55, 17);
			this->checkBoxGreen->TabIndex = 1;
			this->checkBoxGreen->Text = L"Green";
			this->checkBoxGreen->UseVisualStyleBackColor = true;
			// 
			// checkBoxBlue
			// 
			this->checkBoxBlue->AutoSize = true;
			this->checkBoxBlue->Location = System::Drawing::Point(578, 172);
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
			// ToolsForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(817, 323);
			this->ControlBox = false;
			this->Controls->Add(this->buttonResetLight);
			this->Controls->Add(this->radioButtonMoveLight);
			this->Controls->Add(this->checkBoxBlue);
			this->Controls->Add(this->checkBoxGreen);
			this->Controls->Add(this->checkBoxRed);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Name = L"ToolsForm";
			this->Text = L"ToolsForm";
			this->TopMost = true;
			this->ResumeLayout(false);
			this->PerformLayout();

		}

	#pragma endregion
	private: System::Void radioButtonMoveLight_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {

	}

	private: System::Void buttonResetLight_Click(System::Object^ sender, System::EventArgs^ e) {
		resetLightRequested = true; // Request the light position to reset
	}



};
}
