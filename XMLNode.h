#ifndef XMLNODE_H
#define XMLNODE_H

#include <QVector>
#include <QVariant>
#include <QDomNamedNodeMap>

class XMLNode
{

public:
    XMLNode(const QDomNode &node, int row, XMLNode *parent = nullptr);
    ~XMLNode();
    XMLNode *child(int i);
    XMLNode *parent();
    QDomNode node() const;
    void setNode(QDomNode node);
    int row() const;

    void setNodeText(QString text);
    void addChild();
    bool insertChild(int row);
private:
    QDomNode domNode;
    QHash<int, XMLNode *> childItems;
    XMLNode *parentItem;
    int rowNumber;
};

#endif // XMLNODE_H
