#define STDLIB_API_EXPORT
#include "stdlib_dll.h"


STDLIB_API int32 __StrLen(const char* str)
{
    const char* eos = str;
    while (*eos++)
        ;
    return (eos - str - 1);
}


STDLIB_API _bool __StrCpy(char* src, char* dest)
{
    while (*src != '\0')
        *dest++ = *src++;
    *dest++ = '\0';
    return _true;
}


_bool __StrNCpy(char* src, char* dest, int n)
{
    for (int i = 0; i < n; i++)
        dest[i] = src[i];

    return _true;
}


_bool __StrNCpyn(char* src, char* dest, int s, int n)
{
    int i = 0;

    for (i = 0; i < n; i++)
        dest[i] = src[i + s];

    dest[i] = 0;

    return _true;
}


_bool __StrCmp(const char* a, const char* b)
{
    //	int i = 0;
    int l1 = __StrLen(a);
    int l2 = __StrLen(b);

    if (l1 != l2)
        return _false;

    while (*a != 0)
        if (*a++ != *b++)
            return _false;

    return _true;
}


_bool __StrNCmp(const char* a, const char* b, int n)
{
    for (int i = 0; i < n; i++)
        if (a[i] != b[i])
            return _false;

    return _true;
}


_bool __StrNCmpn(const char* a, const char* b, int s, int n)
{
    for (int i = s; i < s + n; i++)
        if (a[i] != b[i])
            return _false;

    return _true;
}


_bool __StrCat(char* a, char* b)
{
    int i = 0;
    int l1 = 0;
    int l2 = 0;

    l1 = __StrLen(a);
    l2 = __StrLen(b);

    for (i = 0; i < l2; i++)
        a[l1 + i] = b[i];

    a[l1 + i] = 0;
    return _true;
}


_bool __StrNCat(char* a, char* b, int n)
{
    int l = __StrLen(a);
    for (int i = 0; i < n; i++)
        a[l + i] = b[i];

    return _true;
}

char* __StrChr(char* str, char find)
{
    int i = 0;

    while (str[i] != '/0')
    {
        if (str[i] == find)
            return &str[i];
        i++;
    }
    return 0;
}


char* __StrRevChr(char* str, char find)
{
    int i = __StrLen(str) - 1;

    while (str[i] != '/0')
    {
        if (str[i] == find)
            return &str[i];
        i--;
    }
    return 0;
}


char* __StrStr(char* src, char* substr)
{
    for (int i = 0; i < __StrLen(src); i++)
    {
        int n = 0;
        while (src[i] == substr[n])
        {
            n++;
            i++;
            if (n == __StrLen(substr))
                return &src[i];
        }
    }
    return 0;
}


_bool __StrIns(char* src, char* ins, int s)
{
    int  n = __StrLen(ins);
    int  l1 = __StrLen(src);
    char temp[10] = {0};
    for (int i = s; i <= l1; i++)
    {
        temp[i - s] = src[i - 1];
    }
    for (int i = 0; i < n; i++)
    {
        src[s + i] = ins[i];
    }

    for (int j = 0; j < l1 - s - n; j++)
    {
        src[j + s + n] = temp[j];
    }
    return _true;
}


_bool __StrDel(char* src, char* del)
{
    for (int i = 0; i < __StrLen(src); i++)
    {
        int n = 0;
        while (src[i] == del[n])
        {
            n++;
            i++;
            if (n == __StrLen(del))
            {
                for (int j = 0; j < __StrLen(src); j++)
                {
                    src[i + j - n] = src[i + j];
                }
                break;
            }
        }
    }
    return _true;
}


_bool __StrDeln(char* src, int s, int n)
{
    for (int j = 0; j < __StrLen(src); j++)
    {
        src[s + j] = src[s + j + n];
    }
    return _true;
}