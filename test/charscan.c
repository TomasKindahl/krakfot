#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "scan.h"

#include "parse.h"

int main(int argc, char **argv) {
    scanner *scan = new_scanner(argv[1]);
    parser *G = new_parser(scan);

    if(!scan) {
        printf("FATAL: file nonexistent %s\n", argv[1]);
        return -1;
    }
    parse_glyphs(G);
    print_glyphs(G);
    return 0;
}
