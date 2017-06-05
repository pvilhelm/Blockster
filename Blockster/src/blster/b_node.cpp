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
    return this->v_children.size();
}

int bster::b_node::getNInports()
{
    return this->v_parents.size();
}

bool bster::b_node::hasPorts()
{
    return (this->v_parents.size()+this->v_children.size()) != 0;
}

void bster::b_node::addPort(t_port port)
{
	if (port.port_nr < 0) {
		throw new std::runtime_error("Port with negative number added to node "+this->node_id + 
			" with port nr:"+std::to_string(port.port_nr) +" "+ std::to_string(__LINE__) + ":" + __FILE__);
	}

	if (port.dir == PORT_DIRS::OUT) {
		node_port_ptr npp(port); // create a port node ptr struct pointing nowhere

		v_children.push_back(npp);
		sortPortPtrVectors();
	}
	else if (port.dir == PORT_DIRS::IN) {
		node_port_ptr npp(port); // create a port node ptr struct pointing nowhere

		v_parents.push_back(npp);
		sortPortPtrVectors();
	}//TODO add "uni" port 
	else{
		throw new std::runtime_error("Port added with invalid direction " + std::to_string(__LINE__) + ":" + __FILE__);
	}
}



void bster::b_node::sortPortPtrVectors()
{
	std::sort(v_children.begin(), v_children.end(), [](const node_port_ptr& lhs, const node_port_ptr& rhs) {
		return lhs.local_port.port_nr < rhs.local_port.port_nr;
	});
	std::sort(v_parents.begin(), v_parents.end(), [](const node_port_ptr& lhs, const node_port_ptr& rhs) {
		return lhs.local_port.port_nr < rhs.local_port.port_nr;
	});
}


std::string bster::b_node::enumSignalTypeToBsterString(bster::SIGNAL_TYPES type)
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
			throw new std::runtime_error("Non existing SignalType in node "+std::to_string(__LINE__)+":"+__FILE__);
    }
}



bster::SIGNAL_TYPES bster::b_node::bsterSignalTypeStringToEnum(std::string type)
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

    return str_to_enum_map.at(type);

}

bster::port::port()
{
}

bster::port::port(PORT_DIRS dir, short port_nr) : port(dir, port_nr, SIGNAL_TYPES::INVALID_TYPE)
{
}

bster::port::port(PORT_DIRS dir, short port_nr, SIGNAL_TYPES signal_type)
{
	this->dir = dir;
	this->port_nr = port_nr;
	this->signal_type = signal_type;
}
