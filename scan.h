#ifndef _SCAN_H
#define _SCAN_H

#define SCAN_ERROR  0
#define SCAN_GCODE  1
#define SCAN_EQUALS 2
#define SCAN_LBRACE 3
#define SCAN_RBRACE 4
#define SCAN_LPAR   5
#define SCAN_RPAR   6
#define SCAN_HEXINT 7
#define SCAN_FLOAT  8
#define SCAN_COLON  9
#define SCAN_COMMA  10
#define SCAN_GTR    11
#define SCAN_EOF    12

typedef struct _scantok_S {
    int type;
    union {
    	int ival;
    	float fval;
    } var;
} scantok;

typedef struct _scanner_S {
    FILE *inf;
    scantok *token;
    int unscanned;
} scanner;

/** method new_scanner                                      **/
/**   args: filename - a string of the file to open         **/
/**   does: returns a newly created scanner with a file     **/
/**         opened                                          **/
scanner *new_scanner(char *filename);

/** method scan                                             **/
/**   args: sc - a pointer to the scanner                   **/
/**   returns: the token identifier as an integer           **/
/**   does: scans one token from the instream and put the   **/
/**         data into the scan object                       **/
int scan(scanner *sc);

/* unscan: unscan the current token */
/* NOT USED! DON'T USE! */
int unscan(scanner *sc);

/* print_tok: prints the currently scanned token */
/* ONLY FOR DEBUG! */
void print_tok(scanner *sc);

/** method get_type                                         **/
/**   args: sc - a pointer to the scanner                   **/
/**   returns: the token identifier as an integer           **/
/**   does: returns the token identifier of the currently   **/
/**         scanned object                                  **/
int get_type(scanner *sc);

/** method get_ival                                         **/
/**   args: sc - a pointer to the scanner                   **/
/**   returns: the integer value previously scanned         **/            
/**   does: returns the  integer value previously scanned   **/
/**         if of type SCAN_GCODE or SCAN_HEXINT            **/
int get_ival(scanner *sc);

/** method get_fval                                         **/
/**   args: sc - a pointer to the scanner                   **/
/**   returns: the float value previously scanned           **/            
/**   does: returns the  integer value previously scanned   **/
/**         if of type SCAN_FLOAT                           **/
float get_fval(scanner *sc);

#endif /* _SCAN_H */