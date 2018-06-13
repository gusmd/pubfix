#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMessageBox>

#include <tlhelp32.h>

#include <bitset>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->exeEdit->setText("TslGame.exe");

    connect(ui->singleCoreButton, &QPushButton::released, this, &MainWindow::setToCPU0);
    connect(ui->allCoresButton, &QPushButton::released, this, &MainWindow::setToAllCPU);
    connect(ui->findButton, &QPushButton::released, this, &MainWindow::findPID);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setToCPU0()
{
    auto process = getHandle();
    DWORD_PTR processAffinityMask;
    DWORD_PTR systemAffinityMask;

    if (!GetProcessAffinityMask(process, &processAffinityMask, &systemAffinityMask)) {
        std::cout << "Could not retrieve process affinity mask. Make sure PID is correct."
                  << std::endl;
        std::cout << "Error code:" << GetLastError() << std::endl;
        return;
    }

    DWORD_PTR singleCoreMask = 1;

    std::cout << processAffinityMask << std::endl;
    std::cout << systemAffinityMask << std::endl;
    std::cout << "Current affinity mask: " << std::bitset<64>(processAffinityMask) << std::endl;
    std::cout << "Setting to: " << std::bitset<64>(singleCoreMask) << std::endl;

    if (!SetProcessAffinityMask(process, singleCoreMask)) {
        std::cout << "Could not modify process affinity mask. Make sure PID is correct."
                  << std::endl;
        std::cout << "Error code:" << GetLastError() << std::endl;
        return;
    }
    std::cout << "Done!" << std::endl;
}

void MainWindow::setToAllCPU()
{
    auto process = getHandle();
    DWORD_PTR processAffinityMask;
    DWORD_PTR systemAffinityMask;

    if (!GetProcessAffinityMask(process, &processAffinityMask, &systemAffinityMask)) {
        std::cout << "Could not retrieve process affinity mask. Make sure PID is correct."
                  << std::endl;
        std::cout << "Error code:" << GetLastError() << std::endl;
        return;
    }

    std::cout << processAffinityMask << std::endl;
    std::cout << systemAffinityMask << std::endl;
    std::cout << "Current affinity mask: " << std::bitset<64>(processAffinityMask) << std::endl;
    std::cout << "Setting to: " << std::bitset<64>(systemAffinityMask) << std::endl;

    if (!SetProcessAffinityMask(process, systemAffinityMask)) {
        std::cout << "Could not modify process affinity mask. Make sure PID is correct."
                  << std::endl;
        std::cout << "Error code:" << GetLastError() << std::endl;
        return;
    }
    std::cout << "Done!" << std::endl;
}

void MainWindow::findPID()
{
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    QString exeName = ui->exeEdit->text();

    bool found = false;
    if (Process32First(snapshot, &entry) == TRUE) {
        while (Process32Next(snapshot, &entry) == TRUE) {
            if (QString::compare(QString(entry.szExeFile), exeName, Qt::CaseInsensitive) == 0) {
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
                auto pid = entry.th32ProcessID;
                CloseHandle(hProcess);

                ui->pidEdit->setText(QString::number(pid));

                found = true;
            }
        }
    }

    if (!found) {
        QMessageBox::critical(this, "Find process PID",
                              "Could not find a process with the provided name.");
    }

    CloseHandle(snapshot);
}

HANDLE MainWindow::getHandle() const
{
    return getHandleFromPID(ui->pidEdit->text().toInt());
}

HANDLE MainWindow::getHandleFromPID(int PID, DWORD accessRights) const
{
    return OpenProcess(accessRights, TRUE, PID);
}

void MainWindow::setAffinity(HANDLE h, DWORD_PTR mask) const
{
    BOOL success = SetProcessAffinityMask(h, mask);
}
