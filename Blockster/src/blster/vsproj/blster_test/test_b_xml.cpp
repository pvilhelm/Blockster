#include "catch.hpp"
#include "b_node.h"
#include "b_xml.h"

using namespace bster;

TEST_CASE("Test b_xml", "[std]") {

	SECTION("Constructor") { ; }

	SECTION("getNodeFromXml()") {

		std::string str_node_xml =
			R"asd(<Node id="constant_0000000">
<Node_name>Constant 0</Node_name>
<Node_type type="cpp">
<Lib_path>core/sources/constant</Lib_path>
</Node_type>
<Node_init>
<Members>
    <Member>
    <Member_value>11</Member_value>
    <Member_name>value</Member_name>
    <Member_type>single</Member_type>
    <Member_tunable>no</Member_tunable>
    </Member>
</Members>
</Node_init>
<Ports>
    <Outports>
    <Outport id="0">
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

		b_node node = b_xml::getNodeFromXml(str_node_xml);
		int a = 1 + 1;
	}
}
