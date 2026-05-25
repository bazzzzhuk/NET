#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<iostream>
#include<cstdio>
#include"resource.h"
#include<CommCtrl.h>


BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, (DLGPROC)DlgProc, NULL);
	return 0;
}

void PrintInfo(HWND hwnd);


BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hIPaddress = GetDlgItem(hwnd, IDC_IP_ADDRESS);
	HWND HIPmask = GetDlgItem(hwnd, IDC_IP_MASK);
	HWND hIPPrefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);
	DWORD dwIPaddress = 0;
	DWORD dwIPmask = 0;
	DWORD dwIPPrefix = 0;
	CHAR szIPPrefix[3] = {};
	switch (uMsg)
	{
	case WM_INITDIALOG:
		//InitCommonControls();
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		std::cout << "Init" << std::endl;
		SetFocus(GetDlgItem(hwnd, IDC_IP_ADDRESS));
		SendMessage(GetDlgItem(hwnd, IDC_SPIN_PREFIX), UDM_SETRANGE, 0, MAKEWORD(30, 0));
		break;
	case WM_NOTIFY:
	{
		NMHDR* pNMHDR = (NMHDR*)lParam;
		//if (pNMHDR->idFrom == IDC_IP_MASK && pNMHDR->code == IPN_FIELDCHANGED)
		{

			/*SendMessage(HIPmask, IPM_GETADDRESS, 0, (LPARAM)&dwIPmask);
			for (dwIPPrefix = 0; dwIPmask; dwIPPrefix++)dwIPmask <<= 1;
			CHAR szIPPrefix[3] = {};
			sprintf(szIPPrefix, "%i", dwIPPrefix);
			std::cout << szIPPrefix << std::endl;
			SendMessage(hIPPrefix, WM_SETTEXT, 0, (LPARAM)szIPPrefix);*/
			//SendMessage(HIPmask, IPM_GETADDRESS, 0, (LPARAM)&dwIPmask);
			////dwIPmask &= 0xFFFFFFFC;
			//for (dwIPPrefix = 0; dwIPmask; dwIPPrefix++)dwIPmask <<= 1;
			//CHAR szIPPrefix[3] = {};
			//sprintf(szIPPrefix, "%i", dwIPPrefix);
			//std::cout << szIPPrefix << std::endl;
			//SendMessage(hIPPrefix, WM_SETTEXT, 0, (LPARAM)szIPPrefix);

			if (pNMHDR->idFrom == IDC_IP_MASK)
			{
				DWORD dwIPmask = 0;
				DWORD dwIPPrefix = 0;
				SendMessage(HIPmask, IPM_GETADDRESS, 0, (LPARAM)&dwIPmask);
				for (dwIPPrefix = 0; dwIPmask; dwIPPrefix++)dwIPmask <<= 1;
				CHAR szIPPrefix[3] = {};
				sprintf(szIPPrefix, "%i", dwIPPrefix);
				std::cout << szIPPrefix << std::endl;
				SendMessage(hIPPrefix, WM_SETTEXT, 0, (LPARAM)szIPPrefix);
			}

		}
	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_IP_ADDRESS:
		{
			if (HIWORD(wParam) == EN_CHANGE)
			{
				SendMessage(hIPaddress, IPM_GETADDRESS, 0, (LPARAM)&dwIPaddress);
				std::cout << FIRST_IPADDRESS(dwIPaddress) << std::endl;
				if (FIRST_IPADDRESS(dwIPaddress) < 128)dwIPmask = 0xFF000000, dwIPPrefix = 8;
				else if (FIRST_IPADDRESS(dwIPaddress) < 192)dwIPmask = 0xFFFF0000, dwIPPrefix = 16;
				else if (FIRST_IPADDRESS(dwIPaddress) < 224)dwIPmask = 0xFFFFFF00, dwIPPrefix = 24;
				std::cout << dwIPmask << std::endl;
				SendMessage(HIPmask, IPM_SETADDRESS, 0, dwIPmask);
				sprintf(szIPPrefix, "%i", dwIPPrefix);
				SendMessage(hIPPrefix, WM_SETTEXT, 0, (LPARAM)szIPPrefix);
			}
		}
		//break;
		case IDC_IP_MASK:
		{
			if (HIWORD(wParam) == EN_CHANGE)
			{
				/*SendMessage(HIPmask, IPM_GETADDRESS, 0, (LPARAM)&dwIPmask);
				dwIPmask &= 0xFFFFFFFC;
				for (dwIPPrefix = 0; dwIPmask; dwIPPrefix++)dwIPmask <<= 1;
				CHAR szIPPrefix[3] = {};
				sprintf(szIPPrefix, "%i", dwIPPrefix);
				std::cout << szIPPrefix << std::endl;
				SendMessage(hIPPrefix, WM_SETTEXT, 0, (LPARAM)szIPPrefix);*/
			}
		}
		break;
		case IDC_EDIT_PREFIX:
		{
			DWORD dwIPaddress = 0;
			DWORD dwIPmask = 0;
			DWORD dwIPPrefix = 0;
			if (HIWORD(wParam) == EN_CHANGE)
			{
				CHAR szIPPrefix[3] = {};
				SendMessage(hIPPrefix, WM_GETTEXT, 3, (LPARAM)szIPPrefix);
				DWORD dwIPprefix = std::atoi(szIPPrefix);
				DWORD dwIPmask = UINT_MAX;
				for (int i = 0; i < 32 - dwIPprefix; i++)dwIPmask <<= 1;
				SendMessage(HIPmask, IPM_SETADDRESS, 0, dwIPmask);
			}
		}
		break;
		case IDC_BUTTON_RESET:
			SendMessage(hIPaddress, IPM_CLEARADDRESS, 0, 0);
			SendMessage(HIPmask, IPM_CLEARADDRESS, 0, 0);
			SendMessage(hIPPrefix, WM_SETTEXT, 0, (LPARAM)"");
			SetFocus(GetDlgItem(hwnd, IDC_IP_ADDRESS));
			break;
		case IDOK:
			PrintInfo(hwnd);
			break;
		case IDCANCEL:EndDialog(hwnd, 0);
		}
	}
	break;
	case WM_CLOSE: EndDialog(hwnd, 0);
	}
	return FALSE;
}
LPSTR FormatAddress(CHAR szBuffer[], CONST CHAR szMessage[], DWORD dwIPaddress)
{
	sprintf
	(
		szBuffer, 
		"%s%i.%i.%i.%i",
		szMessage,
		FIRST_IPADDRESS(dwIPaddress),
		SECOND_IPADDRESS(dwIPaddress),
		THIRD_IPADDRESS(dwIPaddress),
		FOURTH_IPADDRESS(dwIPaddress)
	);
	return szBuffer;
}
LPSTR FormatCount(CHAR szBuffer[], CONST CHAR szMessage[], DWORD dwCount)
{
	sprintf(szBuffer, "%s%i", szMessage, dwCount);
	return szBuffer;
}
void PrintInfo(HWND hwnd)
{
	HWND hIPaddress = GetDlgItem(hwnd, IDC_IP_ADDRESS);
	HWND hIPmask = GetDlgItem(hwnd, IDC_IP_MASK);
	HWND hStaticInfo = GetDlgItem(hwnd, IDC_STATIC_INFO);
	DWORD dwIpaddress = 0;
	DWORD dwIPmask = 0;
	CHAR szInfo[1024] = {};
	CHAR szNETaddressBuffer[1024] = {};
	CHAR szBroadCastBuffer[1024] = {};
	CHAR szIPcontBuffer[1024] = {};
	CHAR szHostCountBuffer[1024] = {};
	SendMessage(hIPaddress, IPM_GETADDRESS, 0, (LPARAM)&dwIpaddress);
	SendMessage(hIPmask, IPM_GETADDRESS, 0, (LPARAM)&dwIPmask);
	DWORD dwNetworAddress = dwIpaddress & dwIPmask;
	DWORD dwBroadCastaddress = dwIpaddress | ~dwIPmask;
	sprintf
	(
		szInfo,
		"%s\n%s\n%s\n%s",
		FormatAddress(szNETaddressBuffer, "Адрес сети:\t\t\t\t", dwNetworAddress),
		FormatAddress(szBroadCastBuffer, "Широковещательный адрес:\t\t", dwBroadCastaddress),
		FormatCount(szIPcontBuffer, "Количество IP - адресов:\t\t", dwBroadCastaddress - dwNetworAddress + 1),
		FormatCount(szHostCountBuffer, "Количество улов:\t\t\t", dwBroadCastaddress - dwNetworAddress - 1)
	);
	SendMessage(hStaticInfo, WM_SETTEXT, 0, (LPARAM)szInfo);
}