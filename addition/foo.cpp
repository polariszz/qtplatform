#include "foo.h"

char* WcharToChar(wchar_t* wc)
{
    char *m_char;
    int len= WideCharToMultiByte(CP_ACP,0,wc,wcslen(wc),NULL,0,NULL,NULL);
    m_char=new char[len+1];
    WideCharToMultiByte(CP_ACP,0,wc,wcslen(wc),m_char,len,NULL,NULL);
    m_char[len]='\0';
    return m_char;
}
