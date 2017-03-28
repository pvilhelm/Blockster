import re
import xml.etree.ElementTree as ET
import blockster.gen.util as bgu


__all__ = ["Add_list_to_elXs_text_to_elY_to_el","GenDataStruct","GenStructDef","ParseMember","GenerateTypeAndLibString","parseSettings","AddCommonIncludesToXMLel"]


    

def GenDataStruct(node,node_id,fnc_name,root_element):

    member_list = node.findall(".//Members/Member")


    struct_data = ET.SubElement(root_element,'Struct_data')
    structname_t = fnc_name+"_t"
    struct_data_text = structname_t +" "+ node_id +" =\n{\n" 
    intend = len(struct_data_text)
    
    list_of_valuecomment_pairs = []

    if(len(member_list)!=0):
        for i, member in enumerate(member_list):
            member_cpp_value,member_value_name,member_cpp_type,member_value,member_type = ParseMember(member,node_id)
            comment = "\t// C++: "+member_cpp_type+" "+member_value_name+"\tBlockster: "+member_type+" "+member_value
            value = "\t" + member_cpp_value 
            list_of_valuecomment_pairs.extend([[value,comment]]) 

    inports = node.find(".//Ports/Inports")
    if(inports):
        for inport in inports:
            portnr = inport.attrib.get("id")
            srcnode = inport.find("Inport_source_node_id").text
            srcportnr = inport.find("Inport_source_node_Outport_id").text   
            porttype = inport.find("Inport_type").text
            comment = "\t// Constant pointer to outport"
            value = "\t"+"&"+srcnode+".out_"+portnr.rjust(3, "0")
            list_of_valuecomment_pairs.extend([[value,comment]])   

    i = 0         
    for pair in list_of_valuecomment_pairs:
        struct_data_text += value
        if(i+1 != len(list_of_valuecomment_pairs)):
            struct_data_text += ","+comment+"\n"
        else:
            struct_data_text += comment+"\n"
        ++i

            
    struct_data_text +="};\n"
    struct_data.text = struct_data_text

def GenStructDef(node,node_id,fnc_name,root_element):
    members = node.find(".//Node_init//Members")
    ports = node.find("Ports")
    
    inports = ports.find("Inports")
    outports = ports.find("Outports")
    struct_def = ET.SubElement(root_element,'Struct_def')
    struct_def_text =  "typedef struct "+fnc_name+ "{\n"
    for member in members:
        member_cpp_value,member_value_name,member_cpp_type,member_value,member_type = ParseMember(member,node_id)
        comment = "\t// Blockster: "+member_type
        struct_def_text+= "\t"+member_cpp_type+" "+member_value_name+";\t"+comment+"\n"
    if(inports):
        for inport in inports:
            portnr = inport.attrib.get("id")
            srcnode = inport.find("Inport_source_node_id").text
            srcportnr = inport.find("Inport_source_node_Outport_id").text   
            porttype = inport.find("Inport_type").text
            comment = "\t// Constant pointer to outport"
            struct_def_text+= "\t"+ bgu.CppTypeFromBlocksterType(porttype) +"* const "+"in_"+portnr.rjust(3, "0")+";"+comment+"\n"
    if(outports):
        for outport in outports:
            portnr = outport.attrib.get("id")
            porttype = outport.find("Outport_type").text
            comment = "\t// Outport"
            struct_def_text+= "\t"+ bgu.CppTypeFromBlocksterType(porttype) +" "+ "out_"+portnr.rjust(3, "0")+";\n"
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

    return member_cpp_value,member_value_name,member_cpp_type,member_value.text,member_type.text

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

    ports = node.find("Ports")
    out_ports = ports.find('Outports')
    in_ports = ports.find('Inports')

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

def Add_list_to_elXs_text_to_elY_to_el(text_list,X,Y,el):
    """Adds the strings in include_list to text in grandchild elements in Common_includes
     element in xml.etree.Element el"""
    el_Y = el.find(Y)
    if(not el_Y):
        el_Y = ET.SubElement(el,Y)    
     
    for text in text_list:
        el_X = ET.SubElement(el_Y,X)
        el_X.text = text

def GenerateTypeAndLibString(in_ports,out_ports,node_id,block_path):
    in_types_abb = ""
    if(in_ports):
        in_types_abb += "_I"
        for inport in in_ports:   
            type_el = inport.find("Inport_type")
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