#define STDLIB_API_EXPORT
#include "stdlib_dll.h"


using namespace krystallic::Stdlib;


_bool str_to_int8(char* str, int8* outer)
{
    int8  ret = 0;
    int   i = 0;
    _bool negative = False;

    uint len = StrLen(str);
    int32 n = 0;
    //str_to_int(str, &n);
    // Должно быть в диапозоне от -128 до 127 (0, 255)
    if (-128 > n || n > 127)
    {
        return False;
    }
    // Обработка отрицательных чисел
    if (str[0] == '-')
    {
        negative = True;
        i = 1; // Начинаем со второго символа, так как первый - знак "-"
    }
    // если отрицательное, длина будет на знак больше из-за знака
    if (negative)
    {
        if (len > 4)
        {
            return False;
        }
    }
    if (len > 3)
    {
        return False;
    }

    // Преобразование символов в числовое значение
    while (str[i] != '\0')
    {
        // Проверяем, что текущий символ является цифрой
        if (str[i] >= '0' && str[i] <= '9')
        {
            // Умножаем текущий результат на 10 и добавляем новую цифру
            ret = ret * 10 + (str[i] - '0');
        }
        else
        {
            // Если встречаем нецифровой символ, прерываем цикл
            break;
        }
        i++;
    }

    // Учитываем знак числа
    if (negative)
    {
        ret = -ret;
    }

    *outer = ret;
    return True;
}


_bool str_to_int16(char* str, int16* outer)
{
    int16 ret = 0;
    int   i = 0;
    _bool negative = False;

    uint len = StrLen(str);
    int32 n = 0;
    //str_to_int(str, &n);

    if (-32768 > n || n > 32767)
    {
        return False;
    }
    // Обработка отрицательных чисел
    if (str[0] == '-')
    {
        negative = True;
        i = 1; // Начинаем со второго символа, так как первый - знак "-"
    }
    if (negative)
    {
        if (len > 6)
        {
            return False;
        }
    }
    if (len > 5)
    {
        return False;
    }
    // Преобразование символов в числовое значение
    while (str[i] != '\0')
    {
        // Проверяем, что текущий символ является цифрой
        if (str[i] >= '0' && str[i] <= '9')
        {
            // Умножаем текущий результат на 10 и добавляем новую цифру
            ret = ret * 10 + (str[i] - '0');
        }
        else
        {
            // Если встречаем нецифровой символ, прерываем цикл
            break;
        }
        i++;
    }

    // Учитываем знак числа
    if (negative)
    {
        ret = -ret;
    }

    *outer = ret;
    return True;
}


_bool str_to_int32(char* str, int32* outer)
{
    int32 ret = 0;
    int   i = 0;
    _bool negative = False;

    uint len = StrLen(str);

    // Обработка отрицательных чисел
    if (str[0] == '-')
    {
        negative = True;
        i = 1; // Начинаем со второго символа, так как первый - знак "-"
    }
    if (negative)
    {
        if (len > 11)
        {
            return False;
        }
    }
    if (len > 10)
    {
        return False;
    }
    // Преобразование символов в числовое значение
    while (str[i] != '\0')
    {
        // Проверяем, что текущий символ является цифрой
        if (str[i] >= '0' && str[i] <= '9')
        {
            // Умножаем текущий результат на 10 и добавляем новую цифру
            ret = ret * 10 + (str[i] - '0');
        }
        else
        {
            // Если встречаем нецифровой символ, прерываем цикл
            break;
        }
        i++;
    }

    // Учитываем знак числа
    if (negative)
    {
        ret = -ret;
    }

    *outer = ret;
    return True;
}


_bool str_to_int64(char* str, int64* outer)
{
    int64 ret = 0;
    int   i = 0;
    _bool negative = False;

    uint len = StrLen(str);
    //	int32 n = 0;
    //str_to_int(str, &n);

    //	if (-9223372036854775808 > n > 9223372036854775807)
    //	{
    //		return False;
    //	}
    // Обработка отрицательных чисел
    if (str[0] == '-')
    {
        negative = True;
        i = 1; // Начинаем со второго символа, так как первый - знак "-"
    }
    if (negative)
    {
        if (len > 20)
        {
            return False;
        }
    }
    if (len > 19)
    {
        return False;
    }
    // Преобразование символов в числовое значение
    while (str[i] != '\0')
    {
        // Проверяем, что текущий символ является цифрой
        if (str[i] >= '0' && str[i] <= '9')
        {
            // Умножаем текущий результат на 10 и добавляем новую цифру
            ret = ret * 10 + (str[i] - '0');
        }
        else
        {
            // Если встречаем нецифровой символ, прерываем цикл
            break;
        }
        i++;
    }

    // Учитываем знак числа
    if (negative)
    {
        ret = -ret;
    }

    *outer = ret;
    return True;
}


_bool str_to_uint8(char* str, uint8* outer)
{
    uint8 ret = 0;
    int   i = 0;

    uint len = StrLen(str);
    int32 n = 0;
    //str_to_int(str, &n);

    if (0 > n || n > 255)
    {
        return False;
    }
    // Обработка отрицательных чисел

    if (len > 3)
    {
        return False;
    }

    // Преобразование символов в числовое значение
    while (str[i] != '\0')
    {
        // Проверяем, что текущий символ является цифрой
        if (str[i] >= '0' && str[i] <= '9')
        {
            // Умножаем текущий результат на 10 и добавляем новую цифру
            ret = ret * 10 + (str[i] - '0');
        }
        else
        {
            // Если встречаем нецифровой символ, прерываем цикл
            break;
        }
        i++;
    }

    *outer = ret;
    return True;
}


_bool str_to_uint16(char* str, uint16* outer)
{
    uint16 ret = 0;
    int    i = 0;

    uint len = StrLen(str);
    int32 n = 0;
    //str_to_int(str, &n);

    if (0 > n || n > 65536)
    {
        return False;
    }

    if (len > 5)
    {
        // если длина больше 4 (3 цифры и знак \0), завершаем программу
        return False;
    }
    // Преобразование символов в числовое значение
    while (str[i] != '\0')
    {
        // Проверяем, что текущий символ является цифрой
        if (str[i] >= '0' && str[i] <= '9')
        {
            // Умножаем текущий результат на 10 и добавляем новую цифру
            ret = ret * 10 + (str[i] - '0');
        }
        else
        {
            // Если встречаем нецифровой символ, прерываем цикл
            break;
        }
        i++;
    }

    *outer = ret;
    return True;
}


_bool str_to_uint32(char* str, uint32* outer)
{
    uint32 ret = 0;
    int    i = 0;

    uint len = StrLen(str);
    int32 n = 0;
    //str_to_int(str, &n);
    if (0 > n || n > 0xffffffffu)
    {
        return False;
    }

    if (len > 10)
    {
        // если длина больше 4 (3 цифры и знак \0), завершаем программу
        return False;
    }
    // Преобразование символов в числовое значение
    while (str[i] != '\0')
    {
        // Проверяем, что текущий символ является цифрой
        if (str[i] >= '0' && str[i] <= '9')
        {
            // Умножаем текущий результат на 10 и добавляем новую цифру
            ret = ret * 10 + (str[i] - '0');
        }
        else
        {
            // Если встречаем нецифровой символ, прерываем цикл
            break;
        }
        i++;
    }

    *outer = ret;
    return True;
}


_bool str_to_uint64(char* str, uint64* outer)
{
    uint64 ret = 0;
    int    i = 0;

    uint len = StrLen(str);
    int32 n = 0;
    //str_to_int(str, &n);
    if (0 > n || n > 0xffffffffffffffffull)
    {
        return False;
    }

    if (len > 20)
    {
        return False;
    }
    // Преобразование символов в числовое значение
    while (str[i] != '\0')
    {
        // Проверяем, что текущий символ является цифрой
        if (str[i] >= '0' && str[i] <= '9')
        {
            // Умножаем текущий результат на 10 и добавляем новую цифру
            ret = ret * 10 + (str[i] - '0');
        }
        else
        {
            // Если встречаем нецифровой символ, прерываем цикл
            break;
        }
        i++;
    }

    *outer = ret;
    return True;
}



float str_to_float(char* str)
{
    return 0;
}


double str_to_double(char* str)
{
    return 0;
}