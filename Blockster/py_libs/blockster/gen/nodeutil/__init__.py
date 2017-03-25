import re
import xml.etree.ElementTree as ET
import blockster.gen.util as bgu


__all__ = ["GenStructDef","ParseMember","GenerateTypeAndLibString","parseSettings","AddCommonIncludesToXMLel"]

def GenStructDef(members,node_id,fnc_name,root_element):
    if(len(members)==0):
        return
    struct_def = ET.SubElement(root_element,'Struct_def')
    struct_def_text =  "typedef struct "+fnc_name+ "{\n"
    for member in members:
        member_cpp_value,member_value_name,member_cpp_type = ParseMember(member,node_id)
        struct_def_text+= "\t"+member_cpp_type+" "+member_value_name+";\n"
                        
    struct_def_text+= "} "+fnc_name+"_t;\n"
    struct_def.text = struct_def_text    

def ParseMember(member,node_id):
    member_type = member.find("Member_type")
    member_value = member.find("Member_value")
    member_value_name = member.find("Member_name")
    member_tunable = member.find("Member_tunable")

    
    if(type(member_tunable)==ET.Element): #If the member is tunable the variable need to be declared volatile
        volatile = "volatile " if member_tunable.text == "yes" else ""
    else:
        volatile = ""
        
    assert(type(member_type)==ET.Element),"Member_type element missing in node id {}".format(node_id)
    assert(type(member_value)==ET.Element),"Member_value element missing in node id {}".format(node_id)
    assert(member_value.text),"Member_value empty in node {}".format(node_id)
    
    #parse valid cpp type from Blockster type
    member_cpp_type = volatile+bgu.CppTypeFromBlocksterType(member_type.text)
    #Parse valid cpp value from blockster value
    member_cpp_value = bgu.CppValueFromBlocksterValue(member_value.text,member_cpp_type)
    assert(member_cpp_value),"Member_value not legal in node {}".format(node_id)
    assert(type(member_value_name)==ET.Element),"Member_name element missing in node id {}".format(node_id)
    member_value_name = member_value_name.text
    assert(member_value_name),"Member_name empty in node {}".format(node_id)

    return member_cpp_value,member_value_name,member_cpp_type

def parseSettings(node):
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

    # out_port_list = []
    # if(out_ports):
        # for out_port in out_ports:
            # if(out_port.tag != "Out_port"):
                # continue
            # out_port_list.append(out_port)

    # in_port_list = []
    # if(in_ports):
        # for in_port in in_ports:
            # if(in_port.tag != "In_port"):
                # continue
            # in_port_list.append(in_port)

    return node_type,block_path,block_name,in_ports,out_ports

def AddCommonIncludesToXMLel(el,include_list):
    """Adds the strings in include_list to text in grandchild elements in Common_includes
     element in xml.etree.Element el"""
    common_includes = el.find('Common_includes')
    if(not common_includes):
        common_includes = ET.SubElement(el,'Common_includes')    
     
    for include in include_list:
        a = ET.SubElement(common_includes,'Include')
        a.text = include

def GenerateTypeAndLibString(in_ports,out_ports,node_id,block_path):
    in_types_abb = ""
    if(in_ports):
        in_types_abb += "_I"
        for inport in in_ports:   
            type_el = in_port.find("Inport_type")
            assert(type(type_el)==ET.Element),"Missing Inport type element in node {}".format(node_id)
            type_abb = bgu.AbbreviationFromType(type_el.text)
            assert(type_abb),"Illegal port type in node {}".format(node_id)
            in_types_abb +=type_abb
    out_types_abb = ""
    if(out_ports):
        out_types_abb += "_O"
        for outport in out_ports:   
            type_el = outport.find("Outport_type")
            assert(type(type_el)==ET.Element),"Missing Outport_type element in node {}".format(node_id)
            type_abb = bgu.AbbreviationFromType(type_el.text)
            assert(type_abb),"Illegal port type in node {}".format(node_id)
            out_types_abb +=type_abb
    
    tmp = block_path.replace("_","__") #so that eg. ./lib_A/ doesn't colide with ./lib/A/
    lib_folders = tmp.rsplit("/")
    lib_folders.reverse()
    libs ="_"+"_".join(lib_folders)

    return libs,out_types_abb,in_types_abb


