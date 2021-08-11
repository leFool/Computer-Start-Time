#include "registry.h"

int installtoregistry() {
	HKEY hk;
	int status = 1;
	char *address = "\"C:\\Cst\\cst.exe\" -r";
	if (RegOpenKeyExA(HKEY_CURRENT_USER,
			"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0,
			KEY_ALL_ACCESS, &hk) != ERROR_SUCCESS) {
		return 0;
	}
	if (RegSetValueExA(hk, "Cst", 0, REG_SZ, (BYTE *) address,
			strlen(address)) != ERROR_SUCCESS) {
		status = 0;
	}
	RegCloseKey(hk);
	return status;
}

int uninstallfromregistry() {
	HKEY hk;
	int status = 1;
	if (RegOpenKeyExA(HKEY_CURRENT_USER,
			"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0,
			KEY_ALL_ACCESS, &hk) != ERROR_SUCCESS) {
		return 0;
	}
	if (RegDeleteValueA(hk, "Cst") != ERROR_SUCCESS) {
		status = 0;
	}
	RegCloseKey(hk);
	return status;
}
