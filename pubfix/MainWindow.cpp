#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMessageBox>
#include <QSettings>
#include <QTimer>
#include <QtGlobal>

#include <QHotkey>

#include <tlhelp32.h>

#include <bitset>
#include <iostream>

static constexpr char *single_key = "single_key";
static constexpr char *all_key = "all_key";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_timer(new QTimer(this))
{
    ui->setupUi(this);

    for (auto &button : { ui->allCoresButton, ui->singleCoreButton, ui->findButton }) {
        button->setEnabled(false);
    }

    // Initialize hotkeys to default or stored values
    QSettings settings("gusmd", "pubfix");
    const QKeySequence singleKey = QKeySequence::fromString(
        settings.value(single_key, ui->singleCoreKeyEdit->keySequence().toString()).toString());
    const QKeySequence allKey = QKeySequence::fromString(
        settings.value(all_key, ui->allCoresKeyEdit->keySequence().toString()).toString());

    // Set value on fields
    ui->singleCoreKeyEdit->setKeySequence(singleKey);
    ui->allCoresKeyEdit->setKeySequence(allKey);

    // Create QHotkey objects
    m_singleHotkey
        = new QHotkey(singleKey, ui->hotkeyGroupBox->isChecked(), QApplication::instance());
    m_allHotkey = new QHotkey(allKey, ui->hotkeyGroupBox->isChecked(), QApplication::instance());

    connect(ui->singleCoreKeyEdit, &QKeySequenceEdit::keySequenceChanged, this,
            &MainWindow::updateSingleCoreSequence);
    connect(ui->allCoresKeyEdit, &QKeySequenceEdit::keySequenceChanged, this,
            &MainWindow::updateAllCoresSequence);

    connect(ui->hotkeyGroupBox, &QGroupBox::toggled, this, &MainWindow::updateHotkeysState);

    connect(m_singleHotkey, &QHotkey::activated, this, &MainWindow::setToCPU0);
    connect(m_allHotkey, &QHotkey::activated, this, &MainWindow::setToAllCPU);

    connect(ui->singleCoreButton, &QPushButton::released, this, &MainWindow::setToCPU0);
    connect(ui->allCoresButton, &QPushButton::released, this, &MainWindow::setToAllCPU);

    connect(m_timer, &QTimer::timeout, this, &MainWindow::findPID);
    connect(ui->findButton, &QPushButton::released, this, &MainWindow::toggleMonitor);

    connect(ui->exeEdit, &QLineEdit::textChanged, this, &MainWindow::updateExeButtonState);
    connect(ui->pidSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this,
            &MainWindow::updateCoreButtonsState);

    ui->exeEdit->setText("TslGame.exe");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::toggleMonitor()
{
    if (m_timer->isActive()) {
        m_timer->stop();
        ui->findButton->setText("Start Monitoring");
        ui->statusLabel->setText("Not Monitoring");
    } else {
        m_timer->start(1000);
        ui->findButton->setText("Stop Monitoring");
    }
}

void MainWindow::setToCPU0()
{
    if (ui->pidSpinBox->value() == 0) {
        std::cout << "Invalid PID." << std::endl;
        return;
    }

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
    if (ui->pidSpinBox->value() == 0) {
        std::cout << "Invalid PID." << std::endl;
        return;
    }

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

                ui->pidSpinBox->setValue(pid);
                ui->statusLabel->setText("Found!");

                found = true;
            }
        }
    }

    if (!found) {
        ui->statusLabel->setText("Monitoring...");
        ui->pidSpinBox->setValue(0);
    }

    CloseHandle(snapshot);
}

void MainWindow::updateExeButtonState(const QString &text)
{
    ui->findButton->setEnabled(!text.isEmpty());
}

void MainWindow::updateCoreButtonsState(int value)
{
    ui->allCoresButton->setEnabled(value != 0);
    ui->singleCoreButton->setEnabled(value != 0);
}

void MainWindow::updateHotkeysState(bool checked)
{
    for (auto hotkey : { m_singleHotkey, m_allHotkey }) {
        hotkey->setRegistered(checked);
    }
}

void MainWindow::updateSingleCoreSequence(const QKeySequence &sequence)
{
    // Store new sequence in settings
    QSettings("gusmd", "pubfix").setValue(single_key, sequence.toString());
    m_singleHotkey->setShortcut(sequence, ui->hotkeyGroupBox->isChecked());
}

void MainWindow::updateAllCoresSequence(const QKeySequence &sequence)
{
    // Store new sequence in settings
    QSettings("gusmd", "pubfix").setValue(all_key, sequence.toString());
    m_allHotkey->setShortcut(sequence, ui->hotkeyGroupBox->isChecked());
}

HANDLE MainWindow::getHandle() const
{
    return getHandleFromPID(ui->pidSpinBox->value());
}

HANDLE MainWindow::getHandleFromPID(int PID, DWORD accessRights) const
{
    return OpenProcess(accessRights, TRUE, PID);
}

void MainWindow::setAffinity(HANDLE h, DWORD_PTR mask) const
{
    BOOL success = SetProcessAffinityMask(h, mask);
}
