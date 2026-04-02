/*
* Copyright (c) 2025-present Wojciech Kaptur ( _Sqyd_ / Sqydev )
* Github: https://github.com/Sqydev
* GPG Fingerprint: 6DC2516B0DFDA9C59661650722F7B8A777F33B56
* 
* This software is provided "as-is", without any express or implied warranty. In no event
* will the authors be held liable for any damages arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose, including commercial
* applications, and to alter it and redistribute it freely, subject to the following restrictions:
* 
* 1. Non-Misrepresentation: The origin of this software must not be misrepresented; 
*    you must not claim that you wrote the original software. An acknowledgment in 
*    product documentation is appreciated but not required.
* 
* 2. Source-Level Copyleft: Any altered versions (forks) of this software's source code, 
*    or files containing significant portions of this code, must be distributed under 
*    these same license terms. Such modified source code must be made publicly available 
*    to any recipient, even if used over a network (SaaS).
* 
* 3. Proprietary Integration: This software may be integrated into, linked with, or 
*    used as a component of proprietary and closed-source products. In such cases, 
*    the surrounding proprietary application code does not need to be disclosed, 
*    provided that the original or modified source code of THIS software remains 
*    available under the terms of Section 2.
* 
* 4. Persistent Metadata: All original credits, including those in the source code headers 
*    and binary metadata (e.g., ELF .comment section, PE StringFileInfo, or equivalent), 
*    must not be removed. You may add your own credits to forks, provided the original 
*    authorship remains clearly identified.
* 
* 5. Notice Retention: This license notice may not be removed or altered from any 
*    source or binary distribution.
*/

#include "./drawTextfCORE.h"
#include "./coredata.h"
#include "./common_utils.h"

#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#include <wchar.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <float.h>

#define MAX(a,b) ((a)>(b) ? (a) : (b))
#define MIN(a,b) ((a)<(b) ? (a) : (b))

#define ALT_FORM (1U<<('#'-' '))
#define ZERO_PAD (1U<<('0'-' '))
#define LEFT_ADJ (1U<<('-'-' '))
#define PAD_POS  (1U<<(' '-' '))
#define MARK_POS (1U<<('+'-' '))
#define GROUPED  (1U<<('\''-' '))
#define FLAGMASK (ALT_FORM|ZERO_PAD|LEFT_ADJ|PAD_POS|MARK_POS|GROUPED)

enum {
	BARE, LPRE, LLPRE, HPRE, HHPRE, BIGLPRE, ZTPRE, JPRE,
	STOP,
	PTR, INT, UINT, ULLONG, LONG, ULONG,
	SHORT, USHORT, CHAR, UCHAR, LLONG, SIZET, IMAX, UMAX, PDIFF, UIPTR,
	DBL, LDBL, NOARG, MAXSTATE
};

#define OOB(x) ((unsigned)(x)-'A' > (unsigned)('z'-'A'))

union arg {
	uintmax_t i;
	long double f;
	void *p;
};

static unsigned char states[8]['z'-'A'+1];
static int states_initialized = 0;

static void init_states(void) {
	if (states_initialized) return;
	memset(states, 0, sizeof(states));

	states[0]['d'-'A']=INT;  states[0]['i'-'A']=INT;  states[0]['o'-'A']=UINT; states[0]['u'-'A']=UINT; states[0]['x'-'A']=UINT; states[0]['X'-'A']=UINT;
	states[0]['e'-'A']=DBL;  states[0]['f'-'A']=DBL;  states[0]['g'-'A']=DBL;  states[0]['a'-'A']=DBL;
	states[0]['E'-'A']=DBL;  states[0]['F'-'A']=DBL;  states[0]['G'-'A']=DBL;  states[0]['A'-'A']=DBL;
	states[0]['c'-'A']=INT;  states[0]['C'-'A']=UINT; states[0]['s'-'A']=PTR;  states[0]['S'-'A']=PTR;  states[0]['p'-'A']=UIPTR; states[0]['n'-'A']=PTR;
	states[0]['m'-'A']=NOARG;
	states[0]['l'-'A']=LPRE; states[0]['h'-'A']=HPRE; states[0]['L'-'A']=BIGLPRE; states[0]['z'-'A']=ZTPRE; states[0]['j'-'A']=JPRE; states[0]['t'-'A']=ZTPRE;

	states[1]['d'-'A']=LONG;  states[1]['i'-'A']=LONG;  states[1]['o'-'A']=ULONG; states[1]['u'-'A']=ULONG; states[1]['x'-'A']=ULONG; states[1]['X'-'A']=ULONG;
	states[1]['e'-'A']=DBL;   states[1]['f'-'A']=DBL;   states[1]['g'-'A']=DBL;   states[1]['a'-'A']=DBL;
	states[1]['E'-'A']=DBL;   states[1]['F'-'A']=DBL;   states[1]['G'-'A']=DBL;   states[1]['A'-'A']=DBL;
	states[1]['c'-'A']=UINT;  states[1]['s'-'A']=PTR;   states[1]['n'-'A']=PTR;   states[1]['l'-'A']=LLPRE;

	states[2]['d'-'A']=LLONG; states[2]['i'-'A']=LLONG; states[2]['o'-'A']=ULLONG; states[2]['u'-'A']=ULLONG; states[2]['x'-'A']=ULLONG; states[2]['X'-'A']=ULLONG; states[2]['n'-'A']=PTR;

	states[3]['d'-'A']=SHORT; states[3]['i'-'A']=SHORT; states[3]['o'-'A']=USHORT; states[3]['u'-'A']=USHORT; states[3]['x'-'A']=USHORT; states[3]['X'-'A']=USHORT; states[3]['n'-'A']=PTR; states[3]['h'-'A']=HHPRE;

	states[4]['d'-'A']=CHAR;  states[4]['i'-'A']=CHAR;  states[4]['o'-'A']=UCHAR;  states[4]['u'-'A']=UCHAR;  states[4]['x'-'A']=UCHAR;  states[4]['X'-'A']=UCHAR;  states[4]['n'-'A']=PTR;

	states[5]['e'-'A']=LDBL;  states[5]['f'-'A']=LDBL;  states[5]['g'-'A']=LDBL;   states[5]['a'-'A']=LDBL;
	states[5]['E'-'A']=LDBL;  states[5]['F'-'A']=LDBL;  states[5]['G'-'A']=LDBL;   states[5]['A'-'A']=LDBL;   states[5]['n'-'A']=PTR;

	states[6]['d'-'A']=PDIFF; states[6]['i'-'A']=PDIFF; states[6]['o'-'A']=SIZET;  states[6]['u'-'A']=SIZET;  states[6]['x'-'A']=SIZET;  states[6]['X'-'A']=SIZET;  states[6]['n'-'A']=PTR;

	states[7]['d'-'A']=IMAX;  states[7]['i'-'A']=IMAX;  states[7]['o'-'A']=UMAX;   states[7]['u'-'A']=UMAX;   states[7]['x'-'A']=UMAX;   states[7]['X'-'A']=UMAX;   states[7]['n'-'A']=PTR;

	states_initialized = 1;
}


typedef struct {
	float  curX;
	float  curY;
	Vector2 dir;
	Color* local_fg;
	Color* local_bg;
	int	spacing;
	double angle;
} DrawContext;

static void draw_out(DrawContext* ctx, const char* s, size_t l) {
	if (!s) return;

	const char* p   = s;
	const char* end = s + l;
	while(p < end) {
		if(ctx->curX < 0 || ctx->curX >= DATA.TuiData.tuidimm.x) return;
		if(ctx->curY < 0 || ctx->curY >= DATA.TuiData.tuidimm.y) return;

		int vWidth = GetCharWidth(p);
		if(ctx->curX >= DATA.TuiData.tuidimm.x - (vWidth - 1))
			ctx->curX -= vWidth - 1;

		int clen;
		if	 ((*p & 0x80) == 0)	clen = 1;
		else if((*p & 0xE0) == 0xC0) clen = 2;
		else if((*p & 0xF0) == 0xE0) clen = 3;
		else if((*p & 0xF8) == 0xF0) clen = 4;
		else { p++; continue; }

		if(p + clen > end) break;

		char tmp[5] = {0};
		memcpy(tmp, p, clen);
		DrawCharEx(tmp, (int)roundf(ctx->curX), (int)roundf(ctx->curY), ctx->local_fg, ctx->local_bg);

		ctx->curX += ctx->dir.x * (ctx->spacing + 1);
		ctx->curY += ctx->dir.y * (ctx->spacing + 1);
		p += clen;
	}
}

static void draw_pad(DrawContext* ctx, char c, int w, int l, int fl) {
	char buf[256];
	if(fl & (LEFT_ADJ | ZERO_PAD) || l >= w) return;
	l = w - l;
	memset(buf, c, MIN(l, (int)sizeof(buf)));
	for(; l >= (int)sizeof(buf); l -= (int)sizeof(buf))
		draw_out(ctx, buf, sizeof(buf));
	if(l > 0) draw_out(ctx, buf, l);
}

static const char xdigits[16] = "0123456789ABCDEF";

static char* fmt_x(uintmax_t x, char* s, int lower) {
	for(; x; x>>=4) *--s = xdigits[x&15] | lower;
	return s;
}
static char* fmt_o(uintmax_t x, char* s) {
	for(; x; x>>=3) *--s = '0' + (x&7);
	return s;
}
static char* fmt_u(uintmax_t x, char* s) {
	unsigned long y;
	for(; x>ULONG_MAX; x/=10) *--s = '0' + x%10;
	for(y=x; y>=10; y/=10) *--s = '0' + y%10;
	if(y) *--s = '0' + y;
	return s;
}

static int getint(char** s) {
	int i;
	for(i=0; isdigit(**s); (*s)++) {
		if(i > (int)(INT_MAX/10U) || **s-'0' > (int)(INT_MAX-10*(unsigned)i)) i = -1;
		else i = 10*i + (**s-'0');
	}
	return i;
}

#if LDBL_MANT_DIG == 53
typedef char compiler_defines_long_double_incorrectly[9-(int)sizeof(long double)];
#endif

static int fmt_fp(DrawContext* ctx, long double y, int w, int p, int fl, int t, int ps) {
	int max_mant_dig  = (ps==BIGLPRE) ? LDBL_MANT_DIG : DBL_MANT_DIG;
	int max_mant_exp  = (ps==BIGLPRE) ? LDBL_MAX_EXP  : DBL_MAX_EXP;
	int max_mant_slots = 1 + (max_mant_dig-29+7)/8 + 1;
	int max_exp_slots  = (max_mant_exp+max_mant_dig+28+8)/9;
	int bufsize = max_mant_slots + max_exp_slots;
	uint32_t big[bufsize];
	uint32_t *a, *d, *r, *z;
	int e2=0, e, i, j, l;
	char buf[9+LDBL_MANT_DIG/4], *s;
	const char *prefix="-0X+0X 0X-0x+0x 0x";
	int pl;
	char ebuf0[3*sizeof(int)], *ebuf=&ebuf0[3*sizeof(int)], *estr;

	pl=1;
	if(signbit(y)) { y=-y; }
	else if(fl & MARK_POS) { prefix+=3; }
	else if(fl & PAD_POS)  { prefix+=6; }
	else { prefix++; pl=0; }

	if(!isfinite(y)) {
		char *s2 = (t&32)?"inf":"INF";
		if(y!=y) s2=(t&32)?"nan":"NAN";
		draw_pad(ctx, ' ', w, 3+pl, fl&~ZERO_PAD);
		draw_out(ctx, prefix, pl);
		draw_out(ctx, s2, 3);
		draw_pad(ctx, ' ', w, 3+pl, fl^LEFT_ADJ);
		return MAX(w, 3+pl);
	}

	y = frexpl(y, &e2) * 2;
	if(y) e2--;

	if((t|32)=='a') {
		if(t&32) prefix += 9;
		pl += 2;
		if(p>=0 && p<(LDBL_MANT_DIG-1+3)/4) {
			double round = scalbn(1, LDBL_MANT_DIG-1-(p*4));
			if(*prefix=='-') { y=-y; y-=round; y+=round; y=-y; }
			else			 { y+=round; y-=round; }
		}
		estr=fmt_u(e2<0 ? -e2 : e2, ebuf);
		if(estr==ebuf) *--estr='0';
		*--estr = (e2<0 ? '-' : '+');
		*--estr = t+('p'-'a');
		s=buf;
		do {
			int x=y;
			*s++=xdigits[x]|(t&32);
			y=16*(y-x);
			if(s-buf==1 && (y||p>0||(fl&ALT_FORM))) *s++='.';
		} while(y);
		if(p > INT_MAX-2-(ebuf-estr)-pl) return -1;
		if(p && s-buf-2 < p) l = (p+2) + (ebuf-estr);
		else				 l = (s-buf) + (ebuf-estr);
		draw_pad(ctx, ' ', w, pl+l, fl);
		draw_out(ctx, prefix, pl);
		draw_pad(ctx, '0', w, pl+l, fl^ZERO_PAD);
		draw_out(ctx, buf, s-buf);
		draw_pad(ctx, '0', l-(ebuf-estr)-(s-buf), 0, 0);
		draw_out(ctx, estr, ebuf-estr);
		draw_pad(ctx, ' ', w, pl+l, fl^LEFT_ADJ);
		return MAX(w, pl+l);
	}

	if(p<0) p=6;
	if(y) y *= 0x1p28, e2-=28;

	if(e2<0) a=r=z=big;
	else	 a=r=z=big+sizeof(big)/sizeof(*big) - max_mant_slots - 1;

	do { *z = y; y = 1000000000*(y-*z++); } while(y);

	while(e2>0) {
		uint32_t carry=0;
		int sh=MIN(29,e2);
		for(d=z-1; d>=a; d--) {
			uint64_t x = ((uint64_t)*d<<sh)+carry;
			*d = x % 1000000000;
			carry = x / 1000000000;
		}
		if(carry) *--a = carry;
		while(z>a && !z[-1]) z--;
		e2-=sh;
	}
	while(e2<0) {
		uint32_t carry=0, *b;
		int sh=MIN(9,-e2), need=1+(p+LDBL_MANT_DIG/3U+8)/9;
		for(d=a; d<z; d++) {
			uint32_t rm = *d & ((1<<sh)-1);
			*d = (*d>>sh) + carry;
			carry = (1000000000>>sh) * rm;
		}
		if(!*a) a++;
		if(carry) *z++ = carry;
		b = (t|32)=='f' ? r : a;
		if(z-b > need) z = b+need;
		e2+=sh;
	}

	if(a<z) for(i=10, e=9*(r-a); (uint32_t)*a >= (uint32_t)i; i*=10, e++);
	else e=0;

	j = p - ((t|32)!='f')*e - ((t|32)=='g' && p);
	if(j < 9*(z-r-1)) {
		uint32_t x;
		d = r + 1 + ((j+9*LDBL_MAX_EXP)/9 - LDBL_MAX_EXP);
		j += 9*LDBL_MAX_EXP;
		j %= 9;
		for(i=10, j++; j<9; i*=10, j++);
		x = *d % i;
		if(x || d+1!=z) {
			long double rnd = 2/LDBL_EPSILON;
			long double small;
			if((*d/i & 1) || (i==1000000000 && d>a && (d[-1]&1))) rnd += 2;
			if(x < (uint32_t)i/2) small=0x0.8p0;
			else if(x == (uint32_t)i/2 && d+1==z) small=0x1.0p0;
			else				 small=0x1.8p0;
			if(pl && *prefix=='-') rnd*=-1, small*=-1;
			*d -= x;
			if(rnd+small != rnd) {
				*d = *d + i;
				while(*d > 999999999) {
					*d--=0;
					if(d<a) *--a=0;
					(*d)++;
				}
				if(a<z) for(i=10, e=9*(r-a); (uint32_t)*a >= (uint32_t)i; i*=10, e++);
			}
		}
		if(z>d+1) z=d+1;
	}
	for(; z>a && !z[-1]; z--);

	if((t|32)=='g') {
		if(!p) p++;
		if(p>e && e>=-4) { t--; p-=e+1; }
		else			 { t-=2; p--; }
		if(!(fl&ALT_FORM)) {
			if(z>a && z[-1]) for(i=10, j=0; z[-1]%i==0; i*=10, j++);
			else j=9;
			if((t|32)=='f') p = MIN(p, MAX(0, 9*(z-r-1)-j));
			else			 p = MIN(p, MAX(0, 9*(z-r-1)+e-j));
		}
	}

	if(p > INT_MAX-1-(p || (fl&ALT_FORM))) return -1;
	l = 1 + p + (p || (fl&ALT_FORM));
	if((t|32)=='f') {
		if(e > INT_MAX-l) return -1;
		if(e>0) l+=e;
	} else {
		estr=fmt_u(e<0 ? -e : e, ebuf);
		while(ebuf-estr<2) *--estr='0';
		*--estr = (e<0 ? '-' : '+');
		*--estr = t;
		if(ebuf-estr > INT_MAX-l) return -1;
		l += ebuf-estr;
	}
	if(l > INT_MAX-pl) return -1;

	draw_pad(ctx, ' ', w, pl+l, fl);
	draw_out(ctx, prefix, pl);
	draw_pad(ctx, '0', w, pl+l, fl^ZERO_PAD);

	if((t|32)=='f') {
		if(a>r) a=r;
		for(d=a; d<=r; d++) {
			char *s2 = fmt_u(*d, buf+9);
			if(d!=a) while(s2>buf) *--s2='0';
			else if(s2==buf+9) *--s2='0';
			draw_out(ctx, s2, buf+9-s2);
		}
		if(p || (fl&ALT_FORM)) draw_out(ctx, ".", 1);
		for(; d<z && p>0; d++, p-=9) {
			char *s2 = fmt_u(*d, buf+9);
			while(s2>buf) *--s2='0';
			draw_out(ctx, s2, MIN(9,p));
		}
		draw_pad(ctx, '0', p+9, 9, 0);
	} else {
		if(z<=a) z=a+1;
		for(d=a; d<z && p>=0; d++) {
			char *s2 = fmt_u(*d, buf+9);
			if(s2==buf+9) *--s2='0';
			if(d!=a) while(s2>buf) *--s2='0';
			else {
				draw_out(ctx, s2++, 1);
				if(p>0||(fl&ALT_FORM)) draw_out(ctx, ".", 1);
			}
			draw_out(ctx, s2, MIN(buf+9-s2, p));
			p -= buf+9-s2;
		}
		draw_pad(ctx, '0', p+18, 18, 0);
		draw_out(ctx, estr, ebuf-estr);
	}
	draw_pad(ctx, ' ', w, pl+l, fl^LEFT_ADJ);
	return MAX(w, pl+l);
}

void DrawTextfCORE(const char* text, int x, int y, int originX, int originY, Color* fg, Color* bg, int spacing, double angle, va_list va) {
	if(text == NULL) {
		UniWriteLen(UNI_WRITE_TARGET_STDERR, "Text is NULL\n");
		return;
	}

	init_states();

	DrawContext ctx = {0};

	Color local_fg_storage;
	Color local_bg_storage;

	ctx.local_fg = NULL;
	ctx.local_bg = NULL;

	if (fg) {
		local_fg_storage = *fg;
		ctx.local_fg = &local_fg_storage;
	}

	if (bg) {
		local_bg_storage = *bg;
		ctx.local_bg = &local_bg_storage;
	}

	ctx.spacing = spacing;
	ctx.angle = angle;

	float fcos, fsin;
	float q = (float)(angle / (PI / 2.0));
	int qi = (int)roundf(q);
	
	if(fabsf(q - qi) < 0.001f) {
		int di = (qi % 4 + 4) % 4;
		switch(di) {
		case 0: fcos =  1.f; fsin =  0.f; break;
		case 1: fcos =  0.f; fsin =  1.f; break;
		case 2: fcos = -1.f; fsin =  0.f; break;
		case 3: fcos =  0.f; fsin = -1.f; break;
		default: fcos = 1.f; fsin = 0.f; break;
		}
		ctx.dir.x = fcos;
		ctx.dir.y = fsin;
	} else {
		fcos = cosf((float)angle);
		fsin = sinf((float)angle);
		
		float mx = fmaxf(fabsf(fcos), fabsf(fsin));
		if(mx > 0.f) { 
			ctx.dir.x = fcos / mx; 
			ctx.dir.y = fsin / mx; 
		} else {
			ctx.dir.x = fcos;
			ctx.dir.y = fsin;
		}
	}

	ctx.curX = x - (originX * fcos) + (originY * fsin);
	ctx.curY = y - (originX * fsin) - (originY * fcos);

	char *a, *z, *s = (char*)text;
	unsigned fl;
	int w, p, xp;
	union arg arg;
	unsigned st, ps;
	int cnt=0, l=0;
	int loop_i;
	char buf[sizeof(uintmax_t)*3];
	const char* prefix;
	int t, pl;
	wchar_t wc[2], *ws;
	char mb[4];

	for(;;) {
		if(l > INT_MAX - cnt) goto format_error;
		cnt += l;
		if(!*s) break;

		a = s;
		while(*s) {
			if(*s == '%') break;

			if(*s == '\\' && *(s+1) == '$') {
				draw_out(&ctx, a, s - a);
				draw_out(&ctx, "$", 1);
				s += 2; a = s;
				continue;
			}

			if(*s == '$') {
				if(s > a) draw_out(&ctx, a, s - a);

				if(strncmp(s, "$fg", 3) == 0) {
					Color temp = va_arg(va, Color);
	
					local_fg_storage = temp;
					ctx.local_fg = &local_fg_storage;
	
					s += 3; a = s; continue;
				}

				if(strncmp(s, "$bg", 3) == 0) {
					Color temp = va_arg(va, Color);
	
					local_bg_storage = temp;
					ctx.local_bg = &local_bg_storage;

					s += 3; a = s; continue;
				}

				if(strncmp(s, "$sp", 3) == 0) {
					ctx.spacing = va_arg(va, int);
					s += 3; a = s; continue;
				}

				if(strncmp(s, "$an", 3) == 0) {
					ctx.angle = va_arg(va, double);
	
					float q = ctx.angle / (PI / 2.0f);
					int qi = (int)roundf(q);

					if(fabsf(fmodf(ctx.angle, PI/2)) < 0.0001f) {
						int dir_idx = (qi % 4 + 4) % 4;

						switch(dir_idx) {
							case 0: ctx.dir.x = 1.0f; ctx.dir.y = 0.0f; break;
							case 1: ctx.dir.x = 0.0f; ctx.dir.y = 1.0f; break;
							case 2: ctx.dir.x = -1.0f; ctx.dir.y = 0.0f; break;
							case 3: ctx.dir.x = 0.0f; ctx.dir.y = -1.0f; break;
						}
					} else {
						float fcos = cosf(ctx.angle);
						float fsin = sinf(ctx.angle);

						float mx = fmaxf(fabsf(fcos), fabsf(fsin));
						if(mx > 0.f) {
							ctx.dir.x = fcos / mx;
							ctx.dir.y = fsin / mx;
						} else {
							ctx.dir.x = fcos;
							ctx.dir.y = fsin;
						}
					}
	
					s += 3; a = s; continue;
				}
			}
			s++;
		}

		for(z=s; s[0]=='%' && s[1]=='%'; z++, s+=2);
		if(z-a > INT_MAX-cnt) goto format_error;
		l = z-a;
		draw_out(&ctx, a, l);
		if(l) continue;

		s++; 

		for(fl=0; (unsigned)*s-' '<32 && (FLAGMASK&(1U<<(*s-' '))); s++)
			fl |= 1U<<(*s-' ');

		if(*s=='*') {
			w = va_arg(va, int);
			s++;
			if(w<0) { fl|=LEFT_ADJ; w=-w; }
		} else if((w=getint(&s))<0) goto format_error;

		if(*s=='.' && s[1]=='*') {
			p = va_arg(va, int);
			s+=2;
			xp = (p>=0);
		} else if(*s=='.') {
			s++;
			p = getint(&s);
			xp = 1;
		} else {
			p = -1; xp = 0;
		}

		st=0;
		do {
			if(OOB(*s)) goto format_error;
			ps=st;
			st = states[st][(unsigned char)(*s++) - 'A'];
		} while(st-1<STOP);
		
		if(!st) goto format_error;
		
		if(st != NOARG) {
			switch(st) {
			case PTR:	arg.p = va_arg(va, void*);			break;
			case INT:	arg.i = va_arg(va, int);			  break;
			case UINT:   arg.i = va_arg(va, unsigned int);	 break;
			case LONG:   arg.i = va_arg(va, long);			 break;
			case ULONG:  arg.i = va_arg(va, unsigned long);	break;
			case ULLONG: arg.i = va_arg(va, unsigned long long);break;
			case SHORT:  arg.i = (short)va_arg(va, int);	   break;
			case USHORT: arg.i = (unsigned short)va_arg(va, int);break;
			case CHAR:   arg.i = (signed char)va_arg(va, int); break;
			case UCHAR:  arg.i = (unsigned char)va_arg(va, int);break;
			case LLONG:  arg.i = va_arg(va, long long);		break;
			case SIZET:  arg.i = va_arg(va, size_t);		   break;
			case IMAX:   arg.i = va_arg(va, intmax_t);		 break;
			case UMAX:   arg.i = va_arg(va, uintmax_t);		break;
			case PDIFF:  arg.i = va_arg(va, ptrdiff_t);		break;
			case UIPTR:  arg.i = (uintptr_t)va_arg(va, void*); break;
			case DBL:	arg.f = va_arg(va, double);		   break;
			case LDBL:   arg.f = va_arg(va, long double);	  break;
			}
		}

		z = buf + sizeof(buf);
		prefix = "-+   0X0x";
		pl = 0;
		t = s[-1];
		if(ps && (t&15)==3) t&=~32;
		if(fl & LEFT_ADJ) fl &= ~ZERO_PAD;

		switch(t) {
		case 'n':
			switch(ps) {
			case BARE:   *(int*)arg.p		  = cnt; break;
			case LPRE:   *(long*)arg.p		 = cnt; break;
			case LLPRE:  *(long long*)arg.p	 = cnt; break;
			case HPRE:   *(unsigned short*)arg.p= cnt; break;
			case HHPRE:  *(unsigned char*)arg.p = cnt; break;
			case ZTPRE:  *(size_t*)arg.p	   = cnt; break;
			case JPRE:   *(uintmax_t*)arg.p	 = cnt; break;
			}
			continue;
		case 'p':
			p = MAX(p, 2*(int)sizeof(void*));
			t = 'x';
			fl |= ALT_FORM;
			// NOTE: I didn't know you could do that

			/* fallthrough */
		case 'x': case 'X':
			a = fmt_x(arg.i, z, t&32);
			if(arg.i && (fl & ALT_FORM)) prefix+=(t>>4), pl=2;
			goto ifmt_tail;
		case 'o':
			a = fmt_o(arg.i, z);
			if((fl&ALT_FORM) && p<z-a+1) p=z-a+1;
			goto ifmt_tail;
		case 'd': case 'i':
			pl=1;
			if(arg.i>INTMAX_MAX) { arg.i=-arg.i; }
			else if(fl & MARK_POS) { prefix++; }
			else if(fl & PAD_POS)  { prefix+=2; }
			else pl=0;
			/* fallthrough */
		case 'u':
			a = fmt_u(arg.i, z);
		ifmt_tail:
			if(xp && p<0) goto format_error;
			if(xp) fl &= ~ZERO_PAD;
			if(!arg.i && !p) { a=z; break; }
			p = MAX(p, z-a + !arg.i);
			break;
		narrow_c:
		case 'c':
			*(a=z-(p=1)) = arg.i;
			fl &= ~ZERO_PAD;
			break;
		case 'm':
			a = (char *)strerror(errno);
			z = a + strnlen(a, p < 0 ? (size_t)INT_MAX : (size_t)p);
			if (p < 0 && *z) goto format_error;
			p = (int)(z - a);
			fl &= ~ZERO_PAD;
			break;
		case 's':
			a = (char *)(arg.p ? arg.p : "(null)");
			z = a + strnlen(a, p < 0 ? (size_t)INT_MAX : (size_t)p);
			if (p < 0 && *z) goto format_error;
			p = (int)(z - a);
			fl &= ~ZERO_PAD;
			break;
		case 'C':
			if(!arg.i) goto narrow_c;
			wc[0] = arg.i; wc[1] = 0;
			arg.p = wc; p = -1;
			/* fallthrough */
		case 'S':
			ws = arg.p;
			for(loop_i=0, l=0; (size_t)loop_i<(size_t)p && *ws && (l=wctomb(mb, *ws++))>=0 && l<=(int)(p-loop_i); loop_i+=l);
			if(l<0) return;
			p = (int)loop_i;
			draw_pad(&ctx, ' ', w, p, fl);
			ws = arg.p;
			for(loop_i=0; (size_t)loop_i<(size_t)p && *ws && (size_t)(loop_i+(l=wctomb(mb,*ws++)))<=(size_t)p; loop_i+=l)
				draw_out(&ctx, mb, l);
			draw_pad(&ctx, ' ', w, p, fl^LEFT_ADJ);
			l = w>p ? w : p;
			continue;
		case 'e': case 'f': case 'g': case 'a':
		case 'E': case 'F': case 'G': case 'A':
			if(xp && p<0) goto format_error;
			l = fmt_fp(&ctx, arg.f, w, p, fl, t, ps);
			if(l<0) goto format_error;
			continue;
		}

		if(p < z-a) p = z-a;
		if(p > INT_MAX-pl) goto format_error;
		if(w < pl+p) w = pl+p;
		if(w > INT_MAX-cnt) goto format_error;

		draw_pad(&ctx, ' ', w, pl+p, fl);
		draw_out(&ctx, prefix, pl);
		draw_pad(&ctx, '0', w, pl+p, fl^ZERO_PAD);
		draw_pad(&ctx, '0', p, z-a, 0);
		draw_out(&ctx, a, z-a);
		draw_pad(&ctx, ' ', w, pl+p, fl^LEFT_ADJ);
		l = w;
	}
	
	return;

format_error:
	errno = EINVAL; 
	return;
}
