
#pragma once

namespace ToolsGUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for SpecularForm
	/// </summary>
	public ref class SpecularForm : public System::Windows::Forms::Form
	{
	public:
		SpecularForm(void)
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
		~SpecularForm()
		{
			if (components)
			{
				delete components;
			}
		}

	public: System::Windows::Forms::TrackBar^ trackBarSpecular;
	private: System::Windows::Forms::Label^ labelSpecular;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->trackBarSpecular = (gcnew System::Windows::Forms::TrackBar());
			this->labelSpecular = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBarSpecular))->BeginInit();
			this->SuspendLayout();
			//
			// trackBarSpecular
			//
			this->trackBarSpecular->LargeChange = 10;
			this->trackBarSpecular->Location = System::Drawing::Point(12, 12);
			this->trackBarSpecular->Maximum = 128;
			this->trackBarSpecular->Minimum = 1;
			this->trackBarSpecular->Name = L"trackBarSpecular";
			this->trackBarSpecular->Size = System::Drawing::Size(310, 45);
			this->trackBarSpecular->TabIndex = 0;
			this->trackBarSpecular->TickFrequency = 10;
			this->trackBarSpecular->Value = 4;
			this->trackBarSpecular->Scroll += gcnew System::EventHandler(this, &SpecularForm::trackBarSpecular_Scroll);
			//
			// labelSpecular
			//
			this->labelSpecular->AutoSize = true;
			this->labelSpecular->Location = System::Drawing::Point(12, 60);
			this->labelSpecular->Name = L"labelSpecular";
			this->labelSpecular->Size = System::Drawing::Size(94, 13);
			this->labelSpecular->TabIndex = 1;
			this->labelSpecular->Text = L"Specular Power: 4";
			this->labelSpecular->Click += gcnew System::EventHandler(this, &SpecularForm::labelSpecular_Click);
			//
			// SpecularForm
			//
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(334, 111);
			this->ControlBox = false;
			this->Controls->Add(this->labelSpecular);
			this->Controls->Add(this->trackBarSpecular);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Name = L"SpecularForm";
			this->Text = L"Specular Lighting";
			this->TopMost = true;
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBarSpecular))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();
		}
#pragma endregion

	private: System::Void trackBarSpecular_Scroll(System::Object^ sender, System::EventArgs^ e) {
		labelSpecular->Text = L"Specular Power: " + trackBarSpecular->Value.ToString();
	}

	private: System::Void labelSpecular_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	};
}
