#include <windows.h>
#include <iostream>

int main() {
    HKEY hKey;
    const char* subKey = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";

    if (RegOpenKeyExA(HKEY_CURRENT_USER, subKey, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        std::cout << "Eroare la deschiderea cheii!\n";
        return 1;
    }

    char valueName[256];
    BYTE data[1024];
    DWORD valueNameSize, dataSize, type;
    DWORD index = 0;

    while (true) {
        valueNameSize = sizeof(valueName);
        dataSize = sizeof(data);

        LONG result = RegEnumValueA(hKey, index, valueName, &valueNameSize,
            NULL, &type, data, &dataSize);

        if (result == ERROR_NO_MORE_ITEMS)
            break;

        if (result == ERROR_SUCCESS) {
            std::cout << "Nume: " << valueName << "\n";

            if (type == REG_SZ)
                std::cout << "Valoare: " << (char*)data << "\n";

            std::cout << "-------------------\n";
        }

        index++;
    }

    RegCloseKey(hKey);
    return 0;
}