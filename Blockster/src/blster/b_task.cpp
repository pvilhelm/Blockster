
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

void bster::b_task::addNode(std::shared_ptr<b_node> node)
{
    this->v_ptr_nodes.push_back(std::move(node));
}

void bster::b_task::addNode(b_node node)
{
    this->v_ptr_nodes.push_back(std::make_shared<b_node>(node));
}

void bster::b_task::processAllNodes()
{
    using namespace std;

    //map node ids to node ptr
    for (auto node : v_ptr_nodes) {
        map_nodeidstr_to_nodeptr[node->node_id] = node;
    }

    //TODO: Here execution order -1 need to be replaced with the proper value by some serach

    //find the root execution node (execution order == 0)
    const auto& i = find_if(this->v_ptr_nodes.begin(), this->v_ptr_nodes.end(), [](auto& ptrn) {return ptrn->node_exec_order == 0; });
    if (i == v_ptr_nodes.end())
        throw std::runtime_error("No root node in task " + to_string(this->task_id) + ". " + to_string(__LINE__) + ":" + __FILE__);
    ptr_root_node = *i;

    //look through the rest of the nodes so their are no more "root" nodes
    long root_count = count_if(i, this->v_ptr_nodes.end(), [](auto& ptrn) {return ptrn->node_exec_order == 0; });
    if (root_count != 1) {
        throw std::runtime_error("More then one root node in " + to_string(this->task_id) + ". " + to_string(__LINE__) + ":" + __FILE__);
    }


    //connect node ports with each other
    //function connects all nodes to the right node and port for each node
    linkNodesPorts();
    //calculate and set execution order of all the nodes begining with node 0 (root)
    calculateExecOrder(ptr_root_node);

    //map execution order to node ptr
    for (auto node : v_ptr_nodes) {
        map_execorder_to_nodeptr.insert({ node->node_exec_order,node });
    }
}

void bster::b_task::calculateExecOrder(std::shared_ptr<b_node> root_node)
{
    //make a map of temporary node execution orders
    std::map<std::string, int> map_nodeid_to_tmp_execorder;
    for (auto node : this->v_ptr_nodes) {

        const auto& pair = map_nodeid_to_tmp_execorder.insert({ node->node_id, node == root_node ? 0 : -1 });
        if (pair.second == false) {
            throw std::runtime_error("Tried to add allready existing node id to map_nodeid_to_tmp_execorder for task id"+std::to_string(this->task_id));
        }
    }

    //TODO make this algoritm smart and not brute ...
    //iterate over all nodes and make the execution order n+1 where n is the max of its parents exec. order
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto node : this->v_ptr_nodes) {
            //root node is always exec order 0
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
            //update temp execution order to max parent's + 1
            //note that isolated "sources" will get a n_max of 0 and thus get a execution order of 1
            if (map_nodeid_to_tmp_execorder.at(node->node_id) != n_max + 1) {
                map_nodeid_to_tmp_execorder[node->node_id] = n_max + 1;
                changed = true;//when no node is changed the algoritm has converged
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

    //write ascending execution order to the nodes execution order sorted by temp execution order
    //(to prevent multiple identical executions order like inte temp execution order)
    int n = 0;
    for (auto pair : pairs) {
        map_nodeidstr_to_nodeptr[pair.first]->node_exec_order = n++;
    }
}

void bster::b_task::linkNodesPorts()
{
    //iterate over all nodes and link to the correct remote node ptr and make remote node ptr remote port nr pair
    //for inports and outports
    for (auto node : this->v_ptr_nodes) {
        for (auto& p : node->v_outports) {
            //iterade over all node ptr and remote port nr pairs
            for (auto& id : p.v_pair_remote_node_id_remote_port_nr) {
                //find the node with that id
                auto it = find_if(this->v_ptr_nodes.begin(), this->v_ptr_nodes.end(),
                    [&](auto& n) {return n->node_id == id.first; });
                //check if we found the id in the tasks vector of nodes
                if (it == this->v_ptr_nodes.end()) {
                    throw std::runtime_error("Missing node " + id.first + "in task " + std::to_string(this->task_id) +
                        " referenced by node id " + node->node_id + " " + std::to_string(__LINE__) + ":" + __FILE__);
                }
                //copy the ptr to the node
                auto trgt_node = *it;

                //make a pair of a node ptr to the found node and the port on that node thats connected to
                //the current node processed by linkNodesPorts()
                auto&& pair = std::pair<std::shared_ptr<b_node>, short>(trgt_node, id.second);
                //auto ptr_pair = std::make_shared<std::pair<std::shared_ptr<b_node>, short>>(pair);
                p.v_pair_remote_node_portnr.push_back(pair);
            }
        }

        for (auto& p : node->v_inports) {

            //iterade over all node ptr and remote port nr pairs
            for (auto& id : p.v_pair_remote_node_id_remote_port_nr) {
                //find the node with that id
                auto it = find_if(this->v_ptr_nodes.begin(), this->v_ptr_nodes.end(),
                    [&](auto& n) {return n->node_id == id.first; });
                //check if we found the id in the tasks vector of nodes
                if (it == this->v_ptr_nodes.end()) {
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
            }
        }
    }

    return;
}


