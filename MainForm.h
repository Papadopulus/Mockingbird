#pragma once
#include <filesystem>
#include <fstream>
#include "Home.h"
#include "FileForm.h"
#include "FilesForm.h"

namespace CppCLRWinFormsProject {


	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;

	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void);

	protected:
		~MainForm();
	private:
		MockingApplication::Home^ homeForm;
		MockingApplication::FileForm^ fileForm;
		MockingApplication::FilesForm^ filesForm;

	private: System::Windows::Forms::Panel^ pnlSideBar;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::FlowLayoutPanel^ flowLayoutPanel1;
	private: System::Windows::Forms::Button^ homeButton;
	private: System::Windows::Forms::Panel^ homeContainer;
	private: System::Windows::Forms::PictureBox^ pictureBox2;
	private: System::Windows::Forms::Panel^ fileContainer;
	private: System::Windows::Forms::Button^ mockOneFile_btn;
	private: System::Windows::Forms::Panel^ filesContainer;
	private: System::Windows::Forms::Button^ mockFiles_btn;
	private: ReaLTaiizor::Controls::NightControlBox^ nightControlBox1;

	protected:
	private:
		System::ComponentModel::Container^ components;

		void InitializeComponent(void);

	private:
		System::Void homeButton_Click(System::Object^ sender, System::EventArgs^ e);
		void HomeForm_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e);
		System::Void mockOneFile_btn_Click(System::Object^ sender, System::EventArgs^ e);
		void FileForm_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e);
		System::Void mockFiles_btn_Click(System::Object^ sender, System::EventArgs^ e);
		void FilesForm_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e);
		System::Void pnlSideBar_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);

		[DllImport("user32.dll")]
			static bool ReleaseCapture();
		[DllImport("user32.dll")]
			static IntPtr SendMessage(IntPtr hWnd, int Msg, IntPtr wParam, IntPtr lParam);
		const int WM_NCLBUTTONDOWN = 0xA1;
		const IntPtr HTCAPTION = IntPtr(0x2);
	};
}
