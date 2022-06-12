#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "scan.h"

#include "parse.h"

/***********************/
/**** point_flexarr ****/
/***********************/

point_flexarr *new_point_flexarr(int max_size) {
	point_flexarr *res = (point_flexarr *)malloc(sizeof(point_flexarr));
	res->size = max_size;
	res->parr = (bpoint *)malloc(sizeof(bpoint)*res->size);
	res->ins = 0;
}

int point_flexarr_add_point(point_flexarr *PTFA, int is_comma, float x, float y) {
	if(PTFA->ins >= PTFA->size) {
		printf("FATAL: point_flexarr_add_point overflow\n");
		exit(-3);
	}
	PTFA->parr[PTFA->ins].bez_ctrl = is_comma;
	PTFA->parr[PTFA->ins].x = x;
	PTFA->parr[PTFA->ins].y = y;
	PTFA->ins++;
	return 1;
}

void point_flexarr_print(point_flexarr *PTFA, float scale_x, float scale_y) {
	int ix;
	char c;
	printf("(");
	for(ix = 0; ix < PTFA->ins; ix++) {
		if(ix) printf(" ");
		c = (PTFA->parr[ix].bez_ctrl == 0)?':':',';
		printf("%G%c%G", PTFA->parr[ix].x*scale_x, c, PTFA->parr[ix].y*scale_y);
	}
	printf(")");
}

/**********************/
/**** path_flexarr ****/
/**********************/

path_flexarr *new_path_flexarr(int max_size) {
	path_flexarr *res = (path_flexarr *)malloc(sizeof(path_flexarr));
	res->size = max_size;
	res->path = (point_flexarr **)malloc(sizeof(point_flexarr *)*res->size);
	res->ins = 0;
}

int path_flexarr_add_path(path_flexarr *PFA) {
	if(PFA->ins >= PFA->size) {
		printf("FATAL: path_flexarr_add_path overflow\n");
		exit(-4);
	}
	PFA->path[PFA->ins] = new_point_flexarr(100);
	PFA->ins++;
	return 1;
}

int path_flexarr_add_point(path_flexarr *PFA, int is_comma, float x, float y) {
	int last_ix = PFA->ins-1;
	point_flexarr_add_point(PFA->path[last_ix], is_comma, x, y);
	return 1;
}

void path_flexarr_print(path_flexarr *PFA, float scale_x, float scale_y) {
	int ix;
	for(ix = 0; ix < PFA->ins; ix++) {
		printf("  ");
		point_flexarr_print(PFA->path[ix], scale_x, scale_y);
		printf("\n");
	}
}

/*********************/
/**** glyph class ****/
/*********************/

glyph *new_glyph() {
	glyph *res = (glyph *)malloc(sizeof(glyph));
	return res;
}

int get_num_lines(glyph *G) {
	return G->contents->ins;
}

int get_num_points_on_line(glyph *G, int path) {
	return G->contents->path[path]->ins;
}

float get_x(glyph *G, int path, int pnt) {
	return G->contents->path[path]->parr[pnt].x;
}

float get_y(glyph *G, int path, int pnt) {
	return G->contents->path[path]->parr[pnt].y;
}

int is_bez_ctrl(glyph *G, int path, int pnt) {
	return G->contents->path[path]->parr[pnt].bez_ctrl;
}

float get_adv_x(glyph *G) {
	return G->adjust.x;
}

float get_adv_y(glyph *G) {
	return G->adjust.y;
}

/**********************/
/**** parser class ****/
/**********************/

parser *new_parser(int size) {
	int ix;
	parser *res = (parser *)malloc(sizeof(parser));
	res->scan = 0;
	res->cglyph = (glyph **)malloc(sizeof(glyph *)*size);
	res->num_cglyphs = size;
	for(ix = 0; ix < size; ix++) {
		res->cglyph[ix] = 0;
	}
	return res;
}

parser *set_scanner(parser *P, scanner *scan) {
	P->scan = scan;
	return P;
}

int print_glyphs_help(glyph *G, float scale_x, float scale_y) {
	if(!G) return 1;
	printf("@%X = {\n", G->char_code);
	path_flexarr_print(G->contents, scale_x, scale_y);
	printf("  >%g:%g\n", G->adjust.x*scale_x, G->adjust.y*scale_y);
	printf("}\n");
}

int print_glyphs(parser *P, float scale_x, float scale_y) {
	int ix;
	for(ix = 0; ix < P->num_cglyphs; ix++) 
	    print_glyphs_help(P->cglyph[ix], scale_x, scale_y);
	return 1;
}

glyph *getelem(parser *P, int index) {
	if(index < 0 || index >= P->num_cglyphs) {
		fprintf(stderr, "ERROR: glyphlist index out of bounds %i\n", index);
		return 0;
	}
	return P->cglyph[index];
}

scanner *get_scanner(parser *P) {
	return P->scan;
}

#define PST_INIT  1
#define PST_CHEX  2
#define PST_EQ    3
#define PST_CONT  4
#define PST_PATH  5
#define PST_PPT1  6
#define PST_PPT2  7
#define PST_ADJ1  8
#define PST_ADJ2  9
#define PST_APT1  10
#define PST_APT2  11
#define PST_ERR   12

int parse_glyphs(parser *P) {
	int status = PST_INIT;
	float x, y; /* for adding points */
	int is_comma;
    glyph *G = 0;

	while(scan(P->scan) != SCAN_EOF) {
		int type = get_type(P->scan);
		switch(status) {
		    case PST_INIT:
		    	/* printf("(init)\n"); */
		    	if(type == SCAN_GCODE) {
		    		G = new_glyph(P->cglyph);
		    		status = PST_CHEX;
		    		G->char_code = get_ival(get_scanner(P));
		    		P->cglyph[G->char_code] = G;
                }
		    	else status = PST_ERR;
		  	    break;
		    case PST_CHEX:
		    	/* printf("(@X)\n"); */
		    	if(type == SCAN_EQUALS) status = PST_EQ;
		    	else status = PST_ERR;
		  	    break;
		    case PST_EQ:
		    	/* printf("(=)\n"); */
		    	if(type == SCAN_LBRACE) {
		    		status = PST_CONT;
		    		G->contents = new_path_flexarr(100);
		    	}
		    	else status = PST_ERR;
		  	    break;
		    case PST_CONT:
		    	/* printf("(cont.)\n"); */
		    	if(type == SCAN_LPAR) {
		    		status = PST_PATH;
		    		path_flexarr_add_path(G->contents);
		    	}
		    	else if(type == SCAN_GTR) status = PST_ADJ1;
		    	else status = PST_ERR;
		  	    break;
		    case PST_PATH:
		    	/* printf("(path)\n"); */
		    	if(type == SCAN_FLOAT) {
		    		status = PST_PPT1;
		    		x = get_fval(get_scanner(P));
		    	}
		    	else if(type == SCAN_RPAR) status = PST_CONT;
		    	else status = PST_ERR;
		  	    break;
		    case PST_PPT1:
		    	/* printf("(ppt₁)\n"); */
		    	if(type == SCAN_COLON) {
		    		status = PST_PPT2;
		    		is_comma = 0;
		    	}
		    	else if(type == SCAN_COMMA) {
		    		status = PST_PPT2;
		    		is_comma = 1;
		    	}
		    	else status = PST_ERR;
		  	    break;
		    case PST_PPT2:
		    	/* printf("(ppt₂)\n"); */
		    	if(type == SCAN_FLOAT) {
		    		status = PST_PATH;
		    		y = get_fval(get_scanner(P));
		    		path_flexarr_add_point(G->contents, is_comma, x, y);
		    	}
		    	else status = PST_ERR;
		  	    break;
		    case PST_ADJ1:
		    	/* printf("(adj₁)\n"); */
		    	if(type == SCAN_FLOAT) { 
		    		status = PST_APT1;
		    		G->adjust.x = get_fval(get_scanner(P));
		    	}
		    	else status = PST_ERR;
		  	    break;
		    case PST_ADJ2:
		    	/* printf("(adj₂)\n"); */
		    	if(type == SCAN_RBRACE) {
		    		status = PST_INIT;
		    		G = 0;
		    	}
		    	else status = PST_ERR;
		  	    break;
		    case PST_APT1:
		    	/* printf("(apt₁)\n"); */
		    	if(type == SCAN_COLON) status = PST_APT2;
		    	else status = PST_ERR;
		  	    break;
		    case PST_APT2:
		    	/* printf("(apt₂)\n"); */
		    	if(type == SCAN_FLOAT) {
		    		status = PST_ADJ2;
		    		G->adjust.y = get_fval(get_scanner(P));
		    	}
		    	else status = PST_ERR;
		  	    break;
		    case PST_ERR:
		    	printf("ERROR!\n");
		    	return 0;
		  	    break;
		    default:
		  	    break;
		}
		/* printf("  "); */
		/* print_tok(P->scan); */
	}
	printf("SUCCESS!\n");
}
