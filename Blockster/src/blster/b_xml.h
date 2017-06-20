#pragma once

#include <string>

#include "b_node.h"
#include "b_program_tree.h"
#include "pugixml.hpp"

namespace bster {

    b_node xml_el_to_node(pugi::xml_node node_el);
    b_node xml_str_to_node(std::string str_xml);
    b_node xml_file_to_node(std::string path);
    std::string node_to_xml_str(const b_node& node);

    b_program_tree xml_file_to_program_tree(std::string path);
    b_program_tree xml_str_to_program_tree(std::string str_xml);
    std::string program_tree_to_xml_str(const b_program_tree& program_tree);

}
