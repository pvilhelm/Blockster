import xml.etree.ElementTree as ET
import re
import blockster.gen.util as bgu
import blockster.gen.nodeutil as nutil

class node:
    """A class that represents a node in a blockster program and is used for convinience in code generation.
	
    Constructor taxes a xml-element of the node as input."""
    

    def __init__(self,el_node):
        self.el_node = el_node
        self.str_node_id = self.el_node.attrib.get("id")
        self.str_node_name = self.f("Node_name").text
        self.str_node_type = self.att("Node_type","type")
        self.str_libpath = self.f("Node_type/Lib_path").text
        self.str_block_name = re.search(r"[\/]([\w]*$)",self.str_libpath)[1]
        self.list_elmember = self.fa("Node_init/Members/Member")
        self.list_elinport = self.fa("Ports/Inports/Inport")
        self.list_eloutport = self.fa("Ports/Outports/Outport")
        self.str_taskid  = self.f("Node_execution/Node_task").text
        self.str_execorder = self.f("Node_execution/Node_execution_order").text

        self.ret_root_el = ET.Element('Node_gen') #create xml tree for return
        self.ret_root_el.set("id",self.str_node_id) #set same id as in the program specification xml
        self.ret_xml = ET.ElementTree(self.ret_root_el)
         
        self.list_include_strs = []
        # call self.generate_include_list(list_of_include_strings) to generate

        self.dict_codeblocks = {}

        self.str_struct_name = ""
        self.generate_unique_struct_name() #Assigns unique name to self.str_struct_name
        
        self.str_struct_def = ""
        self.generate_struct_def() #Assigns struct definition to self.str_struct_def

        self.str_struct_data =""
        self.generate_struct_data()

        self.str_code_update = ""
        self.str_code_init = ""
        self.str_code_terminate = ""
        self.str_code_derivate = ""
        

        
    def att(self,x,y) : return self.el_node.find(x).attrib.get(y) if self.el_node.find(x) else None
    def f(self,x) : return self.el_node.find(x)
    def fa(self,x) : return self.el_node.findall(x)

    def generate_include_list(self, list_of_include_strings):  
        self.list_include_strs.extend(list_of_include_strings)

    def generate_xml(self):
        root_element = ET.Element('Node_gen') #create xml tree for return
        root_element.set("id",self.str_node_id) #set same id as in the program specification xml
        response_xml = ET.ElementTree(root_element)

        if self.str_struct_data:
            el_struct_data = ET.SubElement(root_element,'Struct_data')
            el_struct_data.text = self.str_struct_data
        if self.str_struct_def:
            el_struct_def = ET.SubElement(root_element,'Struct_def')
            el_struct_def.text = self.str_struct_def
      

        if self.list_include_strs:
            nutil.Add_list_to_elXs_text_to_elY_to_el(self.list_include_strs,"Include","Common_includes",root_element)
        if self.str_code_update:
            nutil.Add_list_to_elXs_text_to_elY_to_el([self.str_code_update],"Update","Executable",root_element)  
        if self.str_code_init: 
            nutil.Add_list_to_elXs_text_to_elY_to_el([self.str_code_init],"Init","Executable",root_element)
        if self.str_code_terminate: 
            nutil.Add_list_to_elXs_text_to_elY_to_el([self.str_code_terminate],"Terminate","Executable",root_element)

        return response_xml
    
    def generate_struct_data(self):

        member_list = self.list_elmember


        
        structname_t = self.str_struct_name+"_t"
        struct_data_text = structname_t +" "+ self.str_node_id +" =\n{\n" 
        intend = len(struct_data_text)
    
        list_of_valuecomment_pairs = []

        if(len(member_list)!=0):
            for i, member in enumerate(member_list):
                member_cpp_value,member_value_name,member_cpp_type,member_value,member_type = ParseMember(member,self.str_node_id)
                comment = "\t// C++: "+member_cpp_type+" "+member_value_name+"\tBlockster: "+member_type+" "+member_value
                value = "\t" + member_cpp_value 
                list_of_valuecomment_pairs.extend([[value,comment]]) 

        inports = self.list_elinport
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
            struct_data_text += pair[0]
            if(i+1 != len(list_of_valuecomment_pairs)):
                struct_data_text += ","+pair[1]+"\n"
            else:
                struct_data_text += pair[1]+"\n"
            ++i

            
        struct_data_text +="};"
        self.str_struct_data = struct_data_text

    def generate_unique_struct_name(self):
        in_types_abb = ""
        if(self.list_elinport):
            in_types_abb += "_I"
            for inport in self.list_elinport:   
                type_el = inport.find("Inport_type")
                assert(type(type_el)==ET.Element),"Missing Inport type element in node {}".format(self.str_node_id)
                type_abb = bgu.AbbreviationFromType(type_el.text)
                assert(type_abb),"Illegal port type in node {}".format(self.str_node_id)
                in_types_abb +=type_abb
        out_types_abb = ""
        if(self.list_eloutport):
            out_types_abb += "_O"
            for outport in self.list_eloutport:   
                type_el = outport.find("Outport_type")
                assert(type(type_el)==ET.Element),"Missing Outport_type element in node {}".format(self.str_node_id)
                type_abb = bgu.AbbreviationFromType(type_el.text)
                assert(type_abb),"Illegal port type in node {}".format(self.str_node_id)
                out_types_abb +=type_abb
    
        tmp = self.str_libpath.replace("_","__")  #so that eg. ./lib_A/ doesn't colide with ./lib/A/
        lib_folders = tmp.rsplit("/")       #TODO: ./lib_A_/B/ will colide with ./lib_A_B ...
        lib_folders.reverse()
        libs ="_"+"_".join(lib_folders)
        self.str_struct_name = self.str_block_name +"_std"+in_types_abb+out_types_abb+libs

    def generate_struct_def(self):
        members = self.list_elmember 
        inports = self.list_elinport
        outports = self.list_eloutport
         
        struct_def_text =  "typedef struct "+self.str_struct_name+ "{\n"
        for member in members:
            member_cpp_value,member_value_name,member_cpp_type,member_value,member_type = ParseMember(member,self.str_node_id)
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
        struct_def_text+= "} "+self.str_struct_name+"_t;"
        self.str_struct_def = struct_def_text 

    def test(self):
        print(self.str_node_name)
        print(self.str_node_id)
        print(self.str_node_type)
        print(self.str_taskid)
        print(self.str_execorder)
        print(self.str_block_name)
        print(self.str_libpath)
        print(self.str_struct_name)
        print(self.str_struct_def)
        print(self.str_struct_data)

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