#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDomDocument>
#include <QStandardItemModel>
#include <QTableWidgetItem>

#include <QDebug>

#include "XMLNodeModel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void openFile();

private slots:
    void on_treeView_pressed(const QModelIndex &index);

    void on_tableWidget_cellChanged(int row, int column);


    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void onCustomContextMenu(const QPoint &);

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *m_ui;
    QDomNamedNodeMap m_atts;
    QDomElement m_cur;
    XMLNodeModel *m_model;
    XMLNode * m_curNode;
    QDomDocument m_doc;
    QString m_fileName;

    void updateTable();
};

#endif // MAINWINDOW_H
