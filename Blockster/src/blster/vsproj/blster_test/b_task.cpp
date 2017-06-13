
#include <map>
#include <stdexcept>

#include "b_task.h"
#include "b_node.h"

bster::b_task::b_task(int task_id)
{
	this->task_id = task_id;
}


bster::b_task::~b_task()
{
}

void bster::b_task::addNode(std::unique_ptr<b_node> node)
{
	this->v_nodes.push_back(std::move(node));
}

void bster::b_task::processAllNodes()
{
	using namespace std;
	//tree walk magic!
	//essentially connect node ports with each other
	
	//map node ids to node ptr
	map<string, shared_ptr<b_node>> map_nodeidstr_to_nodeptr;
	
	for (auto node : v_nodes) {
		map_nodeidstr_to_nodeptr[node->node_id] = node;
	}

	//map execution order to node ptr
	multimap<int, shared_ptr<b_node>> map_execorder_to_nodeptr; 

	for (auto node : v_nodes) {
		map_execorder_to_nodeptr.insert({ node->node_exec_order,node });
	}

	//TODO: Here execution order -1 need to be replaced with the proper value by some serach

	//find the root execution node (==0)
	auto& i = map_execorder_to_nodeptr.find(0);
	if (i == map_execorder_to_nodeptr.end()) 
		throw std::runtime_error("No root node in task " + to_string(this->task_id) + ". " + to_string(__LINE__) + ":" + __FILE__);
	shared_ptr<b_node> root_node = i->second;

	//First off, make t_node_port_ptr for each node and associate it with the correct nodes

	//recursive function connects all node_port_ptr to the right node and port for each node
	linkNodesPorts(root_node);
}

void bster::b_task::linkNodesPorts(std::shared_ptr<b_node> node)
{
	for (auto& p : node->v_outports) {
		
		//iterade over all node ptr and remote port nr pairs
		for (auto& pair_nodeptr_portnr : p.v_remote_node_id) {

		}
	}

}


 