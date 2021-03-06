/* chardefs.h -- Character definitions for readline. */

/* Copyright (C) 1994-2009 Free Software Foundation, Inc.

   This file is part of the GNU Readline Library (Readline), a library
   for reading lines of text with interactive input and history editing.

   Readline is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Readline is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Readline.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _CHARDEFS_H_
#define _CHARDEFS_H_

#include <ctype.h>

#if defined (HAVE_CONFIG_H)
#  if defined (HAVE_STRING_H)
#    if ! defined (STDC_HEADERS) && defined (HAVE_MEMORY_H)
#      include <memory.h>
#    endif
#    include <string.h>
#  endif /* HAVE_STRING_H */
#  if defined (HAVE_STRINGS_H)
#    include <strings.h>
#  endif /* HAVE_STRINGS_H */
#else
#  include <string.h>
#endif /* !HAVE_CONFIG_H */

#ifdef __MVS__
extern char E2Atab[256];     /* ebcdic  to ascii   table */
extern char A2Etab[256];     /* ascii   to ebcdic  table */
extern char E2Ctab[256];     /* ebcdic  to control table */
extern char C2Etab[256];     /* control to ebcdic  table */
#define E2A(c) (E2Atab[(c)])
#define A2E(c) (A2Etab[(c)])
#else
#define E2A(c) (c)
#define A2E(c) (c)
#endif

#ifndef whitespace
#define whitespace(c) (((c) == ' ') || ((c) == '\t'))
#endif

#ifdef CTRL
#  undef CTRL
#endif
#ifdef UNCTRL
#  undef UNCTRL
#endif

/* Some character stuff. */
#define control_character_threshold 0x020   /* Smaller than this is control. */
#define control_character_mask 0x1f	    /* 0x20 - 1 */
#define meta_character_threshold 0x07f	    /* Larger than this is Meta. */
#define control_character_bit 0x40	    /* 0x000000, must be off. */
#define meta_character_bit 0x080	    /* x0000000, must be on. */
#define largest_char 255		    /* Largest character value. */

#ifdef __MVS__
#define CTRL_CHAR(c) (E2A(c) < control_character_threshold && ((E2A(c) & 0x80) == 0))
#define META_CHAR(c) (E2A(c) > meta_character_threshold && E2A(c) <= largest_char)

#define CTRL(c) A2E(E2A(c) & control_character_mask) 
#define META(c) A2E(E2A(c) | meta_character_bit)

#define UNMETA(c) A2E(E2A(c) & (~meta_character_bit))
#define UNCTRL(c) _rl_to_upper(A2E(E2A(c)|control_character_bit))
#else
#define CTRL_CHAR(c) ((c) < control_character_threshold && (((c) & 0x80) == 0))
#define META_CHAR(c) ((c) > meta_character_threshold && (c) <= largest_char)

#define CTRL(c) ((c) & control_character_mask)
#define META(c) ((c) | meta_character_bit)

#define UNMETA(c) ((c) & (~meta_character_bit))
#define UNCTRL(c) _rl_to_upper(((c)|control_character_bit))
#endif

#ifdef __MVS__
#define DEL     0x07
#define CTRL_A  0x01
#define CTRL_B  0x02
#define CTRL_C  0x03
#define CTRL_D  0x37
#define CTRL_E  0x2d
#define CTRL_F  0x2e
#define CTRL_G  0x2f
#define CTRL_H  0x16
#define CTRL_I  0x05
#define CTRL_J  0x15
#define CTRL_K  0x0b
#define CTRL_L  0x0c
#define CTRL_M  0x0d
#define CTRL_N  0x0e
#define CTRL_O  0x0f
#define CTRL_P  0x10
#define CTRL_Q  0x11
#define CTRL_R  0x12
#define CTRL_S  0x13
#define CTRL_T  0x3c
#define CTRL_U  0x3d
#define CTRL_V  0x32
#define CTRL_W  0x26
#define CTRL_X  0x18
#define CTRL_Y  0x19
#define CTRL_Z  0x3f
#define CTRL_LB 0x27
#endif


#if defined STDC_HEADERS || (!defined (isascii) && !defined (HAVE_ISASCII)) || defined(__MVS__)
#  define IN_CTYPE_DOMAIN(c) 1
#else
#  define IN_CTYPE_DOMAIN(c) isascii(c)
#endif

#if !defined (isxdigit) && !defined (HAVE_ISXDIGIT)
#  define isxdigit(c)   (isdigit((c)) || ((c) >= 'a' && (c) <= 'f') || ((c) >= 'A' && (c) <= 'F'))
#endif

#if defined (CTYPE_NON_ASCII)
#  define NON_NEGATIVE(c) 1
#else
#  define NON_NEGATIVE(c) ((unsigned char)(c) == (c))
#endif

/* Some systems define these; we want our definitions. */
#undef ISPRINT

/* Beware:  these only work with single-byte ASCII characters. */

#define ISALNUM(c)	(IN_CTYPE_DOMAIN (c) && isalnum (c))
#define ISALPHA(c)	(IN_CTYPE_DOMAIN (c) && isalpha (c))
#define ISDIGIT(c)	(IN_CTYPE_DOMAIN (c) && isdigit (c))
#define ISLOWER(c)	(IN_CTYPE_DOMAIN (c) && islower (c))
#define ISPRINT(c)	(IN_CTYPE_DOMAIN (c) && isprint (c))
#define ISUPPER(c)	(IN_CTYPE_DOMAIN (c) && isupper (c))
#define ISXDIGIT(c)	(IN_CTYPE_DOMAIN (c) && isxdigit (c))

#define _rl_lowercase_p(c)	(NON_NEGATIVE(c) && ISLOWER(c))
#define _rl_uppercase_p(c)	(NON_NEGATIVE(c) && ISUPPER(c))
#define _rl_digit_p(c)		((c) >= '0' && (c) <= '9')

#define _rl_pure_alphabetic(c)	(NON_NEGATIVE(c) && ISALPHA(c))
#define ALPHABETIC(c)		(NON_NEGATIVE(c) && ISALNUM(c))

#ifndef _rl_to_upper
#  define _rl_to_upper(c) (_rl_lowercase_p(c) ? toupper((unsigned char)c) : (c))
#  define _rl_to_lower(c) (_rl_uppercase_p(c) ? tolower((unsigned char)c) : (c))
#endif

#ifndef _rl_digit_value
#  define _rl_digit_value(x) ((x) - '0')
#endif

#ifndef _rl_isident
#  define _rl_isident(c) (ISALNUM(c) || (c) == '_')
#endif

#ifndef ISOCTAL
#  define ISOCTAL(c)	((c) >= '0' && (c) <= '7')
#endif
#define OCTVALUE(c)	((c) - '0')

#define HEXVALUE(c) \
  (((c) >= 'a' && (c) <= 'f') \
  	? (c)-'a'+10 \
  	: (c) >= 'A' && (c) <= 'F' ? (c)-'A'+10 : (c)-'0')

#ifndef NEWLINE
#define NEWLINE '\n'
#endif

#ifndef RETURN
#define RETURN CTRL_M
#endif

#ifndef RUBOUT
#define RUBOUT DEL
#endif

#ifndef TAB
#define TAB '\t'
#endif

#ifdef ABORT_CHAR
#undef ABORT_CHAR
#endif
#define ABORT_CHAR CTRL_G

#ifdef PAGE
#undef PAGE
#endif
#define PAGE CTRL_L

#ifdef SPACE
#undef SPACE
#endif
#define SPACE ' '	/* XXX - was 0x20 */

#ifdef ESC
#undef ESC
#endif
#define ESC CTRL_LB

#endif  /* _CHARDEFS_H_ */
