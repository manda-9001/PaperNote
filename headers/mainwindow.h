#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_actionNew_file_triggered();

    void on_actionClose_file_triggered();

    void closeTab(int index);

    void createTab();

private:
    Ui::MainWindow *ui;

    QTabWidget *tabs;
};
#endif // MAINWINDOW_H
