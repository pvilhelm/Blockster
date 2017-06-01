#include "b_node.h"



bster::b_node::b_node()
{

}

bster::b_node::~b_node()
{

}



int bster::b_node::getNOutports()
{
    return this->v_out_ports.size();
}

int bster::b_node::getNInports()
{
    return this->v_in_ports.size();
}

bool bster::b_node::hasPorts()
{
    return (this->v_in_ports.size()+this->v_out_ports.size()) != 0;
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
            return "";//enumTypeToBlocksterString(INVALID_TYPE);
    }
}



bster::SIGNAL_TYPES bster::b_node::bsterSignalTypeStringToEnum(std::string type)
{
    if(type.front()!='u')
        goto A;

    if(type=="uint8")
        return SIGNAL_TYPES::UINT8;
    if(type=="uint16")
        return SIGNAL_TYPES::UINT16;
    if(type=="uint32")
        return SIGNAL_TYPES::UINT32;
    if(type=="uint64")
        return SIGNAL_TYPES::UINT64;
    if(type=="uint128")
        return SIGNAL_TYPES::UINT128;
    A:
    if(type.front()!='i')
        goto B;
    if(type=="int8")
        return SIGNAL_TYPES::INT8;
    if(type=="int16")
        return SIGNAL_TYPES::INT16;
    if(type=="int32")
        return SIGNAL_TYPES::INT32;
    if(type=="int64")
        return SIGNAL_TYPES::INT64;
    if(type=="int128")
        return SIGNAL_TYPES::INT128;
    B:
    if(type=="single")
        return SIGNAL_TYPES::SINGLE;
    if(type=="double")
        return SIGNAL_TYPES::DOUBLE;
    if(type=="quad")
        return SIGNAL_TYPES::QUAD;
    if(type=="bool")
        return SIGNAL_TYPES::BOOL;
    if(type=="inherit")
        return SIGNAL_TYPES::INHERIT;
    if(type=="vector")
        return SIGNAL_TYPES::VECTOR;
    if(type=="matrix")
        return SIGNAL_TYPES::MATRIX;
    if(type=="")
        return SIGNAL_TYPES::INVALID_TYPE;

    return SIGNAL_TYPES::INVALID_TYPE;
}
