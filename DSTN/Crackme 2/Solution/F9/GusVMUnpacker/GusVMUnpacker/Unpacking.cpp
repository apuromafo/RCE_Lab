#include "stdafx.h"
#include "Unpacking.h"

LPPROCESS_INFORMATION process; // puntero a proceso devuelto por InitDebugEx
long long address;
ULONG_PTR registerValue;
long long moduleProcess;
DWORD fileSize;
BOOL salto;
HANDLE archivoInstrucciones;
DWORD bytesWriten;
char instrucciones[100] = { 0 };

char jumps[][10] = {
	"JO",
	"JNO",
	"JB",
	"JNB",
	"JZ",
	"JNZ",
	"JBE",
	"JNBE",
	"JS",
	"JNS",
	"JP",
	"JNP",
	"JL",
	"JNL",
	"JLE",
	"JNLE",
	"JMP"
};


void setInformation()
/***
*	Función para establecer la información necesaria
*	para el desempaquetado
*/
{
	process = NULL;
	address = NULL;
	registerValue = NULL;
	moduleProcess = NULL;
	fileSize = NULL;
	salto = FALSE;
	// Establecer el tipo de breakpoint
	SetBPXOptions(UE_BREAKPOINT_INT3);

	// abrir el archivo para guardar instrucciones
	archivoInstrucciones = CreateFile("instrucciones_ejecutadas.txt",
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	char cabecera[] = ";=======================================\n"\
		"; Desvirtualizador de VM Aces_v3 de @MZ_IAT\n"\
		"; Autor: F9\n"\
		"; Basado en framework TitanEngine\n"\
		";=======================================\n";
	WriteFile(archivoInstrucciones,
		cabecera,
		strlen(cabecera),
		&bytesWriten,
		NULL);
}

void finishUnpack()
{
	CloseHandle(archivoInstrucciones);
	fprintf(stdout, "[!] Finiquitao la ejecucion de la VM...\n");
}

void Unpack(char *name)
/***
*	Función para comenzar con el debugging
*	y establecer el primer breakpoint
*/
{
	process = (LPPROCESS_INFORMATION) InitDebugEx(name, NULL, NULL, callbackEntryPoint);
	
	if (process) {
		WIN32_FILE_ATTRIBUTE_DATA fad;
		if (!GetFileAttributesExA(name, GetFileExInfoStandard, &fad))
		{
			fprintf(stderr, "[-] Error obteniendo atributos del archivo\n");
			exit(-1);
		}
		fileSize = fad.nFileSizeLow;
		
		DebugLoop();	
		
		finishUnpack();
	}
		
	else
		fprintf(stderr, "[-] Error arrancando el debugger para el archivo %s! Parando ejecucion\n", name);
}

void callbackEntryPoint()
/***
*	Callback para la entrada del programa, aquí nos debemos asegurar
*	que se ejecute correctamente el programa, necesitaremos poner un
*	bp al final de VirtualAlloc y checkear salida.
*/
{
	BOOL result = SetAPIBreakPoint("kernel32.dll", // Nombre de la DLL
		"VirtualAlloc", // nombre de la función 
		UE_BREAKPOINT, // tipo de breakpoint
		UE_APIEND, // dónde establece el breakpoint
		callbackVirtualAllocEnd);

	if (!result)
	{
		fprintf(stderr, "[-] Error al establecer el breakpoint, saliendo!\n");
		StopDebug();
	}


	// Establecer breakpoint en la instrucción que establece el EIP de instrucciones normales
	BYTE searchPattern[] = { 0x89, 0x82, 0xB8, 0x00, 0x00, 0x00, 0x33, 0xC9, 0x8A, 0x8B, 0x55, 0x20 };
	moduleProcess = GetDebuggedFileBaseAddress();
	address = Find((LPVOID)moduleProcess, fileSize, searchPattern, sizeof(searchPattern) / sizeof(BYTE), NULL);
	void *instruction = Disassemble((LPVOID)address);

	fprintf(stdout, "[+] Base archivo debuggeado: 0x%08X\n", moduleProcess);
	fprintf(stdout, "[+] File Size: 0x%08X\n", fileSize);
	fprintf(stdout, "[+] Direccion con patron: 0x%08X\n", address);
	// muestra si es la instrucción que queremos
	fprintf(stdout, "[+] Instruccion a setear breakpoint: %s\n", instruction);
	// Si todo fue bien ponemos el breakpoint en la asignación de dirección de instrucción normal
	result = SetBPX(address, UE_BREAKPOINT, callbackSetEIPToInstruction);
	if (!result)
	{
		fprintf(stderr, "[-] Error estableciendo breakpoint en Set EIP To Instruction\n");
		StopDebug();
	}


	// Establecer breakpoint de funciones de API que se van a ejecutar
	BYTE searchPattern2[] = { 0x89,0x82,0xB8,0x00,0x00,0x00,0xE9,0x74 };
	address = Find((LPVOID)moduleProcess, fileSize, searchPattern2, sizeof(searchPattern2) / sizeof(BYTE), NULL);
	instruction = Disassemble((LPVOID)address);
	// Si todo fue bien ponemos el breakpoint en la asignación de dirección de API
	result = SetBPX(address, UE_BREAKPOINT, callbackSetEIPToApi);
	fprintf(stdout, "[+] Segunda direccion con patron: 0x%08X\n", address);
	fprintf(stdout, "[+] Instruccion a setear breakpoint: %s\n", instruction);
	if (!result)
	{
		fprintf(stderr, "[-] Error estableciendo breakpoint en Set EIP To API\n");
		StopDebug();
	}

	// Establecer breakpoint de saltos
	BYTE searchPattern3[] = { 0xD7,0x83,0xC4,0x14,0x5B,0xC9,0xC2,0x08,0x00 };
	address = Find((LPVOID)moduleProcess, fileSize, searchPattern3, sizeof(searchPattern3) / sizeof(BYTE), NULL);
	instruction = Disassemble((LPVOID)address);
	result = SetBPX(address, UE_BREAKPOINT, callbackSetEIPToJCC);
	fprintf(stdout, "[+] Tercera direccion con patron: 0x%08X\n", address);
	fprintf(stdout, "[+] Instruccion a setear breakpoint: %s\n", instruction);
	if (!result)
	{
		fprintf(stderr, "[-] Error estableciendo breakpoint en Set EIP To JCC\n");
		StopDebug();
	}
}

void callbackVirtualAllocEnd()
/***
*	Callback para el breakpoint en VirtualAlloc,
*	tendremos que ver la salida.
*/
{
	BOOL result = false;
	address = GetContextData(UE_EAX);
	if (address != NULL)
	{
		fprintf(stdout, "[+] Salida de VirtualAlloc: 0x%08X\n", (unsigned int) address);
		result = DeleteAPIBreakPoint("kernel32.dll",
			"VirtualAlloc",
			UE_APIEND);

		if (!result)
		{
			fprintf(stderr, "[-] Error quitando breakpoint!\n");
			StopDebug();
		}

	}
	else
	{
		fprintf(stderr, "[-] Error en VirtualAlloc: 0x%08X\n", (unsigned int) address);
		result = DeleteAPIBreakPoint("kernel32.dll",
			"VirtualAlloc",
			UE_APIEND);

		if (!result)
			fprintf(stderr, "[-] Error quitando breakpoint!\n");

		StopDebug();
	}
}

void callbackSetEIPToInstruction()
/***
*	Función que sacará la instrucción real no perteneciente a API
*	de windows
*/
{
	registerValue = GetContextData(UE_EAX);
	void *instruction = Disassemble((LPVOID)registerValue);

	if (registerValue == 0)
	{
		fprintf(stderr, "[-] Error al obtener el parametro\n");
		StopDebug();
	}

	/*
	fprintf(stdout, "[+] Valor de EAX (puntero instruccion): %08X\n", pointer);
	fprintf(stdout, "[+] Instruccion a ejecutar: %s\n", instruction);
	*/
	if(salto)
	{
		snprintf(instrucciones, 100, " %08Xh\n", registerValue);
		WriteFile(archivoInstrucciones,
			instrucciones,
			strlen(instrucciones),
			&bytesWriten,
			NULL
		);
		fprintf(stdout, " %08Xh\n", registerValue);
		salto = FALSE;
	}
	snprintf(instrucciones, 100, "%08X:\t%s\n", registerValue, instruction);
	WriteFile(archivoInstrucciones,
		instrucciones,
		strlen(instrucciones),
		&bytesWriten,
		NULL
	);
	fprintf(stdout, "%08X:\t%s\n", registerValue, instruction);
}

void callbackSetEIPToApi()
/***
*	Función que sacará la dirección de la API de windows a ejecutar
*/
{
	registerValue = GetContextData(UE_EAX);
	void *instruction = Disassemble((LPVOID)registerValue);
	DWORD64 dwDisplacement = 0;
	char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
	PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;

	pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
	pSymbol->MaxNameLen = MAX_SYM_NAME;

	if (registerValue == 0)
	{
		fprintf(stderr, "[-] Error al obtener el parametro\n");
		StopDebug();
	}
	
	SymInitialize(process->hProcess,
		NULL,
		TRUE);


	if (SymFromAddr(process->hProcess,
		(DWORD64) registerValue,
		&dwDisplacement,
		pSymbol))
	{
		snprintf(instrucciones, 100, "00000000:\tCALL %s\n", pSymbol->Name);
		WriteFile(archivoInstrucciones,
			instrucciones,
			strlen(instrucciones),
			&bytesWriten,
			NULL
		);
		fprintf(stdout, "00000000:\tCALL %s\n",pSymbol->Name);
		
	}else
	{
		fprintf(stderr, "[-] SymFromAddr GetLastError: %d\n", GetLastError());
		snprintf(instrucciones, 100, "00000000:\tCALL 0x%08X\n", registerValue);
		WriteFile(archivoInstrucciones,
			instrucciones,
			strlen(instrucciones),
			&bytesWriten,
			NULL
		);
		fprintf(stdout, "00000000:\tCALL 0x%08X\n", registerValue);
	}
}

void callbackSetEIPToJCC()
/***
*	Función para obtener el tipo de salto de la instrucción
*	de la VM
*/
{
	registerValue = GetContextData(UE_EAX) & 0x000000FF;
	snprintf(instrucciones, 100, "00000000:\t%s", jumps[registerValue]);
	WriteFile(archivoInstrucciones,
		instrucciones,
		strlen(instrucciones),
		&bytesWriten,
		NULL
	);
	fprintf(stdout, "00000000:\t%s", jumps[registerValue]);
	salto = TRUE;
}

HMODULE getBaseAddressFromFunctionAddress(DWORD address)
/***
*	Función para obtener la base de una DLL
*	dada una dirección de una función
*/
{
	DWORD baseAddress = address;

	baseAddress &= 0xFFFFF000;

	while (*((WORD *)baseAddress) != 0x5A4D)
	{
		baseAddress -= 0x1000;
	}

	return (HMODULE)baseAddress;

}