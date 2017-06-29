#include "config.h"

#include <QFile>
#include <QRegExp>
#include <QTextStream>
#include <QDir>
#include <QDebug>

Config::Config()
{
    confParHashtable = new QHash<QString,QString>;


    QString current_dir = QDir::currentPath();

    QFile file("config.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        throw std::runtime_error("config.txt not found in current directory "+current_dir.toStdString());
    }

    try{
        QTextStream in(&file);
        lineN = 1;
        while (!in.atEnd()) {
            QString line = in.readLine();
            ParseLine(line);
            lineN++;
        }
    }
    catch(std::exception e){
        qDebug()<< "config.txt error ..." << __LINE__ <<":"<< __FILE__ ;
        file.close();
        throw e;
    }
    file.close();
}

void Config::ParseLine(QString line)
{
    QRegExp is_comment("^\\s*#.*$");
    if(is_comment.exactMatch(line))
        return;
    QRegExp is_white("^\\s*$");
    if(is_white.exactMatch(line))
        return;

    QRegExp par_val("^\\s*(\\w+)\\s*=\\s*\"([^\"]*)\"");
    if(par_val.indexIn(line) != -1){
        confParHashtable->insert(par_val.cap(1), par_val.cap(2));
    }
    else{
        throw std::runtime_error("config.txt has a syntax error at line: "+std::to_string(lineN));
    }

}
