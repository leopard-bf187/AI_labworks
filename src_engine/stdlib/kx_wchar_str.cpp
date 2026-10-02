#define STDLIB_API_EXPORT
#include "stdlib_dll.h"


int kx_wstrlen(wchar* str);


_bool kx_wstrcmp(wchar* str1, wchar* str2) // compares two strings ;
{
    int i = 0;
    int size_str1 = kx_wstrlen(str1);
    int size_str2 = kx_wstrlen(str2);

    if (size_str1 != size_str2)
        return _false;

    for (i = 0; i < size_str1; i++)
    {
        if (str1[i] != str2[i])
            return _false;
    }

    return _true;
}


_bool kx_wstrncmp(wchar* str1, wchar* str2, int n) // compareds parts of strings to the specil index "n"
{
    int i = 0;
    int size_str1 = kx_wstrlen(str1);
    int size_str2 = kx_wstrlen(str2);

    if (size_str1 < n || size_str2 < n)
        return _false;

    for (i = 0; i < n; i++)
    {
        if (str1[i] != str2[i])
            return _false;
    }

    return _true;
}


wchar* kx_wstrcat(wchar* str1, wchar* str2) // concatenate two strings;
{
    int i = 0;
    int sz1 = kx_wstrlen(str1);
    int sz2 = kx_wstrlen(str2);

    wchar* c = (wchar*) malloc(sizeof(wchar) * (sz1 + sz2));

    for (i = 0; i < sz1; i++)
        c[i] = str1[i];

    for (i = 0; i < sz2; i++)
        c[i + sz1] = str2[i];

    c[sz1 + sz2] = L'\0';
    return c;
}


wchar* kx_wstrcat_tobegin(wchar* str1,
                          wchar* str2) // insert substring in the begin of other string(concatenate invers);
{
    int    i = 0;
    int    sz1 = kx_wstrlen(str1);
    int    sz2 = kx_wstrlen(str2);
    wchar* c = (wchar*) malloc(sizeof(wchar) * (sz1 + sz2));

    for (i = 0; i < sz2; i++)
        c[i] = str2[i];

    for (i = 0; i < sz1; i++)
        c[i + sz1] = str1[i];

    c[sz1 + sz2] = L'\0';
    return c;
}


wchar* kx_wstrins(int ind, wchar* str1, wchar* str2) // insert substring in other string by index;
{
    int    i = 0, j = 0;
    int    sz1 = 0, sz2 = 0;
    wchar* c = 0;
    sz1 = kx_wstrlen(str1);
    sz2 = kx_wstrlen(str2);

    c = (wchar*) malloc(sizeof(wchar) * (sz1 + sz2));

    for (i = 0; i < ind; i++)
        c[i] = str1[i];

    for (j = 0; j < sz2; j++)
        c[i + j] = str2[j];

    for (i = i + sz2; i < (sz1 + sz2); i++)
        c[i] = str1[i - sz2];

    c[sz1 + sz2] = L'\0';
    return c;
}


wchar* kx_wsubstr(wchar* str1, wchar* str2) // find substring from the string
{
    int i = 0, j = 0, sz1 = 0, sz2 = 0, tmp = 0;
    sz1 = kx_wstrlen(str1);
    sz2 = kx_wstrlen(str2);
    for (i = 0; i < sz1; i++)
    {
        j = 0;
        tmp = i;
        while (str1[tmp] == str2[j])
        {
            tmp++;
            j++;
        }

        if (j == (sz2 - 1))
            return 0;
    }
    return 0;
}


wchar* kx_wstrchr(wchar* str, wchar s) // find char in the string by index
{
    int i = 0;

    while (str[i] != L'\0')
    {
        if (str[i] == s)
            return &str[i];
        else
            i++;
    }

    return 0;
}


wchar* kx_wstrcpy(wchar* from, wchar* to) // copy one string in another empty
{
    wchar* addres = to;

    while (*from != L'\0')
        *to++ = *from++;

    *to++ = L'\0';

    return addres;
}


wchar* kx_wstrncpy(wchar* from, wchar* to, uint n) // copy one string in another empty to the specil index "n"
{
    return 0;
}


int kx_wstrlen(wchar* str) // gets length of string
{
    int i = 0;
    while (str[i] != L'\0')
        i++;
    return i;
}