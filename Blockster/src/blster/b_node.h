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

		port() : port(PORT_DIRS::INVALID,-1,SIGNAL_TYPES::INVALID_TYPE){}

        port(PORT_DIRS dir, short port_nr) : port(dir, port_nr, SIGNAL_TYPES::INVALID_TYPE){}

        port(PORT_DIRS dir, short port_nr, SIGNAL_TYPES signal_type){
            this->dir = dir;
            this->port_nr = port_nr;
            this->signal_type = signal_type;
        }

    } t_port;

    class b_node;

    typedef struct node_port_ptr{
        b_node* remote_node = 0;
        t_port local_port; //the port belongs to remote_node
        short remote_port_nr = -1;

        node_port_ptr(t_port local_port) : node_port_ptr(local_port,-1,0){}

        node_port_ptr(t_port local_port, short remote_port_nr) : node_port_ptr(local_port,remote_port_nr,0){}

        node_port_ptr(t_port local_port, short remote_port_nr, b_node* remote_node){
            this->remote_node = remote_node;
            this->local_port = local_port;
            this->remote_port_nr = remote_port_nr;
        }



    } t_node_port_ptr;

    class b_node
    {
    public:
        b_node();
        virtual ~b_node();

        std::string node_name = "";
        std::string node_id = "";
        std::string node_type = "";
        t_pos node_pos = {0,0,0};

        std::vector<t_node_port_ptr> v_children;
        std::vector<t_node_port_ptr> v_parents;

        static std::string enumSignalTypeToBsterString(SIGNAL_TYPES type);
        static SIGNAL_TYPES bsterSignalTypeStringToEnum(std::string type);

        int getNOutports();
        int getNInports();
        bool hasPorts();

    private:
    };
}
#endif // B_NODE_H
