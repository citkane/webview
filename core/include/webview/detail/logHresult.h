#include <iostream>
#include <windows.h> // for FormatMessageA and other Windows API functions

void LogHRESULT(const char *funcName, HRESULT hr) {
  if (FAILED(hr)) {
    LPSTR messageBuffer = nullptr;
    DWORD bufferLength = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&messageBuffer, 0, nullptr);
    if (bufferLength > 0) {
      std::cerr << "Function: " << funcName
                << ", Failed with error: " << messageBuffer << '\n';
    } else {
      std::cerr << "Function: " << funcName << ", Failed with HRESULT error 0x"
                << std::hex << std::uppercase << hr << '\n';
    }
    LocalFree(messageBuffer); // Free the buffer allocated by FormatMessageA
  } else {
    std::cout << "Function executed successfully\n";
  }
}