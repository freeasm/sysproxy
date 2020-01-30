#include "pch.h"

int initialize(INTERNET_PER_CONN_OPTION_LIST* options, int count)
{
	if (count < 1)
	{
		return INVALID_OPTION_COUNT;
	}

	DWORD dwBufferSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
	options->dwSize = dwBufferSize;
	options->dwOptionCount = count;
	options->dwOptionError = 0;
	options->pOptions = (INTERNET_PER_CONN_OPTION*)calloc(count, sizeof(INTERNET_PER_CONN_OPTION));

	if (!options->pOptions)
	{
		return NO_MEMORY;
	}

	options->pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;
    return RET_NO_ERROR;
}

int _apply(INTERNET_PER_CONN_OPTION_LIST* options, LPTSTR conn)
{
    options->pszConnection = conn;

    if (!InternetSetOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, options, sizeof(INTERNET_PER_CONN_OPTION_LIST)))
    {
        return SYSCALL_FAILED;
    }

    if (!InternetSetOption(NULL, INTERNET_OPTION_PROXY_SETTINGS_CHANGED, NULL, 0))
    {
        return SYSCALL_FAILED;
    }

    if (!InternetSetOption(NULL, INTERNET_OPTION_REFRESH, NULL, 0))
    {
        return SYSCALL_FAILED;
    }

    return RET_NO_ERROR;
}

int apply(INTERNET_PER_CONN_OPTION_LIST* options)
{
	DWORD dwCb, dwRet, dwEntries;
	dwCb = dwEntries = 0;

	dwRet = RasEnumEntries(NULL, NULL, NULL, &dwCb, &dwEntries);
	if (dwRet == ERROR_BUFFER_TOO_SMALL)
	{
		LPRASENTRYNAME lpRasEntryName = (LPRASENTRYNAME)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwCb);

        if (lpRasEntryName == NULL)
        {
            return NO_MEMORY;
        }

        for (DWORD i = 0; i < dwEntries; i++)
        {
            lpRasEntryName[i].dwSize = sizeof(RASENTRYNAME);
        }

        dwRet = RasEnumEntries(NULL, NULL, lpRasEntryName, &dwCb, &dwEntries);

        int ret;
        if (ERROR_SUCCESS != dwRet)
        {
            ret = SYSCALL_FAILED;
        }
        else
        {
            ret = _apply(options, NULL);

            for (DWORD i = 0; i < dwEntries && ret == 0; i++)
            {
                ret = _apply(options, lpRasEntryName[i].szEntryName);
            }
        }

        HeapFree(GetProcessHeap(), 0, lpRasEntryName);

        return ret;
	}

    return _apply(options, NULL);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

DLLEXPORT BOOL SetDIRECT()
{
    INTERNET_PER_CONN_OPTION_LIST options;
	initialize(&options, 1);

	options.pOptions[0].Value.dwValue = PROXY_TYPE_AUTO_DETECT | PROXY_TYPE_DIRECT;

    BOOL result = apply(&options) == RET_NO_ERROR ? TRUE : FALSE;

    free(options.pOptions);
    return result;
}

DLLEXPORT BOOL SetGlobal(LPTSTR remote, LPTSTR bypass)
{
    INTERNET_PER_CONN_OPTION_LIST options;
    initialize(&options, 3);

    options.pOptions[0].Value.dwValue = PROXY_TYPE_PROXY | PROXY_TYPE_DIRECT;
    options.pOptions[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;
    options.pOptions[1].Value.pszValue = remote;
    options.pOptions[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
    options.pOptions[2].Value.pszValue = bypass;

    BOOL result = apply(&options) == RET_NO_ERROR ? TRUE : FALSE;

    free(options.pOptions);
    return result;
}

DLLEXPORT BOOL SetURL(LPTSTR remote)
{
    INTERNET_PER_CONN_OPTION_LIST options;
    initialize(&options, 2);

    options.pOptions[0].Value.dwValue = PROXY_TYPE_AUTO_PROXY_URL | PROXY_TYPE_DIRECT;
    options.pOptions[1].dwOption = INTERNET_PER_CONN_AUTOCONFIG_URL;
    options.pOptions[1].Value.pszValue = remote;

    BOOL result = apply(&options) == RET_NO_ERROR ? TRUE : FALSE;

    free(options.pOptions);
    return result;
}