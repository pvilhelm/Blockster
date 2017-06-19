#pragma once

#include <vector>
#include <memory>
#include <string>

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

		void setVectorOfNodes(std::vector<std::shared_ptr<b_node>> v_nodes);//Move 
		void makeTaskVectors();

	private:

	};
}