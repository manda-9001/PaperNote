#include "headers/mainwindow.h"
#include "ui_mainwindow.h"

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

    QFontDatabase::addApplicationFont(":/fonts/Montserrat.ttf");
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
    fileedit->setObjectName("Edit");

    QFont font = fileedit->document()->defaultFont();
    font.setFamily("Montserrat");
    fileedit->setFont(font);
    fileedit->setTabStopDistance(QFontMetrics(fileedit->font()).horizontalAdvance(' ')*4);

    tablayout->addWidget(fileedit);

    int tab = tabs->addTab(tabFrame, "Untitled");
    tabs->setCurrentIndex(tab);
}

QPlainTextEdit* MainWindow::currentTextEdit(){
    QList<QPlainTextEdit *> fileEditList = tabs->findChildren<QPlainTextEdit *>("Edit");
    for (int i=0; i<fileEditList.count(); ++i){
        if (tabs->indexOf(fileEditList[i]->parentWidget()) == tabs->currentIndex()){
            return fileEditList[i];
        }
    }
    return new QPlainTextEdit;
}

void MainWindow::openTabFile(QString filepath){
    QFile file(filepath);
    QFileInfo name(filepath);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", file.errorString());
        return;
    }

    tabs->setTabToolTip(tabs->currentIndex(), filepath);

    QTextStream in(&file);
    QString content = in.readAll();

    MainWindow::currentTextEdit()->setPlainText(content);

    file.close();

    tabs->setTabText(tabs->currentIndex(), name.fileName());
}

void MainWindow::on_actionOpen_file_triggered()
{
    QString filepath = QFileDialog::getOpenFileName(this, "Open file");

    MainWindow::createTab();
    MainWindow::openTabFile(filepath);
}

