#ifndef BLOCKSTERSESSION_H
#define BLOCKSTERSESSION_H

#include <vector>
#include "programcontainer.h"
#include "config.h"

class BlocksterSession
{
public:
    BlocksterSession();
    QList<ProgramContainer*> openPrograms;
    Config config;

};

#endif // BLOCKSTERSESSION_H
