#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "scan.h"

/* https://developer.mozilla.org/en-US/docs/Web/SVG/Tutorial/Paths  */
/* https://personal.math.ubc.ca/~cass/graphics/text/www/pdf/ch6.pdf */

/*  here   svg path    PostScript  */
/*---------------------------------*/
/*  MV     M           moveto      */
/*  LT     L           lineto      */
/*  C1     (C,)        (curveto)   */
/*  C2     (C,)        (curveto)   */
/*  CT     C           curveto     */
/*  STK    (implicit)  stroke      */
/*  ADJ    (implicit)  translate   */
/*---------------------------------*/

char internal_glyph_data [] =
"@20 = {"
"  >6:0"
"}"
"@7C = {"
"  (0:0 0:10 5:10 5:0 0:0)"
"  >6:0"
"}"
"@41 = {"
"  (-0.8:0 1.2:0)"
"  (5.2:0 7.2:0)"
"  (0.2:0 3.2:10.2 6.2:0)"
"  (1.2:3.5 5.2:3.5)"
"  >8:0"
"}"
"@42 = {"
"  (0.8:0 0.8:10)"
"  (-0.2:10 2.3:10 6.3,10 6.3,5 2.3:5 0.8:5)"
"  (0.8:5 2.8:5 6.8,5 6.8,0 2.8:0 -0.2:0)"
"  >8:0"
"}"
"@43 = {"
"  (6:10.2 6:8.2 6,9.2 5,10.2 3.85:10.2 3.3:10.2"
"   -1,10.2 -0.85,-0.2 3.3:-0.2 3.85:-0.2 5,-0.2"
"   6,0.8 6:2 6:2)"
"  >8:0"
"}"
"@44 = {"
"  (-0.3:0 1.9:0 7.4,0 7.4,10 1.9:10 1.9:10 -0.3:10)"
"  (0.8:0 0.8:10)"
"  >8:0"
"}"
"@45 = {"
"  (1:0 6.5:0 6.5:1)"
"  (1:10 6.5:10 6.5:9)"
"  (2:0 2:10)"
"  (2:5 5.5:5)"
"  (5.5:4 5.5:6)"
"  >8:0"
"}"
"@45 = {"
"  (-0.2:0 6:0 6:1)"
"  (-0.2:10 6:10 6:9)"
"  (0.8:0 0.8:10)"
"  (0.8:5 5:5)"
"  (5:4 5:6)"
"  >8:0"
"}"
"@46 = {"
"  (-0.2:0 2.8:0)"
"  (-0.2:10 6:10 6:9)"
"  (0.8:0 0.8:10)"
"  (0.8:5 5:5)"
"  (5:4 5:6)"
"  >8:0"
"}"
"@47 = {"
"  (6:10.2 6:8.2 6,9.2 5,10.2 3.85:10.2 3.3:10.2"
"   -1,10.2 -0.85,-0.2 3.3:-0.2 3.85:-0.2 5,-0.2"
"   6,0.8 6:2 6:2)"
"  (3.5:5 6:5 6:0.1)"
"  >8:0"
"}"
"@48 = {"
"  (-0.3:0 1.7:0)"
"  (-0.3:10 1.7:10)"
"  (0.7:0 0.7:10)"
"  (4.7:0 6.7:0)"
"  (4.7:10 6.7:10)"
"  (5.7:0 5.7:10)"
"  (0.7:5 5.7:5)"
"  >8:0"
"}"
"@49 = {"
"  (0.5:0 3.5:0)"
"  (0.5:10 3.5:10)"
"  (2:0 2:10)"
"  >6:0"
"}"
"@4A = {"
"  (3.5:10 6.5:10)"
"  (0:2.5 0,-1.3 5.5,-1.3 5.5:2.5 5.5:10)"
"  >8:0"
"}"
"@4B = {"
"  (-0.4:0 2.6:0)"
"  (-0.4:10 1.6:10)"
"  (0.6:0 0.6:10)"
"  (6.1:10 5.6,10 4.6,9 4.6:8 4.6,7 3.6,5 1.6:5 0.6:5)"
"  (2.1:5 4.6,5 3.6,0 5.6:0 6.6:0)"
"  >8:0"
"}"
"@4C = {"
"  (0:0 5.5:0 5.5:1)"
"  (0:10 3:10)"
"  (1:0 1:10)"
"  >8:0"
"}"
"@4D = {"
"  (-0.7:0 1.3:0)"
"  (0.3:0 0.3:10)"
"  (6.3:0 8.3:0)"
"  (7.3:0 7.3:10)"
"  (-0.7:10 0.3:10 3.8:3 7.3:10 8.3:10)"
"  >10:0"
"}"
"@4E = {"
"  (-0.7:0 1.3:0)"
"  (4.3:10 6.3:10)"
"  (0.3:0 0.3:10)"
"  (5.3:0 5.3:10)"
"  (-0.7:10 0.3:10 5.3:0 6.3:0)"
"  >8:0"
"}"
"@4F = {"
"  (3.5:10.2 -1.45,10.2 -1.45,-0.2 3.5:-0.2 3.5:-0.2"
"   8.45,-0.2 8.45,10.2 3.5:10.2 3.5:10.2)"
"  >9:0"
"}"
"@50 = {"
"  (1:0 4:0)"
"  (2:0 2:10)"
"  (1:10 3.5:10 7.5,10 7.5,5 3.5:5 2:5)"
"  >8:0"
"}"
"@50 = {"
"  (0:0 3:0)"
"  (1:0 1:10)"
"  (0:10 2.5:10 6.5,10 6.5,5 2.5:5 1:5)"
"  >8:0"
"}"
"@51 = {"
"  (3.5:10.2 -1.45,10.2 -1.45,-0.2 3.5:-0.2"
"   3.5:-0.2 8.45,-0.2 8.45,10.2 3.5:10.2 3.5:10.2)"
"  (3.5:-0.2 1.3,-0.2 1.3,3 3.5:3 4.325,3 5.15,2.5"
"   5.15:0 5.15,-0.5 5.7,-1 6.8:-1 8.45:-1)"
"  >9:0"
"}"
"@52 = {"
"  (-0.5:0 2.5:0)"
"  (0.5:0 0.5:10)"
"  (-0.5:10 2:10 6,10 6,5 2:5 0.5:5)"
"  (2:5 4.5,5 3.5,0 5.5:0 6.5:0)"
"  >8:0"
"}"
"@53 = {"
"  (5:10 5:8 5,9 4,10.2 3:10.2 -1.2,10.2 -1.2,5"
"   2.5:5 3:5 6.5,5 6.5,-0.3 3:-0.3 2.5:-0.3"
"   1.5,-0.3 -0.3,0.7 -0.3:1.7 -0.3:2)"
"  >8:0"
"}"
"@54 = {"
"  (0:9 0:10 6:10 6:9)"
"  (3:0 3:10)"
"  (1:0 5:0)"
"  >8:0"
"}"
"@55 = {"
"  (-0.7:10 1.3:10)"
"  (4.3:10 6.3:10)"
"  (0.3:10 0.3:2.5 0.3,-1.3 5.3,-1.3 5.3:2.5 5.3:10)"
"  >8:0"
"}"
"@56 = {"
"  (-0.5:10 1.5:10)"
"  (4.5:10 6.5:10)"
"  (0.5:10 3:-0.3 5.5:10)"
"  >8:0"
"}"
"@57 = {"
"  (-0.3:10 1.7:10)"
"  (6.7:10 8.7:10)"
"  (0.7:10 2.2:-0.2 4.2:6 6.2:-0.2 7.7:10)"
"  >10:0"
"}"
"@58 = {"
"  (-0.5:0 1.5:0)"
"  (-0.5:10 1.5:10)"
"  (4.5:0 6.5:0)"
"  (4.5:10 6.5:10)"
"  (5.5:0 0.5:10)"
"  (0.5:0 5.5:10)"
"  >8:0"
"}"
"@59 = {"
"  (-0.5:10 1.5:10)"
"  (4.5:10 6.5:10)"
"  (0.5:10 3:5 5.5:10)"
"  (3:0 3:5)"
"  (2:0 4:0)"
"  >8:0"
"}"
"@5A = {"
"  (1.5:9 1.5:10 6.5:10"
"   1.5:0 6.5:0 6.5:1)"
"  >8:0"
"}"
"@5A = {"
"  (0.5:9 0.5:10 5.5:10 0.5:0 5.5:0 5.5:1)"
"  >8:0"
"}"
"@61 = {"
"  (0.2:4.8 0.6,6.6 4,7 3.88:4 3.88:0.8 4,0.4"
"   4.4,0 4.8:0)"
"  (3.88:4 3.88,2.8 -0.4,3.6 -0.2:1.4 -0.2,-0.8"
"   3.88,-0.8 3.88:2)"
"  >6:0"
"}"
"@62 = {"
"  (-0.8:9.8 0:10 0:0.8 0.4,-0.5 4,-1 4:2.8 4:4"
"   4,7 0,6.8 0:4)"
"  >6:0"
"}"
"@63 = {"
"  (3.9:5.8 3.9:4.4 3.9,6.6 -0.1,7.4 -0.1:3"
"   -0.1,-1.4 3.9,-0.5 3.9:1.2)"
"  >6:0"
"}"
"@64 = {"
"  (4:4.4 4,6.6 0,7.4 0:3 0,-1.4 4,-0.5 4:1.6)"
"  (3.08:9.8 4:10 4:0.8 4,0.4 4.4,0 4.8:0)"
"  >6:0"
"}"
"@65 = {"
"  (-0.1:3.2 3.98:3.2 3.9:4.4 3.9,6.4 -0.1,7.4 -0.1:3"
"   -0.1,-1.4 3.9,-0.5 3.9:1.6)"
"  >6:0"
"}"
"@66 = {"
"  (3.6:8.9 3.6,9.7 0.8,10.3 0.8:8.1 0.8:-2)"
"  (0:6 3.6:6)"
"  >6:0"
"}"
"@67 = {"
"  (3.2:4.4 3.2,6.4 0,6.8 0:4 0,1.2 3.2,1.6 3.2:3.6"
"   3.2:4 3.2,4.8 3.2,5.2 4.4:6)"
"  (1.2:2 0,2 0,0.8 1.2:0.8 2:0.8 3.2,0.8 4,0.8"
"   4:-0.4 4:-0.8 4,-2.8 0,-2.8 0:-0.8 0:-0.4 0,0.4"
"   0.4,0.8 1.2:0.8)"
"  >6:0"
"}"
"@68 = {"
"  (-0.68:9.8 0:10 0:0)"
"  (-0.8:0 0.8:0)"
"  (4.8:0 4:0 4:3 4,7.4 0.8,6 0.04:4)"
"  >6:0"
"}"
"@69 = {"
"  (-0.52:6 0.4:6.2 0.4:0)"
"  (-0.4:0 1.2:0)"
"  >3:0"
"}"
"@6A = {"
"  (-0.52:6 0.4:6.2 0.4:0 0.4,-0.6 0.4,-2.8 -1.6:-1.8)"
"  >3:0"
"}"
"@6B = {"
"  (-0.92:9.8 0:10 0:0)"
"  (-0.8:0 0.8:0)"
"  (3.6:6 0.08:2)"
"  (2.8:6 4.4:6)"
"  (1.2:3.2 4:0)"
"  (3.2:0 4.8:0)"
"  >6:0"
"}"
"@6C = {"
"  (0.68:9 1.6:9.2 1.6:0)"
"  (0.8:0 2.4:0)"
"  >3:0"
"}"
"@6C = {"
"  (-0.92:9.8 0:10 0:0)"
"  (-0.8:0 0.8:0)"
"  >3:0"
"}"
"@6D = {"
"  (-0.7176:6 0:6.2 0:0)"
"  (-0.624:0 0.624:0)"
"  (3.744:0 3.12:0 3.12:3.6 3.12,8 0.624,6 0.0624:4)"
"  (6.864:0 6.24:0 6.24:3.6 6.24,8 3.744,6 3.1824:4)"
"  >8.97:0"
"}"
"@6E = {"
"  (-0.92:6 0:6.2 0:0)"
"  (-0.8:0 0.8:0)"
"  (4.8:0 4:0 4:3.6 4,8 0.8,6 0.08:4)"
"  >6:0"
"}"
"@6F = {"
"  (-0.2:3 -0.2,-1.26 4.2,-1.26 4.2:3 4.2,7.26 -0.2,7.26 -0.2:3)"
"  >6:0"
"}"
"@70 = {"
"  (0.8:5 2,6.8 4.8,7.2 4.8:3 4.8,-0.4 1.6,-0.4 0.8:0.8)"
"  (-0.12:6 0.8:6.2 0.8:-2)"
"  (0:-2 1.6:-2)"
"  >6:0"
"}"
"@70 = {"
"  (0:5 1.29,6.8 4.3,7.2 4.3:3 4.3,-0.4 0.86,-0.4 0:0.8)"
"  (-1:6 0:6.2 0:-2)"
"  (-0.86:-2 0.86:-2)"
"  >6:0"
"}"
"@71 = {"
"  (4.095:4.62 4.095,6.93 -0.105,7.77 -0.105:3.15"
"   -0.105,-1.47 4.095,-0.525 4.095:1.68)"
"  (4.935:6.3 4.095:6.3 4.095:-2.1)"
"  (4.935:-2.1 3.255:-2.1)"
"  >6:0"
"}"
"@72 = {"
"  (3.96:5.2 3.96,7 0,6.5 0:3.6 0:2.8)"
"  (-1.012:6 0:6.2 0:0)"
"  (-0.88:0 0.88:0)"
"  >6:0"
"}"
"@73 = {"
"  (4:6 4:4.4)"
"  (4:4.4 4,6.6 0,7 0:4.4 0,2.6 4,3.4 4:1.6"
"   4,-1 0,-0.6 0:1.6)"
"  >6:0"
"}"
"@74 = {"
"  (0.6:7.6 0.6:1.4 0.6,-0.7 3.4,-0.3 3.4:0.6)"
"  (-0.2:6 3.4:6)"
"  >6:0"
"}"
"@75 = {"
"  (-0.92:6 0:6.2 0:2 0,-1.2 4,-0.4 4:2.4)"
"  (3.08:6 4:6.2 4:0.8 4,0.4 4.4,0 4.8:0)"
"  >6:0"
"}"
"@76 = {"
"  (-0.2:6 1.2:6)"
"  (4.4:6 5.8:6)"
"  (0.6:6 2.8:-0.14 5:6)"
"  >6.4:0"
"}"
"@76 = {"
"  (-1:6 0.4:6)"
"  (3.6:6 5:6)"
"  (-0.2:6 2:-0.14 4.2:6)"
"  >6:0"
"}"
"@77 = {"
"  (-0.87:6 0.348:6)"
"  (6.612:6 7.83:6)"
"  (-0.174:6 1.74:-0.2 3.48:6 5.22:-0.2 7.134:6)"
"  >9:0"
"}"
"@78 = {"
"  (0:6 1.6:6)"
"  (0:0 1.6:0)"
"  (4:6 5.6:6)"
"  (4:0 5.6:0)"
"  (4.8:6 0.8:0)"
"  (0.8:6 4.8:0)"
"  >6.4:0"
"}"
"@78 = {"
"  (-0.8:6 0.8:6)"
"  (-0.8:0 0.8:0)"
"  (3.2:6 4.8:6)"
"  (3.2:0 4.8:0)"
"  (4:6 0:0)"
"  (0:6 4:0)"
"  >6:0"
"}"
"@79 = {"
"  (4.8:6 3.08:0 2.8,-1 2.8,-2.8 0.4:-1.6)"
"  (0.8:6 3:0)"
"  (0:6 1.6:6)"
"  (4:6 5.6:6)"
"  >6:0"
"}"
"@79 = {"
"  (3.9:6 2.18:0 1.9,-1 1.9,-2.8 -0.5:-1.6)"
"  (-0.1:6 2.1:0)"
"  (-0.9:6 0.7:6)"
"  (3.1:6 4.7:6)"
"  >6:0"
"}"
"@7A = {"
"  (0:5.2 0:6 4:6 0:0 4:0 4:0.8)"
"  >6:0"
"}"
"@7C = {"
"  (1.5:-2 1.5:10)"
"  >3:0"
"}"
;

FILE *stropen(char *str, char *mode) {
	return fmemopen(str, strlen(str), mode);
}

scanner *new_scanner(char *filename) {
	scanner *res = 0;
	FILE *inf;
    char *USE_INTERNAL_DATA = 0;
	if(filename == USE_INTERNAL_DATA) {
        inf =  stropen(internal_glyph_data, "rt");
	}
	else {
        inf = fopen(filename, "rt");
	}
	if(inf != 0) {
	    res = (scanner *)malloc(sizeof(scanner));
	    res->inf = inf;
	    res->token = (scantok *)malloc(sizeof(scantok));
	    res->unscanned = 0;
	}
	return res;
}

void tok_set(scantok *tok, int type) {
	tok->type = type;
	tok->var.ival = 0;
}

void tok_seti(scantok *tok, int type, int ival) {
	tok->type = type;
	tok->var.ival = ival;
}

void tok_setf(scantok *tok, int type, float fval) {
	tok->type = type;
	tok->var.fval = fval;
}

int ignore_blanks(FILE *inf) {
	int ch = fgetc(inf);
	while(!feof(inf) && strchr(" \t\n", ch))
		ch = fgetc(inf);
	return ch;
}

int hex_digit(char ch) {
	if('0' <= ch && ch <= '9') return 1;
	if('A' <= ch && ch <= 'F') return 1;
	return 0;
}

int to_hex(char ch) {
	if('0' <= ch && ch <= '9') return ch-'0';
	if('A' <= ch && ch <= 'F') return ch-'A'+10;
	return -1;
}

int scan_hex(FILE *inf) {
	int hex = 0, ch;
	ch = fgetc(inf);
	if(!hex_digit(ch)) return -1;
	hex = hex*0x10 + to_hex(ch);
	ch = fgetc(inf);
	while(hex_digit(ch)) {
		hex = hex*0x10 + to_hex(ch);
		ch = fgetc(inf);
	}
	ungetc(ch, inf);
	return hex;
}

int is_float_start(int ch) {
	if(ch == '.') return 1;
	if(ch == '+' || ch == '-') return 1;
	if('0' <= ch && ch <= '9') return 1;
	return 0;
}

int is_digit(int ch) {
	return '0' <= ch && ch <= '9';
}

int to_num(char ch) {
	if('0' <= ch && ch <= '9') return ch-'0';
	return -1;
}

float scan_float(FILE *inf, int *OK) {
	int ch;
	int sgn = 1, ipart = 0, fpart = 0, fraction = 0;
	*OK = 1;
	/* First character */
	ch = fgetc(inf);
	if(ch == '+') sgn = 1;
	else if(ch == '-') sgn = -1;
	else if(ch == '.') fraction = 10;
	else if(is_digit(ch)) ipart = ipart*10 + to_num(ch);
    else { *OK = 0; return 0; }

	/* Integer part */
	ch = fgetc(inf);
	while(fraction == 0) {
		if(ch == '.') fraction = 1;
	    else if(is_digit(ch)) ipart = ipart*10 + to_num(ch);
        else {
        	ungetc(ch, inf);
            return sgn*(ipart);
        }
        ch = fgetc(inf);
	}

	/* Fractional part */
	while(1) {
		if(ch == '.') { *OK = 0; return 0; }
	    else if(is_digit(ch)) {
	    	fpart = fpart*10 + to_num(ch);
	    	fraction *= 10;
	    }
        else {
        	ungetc(ch, inf);
            return sgn*(ipart + (float)fpart/(float)fraction);
        }
        ch = fgetc(inf);
	}
}

int unscan(scanner *sc) {
	if(sc->unscanned) {
		fprintf(stderr, "FATAL: only one unscan allowed\n");
		exit(-1);
	    return 0;
	}
	sc->unscanned = 1;
	return 1;
}

int scan(scanner *sc) {
	int ch;
	ch = ignore_blanks(sc->inf);
	if(sc->unscanned) {
	    sc->unscanned = 0;
	    return sc->token->type;
	}
	if(feof(sc->inf)) {
	    tok_set(sc->token, SCAN_EOF);
		return SCAN_EOF;
	}
	if(ch == '@') {
		int hex = scan_hex(sc->inf);
		if(hex == -1) {
            tok_set(sc->token, SCAN_ERROR);
            return SCAN_ERROR;
		}
	    tok_seti(sc->token, SCAN_GCODE, hex);
	    return SCAN_GCODE;
	}
	if(ch == '=') {
	    tok_set(sc->token, SCAN_EQUALS);
		return SCAN_EQUALS;
	}
	if(ch == '{') {
	    tok_set(sc->token, SCAN_LBRACE);
		return SCAN_LBRACE;
	}
	if(ch == '}') {
	    tok_set(sc->token, SCAN_RBRACE);
		return SCAN_RBRACE;
	}
	if(ch == '(') {
	    tok_set(sc->token, SCAN_LPAR);
		return SCAN_LPAR;
	}
	if(ch == ')') {
	    tok_set(sc->token, SCAN_RPAR);
		return SCAN_RPAR;
	}
	if(ch == ':') {
	    tok_set(sc->token, SCAN_COLON);
		return SCAN_COLON;
	}
	if(ch == ',') {
	    tok_set(sc->token, SCAN_COMMA);
		return SCAN_COMMA;
	}
	if(ch == '>') {
	    tok_set(sc->token, SCAN_GTR);
		return SCAN_GTR;
	}
	if(is_float_start(ch)) {
		int OK;
		ungetc(ch, sc->inf);
		float fval = scan_float(sc->inf, &OK);
		if(!OK) {
            tok_set(sc->token, SCAN_ERROR);
            return SCAN_ERROR;
		}
		tok_setf(sc->token, SCAN_FLOAT, fval);
		return SCAN_FLOAT;
	}
	tok_set(sc->token, SCAN_ERROR);
	return SCAN_ERROR;
}

int get_type(scanner *sc) {
	return sc->token->type;
}

int get_ival(scanner *sc) {
	switch(sc->token->type) {
	    case SCAN_GCODE:
	    case SCAN_HEXINT:
		    return sc->token->var.ival;
		default: 
			fprintf(stderr, "FATAL: get_ival on non-ival token!\n");
			print_tok(sc);
			printf("\n");
			exit(-2);
	}
	return -1;
}

float get_fval(scanner *sc) {
	switch(sc->token->type) {
	    case SCAN_FLOAT:
		    return sc->token->var.fval;
		default: 
			printf("FATAL: get_fval on non-fval token!\n  ");
			print_tok(sc);
			printf("\n");
			exit(-2);
	}
	return -1;
}

void print_tok(scanner *sc) {
    scantok *tok = sc->token;
	switch(tok->type) {
	    case SCAN_ERROR:
	    	printf("error\n");
	    	break;
        case SCAN_GCODE:
	    	printf("@%X\n", tok->var.ival);
	    	break;
        case SCAN_EQUALS:
	    	printf("=\n");
	    	break;
        case SCAN_LBRACE:
	    	printf("{\n");
	    	break;
        case SCAN_RBRACE:
	    	printf("}\n");
	    	break;
        case SCAN_LPAR:
	    	printf("(\n");
	    	break;
        case SCAN_RPAR:
	    	printf(")\n");
	    	break;
        case SCAN_HEXINT:
	    	printf("%x\n", tok->var.ival);
	    	break;
        case SCAN_FLOAT:
	    	printf("%g\n", tok->var.fval);
	    	break;
        case SCAN_COLON:
	    	printf(":\n");
	    	break;
        case SCAN_COMMA:
	    	printf(",\n");
	    	break;
        case SCAN_GTR:
	    	printf(">\n");
	    	break;
        case SCAN_EOF:
	    	printf("EOF\n");
	    	break;
        default:
	    	printf("(unknown)\n");
	    	break;
	}
}
