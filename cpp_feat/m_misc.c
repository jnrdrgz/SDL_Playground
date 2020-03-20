#include <stdio.h>
#include "stringsdef.h"

typedef struct
{
    char*	name;
    int*	location;
    int		defaultvalue;
    int		scantranslate;		// PC scan code hack
    int		untranslated;		// lousy hack
} default_t;

default_t defaults[] =
{
    {"test",0,(int) HUSTR_CHATMACRO0},
    {"test1",0,0},
};


int main(int argc, char const *argv[])
{
	printf("%d\n", (int)defaults[0].name);
	return 0;
}
