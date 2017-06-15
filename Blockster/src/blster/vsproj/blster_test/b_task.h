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

		std::vector<std::shared_ptr<b_node>> v_nodes;
		int task_id = -1;

		void addNode(std::unique_ptr<b_node> node);
		void processAllNodes();
		
		void calculateExecOrder(std::shared_ptr<b_node> node);
		void linkNodesPorts(std::shared_ptr<b_node> node, bool is_root = false);

		std::map<std::string, std::shared_ptr<b_node>> map_nodeidstr_to_nodeptr;
		std::map<int, std::shared_ptr<b_node>> map_execorder_to_nodeptr;
		
	};
}

