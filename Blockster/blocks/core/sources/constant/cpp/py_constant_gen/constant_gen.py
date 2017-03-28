import xml.etree.ElementTree as ET
import re
import blockster.gen.util as bgu
import blockster.gen.nodeutil as nutil

def generateFromNode(node):
    """
    Generates a xml tree for the node input that will later be used to generate the program.
    Also generates header and source files for the block if needed.
    """
    
    node_id = node.attrib.get("id")
    assert(node_id),"Node missing attribute id"
    assert_children = ['Node_name','Node_init','Node_execution','Node_type','Node_settings']
    assert(bgu.XmlElHasChildren(node,assert_children)),"Node {} missing child(ren)".format(node_id)

    root_element = ET.Element('Node_gen') #create xml tree for return
    root_element.set("id",node_id) #set same id as in the program specification xml
    response_xml = ET.ElementTree(root_element)
    
    #Parse settings and some pathes
    node_type,block_path,block_name,inports,outports = nutil.parseSettings(node)

    #Parse node init 
    node_init, = bgu.GetXmlElChildren(node,["Node_init"])
    members,  = bgu.GetXmlElChildren(node_init,["Members"])

    #Generate common program includes e.g. <cstdint>
    include_list = ['<cstdint>']
    nutil.Add_list_to_elXs_text_to_elY_to_el(include_list,"Include","Common_includes",root_element)

    #Generate function headers for block, specific to this nodes functions
        #function_headers = ET.SubElement(root_element,'Function_headers')
    
        #Generate some sub strings with helper function
    libs,out_types_abb,in_types_abb = nutil.GenerateTypeAndLibString(inports,outports,node_id, block_path)

    fnc_name = block_name +"_std"+in_types_abb+out_types_abb+libs
    
    header_name = fnc_name +".h"

    header_text = "void "+fnc_name+"(){}" #Empty function, so its not added to the xml tree

   #Generate the node specic code for init,update,derivate,terminate and reset -- if needed
    
        #Get C++ value and name for member of the node
    

        #Generate a struct definition for the node
    nutil.GenStructDef(node,node_id,fnc_name,root_element)     
    

        #Generate a data struct for the node
    nutil.GenDataStruct(node,node_id,fnc_name,root_element)

        #Generate node Execution_order
    code_update = (
"{\n"
"   "+node_id+".out_000 = "+node_id+".value;\n"
"}\n")

    nutil.Add_list_to_elXs_text_to_elY_to_el([code_update],"Update","Executable",root_element)   

    return response_xml

