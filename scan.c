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
"@41 = {"
"  (0:0 2:0)"
"  (6:0 8:0)"
"  (1:0 4:10.2 7:0)"
"  (2:3.5 6:3.5)"
"  >8:0"
"}"
"@42 = {"
"  (2:0 2:10)"
"  (1:10 3.5:10 7.5,10 7.5,5 3.5:5 2:5)"
"  (2:5 4:5 8,5 8,0 4:0 1:0)"
"  >8:0"
"}"
"@43 = {"
"  (6.5:10.2 6.5:8.2 6.5,9.2 5.5,10.2 4.5:10.2"
"   4:10.2 0.2,10.2 0.3,-0.2 4:-0.2 4.5:-0.2"
"   5.5,-0.2 6.5,0.8 6.5:2 6.5:2)"
"  >8:0"
"}"
"@44 = {"
"  (1:0 3:0 8,0 8,10 3:10 3:10 1:10)"
"  (2:0 2:10)"
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
"@46 = {"
"  (1:0 4:0)"
"  (1:10 6.5:10 6.5:9)"
"  (2:0 2:10)"
"  (2:5 5.5:5)"
"  (5.5:4 5.5:6)"
"  >8:0"
"}"
"@47 = {"
"  (6.5:10.2 6.5:8.2 6.5,9.2 5.5,10.2 4.5:10.2"
"   4:10.2 0.2,10.2 0.3,-0.2 4:-0.2 4.5:-0.2"
"   5.5,-0.2 6.5,0.8 6.5:2 6.5:2)"
"  (4:5 6.5:5 6.5:0.2)"
"  >8:0"
"}"
"@48 = {"
"  (0.5:0 2.5:0) (0.5:10 2.5:10)"
"  (1.5:0 1.5:10) (5.5:0 7.5:0)"
"  (5.5:10 7.5:10) (6.5:0 6.5:10)"
"  (1.5:5 6.5:5)"
"  >8:0"
"}"
"@49 = {"
"  (2:0 4:0)"
"  (2:10 4:10)"
"  (3:0 3:10)"
"  >6:0"
"}"
"@4A = {"
"  (4.5:10 7.5:10)"
"  (1:2.5 1,-1.3 6.5,-1.3"
"   6.5:2.5 6.5:10)"
"  >8:0"
"}"
"@4B = {"
"  (1:0 4:0)"
"  (2:0 2:10)"
"  (7.5:10 7,10 6,9 6:8"
"   6,7 5,5 3:5 2:5)"
"  (3.5:5 6,5 5,0 7:0 8:0)"
"  >8:0"
"}"
"@4C = {"
"  (1:0 6.5:0 6.5:1)"
"  (1:10 4:10)"
"  (2:0 2:10)"
"  >8:0"
"}"
"@4D = {"
"  (0.5:0 2.5:0)"
"  (1.5:0 1.5:10) (7.5:0 9.5:0)"
"  (8.5:0 8.5:10)"
"  (0.5:10 1.5:10 5:3 8.5:10 9.5:10)"
"  >10:0"
"}"
"@4E = {"
"  (0.5:0 2.5:0)"
"  (5.5:10 7.5:10)"
"  (1.5:00 1.5:10)"
"  (6.5:0 6.5:10)"
"  (0.5:10 1.5:10 6.5:0 7.5:0)"
"  >8:0"
"}"
"@4F = {"
"  (5:10.2 0.5,10.2 0.5,-0.2 5:-0.2"
"   5:-0.2 9.5,-0.2 9.5,10.2 5:10.2 5:10.2)"
"  >10:0"
"}"
"@50 = {"
"  (1:0 4:0)"
"  (2:0 2:10)"
"  (1:10 3.5:10 7.5,10 7.5,5 3.5:5"
"   2:5)"
"  >8:0"
"}"
"@51 = {"
"  (5:10.2 0.5,10.2 0.5,-0.2 5:-0.2"
"   5:-0.2 9.5,-0.2 9.5,10.2 5:10.2 5:10.2)"
"  (5:-0.2 3,-0.2 3,3 5:3"
"   5.75,3 6.5,2.5 6.5:0 6.5,-0.5 7,-1 8:-1"
"   9.5:-1)"
"  >10:0"
"}"
"@52 = {"
"  (1:0 4:0)"
"  (2:0 2:10)"
"  (1:10 3.5:10 7.5,10 7.5,5 3.5:5 2:5)"
"  (3.5:5 6,5 5,0 7:0 8:0)"
"  >8:0"
"}"
"@53 = {"
"  (6.5:10 6.5:8 6.5,9 5.5,10.2 4.5:10.2"
"   0.3,10.2 0.3,5 4:5 4.5:5"
"   8,5 8,-0.3 4.5:-0.3 4:-0.3"
"   3,-0.3 1.2,0.7 1.2:1.7 1.2:2)"
"  >8:0"
"}"
"@54 = {"
"  (1:9 1:10 7:10 7:9)"
"  (4:0 4:10) (2:0 6:0)"
"  >8:0"
"}"
"@55 = {"
"  (0.5:10 2.5:10)"
"  (5.5:10 7.5:10)"
"  (1.5:10 1.5:2.5 1.5,-1.3 6.5,-1.3"
"   6.5:2.5 6.5:10)"
"  >8:0"
"}"
"@56 = {"
"  (0.5:10 2.5:10)"
"  (5.5:10 7.5:10)"
"  (1.5:10 4:-0.3 6.5:10)"
"  >8:0"
"}"
"@57 = {"
"  (0.5:10 2.5:10)"
"  (7.5:10 9.5:10)"
"  (1.5:10 3:-0.2 5:6 7:-0.2 8.5:10)"
"  >10:0"
"}"
"@58 = {"
"  (0.5:0 2.5:0) (0.5:10 2.5:10)"
"  (5.5:0 7.5:0) (5.5:10 7.5:10)"
"  (6.5:0 1.5:10) (1.5:0 6.5:10)"
"  >8:0"
"}"
"@59 = {"
"  (0.5:10 2.5:10)"
"  (5.5:10 7.5:10)"
"  (1.5:10 4:5 6.5:10)"
"  (4:0 4:5)"
"  (3:0 5:0)"
"  >8:0"
"}"
"@5A = {"
"  (1.5:9 1.5:10 6.5:10"
"   1.5:0 6.5:0 6.5:1)"
"  >8:0"
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
