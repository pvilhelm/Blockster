#pragma once

#include <vector>

#include "b_node.h"
#include "b_task.h"

namespace bster{

	class b_program_tree {

	public:
		b_program_tree();
		~b_program_tree();

		std::vector<b_node> v_nodes;//initial vector of all nodes
		std::vector<b_task> v_tasks;//vector of all tasks sorted by task id. INF = int32max

		void setVectorOfNodes(std::vector<b_node> v_nodes);//Move 
		void makeTaskVectors();

	private:

	};
}