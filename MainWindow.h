#pragma once

#include <QMainWindow>

#include <Windows.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

public slots:
  void setToCPU0();
  void setToAllCPU();

private:
  Ui::MainWindow *ui;

  HANDLE getHandleFromPID(int PID, DWORD accessRights = PROCESS_ALL_ACCESS);
  void setAffinity(HANDLE h, DWORD_PTR mask);
};
