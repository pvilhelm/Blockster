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
    node_init = node.find("Node_init")
    assert(type(node_init)==ET.Element),"Node id {} missing Node_init element".format(node_id)

    members = node_init.find("Members")
    assert(type(members)==ET.Element),"Missing element Members in Node id {}".format(node_id)
    member = members.find("Member")
    assert(type(member)==ET.Element),"Missing element Member in Node id {}".format(node_id)  
    

    #Generate common program includes e.g. <cstdint>
    include_list = ['<cstdint>']
    nutil.AddCommonIncludesToXMLel(root_element,include_list)

    #Generate function headers for block, specific to this nodes functions
    function_headers = ET.SubElement(root_element,'Function_headers')
    
        #Generate some sub strings with helper function
    libs,out_types_abb,in_types_abb = nutil.GenerateTypeAndLibString(inports,outports,node_id, block_path)

    fnc_name = block_name +"_std"+in_types_abb+out_types_abb+libs
    
    header_name = fnc_name +".h"

    header_text = "void "+fnc_name+"(){}" #Empty function, so its not added to the xml tree

   #Generate the node specic code for init,update,derivate,terminate and reset -- if needed
    
        #Get C++ value and name for member of the node
    

        #Generate a struct definition for the node
    nutil.GenStructDef(members,node_id,fnc_name,root_element)     
    

        #Generate a data struct for the node
    member_cpp_value,member_value_name,member_cpp_type = nutil.ParseMember(member,node_id)

    struct_data = ET.SubElement(root_element,'Struct_data')
    structname_t = fnc_name+"_t"
    struct_data_text = ( structname_t +" "+ node_id +" = { " + member_cpp_value + "};\n" )
    struct_data.text = struct_data_text
    response_xml.write("test.xml","UTF-8")
    return response_xml

