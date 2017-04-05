
import xml.etree.ElementTree as ET
import re
import os
import sys
import importlib
import blockster.gen.maingen as maingen
import blockster.gen.node as bgnode

this_blockster_ver = "0.0.1"
blockster_libs_path = "C:/Users/Petter/Source/Repos/Blockster/Blockster"
program_xml_path = "C:/Users/Petter/Source/Repos/Blockster/Blockster/program1/program1.xml"
program_folder = re.findall(r"^.*(?=\/)",program_xml_path)

#parse program
prgm_xml = ET.parse(program_xml_path)
root_elem = prgm_xml.getroot()
assert(root_elem)
assert(root_elem.tag == "Blockster")
assert(root_elem.attrib.get("version")==this_blockster_ver)

meta = root_elem.find("Meta")
assert(meta),"Could not find Meta element"
program_name = meta.find("Program_name")
assert(type(program_name)==ET.Element),"could not find Program_name element"
program_name = program_name.text
assert(program_name),"Program_name text field empty"

tasks = root_elem.find("Tasks").findall("Task")
assert(len(tasks)>0)

nodes = root_elem.find("Nodes").findall("Node")
assert(len(nodes)>0)

#Make dict of all nodes
nodes_dict = {}
for node in nodes:
    node_id = node.attrib.get("id")
    assert(node_id) #no node id
    assert(node_id not in nodes_dict) #duplicate node id
    nodes_dict[node_id] = node

#check that all node port target and sources exist and have matching types
for node_id, node in nodes_dict.items():
    ports = node.find("Ports")
    assert(ports)
    Inports = ports.find("Inports")
    if(Inports):
        for Inport in Inports:
            Inport_id = Inport.attrib.get("id")
            assert(Inport_id)
            element_Inport_type = Inport.find("Inport_type")
            assert type(element_Inport_type)==ET.Element, "Inport element missing in node {0} id {1}".format(node.tag,node_id)
            inport_type = element_Inport_type.text
            element_Inport_src_node_id = Inport.find("Inport_source_node_id")
            assert type(element_Inport_src_node_id)==ET.Element, "Inport_source_node_id element missing in node {0} id {1}".format(node.tag,node_id)
            Inport_src_node_id = element_Inport_src_node_id.text
            assert(Inport_src_node_id in nodes_dict), "source node for node in port doesn't exist in xml tree. Source node id {}".format(Inport_src_node_id)
            source_node = nodes_dict[Inport_src_node_id]
            source_node_Outports = source_node.find(".//Ports/Outports")
            assert type(source_node_Outports)==ET.Element,"Outports element missing in node id {}".format(Inport_src_node_id)
            source_node_Outports_ids_dict = {}
            for source_node_Outport in source_node_Outports:
                if source_node_Outport.tag != "Outport":
                    continue  #Element is not an Outport element
                source_node_Outport_id = source_node_Outport.attrib.get("id")
                assert type(source_node_Outport_id)==str, "Source_node out port id attribute missing in node id {}".format(Inport_src_node_id)
                assert(source_node_Outport_id not in source_node_Outports_ids_dict), "multiple outports with same id in node {}".format(Inport_src_node_id)
                source_node_Outports_ids_dict[source_node_Outport_id] = source_node_Outport

            element_Inport_source_node_Outport_id = Inport.find("Inport_source_node_Outport_id")
            assert type(element_Inport_source_node_Outport_id)==ET.Element ,"missing tag in inport in node {}".format(node_id)
            Inport_source_node_Outport_id = element_Inport_source_node_Outport_id.text
            assert(Inport_source_node_Outport_id in source_node_Outports_ids_dict),"Inport and outport id's doesent match {} {}".format(node_id,Inport_src_node_id)
            source_node_Outport = source_node_Outports_ids_dict[Inport_source_node_Outport_id]
            assert type(source_node_Outport)==ET.Element, "source node has no out port of corrensponding in ports id on target node. In node id {} Out node id{}".format(node_id,Inport_src_node_id)
            element_source_node_Outport_type = source_node_Outport.find("Outport_type")
            assert type(element_source_node_Outport_type)==ET.Element,"missing Outport_type tag in node id {}".format(Inport_src_node_id)
            source_node_Outport_type = element_source_node_Outport_type.text
            assert(source_node_Outport_type == inport_type),"Inport type and outport type doesn't match {} {}".format(node_id,Inport_src_node_id)

#make dict of all tasks
tasks_dict = {}
for task in tasks:
    task_id = task.attrib.get("id")
    assert(task_id), "no task id attribute in a task"
    assert(task_id not in tasks_dict), "duplicate task id"
    tasks_dict[task_id]=task

#generate code for all nodes
module_dic = {}
pgrm_gen_tree_root = ET.Element("Generation_tree")
pgrm_gen_tree_root.set("program",program_name)
xml_pgrm_gen_tree = ET.ElementTree(pgrm_gen_tree_root)
    #Generate subelements for the tree
ET.SubElement(pgrm_gen_tree_root,"Common_includes")
ET.SubElement(pgrm_gen_tree_root,"Function_headers")
ET.SubElement(pgrm_gen_tree_root,"Struct_defs")
ET.SubElement(pgrm_gen_tree_root,"Struct_data")
ET.SubElement(pgrm_gen_tree_root,"Execution_orders")
ET.SubElement(pgrm_gen_tree_root,"Tasks")
ET.SubElement(pgrm_gen_tree_root,"Executable")
dict_commonincludes = {}
dict_functionheaders = {}

for node in nodes:
    #i = bgnode.node(node) #test
    node_id = node.attrib.get("id")
    node_type = node.find("Node_type")
    assert(node_type), "Node type element missing in node {}".format(node_id)
    generation_type = node_type.attrib.get("type")
    assert(generation_type),"Missing Node_type attribute type in node {}".format(node_id)
    if(generation_type == "cpp"):
        element_block_path = node_type.find("Lib_path")
        assert type(element_block_path)==ET.Element,"Node {} missing element Lib_path".format(node_id)
        block_path = element_block_path.text
        block_name = re.search(r"[\/]([\w]*$)",block_path)
        assert(block_name),"Could not find block name from block path {}".format(block_path)
        block_name = block_name.group(1)
        assert(block_path),"Node {} missing element Lib_path text field".format(node_id)
        py_cpp_generator_path = blockster_libs_path +"/blocks/"+block_path+"/cpp/py_"+block_name+"_gen"
        #os_path = os.path.abspath(py_cpp_generator_path)
        #if os_path not in sys.path:
        #   sys.path.append(os_path)
        #gen_module = importlib.import_module(block_name + "_gen")
        gen_file_path = py_cpp_generator_path+"/"+block_name + "_gen.py"
        if gen_file_path not in module_dic:
            spec = importlib.util.spec_from_file_location(block_name + "_gen",gen_file_path)
            gen_module = importlib.util.module_from_spec(spec)
            spec.loader.exec_module(gen_module)
            module_dic[gen_file_path]=gen_module
        else:
            gen_module = module_dic[gen_file_path]

        xml_node_gen_tree = gen_module.generateFromNode(node)
        #parse xml_gen_tree 
        node_gen_root = xml_node_gen_tree.getroot()
        assert(type(node_gen_root)==ET.Element),"Could not retrive xml_node_gen_tree root element node id{}".format(node_id)
        node_id = node_gen_root.attrib.get("id")
        assert(node_id)
        prgrm_commonincludes = pgrm_gen_tree_root.find("Common_includes")
        node_commonincludes = node_gen_root.find("Common_includes")
        if(node_commonincludes is not None):
            for element in node_commonincludes:
                if(element.tag != "Include"):
                    continue
                incl = element.text
                
                if (incl not in dict_commonincludes):
                    dict_commonincludes[incl]=incl
                    el_incl = ET.SubElement(prgrm_commonincludes,"Include")
                    el_incl.text = incl
        el_struct_data = node_gen_root.find("Struct_data")
        if(el_struct_data is not None):
            prgrm_strctdata = pgrm_gen_tree_root.find("Struct_data")
            if(not re.match(r"\s*$",el_struct_data.tag)):
                el_tmp = ET.Element("Struct_datum",{"id":node_id})
                el_tmp.text = el_struct_data.text
                prgrm_strctdata.append(el_tmp)
        el_struct_def = node_gen_root.find("Struct_def")
        if(el_struct_def is not None):
            prgrm_strctdefs = pgrm_gen_tree_root.find("Struct_defs")
            if(not re.match(r"\s*$",el_struct_def.tag)):
                el_tmp = ET.Element("Struct_def",{"id":node_id})
                el_tmp.text = el_struct_def.text
                prgrm_strctdefs.append(el_tmp)
        el_node_exe = node_gen_root.find("Executable")
        if(el_node_exe is not None):
            el_prgrm_exe = pgrm_gen_tree_root.find("Executable")
            for el in el_node_exe:
                if(not re.match(r"\s*$",el.tag)):
                    el_tmp = ET.Element(el.tag,{"id":node_id})
                    el_tmp.text = el.text
                    el_prgrm_exe.append(el_tmp)

        node_id = node.attrib.get("id")
        task_id = node.find(".//Node_execution/Node_task").text
        node_exec_order = node.find(".//Node_execution/Node_execution_order")
        pgrm_execution_orders = pgrm_gen_tree_root.find('Execution_orders')
        el_tmp = ET.SubElement(pgrm_execution_orders,"Execution_order",{"id":node_id,"task_id":task_id,"exec_order":node_exec_order.text})
        
for task in tasks:
    task_id = task.attrib.get("id")
    str_task_period = task.find("Task_period").text
    prgrm_tasks = pgrm_gen_tree_root.find("Tasks")
    prgrm_tasks.append(ET.Element("Task",{"id":task_id,"period":str_task_period}))

xml_pgrm_gen_tree.write("Gen_code_tree.xml","UTF-8")

main = maingen.main(xml_pgrm_gen_tree)

f = open("program1.cpp","w")
f.write(main.str_main)
f.close()