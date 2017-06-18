#include "catch.hpp"
#include "b_node.h"
#include "b_xml.h"
#include <iostream>

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

		b_node node = get_node_from_XML_str(str_node_xml);
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
		REQUIRE(node.map_membername_ptrmember.size() == 2);
		REQUIRE(node.map_membername_ptrmember.at("value")->member_name == "value");
		REQUIRE(node.map_membername_ptrmember.at("value")->member_tunable == false);
		REQUIRE(node.map_membername_ptrmember.at("value")->member_value == "11");
		REQUIRE(node.map_membername_ptrmember.at("value")->signal_type == SIGNAL_TYPES::SINGLE);
		REQUIRE(node.map_membername_ptrmember.at("value1")->member_name == "value1");
		REQUIRE(node.map_membername_ptrmember.at("value1")->member_tunable == true);
		REQUIRE(node.map_membername_ptrmember.at("value1")->member_value == "22");
		REQUIRE(node.map_membername_ptrmember.at("value1")->signal_type == SIGNAL_TYPES::DOUBLE);
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
		REQUIRE_NOTHROW(node = get_node_from_XML_file(R"(aconstnode.xml)"));
		REQUIRE(node.node_pos.x == 1);
		REQUIRE(node.node_pos.y == 2);
		REQUIRE(node.node_pos.z == 3.5);
		REQUIRE(node.node_pos.rot == 45.1);
		REQUIRE(node.node_id == "gain_0000000");
		REQUIRE(node.node_lib_path == "core/sources/gain");
		REQUIRE(node.node_name == "Constant 0");
		REQUIRE(node.node_task_id == "0");
		REQUIRE(node.node_exec_order == 0);
		REQUIRE(node.map_membername_ptrmember.size() == 2);
		REQUIRE(node.map_membername_ptrmember.at("value")->member_name == "value");
		REQUIRE(node.map_membername_ptrmember.at("value")->member_tunable == false);
		REQUIRE(node.map_membername_ptrmember.at("value")->member_value == "11");
		REQUIRE(node.map_membername_ptrmember.at("value")->signal_type == SIGNAL_TYPES::SINGLE);
		REQUIRE(node.map_membername_ptrmember.at("value1")->member_name == "value1");
		REQUIRE(node.map_membername_ptrmember.at("value1")->member_tunable == true);
		REQUIRE(node.map_membername_ptrmember.at("value1")->member_value == "22");
		REQUIRE(node.map_membername_ptrmember.at("value1")->signal_type == SIGNAL_TYPES::DOUBLE);
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

		REQUIRE_THROWS(get_node_from_XML_file(R"(bad_aconstnode.xml)"));
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
		std::string answer = node_to_xml_str(node);

		std::cout << answer;

		b_node ans_node = get_node_from_XML_str(answer);

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
}
