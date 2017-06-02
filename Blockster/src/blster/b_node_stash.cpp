#include "b_node_stash.h"

#include <exception>

bster::b_node_stash::b_node_stash()
{

}

void bster::b_node_stash::addNode(bster::b_node node)
{
    //add node to vector of all nodes
    v_nodes.push_back(node);

    //check how many nodes of this type that has been added
    //
    int n = map_nodetype_to_ncount.count(node.node_type);

    if(!n){
        //not added yet
		map_nodetype_to_ncount.insert({ node.node_type,1 });
    }
    else{
        //increase count with 1
        map_nodetype_to_ncount[node.node_type] = ++n;
    }
}

int bster::b_node_stash::getCountByNodeType(std::string type)
{
    return map_nodetype_to_ncount.count(type);
}

