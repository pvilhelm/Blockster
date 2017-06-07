#include "b_node_stash.h"

#include <exception>
#include <iterator>


bster::b_node_stash::b_node_stash()
{

}

void bster::b_node_stash::addNode(bster::b_node node)
{
    //add node to vector of all nodes
    v_nodes.push_back(std::make_unique<b_node>(node));

    //check how many nodes of this type that has been added
    //
    int n = map_nodetype_to_ncount.count(node.node_type);

    if(!n){
        //not added yet
		map_nodetype_to_ncount.insert({ node.node_type,1 });
    }
    else{
        //increase count with 1
        map_nodetype_to_ncount[node.node_type] = map_nodetype_to_ncount[node.node_type]+1;
    }
}

int bster::b_node_stash::getCountByNodeType(std::string type)
{
	if (map_nodetype_to_ncount.count(type)) {
		return map_nodetype_to_ncount[type];
	}
	return 0;
}

bool bster::b_node_stash::verifyNodes()
{
	for (auto&& node : v_nodes) {
		if (!verify(node)) {
			return false; 
		}
	}
	return true;
}

bool bster::b_node_stash::verify(std::unique_ptr<b_node>& node)
{
	
	if (node->node_id == "")
		return false;
	if (node->node_type == "")
		return false;
	if (std::isnan(node->node_pos.x))
		return false;
	if (std::isnan(node->node_pos.y))
		return false;
	if (std::isnan(node->node_pos.z))
		return false;
	if (node->node_task_id == "")
		return false; 
	return true;
}

