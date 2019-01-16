#-*- coding: utf-8 -*-
import os
import sys
import idc
import idaapi

STRUCT_NAME = "gusStructure1"
FIRST_ADDRESS = 0x00D42040
DIFFERENCES = 0x42

def makeStruct(strname = ""):
    '''
    Función para crear la estructura de Gustavo en IDA con idc.

    @param strname: nombre de la estructura

    @return: 0-todo bien, -1-caca
    '''
    sid = idc.GetStrucIdByName(strname)
    if sid == idc.BADADDR:
        sid = idc.AddStrucEx(-1,strname,0)
    else:
        print "[-] Error. Structure %s already exists." % strname
        return -1
    print idc.AddStrucMember(sid,"instructionType",-1,idc.FF_BYTE,-1,1)
    print idc.AddStrucMember(sid,"hashSha",-1,idc.FF_BYTE,-1,20)
    print idc.AddStrucMember(sid,"instructionSize",-1,idc.FF_BYTE,-1,1)
    print idc.AddStrucMember(sid,"branchHash",-1,idc.FF_BYTE,-1,20)
    print idc.AddStrucMember(sid,"nextHash",-1,idc.FF_BYTE,-1,20)
    print idc.AddStrucMember(sid,"salt",-1,idc.FF_DWRD,-1,4)
    
    
    return 0

def MakeStructEx(ea = None,size = -1,strname = "",strid = None):
    '''
    Convierte una dirección de memoria en un item de una estructura

    @param ea: dirección a establecer
    @param size: tamaño de la estructura en bytes.
    @param strname: nombre de la estructura

    @return: 1-todo bien, 0-mierda
    '''
    if strid is None:
        strid = idaapi.get_struc_id(strname)

    if size == -1:
        size = idaapi.get_struc_size(strid)
        
    return idaapi.doStruct(ea,size,strid)

def SetStructName(ea,id_):
    '''
    Función para establecer el nombre de una dirección de memoria

    @param ea: dirección a poner nombre
    @param id_: identificador a poner en el nombre genérico
    '''
    idc.MakeName(ea,"gusInstruction%s" % str(id_))

print "[*] Creating struct if does not exists"

if makeStruct(STRUCT_NAME) == -1:
    print "[+] Struct already created, continue..."
else:
    print "[+] Struct created, continue..."



print '[*] Getting struct id and size:'

strid = idaapi.get_struc_id(STRUCT_NAME)
size = idaapi.get_struc_size(strid)

print "[+] Struct id: %d" % strid
print "[+] Struct size: %d\n\n" % size

fa = FIRST_ADDRESS
for i in range(10):
    print "[*] Setting address 0x%08X as structure" % (fa + (0x42*i))
    MakeStructEx(fa + (0x42*i),size,STRUCT_NAME,strid)
    SetStructName(fa + (0x42*i),i)

