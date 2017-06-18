#include <exception>

#include "b_xml.h"
#include "pugixml.hpp"

#include <iostream>
#include <memory>

void throw_if_not(bool cond, std::string message = "", int line = 0, char * file = nullptr) {
	if (!cond) throw std::runtime_error(message +" "+ (line ? std::to_string(line) : "") + (file ? ":"+std::string(file) : ""));
};


bster::b_node get_node_from_XML_doc(std::unique_ptr<pugi::xml_document> doc) {
	using namespace pugi;
	using namespace bster;


	b_node node;
	xml_node tmp_node;
	xml_attribute tmp_attr;

	//get root xml node 
	xml_node root_node = doc->root().first_child();
	throw_if_not(root_node, "Couln't not parse root node", __LINE__, __FILE__);
	//TODO diffirientiate block, comment, alone port etc

	//get node id
	std::string node_id;
	xml_attribute attr_node_id = root_node.attribute("id");
	throw_if_not(attr_node_id, "Root missing node id attribute ", __LINE__, __FILE__);
	node.node_id = attr_node_id.as_string();

	//get node name
	string_t node_name = root_node.child_value("Node_name");
	node.node_name = node_name;

	//get node type
	tmp_node = root_node.first_element_by_path("./Node_type");
	throw_if_not(tmp_node, "XML tree missing element Node_type in node id " + node.node_id);
	tmp_attr = tmp_node.attribute("type");
	throw_if_not(tmp_attr, "Element Node_type missing attribute \"type\" in node id " + node.node_id);

	//get lib path
	tmp_node = root_node.first_element_by_path("./Node_type/Lib_path");
	throw_if_not(tmp_node, "XML tree missing element /Node_type/Lib_path in node id " + node.node_id);
	node.node_lib_path = tmp_node.text().as_string();

	//get members
	tmp_node = root_node.first_element_by_path("./Node_init/Members");

	for (auto i : tmp_node.children("Member")) {
		std::shared_ptr<t_member> tmp_member = std::make_shared<t_member>();
		tmp_member->member_value = i.child("Member_value").text().as_string();
		tmp_member->member_name = i.child("Member_name").text().as_string();
		tmp_member->member_tunable = i.child("Member_tunable").text().as_string() == std::string("yes") ? true : false;
		tmp_member->signal_type = bsterSignalTypeStringToEnum(i.child("Member_type").text().as_string());
		node.map_membername_ptrmember.insert({ tmp_member->member_name, std::move(tmp_member) });
	}

	//get outports
	for (auto i : root_node.first_element_by_path("./Ports/Outports").children("Outport")) {
		t_port tmp_oport;
		tmp_oport.dir = PORT_DIRS::OUT;
		//get id
		tmp_oport.local_port_nr = i.attribute("id").as_uint();
		//get port type
		SIGNAL_TYPES sig_type = bsterSignalTypeStringToEnum(i.child("Outport_type").text().as_string());
		if (sig_type == SIGNAL_TYPES::INHERIT)
			;//TODO further process inherit@members@value etc
		tmp_oport.signal_type = sig_type;

		//iterate over all "Outport_target" and add it to the node 
		auto range_of_targets = i.children("Outport_target");
		for (auto t : range_of_targets) {
			std::pair<std::string, short> target_id_target_port;
			target_id_target_port.first = t.child("Outport_target_node_id").text().as_string();
			target_id_target_port.second = t.child("Outport_target_node_Inport_id").text().as_int();
			tmp_oport.v_pair_remote_node_id_remote_port_nr.push_back(target_id_target_port);
		}
		node.addPort(tmp_oport);
	}

	//get inports
	for (auto i : root_node.first_element_by_path("./Ports/Inports").children("Inport")) {
		t_port tmp_iport;
		tmp_iport.dir = PORT_DIRS::IN;
		//get id
		tmp_iport.local_port_nr = i.attribute("id").as_uint();
		//get port type
		SIGNAL_TYPES sig_type = bsterSignalTypeStringToEnum(i.child("Inport_type").text().as_string());
		if (sig_type == SIGNAL_TYPES::INHERIT)
			;//TODO further process inherit@members@value etc
		tmp_iport.signal_type = sig_type;
		//add target node and target node port nr
		std::pair<std::string, short> target_id_target_port;
		target_id_target_port.first = i.child("Inport_source_node_id").text().as_string();
		target_id_target_port.second = i.child("Inport_source_node_Outport_id").text().as_int();
		tmp_iport.v_pair_remote_node_id_remote_port_nr.push_back(target_id_target_port);
		node.addPort(tmp_iport);
	}

	//get execution task id
	node.node_task_id = root_node.first_element_by_path("./Node_execution/Node_task").text().as_string();
	node.node_exec_order = root_node.first_element_by_path("./Node_execution/Node_execution_order").text().as_int();

	return node;
}



bster::b_node bster::get_node_from_XML(std::string str_xml)
{
	using namespace pugi;

	std::unique_ptr<xml_document> doc = std::make_unique<xml_document>();

	//load doc with string
	xml_parse_result result = doc->load_string(str_xml.c_str());
	if (!result) {
		throw std::runtime_error("XML string could not be loaded: " + std::to_string(__LINE__) + ":" + __FILE__+"\n"+result.description());
	}

	return get_node_from_XML_doc(std::move(doc));;
}

bster::b_node bster::get_node_from_XML_file(std::string str_path)
{
	std::unique_ptr<pugi::xml_document> doc = std::make_unique<pugi::xml_document>();
	pugi::xml_parse_result result = doc->load_file(str_path.c_str());
	if (!result) {
		throw std::runtime_error("XML file could not be loaded: " + std::to_string(__LINE__) + ":" + __FILE__ + "\n" + result.description());
	}
	return get_node_from_XML_doc(std::move(doc));
}

bster::b_program_tree bster::get_program_tree_from_XML(std::string str_xml)
{
	return b_program_tree();
}


