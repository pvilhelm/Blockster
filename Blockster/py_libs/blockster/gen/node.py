import xml.etree.ElementTree as ET
import re


class node:
    """A class that represents a node in a blockster program and is used for convinience in code generation.
    
    Constructor taxes a xml-element of the node as input."""

    def __init__(self,el_node):
        self.el_node = el_node
        self.node_id = self.el_node.attrib.get("id")

        def att(x,y) : return self.el_node.find(x).attrib.get(y) if self.el_node.find(x) else None
        def f(x) : return self.el_node.find(x)
        def fa(x) : return self.el_node.findall(x)

        self.str_node_name = f("Node_name").text
        self.str_node_type = att("Node_type","type")
        self.str_libpath = f("Node_type/Lib_path").text
        self.list_elmember = fa("Node_init/Members/Member")
        self.list_elinport = fa("Ports/Inports/Inport")
        self.list_eloutport = fa("Ports/Outports/Outport")
        self.str_taskid  = f("Node_execution/Node_task").text
        self.str_execorder = f("Node_execution/Node_execution_order")