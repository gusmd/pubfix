#pragma once

#include <QMainWindow>

#include <Windows.h>

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
    void setToCPU0();
    void setToAllCPU();
    void findPID();

private:
    Ui::MainWindow *ui;

    HANDLE getHandle() const;
    HANDLE getHandleFromPID(int PID, DWORD accessRights = PROCESS_ALL_ACCESS) const;
    void setAffinity(HANDLE h, DWORD_PTR mask) const;
};
