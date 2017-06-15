
#include <map>
#include <stdexcept>
#include <algorithm>
#include <exception>
#include <unordered_set>
#include <tuple>

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
	
	//map node ids to node ptr
	for (auto node : v_nodes) {
		map_nodeidstr_to_nodeptr[node->node_id] = node;
	}

	//TODO: Here execution order -1 need to be replaced with the proper value by some serach

	//find the root execution node (==0)
	auto& i = find_if(this->v_nodes.begin(), this->v_nodes.end(), [](auto& ptrn) {return ptrn->node_exec_order == 0; });
	if (i == v_nodes.end())
		throw std::runtime_error("No root node in task " + to_string(this->task_id) + ". " + to_string(__LINE__) + ":" + __FILE__);
	shared_ptr<b_node> root_node = *i;
	
	//First off, make t_node_port_ptr for each node and associate it with the correct nodes
	//tree walk magic!
	//essentially connect node ports with each other
	//recursive function connects all node_port_ptr to the right node and port for each node
	linkNodesPorts(root_node, true);
	//calculate execution order of all the nodes begining with node 0 (root)
	calculateExecOrder(root_node);

	//map execution order to node ptr
	for (auto node : v_nodes) {
		map_execorder_to_nodeptr.insert({ node->node_exec_order,node });
	}
}

void bster::b_task::calculateExecOrder(std::shared_ptr<b_node> root_node)
{
	//make a map of temporary node execution orders
	std::map<std::string, int> map_nodeid_to_tmp_execorder;
	for (auto node : this->v_nodes) {

		auto& pair = map_nodeid_to_tmp_execorder.insert({ node->node_id, node == root_node ? 0 : -1 });
		if (pair.second == false) {
			throw std::runtime_error("Tried to add allready existing node id to map_nodeid_to_tmp_execorder for task id"+std::to_string(this->task_id));
		}
	}
	
	//TODO make this algoritm smart and not brute ... 
	//iterate over all nodes and make the execution order n+1 where n is the max of its parents exec. order
	bool changed = true;
	while (changed) {
		changed = false; 
		for (auto node : this->v_nodes) {
			if (root_node == node) {
				continue;
			}
			//find highest temp execution order of the nodes parents. 
			int n_max = 0;
			for (auto inport : node->v_inports) {
				for (auto pair : inport.v_pair_remote_node_portnr) {
					int n = map_nodeid_to_tmp_execorder[pair.first->node_id];
					if (n > n_max) {
						n_max = n;
					}
				}
			}
			if (map_nodeid_to_tmp_execorder.at(node->node_id) != n_max + 1) {
				map_nodeid_to_tmp_execorder[node->node_id] = n_max + 1;
				changed = true;
			}
		}
	}

	//sort the map according to tmp execution order
	std::vector<std::pair<std::string, int>> pairs;
	for (auto itr = map_nodeid_to_tmp_execorder.begin(); itr != map_nodeid_to_tmp_execorder.end(); ++itr)
		pairs.push_back(*itr);

	sort(pairs.begin(), pairs.end(), [](auto& a, auto& b)
	{
		return a.second < b.second;
	}
	);
	
	//write temp execution order to the nodes execution order
	int n = 0;
	for (auto pair : pairs) {
		map_nodeidstr_to_nodeptr[pair.first]->node_exec_order = n++;
	}
}

void bster::b_task::linkNodesPorts(std::shared_ptr<b_node> node, bool is_root)
{
	static std::unique_ptr<std::unordered_set<std::string>> set_visited_node_id_strs;
	
	if (is_root) {
		set_visited_node_id_strs = std::make_unique<std::unordered_set<std::string>>();
	}
	set_visited_node_id_strs->insert(node->node_id);

	//iterate over and find all nodes in outports and link to the correct node ptr
	for (auto& p : node->v_outports) {
		//iterade over all node ptr and remote port nr pairs
		for (auto& id : p.v_remote_node_id) {
			//find the node with that id
			auto it = find_if(this->v_nodes.begin(), this->v_nodes.end(),
				[&](auto& n) {return n->node_id == id.first; });
			//check if we found the id in the tasks vector of nodes
			if (it == this->v_nodes.end()) {
				throw std::runtime_error("Missing node " + id.first + "in task " + std::to_string(this->task_id) +
					"referenced by node id " + node->node_id + " " + std::to_string(__LINE__) + ":" + __FILE__);
			}
			
			//copy the ptr to the node
			auto trgt_node = *it;
			
			//make a pair of a node ptr to the found node and the port on that node thats connected to 
			//the current node processed by linkNodesPorts()
			auto&& pair = std::pair<std::shared_ptr<b_node>, short>(trgt_node, id.second);
			//auto ptr_pair = std::make_shared<std::pair<std::shared_ptr<b_node>, short>>(pair);				 
			p.v_pair_remote_node_portnr.push_back(pair);

			//process trgt_node if already note processed
			if(set_visited_node_id_strs->find(trgt_node->node_id) == set_visited_node_id_strs->end())
				linkNodesPorts(trgt_node);
		}
	}
	//iterate over and find all nodes in inports and link to the correct node ptr
	for (auto& p : node->v_inports) {

		//iterade over all node ptr and remote port nr pairs
		for (auto& id : p.v_remote_node_id) {
			//find the node with that id
			auto it = find_if(this->v_nodes.begin(), this->v_nodes.end(),
				[&](auto& n) {return n->node_id == id.first; });
			//check if we found the id in the tasks vector of nodes
			if (it == this->v_nodes.end()) {
				throw std::runtime_error("Missing node " + id.first + "in task " + std::to_string(this->task_id) +
					"referenced by node id " + node->node_id + " " + std::to_string(__LINE__) + ":" + __FILE__);
			}

			//copy the ptr to the node
			auto trgt_node = *it;

			//make a pair of a node ptr to the found node and the port on that node thats connected to 
			//the current node processed by linkNodesPorts()
			auto&& pair = std::pair<std::shared_ptr<b_node>, short>(trgt_node, id.second);
			//auto ptr_pair = std::make_shared<std::pair<std::shared_ptr<b_node>, short>>(pair);				 
			p.v_pair_remote_node_portnr.push_back(pair);

			//process trgt_node if already note processed
			if (set_visited_node_id_strs->find(trgt_node->node_id) == set_visited_node_id_strs->end())
				linkNodesPorts(trgt_node);
		}
	}

	if (is_root) {
		set_visited_node_id_strs->clear();
	}

	return;
}


 