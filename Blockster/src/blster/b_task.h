#pragma once

#include <vector>
#include <memory>
#include <map>
#include <string>

#include "b_node.h"

namespace bster {
	class b_task
	{
	public:
		b_task(int task_id);
		~b_task();

		std::map<std::string, std::shared_ptr<b_node>> map_nodeidstr_to_nodeptr;
		std::map<int, std::shared_ptr<b_node>> map_execorder_to_nodeptr;
		std::vector<std::shared_ptr<b_node>> v_ptr_nodes;
		std::shared_ptr<b_node> ptr_root_node;
		int task_id = -1;
		double task_period = -1;

		void addNode(std::shared_ptr<b_node> node);
		void processAllNodes();
		
		void calculateExecOrder(std::shared_ptr<b_node> node);
		void linkNodesPorts();
	};
}

