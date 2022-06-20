#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "scan.h"

#include "parse.h"

int main(int argc, char **argv) {
    scanner *scan = new_scanner(argv[1]);
    parser *P = new_parser(65536);
    float scale_x = 1, scale_y = 1, off_x = 0, off_y = 0;

    if(argc >= 3) scale_x = atof(argv[2]);
    if(argc >= 4) scale_y = atof(argv[3]);
    if(argc >= 5) off_x = atof(argv[4]);
    if(argc >= 6) off_y = atof(argv[5]);
        
    if(!scan) {
        printf("FATAL: file nonexistent %s\n", argv[1]);
        return -1;
    }
    set_scanner(P, scan);
    parse_glyphs(P);
    printf("scaling: %g+%g %g+%g\n", scale_x, off_x, scale_y, off_y);
    print_glyphs(P, scale_x, off_x, scale_y, off_y);
    return 0;
}
