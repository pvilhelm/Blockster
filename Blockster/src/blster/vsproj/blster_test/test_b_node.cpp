#include "catch.hpp"

#include "b_node.h"

using namespace bster;

TEST_CASE("Test b_node fundamental", "[std]") {

	//retard check
	{
		b_node* a = 0;
		REQUIRE_NOTHROW(a = new b_node());
		REQUIRE_NOTHROW(delete a);
		REQUIRE(( a = new b_node()));
		delete a;
	}
	//ensure default constructor doesnt change
	{
		b_node b;
		REQUIRE(b.node_id == "");
		REQUIRE(b.node_name == "");
		REQUIRE(b.node_type == "");
		REQUIRE(b.node_pos.x == 0.);
		REQUIRE(b.node_pos.y == 0.);
		REQUIRE(b.node_pos.z == 0.);
		REQUIRE(b.getNInports() == 0);
		REQUIRE(b.getNOutports() == 0);
		REQUIRE_FALSE(b.hasPorts());
	}

}