#ifndef FINDPARENT_H
#define FINDPARENT_H

#include <QObject>
#include <QString>



template <class T>
T* find_parent(QObject* child, QString parent_name)
{
    T *p = 0;

    if (child)
    {
        QObject *parent = child->parent();
        while (parent)
        {
            p = dynamic_cast<T *>(parent);
            QString name = parent->objectName();
            if (p && (name == parent_name))
                return p;
            parent = parent->parent();
        }
    }

    return p;
}

#endif // FINDPARENT_H
