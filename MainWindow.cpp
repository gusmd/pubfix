#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <bitset>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  connect(ui->singleCoreButton, &QPushButton::released, this, &MainWindow::setToCPU0);
  connect(ui->allCoresButton, &QPushButton::released, this, &MainWindow::setToAllCPU);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::setToCPU0() {
  auto pid = ui->num1Edit->text().toInt();
  auto process = getHandleFromPID(pid);
  DWORD_PTR processAffinityMask;
  DWORD_PTR systemAffinityMask;

  if (!GetProcessAffinityMask(process, &processAffinityMask,
                              &systemAffinityMask)) {
    std::cout
        << "Could not retrieve process affinity mask. Make sure PID is correct."
        << std::endl;
    std::cout << "Error code:" << GetLastError() << std::endl;
    return;
  }

  DWORD_PTR singleCoreMask = 1;

  std::cout << processAffinityMask << std::endl;
  std::cout << systemAffinityMask << std::endl;
  std::cout << "Current affinity mask: " << std::bitset<64>(processAffinityMask)
            << std::endl;
  std::cout << "Setting to: " << std::bitset<64>(singleCoreMask) << std::endl;

  if (!SetProcessAffinityMask(process, singleCoreMask)) {
    std::cout
        << "Could not modify process affinity mask. Make sure PID is correct."
        << std::endl;
    std::cout << "Error code:" << GetLastError() << std::endl;
    return;
  }
  std::cout << "Done!" << std::endl;
}

void MainWindow::setToAllCPU() {
  auto pid = ui->num1Edit->text().toInt();
  auto process = getHandleFromPID(pid);
  DWORD_PTR processAffinityMask;
  DWORD_PTR systemAffinityMask;

  if (!GetProcessAffinityMask(process, &processAffinityMask,
                              &systemAffinityMask)) {
    std::cout
        << "Could not retrieve process affinity mask. Make sure PID is correct."
        << std::endl;
    std::cout << "Error code:" << GetLastError() << std::endl;
    return;
  }

  std::cout << processAffinityMask << std::endl;
  std::cout << systemAffinityMask << std::endl;
  std::cout << "Current affinity mask: " << std::bitset<64>(processAffinityMask)
            << std::endl;
  std::cout << "Setting to: " << std::bitset<64>(systemAffinityMask)
            << std::endl;

  if (!SetProcessAffinityMask(process, systemAffinityMask)) {
    std::cout
        << "Could not modify process affinity mask. Make sure PID is correct."
        << std::endl;
    std::cout << "Error code:" << GetLastError() << std::endl;
    return;
  }
  std::cout << "Done!" << std::endl;
}

HANDLE MainWindow::getHandleFromPID(int PID, DWORD accessRights) {
  return OpenProcess(accessRights, TRUE, PID);
}

void MainWindow::setAffinity(HANDLE h, DWORD_PTR mask) {
  BOOL success = SetProcessAffinityMask(h, mask);
}
