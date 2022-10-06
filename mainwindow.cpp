#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
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
    ui->pushButton_3->setIcon(icon3);
    this->ui->listView->setWordWrap(true);
}

MainWindow::~MainWindow()
{
    delete ui;
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
            QFile file(str);
            QString resolution = "";
            if (str.lastIndexOf('.') == -1) {
                return;
            }
            for(int i = str.lastIndexOf('.'); i < str.size(); i++) {
                resolution.append(str[i]);
            }
            if (resolution != ".jpg" && resolution != ".gif" && resolution != ".tif" && resolution != ".bmp" &&
                    resolution != "png" && resolution != ".pcx" && resolution != ".BMP") {
                return;
            }
            ui->setRes->setText(resolution);
            QString fileName = "";
            for(int i = str.lastIndexOf('/') + 1; i < str.lastIndexOf('.'); i++) {
                fileName.append(str[i]);
            }
            ui->setName->setText(fileName);
            QImage im(str);
            QSize si = im.size();
            int height = si.height();
            int width = si.width();
            ui->setSize->setText(QString::number(width)+"x"+QString::number(height));
            ui->setDepth->setText(QString::number(im.depth()));


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
