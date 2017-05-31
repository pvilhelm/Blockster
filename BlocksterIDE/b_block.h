#ifndef B_BLOCK_H
#define B_BLOCK_H

#include "b_node.h"
#include <vector>

namespace bster{

class b_block : public b_node
{
public:
    b_block();
    virtual ~b_block();

    std::vector<PORT_SIGNAL_TYPES> v_porttypes;

};

}

#endif // B_BLOCK_H
