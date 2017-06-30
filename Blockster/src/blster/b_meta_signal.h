#pragma once

#include <vector>
#include <string>
#include <memory>

#include "b_node.h"

namespace bster {

	class b_meta_signal {

	public:
		b_meta_signal();
		~b_meta_signal();

		std::string name = "";
		std::shared_ptr<b_meta_signal_node> start_node;
		
	};

	class b_meta_signal_node {
		public:
			enum class NODE_TYPE {
				START,
				END,
				UNCONNECTED_START,
				UNCONNECTED_END,
				SPLIT
			};

			b_meta_signal_node();
			~b_meta_signal_node();

			std::shared_ptr<b_meta_signal> god;
			std::shared_ptr<b_meta_signal_node> parent;
			std::vector<std::shared_ptr<b_meta_signal_node>> v_children;

	};
}