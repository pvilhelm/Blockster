#include "b_program_tree.h"

#include <exception>
#include <string>
#include <algorithm>
#include <limits>
#include <sstream>
#include <memory>
#include <regex>
#include <cstdio>

std::string getLastFolderName(std::string path){
    std::regex r(R"((?:.*[\\\/])?(\w+))");
    std::smatch sm;
    std::regex_match (path,sm,r);
    if(sm.size()!=2){
    throw std::runtime_error("No last folder to get name from in "+path+". "
                    +std::to_string(__LINE__)+":"+__FILE__);
    }
    return sm[1];
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
	for (auto& node : this->v_nodes) {
		std::string last_folder = getLastFolderName(node->node_lib_path);
		std::string node_id = node->node_id;
		//if there is no entry for last_folder 
		if (map_last_folder_to_set_of_nodeids.end() == 
			map_last_folder_to_set_of_nodeids.find(last_folder)) {

			map_last_folder_to_set_of_nodeids[last_folder] = std::set<std::string>{ node_id };
		}
		else {
			map_last_folder_to_set_of_nodeids[last_folder].insert(node_id);
		}
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
    for (auto&& node : this->v_nodes) {
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
    //extract last folder in node_lib_path
    std::string folder_name = getLastFolderName(node_lib_path);
    unsigned int n_last = map_last_folder_in_node_path_to_n_nodes[folder_name];
    unsigned int n_new = ++n_last;
    map_last_folder_in_node_path_to_n_nodes[folder_name] = n_new;

    size_t size = 1+snprintf(nullptr,0,"%06u",n_new);

    char* buffer = new char[size];
    snprintf(buffer,sizeof(char)*size,"%06u",n_new);
    std::string full_id = folder_name+"_"+buffer;
    delete[] buffer;

    return full_id;
}
