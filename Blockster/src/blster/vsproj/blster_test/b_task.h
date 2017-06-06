#pragma once

#include <vector>

#include "b_node.h"

namespace bster {
	class b_task
	{
	public:
		b_task(int task_id);
		~b_task();

		std::vector<b_node> v_nodes;
		int task_id = -1;

		void addNode(b_node node);
		void processAllNodes();

	};
}

