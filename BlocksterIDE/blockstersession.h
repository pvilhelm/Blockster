#ifndef BLOCKSTERSESSION_H
#define BLOCKSTERSESSION_H

#include <vector>
#include "programcontainer.h"

class BlocksterSession
{
public:
    BlocksterSession();
    QList<ProgramContainer*> openPrograms;


};

#endif // BLOCKSTERSESSION_H
