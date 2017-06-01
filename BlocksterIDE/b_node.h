#ifndef B_NODE_H
#define B_NODE_H

#include <vector>
#include <string>



namespace bster{

    enum class SIGNAL_TYPES {
        INVALID_TYPE = 0,
        UINT = 1,
        UINT8 = UINT+(1<<1),
        UINT16 = UINT+(2<<1),
        UINT32 = UINT+(3<<1),
        UINT64 = UINT+(4<<1),
        UINT128 = UINT+(5<<1),
        UINT_N = UINT+(6<<1),
        INT = 16,
        INT8 = INT+(1<<5),
        INT16 = INT+(2<<5),
        INT32 = INT+(3<<5),
        INT64 = INT+(4<<5),
        INT128 = INT+(5<<5),
        INT_N = INT+(6<<5),
        FLOAT = 256,//as in floating point not c-float
        HALF = 256+(1<<9),
        SINGLE = 256+(2<<9),//as in c-float
        DOUBLE = 256+(3<<9),
        QUAD = 256+(4<<9),
        OCTUPLE = 256+(5<<9),
        BOOL = 4096,
        VECTOR = 8192,
        MATRIX = 1<<14,
        INHERIT = 1<<15
    };

    enum class PORT_DIRS{
        INVALID = 0,
        IN = 1,
        OUT = 2,
        UNI = IN+OUT,
    } ;

    enum class PORT_TYPES{
        INVALID = 0,
        SIGNALS = 1,
        CONTROL = 2,//triggers etc
        EXECUTION_FLOW = 4,//declares which block executes after which etc
        RELATION = 8//just meta or config character for the connection
    };

    typedef struct pos{
        double x;
        double y;
        double z;
    } t_pos;

    typedef struct port{
        PORT_DIRS dir = PORT_DIRS::INVALID;
        SIGNAL_TYPES signal_type = SIGNAL_TYPES::INVALID_TYPE;
        short port_nr = -1;
    } t_port;

    class b_node;

    typedef struct node_port_ptr{
        b_node* node;
        t_port port;
    } t_node_port_ptr;

    class b_node
    {
    public:
        b_node();
        virtual ~b_node();

        std::string node_name = "";
        std::string node_id = "";
        t_pos node_pos = {0,0,0};

        std::vector<t_node_port_ptr> v_out_ports;
        std::vector<t_node_port_ptr> v_in_ports;

        std::string enumSignalTypeToBsterString(SIGNAL_TYPES type);
        SIGNAL_TYPES bsterSignalTypeStringToEnum(std::string type);
        int getNOutports();
        int getNInports();
        bool hasPorts();

    private:
    };
}
#endif // B_NODE_H
