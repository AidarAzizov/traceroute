#include "../includes/ft_traceroute.h"

BOOL        printAndExit(const char* message)
{
    printf("traceroute: %s\n", message);
    return FALSE;
}

BOOL        printAndExitWStr(const char* message, const char *second_message)
{
    printf("traceroute: %s %s\n", message, second_message);
    return FALSE;
}

BOOL         printAndExitWInt(const char* message, int code)
{
    printf("traceroute: %s %d", message, code);
    return code;
}

uint16_t    GetCSum(const uint8_t* Buf, uint32_t Len, uint16_t StartVal)
{
    uint32_t        CSum;
    const uint16_t* Ptr;

    CSum = StartVal;
    if (Len & 0x1)
        CSum += Buf[Len - 1];
    Ptr = (const uint16_t*)Buf;
    Len >>= 1;
    for (uint32_t i = 0; i < Len; i++)
        CSum += Ptr[i];
    while (CSum >> 16)
        CSum = (CSum & 0xFFFF) + (CSum >> 16);
    return (uint16_t)CSum;
}

BOOL        initTime(struct timeval* timeout)
{
    if (gettimeofday(timeout, NULL) != SUCCESS_CODE)
        return printAndExit("Failed to initialize time");
    return TRUE;
}

int         count_not_of(const char* str, char symb)
{
    int count;
    int i;

    count = 0;
    i = 0;
    while (str[i] != '\0')
    {
        if (str[i++] != symb)
            count++;
    }
    return count;
}
