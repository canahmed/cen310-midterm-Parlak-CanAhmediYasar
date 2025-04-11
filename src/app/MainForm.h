#pragma once

// Windows Forms için gerekli başlıklar
#include <Windows.h>

// C++/CLI için gerekli tanımlamalar
#ifdef _MANAGED
  #pragma managed(push, off)
#endif

#include "../lib/monte_carlo_pi.h"

#ifdef _MANAGED
  #pragma managed(pop)
#endif

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Text;
using namespace System::Data;

namespace monte_carlo_pi_app {

public ref class MainForm : public Form {
 public:
  MainForm(void) {
    InitializeComponent();
  }

 protected:
  ~MainForm() {
    if (components) {
      delete components;
    }
  }

 private:
  System::ComponentModel::Container ^components;
  NumericUpDown ^numPoints;
  NumericUpDown ^numThreads;
  Button ^calculateButton;
  Label ^resultLabel;
  Label ^pointsLabel;
  Label ^threadsLabel;
  ProgressBar ^progressBar;
  System::Drawing::Font ^defaultFont;
  BackgroundWorker ^backgroundWorker;

  void InitializeComponent(void) {
    // Varsayılan font ayarı
    this->defaultFont = gcnew System::Drawing::Font("Segoe UI", 9);
    this->numPoints = (gcnew NumericUpDown());
    this->numThreads = (gcnew NumericUpDown());
    this->calculateButton = (gcnew Button());
    this->resultLabel = (gcnew Label());
    this->pointsLabel = (gcnew Label());
    this->threadsLabel = (gcnew Label());
    this->progressBar = (gcnew ProgressBar());
    this->backgroundWorker = (gcnew BackgroundWorker());
    this->SuspendLayout();
    // pointsLabel
    this->pointsLabel->AutoSize = true;
    this->pointsLabel->Location = Drawing::Point(12, 15);
    this->pointsLabel->Name = L"pointsLabel";
    this->pointsLabel->Size = Drawing::Size(100, 13);
    this->pointsLabel->TabIndex = 0;
    this->pointsLabel->Text = L"Number of Points:";
    this->pointsLabel->Font = defaultFont;
    // numPoints
    this->numPoints->Location = Drawing::Point(118, 13);
    this->numPoints->Maximum = Decimal(1000000000);
    this->numPoints->Minimum = Decimal(1000);
    this->numPoints->Name = L"numPoints";
    this->numPoints->Size = Drawing::Size(120, 20);
    this->numPoints->TabIndex = 1;
    this->numPoints->Value = Decimal(1000000);
    this->numPoints->Font = defaultFont;
    // threadsLabel
    this->threadsLabel->AutoSize = true;
    this->threadsLabel->Location = Drawing::Point(12, 41);
    this->threadsLabel->Name = L"threadsLabel";
    this->threadsLabel->Size = Drawing::Size(100, 13);
    this->threadsLabel->TabIndex = 2;
    this->threadsLabel->Text = L"Number of Threads:";
    this->threadsLabel->Font = defaultFont;
    // numThreads
    this->numThreads->Location = Drawing::Point(118, 39);
    this->numThreads->Maximum = Decimal(64);
    this->numThreads->Minimum = Decimal(1);
    this->numThreads->Name = L"numThreads";
    this->numThreads->Size = Drawing::Size(120, 20);
    this->numThreads->TabIndex = 3;
    this->numThreads->Value = Decimal(4);
    this->numThreads->Font = defaultFont;
    // calculateButton
    this->calculateButton->Location = Drawing::Point(118, 65);
    this->calculateButton->Name = L"calculateButton";
    this->calculateButton->Size = Drawing::Size(120, 23);
    this->calculateButton->TabIndex = 4;
    this->calculateButton->Text = L"Calculate";
    this->calculateButton->Font = defaultFont;
    this->calculateButton->Click += gcnew EventHandler(this, &MainForm::calculateButton_Click);
    // resultLabel
    this->resultLabel->AutoSize = true;
    this->resultLabel->Location = Drawing::Point(12, 100);
    this->resultLabel->Name = L"resultLabel";
    this->resultLabel->Size = Drawing::Size(0, 13);
    this->resultLabel->TabIndex = 5;
    this->resultLabel->Font = defaultFont;
    // progressBar
    this->progressBar->Location = Drawing::Point(12, 120);
    this->progressBar->Name = L"progressBar";
    this->progressBar->Size = Drawing::Size(226, 23);
    this->progressBar->TabIndex = 6;
    this->progressBar->Visible = false;
    // MainForm
    this->AutoScaleDimensions = Drawing::SizeF(6, 13);
    this->AutoScaleMode = Windows::Forms::AutoScaleMode::Font;
    this->ClientSize = Drawing::Size(250, 160);
    this->Controls->Add(this->progressBar);
    this->Controls->Add(this->resultLabel);
    this->Controls->Add(this->calculateButton);
    this->Controls->Add(this->numThreads);
    this->Controls->Add(this->threadsLabel);
    this->Controls->Add(this->numPoints);
    this->Controls->Add(this->pointsLabel);
    this->FormBorderStyle = Windows::Forms::FormBorderStyle::FixedSingle;
    this->MaximizeBox = false;
    this->Name = L"MainForm";
    this->Text = L"Monte Carlo Pi Calculator";
    this->Font = defaultFont;
    this->ResumeLayout(false);
    this->PerformLayout();
  }

  void calculateButton_Click(Object ^ /*sender*/, EventArgs ^ /*e*/) {
    calculateButton->Enabled = false;
    progressBar->Visible = true;
    progressBar->Style = ProgressBarStyle::Marquee;
    backgroundWorker->DoWork += gcnew DoWorkEventHandler(this, &MainForm::CalculatePi);
    backgroundWorker->RunWorkerCompleted += gcnew RunWorkerCompletedEventHandler(this, &MainForm::CalculationComplete);
    backgroundWorker->RunWorkerAsync();
  }

  void CalculatePi(Object ^ /*sender*/, DoWorkEventArgs ^e) {
    try {
      long long points = Decimal::ToInt64(numPoints->Value);
      int threads = Decimal::ToInt32(numThreads->Value);
      double pi = monte_carlo_pi::calculate_pi_parallel(points, threads);
      e->Result = pi;
    } catch (Exception ^ex) {
      MessageBox::Show(ex->Message, L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
      e->Result = 0.0;
    }
  }

  void CalculationComplete(Object ^ /*sender*/, RunWorkerCompletedEventArgs ^e) {
    double pi = safe_cast<double>(e->Result);
    resultLabel->Text = String::Format(L"PI = {0:F6}", pi);
    calculateButton->Enabled = true;
    progressBar->Visible = false;
  }
};
}
