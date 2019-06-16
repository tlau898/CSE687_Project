///////////////////////////////////////////////////////////////////////////
// Window.cpp - C++\CLI implementation of WPF Application                //
//          - Demo for CSE 687 Project #4                                //
// ver 2.0                                                               //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015             //
// Terence Lau, John Schurman, 2019                                      //
///////////////////////////////////////////////////////////////////////////
/*
*  To run as a Windows Application:
*  - Set Project Properties > Linker > System > Subsystem to Windows
*  - Comment out int main(...) at bottom
*  - Uncomment int _stdcall WinMain() at bottom
*  To run as a Console Application:
*  - Set Project Properties > Linker > System > Subsytem to Console
*  - Uncomment int main(...) at bottom
*  - Comment out int _stdcall WinMain() at bottom
*/
#include "Window.h"
using namespace CppCliWindows;

WPFCppCliDemo::WPFCppCliDemo()
{
  // set up channel

  ObjectFactory* pObjFact = new ObjectFactory;
  pSendr_ = pObjFact->createSendr();
  pRecvr_ = pObjFact->createRecvr();
  pChann_ = pObjFact->createMockChannel(pSendr_, pRecvr_);
  pChann_->start();
  delete pObjFact;

  // client's receive thread

  recvThread = gcnew Thread(gcnew ThreadStart(this, &WPFCppCliDemo::getMessage));
  recvThread->Start();

  // set event handlers

  this->Loaded += 
    gcnew System::Windows::RoutedEventHandler(this, &WPFCppCliDemo::OnLoaded);
  this->Closing += 
    gcnew CancelEventHandler(this, &WPFCppCliDemo::Unloading);
  addButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::addTest);
  removeButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::removeTest);
  hSendButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::sendMessage);
  hClearButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clear);
  hFolderBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForFolder);
  hShowItemsButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::getItemsFromList);

  // set Window properties

  this->Title = "Test Harness Client";
  this->Width = 1000;
  this->Height = 750;

  // attach dock panel to Window

  this->Content = hDockPanel;
  hDockPanel->Children->Add(hStatusBar);
  hDockPanel->SetDock(hStatusBar, Dock::Bottom);
  hDockPanel->Children->Add(hGrid);

  // setup Window controls and views

  setupTestReqRow();
  setUpTabControl();
  setUpStatusBar();
  setUpSendMessageView();
  setUpFileListView();
  setUpConnectionView();
}

WPFCppCliDemo::~WPFCppCliDemo()
{
  delete pChann_;
  delete pSendr_;
  delete pRecvr_;
}

void WPFCppCliDemo::setUpStatusBar()
{
  hStatusBar->Items->Add(hStatusBarItem);
  hStatus->Text = "very important messages will appear here";
  hStatusBarItem->Content = hStatus;
  hStatusBar->Padding = Thickness(10, 2, 10, 2);
}

void WPFCppCliDemo::setupTestReqRow()
{
   //Setup listbox for available tests
   RowDefinition^ hRow0Def = gcnew RowDefinition();
   hRow0Def->Height = GridLength(150);
   hSendMessageGrid->RowDefinitions->Add(hRow0Def);
   Border^ hBorder1 = gcnew Border();
   hBorder1->BorderThickness = Thickness(1);
   hBorder1->BorderBrush = Brushes::Black;
   hBorder1->Child = masterLBox;
   masterLBox->Width = 200;
   masterLBox->Items->Add("SampleDriverA");
   masterLBox->Items->Add("SampleDriverB");
   masterLBox->Items->Add("SampleDriverC");
   masterLBox->Items->Add("SampleDriverD");
   masterLBox->SelectedIndex = 0;

   //Setup listbox for requested tests
   Border^ hBorder2 = gcnew Border();
   hBorder2->BorderThickness = Thickness(1);
   hBorder2->BorderBrush = Brushes::Black;
   hBorder2->Child = requestBox;
   requestBox->Width = 200;

   //Setup Add Test Button
   Border^ hBorder3 = gcnew Border();
   hBorder3->Width = 120;
   hBorder3->Height = 30;
   hBorder3->BorderThickness = Thickness(1);
   hBorder3->BorderBrush = Brushes::Black;
   addButton->Content = "Add Test -->";
   hBorder3->Child = addButton;

   //Setup Remove Test Button
   Border^ hBorder4 = gcnew Border();
   hBorder4->Width = 120;
   hBorder4->Height = 30;
   hBorder4->BorderThickness = Thickness(1);
   hBorder4->BorderBrush = Brushes::Black;
   removeButton->Content = "<-- Remove Test";
   hBorder4->Child = removeButton;

   TextBlock^ hSpacer0 = gcnew TextBlock();
   hSpacer0->Height = 10;

   //Setup Help Box
   Border^ hBorder0 = gcnew Border();
   hBorder0->Width = 380;
   hBorder0->BorderThickness = Thickness(1);
   hBorder0->BorderBrush = Brushes::Black;
   helpBox->FontWeight = FontWeights::Bold;
   helpBox->Text += " How to use this application:\n";
   helpBox->Text += " 1. Select test to add from list of available tests on the right\n";
   helpBox->Text += " 2. Click Add Test button to add the selected test to test request\n";
   helpBox->Text += " 3. Click Send Test Request button to generate test request\n";
   helpBox->Text += " 4. Wait for test results to appear below\n";
   hBorder0->Child = helpBox;

   buttonPanel->Children->Add(hBorder3);
   buttonPanel->Children->Add(hSpacer0);
   buttonPanel->Children->Add(hBorder4);
   buttonPanel->VerticalAlignment = System::Windows::VerticalAlignment::Center;

   TextBlock^ hSpacer1 = gcnew TextBlock();
   hSpacer1->Width = 16;

   TextBlock^ hSpacer2 = gcnew TextBlock();
   hSpacer2->Width = 16;

   //Add all controls to test panel
   testPanel->Children->Add(hBorder0);
   testPanel->Children->Add(hBorder1);
   testPanel->Children->Add(hSpacer1);
   testPanel->Children->Add(buttonPanel);
   testPanel->Children->Add(hSpacer2);
   testPanel->Children->Add(hBorder2);
   testPanel->Orientation = Orientation::Horizontal;
   testPanel->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
   hSendMessageGrid->SetRow(testPanel,0);
   hSendMessageGrid->Children->Add(testPanel);

   RowDefinition^ hRowPad = gcnew RowDefinition();
   hRowPad->Height = GridLength(20);
   hSendMessageGrid->RowDefinitions->Add(hRowPad);

}

void WPFCppCliDemo::setUpTabControl()
{
  hGrid->Children->Add(hTabControl);
  hSendMessageTab->Header = "Demo";
  hFileListTab->Header = "File List";
  hTabControl->Items->Add(hSendMessageTab);
}

void WPFCppCliDemo::setTextBlockProperties()
{
  RowDefinition^ hRow1Def = gcnew RowDefinition();
  hSendMessageGrid->RowDefinitions->Add(hRow1Def);
  Border^ hBorder1 = gcnew Border();
  hBorder1->BorderThickness = Thickness(1);
  hBorder1->BorderBrush = Brushes::Black;
  hBorder1->Child = hTextBlock1;
  hTextBlock1->Padding = Thickness(15);
  hTextBlock1->Text = "";
  hTextBlock1->FontWeight = FontWeights::Bold;
  hTextBlock1->FontSize = 16;
  hScrollViewer1->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
  hScrollViewer1->Content = hBorder1;
  hSendMessageGrid->SetRow(hScrollViewer1, 2);
  hSendMessageGrid->Children->Add(hScrollViewer1);
}

void WPFCppCliDemo::setButtonsProperties()
{
  RowDefinition^ hRow2Def = gcnew RowDefinition();
  hRow2Def->Height = GridLength(75);
  hSendMessageGrid->RowDefinitions->Add(hRow2Def);
  hSendButton->Content = "Send Test Request";
  Border^ hBorder2 = gcnew Border();
  hBorder2->Width = 120;
  hBorder2->Height = 30;
  hBorder2->BorderThickness = Thickness(1);
  hBorder2->BorderBrush = Brushes::Black;
  hClearButton->Content = "Clear";
  hBorder2->Child = hSendButton;
  Border^ hBorder3 = gcnew Border();
  hBorder3->Width = 120;
  hBorder3->Height = 30;
  hBorder3->BorderThickness = Thickness(1);
  hBorder3->BorderBrush = Brushes::Black;
  hBorder3->Child = hClearButton;
  hStackPanel1->Children->Add(hBorder2);
  TextBlock^ hSpacer = gcnew TextBlock();
  hSpacer->Width = 10;
  hStackPanel1->Children->Add(hSpacer);
  hStackPanel1->Children->Add(hBorder3);
  hStackPanel1->Orientation = Orientation::Horizontal;
  hStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
  hSendMessageGrid->SetRow(hStackPanel1, 3);
  hSendMessageGrid->Children->Add(hStackPanel1);
}

void WPFCppCliDemo::setUpSendMessageView()
{
  hSendMessageGrid->Margin = Thickness(20);
  hSendMessageTab->Content = hSendMessageGrid;

  setTextBlockProperties();
  setButtonsProperties();
}

std::string WPFCppCliDemo::toStdString(String^ pStr)
{
  std::string dst;
  for (int i = 0; i < pStr->Length; ++i)
    dst += (char)pStr[i];
  return dst;
}

void WPFCppCliDemo::addTest(Object^ obj, RoutedEventArgs^ args)
{
   if (masterLBox->SelectedIndex >= 0)
   {
      System::String^ selectedTest = masterLBox->SelectedItem->ToString();
      requestBox->Items->Add(selectedTest);
   }
}

void WPFCppCliDemo::removeTest(Object^ obj, RoutedEventArgs^ args)
{
   if (requestBox->SelectedIndex >= 0)
   {
      requestBox->Items->RemoveAt(requestBox->SelectedIndex);
   }
}

void WPFCppCliDemo::sendMessage(Object^ obj, RoutedEventArgs^ args)
{
   System::String^ xmlTestRequest = "";
   System::String^ displayRequest = "";
   //For each item in requested box add to test request
   for(int index = 0; index < requestBox->Items->Count; index++)
   {
      xmlTestRequest += "<testelement>" + requestBox->Items[index]->ToString() + "</testelement>";
      displayRequest += requestBox->Items[index]->ToString();
      if (index+1 < requestBox->Items->Count)
      {
         displayRequest += ", ";
      }
   }

   if (xmlTestRequest != "")
   {
      pSendr_->postMessage(toStdString(xmlTestRequest));
   }
   hTextBlock1->Text += "Sent Test Request: " + displayRequest + "\n";
   hStatus->Text = "Sent message";
}

String^ WPFCppCliDemo::toSystemString(std::string& str)
{
  StringBuilder^ pStr = gcnew StringBuilder();
  for (size_t i = 0; i < str.size(); ++i)
    pStr->Append((Char)str[i]);
  return pStr->ToString();
}

void WPFCppCliDemo::addText(String^ msg)
{
  hTextBlock1->Text += msg + "\n";
}

void WPFCppCliDemo::getMessage()
{
  // recvThread runs this function

  while (true)
  {
    std::string msg = pRecvr_->getMessage();
    String^ sMsg = toSystemString(msg);
    array<String^>^ args = gcnew array<String^>(1);
    args[0] = sMsg;

    Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::addText);
    Dispatcher->Invoke(act, args);  // must call addText on main UI thread
  }
}

void WPFCppCliDemo::clear(Object^ sender, RoutedEventArgs^ args)
{
  Console::Write("\n  cleared message text");
  hStatus->Text = "Cleared message";
  hTextBlock1->Text = "";
}


void WPFCppCliDemo::getItemsFromList(Object^ sender, RoutedEventArgs^ args)
{
	int index = 0;
	int count = hListBox->SelectedItems->Count;
	hStatus->Text = "Show Selected Items";
	array<System::String^>^ items = gcnew array<String^>(count);
	if (count > 0) {
		for each (String^ item in hListBox->SelectedItems)
		{
			items[index++] = item;
		}
	}

	hListBox->Items->Clear();
	if (count > 0) {
		for each (String^ item in items)
		{
			hListBox->Items->Add(item);
		}
	}
}

void WPFCppCliDemo::setUpFileListView()
{
  hFileListGrid->Margin = Thickness(20);
  hFileListTab->Content = hFileListGrid;
  RowDefinition^ hRow1Def = gcnew RowDefinition();
  hFileListGrid->RowDefinitions->Add(hRow1Def);
  Border^ hBorder1 = gcnew Border();
  hBorder1->BorderThickness = Thickness(1);
  hBorder1->BorderBrush = Brushes::Black;
  hListBox->SelectionMode = SelectionMode::Multiple;
  hBorder1->Child = hListBox;
  hFileListGrid->SetRow(hBorder1, 0);
  hFileListGrid->Children->Add(hBorder1);

  RowDefinition^ hRow2Def = gcnew RowDefinition();
  hRow2Def->Height = GridLength(75);
  RowDefinition^ hRow2Def2 = gcnew RowDefinition();
  hRow2Def2->Height = GridLength(75);
  hFileListGrid->RowDefinitions->Add(hRow2Def);
  hFileListGrid->RowDefinitions->Add(hRow2Def2);
  hFolderBrowseButton->Content = "Select Directory";
  hFolderBrowseButton->Height = 30;
  hFolderBrowseButton->Width = 120;
  hFolderBrowseButton->BorderThickness = Thickness(2);
  hFolderBrowseButton->BorderBrush = Brushes::Black;
  hFileListGrid->SetRow(hFolderBrowseButton, 1);
  hFileListGrid->Children->Add(hFolderBrowseButton);

  // Show selected items button.
  hShowItemsButton->Content = "Show Selected Items";
  hShowItemsButton->Height = 30;
  hShowItemsButton->Width = 120;
  hShowItemsButton->BorderThickness = Thickness(2);
  hShowItemsButton->BorderBrush = Brushes::Black;
  hFileListGrid->SetRow(hShowItemsButton, 2);
  hFileListGrid->Children->Add(hShowItemsButton);

  hFolderBrowserDialog->ShowNewFolderButton = false;
  hFolderBrowserDialog->SelectedPath = System::IO::Directory::GetCurrentDirectory();
}

void WPFCppCliDemo::browseForFolder(Object^ sender, RoutedEventArgs^ args)
{
  std::cout << "\n  Browsing for folder";
  hListBox->Items->Clear();
  System::Windows::Forms::DialogResult result;
  result = hFolderBrowserDialog->ShowDialog();
  if (result == System::Windows::Forms::DialogResult::OK)
  {
    String^ path = hFolderBrowserDialog->SelectedPath;
    std::cout << "\n  opening folder \"" << toStdString(path) << "\"";
    array<String^>^ files = System::IO::Directory::GetFiles(path, L"*.*");
    for (int i = 0; i < files->Length; ++i)
      hListBox->Items->Add(files[i]);
    array<String^>^ dirs = System::IO::Directory::GetDirectories(path);
    for (int i = 0; i < dirs->Length; ++i)
      hListBox->Items->Add(L"<> " + dirs[i]);
  }
}
void WPFCppCliDemo::setUpConnectionView()
{
}

void WPFCppCliDemo::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
}
void WPFCppCliDemo::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
}

[STAThread]
//int _stdcall WinMain()
int main(array<System::String^>^ args)
{
  Console::WriteLine(L"\n Starting WPFCppCliDemo");

  Application^ app = gcnew Application();
  app->Run(gcnew WPFCppCliDemo());
  Console::WriteLine(L"\n\n");
}