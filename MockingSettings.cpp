#include "pch.h"
#include "MockingSettings.h"

using namespace MockingApplication;

MockingSettings::MockingSettings(void)
{
	InitializeComponent();
}
MockingSettings::~MockingSettings()
{
	if (components)
	{
		delete components;
	}
}
void MockingSettings::InitializeComponent(void)
{
	this->checkedListBox = (gcnew System::Windows::Forms::CheckedListBox());
	this->top_pnl = (gcnew System::Windows::Forms::Panel());
	this->title_lbl = (gcnew System::Windows::Forms::Label());
	this->close_btn = (gcnew System::Windows::Forms::Button());
	this->flowLayoutPanel = (gcnew System::Windows::Forms::FlowLayoutPanel());
	this->save_btn = (gcnew System::Windows::Forms::Button());
	this->apply_btn = (gcnew System::Windows::Forms::Button());
	this->top_pnl->SuspendLayout();
	this->SuspendLayout();
	// 
	// checkedListBox
	// 
	this->checkedListBox->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->checkedListBox->FormattingEnabled = true;
	this->checkedListBox->Location = System::Drawing::Point(45, 84);
	this->checkedListBox->Name = L"checkedListBox";
	this->checkedListBox->Size = System::Drawing::Size(598, 220);
	this->checkedListBox->TabIndex = 1;
	// 
	// top_pnl
	// 
	this->top_pnl->BackColor = System::Drawing::Color::White;
	this->top_pnl->Controls->Add(this->title_lbl);
	this->top_pnl->Location = System::Drawing::Point(1, 0);
	this->top_pnl->Name = L"top_pnl";
	this->top_pnl->Size = System::Drawing::Size(807, 46);
	this->top_pnl->TabIndex = 2;
	this->top_pnl->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MockingSettings::top_pnl_MouseDown);
	// 
	// title_lbl
	// 
	this->title_lbl->AutoSize = true;
	this->title_lbl->Font = (gcnew System::Drawing::Font(L"Segoe UI", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->title_lbl->Location = System::Drawing::Point(11, 9);
	this->title_lbl->Name = L"title_lbl";
	this->title_lbl->Size = System::Drawing::Size(168, 25);
	this->title_lbl->TabIndex = 0;
	this->title_lbl->Text = L"Mocking Settings";
	// 
	// close_btn
	// 
	this->close_btn->Location = System::Drawing::Point(70, 500);
	this->close_btn->Name = L"close_btn";
	this->close_btn->Size = System::Drawing::Size(108, 29);
	this->close_btn->TabIndex = 3;
	this->close_btn->Text = L"Close";
	this->close_btn->UseVisualStyleBackColor = true;
	this->close_btn->Click += gcnew System::EventHandler(this, &MockingSettings::close_btn_Click);
	// 
	// flowLayoutPanel
	// 
	this->flowLayoutPanel->AutoScroll = true;
	this->flowLayoutPanel->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
	this->flowLayoutPanel->FlowDirection = System::Windows::Forms::FlowDirection::TopDown;
	this->flowLayoutPanel->Location = System::Drawing::Point(45, 321);
	this->flowLayoutPanel->Name = L"flowLayoutPanel";
	this->flowLayoutPanel->Size = System::Drawing::Size(598, 135);
	this->flowLayoutPanel->TabIndex = 4;
	// 
	// save_btn
	// 
	this->save_btn->Location = System::Drawing::Point(513, 500);
	this->save_btn->Name = L"save_btn";
	this->save_btn->Size = System::Drawing::Size(108, 29);
	this->save_btn->TabIndex = 5;
	this->save_btn->Text = L"Save";
	this->save_btn->UseVisualStyleBackColor = true;
	// 
	// apply_btn
	// 
	this->apply_btn->Location = System::Drawing::Point(524, 463);
	this->apply_btn->Name = L"apply_btn";
	this->apply_btn->Size = System::Drawing::Size(86, 22);
	this->apply_btn->TabIndex = 6;
	this->apply_btn->Text = L"Apply";
	this->apply_btn->UseVisualStyleBackColor = true;
	// 
	// MockingSettings
	// 
	this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	this->ClientSize = System::Drawing::Size(680, 556);
	this->Controls->Add(this->apply_btn);
	this->Controls->Add(this->save_btn);
	this->Controls->Add(this->flowLayoutPanel);
	this->Controls->Add(this->close_btn);
	this->Controls->Add(this->top_pnl);
	this->Controls->Add(this->checkedListBox);
	this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
	this->Name = L"MockingSettings";
	this->Text = L"MockingSettings";
	this->top_pnl->ResumeLayout(false);
	this->top_pnl->PerformLayout();
	this->ResumeLayout(false);

}
System::Void MockingSettings::top_pnl_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
	if (e->Button == System::Windows::Forms::MouseButtons::Left) {
		ReleaseCapture();
		SendMessage(this->Handle, WM_NCLBUTTONDOWN, HTCAPTION, IntPtr(0));
	}
}
System::Void MockingSettings::close_btn_Click(System::Object^ sender, System::EventArgs^ e)
{
	this->Close();
}