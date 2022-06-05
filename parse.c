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
	res->parr = (point *)malloc(sizeof(point)*res->size);
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

void point_flexarr_print(point_flexarr *PTFA) {
	int ix;
	char c;
	printf("(");
	for(ix = 0; ix < PTFA->ins; ix++) {
		if(ix) printf(" ");
		c = (PTFA->parr[ix].bez_ctrl == 0)?':':',';
		printf("%G%c%G", PTFA->parr[ix].x, c, PTFA->parr[ix].y);
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

void path_flexarr_print(path_flexarr *PFA) {
	int ix;
	for(ix = 0; ix < PFA->ins; ix++) {
		printf("  ");
		point_flexarr_print(PFA->path[ix]);
		printf("\n");
	}
}

/*********************/
/**** glyph class ****/
/*********************/

glyph *new_glyph(glyph *prev) {
	glyph *res = (glyph *)malloc(sizeof(glyph));
	res->prev = prev;
	return res;
}

/**********************/
/**** parser class ****/
/**********************/

parser *new_parser(scanner *scan) {
	parser *res = (parser *)malloc(sizeof(parser));
	res->scan = scan;
	res->cglyph = 0;
	return res;
}

int print_glyphs_help(glyph *G) {
	if(!G) return 1;
	printf("@%X = {\n", G->char_code);
	path_flexarr_print(G->contents);
	printf("  >%g:%g\n", G->adjust.x, G->adjust.y);
	printf("}\n");
	print_glyphs_help(G->prev);
	return 1;
}

int print_glyphs(parser *P) {
	return print_glyphs_help(P->cglyph);
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
	while(scan(P->scan) != SCAN_EOF) {
		int type = get_type(P->scan);
		glyph *G = P->cglyph;
		switch(status) {
		    case PST_INIT:
		    	/* printf("(init)\n"); */
		    	if(type == SCAN_GCODE) {
		    		status = PST_CHEX;
		    		P->cglyph = new_glyph(P->cglyph);
		    		G = P->cglyph;
		    		G->char_code = get_ival(get_scanner(P));
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
		    	if(type == SCAN_RBRACE) status = PST_INIT;
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

