#ifndef B_BLOCK_H
#define B_BLOCK_H

#include "b_node.h"
#include <vector>
#include <string>

namespace bster{

/*struct member{
    std::string name ="";
    std::string value = "";
    SIGNAL_TYPES type = SIGNAL_TYPES::INVALID_TYPE;
    SIGNAL_TYPES effective_type = SIGNAL_TYPES::INVALID_TYPE;
    std::vector<SIGNAL_TYPES> v_allowed_types;
    std::string inherit_type_from = "";

};*/

class b_block : public b_node
{
public:


    b_block();
    virtual ~b_block();

    //std::vector<t_member> v_members;
};

}

#endif // B_BLOCK_H
