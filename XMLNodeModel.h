#ifndef XMLNODEMODEL_H
#define XMLNODEMODEL_H

#include <QAbstractItemModel>
#include <QDomDocument>
#include <QStyleOptionViewItem>
#include <QAction>

#include "XMLNode.h"

class XMLNodeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    XMLNodeModel(QDomDocument & doc, QObject * parent = nullptr);
    ~XMLNodeModel() override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    QDomDocument  getDoc()
    {
        return m_doc;
    }

    bool removeRows(int row, int count, const QModelIndex &parent) override;
public slots:
    void addItem();
    void removeItem();
private:
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    XMLNode * m_root;
    QDomDocument m_doc;

};

#endif // XMLNODEMODEL_H
