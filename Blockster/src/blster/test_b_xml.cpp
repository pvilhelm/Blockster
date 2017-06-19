#include "catch.hpp"
#include <iostream>

#include "b_node.h"
#include "b_xml.h"
#include "b_node_stash.h"
#include "b_program_tree.h"

using namespace bster;

TEST_CASE("Test b_xml", "[std]") {

	SECTION("Constructor") { ; }

	SECTION("get_node_from_XML_str()") {

		std::string str_node_xml =
			R"asd(<?xml version="1.0" encoding="utf-8"?>
<Node id="gain_0000000">
	<Node_name>Constant 0</Node_name>
	<Node_type type="cpp">
		<Lib_path>core/sources/gain</Lib_path>
	</Node_type>
	<Node_init>
		<Members>
			<Member>
				<Member_value>11</Member_value>
				<Member_name>value</Member_name>
				<Member_type>single</Member_type>
				<Member_tunable>no</Member_tunable>
			</Member>
			<Member>
				<Member_value>22</Member_value>
				<Member_name>value1</Member_name>
				<Member_type>double</Member_type>
				<Member_tunable>yes</Member_tunable>
			</Member>
		</Members>
	</Node_init>
	<Ports>
		<Inports>
		  <Inport id="0">
			<Inport_type>single</Inport_type>
			<Inport_allowed_types>single double int uint</Inport_allowed_types>
			<Inport_source_node_id>mult_001</Inport_source_node_id>
			<Inport_source_node_Outport_id>2</Inport_source_node_Outport_id>
		  </Inport>
		</Inports>
		<Outports>
			<Outport id="0">
				<Outport_target>
						<Outport_target_node_id>constant_001</Outport_target_node_id>
						<Outport_target_node_Inport_id>1</Outport_target_node_Inport_id>
				</Outport_target>
				<Outport_type>single</Outport_type>
			</Outport>
		</Outports>
	</Ports>
	<Node_settings>
	</Node_settings>
	<Node_execution>
	<Node_task>0</Node_task>
		<Node_execution_order>0</Node_execution_order>
	</Node_execution>
	<Node_visualisation>
		<Node_shape shape="rectangle">
			<Node_width>50</Node_width>
			<Node_height>50</Node_height>
		</Node_shape>
		<Node_position>
			<X>1</X>
			<Y>2</Y>
			<Z>3.50</Z>
			<Rotation_degrees>45.1</Rotation_degrees>
		</Node_position>
	  </Node_visualisation>
</Node>)asd";

		b_node node = xml_str_to_node(str_node_xml);
		REQUIRE(node.node_lib_type == "cpp");
		REQUIRE(node.node_pos.x == 1);
		REQUIRE(node.node_pos.y == 2);
		REQUIRE(node.node_pos.z == 3.5);
		REQUIRE(node.node_pos.rot == 45.1);
		REQUIRE(node.node_id == "gain_0000000");
		REQUIRE(node.node_lib_path == "core/sources/gain");
		REQUIRE(node.node_name == "Constant 0");
		REQUIRE(node.node_task_id == "0");
		REQUIRE(node.node_exec_order == 0);
		REQUIRE(node.map_membername_member.size() == 2);
		REQUIRE(node.map_membername_member.at("value").member_name == "value");
		REQUIRE(node.map_membername_member.at("value").member_tunable == false);
		REQUIRE(node.map_membername_member.at("value").member_value == "11");
		REQUIRE(node.map_membername_member.at("value").signal_type == SIGNAL_TYPES::SINGLE);
		REQUIRE(node.map_membername_member.at("value1").member_name == "value1");
		REQUIRE(node.map_membername_member.at("value1").member_tunable == true);
		REQUIRE(node.map_membername_member.at("value1").member_value == "22");
		REQUIRE(node.map_membername_member.at("value1").signal_type == SIGNAL_TYPES::DOUBLE);
		REQUIRE(node.hasPorts());
		REQUIRE(node.getNInports() == 1);
		REQUIRE(node.getNOutports() == 1);
		REQUIRE(node.v_outports[0].dir == PORT_DIRS::OUT);
		REQUIRE(node.v_outports[0].local_port_nr == 0);
		REQUIRE(node.v_outports[0].signal_type == SIGNAL_TYPES::SINGLE);
		REQUIRE(node.v_outports[0].v_pair_remote_node_id_remote_port_nr[0].first == "constant_001");
		REQUIRE(node.v_outports[0].v_pair_remote_node_id_remote_port_nr[0].second == 1);
		REQUIRE(node.v_inports[0].dir == PORT_DIRS::IN);
		REQUIRE(node.v_inports[0].local_port_nr == 0);
		REQUIRE(node.v_inports[0].signal_type == SIGNAL_TYPES::SINGLE);
		REQUIRE(node.v_inports[0].v_pair_remote_node_id_remote_port_nr[0].first == "mult_001");
		REQUIRE(node.v_inports[0].v_pair_remote_node_id_remote_port_nr[0].second == 2);
	}

	SECTION("get_node_from_XML_file()") {

		b_node node;
		REQUIRE_NOTHROW(node = xml_file_to_node(R"(aconstnode.xml)"));
		REQUIRE(node.node_pos.x == 1);
		REQUIRE(node.node_pos.y == 2);
		REQUIRE(node.node_pos.z == 3.5);
		REQUIRE(node.node_pos.rot == 45.1);
		REQUIRE(node.node_id == "gain_0000000");
		REQUIRE(node.node_lib_path == "core/sources/gain");
		REQUIRE(node.node_name == "Constant 0");
		REQUIRE(node.node_task_id == "0");
		REQUIRE(node.node_exec_order == 0);
		REQUIRE(node.map_membername_member.size() == 2);
		REQUIRE(node.map_membername_member.at("value").member_name == "value");
		REQUIRE(node.map_membername_member.at("value").member_tunable == false);
		REQUIRE(node.map_membername_member.at("value").member_value == "11");
		REQUIRE(node.map_membername_member.at("value").signal_type == SIGNAL_TYPES::SINGLE);
		REQUIRE(node.map_membername_member.at("value1").member_name == "value1");
		REQUIRE(node.map_membername_member.at("value1").member_tunable == true);
		REQUIRE(node.map_membername_member.at("value1").member_value == "22");
		REQUIRE(node.map_membername_member.at("value1").signal_type == SIGNAL_TYPES::DOUBLE);
		REQUIRE(node.hasPorts());
		REQUIRE(node.getNInports() == 1);
		REQUIRE(node.getNOutports() == 1);
		REQUIRE(node.v_outports[0].dir == PORT_DIRS::OUT);
		REQUIRE(node.v_outports[0].local_port_nr == 0);
		REQUIRE(node.v_outports[0].signal_type == SIGNAL_TYPES::SINGLE);
		REQUIRE(node.v_outports[0].v_pair_remote_node_id_remote_port_nr[0].first == "constant_001");
		REQUIRE(node.v_outports[0].v_pair_remote_node_id_remote_port_nr[0].second == 1);
		REQUIRE(node.v_inports[0].dir == PORT_DIRS::IN);
		REQUIRE(node.v_inports[0].local_port_nr == 0);
		REQUIRE(node.v_inports[0].signal_type == SIGNAL_TYPES::SINGLE);
		REQUIRE(node.v_inports[0].v_pair_remote_node_id_remote_port_nr[0].first == "mult_001");
		REQUIRE(node.v_inports[0].v_pair_remote_node_id_remote_port_nr[0].second == 2);

		REQUIRE_THROWS(xml_file_to_node(R"(bad_aconstnode.xml)"));
	}

	SECTION("node_to_xml_str()") {
		b_node node;
		std::string node_id = "mult_0";
		t_pos node_pos = { 1,2,3,90 };
		std::string node_task_id = "0";
		int node_execution_order = 0;
		std::string node_lib_path = "core/math/mult";
		std::string node_name = "Multiply 0";
		std::string node_lib_type = "cpp";

		t_port iport0(PORT_DIRS::IN, 0, SIGNAL_TYPES::SINGLE);
		iport0.v_pair_remote_node_id_remote_port_nr.push_back(std::pair<std::string, short>({ "const_0",0 }));
		node.addPort(iport0);
		t_port iport1(PORT_DIRS::IN, 1, SIGNAL_TYPES::SINGLE);
		iport1.v_pair_remote_node_id_remote_port_nr.push_back(std::pair<std::string, short>({ "const_1",0 }));
		node.addPort(iport1);

		t_port oport0(PORT_DIRS::OUT, 0, SIGNAL_TYPES::SINGLE);
		oport0.v_pair_remote_node_id_remote_port_nr.push_back(std::pair<std::string, short>({ "gain_0",0 }));
		node.addPort(oport0);

		node.node_lib_type = node_lib_type;
		node.node_name = node_name;
		node.node_id = node_id;
		node.node_lib_path = node_lib_path;
		node.node_task_id = node_task_id;
		node.node_exec_order = node_execution_order;
		node.node_pos = node_pos;

		t_member member;
		member.member_name = "value";
		member.member_tunable = false;
		member.member_value = "123";
		member.signal_type = SIGNAL_TYPES::SINGLE;

		node.map_membername_member.insert({ "value",member });

		//convert node to string and back again and check if values are still the same 
		std::string answer = node_to_xml_str(node);
		//std::cout << answer;
		b_node ans_node = xml_str_to_node(answer);

		t_member m_ans;
		m_ans = ans_node.map_membername_member["value"];
		 
		REQUIRE(m_ans.member_name == "value");
		REQUIRE(m_ans.member_tunable == false);
		REQUIRE(m_ans.member_value == "123");
		REQUIRE(m_ans.signal_type == SIGNAL_TYPES::SINGLE);
		REQUIRE(ans_node.node_exec_order == node_execution_order);
		REQUIRE(ans_node.node_id == node_id);
		REQUIRE(ans_node.node_lib_path == node_lib_path);
		REQUIRE(ans_node.node_lib_type == node_lib_type);
		REQUIRE(ans_node.node_name == node_name);
		REQUIRE(ans_node.node_pos.x == node_pos.x);
		REQUIRE(ans_node.node_pos.y == node_pos.y);
		REQUIRE(ans_node.node_pos.z == node_pos.z);
		REQUIRE(ans_node.node_pos.rot == node_pos.rot);
		REQUIRE(ans_node.node_task_id == node_task_id);

		REQUIRE(ans_node.v_inports[0].dir == iport0.dir);
		REQUIRE(ans_node.v_inports[0].local_port_nr == iport0.local_port_nr);
		REQUIRE(ans_node.v_inports[0].signal_type == iport0.signal_type);
		REQUIRE(ans_node.v_inports[0].v_pair_remote_node_id_remote_port_nr[0].first 
			== iport0.v_pair_remote_node_id_remote_port_nr[0].first);
		REQUIRE(ans_node.v_inports[0].v_pair_remote_node_id_remote_port_nr[0].second
			== iport0.v_pair_remote_node_id_remote_port_nr[0].second);

		REQUIRE(ans_node.v_inports[1].dir == iport1.dir);
		REQUIRE(ans_node.v_inports[1].local_port_nr == iport1.local_port_nr);
		REQUIRE(ans_node.v_inports[1].signal_type == iport1.signal_type);
		REQUIRE(ans_node.v_inports[1].v_pair_remote_node_id_remote_port_nr[0].first
			== iport1.v_pair_remote_node_id_remote_port_nr[0].first);
		REQUIRE(ans_node.v_inports[1].v_pair_remote_node_id_remote_port_nr[0].second
			== iport1.v_pair_remote_node_id_remote_port_nr[0].second);

		REQUIRE(ans_node.v_outports[0].dir == oport0.dir);
		REQUIRE(ans_node.v_outports[0].local_port_nr == oport0.local_port_nr);
		REQUIRE(ans_node.v_outports[0].signal_type == oport0.signal_type);
		REQUIRE(ans_node.v_outports[0].v_pair_remote_node_id_remote_port_nr[0].first
			== oport0.v_pair_remote_node_id_remote_port_nr[0].first);
		REQUIRE(ans_node.v_outports[0].v_pair_remote_node_id_remote_port_nr[0].second
			== oport0.v_pair_remote_node_id_remote_port_nr[0].second);
	}

	SECTION("program_tree_to_xml()") {

		b_node_stash bns0;

		b_node const_node = xml_file_to_node("valid_const.xml");
		b_node to_console_node = xml_file_to_node("valid_to_console.xml");
		b_node gain_node = xml_file_to_node("valid_gain.xml");

		gain_node.node_task_id = "0";

		const_node.node_id = "const_0";
		const_node.v_outports[0] = (t_port(PORT_DIRS::OUT, 0, SIGNAL_TYPES::SINGLE, "gain_0", 0));
		const_node.node_task_id = "0";
		const_node.node_exec_order = 0;
		bns0.addNode(const_node);
		b_node tmp_gain = gain_node;
		tmp_gain.node_id = "gain_0";
		tmp_gain.v_inports[0] = (t_port(PORT_DIRS::IN, 0, SIGNAL_TYPES::SINGLE, "const_0", 0));
		tmp_gain.v_outports[0] = (t_port(PORT_DIRS::OUT, 0, SIGNAL_TYPES::SINGLE, "gain_1", 0));
		bns0.addNode(tmp_gain);
		for (int i = 1; i < 10;i++) {
			b_node tmp_gain = gain_node;
			tmp_gain.node_id = "gain_" + std::to_string(i);
			tmp_gain.map_membername_member["value"].member_value = std::to_string(i);
			tmp_gain.v_inports[0] = (t_port(PORT_DIRS::IN, 0, SIGNAL_TYPES::SINGLE, "gain_" + std::to_string(i - 1), 0));
			tmp_gain.v_outports[0] = (t_port(PORT_DIRS::OUT, 0, SIGNAL_TYPES::SINGLE, "gain_" + std::to_string(i + 1), 0));
			bns0.addNode(tmp_gain);
		}
		tmp_gain = gain_node;
		tmp_gain.node_id = "gain_10";
		tmp_gain.v_inports[0] = (t_port(PORT_DIRS::IN, 0, SIGNAL_TYPES::SINGLE, "gain_9", 0));
		tmp_gain.v_outports[0] = (t_port(PORT_DIRS::OUT, 0, SIGNAL_TYPES::SINGLE, "to_console_0", 0));
		bns0.addNode(tmp_gain);
		to_console_node.node_id = "to_console_0";
		to_console_node.node_task_id = "0";
		to_console_node.v_inports[0] = (t_port(PORT_DIRS::IN, 0, SIGNAL_TYPES::SINGLE, "gain_10", 0));
		bns0.addNode(to_console_node);

		b_program_tree bpt;
		bpt.program_name = "A test =)";
		bpt.setVectorOfNodes(std::move(bns0.v_nodes));
		bpt.makeTaskVectors();
		bpt.v_tasks[0]->task_period = 0.01;
		//convert back and forth from xml
		std::string str_xml_pt = program_tree_to_xml_str(bpt);
		b_program_tree bpt_ans = xml_str_to_program_tree(str_xml_pt);

		REQUIRE(bpt_ans.program_name == bpt.program_name);
		REQUIRE(bpt_ans.program_name == "A test =)");
		REQUIRE(bpt_ans.v_tasks.size() == 1);
		REQUIRE(bpt_ans.v_tasks.size() == bpt.v_tasks.size());
		REQUIRE(bpt_ans.v_tasks[0]->task_id == 0);
		REQUIRE(bpt_ans.v_tasks[0]->task_id == bpt.v_tasks[0]->task_id);
		REQUIRE(bpt_ans.v_tasks[0]->task_period == bpt.v_tasks[0]->task_period);
		REQUIRE(bpt_ans.v_tasks[0]->task_period == 0.01);
		REQUIRE(bpt_ans.v_tasks[0]->v_ptr_nodes.size() == bpt.v_tasks[0]->v_ptr_nodes.size());
		for (auto k_v : bpt_ans.v_tasks[0]->map_nodeidstr_to_nodeptr) {
			auto key = k_v.first;
			//check that the program trees have all the same node ids
			REQUIRE_NOTHROW(bpt_ans.v_tasks[0]->map_nodeidstr_to_nodeptr.at(key));
		}
	}
}
