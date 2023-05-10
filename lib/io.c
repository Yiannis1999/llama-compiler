#include <stdio.h>
#include <stdbool.h>

void print_int_0(int n)
{
    printf("%d", n);
}

void print_bool_1(bool b)
{
    if (b)
    {
        printf("true");
    }
    else
    {
        printf("false");
    }
}

void print_char_2(char c)
{
    printf("%c", c);
}

void print_float_3(float f)
{
    printf("%f", f);
}

void print_string_4(char *s)
{
    printf("%s", s);
}

int read_int_5()
{
    int n;
    scanf("%d", &n);
    return n;
}

bool read_bool_6()
{
    char buffer[5];
    scanf("%s", buffer);
    if (buffer[0] == 't' || buffer[0] == 'T')
    {
        return true;
    }
    else
    {
        return false;
    }
}

char read_char_7()
{
    char c;
    scanf(" %c", &c);
    return c;
}

float read_float_8()
{
    float f;
    scanf("%f", &f);
    return f;
}

void read_string_9(char *s)
{
    scanf("%s", s);
}