__all__ = ["GetXmlElChildren","XmlElHasChildren","CppTypeFromBlocksterType","AbbreviationFromType","CppValueFromBlocksterValue"]

import re

def CppValueFromBlocksterValue(s,v_type_in):
    """Returns corrensponding string of C++ value for a string of Blockster value.
    s = string to parse
    v_type = string of C++ type to convert to
    
    Asserts proper value syntax"""
    s = s.lower()

    v_type = v_type_in.split()  #remove any "const","register" or "volatile" etc
    v_type = v_type[-1]         # from eg "volatile float"
    
    if(v_type == "float"):
        
        reg_match = re.match(r"^[+-]?((\d+\.\d?)|(\d?\.\d+)|(\d+))([Ee][+-]?\d+)?$",s)
        assert(reg_match),"Member_value {} not valid {}".format(s,v_type)
        if(s.find(".")!=-1):
            return s+"f"
        elif(s.find("e")!=-1 and s.find("E")!=-1):
            return s+"f"
        else:
            return s+".0f"
    if(v_type == "double"):

        reg_match = re.match(r"^[+-]?((\d+\.\d?)|(\d?\.\d+)|(\d+))([Ee][+-]?\d+)?$",s)
        assert(reg_match),"Member_value {} not valid {}".format(s,v_type)
        if(s.find(".")!=-1):
            return s
        elif(s.find("e")!=-1 and s.find("E")!=-1):
            return s
        else:
            return s+".0"   
    if(re.match(r"^uint\d+_t$",s)):#Parse uintxxx:s
        
        reg_match = re.match(r"^[+]?[1-9]\d*$",s)
        assert(reg_match),"Member_value {} not valid {}".format(s,v_type)
        return s
    if(re.match(r"^int\d+_t$",s)):#Parse intxxx:s
        
        reg_match = re.match(r"^[-+]?[1-9]\d*$",s)
        assert(reg_match),"Member_value {} not valid {}".format(s,v_type)
        return s

def AbbreviationFromType(s):
    """Returns an arbitary but unique abbreviation from a Blockster type."""
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
    """Convert string with Blockster type to string with corrensponding C++ type."""
    if(s == "single"):
        return "float"
    if(s == "double"):
        return "double"
    if(s == "quad"):
        return "quad" 
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

def XmlElHasChildren(node,children_list,unique = True):
    """Checks if an xml etree element has all children specifiec in children_list.
             node:  The xml.etree.Element to check
    children_list:  List of strings with children tag names
    
    Return True or False"""
    for child in children_list:
        if(not unique):
            ans = node.find(child)
            if(type(ans)!=ET.Element):
                return False
        else:
            ans = node.findall(child)
            if(len(ans)!=1):
                return False

    return True

def GetXmlElChildren(node,children_list,assertion = True):
    """Checks if an xml etree element has all children specifiec in children_list.
             node:  The xml.etree.Element to check
    children_list:  List of strings with children tag names
    
    Return tuple of children; or empty tuple, but throws if assertion is on"""
    ret_list = []
    for child in children_list:
        child_list = node.findall(child)
        if(len(child_list)==0):
            if(assertion):
                 raise RuntimeError("Missing child in node {} {}".format(node,node.items()))
            return None
        ret_list.extend(child_list)

    return tuple(ret_list)