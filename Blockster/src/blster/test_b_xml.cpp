#include "catch.hpp"
#include "b_node.h"
#include "b_xml.h"

using namespace bster;

TEST_CASE("Test b_xml", "[std]") {

	SECTION("Constructor") { ; }

	SECTION("getNodeFromXml()") {

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
</Node>)asd";

		b_node node = get_node_from_XML(str_node_xml);
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
}
