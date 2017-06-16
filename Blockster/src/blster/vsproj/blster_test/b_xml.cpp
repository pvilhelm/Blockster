#include <exception>

#include "b_xml.h"
#include "pugixml.hpp"

#include <iostream>

void throw_if_not(bool cond, std::string message = "", int line = 0, char * file = nullptr) {
	if (!cond) throw std::runtime_error(message +" "+ (line ? std::to_string(line) : "") + (file ? ":"+std::string(file) : ""));
};

bster::b_xml::b_xml()
{
}


bster::b_xml::~b_xml()
{
}

bster::b_node bster::b_xml::getNodeFromXml(std::string str_xml)
{
	using namespace pugi;


	
	b_node node;
	xml_document doc;
	xml_node tmp_node;
	xml_attribute tmp_attr;

	//load doc with string
	xml_parse_result result = doc.load_string(str_xml.c_str());
	if (!result) {
		throw std::runtime_error("XML string could not be loaded: " + std::to_string(__LINE__) + ":" + __FILE__+"\n"+result.description());
	}

	//get root xml node 
	xml_node root_node = doc.root().first_child();
	throw_if_not(root_node, "Couln't not parse root node", __LINE__, __FILE__);
	//get node id

	std::cout << root_node.value() << " " << root_node.name();

	std::string node_id;
	xml_attribute attr_node_id = root_node.attribute("id");
	throw_if_not(attr_node_id, "Root missing node id attribute ",__LINE__,__FILE__);
	node.node_id = attr_node_id.as_string();

	//get node name
	string_t node_name = root_node.child_value("Node_name");
	node.node_name = node_name;

	//get node type
	tmp_node = root_node.first_element_by_path("/Node_type");
	throw_if_not(tmp_node, "XML tree missing element Node_type in node id " + node.node_id);
	tmp_attr = tmp_node.attribute("type");
	throw_if_not(tmp_attr, "Element Node_type missing attribute \"type\" in node id "+node.node_id);


}

bster::b_program_tree bster::b_xml::getProgramTreefromXml(std::string str_xml)
{
	return b_program_tree();
}


