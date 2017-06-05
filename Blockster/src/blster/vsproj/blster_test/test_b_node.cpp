#include "catch.hpp"

#include "b_node.h"

#include <numeric>

using namespace bster;

TEST_CASE("Test b_node fundamental", "[std]") {

	SECTION("Constructors") {
		//retard check
		{
			b_node* a = 0;
			REQUIRE_NOTHROW(a = new b_node());
			REQUIRE_NOTHROW(delete a);
			REQUIRE((a = new b_node()));
			delete a;
		}
		//ensure default constructor doesnt change
		{
			b_node b;
			REQUIRE(b.node_id == "");
			REQUIRE(b.node_name == "");
			REQUIRE(b.node_type == "");
			REQUIRE(std::isnan<double>(b.node_pos.x));
			REQUIRE(std::isnan<double>(b.node_pos.y));
			REQUIRE(std::isnan<double>(b.node_pos.z));
			REQUIRE(b.getNInports() == 0);
			REQUIRE(b.getNOutports() == 0);
			REQUIRE_FALSE(b.hasPorts());
		}
		//port
		{
			t_port p;
			REQUIRE(p.dir == bster::PORT_DIRS::INVALID);
			REQUIRE(p.signal_type == bster::SIGNAL_TYPES::INVALID_TYPE);
			REQUIRE(p.port_nr == -1);
		}
		{
			t_port p(PORT_DIRS::IN, 3000);
			REQUIRE(p.dir == PORT_DIRS::IN);
			REQUIRE(p.port_nr == 3000);
			REQUIRE(p.signal_type == SIGNAL_TYPES::INVALID_TYPE);
		}
		{
			t_port p(PORT_DIRS::IN, -1,SIGNAL_TYPES::DOUBLE);
			REQUIRE(p.dir == PORT_DIRS::IN);
			REQUIRE(p.port_nr == -1);
			REQUIRE(p.signal_type == SIGNAL_TYPES::DOUBLE);
		}
		//node_port_ptr
		{
			t_port tp;
			t_node_port_ptr p(tp);
			REQUIRE(p.local_port.dir == PORT_DIRS::INVALID);
			REQUIRE(p.local_port.port_nr == -1);
			REQUIRE(p.local_port.signal_type == SIGNAL_TYPES::INVALID_TYPE);
			REQUIRE(p.remote_node == 0);
			REQUIRE(p.remote_port_nr == -1);

			tp.port_nr = 2000;
			t_node_port_ptr p1(tp,10,(b_node*)100);
			REQUIRE(p1.local_port.dir == PORT_DIRS::INVALID);
			REQUIRE(p1.local_port.port_nr == 2000);
			REQUIRE(p1.local_port.signal_type == SIGNAL_TYPES::INVALID_TYPE);
			REQUIRE(p1.remote_node == (b_node*)100);
			REQUIRE(p1.remote_port_nr == 10);

		}
	}

	SECTION("addPort()")
	{
		{//check that ports inserted in "wrong" order are sorted properly
			b_node b0;
			t_port p0(PORT_DIRS::IN,0,	SIGNAL_TYPES::SINGLE);
			t_port p1(PORT_DIRS::IN, 1, SIGNAL_TYPES::SINGLE);
			t_port p2(PORT_DIRS::IN, 2, SIGNAL_TYPES::SINGLE);
			t_port p3(PORT_DIRS::IN, 3, SIGNAL_TYPES::SINGLE);
			t_port p4(PORT_DIRS::IN, 4, SIGNAL_TYPES::SINGLE);
			t_port p5(PORT_DIRS::IN, 5, SIGNAL_TYPES::SINGLE);
			t_port pp0(PORT_DIRS::OUT, 0, SIGNAL_TYPES::DOUBLE);
			t_port pp1(PORT_DIRS::OUT, 1, SIGNAL_TYPES::DOUBLE);
			t_port pp2(PORT_DIRS::OUT, 2, SIGNAL_TYPES::DOUBLE);
			t_port pp3(PORT_DIRS::OUT, 3, SIGNAL_TYPES::DOUBLE);
			t_port pp4(PORT_DIRS::OUT, 4, SIGNAL_TYPES::DOUBLE);
			t_port pp5(PORT_DIRS::OUT, 5, SIGNAL_TYPES::DOUBLE);
			b0.addPort(p2);
			b0.addPort(p3);
			b0.addPort(p1);
			b0.addPort(p0);
			b0.addPort(p5);
			b0.addPort(p4);

			b0.addPort(pp2);
			b0.addPort(pp3);
			b0.addPort(pp1);
			b0.addPort(pp0);
			b0.addPort(pp5);
			b0.addPort(pp4);

			//port nr is the key for the sort
			//also check that out ports end up in v_parents and in in children 
			short i = 0;
			for (auto pnptr : b0.v_children) {
				REQUIRE(pnptr.local_port.port_nr == i);
				REQUIRE(pnptr.local_port.dir == PORT_DIRS::OUT);
				i++;
			}
			i = 0;
			for (auto pnptr : b0.v_parents) {
				REQUIRE(pnptr.local_port.port_nr == i);
				REQUIRE(pnptr.local_port.dir == PORT_DIRS::IN);
				i++;
			}
		}
		//check that adding an incomplete port to a node throws
		{
			b_node n0;
			t_port p0;
			REQUIRE_THROWS(n0.addPort(p0));
			t_port p1(bster::PORT_DIRS::IN, -1);//invalid port nr
			REQUIRE_THROWS(n0.addPort(p1));
			t_port p2(bster::PORT_DIRS::IN, 0);//valid
			REQUIRE_NOTHROW(n0.addPort(p2));
			t_port p3(bster::PORT_DIRS::UNI, 0);//UNI not implemeted yet
			REQUIRE_THROWS(n0.addPort(p3));
		}

	}

	SECTION("hasPorts()") {
		b_node b0;
		t_port p0(PORT_DIRS::IN, 0);
		REQUIRE_FALSE(b0.hasPorts());
		b0.addPort(p0);
		REQUIRE(b0.hasPorts());
		b_node b1;
		t_port p1(PORT_DIRS::OUT, 0);
		b1.addPort(p1);
		REQUIRE(b1.hasPorts());
		b_node b2;
		t_port p2;
		REQUIRE_THROWS(b2.addPort(p2));
		REQUIRE_FALSE(b2.hasPorts());//no port added since threw
		t_port p3(PORT_DIRS::OUT, 0);
		b2.addPort(p3);
		REQUIRE_THROWS(b2.addPort(p2));
		REQUIRE(b2.hasPorts());
	}

	SECTION("getNInports()/getNOutports()") {
		b_node b0;
		const int n_in = 300;
		const int n_out = 301;
		
		REQUIRE(b0.getNInports() == 0);
		REQUIRE(b0.getNOutports() == 0);

		for (int i = 0; i < n_in; i++) {
			b0.addPort(t_port(PORT_DIRS::IN, i));
		}
		for (int i = 0; i < n_out; i++) {
			b0.addPort(t_port(PORT_DIRS::OUT, i));
		}

		REQUIRE(b0.getNInports() == n_in);
		REQUIRE(b0.getNOutports() == n_out);
		
	}

	SECTION("bsterSignalTypeStringToEnum()") {
		REQUIRE(b_node::bsterSignalTypeStringToEnum("int8") == SIGNAL_TYPES::INT8);
		REQUIRE(b_node::bsterSignalTypeStringToEnum("int16") == SIGNAL_TYPES::INT16);
		REQUIRE(b_node::bsterSignalTypeStringToEnum("int32") == SIGNAL_TYPES::INT32);
		REQUIRE(b_node::bsterSignalTypeStringToEnum("int64") == SIGNAL_TYPES::INT64);
		REQUIRE(b_node::bsterSignalTypeStringToEnum("int128") == SIGNAL_TYPES::INT128);
		REQUIRE(b_node::bsterSignalTypeStringToEnum("uint8") == SIGNAL_TYPES::UINT8);
		REQUIRE(b_node::bsterSignalTypeStringToEnum("uint16") == SIGNAL_TYPES::UINT16);
		REQUIRE(b_node::bsterSignalTypeStringToEnum("uint32") == SIGNAL_TYPES::UINT32);
		REQUIRE(b_node::bsterSignalTypeStringToEnum("uint64") == SIGNAL_TYPES::UINT64);
		REQUIRE(b_node::bsterSignalTypeStringToEnum("uint128") == SIGNAL_TYPES::UINT128);
		REQUIRE(b_node::bsterSignalTypeStringToEnum("single") == SIGNAL_TYPES::SINGLE);
		REQUIRE(b_node::bsterSignalTypeStringToEnum("double") == SIGNAL_TYPES::DOUBLE);
		REQUIRE(b_node::bsterSignalTypeStringToEnum("quad") == SIGNAL_TYPES::QUAD);
		REQUIRE(b_node::bsterSignalTypeStringToEnum("bool") == SIGNAL_TYPES::BOOL);
		REQUIRE(b_node::bsterSignalTypeStringToEnum("inherit") == SIGNAL_TYPES::INHERIT);
		REQUIRE(b_node::bsterSignalTypeStringToEnum("vector") == SIGNAL_TYPES::VECTOR);
		REQUIRE(b_node::bsterSignalTypeStringToEnum("matrix") == SIGNAL_TYPES::MATRIX);
		REQUIRE(b_node::bsterSignalTypeStringToEnum("") == SIGNAL_TYPES::INVALID_TYPE);
		REQUIRE_THROWS(b_node::bsterSignalTypeStringToEnum(" uint8"));
		REQUIRE_THROWS(b_node::bsterSignalTypeStringToEnum(" "));
		REQUIRE_THROWS(b_node::bsterSignalTypeStringToEnum("Double"));
		REQUIRE_THROWS(b_node::bsterSignalTypeStringToEnum("DOUBLE"));
		REQUIRE_THROWS(b_node::bsterSignalTypeStringToEnum("int 8"));
	}

	SECTION("enumSignalTypeToBsterString()") {
		REQUIRE(b_node::enumSignalTypeToBsterString(SIGNAL_TYPES::INT8) == "int8");
		REQUIRE(b_node::enumSignalTypeToBsterString(SIGNAL_TYPES::INT16) == "int16");
		REQUIRE(b_node::enumSignalTypeToBsterString(SIGNAL_TYPES::INT32) == "int32");
		REQUIRE(b_node::enumSignalTypeToBsterString(SIGNAL_TYPES::INT64) == "int64");
		REQUIRE(b_node::enumSignalTypeToBsterString(SIGNAL_TYPES::INT128) == "int128");
		REQUIRE(b_node::enumSignalTypeToBsterString(SIGNAL_TYPES::UINT8) == "uint8");
		REQUIRE(b_node::enumSignalTypeToBsterString(SIGNAL_TYPES::UINT16) == "uint16");
		REQUIRE(b_node::enumSignalTypeToBsterString(SIGNAL_TYPES::UINT32) == "uint32");
		REQUIRE(b_node::enumSignalTypeToBsterString(SIGNAL_TYPES::UINT64) == "uint64");
		REQUIRE(b_node::enumSignalTypeToBsterString(SIGNAL_TYPES::UINT128) == "uint128");
		REQUIRE(b_node::enumSignalTypeToBsterString(SIGNAL_TYPES::SINGLE) == "single");
		REQUIRE(b_node::enumSignalTypeToBsterString(SIGNAL_TYPES::DOUBLE) == "double");
		REQUIRE(b_node::enumSignalTypeToBsterString(SIGNAL_TYPES::QUAD) == "quad");
		REQUIRE(b_node::enumSignalTypeToBsterString(SIGNAL_TYPES::BOOL) == "bool");
		REQUIRE(b_node::enumSignalTypeToBsterString(SIGNAL_TYPES::INHERIT) == "inherit");
		REQUIRE(b_node::enumSignalTypeToBsterString(SIGNAL_TYPES::VECTOR) == "vector");
		REQUIRE(b_node::enumSignalTypeToBsterString(SIGNAL_TYPES::MATRIX) == "matrix");
		REQUIRE(b_node::enumSignalTypeToBsterString(SIGNAL_TYPES::INVALID_TYPE) == "");
		REQUIRE_THROWS(b_node::enumSignalTypeToBsterString(SIGNAL_TYPES::HALF));
		REQUIRE_THROWS(b_node::enumSignalTypeToBsterString(SIGNAL_TYPES::OCTUPLE));
	}
}