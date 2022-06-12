#ifndef _PARSE_H
#define _PARSE_H

/*********************/
/**** point class ****/
/*********************/

typedef struct _bpoint_S {
	int bez_ctrl; /* 0 if a polygon node, 1 if a bezier ctrl point */
	float x, y;
} bpoint;

/***********************/
/**** point_flexarr ****/
/***********************/

typedef struct _point_flexarr_S {
	bpoint *parr;
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
    bpoint adjust;
} glyph;

/** method get_num_lines                                    **/
/**   args: G - the glyph to query                          **/
/**   does: returns the number of paths in the glyph        **/
int get_num_lines(glyph *G);

/** method get_num_points_on_line                           **/
/**   args: G    - the glyph to query                       **/
/**         path - the line number of the path              **/
/**   does: returns the number of paths in the glyph        **/
int get_num_points_on_line(glyph *G, int path);

/** method get_x                                            **/
/**   args: G    - the glyph to query                       **/
/**         path - the line number of the path              **/
/**         pnt  - the point number in the path             **/
/**   does: returns the x coordinate of the point           **/
float get_x(glyph *G, int path, int pnt);

/** method get_y                                            **/
/**   args: G    - the glyph to query                       **/
/**         path - the line number of the path              **/
/**         pnt  - the point number in the path             **/
/**   does: returns the y coordinate of the point           **/
float get_y(glyph *G, int path, int pnt);

/** method is_bez_ctrl                                      **/
/**   args: G    - the glyph to query                       **/
/**         path - the line number of the path              **/
/**         pnt  - the point number in the path             **/
/**   does: returns whether point is a bezier control point **/
int is_bez_ctrl(glyph *G, int path, int pnt);

/** method get_adv_y                                        **/
/**   args: G    - the glyph to query                       **/
/**   does: returns the x advance of the glyph              **/
float get_adv_x(glyph *G);

/** method get_adv_y                                        **/
/**   args: G    - the glyph to query                       **/
/**   does: returns the y advance of the glyph              **/
float get_adv_y(glyph *G);

/**********************/
/**** parser class ****/
/**********************/

typedef struct _parser_S {
	glyph **cglyph;   /* Glyph list     */
	int num_cglyphs;
    scanner *scan;    /* Scanner status */
} parser;

/** method new_parser                                       **/
/**   args: size - the number of glyphs to reserve          **/
/**   does: returns a newly created parser                  **/
parser *new_parser(int size);

/** method set_scanner                                      **/
/**   args: P    - the parser on which to set scanner       **/
/**         scan - pointer to the scanner                   **/
/**   does: attaches a scanner to a parser                  **/
parser *set_scanner(parser *P, scanner *scan);

/** method parse_glyphs                                     **/
/**   args: P - pointer to a parser                         **/
/**   does: parse_glyphs parses the attached scanner stream **/
/**         according to the glyph description language and **/
/**         stores it as a glyph                            **/
int parse_glyphs(parser *P);

/** method print_glyphs                                     **/
/**   args: P       - pointer to a parser                   **/
/**         scale_x - rescale factor in dimension x         **/
/**         scale_y - rescale factor in dimension y         **/
/**   does: prints the glyph list pointed to by the parser  **/
/**         object                                          **/
int print_glyphs(parser *P, float scale_x, float scale_y);

/** method getelem                                          **/
/**   args: P     - pointer to a parser                     **/
/**         index - index of glyph to get                   **/
/**   does: get the glyph with the index number             **/
glyph *getelem(parser *P, int index);

#endif /* _PARSE_H */