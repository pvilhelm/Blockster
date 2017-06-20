#include "catch.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <sstream>

#include "b_node.h"
#include "b_program_tree.h"
#include "b_xml.h"

using namespace bster;



TEST_CASE("Test b_program_tree", "[std]") {
	SECTION("Constructor") {
		b_program_tree* bpt = 0;
		REQUIRE_NOTHROW(bpt = new b_program_tree());
		REQUIRE_NOTHROW(delete bpt);
	}

	SECTION("setVectorOfNodes()") {
		std::vector<std::shared_ptr<b_node>> bns0;
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
			bns0.push_back(std::make_shared<b_node>(n0));
			bns0.push_back(std::make_shared<b_node>(n1));
			bns0.push_back(std::make_shared<b_node>(n2));
		}
		b_program_tree bpt;
		int old_size = bns0.size();
		bpt.setVectorOfNodes(std::move(bns0));
		
		
		REQUIRE(bns0.size() == 0);	//verify that the "old" one is acctually
		//"moved". This is not required by the c++14 standard so if this messes up tests 
		//just remove the test.
		REQUIRE(old_size == bpt.v_nodes.size());
	}

	SECTION("makeTaskVectors()") {
		std::vector<std::shared_ptr<b_node>> bns0;
		const int n = 16;
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
			bns0.push_back(std::make_shared<b_node>(n0));
			bns0.push_back(std::make_shared<b_node>(n1));
			bns0.push_back(std::make_shared<b_node>(n2));
		}
		b_program_tree bpt; 
		int old_size = bns0.size(); 
		bpt.setVectorOfNodes(std::move(bns0)); 

		REQUIRE_NOTHROW(bpt.makeTaskVectors());
		REQUIRE(bpt.v_tasks.size() == n_tasks);
		for (int i = 0; i < bpt.v_tasks.size(); i++) {
			//ensure the task or stored in order by task id
			if (i != bpt.v_tasks.size() - 1)
				REQUIRE(bpt.v_tasks[i]->task_id < bpt.v_tasks[i + 1]->task_id);
		}
		//assure that the temporary v_nodes is acctually cleared
		REQUIRE(bpt.v_nodes.size() == 0);

		//test addNode()
		
		{
			std::vector<std::shared_ptr<b_node>> bns1;
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
					bns1.push_back(std::make_shared<b_node>(n0));
					bns1.push_back(std::make_shared<b_node>(n1));
					bns1.push_back(std::make_shared<b_node>(n2));
			}
			b_program_tree pt0;  
			pt0.setVectorOfNodes(std::move(bns1));
			REQUIRE_THROWS(pt0.makeTaskVectors());//missing one task id
		}
		{//test so task id -1 throws
			std::vector<std::shared_ptr<b_node>> bns1;
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
				bns1.push_back(std::make_shared<b_node>(n0));
				bns1.push_back(std::make_shared<b_node>(n1));
				bns1.push_back(std::make_shared<b_node>(n2));
			}
			b_program_tree pt0;
			pt0.setVectorOfNodes(std::move(bns1));
			REQUIRE_THROWS(pt0.makeTaskVectors());//missing one task id
		}

		
	}

	SECTION("addNode()") {

		std::ifstream is;
		std::stringstream ss;
		is.open("small_pgrm.xml");
		REQUIRE(is.is_open());
		ss << is.rdbuf();

		b_program_tree bpt;
		bpt = xml_str_to_program_tree(ss.str());

		b_node node;
		node.node_task_id = "0";
		node.node_lib_path = "iso/piso";
		node.node_lib_type = "cpp";
		node.node_id = "isolated_0";
		
		bpt.addNode(node);

		auto ptr_node_ans = bpt.v_tasks[0]->map_nodeidstr_to_nodeptr["isolated_0"];
		REQUIRE(ptr_node_ans->node_id == node.node_id);
		REQUIRE(ptr_node_ans->node_exec_order > 0);
	}
}

