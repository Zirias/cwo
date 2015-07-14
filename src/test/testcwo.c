#include <cwo/cwo.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    const cwo_Type *t1;
    cwo_String *s1, *s2, *s3;

    cwo_Init();

    cwo_String_create(&s1, "Test");
    CALL(s1, "toString", &s2);
    cwo_Object_typeOf(s1, &t1);
    CALL(t1, "toString", &s3);

    puts(cwo_String_cstr(s1));
    puts(cwo_String_cstr(s2));
    puts(cwo_String_cstr(s3));

    cwo_String_destroy(s3);
    cwo_String_destroy(s2);
    cwo_String_destroy(s1);

    cwo_Done();
}
