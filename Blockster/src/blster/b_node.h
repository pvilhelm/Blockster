#ifndef B_NODE_H
#define B_NODE_H

#include <vector>
#include <string>
#include <memory>
//#include <cfloat>


namespace bster{

    enum class SIGNAL_TYPES {
        INVALID_TYPE = 0,
        UINT8,
        UINT16,
        UINT32,
        UINT64,
        UINT128,
        INT8,
        INT16,
        INT32,
        INT64,
        INT128,
        HALF,//reserved
        SINGLE,//as in c-float
        DOUBLE,
        QUAD,
        OCTUPLE,//reserved 
        BOOL,
        VECTOR,
        MATRIX,
        INHERIT
    };

	
    enum class PORT_DIRS{
        INVALID = 0,
        IN,
        OUT,
        UNI,
    } ;

	
    enum class PORT_TYPES{
        INVALID = 0,
        SIGNALS,
        CONTROL,//triggers etc
        EXECUTION_FLOW,//declares which block executes after which etc
        RELATION //just meta or config character for the connection
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

		port();
		port(PORT_DIRS dir, short port_nr);
		port(PORT_DIRS dir, short port_nr, SIGNAL_TYPES signal_type);

    } t_port;

    class b_node;

    typedef struct node_port_ptr{
        std::shared_ptr<b_node> remote_node;
        t_port local_port; //the port belongs to remote_node
        short remote_port_nr = -1;

        node_port_ptr(t_port local_port) : node_port_ptr(local_port, -1, nullptr){}

        node_port_ptr(t_port local_port, short remote_port_nr) : node_port_ptr(local_port,remote_port_nr,nullptr){}

        node_port_ptr(t_port local_port, short remote_port_nr, std::shared_ptr<b_node> remote_node){
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

        std::string node_name = "";//optional pretty name for the node
        std::string node_id = ""; //global id for the node
        std::string node_type = ""; //ie. the path to the nodes lib folder
		std::string node_task_id = "";
		int node_exec_order = -1;

        t_pos node_pos = {	std::numeric_limits<double>::quiet_NaN(),
							std::numeric_limits<double>::quiet_NaN(),
							std::numeric_limits<double>::quiet_NaN()};

        std::vector<t_node_port_ptr> v_children;
        std::vector<t_node_port_ptr> v_parents;

        static std::string enumSignalTypeToBsterString(SIGNAL_TYPES type);
        static SIGNAL_TYPES bsterSignalTypeStringToEnum(std::string type);

        int getNOutports();
        int getNInports();
        bool hasPorts();
		void addPort(t_port port);
		

    private:
		void sortPortPtrVectors();
    };
}


#endif // B_NODE_H
