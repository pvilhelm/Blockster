#ifndef BLOCKSTER_SESSION_H
#define BLOCKSTER_SESSION_H

#include <QObject>
#include <QWidget>

#include <vector>
#include <string>
#include <map>
#include <memory>

#include "lib_container.h"
#include "config.h"

class BlocksterSession : public QWidget
{
public:
    BlocksterSession();

    void addLibBrowser();
    void addProgram();
    void addProgram(std::string path_to_xml);
    Config config;
};

#endif // BLOCKSTER_SESSION_H
