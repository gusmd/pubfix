#pragma once

#include <QMainWindow>

#include <Windows.h>

class QTimer;

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

private:
    Ui::MainWindow *ui;

    QTimer *m_timer;

    HANDLE getHandle() const;
    HANDLE getHandleFromPID(int PID, DWORD accessRights = PROCESS_ALL_ACCESS) const;
    void setAffinity(HANDLE h, DWORD_PTR mask) const;
};
