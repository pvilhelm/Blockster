import xml.etree.ElementTree as ET

__all__ = ["main"]

class main:

    main_top_comment = " /* This code is generated for a Blockster program */ "

    def __init__(self,code_xml):
        self.code_block = ""
        self.header_block = ""
        self.code_xml = code_xml
        self.el_root = code_xml.getroot()
        #
        self.el_tasks = None #Inited in gen_list_task_ids()
        self.list_task_ids = self.gen_list_task_ids()
        self.dict_nodeidlist_by_taskid = {} #List of node ids by task id in dict
        self.dict_taskid_by_nodeid = {} #Reverse of above but no lists
        self.list_node_id = []
        self.list_elexe_orders = []
        self.gen_dict_nodeidlist_by_taskid()
        #
        self.str_program_name = self.el_root.attrib.get("program")
        #
        self.str_common_includes = "" 
        self.gen_common_includes()
        #
        self.str_struct_defs = ""
        self.gen_struct_defs()
        #
        self.str_struct_data = ""
        self.gen_struct_data()

        #
        self.str_update_fcns = ""
        self.gen_update_fcns()
        #
        self.str_main = ""
        self.gen_main()
    
    def gen_main(self):
        self.str_main += self.main_top_comment+"\n"*2

        #Add common headers
        self.str_main += self.str_common_includes+"\n"*2

        #Add struct defs
        self.str_main += self.str_struct_defs+"\n"*2

        #Add struct data
        self.str_main += self.str_struct_data+"\n"*2

        #Add update functions
        self.str_main += self.str_update_fcns+"\n"*2

        #Add main function
        str_mainfcn = "int main(int argc, char *argv[]){\n"
        str_mainfcn+= "\tfor(;;){\n"
        str_mainfcn+= "\t\ttask_0_update();\n"  #TODO hardcoded
        str_mainfcn+= "\t}\n"
        str_mainfcn+= "\treturn 1;\n}\n"

        self.str_main += str_mainfcn

    def gen_struct_data(self):
        list_elstruct_data = self.el_root.findall("Struct_data/Struct_datum")
        for el in list_elstruct_data:
            self.str_struct_data += el.text + "\n" + "\n"



    def gen_dict_nodeidlist_by_taskid(self):
        ellist_exec_order = self.el_root.findall(".//Execution_orders/Execution_order")
        
        for exe in self.el_root.find("Execution_orders"):
            self.list_elexe_orders.append(exe)
            node_id = exe.attrib.get("id")
            self.list_node_id.append(node_id)
            task_id = exe.attrib.get("task_id")
            if(task_id not in self.dict_nodeidlist_by_taskid):
                self.dict_nodeidlist_by_taskid[task_id] = []
            self.dict_nodeidlist_by_taskid[task_id].append(node_id)
            self.dict_taskid_by_nodeid[node_id] = task_id
      
    def gen_list_task_ids(self):
        self.el_tasks = self.el_root.find("Tasks")
        ret_list = []
        for el in self.el_tasks:
            if(el.tag != "Task"):
                continue; 
            ret_list.append(el.attrib.get("id"))
        return sorted(ret_list)

    def gen_update_fcns(self):
        self.list_elupdate = self.el_root.findall("Executable/Update")

        dict_task_by_dict_nodeid_by_execorder = {}
        for el in self.list_elexe_orders:
            ex_order = el.attrib.get("exec_order")
            node_id = el.attrib.get("id")
            task_id = el.attrib.get("task_id")
            if task_id not in dict_task_by_dict_nodeid_by_execorder:
                dict_task_by_dict_nodeid_by_execorder[task_id] = {ex_order:node_id}
            else:
                dict_nodeid_by_execorder = dict_task_by_dict_nodeid_by_execorder[task_id]
                dict_nodeid_by_execorder[ex_order] = node_id

        dict_list_of_updatefcns_by_task = {}

        #init dict with list
        for task_id in self.list_task_ids:
            dict_list_of_updatefcns_by_task[task_id]=[]

        dict_updatefcn_str_by_nodeid = {}

        for el in self.list_elupdate:
            node_id = el.attrib.get("id")
            task_id =  self.dict_taskid_by_nodeid[node_id]
            #list_updatefcn_strs = dict_list_of_updatefcns_by_task[task_id]
            #list_updatefcn_strs.append(el.text)
            dict_updatefcn_str_by_nodeid[node_id] = el.text
            
        for task_id in self.list_task_ids:
            dict_nodeid_by_execorder = dict_task_by_dict_nodeid_by_execorder[task_id]
           
            ex_order = sorted(dict_nodeid_by_execorder.keys())
            str_updatefcn = "void task_"+task_id+"_update(){\n"
            for n in ex_order:
                node_id = dict_nodeid_by_execorder[n]
                if node_id in dict_updatefcn_str_by_nodeid:
                    str_updatefcn += dict_updatefcn_str_by_nodeid[node_id]
            str_updatefcn +="}\n"+ "\n"

            self.str_update_fcns += str_updatefcn
            if self.str_update_fcns[-1]!="\n":
                self.str_update_fcns+="\n"


    def gen_common_includes(self):
        self.el_common_includes = self.el_root.find("Common_includes")
        if(self.el_common_includes == None):
            return False
        if(len(self.el_common_includes)==0):
            return False
        
        #Generate string for common includes
        dict_of_includes = {}
        for el_include in self.el_common_includes:
            if(el_include.tag != "Include"):
                continue
            else:
                str_include = "#include "+el_include.text.strip()+"\n"
                dict_of_includes[el_include.text.strip()] = str_include
        tmp_list = list(dict_of_includes.keys())
        tmp_list.sort()
        for i in tmp_list:
            self.str_common_includes += dict_of_includes[i]
    
        
    def gen_struct_defs(self):
        #Generate string for struct definitions
        dict_struct_defs = {}
        self.el_struct_defs = self.el_root.find("Struct_defs")
        if(self.el_struct_defs is None):
            return False
        if(len(self.el_common_includes)==0):
            return False

        for el_struct_def in self.el_struct_defs:
            if(el_struct_def.tag != "Struct_def"):
                continue
            else:
                str_struct_def = el_struct_def.text
                dict_struct_defs[str_struct_def] = str_struct_def
        tmp_list = list(dict_struct_defs.keys())
        tmp_list.sort()
        for i in tmp_list:
            self.str_struct_defs += dict_struct_defs[i]+ "\n" + "\n"

    def mainToString(self):
        return self.str_common_includes+self.str_struct_defs
