#include "headers/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setWindowTitle("Papernote");
    ui->setupUi(this);

    tabs = new QTabWidget(this);
    tabs->setTabsClosable(true);
    tabs->setMovable(true);
    setCentralWidget(window);

    treeview->setMaximumWidth(0);
    treeview->setMidLineWidth(0);

    window->addWidget(treeview);
    window->addWidget(tabs);

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

    QLabel *status = new QLabel(this);
    status->setText("Line 1, Column 1");
    status->setObjectName("status");


    tablayout->addWidget(fileedit);
    tablayout->addWidget(status);

    int tab = tabs->addTab(tabFrame, "Untitled");
    tabs->setCurrentIndex(tab);

    tabs->setTabToolTip(tabs->currentIndex(), "Untitled");

    connect(MainWindow::currentTextEdit(), SIGNAL(textChanged()), this, SLOT(textEditChanged()));
    connect(MainWindow::currentTextEdit(), SIGNAL(cursorPositionChanged()), this, SLOT(StatUpdate()));
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
QLabel* MainWindow::currentStatus(){
    QList<QLabel *> statusList = tabs->findChildren<QLabel *>("status");
    for (int i=0; i<statusList.count(); ++i){
        if (tabs->indexOf(statusList[i]->parentWidget()) == tabs->currentIndex()){
            return statusList[i];
        }
    }
    return new QLabel;
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
    tabs->tabBar()->setTabData(tabs->currentIndex(), filepath);
}

void MainWindow::on_actionOpen_file_triggered(){
    QString filepath = QFileDialog::getOpenFileName(this, "Open file");

    MainWindow::createTab();
    MainWindow::openTabFile(filepath);

}

void MainWindow::textEditChanged(){
    QString tabName = tabs->tabText(tabs->currentIndex());
    if (tabName.at(0) != "*") tabs->setTabText(tabs->currentIndex(), "*"+tabName);
}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = tabs->tabText(tabs->currentIndex()),
            filepath = tabs->tabBar()->tabData(tabs->currentIndex()).toString();

    if (fileName=="*Untitled" || fileName=="Untitled"){
        MainWindow::on_actionSave_as_triggered();
        return;
    }
    if (fileName.startsWith('*')) fileName.remove(0, 1);
    QFile file(filepath);
    if (!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot save file : "+ file.errorString());
        return;
}
    QTextStream out(&file);
    QString text = MainWindow::currentTextEdit()->toPlainText();
    out << text;

    file.close();

    QString newTabText = tabs->tabText(tabs->currentIndex()).remove(0, 1);
    tabs->setTabText(tabs->currentIndex(), newTabText);
}


void MainWindow::on_actionSave_as_triggered()
{
    if (tabs->count() == 0){
        QMessageBox::warning(this, "Warning", "Cannot save file !");
        return;
    }

    QString filepath = QFileDialog::getSaveFileName(this, "Save As ...");
    QFile file(filepath);

    if (!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot save file !"+file.errorString());
        return;
    }

    QTextStream out(&file);
    QString content = MainWindow::currentTextEdit()->toPlainText();
    out << content;
    file.close();

    MainWindow::openTabFile(filepath);
}

void MainWindow::StatUpdate(){
    QString row = QString::number(MainWindow::currentTextEdit()->textCursor().blockNumber()+1);
    QString column = QString::number(MainWindow::currentTextEdit()->textCursor().columnNumber()+1);

    QString newStatus = "Line "+row+",Column "+column;

    MainWindow::currentStatus()->setText(newStatus);
}

void MainWindow::on_actionOpen_folder_triggered()
{
    QUrl dir = QFileDialog::getExistingDirectory(this, "Open folder", "/", QFileDialog::ShowDirsOnly);
    dirModel->setRootPath(dir.toString());
    treeview->setModel(dirModel);
    treeview->setRootIndex(dirModel->index(dir.toString()));

    treeview->setMinimumWidth(width() - 50);
}

