#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "scan.h"

scanner *new_scanner(char *filename) {
	scanner *res = 0;
	FILE *inf;
    const int USE_INTERNAL_DATA = 0;
	/* NYI: here test whether filename == USE_INTERNAL_DATA
	 * and then instead
	 *     inf = fmemopen(buffer, strlen(buffer), "rt");
	 */
	inf = fopen(filename, "rt");
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
		if(ch == '.') fraction = 10;
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
	    else if(is_digit(ch)) fpart = fpart*10 + to_num(ch);
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
