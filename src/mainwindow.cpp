#include "headers/mainwindow.h"
#include "ui_mainwindow.h"

#include <QFrame>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tabs = new QTabWidget(this);
    tabs->setTabsClosable(true);
    tabs->setMovable(true);
    setCentralWidget(tabs);

    connect(tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_actionNew_file_triggered()
{
    QFrame *tabFrame = new QFrame(this);
    tabs->addTab(tabFrame, "Untitled");
}


void MainWindow::on_actionClose_file_triggered()
{
    tabs->removeTab(tabs->currentIndex());
}

void MainWindow::closeTab(int index){
    tabs->removeTab(index);
}

