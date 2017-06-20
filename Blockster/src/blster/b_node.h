#ifndef B_NODE_H
#define B_NODE_H

#include <vector>
#include <string>
#include <memory>
#include <tuple>
#include <map>
#include <limits>


namespace bster{

    enum struct PORT_DIRS{
        INVALID = 0,
        INW,
        OUTW,
        UNI
    };

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

    std::string signal_type_enum_to_str(SIGNAL_TYPES type);
    SIGNAL_TYPES signal_type_str_to_enum(std::string type);

    enum class PORT_TYPES{
        INVALID = 0,
        SIGNALS,
        CONTROL,//triggers etc
        EXECUTION_FLOW,//declares which block executes after which etc
        RELATION //just meta or config character for the connection
    };

    enum class NODE_CLASS {
            INVALID = 0,
            BLOCK,
            PORT,
            BUFFER,
            COMMENT
    };

    typedef struct pos{
        double x;
        double y;
        double z;
        double rot;
    } t_pos;

    class b_node;//gotta love C++ ...
    typedef struct port t_port;

    typedef struct port{
        PORT_DIRS dir = PORT_DIRS::INVALID;
        SIGNAL_TYPES signal_type = SIGNAL_TYPES::INVALID_TYPE;
        int local_port_nr = -1;

        std::vector<std::pair<std::string,short>> v_pair_remote_node_id_remote_port_nr;
        std::vector<std::pair<std::shared_ptr<b_node>,short>> v_pair_remote_node_portnr;


        port();
        port(PORT_DIRS dir, short port_nr);
        port(PORT_DIRS dir, short port_nr, SIGNAL_TYPES signal_type);
        port(PORT_DIRS dir, short port_nr, SIGNAL_TYPES signal_type, std::string target_node_id, int target_port_nr);


    } t_port;

    typedef struct member {
            SIGNAL_TYPES signal_type = SIGNAL_TYPES::INVALID_TYPE;
            std::string member_value = "";
            std::string member_name = "";
            bool member_tunable = false; //during runtime
    } t_member;

	enum class SHAPE {
		RECTANGLE,
		INVALID
	};

	std::string shape_enum_to_str(SHAPE type) { if (type == SHAPE::RECTANGLE) return "rectangle"; else return ""; }
	SHAPE shape_str_to_enum(std::string type) { if (type == "rectangle") return SHAPE::RECTANGLE; else return SHAPE::INVALID; }

    class b_node
    {
    public:
        b_node();
        virtual ~b_node();

        std::string node_name = "";//optional pretty name for the node
        std::string node_id = ""; //global id for the node
        std::string node_lib_path = ""; //ie. the path to the nodes lib folder
        std::string node_task_id = "";
        std::string node_lib_type = ""; //attribute type of Node_type
		SHAPE shape = SHAPE::INVALID;
		double node_width = 0;
		double node_height = 0;

        int node_exec_order = -1;
        NODE_CLASS node_class = NODE_CLASS::INVALID;


        t_pos node_pos = {  std::numeric_limits<double>::quiet_NaN(),
                            std::numeric_limits<double>::quiet_NaN(),
                            std::numeric_limits<double>::quiet_NaN(),
                            0.};


        std::map<std::string,t_member> map_membername_member;
        std::vector<t_port> v_inports;
        std::vector<t_port> v_outports;



        int getNOutports();
        int getNInports();
        bool hasPorts();
        void addPort(t_port port);


    private:
        void sortPortPtrVectors();
    };
}
#endif // B_NODE_H
