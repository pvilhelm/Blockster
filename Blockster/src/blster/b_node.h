#ifndef B_NODE_H
#define B_NODE_H

#include <vector>
#include <string>
#include <memory>
#include <tuple>


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

	class b_node;//gotta love C++ ...
	typedef struct port t_port;

    typedef struct port{
        PORT_DIRS dir = PORT_DIRS::INVALID;
        SIGNAL_TYPES signal_type = SIGNAL_TYPES::INVALID_TYPE;
        short local_port_nr = -1;

		std::vector<std::string> v_remote_node_id;
		std::vector<std::pair<std::shared_ptr<b_node>,short>> v_pair_remote_node_portnr;
		
		port();
		port(PORT_DIRS dir, short port_nr);
		port(PORT_DIRS dir, short port_nr, SIGNAL_TYPES signal_type);
		
    } t_port;

	 
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

        

		std::vector<t_port> v_inports;
		std::vector<t_port> v_outports;

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
