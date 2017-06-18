#include "catch.hpp"

#include <string>
#include <iostream>
#include <limits>
//#include <exception>
#include <stdexcept>

#include "b_node_stash.h"
#include "b_node.h"
#include "b_program_tree.h"

using namespace bster;



TEST_CASE("Test b_program_tree", "[std]") {
	SECTION("Constructor") {
		b_program_tree* bpt = 0;
		REQUIRE_NOTHROW(bpt = new b_program_tree());
		REQUIRE_NOTHROW(delete bpt);
	}

	SECTION("setVectorOfNodes()") {
		b_node_stash bns0;
		const int n = 512;
		const int n_tasks = 3;
		for (int i = 0; i < n; i++) {
			b_node n0;
			n0.node_lib_path = "test/type";
			n0.node_id = "type_" + std::to_string(i);
			n0.node_task_id = "0";
			n0.node_exec_order = i;
			b_node n1;
			n1.node_lib_path = "is/another/typety";
			n1.node_id = "typety_" + std::to_string(i);
			n1.node_task_id = "1";
			n1.node_exec_order = i;
			b_node n2;
			n2.node_lib_path = "is/yet/another/typety";
			n2.node_id = "yettypety_" + std::to_string(i);
			n2.node_task_id = "2";
			n2.node_exec_order = i;
			bns0.addNode(n0);
			bns0.addNode(n1);
			bns0.addNode(n2);
		}
		b_program_tree bpt;
		int old_size = bns0.v_nodes.size();
		bpt.setVectorOfNodes(std::move(bns0.v_nodes));
		
		
		REQUIRE(bns0.v_nodes.size() == 0);	//verify that the "old" one is acctually
		//"moved". This is not required by the c++14 standard so if this messes up tests 
		//just remove the test.
		REQUIRE(old_size == bpt.v_nodes.size());
	}

	SECTION("makeTaskVectors()") {
		b_node_stash bns0; 
		const int n = 512;
		const int n_tasks = 3;
		for (int i = 0; i < n; i++) {
			b_node n0; 
			n0.node_lib_path = "test/type"; 
			n0.node_id = "type_" + std::to_string(i); 
			n0.node_task_id = "0"; 
			b_node n1; 
			n1.node_lib_path = "is/another/typety"; 
			n1.node_id = "typety_" + std::to_string(i); 
			n1.node_task_id = "1"; 
			b_node n2; 
			n2.node_lib_path = "is/yet/another/typety"; 
			n2.node_id = "yettypety_" + std::to_string(i); 
			n2.node_task_id = "2"; 
			n0.node_exec_order = i;
			n1.node_exec_order = i;
			n2.node_exec_order = i;
			bns0.addNode(n0); 
			bns0.addNode(n1); 
			bns0.addNode(n2); 
		}
		b_program_tree bpt; 
		int old_size = bns0.v_nodes.size(); 
		bpt.setVectorOfNodes(std::move(bns0.v_nodes)); 

		REQUIRE_NOTHROW(bpt.makeTaskVectors());
		REQUIRE(bpt.v_tasks.size() == n_tasks);
		for (int i = 0; i < bpt.v_tasks.size(); i++) {
			//ensure the task or stored in order by task id
			if (i != bpt.v_tasks.size() - 1)
				REQUIRE(bpt.v_tasks[i]->task_id < bpt.v_tasks[i + 1]->task_id);
		}
		//assure that the temporary v_nodes is acctually cleared
		REQUIRE(bpt.v_nodes.size() == 0);

		{
			b_node_stash bns1; 
			const int n = 8; 
			for (int i = 0; i < n; i++) {
					
					b_node n0; 
					n0.node_lib_path = "test/type"; 
					n0.node_id = "type_" + std::to_string(i); 
					n0.node_task_id = "0"; 
					b_node n1; 
					n1.node_lib_path = "is/another/typety"; 
					n1.node_id = "typety_" + std::to_string(i); 
					n1.node_task_id = "1"; 
					b_node n2; 
					n2.node_lib_path = "is/yet/another/typety"; 
					n2.node_id = "yettypety_" + std::to_string(i); 
					n2.node_task_id = "3"; 
					n0.node_exec_order = i;
					n1.node_exec_order = i;
					n2.node_exec_order = i;
					bns1.addNode(n0);
					bns1.addNode(n1); 
					bns1.addNode(n2); 
			}
			b_program_tree pt0;  
			pt0.setVectorOfNodes(std::move(bns1.v_nodes));
			REQUIRE_THROWS(pt0.makeTaskVectors());//missing one task id
		}
		{//test so task id -1 throws
			b_node_stash bns1;
			const int n = 8;
			for (int i = 0; i < n; i++) {

				b_node n0;
				n0.node_lib_path = "test/type";
				n0.node_id = "type_" + std::to_string(i);
				n0.node_task_id = "0";
				b_node n1;
				n1.node_lib_path = "is/another/typety";
				n1.node_id = "typety_" + std::to_string(i);
				n1.node_task_id = "1";
				b_node n2;
				n2.node_lib_path = "is/yet/another/typety";
				n2.node_id = "yettypety_" + std::to_string(i);
				n2.node_task_id = "-1";
				n0.node_exec_order = i;
				n1.node_exec_order = i;
				n2.node_exec_order = i;
				bns1.addNode(n0);
				bns1.addNode(n1);
				bns1.addNode(n2);
			}
			b_program_tree pt0;
			pt0.setVectorOfNodes(std::move(bns1.v_nodes));
			REQUIRE_THROWS(pt0.makeTaskVectors());//missing one task id
		}
	}
}

#undef MAKE_A_PROGRAM_TREE_0