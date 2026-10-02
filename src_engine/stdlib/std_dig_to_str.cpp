#define STDLIB_API_EXPORT
#include "stdlib_dll.h"


using namespace krystallic::Stdlib;


_bool int8_to_str(int8 val, char* str)
{
    if (-128 > val || val > 127)
        return False;

    char  result[32] = {0};
    _bool isNegative = False;
    int   index = 0;
    int   i = 0;
    char  digit = ' ';
    int   len = 0;

    // Обрабатываем отрицательные числа
    if (val < 0)
    {
        isNegative = True;
        val = -val; // Приводим к положительному числу для дальнейшей обработки
    }

    // Обрабатываем случай, когда число равно 0
    if (val == 0)
    {
        result[0] = '0';
    }
    else
    {
        // Преобразование цифр числа в символы
        while (val > 0)
        {
            digit = '0' + (val % 10);
            result[index++] = digit; // Добавляем символ в начало строки
            val /= 10;
        }
        if (isNegative)
        {
            result[index] = '-';
        }
    }

    len = StrLen(result);

    //str[0] = '-';
    for (i = 0; i < len; i++)
    {
        str[i] = result[len - 1 - i];
    }
    str[len] = '\0';
    return True;
}


_bool int16_to_str(int16 val, char* str)
{
    if (-32768 > val || val > 32767)
        return False;

    char  result[32] = {0};
    _bool isNegative = False;
    int   index = 0;
    int   i = 0;
    char  digit = ' ';
    int   len = 0;

    // Обрабатываем отрицательные числа
    if (val < 0)
    {
        isNegative = True;
        val = -val; // Приводим к положительному числу для дальнейшей обработки
    }

    // Обрабатываем случай, когда число равно 0
    if (val == 0)
    {
        result[0] = '0';
    }
    else
    {
        // Преобразование цифр числа в символы
        while (val > 0)
        {
            digit = '0' + (val % 10);
            result[index++] = digit; // Добавляем символ в начало строки
            val /= 10;
        }
        if (isNegative)
        {
            result[index] = '-';
        }
    }

    len = StrLen(result);

    //str[0] = '-';
    for (i = 0; i < len; i++)
    {
        str[i] = result[len - 1 - i];
    }
    str[len] = '\0';
    return True;
}


_bool int32_to_str(int32 val, char* str)
{
    char  result[32] = {0};
    _bool isNegative = False;
    int   index = 0;
    int   i = 0;
    char  digit = ' ';
    int   len = 0;

    // Обрабатываем отрицательные числа
    if (val < 0)
    {
        isNegative = True;
        val = -val; // Приводим к положительному числу для дальнейшей обработки
    }

    // Обрабатываем случай, когда число равно 0
    if (val == 0)
    {
        result[0] = '0';
    }
    else
    {
        // Преобразование цифр числа в символы
        while (val > 0)
        {
            digit = '0' + (val % 10);
            result[index++] = digit; // Добавляем символ в начало строки
            val /= 10;
        }
        if (isNegative)
        {
            result[index] = '-';
        }
    }

    len = StrLen(result);

    //str[0] = '-';
    for (i = 0; i < len; i++)
    {
        str[i] = result[len - 1 - i];
    }
    str[len] = '\0';
    return True;
}


_bool int64_to_str(int64 val, char* str)
{
    //	if (-9223372036854775808 > val > 9223372036854775807)
    //	{
    //		return False;
    //	}
    char  result[32] = {0};
    _bool isNegative = False;
    int   index = 0;
    int   i = 0;
    char  digit = ' ';
    int   len = 0;

    // Обрабатываем отрицательные числа
    if (val < 0)
    {
        isNegative = True;
        val = -val; // Приводим к положительному числу для дальнейшей обработки
    }

    // Обрабатываем случай, когда число равно 0
    if (val == 0)
    {
        result[0] = '0';
    }
    else
    {
        // Преобразование цифр числа в символы
        while (val > 0)
        {
            digit = '0' + (val % 10);
            result[index++] = digit; // Добавляем символ в начало строки
            val /= 10;
        }
        if (isNegative)
        {
            result[index] = '-';
        }
    }

    len = StrLen(result);

    //str[0] = '-';
    for (i = 0; i < len; i++)
    {
        str[i] = result[len - 1 - i];
    }
    str[len] = '\0';
    return True;
}


_bool uint8_to_str(uint8 val, char* str)
{
    if (0 > val || val > 255)
    {
        return False;
    }
    char  result[32] = {0};
    _bool isNegative = False;
    int   index = 0;
    int   i = 0;
    char  digit = ' ';
    int   len = 0;

    // Обрабатываем отрицательные числа
    if (val < 0)
    {
        isNegative = True;
        val = -val; // Приводим к положительному числу для дальнейшей обработки
    }

    // Обрабатываем случай, когда число равно 0
    if (val == 0)
    {
        result[0] = '0';
    }
    else
    {
        // Преобразование цифр числа в символы
        while (val > 0)
        {
            digit = '0' + (val % 10);
            result[index++] = digit; // Добавляем символ в начало строки
            val /= 10;
        }
        if (isNegative)
        {
            result[index] = '-';
        }
    }

    len = StrLen(result);

    //str[0] = '-';
    for (i = 0; i < len; i++)
    {
        str[i] = result[len - 1 - i];
    }
    str[len] = '\0';
    return True;
}


_bool uint16_to_str(uint16 val, char* str)
{
    if (0 > val || val > 65535)
    {
        return False;
    }
    char  result[32] = {0};
    _bool isNegative = False;
    int   index = 0;
    int   i = 0;
    char  digit = ' ';
    int   len = 0;

    // Обрабатываем отрицательные числа
    if (val < 0)
    {
        isNegative = True;
        val = -val; // Приводим к положительному числу для дальнейшей обработки
    }

    // Обрабатываем случай, когда число равно 0
    if (val == 0)
    {
        result[0] = '0';
    }
    else
    {
        // Преобразование цифр числа в символы
        while (val > 0)
        {
            digit = '0' + (val % 10);
            result[index++] = digit; // Добавляем символ в начало строки
            val /= 10;
        }
        if (isNegative)
        {
            result[index] = '-';
        }
    }

    len = StrLen(result);

    //str[0] = '-';
    for (i = 0; i < len; i++)
    {
        str[i] = result[len - 1 - i];
    }
    str[len] = '\0';
    return True;
}


_bool uint32_to_str(uint32 val, char* str)
{
    if (0 > val || val > 0xffffffffu)
    {
        return False;
    }
    char  result[32] = {0};
    _bool isNegative = False;
    int   index = 0;
    int   i = 0;
    char  digit = ' ';
    int   len = 0;

    // Обрабатываем отрицательные числа
    //if (val < 0) {
    //	isNegative = True;
    //	val = -val; // Приводим к положительному числу для дальнейшей обработки
    //}

    // Обрабатываем случай, когда число равно 0
    if (val == 0)
    {
        result[0] = '0';
    }
    else
    {
        // Преобразование цифр числа в символы
        while (val > 0)
        {
            digit = '0' + (val % 10);
            result[index++] = digit; // Добавляем символ в начало строки
            val /= 10;
        }
        if (isNegative)
        {
            result[index] = '-';
        }
    }

    len = StrLen(result);

    //str[0] = '-';
    for (i = 0; i < len; i++)
    {
        str[i] = result[len - 1 - i];
    }
    str[len] = '\0';
    return True;
}


_bool uint64_to_str(uint64 val, char* str)
{
    if (0 > val || val > 0xffffffffffffffffull)
    {
        return False;
    }
    char  result[32] = {0};
    _bool isNegative = False;
    int   index = 0;
    int   i = 0;
    char  digit = ' ';
    int   len = 0;

    //// Обрабатываем отрицательные числа
    //if (val < 0)
    //{
    //    isNegative = True;
    //    val = -val; // Приводим к положительному числу для дальнейшей обработки
    //}

    // Обрабатываем случай, когда число равно 0
    if (val == 0)
    {
        result[0] = '0';
    }
    else
    {
        // Преобразование цифр числа в символы
        while (val > 0)
        {
            digit = '0' + (val % 10);
            result[index++] = digit; // Добавляем символ в начало строки
            val /= 10;
        }
        if (isNegative)
        {
            result[index] = '-';
        }
    }

    len = StrLen(result);

    //str[0] = '-';
    for (i = 0; i < len; i++)
        str[i] = result[len - 1 - i];
    
    str[len] = '\0';
    return True;
}

