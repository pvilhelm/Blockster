
import xml.etree.ElementTree as ET

def generate(path, node):
    Node_settings = node.find("Node_settings")
    if(len(Node_settings)>0):
        nop     #TODO: pase node settings
    else:
        nop

    node_id = node.attrib.get("id")
    assert(node_id)
    
    
    export_root = ET.Element("Node")
    export_root.set("version","0.0.1")
    export_root.set("id",node_id)
    
    export_xml = ET.ElementTree(export_root)
    
    includes = ET.SubElement(export_root,"Includes")
    includes_str = ""

    node_structs = ET.SubElement(export_root,"Node_structs")
    node_struct_str = ""

    node_init = ET.SubElement(export_root,"Node_init")
    node_init_str = ""

    node_update = ET.SubElement(export_root,"Node_update")
    node_node_update_str = ""

    node_terminate = ET.SubElement(export_root,"Node_terminate")
    node_terminate_str = ""