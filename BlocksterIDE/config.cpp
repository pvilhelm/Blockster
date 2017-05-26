#include "config.h"

#include <QFile>
#include <QRegExp>
#include <QTextStream>
#include <QDir>

Config::Config()
{
    confParHashtable = new QHash<QString,QString>;


    QString test = QDir::currentPath();
    QFile file("config.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        throw std::runtime_error("config.txt not found in current directory.");
    }
    QTextStream in(&file);


    lineN = 1;
    while (!in.atEnd()) {
        QString line = in.readLine();
        ParseLine(line);
        lineN++;
    }
}

void Config::ParseLine(QString line)
{
    QRegExp isComment("^\\s*#.*$");
    if(isComment.exactMatch(line))
        return;
    QRegExp isWhite("^\\s*$");
    if(isWhite.exactMatch(line))
        return;

    QRegExp par_val("^\\s*(\\w+)\\s*=\\s*\"([^\"]*)\"");
    if(par_val.indexIn(line) != -1){
        confParHashtable->insert(par_val.cap(1), par_val.cap(2));
    }
    else{
        throw std::runtime_error("config.txt has a syntax error at line: "+std::to_string(lineN));
    }

}


