#include <u.h>
#include <libc.h>
#include <ctype.h>

typedef double Awkfloat;

enum{
	NUM = 1<<0,
	FLT = 1<<1,
};

static int is_float(char *s, Awkfloat *fp, char **tp)
{
	char c, *p, *q;
	Awkfloat f;

	f = *fp = strtod(s, &p);
	if (tp != nil)
		*tp = p;
	if (p == s)
		return 0;
	else if (isInf(f, 1) || isInf(f, -1) || isNaN(f))
		return 0;
	else if (f == 0.0 && ((q = strchr(s, '0')) == nil || q > p))
		return 0;
	else if (tp != nil)
		return 1;
	for (; (c = *p) != '\0'; p++) {
		switch(c) {
		case ' ':
		case '\t':
		case '\n':
		case '\f':
		case '\r':
		case '\v':
			continue;
		case '\0':
			return 1;
		default:
			return 0;
		}
	}
	return 1;
}

int to_number(char *s, Awkfloat *fp, char **tp)
{
	vlong v;
	char c, *p, *q;

	v = strtoll(s, &p, 0);
	*fp = (Awkfloat)v;
	if (tp != nil)
		*tp = p;
	switch(*p){
	case '.':
	case 'E':
	case 'I':	/* inf */
	case 'N':	/* nan */
	case 'e':
	case 'i':
	case 'n':
		if (is_float(s, fp, tp))
			return NUM | FLT;
		return 0;
	}
	if (p == s)
		return 0;
	else if (v == 0 && ((q = strchr(s, '0')) == nil || q > p))
		return 0;
	else if (tp != nil)
		return NUM;
	for (; (c = *p) != '\0'; p++) {
		switch(c) {
		case ' ':
		case '\t':
		case '\n':
		case '\f':
		case '\r':
		case '\v':
			continue;
		case '\0':
			return NUM;
		default:
			return 0;
		}
	}
	return NUM;
}

void
main(void)
{
	int r;
	Awkfloat f;

	r = to_number("3", &f, nil); assert(r == NUM);
	r = to_number(" 3", &f, nil); assert(r == NUM);
	r = to_number("3 ", &f, nil); assert(r == NUM);
	r = to_number(" 3 ", &f, nil); assert(r == NUM);
	r = to_number("3x", &f, nil); assert(r == 0);
	r = to_number(" 3x", &f, nil); assert(r == 0);
	r = to_number("3x ", &f, nil); assert(r == 0);
	r = to_number("3x tyu", &f, nil); assert(r == 0);
	r = to_number("3e1", &f, nil); assert(r == (NUM | FLT));
	r = to_number("3e", &f, nil); assert(r == 0);
	r = to_number("-3e", &f, nil); assert(r == 0);
	r = to_number(" -3e ", &f, nil); assert(r == 0);
	r = to_number("3.1", &f, nil); assert(r == (NUM | FLT));
	r = to_number(" 3.1", &f, nil); assert(r == (NUM | FLT));
	r = to_number("3.1 ", &f, nil); assert(r == (NUM | FLT));
	r = to_number(" 3.1 ", &f, nil); assert(r == (NUM | FLT));
	r = to_number("x ", &f, nil); assert(r == 0);
	r = to_number(".1 ", &f, nil); assert(r == (NUM | FLT));
	r = to_number("-1e4 ", &f, nil); assert(r == (NUM | FLT));
	r = to_number(".", &f, nil); assert(r == 0);
	r = to_number("", &f, nil); assert(r == 0);
	r = to_number("0", &f, nil); assert(r == NUM);
	r = to_number("0.", &f, nil); assert(r == (NUM | FLT));
	r = to_number("0.0", &f, nil); assert(r == (NUM | FLT));
	r = to_number("0e", &f, nil); assert(r == 0);
	r = to_number("nan", &f, nil); assert(r == 0);
	r = to_number("inf", &f, nil); assert(r == 0);
	/* FIXME: more tests + testcases with tp != nil */
	exits(nil);
}
