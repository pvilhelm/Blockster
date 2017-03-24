import xml.etree.ElementTree as ET
import re

def generateFromNode(node):
    """
    Generates a xml tree for the node input that will later be used to generate the program.
    Also generates header and source files for the block if needed.
    """
 
    node_id = node.attrib.get("id")
    assert(node_id),"Node missing attribute id"

    root_element = ET.Element('Node_gen') #create xml tree for return
    root_element.set("id",node_id) #set same id as in the program specification xml
    response_xml = ET.ElementTree(root_element)

    #Parse settings
    node_type = node.find("Node_type")
    assert(node_type), "Node type element missing in node {}".format(node_id)
    element_block_path = node_type.find("Lib_path")
    assert(type(element_block_path)==ET.Element),"Missing Lib_path element in node {}".format(node_id)
    block_path = element_block_path.text
    block_name = re.search(r"[\/]([\w]*$)",block_path)
    assert(block_name),"Could not find block name from block path {}".format(block_path)
    block_name = block_name.group(1)

    node_settings = node.find("Node_settings")
    assert(type(node_settings)==ET.Element),"No node settings element in node {}".format(node_id)
    
    out_ports = node_settings.find('Outports')
    in_ports = node_settings.find('Inports') 
    
    out_port_list = []
    if(out_ports):
        for out_port in out_ports:
            if(out_port.tag != "Out_port"):
                continue
            out_port_list.append(out_port)
    
    in_port_list = []
    if(in_ports):
        for in_port in in_ports:
            if(in_port.tag != "In_port"):
                continue
            in_port_list.append(in_port)

    #TODO: add support for tunable member

    #Parse node init
    node_init = node.find("Node_init")
    assert(type(node_init)==ET.Element),"Node id {} missing Node_init element".format(node_id)

    members = node_init.find("Members")
    assert(type(members)==ET.Element),"Missing element Members in Node id {}".format(node_id)
    member = members.find("Member")
    assert(type(member)==ET.Element),"Missing element Member in Node id {}".format(node_id)  
    

    #Generate common program includes e.g. <cstdint>
    common_includes = ET.SubElement(root_element,'Common_includes')
    includes = ['<cstdint>']
    for include in includes:
        a = ET.SubElement(common_includes,'Include')
        a.text = include

    #Generate function headers for block, specific to this nodes functions
    function_headers = ET.SubElement(root_element,'Function_headers')
    
    in_types_abb = ""
    if(in_ports):
        in_types_abb += "_I"
        for inport in in_ports:   
            type_el = in_port.find("Inport_type")
            assert(type(type_el)==ET.Element),"Missing Inport type element in node {}".format(node_id)
            type_abb = AbbreviationFromType(type_el.text)
            assert(type_abb),"Illegal port type in node {}".format(node_id)
            in_types_abb +=type_abb
    out_types_abb = ""
    if(out_ports):
        out_types_abb += "_O"
        for outport in out_ports:   
            type_el = out_port.find("Outport_type")
            assert(type(type_el)==ET.Element),"Missing Outport_type element in node {}".format(node_id)
            type_abb = AbbreviationFromType(type_el.text)
            assert(type_abb),"Illegal port type in node {}".format(node_id)
            out_types_abb +=type_abb
    
    tmp = block_path.replace("_","__") #so that eg. ./lib_A/ doesn't colide with ./lib/A/
    lib_folders = tmp.rsplit("/")
    lib_folders.reverse()
    libs ="_"+"_".join(lib_folders)

    fnc_name = block_name +"_std"+in_types_abb+out_types_abb+libs
    
    header_name = fnc_name +".h"

    header_text = "void "+fnc_name+"(){}" #Empty function, so its not added to the xml tree

        #Generate the node specic code for init,update,derivate,terminate and reset -- if needed
    
        #Generate structure to put in program namespace
    
    member_type = member.find("Member_type")
    assert(type(member_type)==ET.Element),"Member_type element missing in node id {}".format(node_id)
    member_cpp_type = CppTypeFromBlocksterType(member_type.text)
    member_value = member.find("Member_value")
    assert(type(member_value)==ET.Element),"Member_value element missing in node id {}".format(node_id)
    member_cpp_value = CppValueFromBlocksterValue(member_value.text,member_cpp_type)
    assert(member_value.text),"Member_value empty in node {}".format(node_id)
    assert(member_cpp_value),"Member_value not legal in node {}".format(node_id)
    member_value_name = member.find("Member_name")
    assert(type(member_value_name)==ET.Element),"Member_name element missing in node id {}".format(node_id)
    member_value_name = member_value_name.text
    assert(member_value_name),"Member_name empty in node {}".format(node_id)

        #Generate a struct definition for the node
    struct_def = ET.SubElement(root_element,'Struct_def')
    struct_def_text = ( "typedef struct "+fnc_name+ " {\n"+
                        "\t"+member_cpp_value+" "+member_value_name+";\n"
                        "} "+fnc_name+"_t;\n")
    struct_def.text = struct_def_text

        #Generate a data struct for the node
    struct_data = ET.SubElement(root_element,'Struct_data')
    struct_t = fnc_name+"_t"
    struct_data_text = ( "struct_t " + node_id +" = { " + member_cpp_value + "};\n" )
    struct_data.text = struct_data_text

       

    return response_xml

def CppValueFromBlocksterValue(s,v_type):
    if(v_type == "float"):
        s.lower()
        reg_match = re.search(r"^(\d*)([\.])?(\d*)([-+]?[eE]?\d+)$",s)
        assert(reg_match),"Member_value {} not valid {}".format(s,v_type)
        return s+"f";

def AbbreviationFromType(s):
    if(s == "single"):
        return "s"
    if(s == "double"):
        return "d"
    if(s == "quad"):
        return "q"
    if(s == "uint8"):
        return "ui8"
    if(s == "uint16"):
        return "ui16"
    if(s == "uint32"):
        return "ui32"
    if(s == "uint64"):
        return "ui64"
    if(s == "uint128"):
        return "ui128"
    if(s == "int8"):
        return "i8"
    if(s == "int16"):
        return "i16"
    if(s == "int32"):
        return "i32"
    if(s == "int64"):
        return "i64"
    if(s == "int128"):
        return "i128"
    return None
        
def CppTypeFromBlocksterType(s):
    if(s == "single"):
        return "float"
    if(s == "double"):
        return "double"
    if(s == "quad"):
        return "quad" #TODO: Implement quad
    if(s == "uint8"):
        return "uint8_t"
    if(s == "uint16"):
        return "uint16_t"
    if(s == "uint32"):
        return "uint32_t"
    if(s == "uint64"):
        return "uint64_t"
    if(s == "uint128"):
        return "uint128_t"
    if(s == "int8"):
        return "int8_t"
    if(s == "int16"):
        return "int16_t"
    if(s == "int32"):
        return "int32_t"
    if(s == "int64"):
        return "int64_t"
    if(s == "int128"):
        return "int128_t"
    return None