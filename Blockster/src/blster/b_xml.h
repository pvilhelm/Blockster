#pragma once

#include <string>

#include "b_node.h"
#include "b_program_tree.h"

namespace bster {
	
	b_node get_node_from_XML_str(std::string str_xml); 
	b_node get_node_from_XML_file(std::string str_path);

	b_program_tree get_program_tree_from_XML(std::string str_xml);
	
	std::string node_to_xml_str(const b_node& node);
}