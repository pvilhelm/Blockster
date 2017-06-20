#include "b_node.h"

#include <unordered_map>
#include <string>
#include <algorithm>
#include <exception>

bster::b_node::b_node()
{
		 
}

bster::b_node::~b_node()
{

}



int bster::b_node::getNOutports()
{
    return this->v_outports.size();
}

int bster::b_node::getNInports()
{
    return this->v_inports.size();
}

bool bster::b_node::hasPorts()
{
    return (this->v_outports.size()+this->v_inports.size()) != 0;
}

void bster::b_node::addPort(t_port port)
{
	if (port.local_port_nr < 0) {
		throw std::runtime_error("Port with negative number added to node "+this->node_id + 
			" with port nr:"+std::to_string(port.local_port_nr) +" "+ std::to_string(__LINE__) + ":" + __FILE__);
	}

	if (port.dir == PORT_DIRS::OUTW) {
		v_outports.push_back(port);
		sortPortPtrVectors();
	}
	else if (port.dir == PORT_DIRS::INW) {
		v_inports.push_back(port);
		sortPortPtrVectors();
	}//TODO add "uni" port 
	else{
		throw std::runtime_error("Port added with invalid direction " + std::to_string(__LINE__) + ":" + __FILE__);
	}
}



void bster::b_node::sortPortPtrVectors()
{
	std::sort(v_outports.begin(), v_outports.end(), [](const t_port& lhs, const t_port& rhs) {
		return lhs.local_port_nr < rhs.local_port_nr;
	});
	std::sort(v_inports.begin(), v_inports.end(), [](const t_port& lhs, const t_port& rhs) {
		return lhs.local_port_nr < rhs.local_port_nr;
	});
}


std::string bster::signal_type_enum_to_str(SIGNAL_TYPES type)
{
    switch(type){
        case SIGNAL_TYPES::INVALID_TYPE:
            return "";
        case SIGNAL_TYPES::UINT8:
            return "uint8";
        case SIGNAL_TYPES::UINT16:
            return "uint16";
        case SIGNAL_TYPES::UINT32:
            return "uint32";
        case SIGNAL_TYPES::UINT64:
            return "uint64";
        case SIGNAL_TYPES::UINT128:
            return "uint128";
        case SIGNAL_TYPES::INT8:
            return "int8";
        case SIGNAL_TYPES::INT16:
            return "int16";
        case SIGNAL_TYPES::INT32:
            return "int32";
        case SIGNAL_TYPES::INT64:
            return "int64";
        case SIGNAL_TYPES::INT128:
            return "int128";
        case SIGNAL_TYPES::SINGLE:
            return "single";
        case SIGNAL_TYPES::DOUBLE:
            return "double";
        case SIGNAL_TYPES::QUAD:
            return "quad";
        case SIGNAL_TYPES::BOOL:
            return "bool";
        case SIGNAL_TYPES::INHERIT:
            return "inherit";
        case SIGNAL_TYPES::VECTOR:
            return "vector";
        case SIGNAL_TYPES::MATRIX:
            return "matrix";
        default:
            //TODO not implementet
            //return "";//enumTypeToBlocksterString(INVALID_TYPE);
			throw std::runtime_error("Non existing SignalType in node "+std::to_string(__LINE__)+":"+__FILE__);
    }
}



bster::SIGNAL_TYPES bster::signal_type_str_to_enum(std::string type)
{

    const std::unordered_map<std::string,SIGNAL_TYPES> str_to_enum_map =
    {
        {"uint8",SIGNAL_TYPES::UINT8},
        {"uint16",SIGNAL_TYPES::UINT16},
        {"uint32",SIGNAL_TYPES::UINT32},
        {"uint64",SIGNAL_TYPES::UINT64},
        {"uint128",SIGNAL_TYPES::UINT128},
        {"inherit",SIGNAL_TYPES::INHERIT},
        {"int8",SIGNAL_TYPES::INT8},
        {"int16",SIGNAL_TYPES::INT16},
        {"int32",SIGNAL_TYPES::INT32},
        {"int64",SIGNAL_TYPES::INT64},
        {"int128",SIGNAL_TYPES::INT128},
        {"single",SIGNAL_TYPES::SINGLE},
        {"double",SIGNAL_TYPES::DOUBLE},
        {"quad",SIGNAL_TYPES::QUAD},
        {"bool",SIGNAL_TYPES::BOOL},
        {"vector",SIGNAL_TYPES::VECTOR},
        {"matrix",SIGNAL_TYPES::MATRIX},
        {"",SIGNAL_TYPES::INVALID_TYPE}
    };
	if (str_to_enum_map.find(type) != str_to_enum_map.end())
		return str_to_enum_map.at(type);
	else if (type.substr(0, 7) == "inherit")
		return SIGNAL_TYPES::INHERIT;//todo: @members@nameofmember etc need to be handlded somewhere
	else
		throw std::runtime_error("No SIGNAL_TYPES for " + type);

}

bster::port::port() : port(PORT_DIRS::INVALID, -1, SIGNAL_TYPES::INVALID_TYPE)
{
}

bster::port::port(PORT_DIRS dir, short port_nr) : port(dir, port_nr, SIGNAL_TYPES::INVALID_TYPE)
{
}

bster::port::port(PORT_DIRS dir, short port_nr, SIGNAL_TYPES signal_type)
{
	this->dir = dir;
	this->local_port_nr = port_nr;
	this->signal_type = signal_type;
}

bster::port::port(PORT_DIRS dir, short port_nr, SIGNAL_TYPES signal_type, std::string target_node_id, int target_port_nr) :
	port(dir, port_nr, signal_type)
{
	this->v_pair_remote_node_id_remote_port_nr.push_back(std::pair<std::string, int>(target_node_id, target_port_nr));
}
