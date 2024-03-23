#include "mainwindow.h"
#include "lexer.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->output->setReadOnly(true);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_input_textChanged() {
  ui->output->clear();
  auto text = ui->input->toPlainText();
  auto result = lex(text);
  auto p = parse::Parser(result);
  connect(&p, &parse::Parser::error, this, &MainWindow::display);
  auto e = p.parse();
  ui->output->append(e ? e->evaluate().toString() : QString("Error!"));
}
void MainWindow::display(const QString &string) { ui->output->append(string); }
