#include "headers/mainwindow.h"
#include "ui_mainwindow.h"

#include <QFrame>
#include <QVBoxLayout>
#include <QPlainTextEdit>

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

    QFontDatabase::addApplicationFont(":/fonts/Gilroy-Regular.ttf");
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_actionNew_file_triggered()
{
    MainWindow::createTab();
}


void MainWindow::on_actionClose_file_triggered()
{
    tabs->removeTab(tabs->currentIndex());
}

void MainWindow::closeTab(int index){
    tabs->removeTab(index);
}

void MainWindow::createTab(){
    QFrame *tabFrame = new QFrame(this);
    QVBoxLayout *tablayout = new QVBoxLayout(tabFrame);
    QPlainTextEdit *fileedit = new QPlainTextEdit();

    QFont font = fileedit->document()->defaultFont();
    font.setFamily("Gilroy-Rgular");
    fileedit->setFont(font);
    fileedit->setTabStopDistance(QFontMetrics(fileedit->font()).horizontalAdvance(' ')*4);

    tablayout->addWidget(fileedit);

    int tab = tabs->addTab(tabFrame, "Untitled");
    tabs->setCurrentIndex(tab);
}
