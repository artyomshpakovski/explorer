#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileInfoList>
#include "qlistviewexplorermodel.h"
#include <QFile>
#include <QtGui/private/qzipwriter_p.h>
#include <QtGui/private/qzipreader_p.h>
#include <QTableWidgetItem>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QDir currentFolder;
    QFileInfoList *aDirList;
    QListViewExplorerModel *model;
    QTableWidget *twInfo;
    int *sorted = new int[5];

private slots:
    void dialogClose();
    void twInfoSelected(int logicalIndex);
    void on_listView_doubleClicked(const QModelIndex &index);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_lineEdit_editingFinished();
    void on_pbChoice_clicked();
};
#endif // MAINWINDOW_H
