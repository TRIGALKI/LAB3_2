#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QString"
#include "QMessageBox"
#include <string>
#include "QFileDialog"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()
            ->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()
            ->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    refresh();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::updTable()
{
    QTableWidgetItem *item;
    ui->tableWidget->setRowCount(0);
    int k = -1;
    ui->tableWidget->setRowCount(counter.capacity);
    for (unsigned i = 0;i<counter.capacity;i++)
    {
        typename std::list<Identifier<std::string>>::iterator it;
        if (!counter.m_hash_table[i].empty())
        {
            for(it = counter.m_hash_table[i].begin();
                it != counter.m_hash_table[i].end();it++)
            {
                k++;
                int ex = 1;
                item = new QTableWidgetItem;
                item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                item->setText(QString::fromStdString((*it).getName()));
                ui->tableWidget->setItem((k*ex), 0, item);
                item = new QTableWidgetItem;
                item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                item->setText(QString::number((*it).getCounter()));
                ui->tableWidget->setItem((k*ex), 1, item);
            }
        }
    }
}

void MainWindow::on_addWorker_clicked()
{
    std::string fio;
    fio = ui->fioOfbaddy->text().toStdString();
    counter.add(fio);
    int num,num1;
    num1 = counter.get(fio).getCounter();
    num = ui->numOfmistakes->text().toInt();
    counter.get(fio).changeCounter(num+num1-1);
    updTable();
    refresh();
}

void MainWindow::on_tupoBan_clicked()
{
    std::string fio;
    fio = ui->fioFired->text().toStdString();
    counter.deletKey(fio);
    updTable();
    refresh();
}

void MainWindow::on_getData_clicked()
{
    std::string fio;
    QString text;
    ui->textBrowser->clear();
    fio = ui->fioCheck->text().toStdString();
    Identifier<std::string> ptr = counter.get(fio);
    if (ptr.getCounter())
        ui->textBrowser->setText(text = text +"\nФИО : "+
                QString::fromStdString(ptr.getName()) + "\nПропуски : " +
                QString::number(ptr.getCounter()));
    else
        QMessageBox::critical(this,
                              "Error message","Похоже что такого сотрудника "
                                              "не существует,возможно он "
                                                "\nбыл уволен ");;
}
void MainWindow::refresh()
{
    ui->delrtAll->setEnabled(false);
    ui->getData->setEnabled(false);
    ui->tupoBan->setEnabled(false);
    ui->orederButton->setEnabled(false);
    for(auto i=counter.m_hash_table.begin();i<counter.m_hash_table.end();i++)
    {
        if ((*i).size() != 0)
        {
            ui->delrtAll->setEnabled(true);
            ui->getData->setEnabled(true);
            ui->tupoBan->setEnabled(true);
            ui->orederButton->setEnabled(true);
            return;
        }
    }
}

void MainWindow::on_delrtAll_clicked()
{
    counter.clearCounter();
    updTable();
    refresh();
}
void MainWindow::on_orederButton_clicked()
{
        std::ofstream fout("list.txt");
        std::list<Identifier<std::string>> FO;
            for (int i = 0;i<int(counter.m_hash_table.capacity());i++)
            {
             if (!(counter.m_hash_table[i].empty()))
             {
                 auto it = counter.m_hash_table[i].begin();
                while(it != counter.m_hash_table[i].end())
                {
                    FO.push_back(*it);
                    it++;
                }
             }
            }
            FO.sort(Identifier<std::string>::identiferComparator());
            FO.resize(3);
            for(auto i = FO.begin();i != FO.end();i++)
            {
                fout << (*i).getName() << std::endl;
                fout << (*i).getCounter() << std::endl;
            }
            fout.close();


}

void MainWindow::on_downloadFromFile_clicked()
{
    QString Qfilestr = QFileDialog::getOpenFileName();;
    std::string filestr = Qfilestr.toStdString();
    counter.operator <<(filestr);
    updTable();
    refresh();
}

void MainWindow::on_downloadToFile_clicked()
{
    QString Qfilestr = QFileDialog::getOpenFileName();;
    std::string filestr = Qfilestr.toStdString();
    counter.operator >>(filestr);
    updTable();
    refresh();
}
