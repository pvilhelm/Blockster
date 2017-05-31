#ifndef B_NODE_H
#define B_NODE_H

#include <vector>
#include <string>

namespace bster{

    enum PORT_SIGNAL_TYPES {
        INVALID_TYPE = 0,
        UINT8 = 1,
        UINT16,
        UINT32,
        UINT64,
        UINT128,
        UINT_N,
        INT8,
        INT16,
        INT32,
        INT64,
        INT128,
        INT_N,
        HALF,
        SINGLE,
        DOUBLE,
        QUAD,
        OCTUPLE,
        BOOL,
        VECTOR,
        MATRIX,
        INHERIT = 255
    };

    class b_node
    {
    public:
        b_node();
        virtual ~b_node();
        std::vector<b_node*> vector_children;//ordered vector of children
        std::vector<b_node*> vector_parents; //d:o parents

        std::string node_name = "";

    private:
    };
}
#endif // B_NODE_H
