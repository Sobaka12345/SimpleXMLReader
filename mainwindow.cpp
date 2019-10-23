#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_model(nullptr)
{
    m_ui->setupUi(this);
    connect(m_ui->openFile, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(m_ui->exit, SIGNAL(triggered()), this, SLOT(close()));

    m_ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_ui->treeView, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(onCustomContextMenu(const QPoint &)));

    m_ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    m_ui->treeView->setModel(m_model);
}

MainWindow::~MainWindow()
{
    delete m_model;
    delete m_ui;
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Выбрать XML файл",
                                                    QDir::currentPath(),
                                                    "XML files (*.xml *xsd)");

    if(fileName.isEmpty())
        return;

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox mb;
        mb.setWindowTitle("Ошибка");
        mb.setText("Невозможно открыть файл!");
        mb.exec();
        return;
    }
    m_fileName = fileName;
    m_doc.setContent(file.readAll(), false);
    XMLNodeModel * oldModel = m_model;
    m_model = new XMLNodeModel(m_doc, this);
    m_ui->treeView->setModel(m_model);
    if(oldModel != nullptr)
        delete oldModel;
    m_ui->treeView->resizeColumnToContents(0);
    m_ui->treeView->resizeColumnToContents(1);

    file.close();
}

void MainWindow::on_treeView_pressed(const QModelIndex &index)
{
    m_curNode = static_cast<XMLNode*>(index.internalPointer());

    m_cur = m_curNode->node().toElement();

    m_atts = m_cur.attributes();
    updateTable();
}

void MainWindow::on_tableWidget_cellChanged(int row, int column)
{
    if(column == 0)
        return;
    if(m_ui->tableWidget->item(row, 1)->text() == "" || m_ui->tableWidget->item(row, 0)->text() == "")
        return;
    qDebug() << m_ui->tableWidget->item(row, 0)->text();
    m_cur.setAttribute(m_ui->tableWidget->item(row, 0)->text(),
                                   m_ui->tableWidget->item(row, 1)->text());

    m_curNode->setNode(m_cur);
}

void MainWindow::updateTable()
{
    while(m_ui->tableWidget->rowCount())
        m_ui->tableWidget->removeRow(0);

    for(int i = 0; i < m_atts.size(); i++)
    {
        m_ui->tableWidget->insertRow(i);
        QTableWidgetItem * item = new QTableWidgetItem(m_atts.item(i).nodeName());
        m_ui->tableWidget->setItem(i,
                                 0,
                                 item);
        item = new QTableWidgetItem(m_atts.item(i).nodeValue());
        m_ui->tableWidget->setItem(i,
                                 1,
                                 item);
    }
}

void MainWindow::on_pushButton_clicked()
{
    m_ui->tableWidget->insertRow(m_ui->tableWidget->rowCount());
}

void MainWindow::on_pushButton_2_clicked()
{
    int row = m_ui->tableWidget->selectedItems().first()->row();
    QString name = m_ui->tableWidget->item(row,0)->text();
    m_atts.removeNamedItem(name);
    m_cur.removeAttribute(name);
    m_ui->tableWidget->removeRow(row);
    m_curNode->setNode(m_cur);
}

void MainWindow::onCustomContextMenu(const QPoint & point)
{
    QModelIndex index = m_ui->treeView->indexAt(point);
        if (index.isValid()) {
            QMenu *menu=new QMenu(this);
            menu->addAction(new QAction("Добавить", this));
            menu->addAction(new QAction("Удалить", this));
            QVariant v = qVariantFromValue((void*)new QModelIndex(index));
            menu->actions()[0]->setData(v);
            connect(menu->actions()[0], &QAction::triggered,
                    m_model, &XMLNodeModel::addItem);
            menu->actions()[1]->setData(v);
            connect(menu->actions()[1], &QAction::triggered,
                    m_model, &XMLNodeModel::removeItem);
            menu->exec(m_ui->treeView->viewport()->mapToGlobal(point));
            //m_ui->treeView->setModel(m_model);
        }

}

void MainWindow::on_pushButton_3_clicked()
{
   QFile file(m_fileName);
   file.open(QIODevice::WriteOnly);
   QTextStream outStream(&file);
    outStream << m_model->getDoc().toByteArray();
    file.close();
}
