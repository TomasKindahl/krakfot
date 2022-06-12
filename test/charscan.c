#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "scan.h"

#include "parse.h"

int main(int argc, char **argv) {
    scanner *scan = new_scanner(argv[1]);
    parser *P = new_parser(65536);
    float scale_x = 1, scale_y = 1;
    
    printf("charscan 1\n");

    if(argc >= 3) scale_x = atof(argv[2]);
    if(argc >= 4) scale_y = atof(argv[3]);
        
    if(!scan) {
        printf("FATAL: file nonexistent %s\n", argv[1]);
        return -1;
    }
    set_scanner(P, scan);
    printf("charscan 2\n");
    parse_glyphs(P);
    printf("charscan 3\n");
    printf("scaling: %g %g\n", scale_x, scale_y);
    print_glyphs(P, scale_x, scale_y);
    printf("charscan 4\n");
    return 0;
}
