#pragma once

#include <string>

#include "b_node.h"
#include "b_program_tree.h"

namespace bster {
	class b_xml
	{
	public:
		b_xml();
		~b_xml();

		b_node static getNodeFromXml(std::string str_xml);
		b_program_tree static getProgramTreefromXml(std::string str_xml);
	};

}