#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    if (argc < 1)
        errx(EXIT_FAILURE, "Usage: %s COMMAND [ARGUMENTS]...", argv[0]);


    if(execvp("./solver",argv) == -1)
    {
        err(EXIT_FAILURE,"execvp()");
    }
    
    return EXIT_SUCCESS;
}
