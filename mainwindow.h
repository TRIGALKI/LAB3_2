#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "Identifier.h"
#include "hashcounter.h"
#include "string.h"

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
    void on_addWorker_clicked();

    void on_tupoBan_clicked();

    void on_getData_clicked();

    void on_delrtAll_clicked();
    void updTable();

    void on_orederButton_clicked();
    void refresh();

    void on_downloadFromFile_clicked();

    void on_downloadToFile_clicked();

private:
    Ui::MainWindow *ui;
    hashCounter<std::string> counter;

};

#endif // MAINWINDOW_H
