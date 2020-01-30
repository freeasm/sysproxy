#pragma once
#define WIN32_LEAN_AND_MEAN

#include <stdlib.h>
#include <Windows.h>
#include <WinInet.h>
#include <Ras.h>
#include <RasError.h>

#define DLLEXPORT extern "C" _declspec(dllexport)

enum RET_ERRORS
{
	RET_NO_ERROR = 0,
	INVALID_FORMAT = 1,
	NO_PERMISSION = 2,
	SYSCALL_FAILED = 3,
	NO_MEMORY = 4,
	INVALID_OPTION_COUNT = 5
};