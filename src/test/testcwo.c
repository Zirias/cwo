#include <cwo/cwo.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    cwo_String *s1, *s2;

    cwo_Init();

    cwo_String_create(&s1, "Test");
    CALL(s1, "toString", &s2);

    puts(cwo_String_cstr(s1));
    puts(cwo_String_cstr(s2));

    cwo_String_destroy(s2);
    cwo_String_destroy(s1);

    cwo_Done();
}
