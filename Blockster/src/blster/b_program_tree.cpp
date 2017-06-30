#include "b_program_tree.h"

#include <exception>
#include <string>
#include <algorithm>
#include <limits>
#include <sstream>
#include <memory>
#include <regex>
#include <cstdio>
#include <numeric>

std::string getLastFolderName(std::string path){
    std::regex r(R"((?:.*[\\\/])?\w+\/template_node_(\w+)\.xml)");
    std::smatch sm;
    std::regex_match (path,sm,r);
    if(sm.empty()){
    throw std::runtime_error("No last folder to get name from in "+path+". "
                    +std::to_string(__LINE__)+":"+__FILE__);
    }
    return sm[1];
}

void bster::b_program_tree::mapNodeLastFolder(bster::b_node& node) {
    std::string last_folder = getLastFolderName(node.node_lib_path);
    std::string node_id = node.node_id;
    //if there is no entry for last_folder add entry with set with node id
    if (map_last_folder_to_set_of_nodeids.end() ==
        map_last_folder_to_set_of_nodeids.find(last_folder)) {
        //map the node
        map_last_folder_to_set_of_nodeids[last_folder] = std::set<std::string>{ node_id };
    }
    else {//add node id to set at entry
        //check if node_id is allready added to the program tree 
        if (map_last_folder_to_set_of_nodeids[last_folder].find(node_id) !=
            map_last_folder_to_set_of_nodeids[last_folder].end()) {
            throw std::runtime_error("Node id " + node_id + "already added to program tree. " +
                                      std::to_string(__LINE__)+":"+__FILE__);
        }
        //map the node
        map_last_folder_to_set_of_nodeids[last_folder].insert(node_id);
    }
}

void bster::b_program_tree::mapNodeLastFolder(std::shared_ptr<bster::b_node>& node) {
    mapNodeLastFolder(*node);
}

bster::b_program_tree::b_program_tree()
{
}

bster::b_program_tree::~b_program_tree()
{
}

void bster::b_program_tree::setVectorOfNodes(std::vector<std::shared_ptr<b_node>> v_nodes)
{
    this->v_nodes = std::move(v_nodes);
}

void bster::b_program_tree::makeTaskVectors()
{


    std::vector<int> task_ids;

    for (auto&& node : this->v_nodes) {
        std::string task_id = node->node_task_id;


        int i_task_id = std::stoi(task_id);
        std::string str_cmp = std::to_string(i_task_id);

        if (str_cmp != task_id) {//round trip failed (not an valid int maybe)
            throw std::runtime_error("Node: " + node->node_id + "has invalid task id " +
                task_id + ". " + str_cmp + "!=" + task_id + " " + std::to_string(__LINE__) + ":" + __FILE__);
        }

        if (i_task_id < 0) {//negative task ids are not valid. -1 is unassigned place holder
            throw std::runtime_error("Node: " + node->node_id + "has invalid task id " +
                task_id + ". " + std::to_string(__LINE__) + ":" + __FILE__);
        }

        if (std::find(task_ids.begin(), task_ids.end(), i_task_id) == task_ids.end()) {
            task_ids.push_back(i_task_id);
        }
    }

    //sort task ids
    std::sort(std::begin(task_ids), std::end(task_ids));

    for (int i : task_ids) {

        std::unique_ptr<b_task> task = std::make_unique<b_task>(i);
        this->v_tasks.push_back(std::move(task));
    }

    //asure all tasks ids are: n_(k+1) = n_k+1
    for (unsigned i = 0; i < v_tasks.size(); i++) {
        auto& t = v_tasks[i];

        if (i != v_tasks.size() - 1){
            //if id is not one smaller then the following id
            if (t->task_id+1 != v_tasks[i + 1]->task_id) {
                throw std::runtime_error("Task ids not in n_(k+1) = n_k+1" +
                    std::to_string(__LINE__) + ":" + __FILE__);
            }
        }
    }

	//map each of the last folder in all node_lib_paths to a vector of node_ids
	//that corrensponds to that path
	// ie /core/bla/constant -> last_folder == constant
    map_last_folder_to_set_of_nodeids.clear(); //might be populated allready
	for (auto& node : this->v_nodes) {
		std::string last_folder = getLastFolderName(node->node_lib_path);
		std::string node_id = node->node_id;
		//if there is no entry for last_folder 
        mapNodeLastFolder(node);
	}

	//check that there are a equal amount of nodes as unique node ids 
	unsigned long n = 0;
	for (auto& p : map_last_folder_to_set_of_nodeids) {
		auto& v = p.second;
		n += v.size();
	}
	if (n != this->v_nodes.size())
		throw std::runtime_error("Not all nodes has unique ids."+std::string(" ")+__FILE__);

    //assign nodes to their tasks
    for (auto& node : this->v_nodes) {
        std::string task_id = node->node_task_id;

        int i_task_id = std::stoi(task_id);

        auto& task = v_tasks.at(i_task_id);
        task->addNode(std::move(node));
    }
    v_nodes.clear();

    for (unsigned int i = 0; i < v_tasks.size(); i++) {
        auto& task = v_tasks[i];
        task->processAllNodes();
    }

    return;
}

void bster::b_program_tree::addNode(b_node node)
{
    //if default node id, assign node id
    if (node.node_id == "") {
        node.node_id = getNextNodeId(node.node_lib_path);
    }
    mapNodeLastFolder(node);

    int task_id = std::stoi(node.node_task_id);
    if (v_tasks.size() - 1 < task_id)
        throw std::runtime_error("Task id greater then index of v_task vector. " + std::to_string(__LINE__) + ":" + __FILE__);
    if (this->v_tasks[task_id]->task_id != task_id) {
        std::stringstream ss;
        for (auto task : v_tasks)
            ss << "Task id: " + std::to_string(task->task_id) << "\n";
        throw std::runtime_error("Task id parameter doens't match task id in v_tasks. Task id:s " + ss.str() + "\n" + std::to_string(__LINE__) + ":" + __FILE__);
    }
    this->v_tasks[task_id]->addNode(node);
    this->v_tasks[task_id]->processAllNodes();
}

std::string bster::b_program_tree::getNextNodeId(std::string node_lib_path)
{
    // Extract last folder in node_lib_path
    std::string folder_name = getLastFolderName(node_lib_path);

    // Extract number from sorted last node id from a vector of all
    //node ids with the same folder_name.

    std::string last_id; // Id of the last sorted id for all nodes with folder_name
                         //as the last folder in their lib path.
    unsigned long long last_id_n;       // Value of the number part of the last id.
    unsigned long long new_id_n;        // last_id_n+1

    // If the map has no entry for folder_name
    if (map_last_folder_to_set_of_nodeids.end() ==
        map_last_folder_to_set_of_nodeids.find(folder_name)) {
        //add an empty set
        //map_last_folder_to_set_of_nodeids[folder_name] = std::set<std::string>{};
        new_id_n = 0;
    }
    else {
        last_id = *map_last_folder_to_set_of_nodeids[folder_name].rbegin();
        //extract number part with regex
        std::regex r(R"(^\w+_(\d+)$)");
        std::smatch sm;
        std::regex_match(last_id, sm, r);
        if (sm.empty())
            throw std::runtime_error("Syntax error in node id " + last_id + " " + std::to_string(__LINE__) +
                +":" + __FILE__);
        std::string num = sm[1];
        last_id_n = std::stoull(num);
        if (last_id_n >= std::numeric_limits<unsigned int>::max())
            throw std::runtime_error("Id number part grows over uint32 max. id n = " + std::to_string(last_id_n) +
                " " + std::to_string(__LINE__) + ":" + __FILE__);
        new_id_n = last_id_n + 1;
    }
    
    //determine buffer size needed for a minimum of XXX digit representation
    size_t size = 1+snprintf(nullptr,0,"%08llu", new_id_n);

    char* buffer = new char[size];
    snprintf(buffer,sizeof(char)*size,"%08llu", new_id_n);
    std::string full_id = folder_name+"_"+buffer;
    delete[] buffer;

    return full_id;
}
