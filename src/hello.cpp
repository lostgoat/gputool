#include <myproject/config.h>
#include <stdio.h>

#ifdef REMOVE_THIS_BLOCK
// Cpp-check sample from http://cppcheck.sourceforge.net/demo/
void f1 (struct fred_t *p)
{
    // dereference p and then check if it's NULL
    int x = p->x;
    if (p)
        do_something (x);
}
#endif

int main ()
{
    printf ("Hello world %d.%d\n", MyProject_VERSION_MAJOR, MyProject_VERSION_MINOR);
    return 0;
}
