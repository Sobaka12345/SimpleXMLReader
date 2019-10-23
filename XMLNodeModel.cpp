#include "XMLNodeModel.h"

XMLNodeModel::XMLNodeModel(QDomDocument & doc, QObject * parent) :
    QAbstractItemModel(parent),
    m_root(new XMLNode(doc,0)),
    m_doc(doc)
{
   // m_root = new XMLNode(doc.attributes(), nullptr);
}

XMLNodeModel::~XMLNodeModel()
{
    delete m_root;
}

int XMLNodeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

bool XMLNodeModel::insertRows(int row, int count, const QModelIndex &parent)
{
    XMLNode* node =(XMLNode*)parent.internalPointer();
    QDomElement el = node->node().toElement();
    el.appendChild(QDomNode());
    beginInsertRows(parent,row, row + count - 1);
    //node->addChild(QDomElement());
    node->setNode(el);
    endInsertRows();
    emit dataChanged(QModelIndex(),QModelIndex());
    emit layoutChanged();
    return true;
}

bool XMLNodeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    XMLNode* node =(XMLNode*)parent.internalPointer();
    XMLNode * par = (XMLNode*)parent.parent().internalPointer();
    QDomElement el = par->node().toElement();
    el.removeChild(node->node());
    beginRemoveRows(parent,row,row);
    //par->setNode(el);
    auto p = node->node().parentNode();
    p.removeChild(node->node());
    endRemoveRows();
    emit dataChanged(QModelIndex(),QModelIndex());
    emit layoutChanged();

    return true;
}

void XMLNodeModel::addItem()
{
    QAction *act = qobject_cast<QAction *>(sender());
    QVariant v = act->data();
    QModelIndex * index = (QModelIndex*)v.value<void *>();
    XMLNode * node = (XMLNode*)index->internalPointer();
    QDomNode el =node->node().toElement();
    insertRow(1,*index);
    //removeRows(0,1,*index);
    //removeRow(1, *index);
    QDomElement child;
    child.setTagName("EMPTY");
    el.appendChild(child);
    node->setNode(el);
    delete index;
}
#include <QDebug>
void XMLNodeModel::removeItem()
{
    QAction *act = qobject_cast<QAction *>(sender());
    QVariant v = act->data();
    QModelIndex * index = (QModelIndex*)v.value<void *>();
    XMLNode * node = (XMLNode*)index->internalPointer();
    QDomNode el = node->node().toElement();
    removeRows(index->row(),1, index->parent());
    delete index;
}

QVariant XMLNodeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    const XMLNode *item = static_cast<XMLNode*>(index.internalPointer());

    const QDomNode node = item->node();


    switch (index.column()) {
        case 0:
            return node.nodeName();
        case 1:
            return node.nodeValue().split('\n').join(' ');
        default:
            break;
    }
    return QVariant();
}

Qt::ItemFlags XMLNodeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QVariant XMLNodeModel::headerData(int section, Qt::Orientation orientation,
                              int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case 0:
                return tr("Name");
            case 1:
                return tr("Value");
            default:
                break;
        }
    }
    return QVariant();
}

QModelIndex XMLNodeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    XMLNode *parentItem;

    if (!parent.isValid())
        parentItem = m_root;
    else
        parentItem = static_cast<XMLNode*>(parent.internalPointer());

    XMLNode *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex XMLNodeModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    XMLNode *childItem = static_cast<XMLNode*>(child.internalPointer());
    XMLNode *parentItem = childItem->parent();

    if (!parentItem || parentItem == m_root)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int XMLNodeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    XMLNode *parentItem;

    if (!parent.isValid())
        parentItem = m_root;
    else
        parentItem = static_cast<XMLNode*>(parent.internalPointer());

    return parentItem->node().childNodes().count();
}
#include <QDebug>
bool XMLNodeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int column = index.column();
    if (column < 0 || column >= 3)
        return false;

    XMLNode * n = static_cast<XMLNode*>(index.internalPointer());
    QDomElement el = n->node().toElement();

    if(column == 1)
        n->setNodeText(value.toString());
    else {
        el.setTagName(value.toString());
        n->setNode(el);
    }

    return true;
}
