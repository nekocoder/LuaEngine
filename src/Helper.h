#include <string>
#include <Windows.h>

/**
 * Convert a LPWSTR to a std::string
 *
 * @param wstr	the string to convert
 *
 * @return strTo	the converted string
 **/
inline std::string wstrtostr(const std::wstring &wstr)
{
    std::string strTo;
    char *szTo = new char[wstr.length() + 1];
    szTo[wstr.size()] = '\0';
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)wstr.length(), NULL, NULL);
    strTo = szTo;
    delete[] szTo;
    return strTo;
}
