#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "scan.h"

#include "parse.h"

int main(int argc, char **argv) {
    scanner *scan = new_scanner(argv[1]);
    parser *P = new_parser(65536);
    printf("charscan 1\n");

    if(!scan) {
        printf("FATAL: file nonexistent %s\n", argv[1]);
        return -1;
    }
    set_scanner(P, scan);
    printf("charscan 2\n");
    parse_glyphs(P);
    printf("charscan 3\n");
    print_glyphs(P);
    printf("charscan 4\n");
    return 0;
}
