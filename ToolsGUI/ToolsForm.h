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
			this->SuspendLayout();
			// 
			// checkBoxRed
			// 
			this->checkBoxRed->AutoSize = true;
			this->checkBoxRed->Location = System::Drawing::Point(28, 28);
			this->checkBoxRed->Name = L"checkBoxRed";
			this->checkBoxRed->Size = System::Drawing::Size(122, 17);
			this->checkBoxRed->TabIndex = 0;
			this->checkBoxRed->Text = L"Render Background";
			this->checkBoxRed->UseVisualStyleBackColor = true;
			// 
			// checkBoxGreen
			// 
			this->checkBoxGreen->AutoSize = true;
			this->checkBoxGreen->Location = System::Drawing::Point(28, 51);
			this->checkBoxGreen->Name = L"checkBoxGreen";
			this->checkBoxGreen->Size = System::Drawing::Size(110, 17);
			this->checkBoxGreen->TabIndex = 1;
			this->checkBoxGreen->Text = L"Render Spotlights";
			this->checkBoxGreen->UseVisualStyleBackColor = true;
			// 
			// checkBoxBlue
			// 
			this->checkBoxBlue->AutoSize = true;
			this->checkBoxBlue->Location = System::Drawing::Point(28, 74);
			this->checkBoxBlue->Name = L"checkBoxBlue";
			this->checkBoxBlue->Size = System::Drawing::Size(47, 17);
			this->checkBoxBlue->TabIndex = 2;
			this->checkBoxBlue->Text = L"Blue";
			this->checkBoxBlue->UseVisualStyleBackColor = true;
			this->checkBoxBlue->Visible = false;
			// 
			// ToolsForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(190, 98);
			this->ControlBox = false;
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
	};
}
