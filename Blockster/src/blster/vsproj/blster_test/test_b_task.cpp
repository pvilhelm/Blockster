#include "catch.hpp"

#include "b_task.h"

using namespace bster;

TEST_CASE("Test b_task", "[std]") {

	SECTION("Constructor") {
		b_task* t = 0;
		REQUIRE_NOTHROW(t = new b_task(222));
		REQUIRE(t->task_id == 222);
		REQUIRE_NOTHROW(delete t);		
	}

	SECTION("addNode()") {
		b_task t0(0);
		b_node n0;
		const int n = 4;
		for (int i = 0; i < 4; i++) {
			t0.addNode(n0);
		}
		REQUIRE(t0.v_nodes.size() == n);
	}
}