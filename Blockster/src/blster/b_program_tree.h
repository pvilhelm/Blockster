#pragma once

#include <vector>
#include <memory>
#include <string>
#include <set>
#include "b_node.h"
#include "b_task.h"

namespace bster{

    class b_program_tree {

    public:
            b_program_tree();
            ~b_program_tree();

            std::string program_name;

            std::vector<std::shared_ptr<b_node>> v_nodes;//initial vector of all nodes (gets cleared) TODO: make private?
            std::vector<std::shared_ptr<b_task>> v_tasks;//vector of all tasks sorted by task id
             
			std::map<std::string, std::set<std::string>> map_last_folder_to_set_of_nodeids;

            void setVectorOfNodes(std::vector<std::shared_ptr<b_node>> v_nodes);//Move
            void makeTaskVectors();
			std::shared_ptr<b_node> addNode(b_node node);
			std::shared_ptr<b_node> addNode(std::shared_ptr<b_node> node);

            std::string getNextNodeId(std::string node_lib_path);
    private:
            void mapNodeLastFolder(std::shared_ptr<bster::b_node>& node);
            void mapNodeLastFolder(bster::b_node& node);
    };
}
