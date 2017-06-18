#ifndef B_NODE_H
#define B_NODE_H

#include <vector>
#include <string>
#include <memory>
#include <tuple>
#include <map> 

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

	std::string enumSignalTypeToBsterString(SIGNAL_TYPES type);
	SIGNAL_TYPES bsterSignalTypeStringToEnum(std::string type);

	/*SIGNAL_TYPES str2SIGNAL_TYPE(std::string str_signal_type) {
		 
		const std::map<std::string, SIGNAL_TYPES> map_str_sigtype = {
			{"", SIGNAL_TYPES::INVALID_TYPE},
			{ "uint8", SIGNAL_TYPES::UINT8 },
			{ "uint16", SIGNAL_TYPES::UINT16 },
			{ "uint32", SIGNAL_TYPES::UINT32 },
			{ "uint64", SIGNAL_TYPES::UINT64 },
			{ "uint128", SIGNAL_TYPES::UINT128 },
			{ "int8", SIGNAL_TYPES::INT8 },
			{ "int16", SIGNAL_TYPES::INT16 },
			{ "int32", SIGNAL_TYPES::INT32 },
			{ "int64", SIGNAL_TYPES::INT64 },
			{ "int128", SIGNAL_TYPES::INT128 },
			{ "half", SIGNAL_TYPES::HALF },//reserved
			{ "single", SIGNAL_TYPES::SINGLE },//as in c-float
			{ "double", SIGNAL_TYPES::DOUBLE },
			{ "quad", SIGNAL_TYPES::QUAD },
			{ "octuple", SIGNAL_TYPES::OCTUPLE },//reserved 
			{ "bool", SIGNAL_TYPES::BOOL },
			{ "vector", SIGNAL_TYPES::VECTOR },
			{ "matrix", SIGNAL_TYPES::MATRIX },
			{ "inherit", SIGNAL_TYPES::INHERIT }
		};

		if (map_str_sigtype.find(str_signal_type) != map_str_sigtype.end())
			return map_str_sigtype.at(str_signal_type);
		else if (str_signal_type.substr(0, 7) == "inherit")
			return SIGNAL_TYPES::INHERIT;//todo: @members@nameofmember etc need to be handlded somewhere
		else
			return SIGNAL_TYPES::INVALID_TYPE;
	}*/
	
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
		
    } t_port;

	typedef struct member {
		SIGNAL_TYPES signal_type = SIGNAL_TYPES::INVALID_TYPE;
		std::string member_value = "";
		std::string member_name = "";
		bool member_tunable = false; //during runtime 
	} t_member;

	 
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

		int node_exec_order = -1;
		NODE_CLASS node_class = NODE_CLASS::INVALID;
		

        t_pos node_pos = {	std::numeric_limits<double>::quiet_NaN(),
							std::numeric_limits<double>::quiet_NaN(),
							std::numeric_limits<double>::quiet_NaN(),
							0.};

        
		std::map<std::string,std::shared_ptr<t_member>> map_membername_ptrmember; 
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
