#ifndef _PARSE_H
#define _PARSE_H

/*********************/
/**** point class ****/
/*********************/

typedef struct _point_S {
	int bez_ctrl; /* 0 if a polygon node, 1 if a bezier ctrl point */
	float x, y;
} point;

/***********************/
/**** point_flexarr ****/
/***********************/

typedef struct _point_flexarr_S {
	point *parr;
	int size;
	int ins;
} point_flexarr;

/**********************/
/**** path_flexarr ****/
/**********************/

typedef struct _path_flexarr_S {
	point_flexarr **path;
	int size;
	int ins;
} path_flexarr;

/*********************/
/**** glyph class ****/
/*********************/

typedef struct _glyph_S {
    int char_code;
    path_flexarr *contents;
    point adjust;
    struct _glyph_S *prev;
} glyph;

/**********************/
/**** parser class ****/
/**********************/

typedef struct _parser_S {
	glyph *cglyph; /* Glyph list     */
    scanner *scan; /* Scanner status */
} parser;

/** method new_parser                                       **/
/**   args: scan - pointer to a scanner                     **/
/**   does: returns a newly created parser based on a       **/
/**         new scanner                                     **/

parser *new_parser(scanner *scan);

/** method parse_glyphs                                     **/
/**   args: P - pointer to a parser                         **/
/**   does: parse_glyphs parses the attached scanner stream **/
/**         according to the glyph description language and **/
/**         stores it as a glyph                            **/

int parse_glyphs(parser *P);

/** method print_glyphs                                     **/
/**   args: P - pointer to a parser                         **/
/**   does: prints the glyph list pointed to by the parser  **/
/**         object                                          **/

int print_glyphs(parser *P);

#endif /* _PARSE_H */