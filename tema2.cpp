#include <windows.h>
#include <setupapi.h>
#include <iostream>
#include <string>
#include <algorithm>

#pragma comment(lib, "setupapi.lib")

std::string GetProperty(HDEVINFO h, SP_DEVINFO_DATA& d, DWORD prop) {
    char buffer[1024];
    DWORD size = 0;

    if (SetupDiGetDeviceRegistryPropertyA(
        h, &d, prop, NULL,
        (PBYTE)buffer, sizeof(buffer), &size)) {

        return std::string(buffer);
    }

    return "";
}

std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return s;
}

bool containsCaseInsensitive(const std::string& a, const std::string& b) {
    std::string A = toLower(a);
    std::string B = toLower(b);

    return A.find(B) != std::string::npos;
}

int main() {
    std::string search;
    std::cout << "Cauta device (ex: usb, audio, nvidia): ";
    std::getline(std::cin, search);

    HDEVINFO h = SetupDiGetClassDevs(
        NULL, NULL, NULL,
        DIGCF_PRESENT | DIGCF_ALLCLASSES);

    if (h == INVALID_HANDLE_VALUE) {
        std::cout << "Eroare la obtinerea device-urilor!\n";
        return 1;
    }

    SP_DEVINFO_DATA d;
    d.cbSize = sizeof(d);

    bool found = false;

    for (int i = 0; SetupDiEnumDeviceInfo(h, i, &d); i++) {

        std::string name = GetProperty(h, d, SPDRP_FRIENDLYNAME);
        std::string desc = GetProperty(h, d, SPDRP_DEVICEDESC);

        std::string full = name + " " + desc;

        if (!full.empty() && containsCaseInsensitive(full, search)) {

            found = true;

            std::cout << "\n=====================\n";

            std::cout << "Nume: "
                << (name.empty() ? desc : name) << "\n";

            std::cout << "Descriere: " << desc << "\n";
            std::cout << "Producator: " << GetProperty(h, d, SPDRP_MFG) << "\n";
            std::cout << "Hardware ID: " << GetProperty(h, d, SPDRP_HARDWAREID) << "\n";
            std::cout << "Clasa: " << GetProperty(h, d, SPDRP_CLASS) << "\n";
        }
    }

    if (!found) {
        std::cout << "\nNiciun device gasit pentru cautarea ta.\n";
    }

    SetupDiDestroyDeviceInfoList(h);
    return 0;
}
