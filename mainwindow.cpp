#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QImageWriter>
#include <QMessageBox>
#include <QFileDialog>
#include <QTableWidget>
#include <QHeaderView>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->aDirList = new QFileInfoList();
    this->model = new QListViewExplorerModel();
    this->currentFolder = QDir(QDir::currentPath());
    this->model->getFolderList(this->currentFolder.absolutePath(),this->aDirList);
    this->ui->listView->setModel(model);
    this->ui->lineEdit->setText(QDir::currentPath());
    QPixmap icon(":/img/folderup.png");
    ui->pushButton->setIcon(icon);
    QPixmap icon2(":/img/list.png");
    ui->pushButton_2->setIcon(icon2);
    QPixmap icon3(":/img/iconlist.png");
    QPixmap icon4(":/img/choice.png");
    ui->pushButton_3->setIcon(icon3);
    ui->pbChoice->setIcon(icon4);
    this->ui->listView->setWordWrap(true);
    setMinimumSize(800,600);
    setMaximumSize(800,600);
    ui->setRes->setAlignment(Qt::AlignLeft);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dialogClose()
{
    this->show();
}


void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    if (this->aDirList->at(index.row()).isDir())
        {
            QString tmp = this->aDirList->at(index.row()).absoluteFilePath();
            model->getFolderList(this->aDirList->at(index.row()).absoluteFilePath(),this->aDirList);
            this->ui->lineEdit->setText(tmp);
        } else {
//            QDesktopServices::openUrl(QUrl(this->aDirList->at(index.row()).absoluteFilePath()));
            QString str = this->aDirList->at(index.row()).absoluteFilePath();
            QString resolution = "";
            if (str.lastIndexOf('.') == -1) {
                return;
            }
            for(int i = str.lastIndexOf('.'); i < str.size(); i++) {
                resolution.append(str[i]);
            }
            if (resolution != ".jpg" && resolution != ".gif" && resolution != ".tif" && resolution != ".bmp" &&
                    resolution != ".png" && resolution != ".pcx" && resolution != ".BMP") {
                return;
            }
            ui->setRes->setText(resolution);
            QString fileName = "";
            for(int i = str.lastIndexOf('/') + 1; i < str.lastIndexOf('.'); i++) {
                fileName.append(str[i]);
            }
            ui->setName->setText(fileName);
            QImage im(str);
            ui->setName->setWordWrap(false);
            ui->setSize->setText(QString::number(im.size().width())+"x"+QString::number(im.size().height()));
//            ui->setDepth->setText(QString::number(im.depth()));
            ui->setDepth->setText(QString::number(im.bitPlaneCount()));
            QImageWriter a(str);
            ui->setCom->setText(QString::number(a.compression()));

        }
}


void MainWindow::on_pushButton_clicked()
{
    currentFolder.cdUp();
    this->ui->lineEdit->setText(currentFolder.absolutePath());
    model->getFolderList(currentFolder.absolutePath(),this->aDirList);
}

void MainWindow::on_pushButton_3_clicked()
{
    this->ui->listView->setUniformItemSizes(false);
    this->ui->listView->setViewMode(QListView::ListMode);

}

void MainWindow::on_pushButton_2_clicked()
{
    this->ui->listView->setUniformItemSizes(true);
    this->ui->listView->setViewMode(QListView::IconMode);
}

void MainWindow::on_lineEdit_editingFinished()
{
    QDir temp(ui->lineEdit->text());
    if (temp.exists()) {
        model->getFolderList(ui->lineEdit->text(), aDirList);
    }
    else {
        ui->lineEdit->setText(currentFolder.absolutePath());
    }
}

void MainWindow::on_pbChoice_clicked()
{
    QFileDialog *files = new QFileDialog;
    files->setWindowTitle("Откройте файлы");
    QList<QUrl> list = files->getOpenFileUrls();
    QDialog *table = new QDialog();
    table->setWindowTitle("Explorer");
    table->setWindowIcon(QPixmap(":/img/folder.png"));
    QGridLayout *tableLayout = new QGridLayout(table);
    table->setMinimumSize(700,700);
    QTableWidget *twInfo = new QTableWidget(table);
    tableLayout->addWidget(twInfo);
    twInfo->setColumnCount(5);
    twInfo->setEditTriggers(0);
    twInfo->setRowCount(list.size());
    twInfo->setHorizontalHeaderItem(0, new QTableWidgetItem("Имя"));
    twInfo->setHorizontalHeaderItem(1, new QTableWidgetItem("Размер"));
    twInfo->setHorizontalHeaderItem(2, new QTableWidgetItem("Расширение"));
    twInfo->setHorizontalHeaderItem(3, new QTableWidgetItem("Глубина цвета"));
    twInfo->setHorizontalHeaderItem(4, new QTableWidgetItem("Сжатие"));
    twInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for(int i = 0; i < list.size(); i++) {
        QFile temp(list[i].toLocalFile());
        QString resolution = "";
        if (temp.fileName().lastIndexOf('.') == -1) {
            twInfo->setRowCount(twInfo->rowCount()-1);
            continue;
        }
        for(int i = temp.fileName().lastIndexOf('.'); i < temp.fileName().size(); i++) {
            resolution.append(temp.fileName()[i]);
        }
        if (resolution != ".jpg" && resolution != ".gif" && resolution != ".tif" && resolution != ".bmp" &&
                resolution != ".png" && resolution != ".pcx" && resolution != ".BMP") {
            twInfo->setRowCount(twInfo->rowCount()-1);
            continue;
        }
        twInfo->setItem(i, 2, new QTableWidgetItem(resolution));
        QString fileName = "";
        for(int i = temp.fileName().lastIndexOf('/') + 1; i < temp.fileName().lastIndexOf('.'); i++) {
            fileName.append(temp.fileName()[i]);
        }
        twInfo->setItem(i,0,new QTableWidgetItem(fileName));
        QImage im(list[i].toLocalFile());
        QImageWriter a(list[i].toLocalFile());
        twInfo->setItem(i, 1, new QTableWidgetItem(QString::number(im.size().width())+"x"+QString::number(im.size().height())));
        twInfo->setItem(i, 3, new QTableWidgetItem(QString::number(im.bitPlaneCount())));
        twInfo->setItem(i, 4, new QTableWidgetItem(QString::number(a.compression())));
    }
    if (twInfo->rowCount() == 0) {
        return;
    }
    table->show();

    connect(table, SIGNAL(rejected()), this, SLOT(dialogClose()));

    this->hide();
}
