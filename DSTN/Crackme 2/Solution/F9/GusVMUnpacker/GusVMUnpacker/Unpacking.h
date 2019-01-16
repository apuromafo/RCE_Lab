#pragma once



#ifndef UNPACKING_H
#define UNPACKING_H

#include "stdafx.h"
#include "DbgHelp.h"
#include <string.h>
#include "TitanEngine/TitanEngine.h"

void setInformation();
void Unpack(char *name);
void callbackEntryPoint();
void callbackVirtualAllocEnd();
void callbackSetEIPToInstruction();
void callbackSetEIPToApi();
void callbackSetEIPToJCC();
HMODULE getBaseAddressFromFunctionAddress(DWORD address);

#endif