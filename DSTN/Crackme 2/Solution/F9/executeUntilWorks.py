#-*- coding: utf-8 -*-
import os
import sys
import idc
import idaapi
import idautils
import time

API_FUNC_TO_BP_NAME = "j_VirtualAlloc"
START_FUNC_NAME = "start"
VALUE_TO_GET = 0x400000
NumberOfErrors = 0

def getAddrOfFunctionInCode(func_name):
	'''
	Obtener la referencia en código de una función

	@param func_name: nombre de la función a buscar

	@result: dirección o direcciones de la función
	'''
	address = idc.LocByName(func_name)

	xrefs = idautils.CodeRefsTo(address,True)

	references = []

	for ref in xrefs:
		references.append(ref)

	return references

print "[*] Getting address of start function"
start = idc.LocByName(START_FUNC_NAME)

print "[+] Address of start function: 0x%08x" % (start)


print "[*] Getting address of VirtualAlloc in code"

virtualAllocInCode = getAddrOfFunctionInCode(API_FUNC_TO_BP_NAME)

print "[+] Address in code of VirtualAlloc: 0x%08X" % (virtualAllocInCode[0])

placeForBreakPoint = idc.NextHead(virtualAllocInCode[0])

print "[+] Address to Set Breakpoint: 0x%08X" % (placeForBreakPoint)

print "[+] Setting Breakpoint"

idaapi.add_bpt(placeForBreakPoint,0,idaapi.BPT_SOFT) # establecemos el breakpoint
idaapi.enable_bpt(placeForBreakPoint, True)
print "[+] Breakpoint set"

while(1):
	try:
		idc.StartDebugger("","","")
		idc.GetDebuggerEvent(idc.WFNE_SUSP, -1)
		print "[+] Waiting for the start of debugger..."
		time.sleep(15)
		eax_value = idc.GetRegValue("EAX")

		print "[+] Value of EAX: 0x%08x" % eax_value

		if eax_value == VALUE_TO_GET:
			break
		else:
			idc.StopDebugger()
			print "[+] Waiting for the stop of debugger"
			time.sleep(15)
	except Exception as e:
		print "[-] Error trying again"
		NumberOfErrors += 1
		if NumberOfErrors == 4:
			print "[-] Max Errors, going out"
			exit(-1)

# si salimos aquí todo va bien, quitamos el breakpoint y chapó
idaapi.enable_bpt(placeForBreakPoint,False)
idaapi.del_bpt(placeForBreakPoint)
