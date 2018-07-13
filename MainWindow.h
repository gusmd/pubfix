#pragma once

#include <QMainWindow>

#include <Windows.h>

class QTimer;
class QHotkey;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void toggleMonitor();
    void setToCPU0();
    void setToAllCPU();
    void findPID();

    void updateExeButtonState(const QString &text);
    void updateCoreButtonsState(int value);

    void updateHotkeysState(bool checked);
    void updateSingleCoreSequence(const QKeySequence &sequence);
    void updateAllCoresSequence(const QKeySequence &sequence);

private:
    Ui::MainWindow *ui;

    QTimer *m_timer;
    QHotkey *m_singleHotkey;
    QHotkey *m_allCoresHotkey;

    HANDLE getHandle() const;
    HANDLE getHandleFromPID(int PID, DWORD accessRights = PROCESS_ALL_ACCESS) const;
    void setAffinity(HANDLE h, DWORD_PTR mask) const;
};
