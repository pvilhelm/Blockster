#include <memory>
#include <tuple>
#include <vector>

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
			t0.addNode(std::make_unique<b_node>(n0));
		}
		REQUIRE(t0.v_nodes.size() == n);
	}

	SECTION("processAllNodes()") {
		using namespace std;
		
		//simple chain
		{
			b_task t0(0);

			string node_ids[] = { "const_0","gain_0","gain_1","to_console_0" };

			//root node
			{
				auto n0 = std::make_unique<b_node>();
				n0->node_id = node_ids[0];
				n0->node_task_id = "0";
				n0->node_type = "core/sources/constant";
				n0->node_exec_order = 0;
				t_port p0(PORT_DIRS::OUT, 0, SIGNAL_TYPES::SINGLE);
				p0.v_remote_node_id.push_back(pair<string, short >(node_ids[1], 0));
				n0->addPort(p0);
				t0.addNode(move(n0));
			}
			//2nd gain
			{
				auto n0 = std::make_unique<b_node>();
				n0->node_id = node_ids[2];
				n0->node_task_id = "0";
				n0->node_type = "core/math/gain";
				n0->node_exec_order = 2;
				t_port p0(PORT_DIRS::OUT, 0, SIGNAL_TYPES::SINGLE);
				p0.v_remote_node_id.push_back(pair<string, short >(string(node_ids[3]), 0));
				t_port p1(PORT_DIRS::IN, 0, SIGNAL_TYPES::SINGLE);
				p1.v_remote_node_id.push_back(pair<string, short >(string(node_ids[1]), 0));
				n0->addPort(p0);
				n0->addPort(p1);
				t0.addNode(move(n0));
			}
			//first gain
			{
				auto n0 = std::make_unique<b_node>();
				n0->node_id = node_ids[1];
				n0->node_task_id = "0";
				n0->node_type = "core/math/gain";
				n0->node_exec_order = 1;
				t_port p0(PORT_DIRS::OUT, 0, SIGNAL_TYPES::SINGLE);
				p0.v_remote_node_id.push_back(pair<string, short >(string(node_ids[2]), 0));
				t_port p1(PORT_DIRS::IN, 0, SIGNAL_TYPES::SINGLE);
				p1.v_remote_node_id.push_back(pair<string, short >(string(node_ids[0]), 0));
				n0->addPort(p0);
				n0->addPort(p1);
				t0.addNode(move(n0));
			}
			//

			//terminal to console node
			{
				auto n0 = std::make_unique<b_node>();
				n0->node_id = node_ids[3];
				n0->node_task_id = "0";
				n0->node_type = "core/sinks/to_console";
				n0->node_exec_order = 3;

				t_port p1(PORT_DIRS::IN, 0, SIGNAL_TYPES::SINGLE);
				p1.v_remote_node_id.push_back(pair<string, short >(string(node_ids[2]), 0));

				n0->addPort(p1);
				t0.addNode(move(n0));
			}

			t0.processAllNodes();

			auto ptr_node = t0.v_nodes.front();
			for (int i = 0;; i++) {
				if (!ptr_node->v_outports.size())
					break;
				REQUIRE(ptr_node->node_id == node_ids[i]);
				auto next_ptr = ptr_node->v_outports.front().v_pair_remote_node_portnr.front().first;
				auto this_node_id_according_to_next_node = next_ptr->v_inports.front().v_remote_node_id.front().first;
				REQUIRE(this_node_id_according_to_next_node == ptr_node->node_id);
				ptr_node = next_ptr;
			}
		}

		//chain with some none looping branches
		{
			b_task t0(0);

			//vector of the node ids
			std::vector<string> v_node_ids = { "const_0","gain_0","gain_1","to_console_0","mult_0","const_1"};
			std::vector<string> v_node_types = { "const","gain","gain","to_console","mult","const" };
			//vector of corrensponding ports of these nodes
			auto l = [](t_port p, string trgt_n_id, short port_nr) {p.v_remote_node_id.push_back(pair<string, short>(trgt_n_id, port_nr)); return p;};

			std::vector<std::vector<t_port>> v_v_ports =
			{	{l(t_port(PORT_DIRS::OUT, 0, SIGNAL_TYPES::SINGLE),v_node_ids[1],0)},
				{
					l(t_port(PORT_DIRS::IN,0,SIGNAL_TYPES::SINGLE),v_node_ids[0],0), 
					l(t_port(PORT_DIRS::OUT,0,SIGNAL_TYPES::SINGLE),v_node_ids[2],0)
				},
				{
					l(t_port(PORT_DIRS::IN,0,SIGNAL_TYPES::SINGLE),v_node_ids[1],0), 
					l(t_port(PORT_DIRS::OUT,0,SIGNAL_TYPES::SINGLE),v_node_ids[4],0)
				},
				{
					l(t_port(PORT_DIRS::IN,0,SIGNAL_TYPES::SINGLE),v_node_ids[2],0), 
					l(t_port(PORT_DIRS::OUT,0,SIGNAL_TYPES::SINGLE),v_node_ids[3],0),
					l(t_port(PORT_DIRS::IN,0,SIGNAL_TYPES::SINGLE),v_node_ids[5],0),
				}
			};//todo make finnished

		}

	}
}