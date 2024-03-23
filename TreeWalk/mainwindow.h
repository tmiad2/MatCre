#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Parser.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

public slots:
  void display(const QString &string);

private slots:

  void on_input_textChanged();

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
