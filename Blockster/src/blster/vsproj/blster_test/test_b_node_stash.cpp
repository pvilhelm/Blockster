#include <memory>

#include "catch.hpp"

#include "b_node_stash.h"
#include "b_node.h"

using namespace bster;

TEST_CASE("Test b_node_stash", "[std]") {

	SECTION("Constructor") {
		b_node_stash* bns;
		REQUIRE_NOTHROW(bns = new b_node_stash);
		REQUIRE(bns);
		REQUIRE_NOTHROW(delete bns);

		b_node_stash bns1;
		REQUIRE(bns1.v_nodes.size() == 0);
	}

	SECTION("addNode()/getCountByNodeType()/verify()/verifyNodes()") {
		{
			b_node_stash bns0;
			const int n = 512;
			for (int i = 0; i < 512; i++) {
				b_node n0;
				n0.node_type = "test/type";
				b_node n1;
				n1.node_type = "is/another/type";
				bns0.addNode(n0);
				bns0.addNode(n1);
			}

			REQUIRE(bns0.getCountByNodeType("test/type") == n);
			REQUIRE(bns0.getCountByNodeType("is/another/type") == n);
			REQUIRE(bns0.getCountByNodeType("not/added") == 0);
			REQUIRE(bns0.getCountByNodeType("not/added") == 0);//still not added
			REQUIRE(bns0.v_nodes.size() == n * 2);
		}
		{
			std::unique_ptr<b_node> n0 = std::make_unique<b_node>();
			REQUIRE_FALSE(b_node_stash::verify(n0));
			n0->node_id = "constant_0000";
			REQUIRE_FALSE(b_node_stash::verify(n0));
			n0->node_type = "is/a/valid/type";
			REQUIRE_FALSE(b_node_stash::verify(n0));
			n0->node_task_id = "1";
			REQUIRE_FALSE(b_node_stash::verify(n0));
			n0->node_pos = pos{ 1., 2., 4. };
			REQUIRE(b_node_stash::verify(n0));
		}
		{
			b_node_stash bns0;
			b_node n0;
			n0.node_type = "is/a/valid/type";
			n0.node_id = "constant_0000";
			n0.node_pos = pos{ 1., 2., 4. };
			n0.node_task_id = "0";

			for (int i = 0; i < 8; i++) {
				bns0.addNode(n0);
			}
			REQUIRE(bns0.verifyNodes());
			bns0.addNode(b_node());
			REQUIRE_FALSE(bns0.verifyNodes());
		}
	}

	 
}