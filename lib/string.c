int strlen_27(char *str)
{
    int len = 0;
    while (*str++ != '\0')
    {
        len++;
    }
    return len;
}

int strcmp_28(char *str1, char *str2)
{
    while (*str1 == *str2)
    {
        if (*str1 == '\0')
        {
            return 0;
        }
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

void strcpy_29(char *dest, char *src)
{
    while (*src != '\0')
    {
        *dest++ = *src++;
    }
    *dest = '\0';
}

void strcat_30(char *dest, char *src)
{
    while (*dest != '\0')
    {
        dest++;
    }
    while (*src != '\0')
    {
        *dest++ = *src++;
    }
    *dest = '\0';
}
