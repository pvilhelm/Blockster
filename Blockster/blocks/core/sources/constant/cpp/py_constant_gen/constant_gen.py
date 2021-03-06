import xml.etree.ElementTree as ET
import re
import blockster.gen.util as bgu
import blockster.gen.nodeutil as nutil
import blockster.gen.node as Node

def generateFromNode(node):
    """
    Generates a xml tree for the node input that will later be used to generate the program.
    Also generates header and source files for the block if needed.
    """
    gen_node = Node.node(node)

    #Generate common program includes e.g. <cstdint>
    include_list = ['<cstdint>']
    gen_node.generate_include_list(include_list)

    code_update = (
    "{\n"
    "   "+gen_node.str_node_id+".out_000 = "+gen_node.str_node_id+".value;\n"
    "}\n")

    #nutil.Add_list_to_elXs_text_to_elY_to_el([code_update],"Update","Executable",root_element)   
    gen_node.str_code_update = code_update

    return gen_node.generate_xml()

