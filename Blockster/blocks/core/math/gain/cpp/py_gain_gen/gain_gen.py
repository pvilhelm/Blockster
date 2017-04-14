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
    
    #header_name = gen_node.str_struct_name +".h"

    #header_text = "void "+gen_node.str_struct_name+"(){}" #Empty function, so its not added to the xml tree

    code_update = (
    "{\n"
    "   " + gen_node.str_node_id + ".out_000 = "+gen_node.str_node_id+".value * *"+gen_node.str_node_id+".in_000;\n"
    "}\n") 
  
    gen_node.str_code_update = code_update

    return gen_node.generate_xml()