#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "scan.h"

#include "parse.h"

int main(int argc, char **argv) {
    scanner *scan = new_scanner(argv[1]);
    parser *P = new_parser();

    if(!scan) {
        printf("FATAL: file nonexistent %s\n", argv[1]);
        return -1;
    }
    set_scanner(P, scan);
    parse_glyphs(P);
    print_glyphs(P);
    return 0;
}
