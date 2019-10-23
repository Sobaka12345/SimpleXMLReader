#include "XMLNode.h"
XMLNode::XMLNode(const QDomNode &node, int row, XMLNode *parent)
    : domNode(node),
      parentItem(parent),
      rowNumber(row)
{}

XMLNode::~XMLNode()
{
    qDeleteAll(childItems);
}

QDomNode XMLNode::node() const
{
    return domNode;
}

void XMLNode::setNode(QDomNode node)
{
    domNode = node;
}

void XMLNode::setNodeText(QString text)
{
    domNode.setNodeValue(text);
}


XMLNode *XMLNode::parent()
{
    return parentItem;
}

XMLNode *XMLNode::child(int i)
{
    XMLNode *childItem = childItems.value(i);
    if (childItem)
        return childItem;

    if (i >= 0 && i < domNode.childNodes().count()) {
        QDomNode childNode = domNode.childNodes().item(i);
        childItem = new XMLNode(childNode, i, this);
        childItems[i] = childItem;
    }

    return childItem;
}

void XMLNode::addChild()
{
    domNode.insertAfter(QDomNode(), domNode.lastChild());
}

// DomItem.cpp

bool XMLNode::insertChild(int row)
{
    if(row < 0 || row > childItems.size())
    {
        return false;
    }

    QDomNode node;
    XMLNode *item = new XMLNode(node, row, this);
    childItems.insert(row, item);

    return true;
}

int XMLNode::row() const
{
    return rowNumber;
}
