#pragma once

namespace MockingApplication {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;

	public ref class MockingSettings : public System::Windows::Forms::Form
	{
	public:
		MockingSettings(void);

	protected:
		~MockingSettings();

	public: 
		System::Windows::Forms::CheckedListBox^ checkedListBox;
		System::Windows::Forms::Button^ apply_btn;
		System::Windows::Forms::FlowLayoutPanel^ flowLayoutPanel;
		System::Windows::Forms::Button^ save_btn;
	private: 
		System::Windows::Forms::Panel^ top_pnl;
		System::Windows::Forms::Button^ close_btn;
		System::Windows::Forms::Label^ title_lbl;

	private:
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void);
#pragma endregion

	private: 
		System::Void top_pnl_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
		   [DllImport("user32.dll")]
			   static bool ReleaseCapture();
		   [DllImport("user32.dll")]
			   static IntPtr SendMessage(IntPtr hWnd, int Msg, IntPtr wParam, IntPtr lParam);
		   const int WM_NCLBUTTONDOWN = 0xA1;
		   const IntPtr HTCAPTION = IntPtr(0x2);

		   System::Void close_btn_Click(System::Object^ sender, System::EventArgs^ e);
};
}
