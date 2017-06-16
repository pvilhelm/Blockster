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
	//TODO diffirientiate block, comment, alone port etc
	
	//get node id
	std::string node_id;
	xml_attribute attr_node_id = root_node.attribute("id");
	throw_if_not(attr_node_id, "Root missing node id attribute ",__LINE__,__FILE__);
	node.node_id = attr_node_id.as_string();

	//get node name
	string_t node_name = root_node.child_value("Node_name");
	node.node_name = node_name;

	//get node type
	tmp_node = root_node.first_element_by_path("./Node_type");
	throw_if_not(tmp_node, "XML tree missing element Node_type in node id " + node.node_id);
	tmp_attr = tmp_node.attribute("type");
	throw_if_not(tmp_attr, "Element Node_type missing attribute \"type\" in node id "+node.node_id);

	//get lib path
	tmp_node = root_node.first_element_by_path("./Node_type/Lib_path");
	throw_if_not(tmp_node, "XML tree missing element /Node_type/Lib_path in node id " + node.node_id);
	node.node_lib_path = tmp_node.value();

	//get members
	tmp_node = root_node.first_element_by_path("./Node_init/Members");
	
	for (auto i : tmp_node.children("Member")) {
		std::shared_ptr<t_member> tmp_member = std::make_shared<t_member>();
		tmp_member->member_value = i.child("Member_value").text().as_string();
		tmp_member->member_name = i.child("Member_name").text().as_string();
		tmp_member->member_tunable = i.child("Member_tunable").text().as_string() == "yes" ? true : false; 
		node.map_membername_ptrmember.insert({ tmp_member->member_name, std::move(tmp_member) });
	}

	//get outports
	for (auto i : root_node.first_element_by_path("/Ports").children("Outports")) {
		t_port tmp_oport;
		tmp_oport.local_port_nr = i.attribute("id").as_uint();
		SIGNAL_TYPES sig_type = bsterSignalTypeStringToEnum(i.child_value("Member_type"));
	}

	return node;
}

bster::b_program_tree bster::b_xml::getProgramTreefromXml(std::string str_xml)
{
	return b_program_tree();
}


