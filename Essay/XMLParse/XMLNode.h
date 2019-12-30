#pragma once   // XMLNODE_H

#include <QMap>

class XMLNode
{
public:
    XMLNode();

private:
    QMap<QString, QString>   mAttributes;
    QMap<QString, XMLNode *> mChildNodes;
    QString                  mText;
};
