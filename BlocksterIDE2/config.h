#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>

class Config
{
public:
    Config();


    QHash<QString, QString>* confParHashtable;

private:
    void ParseLine(QString line);
    int lineN = 0;
};

#endif // CONFIG_H
