#include "headers/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tabs = new QTabWidget(this);
    tabs->addTab(new QWidget, "Tab1");
    tabs->addTab(new QWidget, "Tab2");
    tabs->setTabsClosable(true);
    tabs->setMovable(true);
    setCentralWidget(tabs);
}

MainWindow::~MainWindow()
{
    delete ui;
}