/* yacc -d ./parse.y */
 
/****PROPRIETARY_STATEMENT********************************************/
/*                                                                   */
/*                                                                   */
/* LICENSED MATERIALS - PROPERTY OF IBM                              */
/* "RESTRICTED MATERIALS OF IBM"                                     */
/* 5694-A01 (C) COPYRIGHT IBM CORP. 1993, 2003                       */
/*                                                                   */
/* STATUS= HOT7709                                                   */
/*                                                                   */
/****END_OF_PROPRIETARY_STATEMENT*************************************/
/*
*01* EXTERNAL CLASSIFICATION:  PI
*01* END OF EXTERNAL CLASSIFICATION:
*/
/*
 * Portable way of defining ANSI C prototypes
 */
#ifndef YY_ARGS
   #ifdef __STDC__
     #define YY_ARGS(x)     x
   #else
     #define YY_ARGS(x)     ()
   #endif
 #endif
 
#ifdef YYTRACE
 #define YYDEBUG 1
#else
 #ifndef YYDEBUG
  #define YYDEBUG 0
 #endif
#endif
 
#if YYDEBUG
typedef struct yyNamedType_tag   /* Tokens */
 {
  char  * name;                  /* printable name */
  short token;                   /* token # */
  short type;                    /* token type */
 } yyNamedType;
 
typedef struct yyTypedRules_tag  /* Typed rule table */
 {
  char  * name;                  /* compressed rule string */
  short type;                    /* rule result type */
 } yyTypedRules;
 
#endif
 
#line 21 "./parse.y"

#include "config.h"

#include "bashtypes.h"
#include "bashansi.h"

#include "filecntl.h"

#if defined (HAVE_UNISTD_H)
#  include <unistd.h>
#endif

#if defined (HAVE_LOCALE_H)
#  include <locale.h>
#endif

#include <stdio.h>
#include "chartypes.h"
#include <signal.h>

#include "memalloc.h"

#include "bashintl.h"

#define NEED_STRFTIME_DECL	/* used in externs.h */

#include "shell.h"
#include "trap.h"
#include "flags.h"
#include "parser.h"
#include "mailcheck.h"
#include "test.h"
#include "builtins.h"
#include "builtins/common.h"
#include "builtins/builtext.h"

#include "shmbutil.h"

#if defined (READLINE)
#  include "bashline.h"
#  include <readline/readline.h>
#endif /* READLINE */

#if defined (HISTORY)
#  include "bashhist.h"
#  include <readline/history.h>
#endif /* HISTORY */

#if defined (JOB_CONTROL)
#  include "jobs.h"
#endif /* JOB_CONTROL */

#if defined (ALIAS)
#  include "alias.h"
#else
typedef void *alias_t;
#endif /* ALIAS */

#if defined (PROMPT_STRING_DECODE)
#  ifndef _MINIX
#    include <sys/param.h>
#  endif
#  include <time.h>
#  if defined (TM_IN_SYS_TIME)
#    include <sys/types.h>
#    include <sys/time.h>
#  endif /* TM_IN_SYS_TIME */
#  include "maxpath.h"
#endif /* PROMPT_STRING_DECODE */

#define RE_READ_TOKEN	-99
#define NO_EXPANSION	-100

#ifdef DEBUG
#  define YYDEBUG 1
#else
#  define YYDEBUG 0
#endif

#if defined (HANDLE_MULTIBYTE)
#  define last_shell_getc_is_singlebyte \
	((shell_input_line_index > 1) \
		? shell_input_line_property[shell_input_line_index - 1] \
		: 1)
#  define MBTEST(x)	((x) && last_shell_getc_is_singlebyte)
#else
#  define last_shell_getc_is_singlebyte	1
#  define MBTEST(x)	((x))
#endif

#if defined (EXTENDED_GLOB)
extern int extended_glob;
#endif

extern int eof_encountered;
extern int no_line_editing, running_under_emacs;
extern int current_command_number;
extern int sourcelevel, parse_and_execute_level;
extern int posixly_correct;
extern int last_command_exit_value;
extern pid_t last_command_subst_pid;
extern char *shell_name, *current_host_name;
extern char *dist_version;
extern int patch_level;
extern int dump_translatable_strings, dump_po_strings;
extern sh_builtin_func_t *last_shell_builtin, *this_shell_builtin;
#if defined (BUFFERED_INPUT)
extern int bash_input_fd_changed;
#endif

extern int errno;
/* **************************************************************** */
/*								    */
/*		    "Forward" declarations			    */
/*								    */
/* **************************************************************** */

#ifdef DEBUG
static void debug_parser __P((int));
#endif

static int yy_getc __P((void));
static int yy_ungetc __P((int));

#if defined (READLINE)
static int yy_readline_get __P((void));
static int yy_readline_unget __P((int));
#endif

static int yy_string_get __P((void));
static int yy_string_unget __P((int));
static void rewind_input_string __P((void));
static int yy_stream_get __P((void));
static int yy_stream_unget __P((int));

static int shell_getc __P((int));
static void shell_ungetc __P((int));
static void discard_until __P((int));

#if defined (ALIAS) || defined (DPAREN_ARITHMETIC)
static void push_string __P((char *, int, alias_t *));
static void pop_string __P((void));
static void free_string_list __P((void));
#endif

static char *read_a_line __P((int));

static int reserved_word_acceptable __P((int));
static int yylex __P((void));

static void push_heredoc __P((REDIRECT *));
static char *mk_alexpansion __P((char *));
static int alias_expand_token __P((char *));
static int time_command_acceptable __P((void));
static int special_case_tokens __P((char *));
static int read_token __P((int));
static char *parse_matched_pair __P((int, int, int, int *, int));
static char *parse_comsub __P((int, int, int, int *, int));
#if defined (ARRAY_VARS)
static char *parse_compound_assignment __P((int *));
#endif
#if defined (DPAREN_ARITHMETIC) || defined (ARITH_FOR_COMMAND)
static int parse_dparen __P((int));
static int parse_arith_cmd __P((char **, int));
#endif
#if defined (COND_COMMAND)
static void cond_error __P((void));
static COND_COM *cond_expr __P((void));
static COND_COM *cond_or __P((void));
static COND_COM *cond_and __P((void));
static COND_COM *cond_term __P((void));
static int cond_skip_newlines __P((void));
static COMMAND *parse_cond_command __P((void));
#endif
#if defined (ARRAY_VARS)
static int token_is_assignment __P((char *, int));
static int token_is_ident __P((char *, int));
#endif
static int read_token_word __P((int));
static void discard_parser_constructs __P((int));

static char *error_token_from_token __P((int));
static char *error_token_from_text __P((void));
static void print_offending_line __P((void));
static void report_syntax_error __P((char *));

static void handle_eof_input_unit __P((void));
static void prompt_again __P((void));
#if 0
static void reset_readline_prompt __P((void));
#endif
static void print_prompt __P((void));

#if defined (HANDLE_MULTIBYTE)
static void set_line_mbstate __P((void));
static char *shell_input_line_property = NULL;
#else
#  define set_line_mbstate()
#endif

extern int yyerror __P((const char *));

#ifdef DEBUG
extern int yydebug;
#endif

/* Default prompt strings */
char *primary_prompt = PPROMPT;
char *secondary_prompt = SPROMPT;

/* PROMPT_STRING_POINTER points to one of these, never to an actual string. */
char *ps1_prompt, *ps2_prompt;

/* Handle on the current prompt string.  Indirectly points through
   ps1_ or ps2_prompt. */
char **prompt_string_pointer = (char **)NULL;
char *current_prompt_string;

/* Non-zero means we expand aliases in commands. */
int expand_aliases = 0;

/* If non-zero, the decoded prompt string undergoes parameter and
   variable substitution, command substitution, arithmetic substitution,
   string expansion, process substitution, and quote removal in
   decode_prompt_string. */
int promptvars = 1;

/* If non-zero, $'...' and $"..." are expanded when they appear within
   a ${...} expansion, even when the expansion appears within double
   quotes. */
int extended_quote = 1;

/* The number of lines read from input while creating the current command. */
int current_command_line_count;

/* The number of lines in a command saved while we run parse_and_execute */
int saved_command_line_count;

/* The token that currently denotes the end of parse. */
int shell_eof_token;

/* The token currently being read. */
int current_token;

/* The current parser state. */
int parser_state;

/* Variables to manage the task of reading here documents, because we need to
   defer the reading until after a complete command has been collected. */
#define HEREDOC_MAX 16

static REDIRECT *redir_stack[HEREDOC_MAX];
int need_here_doc;

/* Where shell input comes from.  History expansion is performed on each
   line when the shell is interactive. */
static char *shell_input_line = (char *)NULL;
static int shell_input_line_index;
static int shell_input_line_size;	/* Amount allocated for shell_input_line. */
static int shell_input_line_len;	/* strlen (shell_input_line) */

/* Either zero or EOF. */
static int shell_input_line_terminator;

/* The line number in a script on which a function definition starts. */
static int function_dstart;

/* The line number in a script on which a function body starts. */
static int function_bstart;

/* The line number in a script at which an arithmetic for command starts. */
static int arith_for_lineno;

/* The decoded prompt string.  Used if READLINE is not defined or if
   editing is turned off.  Analogous to current_readline_prompt. */
static char *current_decoded_prompt;

/* The last read token, or NULL.  read_token () uses this for context
   checking. */
static int last_read_token;

/* The token read prior to last_read_token. */
static int token_before_that;

/* The token read prior to token_before_that. */
static int two_tokens_ago;

static int global_extglob;

/* The line number in a script where the word in a `case WORD', `select WORD'
   or `for WORD' begins.  This is a nested command maximum, since the array
   index is decremented after a case, select, or for command is parsed. */
#define MAX_CASE_NEST	128
static int word_lineno[MAX_CASE_NEST+1];
static int word_top = -1;

/* If non-zero, it is the token that we want read_token to return
   regardless of what text is (or isn't) present to be read.  This
   is reset by read_token.  If token_to_read == WORD or
   ASSIGNMENT_WORD, yylval.word should be set to word_desc_to_read. */
static int token_to_read;
static WORD_DESC *word_desc_to_read;

static REDIRECTEE source;
static REDIRECTEE redir;
typedef union {
  WORD_DESC *word;		/* the word that we read. */
  int number;			/* the number that we read. */
  WORD_LIST *word_list;
  COMMAND *command;
  REDIRECT *redirect;
  ELEMENT element;
  PATTERN_LIST *pattern;
} YYSTYPE;
#define IF	257
#define THEN	258
#define ELSE	259
#define ELIF	260
#define FI	261
#define CASE	262
#define ESAC	263
#define FOR	264
#define SELECT	265
#define WHILE	266
#define UNTIL	267
#define DO	268
#define DONE	269
#define FUNCTION	270
#define COPROC	271
#define COND_START	272
#define COND_END	273
#define COND_ERROR	274
#define IN	275
#define BANG	276
#define TIME	277
#define TIMEOPT	278
#define TIMEIGN	279
#define WORD	280
#define ASSIGNMENT_WORD	281
#define REDIR_WORD	282
#define NUMBER	283
#define ARITH_CMD	284
#define ARITH_FOR_EXPRS	285
#define COND_CMD	286
#define AND_AND	287
#define OR_OR	288
#define GREATER_GREATER	289
#define LESS_LESS	290
#define LESS_AND	291
#define LESS_LESS_LESS	292
#define GREATER_AND	293
#define SEMI_SEMI	294
#define SEMI_AND	295
#define SEMI_SEMI_AND	296
#define LESS_LESS_MINUS	297
#define AND_GREATER	298
#define AND_GREATER_GREATER	299
#define LESS_GREATER	300
#define GREATER_BAR	301
#define BAR_AND	302
#define yacc_EOF	303
extern int yychar, yyerrflag;
extern YYSTYPE yylval;
#if YYDEBUG
yyTypedRules yyRules[] = {
	{ "&00: %01 &00",  0},
	{ "%01: %09 %36",  4},
	{ "%01: &50",  4},
	{ "%01: &01 &50",  4},
	{ "%01: &51",  4},
	{ "%29: &25",  3},
	{ "%29: %29 &25",  3},
	{ "%26: &53 &25",  5},
	{ "%26: &54 &25",  5},
	{ "%26: &28 &53 &25",  5},
	{ "%26: &28 &54 &25",  5},
	{ "%26: &27 &53 &25",  5},
	{ "%26: &27 &54 &25",  5},
	{ "%26: &34 &25",  5},
	{ "%26: &28 &34 &25",  5},
	{ "%26: &27 &34 &25",  5},
	{ "%26: &46 &25",  5},
	{ "%26: &28 &46 &25",  5},
	{ "%26: &27 &46 &25",  5},
	{ "%26: &45 &25",  5},
	{ "%26: &28 &45 &25",  5},
	{ "%26: &27 &45 &25",  5},
	{ "%26: &35 &25",  5},
	{ "%26: &28 &35 &25",  5},
	{ "%26: &27 &35 &25",  5},
	{ "%26: &42 &25",  5},
	{ "%26: &28 &42 &25",  5},
	{ "%26: &27 &42 &25",  5},
	{ "%26: &37 &25",  5},
	{ "%26: &28 &37 &25",  5},
	{ "%26: &27 &37 &25",  5},
	{ "%26: &36 &28",  5},
	{ "%26: &28 &36 &28",  5},
	{ "%26: &27 &36 &28",  5},
	{ "%26: &38 &28",  5},
	{ "%26: &28 &38 &28",  5},
	{ "%26: &27 &38 &28",  5},
	{ "%26: &36 &25",  5},
	{ "%26: &28 &36 &25",  5},
	{ "%26: &27 &36 &25",  5},
	{ "%26: &38 &25",  5},
	{ "%26: &28 &38 &25",  5},
	{ "%26: &27 &38 &25",  5},
	{ "%26: &38 &55",  5},
	{ "%26: &28 &38 &55",  5},
	{ "%26: &27 &38 &55",  5},
	{ "%26: &36 &55",  5},
	{ "%26: &28 &36 &55",  5},
	{ "%26: &27 &36 &55",  5},
	{ "%26: &43 &25",  5},
	{ "%26: &44 &25",  5},
	{ "%28: &25",  6},
	{ "%28: &26",  6},
	{ "%28: %26",  6},
	{ "%27: %26",  5},
	{ "%27: %27 %26",  5},
	{ "%11: %28",  4},
	{ "%11: %11 %28",  4},
	{ "%02: %11",  4},
	{ "%02: %12",  4},
	{ "%02: %12 %27",  4},
	{ "%02: %21",  4},
	{ "%02: %20",  4},
	{ "%12: %13",  4},
	{ "%12: %15",  4},
	{ "%12: &11 %08 &13 %08 &14",  4},
	{ "%12: &12 %08 &13 %08 &14",  4},
	{ "%12: %14",  4},
	{ "%12: %23",  4},
	{ "%12: %25",  4},
	{ "%12: %16",  4},
	{ "%12: %17",  4},
	{ "%12: %18",  4},
	{ "%12: %19",  4},
	{ "%13: &09 &25 %37 &13 %08 &14",  4},
	{ "%13: &09 &25 %37 &56 %08 &57",  4},
	{ "%13: &09 &25 &49 %37 &13 %08 &14",  4},
	{ "%13: &09 &25 &49 %37 &56 %08 &57",  4},
	{ "%13: &09 &25 %37 &20 %29 %35 %37 &13 %08 &14",  4},
	{ "%13: &09 &25 %37 &20 %29 %35 %37 &56 %08 &57",  4},
	{ "%13: &09 &25 %37 &20 %35 %37 &13 %08 &14",  4},
	{ "%13: &09 &25 %37 &20 %35 %37 &56 %08 &57",  4},
	{ "%19: &09 &30 %35 %37 &13 %08 &14",  4},
	{ "%19: &09 &30 %35 %37 &56 %08 &57",  4},
	{ "%19: &09 &30 &13 %08 &14",  4},
	{ "%19: &09 &30 &56 %08 &57",  4},
	{ "%14: &10 &25 %37 &13 %05 &14",  4},
	{ "%14: &10 &25 %37 &56 %05 &57",  4},
	{ "%14: &10 &25 &49 %37 &13 %05 &14",  4},
	{ "%14: &10 &25 &49 %37 &56 %05 &57",  4},
	{ "%14: &10 &25 %37 &20 %29 %35 %37 &13 %05 &14",  4},
	{ "%14: &10 &25 %37 &20 %29 %35 %37 &56 %05 &57",  4},
	{ "%15: &07 &25 %37 &20 %37 &08",  4},
	{ "%15: &07 &25 %37 &20 %32 %37 &08",  4},
	{ "%15: &07 &25 %37 &20 %33 &08",  4},
	{ "%21: &25 &58 &59 %37 %22",  4},
	{ "%21: &15 &25 &58 &59 %37 %22",  4},
	{ "%21: &15 &25 %37 %22",  4},
	{ "%22: %12",  4},
	{ "%22: %12 %27",  4},
	{ "%25: &58 %08 &59",  4},
	{ "%20: &16 %12",  4},
	{ "%20: &16 %12 %27",  4},
	{ "%20: &16 &25 %12",  4},
	{ "%20: &16 &25 %12 %27",  4},
	{ "%20: &16 %11",  4},
	{ "%23: &02 %08 &03 %08 &06",  4},
	{ "%23: &02 %08 &03 %08 &04 %08 &06",  4},
	{ "%23: &02 %08 &03 %08 %24 &06",  4},
	{ "%16: &56 %08 &57",  4},
	{ "%17: &29",  4},
	{ "%18: &17 &31 &18",  4},
	{ "%24: &05 %08 &03 %08",  4},
	{ "%24: &05 %08 &03 %08 &04 %08",  4},
	{ "%24: &05 %08 &03 %08 %24",  4},
	{ "%33: %31",  7},
	{ "%33: %32 %31",  7},
	{ "%31: %37 %30 &59 %08",  7},
	{ "%31: %37 %30 &59 %37",  7},
	{ "%31: %37 &58 %30 &59 %08",  7},
	{ "%31: %37 &58 %30 &59 %37",  7},
	{ "%32: %31 &39",  7},
	{ "%32: %32 %31 &39",  7},
	{ "%32: %31 &40",  7},
	{ "%32: %32 %31 &40",  7},
	{ "%32: %31 &41",  7},
	{ "%32: %32 %31 &41",  7},
	{ "%30: &25",  3},
	{ "%30: %30 &52 &25",  3},
	{ "%05: %37 %06",  4},
	{ "%08: %05",  4},
	{ "%08: %37 %07",  4},
	{ "%06: %07 &50 %37",  4},
	{ "%06: %07 &48 %37",  4},
	{ "%06: %07 &49 %37",  4},
	{ "%07: %07 &32 %37 %07",  4},
	{ "%07: %07 &33 %37 %07",  4},
	{ "%07: %07 &48 %37 %07",  4},
	{ "%07: %07 &49 %37 %07",  4},
	{ "%07: %07 &50 %37 %07",  4},
	{ "%07: %04",  4},
	{ "%36: &50",  0},
	{ "%36: &51",  0},
	{ "%35: &50",  2},
	{ "%35: &49",  2},
	{ "%35: &51",  2},
	{ "%37:",  0},
	{ "%37: %37 &50",  0},
	{ "%09: %10",  4},
	{ "%09: %10 &48",  4},
	{ "%09: %10 &49",  4},
	{ "%10: %10 &32 %37 %10",  4},
	{ "%10: %10 &33 %37 %10",  4},
	{ "%10: %10 &48 %10",  4},
	{ "%10: %10 &49 %10",  4},
	{ "%10: %04",  4},
	{ "%04: %03",  4},
	{ "%04: &21 %04",  4},
	{ "%04: %34 %04",  4},
	{ "%04: %34 %35",  4},
	{ "%04: &21 %35",  4},
	{ "%03: %03 &52 %37 %03",  4},
	{ "%03: %03 &47 %37 %03",  4},
	{ "%03: %02",  4},
	{ "%34: &22",  2},
	{ "%34: &22 &23",  2},
	{ "%34: &22 &23 &24",  2},
{ "$accept",  0},{ "error",  0}
};
yyNamedType yyTokenTypes[] = {
	{ "$end",  0,  0},
	{ "error",  256,  0},
	{ "IF",  257,  0},
	{ "THEN",  258,  0},
	{ "ELSE",  259,  0},
	{ "ELIF",  260,  0},
	{ "FI",  261,  0},
	{ "CASE",  262,  0},
	{ "ESAC",  263,  0},
	{ "FOR",  264,  0},
	{ "SELECT",  265,  0},
	{ "WHILE",  266,  0},
	{ "UNTIL",  267,  0},
	{ "DO",  268,  0},
	{ "DONE",  269,  0},
	{ "FUNCTION",  270,  0},
	{ "COPROC",  271,  0},
	{ "COND_START",  272,  0},
	{ "COND_END",  273,  0},
	{ "COND_ERROR",  274,  0},
	{ "IN",  275,  0},
	{ "BANG",  276,  0},
	{ "TIME",  277,  0},
	{ "TIMEOPT",  278,  0},
	{ "TIMEIGN",  279,  0},
	{ "WORD",  280,  1},
	{ "ASSIGNMENT_WORD",  281,  1},
	{ "REDIR_WORD",  282,  1},
	{ "NUMBER",  283,  2},
	{ "ARITH_CMD",  284,  3},
	{ "ARITH_FOR_EXPRS",  285,  3},
	{ "COND_CMD",  286,  4},
	{ "AND_AND",  287,  0},
	{ "OR_OR",  288,  0},
	{ "GREATER_GREATER",  289,  0},
	{ "LESS_LESS",  290,  0},
	{ "LESS_AND",  291,  0},
	{ "LESS_LESS_LESS",  292,  0},
	{ "GREATER_AND",  293,  0},
	{ "SEMI_SEMI",  294,  0},
	{ "SEMI_AND",  295,  0},
	{ "SEMI_SEMI_AND",  296,  0},
	{ "LESS_LESS_MINUS",  297,  0},
	{ "AND_GREATER",  298,  0},
	{ "AND_GREATER_GREATER",  299,  0},
	{ "LESS_GREATER",  300,  0},
	{ "GREATER_BAR",  301,  0},
	{ "BAR_AND",  302,  0},
	{ "'&'",  80,  0},
	{ "';'",  94,  0},
	{ "'\\n'",  21,  0},
	{ "yacc_EOF",  303,  0},
	{ "'|'",  79,  0},
	{ "'>'",  110,  0},
	{ "'<'",  76,  0},
	{ "'-'",  96,  0},
	{ "'{'",  192,  0},
	{ "'}'",  208,  0},
	{ "'('",  77,  0},
	{ "')'",  93,  0}

};
#endif
static short yydef[] = {

	  30,   27,   22,  164,  164,  164,  164,    4,    3,  110, 
	  -1,   31,  164,  164,  164,  164,   24,   23,    8,   12, 
	 110,  164,  164,  164,  164,    5,   26,   25,   17,  164, 
	   9,   10,  164,  164,  164,  164,  164,  164,  164,  164, 
	  29,   28,  164,  164,  164,  164,  164,   11,    6,  164, 
	 164,  164,  164,  164,  164,  166,   18,  167,  164,  164, 
	   7,  168,  164,  164,  164,  164,  164,  164,  164,  164, 
	  20,   19,   21,   14,  164,  164,  164,  164,  164,  164, 
	 164,  164,  164,   13,  164,   15,  164,  164,  164,  164, 
	 164,   16
};
static short yyex[] = {

	   0,    0,   -1,    1
};
static short yyact[] = {

	-335, -186,   -5, -185, -340, -184,   -4, -201, -199, -200, 
	  -7,   -6, -202, -203, -204, -205,   -1,  -10, -285, -188, 
	-187, -237, -189, -192, -195, -194, -196, -193, -197, -198, 
	-191, -190, -333,  303,  301,  300,  299,  298,  297,  293, 
	 292,  291,  290,  289,  284,  283,  282,  281,  280,  277, 
	 276,  272,  271,  270,  267,  266,  265,  264,  262,  257, 
	 256,  192,  110,   77,   76,   21,  -12,  278, -219, -186, 
	  -5, -218, -185, -340,   -4, -201, -199, -200,   -7,   -6, 
	-202, -203, -204, -205,   -1,  -10, -285, -188, -187, -237, 
	-189, -192, -195, -194, -196, -193, -197, -198, -191, -190, 
	-217,  303,  301,  300,  299,  298,  297,  293,  292,  291, 
	 290,  289,  284,  283,  282,  281,  280,  277,  276,  272, 
	 271,  270,  267,  266,  265,  264,  262,  257,  192,  110, 
	  94,   77,   76,   21,  -14,  -13,  302,   79,  -18,  -17, 
	 -16,  -15,  288,  287,   94,   80, -182,  286, -186,   -5, 
	-185, -340,   -4, -201, -199, -200,   -7,   -6, -204,  -21, 
	-285, -188, -187, -237, -189, -192, -195, -194, -196, -193, 
	-197, -198, -191, -190,  301,  300,  299,  298,  297,  293, 
	 292,  291,  290,  289,  284,  283,  282,  281,  280,  272, 
	 267,  266,  265,  264,  262,  257,  192,  110,   77,   76, 
	 -22,  280,  -23,  280,  -24,  280,  -25, -177,  285,  280, 
	-186, -185, -188, -187, -189, -192, -195, -194, -196, -193, 
	-197, -198, -191, -190,  301,  300,  299,  298,  297,  293, 
	 292,  291,  290,  289,  283,  282,  110,   76, -186, -185, 
	-286, -285, -188, -187, -189, -192, -195, -194, -196, -193, 
	-197, -198, -191, -190,  301,  300,  299,  298,  297,  293, 
	 292,  291,  290,  289,  283,  282,  281,  280,  110,   76, 
	-174,   77, -287,  280, -288,  280, -294, -297, -303,  283, 
	 280,   96, -291, -300, -306,  283,  280,   96, -309,  280, 
	-312,  280, -315,  280, -318,  280, -321,  280, -324,  280, 
	-165, -164, -166, -169, -172, -171, -173, -170, -168, -167, 
	 301,  300,  297,  293,  292,  291,  290,  289,  110,   76, 
	-155, -154, -156, -159, -162, -161, -163, -160, -158, -157, 
	 301,  300,  297,  293,  292,  291,  290,  289,  110,   76, 
	-329,  280, -330,  280, -334,   21, -338, -339,  303,   21, 
	-208,  279, -186,   -5, -185, -340,   -4, -201, -199, -200, 
	  -7,   -6, -202, -203, -204, -205,   -1,  -10, -285, -188, 
	-187, -237, -189, -192, -195, -194, -196, -193, -197, -198, 
	-191, -190,  301,  300,  299,  298,  297,  293,  292,  291, 
	 290,  289,  284,  283,  282,  281,  280,  277,  276,  272, 
	 271,  270,  267,  266,  265,  264,  262,  257,  192,  110, 
	  77,   76, -236,  273, -341, -186,   -5, -185, -340,   -4, 
	-201, -199, -200,   -7,   -6, -202, -203, -204, -205,   -1, 
	 -10, -285, -188, -187, -237, -189, -192, -195, -194, -196, 
	-193, -197, -198, -191, -190,  301,  300,  299,  298,  297, 
	 293,  292,  291,  290,  289,  284,  283,  282,  281,  280, 
	 277,  276,  272,  271,  270,  267,  266,  265,  264,  262, 
	 257,  192,  110,   77,   76,   21, -238,  208,  -30,  258, 
	  -5, -340,   -4, -201, -199, -200,   -7,   -6, -204, -237, 
	 284,  272,  267,  266,  265,  264,  262,  257,  192,   77, 
	-242,   93, -148,   77,  -33,   94, -219, -218,  -34,  -35, 
	-217,  303,  268,  192,   94,   21,  -37,   94,  -38,  268, 
	 -39,  268,  -40,   93, -292, -295, -301,  283,  280,   96, 
	-289, -298, -304,  283,  280,   96, -307,  280, -310,  280, 
	-313,  280, -316,  280, -319,  280, -322,  280, -325,  280, 
	-326,  280, -293, -296, -302,  283,  280,   96, -290, -299, 
	-305,  283,  280,   96, -308,  280, -311,  280, -314,  280, 
	-317,  280, -320,  280, -323,  280, -327,  280, -328,  280, 
	-341, -186,   -5, -185, -340,   -4, -201, -199, -200,   -7, 
	  -6, -202, -203, -204,  -10, -285, -188, -187, -237, -189, 
	-192, -195, -194, -196, -193, -197, -198, -191, -190,  301, 
	 300,  299,  298,  297,  293,  292,  291,  290,  289,  284, 
	 283,  282,  281,  280,  272,  271,  270,  267,  266,  265, 
	 264,  262,  257,  192,  110,   77,   76,   21,  -16,  -15, 
	 288,  287,  -47,  -46,  -45,  -44,  -43,  288,  287,   94, 
	  80,   21, -341,   -5, -340,   -4, -201, -199, -200,   -7, 
	  -6, -204, -237,  284,  272,  267,  266,  265,  264,  262, 
	 257,  192,   77,   21,  -50,   93, -341,  -51,  275,   21, 
	-341,  -52,  -53, -140,  275,  268,  192,   21, -341,  -54, 
	 -55, -135,  275,  268,  192,   21,  -60,  -59, -241,  261, 
	 260,  259, -341,  -64,  -65,  268,  192,   21, -332,  280, 
	-255,  208, -256,  269, -341,  -66,  -67,  268,  192,   21, 
	-341,  -68,  -69,  268,  192,   21, -219, -218, -332, -217, 
	 303,  280,   94,   21, -274,  269, -275,  269, -239,  261, 
	-231, -229, -227,  296,  295,  294, -246,  263, -341, -115, 
	-248, -225,  280,  263,   77,   21, -219, -218, -331, -217, 
	 303,  280,   94,   21, -253,  208, -254,  269, -265,  208, 
	-266,  269,  -44,  -43,  288,  287,  -80,  258, -240,  261, 
	-230, -228, -226,  296,  295,  294, -341, -115, -247, -225, 
	 280,  263,   77,   21, -225,  280, -104,  -81,   93,   79, 
	-251,  208, -252,  269, -257,  208, -258,  269, -263,  208, 
	-264,  269, -341,  -82,  -83,  268,  192,   21, -104,  -85, 
	  93,   79, -224,  280, -341,  -87,  -88,  268,  192,   21, 
	-341,  -89,  -90,  268,  192,   21,  -91,  -59,  260,  259, 
	-259,  208, -260,  269, -249,  208, -250,  269, -261,  208, 
	-262,  269,   -1
};
static short yypact[] = {

	  67,  136,  142,  148,  200,  210,  210,  224,  254,  271, 
	 350,  351,  352,  352,  352,  352,  382,  382,  224,  254, 
	 490,  503,  504,  505,  517,  224,  640,  640,  647,  652, 
	 224,  224,  680,  688,  688,  688,  688,  696,  696,  696, 
	 136,  136,  696,  696,  696,  696,  696,  224,  224,  702, 
	 702,  710,  710,  734,  734,  445,  445,  445,  738,  740, 
	 224,  743,  748,  756,  756,  768,  768,  768,  768,  768, 
	 774,  774,  774,  783,  804,  818,  818,  818,  818,  818, 
	 824,  830,  830,  838,  840,  445,  840,  840,  844,  844, 
	 844,  445,  851,  849,  847,  845,  843,  841,  445,  445, 
	 445,  833,  827,  823,  820,  815,  811,  809,  807,  805, 
	 647,  803,  801,  798,  795,  790,  779,  777,  771,  769, 
	 760,  767,  765,  445,  760,  752,  747,  663,  739,  445, 
	 445,  663,  737,  735,  730,  723,  717,  713,  711,  709, 
	 705,  699,  382,  382,  692,  684,  678,  675,  663,  445, 
	 445,  609,  609,  579,  577,  575,  573,  571,  569,  567, 
	 565,  561,  555,  551,  549,  547,  545,  543,  541,  539, 
	 537,  533,  527,  523,  521,  519,  511,  501,  479,  477, 
	 445,  413,  348,  345,  343,  341,  330,  310,  299,  297, 
	 295,  293,  291,  289,  285,  279,  275,  273,  208,  205, 
	 203,  201,  174,  147,  101,  101,   33
};
static short yygo[] = {

	 -11, -209,  -42,  -41,   -2,  152,  151, -214, -214, -214, 
	-214, -214, -214, -213, -212, -214, -220,  206,  205,  204, 
	 150,  149,  143,  142,   17,   16, -123, -122, -113, -112, 
	 -96,  -95, -337,   87,   86,   64,   63,   52,   51, -223, 
	 -71,  -72,  -73,  -72,  -71,  -73, -111, -222, -221,  -29, 
	 130,  129,  123,  100,   99,   98,   57,   56,   55, -179, 
	-178, -176, -175, -142, -139, -138, -134, -133, -120, -119, 
	-118, -117, -110, -109, -108, -107,  -84, -233,  -98,  -97, 
	-232,  -94,  -93, -235, -180,   90,   89,   88,   84,   82, 
	  81,   80,   79,   68,   67,   66,   65,   59,   58,   54, 
	  53,   38,   37,   34,   33,   29,    6,    5,    4,    3, 
	-183,  -28,  -28, -216, -215,   -3,  -27,  206,  150,  149, 
	 142,   17,  -20,   -9,  202,  -32,  -49,  -49,  -49,  -19, 
	  -8,  202,  148,  131,  127,   20, -277, -273, -276, -270, 
	-269, -268, -267, -278, -279, -244, -245, -243,  131,  127, 
	-272, -234, -129,   83, -271, -283, -283, -282, -282, -283, 
	-282, -283, -282, -284,   60,   48,   47,   31,   30,   25, 
	  18,    7,  -31,  -48,  -61,  -26,   48,   31,   18, -280, 
	-280, -281,   19,    8, -121, -125,  134, -105, -114,  114, 
	 -74,  -62,   62,  -63, -127, -206,  -76,  -75,  -70,  -36, 
	-210, -211,  204,  176,  134,  124,  120, -336, -153, -152, 
	-151, -150, -149, -147, -146, -145, -141, -137, -136, -132, 
	-131, -130,  -56,  -57,  -58, -128, -126, -124, -124, -116, 
	-124, -124, -106, -103, -102, -101, -100,  -99,  -86,  -92, 
	-124, -124, -181,   87,   86,   84,   80,   78,   77,   76, 
	  75,   74,   69,   64,   63,   62,   52,   51,   50,   49, 
	  46,   45,   44,   43,   42,   39,   36,   35,   32,   24, 
	  23,   22,   21,   15,   14,   13,   12,   -1
};
static short yypgo[] = {

	   0,    0,    0,    1,    1,    1,  147,  147,  143,  143, 
	 143,  143,  143,  152,  194,  191,  191,   84,   39,   49, 
	  49,   49,  110,  110,  110,  116,  116,   16,    4,    4, 
	 195,  195,  195,    4,   16,   16,   16,   16,  116,  116, 
	 116,  201,  201,  201,   49,   49,   49,   32,  188,  188, 
	 193,  193,  193,  193,  193,  193,  191,  191,  152,  152, 
	 141,  140,  139,  150,  150,  150,  154,  144,  144,  144, 
	 138,  138,  138,  137,  137,  137,  137,  137,  137,  142, 
	 142,  142,  142,  136,  136,  136,  136,  136,  136,  136, 
	 136,  130,  130,  130,  130,  130,  130,  130,  130,  130, 
	 130,  130,    1,    1,  123,  123,  175,  175,  181,  181, 
	 181,  163,  163,  163,  163,  163,  163,  163,  163,  163, 
	 163,  163,  163,  163,  163,  163,  163,  163,  163,  163, 
	 163,  163,  163,  163,  163,  163,  163,  163,  163,  163, 
	 163,  163,  163,  163,  163,  163,  163,  163,  163,  163, 
	 163,  163,  163,  163,  163,  185,  185,    0,    0,    0, 
	   0,   84,  207,  207,  242,  242,   39,   39,  194,    0
};
static short yyrlen[] = {

	   0,    0,    0,    1,    1,    2,    1,    2,    2,    3, 
	   3,    4,    2,    4,    2,    4,    5,    2,    3,    4, 
	   4,    4,    1,    2,    2,    3,    3,    1,    4,    4, 
	   1,    2,    3,    1,    2,    2,    2,    2,    1,    4, 
	   4,    1,    1,    1,    1,    4,    4,    2,    3,    1, 
	   3,    2,    3,    2,    3,    2,    5,    4,    5,    6, 
	   3,    1,    3,    6,    7,    5,    3,    4,    6,    5, 
	   6,    7,    6,   10,   10,    7,    7,    6,    6,    5, 
	   5,    7,    7,    9,    9,   10,   10,    7,    7,    6, 
	   6,    1,    1,    1,    1,    1,    1,    1,    5,    5, 
	   1,    1,    1,    1,    2,    1,    2,    1,    1,    1, 
	   1,    2,    2,    3,    3,    2,    3,    3,    2,    3, 
	   3,    2,    3,    3,    2,    3,    3,    2,    3,    3, 
	   2,    3,    3,    2,    3,    3,    2,    3,    3,    2, 
	   3,    3,    2,    3,    3,    2,    3,    3,    2,    3, 
	   3,    3,    3,    2,    2,    2,    1,    1,    2,    1, 
	   2,    1,    1,    1,    0,    2,    3,    3,    1,    2
};
#define YYS0	206
#define YYDELTA	175
#define YYNPACT	207
#define YYNDEF	92

#define YYr167	0
#define YYr168	1
#define YYr169	2
#define YYr58	3
#define YYr59	4
#define YYr60	5
#define YYr98	6
#define YYr99	7
#define YYr101	8
#define YYr102	9
#define YYr103	10
#define YYr104	11
#define YYr105	12
#define YYr112	13
#define YYr116	14
#define YYr118	15
#define YYr120	16
#define YYr131	17
#define YYr133	18
#define YYr137	19
#define YYr138	20
#define YYr139	21
#define YYr148	22
#define YYr149	23
#define YYr150	24
#define YYr153	25
#define YYr154	26
#define YYr156	27
#define YYr161	28
#define YYr162	29
#define YYr164	30
#define YYr165	31
#define YYr166	32
#define YYr163	33
#define YYr160	34
#define YYr159	35
#define YYr158	36
#define YYr157	37
#define YYr155	38
#define YYr152	39
#define YYr151	40
#define YYr145	41
#define YYr144	42
#define YYr143	43
#define YYr140	44
#define YYr136	45
#define YYr135	46
#define YYr129	47
#define YYr128	48
#define YYr127	49
#define YYr126	50
#define YYr125	51
#define YYr124	52
#define YYr123	53
#define YYr122	54
#define YYr121	55
#define YYr119	56
#define YYr117	57
#define YYr114	58
#define YYr113	59
#define YYr111	60
#define YYr110	61
#define YYr109	62
#define YYr108	63
#define YYr107	64
#define YYr106	65
#define YYr100	66
#define YYr97	67
#define YYr96	68
#define YYr95	69
#define YYr94	70
#define YYr93	71
#define YYr92	72
#define YYr91	73
#define YYr90	74
#define YYr89	75
#define YYr88	76
#define YYr87	77
#define YYr86	78
#define YYr85	79
#define YYr84	80
#define YYr83	81
#define YYr82	82
#define YYr81	83
#define YYr80	84
#define YYr79	85
#define YYr78	86
#define YYr77	87
#define YYr76	88
#define YYr75	89
#define YYr74	90
#define YYr73	91
#define YYr72	92
#define YYr71	93
#define YYr70	94
#define YYr69	95
#define YYr68	96
#define YYr67	97
#define YYr66	98
#define YYr65	99
#define YYr64	100
#define YYr63	101
#define YYr62	102
#define YYr61	103
#define YYr57	104
#define YYr56	105
#define YYr55	106
#define YYr54	107
#define YYr53	108
#define YYr52	109
#define YYr51	110
#define YYr50	111
#define YYr49	112
#define YYr48	113
#define YYr47	114
#define YYr46	115
#define YYr45	116
#define YYr44	117
#define YYr43	118
#define YYr42	119
#define YYr41	120
#define YYr40	121
#define YYr39	122
#define YYr38	123
#define YYr37	124
#define YYr36	125
#define YYr35	126
#define YYr34	127
#define YYr33	128
#define YYr32	129
#define YYr31	130
#define YYr30	131
#define YYr29	132
#define YYr28	133
#define YYr27	134
#define YYr26	135
#define YYr25	136
#define YYr24	137
#define YYr23	138
#define YYr22	139
#define YYr21	140
#define YYr20	141
#define YYr19	142
#define YYr18	143
#define YYr17	144
#define YYr16	145
#define YYr15	146
#define YYr14	147
#define YYr13	148
#define YYr12	149
#define YYr11	150
#define YYr10	151
#define YYr9	152
#define YYr8	153
#define YYr7	154
#define YYr6	155
#define YYr5	156
#define YYr4	157
#define YYr3	158
#define YYr2	159
#define YYr1	160
#define YYrACCEPT	YYr167
#define YYrERROR	YYr168
#define YYrLR2	YYr169
#if YYDEBUG
char * yystoken[] = {

	"error",
	"IF",
	"THEN",
	"ELSE",
	"ELIF",
	"FI",
	"CASE",
	"ESAC",
	"FOR",
	"SELECT",
	"WHILE",
	"UNTIL",
	"DO",
	"DONE",
	"FUNCTION",
	"COPROC",
	"COND_START",
	"COND_END",
	"COND_ERROR",
	"IN",
	"BANG",
	"TIME",
	"TIMEOPT",
	"TIMEIGN",
	"WORD",
	"ASSIGNMENT_WORD",
	"REDIR_WORD",
	"NUMBER",
	"ARITH_CMD",
	"ARITH_FOR_EXPRS",
	"COND_CMD",
	"AND_AND",
	"OR_OR",
	"GREATER_GREATER",
	"LESS_LESS",
	"LESS_AND",
	"LESS_LESS_LESS",
	"GREATER_AND",
	"SEMI_SEMI",
	"SEMI_AND",
	"SEMI_SEMI_AND",
	"LESS_LESS_MINUS",
	"AND_GREATER",
	"AND_GREATER_GREATER",
	"LESS_GREATER",
	"GREATER_BAR",
	"BAR_AND",
	"yacc_EOF",
	0
};
char * yysvar[] = {
	"$accept",
	"inputunit",
	"command",
	"pipeline",
	"pipeline_command",
	"list",
	"list0",
	"list1",
	"compound_list",
	"simple_list",
	"simple_list1",
	"simple_command",
	"shell_command",
	"for_command",
	"select_command",
	"case_command",
	"group_command",
	"arith_command",
	"cond_command",
	"arith_for_command",
	"coproc",
	"function_def",
	"function_body",
	"if_command",
	"elif_clause",
	"subshell",
	"redirection",
	"redirection_list",
	"simple_command_element",
	"word_list",
	"pattern",
	"pattern_list",
	"case_clause_sequence",
	"case_clause",
	"timespec",
	"list_terminator",
	"simple_list_terminator",
	"newline_list",
	0
};
short yyrmap[] = {

	 167,  168,  169,   58,   59,   60,   98,   99,  101,  102, 
	 103,  104,  105,  112,  116,  118,  120,  131,  133,  137, 
	 138,  139,  148,  149,  150,  153,  154,  156,  161,  162, 
	 164,  165,  166,  163,  160,  159,  158,  157,  155,  152, 
	 151,  145,  144,  143,  140,  136,  135,  129,  128,  127, 
	 126,  125,  124,  123,  122,  121,  119,  117,  114,  113, 
	 111,  110,  109,  108,  107,  106,  100,   97,   96,   95, 
	  94,   93,   92,   91,   90,   89,   88,   87,   86,   85, 
	  84,   83,   82,   81,   80,   79,   78,   77,   76,   75, 
	  74,   73,   72,   71,   70,   69,   68,   67,   66,   65, 
	  64,   63,   62,   61,   57,   56,   55,   54,   53,   52, 
	  51,   50,   49,   48,   47,   46,   45,   44,   43,   42, 
	  41,   40,   39,   38,   37,   36,   35,   34,   33,   32, 
	  31,   30,   29,   28,   27,   26,   25,   24,   23,   22, 
	  21,   20,   19,   18,   17,   16,   15,   14,   13,   12, 
	  11,   10,    9,    8,    7,    6,    5,    4,    3,    2, 
	   1,  130,  141,  142,  146,  147,  134,  132,  115,    0
};
short yysmap[] = {

	   1,    5,    7,   11,   13,   25,   26,   31,   32,   36, 
	  55,   56,   64,   65,   66,   67,   68,   69,   75,   76, 
	  77,   79,   80,   81,   83,   86,  136,  137,  141,  144, 
	 145,  146,  151,  153,  154,  155,  156,  158,  159,  161, 
	 190,  191,  196,  197,  198,  199,  200,  202,  203,  205, 
	 206,  209,  210,  216,  217,  223,  224,  225,  226,  228, 
	 230,  232,  234,  236,  237,  245,  246,  247,  248,  249, 
	 258,  259,  260,  269,  277,  287,  290,  291,  292,  293, 
	 301,  309,  310,  315,  316,  318,  320,  321,  324,  325, 
	 327,  328,  335,  334,  331,  330,  323,  322,  314,  313, 
	 312,  311,  304,  300,  299,  286,  285,  284,  283,  282, 
	 279,  276,  275,  273,  272,  270,  263,  261,  252,  251, 
	 250,  242,  241,  240,  238,  235,  233,  231,  227,  222, 
	 221,  220,  219,  218,  215,  214,  213,  212,  211,  208, 
	 207,  201,  195,  194,  157,  152,  150,  149,  148,  135, 
	 134,  133,  132,  124,  123,  122,  121,  120,  119,  118, 
	 117,  116,  115,  114,  113,  112,  111,  110,  109,  108, 
	 107,  106,  105,   90,   85,   84,   82,   78,   74,   73, 
	  72,   70,   54,   52,   50,   49,   48,   47,   46,   45, 
	  44,   43,   42,   41,   40,   39,   38,   37,   17,   16, 
	  15,   14,   12,    8,    4,    3,    0,  131,    2,   62, 
	  57,   58,   63,    6,  192,  193,   59,   60,   61,  140, 
	 256,  257,  142,  317,  271,  295,  265,  296,  266,  297, 
	 267,  329,  319,  326,  336,  138,    9,  143,  262,  294, 
	 229,  147,  204,  264,  255,  268,  298,  274,  337,  338, 
	 302,  303,  280,  281,  243,  244,  305,  306,  332,  333, 
	 339,  340,  307,  308,  288,  289,   18,   19,   20,   21, 
	  22,   23,   24,  253,  254,   27,   28,   29,   30,   88, 
	  33,  160,   87,   34,   35,   89,   91,   92,  165,  179, 
	  96,  162,  176,   93,  163,  177,   94,  166,  180,   97, 
	 164,  178,   95,  167,  181,   98,  168,  182,   99,  169, 
	 183,  100,  170,  184,  101,  171,  185,  102,  172,  186, 
	 103,  173,  187,  104,  174,  175,  188,  189,  125,  126, 
	 278,  239,   51,  127,   53,  130,   71,  129,  128,   10,  139
};
int yyntoken = 60;
int yynvar = 38;
int yynstate = 341;
int yynrule = 170;
#endif

#if YYDEBUG
/*
 * Package up YACC context for tracing
 */
typedef struct yyTraceItems_tag
 {
  int   state, lookahead, errflag, done;
  int   rule, npop;
  short * states;
  long   nstates;
  YYSTYPE * values;
  long   nvalues;
  short * types;
 } yyTraceItems;
#endif
 
#line 64 "/etc/yyparse.c"
 
/*---------------------------------------------------------------------*/
/* Copyright 1985, 1990 by Mortice Kern Systems Inc.                   */
/* All rights reserved.                                                */
/*                                                                     */
/* Automaton to interpret LALR(1) tables.                              */
/*                                                                     */
/* Macros:                                                             */
/* yyclearin - clear the lookahead token.                              */
/* yyerrok - forgive a pending error                                   */
/* YYERROR - simulate an error                                         */
/* YYACCEPT - halt and return 0                                        */
/* YYABORT - halt and return 1                                         */
/* YYRETURN(value) - halt and return value.  You should use this       */
/*    instead of return(value).                                        */
/* YYREAD - ensure yychar contains a lookahead token by reading        */
/*    one if it does not.  See also YYSYNC.                            */
/* YYRECOVERING - 1 if syntax error detected and not recovered         */
/*    yet; otherwise, 0.                                               */
/*                                                                     */
/* Preprocessor flags:                                                 */
/* YYDEBUG - includes debug code if 1.  The parser will print          */
/*     a travelogue of the parse if this is defined as 1               */
/*     and yydebug is non-zero.                                        */
/*    yacc -t sets YYDEBUG to 1, but not yydebug.                      */
/* YYTRACE - No longer used but retained so as not to break existing   */
/*           code.  Previously it was defined as:                      */
/*                                                                     */
/*              turn on YYDEBUG, and undefine default trace            */
/*              functions so that the interactive functions in         */
/*              'ytrack.c' will be used.                               */
/*                                                                     */
/* YYSSIZE - size of state and value stacks (default 150).             */
/* YYSTATIC - By default, the state stack is an automatic array.       */
/*    If this is defined, the stack will be static.                    */
/*    In either case, the value stack is static.                       */
/* YYALLOC - Dynamically allocate both the state and value stacks      */
/*    by calling malloc() and free().                                  */
/* YYDYNAMIC - Dynamically allocate (and reallocate, if necessary)     */
/*    both the state and value stacks by calling malloc(),             */
/*    realloc(), and free().                                           */
/* YYSYNC - if defined, yacc guarantees to fetch a lookahead token     */
/*    before any action, even if it doesnt need it for a decision.     */
/*    If YYSYNC is defined, YYREAD will never be necessary unless      */
/*    the user explicitly sets yychar = -1                             */
/*                                                                     */
/* Copyright (c) 1983, by the University of Waterloo                   */
/*---------------------------------------------------------------------*/
/*
 * Prototypes
 */
extern int yylex YY_ARGS((void));
 
#if YYDEBUG
  #include <stdlib.h>     /* common prototypes */
  #include <string.h>
 
  extern char * yyValue           YY_ARGS((YYSTYPE, int)); /* print yylval */
  extern void   yyShowState       YY_ARGS((yyTraceItems *));
  extern void   yyShowReduce      YY_ARGS((yyTraceItems *));
  extern void   yyShowGoto        YY_ARGS((yyTraceItems *));
  extern void   yyShowShift       YY_ARGS((yyTraceItems *));
  extern void   yyShowErrRecovery YY_ARGS((yyTraceItems *));
  extern void   yyShowErrDiscard  YY_ARGS((yyTraceItems *));
  extern void   yyShowRead        YY_ARGS((int));
#endif
 
/*----------------------------------------------------------------------*
 * If YYDEBUG defined and yydebug set,                                  *
 * tracing functions will be called at appropriate times in yyparse()   *
 * Pass state of YACC parse, as filled into yyTraceItems yyx            *
 * If yyx.done is set by the tracing function, yyparse() will terminate *
 * with a return value of -1                                            *
 *----------------------------------------------------------------------*/
#define YY_TRACE(fn) { \
   yyx.state = yystate; yyx.lookahead = yychar; yyx.errflag =yyerrflag; \
   yyx.states = yys+1; yyx.nstates = yyps-yys; \
   yyx.values = yyv+1; yyx.nvalues = yypv-yyv; \
   yyx.types = yytypev+1; yyx.done = 0; \
   yyx.rule = yyi; yyx.npop = yyj; \
   fn(&yyx); \
   if (yyx.done) YYRETURN(-1); }
 
#ifndef I18N
  #define m_textmsg(id, str, cls)  (str)
#else /*I18N*/
  extern char* m_textmsg YY_ARGS((int id, const char* str, char* cls));
#endif/*I18N*/
 
#ifndef YYSSIZE
  #define YYSSIZE  150
#endif
 
#ifdef YYDYNAMIC
  #define YYALLOC
  char *getenv();
  int atoi();
  int yysinc = -1; /* stack size increment, <0 = double, 0 = none, >0 = fixed */
#endif
 
#ifdef YYALLOC
  int yyssize = YYSSIZE;
  #define YYRETURN(val)   { retval = (val); goto yyReturn; }
#else
  #define YYRETURN(val)   return(val);
#endif
 
#define YYERROR        goto yyerrlabel
#define yyerrok        yyerrflag = 0
#define YYACCEPT       YYRETURN(0)
#define YYABORT        YYRETURN(1)
#define YYRECOVERING() (yyerrflag != 0)
 
#if YYDEBUG
  #define yyclearin { if (yydebug) yyShowRead(-1); yychar = -1; }
  /* The if..else makes this macro behave exactly like a statement */
  #define YYREAD   if (yychar < 0) {       \
           if ((yychar = yylex()) < 0)   { \
              if (yychar == -2) YYABORT;   \
              yychar = 0;                  \
           }  /* endif */                  \
           if (yydebug)                    \
              yyShowRead(yychar);          \
        } else
#else
  #define yyclearin yychar = -1
  #define YYREAD   if (yychar < 0) {       \
           if ((yychar = yylex()) < 0) {   \
              if (yychar == -2) YYABORT;   \
              yychar = 0;                  \
           }  /* endif */                  \
        } else
#endif
 
#define YYERRCODE    256    /* value of `error' */
#define YYTOKEN_BASE 256
#define YYQYYP       yyq[yyq-yyp]
 
/*
 * Simulate bitwise negation as if was done on a two's complement machine.
 * This makes the generated code portable to machines with different
 * representations of integers (ie. signed magnitude).
 */
#define  yyneg(s) (-((s)+1))
 
YYSTYPE  yyval;         /* $ */
YYSTYPE  *yypvt;        /* $n */
YYSTYPE  yylval;        /* yylex() sets this */
 
int  yychar,            /* current token */
     yyerrflag,         /* error flag */
     yynerrs;           /* error count */
 
#if YYDEBUG
  int yydebug = 0;      /* debug if this flag is set */
 
  extern char *yysvar[];  /* table of non-terminals (aka 'variables') */
  extern yyNamedType yyTokenTypes[];  /* table of terminals & their types */
  extern short   yyrmap[], yysmap[];  /* map internal rule/states */
  extern int  yynstate, yynvar, yyntoken, yynrule;
 
  extern int  yyGetType YY_ARGS((int));  /* token type */
  extern char *yyptok YY_ARGS((int)); /* printable token string */
  extern int  yyExpandName YY_ARGS((int, int, char *, int));
                /* expand yyRules[] or yyStates[] */
  static char *  yygetState YY_ARGS((int));
 
  #define yyassert(condition, msg, arg) \
     if (!(condition)) { \
        printf(m_textmsg(2824, "\nyacc bug: ", "E")); \
        printf(msg, arg); \
        YYABORT; }
#else /* !YYDEBUG */
  #define yyassert(condition, msg, arg)
#endif
 
#line 1285 "./parse.y"
/* Initial size to allocate for tokens, and the
   amount to grow them by. */
#define TOKEN_DEFAULT_INITIAL_SIZE 496
#define TOKEN_DEFAULT_GROW_SIZE 512

/* Should we call prompt_again? */
#define SHOULD_PROMPT() \
  (interactive && (bash_input.type == st_stdin || bash_input.type == st_stream))

#if defined (ALIAS)
#  define expanding_alias() (pushed_string_list && pushed_string_list->expander)
#else
#  define expanding_alias() 0
#endif

/* Global var is non-zero when end of file has been reached. */
int EOF_Reached = 0;

#ifdef DEBUG
static void
debug_parser (i)
     int i;
{
#if YYDEBUG != 0
  yydebug = i;
#endif
}
#endif

/* yy_getc () returns the next available character from input or EOF.
   yy_ungetc (c) makes `c' the next character to read.
   init_yy_io (get, unget, type, location) makes the function GET the
   installed function for getting the next character, makes UNGET the
   installed function for un-getting a character, sets the type of stream
   (either string or file) from TYPE, and makes LOCATION point to where
   the input is coming from. */

/* Unconditionally returns end-of-file. */
int
return_EOF ()
{
  return (EOF);
}

/* Variable containing the current get and unget functions.
   See ./input.h for a clearer description. */
BASH_INPUT bash_input;

/* Set all of the fields in BASH_INPUT to NULL.  Free bash_input.name if it
   is non-null, avoiding a memory leak. */
void
initialize_bash_input ()
{
  bash_input.type = st_none;
  FREE (bash_input.name);
  bash_input.name = (char *)NULL;
  bash_input.location.file = (FILE *)NULL;
  bash_input.location.string = (char *)NULL;
  bash_input.getter = (sh_cget_func_t *)NULL;
  bash_input.ungetter = (sh_cunget_func_t *)NULL;
}

/* Set the contents of the current bash input stream from
   GET, UNGET, TYPE, NAME, and LOCATION. */
void
init_yy_io (get, unget, type, name, location)
     sh_cget_func_t *get;
     sh_cunget_func_t *unget;
     enum stream_type type;
     const char *name;
     INPUT_STREAM location;
{
  bash_input.type = type;
  FREE (bash_input.name);
  bash_input.name = name ? savestring (name) : (char *)NULL;

  /* XXX */
#if defined (CRAY)
  memcpy((char *)&bash_input.location.string, (char *)&location.string, sizeof(location));
#else
  bash_input.location = location;
#endif
  bash_input.getter = get;
  bash_input.ungetter = unget;
}

char *
yy_input_name ()
{
  return (bash_input.name ? bash_input.name : "stdin");
}

/* Call this to get the next character of input. */
static int
yy_getc ()
{
  return (*(bash_input.getter)) ();
}

/* Call this to unget C.  That is, to make C the next character
   to be read. */
static int
yy_ungetc (c)
     int c;
{
  return (*(bash_input.ungetter)) (c);
}

#if defined (BUFFERED_INPUT)
#ifdef INCLUDE_UNUSED
int
input_file_descriptor ()
{
  switch (bash_input.type)
    {
    case st_stream:
      return (fileno (bash_input.location.file));
    case st_bstream:
      return (bash_input.location.buffered_fd);
    case st_stdin:
    default:
      return (fileno (stdin));
    }
}
#endif
#endif /* BUFFERED_INPUT */

/* **************************************************************** */
/*								    */
/*		  Let input be read from readline ().		    */
/*								    */
/* **************************************************************** */

#if defined (READLINE)
char *current_readline_prompt = (char *)NULL;
char *current_readline_line = (char *)NULL;
int current_readline_line_index = 0;

static int
yy_readline_get ()
{
  SigHandler *old_sigint;
  int line_len;
  unsigned char c;

  if (!current_readline_line)
    {
      if (!bash_readline_initialized)
	initialize_readline ();

#if defined (JOB_CONTROL)
      if (job_control)
	give_terminal_to (shell_pgrp, 0);
#endif /* JOB_CONTROL */

      old_sigint = (SigHandler *)IMPOSSIBLE_TRAP_HANDLER;
      if (signal_is_ignored (SIGINT) == 0)
	{
	  interrupt_immediately++;
	  old_sigint = (SigHandler *)set_signal_handler (SIGINT, sigint_sighandler);
	}
      terminate_immediately = 1;

      current_readline_line = readline (current_readline_prompt ?
      					  current_readline_prompt : "");

      terminate_immediately = 0;
      if (signal_is_ignored (SIGINT) == 0)
	{
	  interrupt_immediately--;
	  if (old_sigint != IMPOSSIBLE_TRAP_HANDLER)
	    set_signal_handler (SIGINT, old_sigint);
	}

#if 0
      /* Reset the prompt to the decoded value of prompt_string_pointer. */
      reset_readline_prompt ();
#endif

      if (current_readline_line == 0)
	return (EOF);

      current_readline_line_index = 0;
      line_len = strlen (current_readline_line);

      current_readline_line = (char *)xrealloc (current_readline_line, 2 + line_len);
      current_readline_line[line_len++] = '\n';
      current_readline_line[line_len] = '\0';
    }

  if (current_readline_line[current_readline_line_index] == 0)
    {
      free (current_readline_line);
      current_readline_line = (char *)NULL;
      return (yy_readline_get ());
    }
  else
    {
      c = current_readline_line[current_readline_line_index++];
      return (c);
    }
}

static int
yy_readline_unget (c)
     int c;
{
  if (current_readline_line_index && current_readline_line)
    current_readline_line[--current_readline_line_index] = c;
  return (c);
}

void
with_input_from_stdin ()
{
  INPUT_STREAM location;

  if (bash_input.type != st_stdin && stream_on_stack (st_stdin) == 0)
    {
      location.string = current_readline_line;
      init_yy_io (yy_readline_get, yy_readline_unget,
		  st_stdin, "readline stdin", location);
    }
}

#else  /* !READLINE */

void
with_input_from_stdin ()
{
  with_input_from_stream (stdin, "stdin");
}
#endif	/* !READLINE */

/* **************************************************************** */
/*								    */
/*   Let input come from STRING.  STRING is zero terminated.	    */
/*								    */
/* **************************************************************** */

static int
yy_string_get ()
{
  register char *string;
  register unsigned char c;

  string = bash_input.location.string;

  /* If the string doesn't exist, or is empty, EOF found. */
  if (string && *string)
    {
      c = *string++;
      bash_input.location.string = string;
      return (c);
    }
  else
    return (EOF);
}

static int
yy_string_unget (c)
     int c;
{
  *(--bash_input.location.string) = c;
  return (c);
}

void
with_input_from_string (string, name)
     char *string;
     const char *name;
{
  INPUT_STREAM location;

  location.string = string;
  init_yy_io (yy_string_get, yy_string_unget, st_string, name, location);
}

/* Count the number of characters we've consumed from bash_input.location.string
   and read into shell_input_line, but have not returned from shell_getc.
   That is the true input location.  Rewind bash_input.location.string by
   that number of characters, so it points to the last character actually
   consumed by the parser. */
static void
rewind_input_string ()
{
  int xchars;

  /* number of unconsumed characters in the input -- XXX need to take newlines
     into account, e.g., $(...\n) */
  xchars = shell_input_line_len - shell_input_line_index;
  if (bash_input.location.string[-1] == '\n')
    xchars++;

  /* XXX - how to reflect bash_input.location.string back to string passed to
     parse_and_execute or xparse_dolparen?  xparse_dolparen needs to know how
     far into the string we parsed.  parse_and_execute knows where bash_input.
     location.string is, and how far from orig_string that is -- that's the
     number of characters the command consumed. */

  /* bash_input.location.string - xchars should be where we parsed to */
  /* need to do more validation on xchars value for sanity -- test cases. */
  bash_input.location.string -= xchars;
}

/* **************************************************************** */
/*								    */
/*		     Let input come from STREAM.		    */
/*								    */
/* **************************************************************** */

/* These two functions used to test the value of the HAVE_RESTARTABLE_SYSCALLS
   define, and just use getc/ungetc if it was defined, but since bash
   installs its signal handlers without the SA_RESTART flag, some signals
   (like SIGCHLD, SIGWINCH, etc.) received during a read(2) will not cause
   the read to be restarted.  We need to restart it ourselves. */

static int
yy_stream_get ()
{
  int result;

  result = EOF;
  if (bash_input.location.file)
    {
      if (interactive)
	{
	  interrupt_immediately++;
	  terminate_immediately++;
	}
      result = getc_with_restart (bash_input.location.file);
      if (interactive)
	{
	  interrupt_immediately--;
	  terminate_immediately--;
	}
    }
  return (result);
}

static int
yy_stream_unget (c)
     int c;
{
  return (ungetc_with_restart (c, bash_input.location.file));
}

void
with_input_from_stream (stream, name)
     FILE *stream;
     const char *name;
{
  INPUT_STREAM location;

  location.file = stream;
  init_yy_io (yy_stream_get, yy_stream_unget, st_stream, name, location);
}

typedef struct stream_saver {
  struct stream_saver *next;
  BASH_INPUT bash_input;
  int line;
#if defined (BUFFERED_INPUT)
  BUFFERED_STREAM *bstream;
#endif /* BUFFERED_INPUT */
} STREAM_SAVER;

/* The globally known line number. */
int line_number = 0;

/* The line number offset set by assigning to LINENO.  Not currently used. */
int line_number_base = 0;

#if defined (COND_COMMAND)
static int cond_lineno;
static int cond_token;
#endif

STREAM_SAVER *stream_list = (STREAM_SAVER *)NULL;

void
push_stream (reset_lineno)
     int reset_lineno;
{
  STREAM_SAVER *saver = (STREAM_SAVER *)xmalloc (sizeof (STREAM_SAVER));

  xbcopy ((char *)&bash_input, (char *)&(saver->bash_input), sizeof (BASH_INPUT));

#if defined (BUFFERED_INPUT)
  saver->bstream = (BUFFERED_STREAM *)NULL;
  /* If we have a buffered stream, clear out buffers[fd]. */
  if (bash_input.type == st_bstream && bash_input.location.buffered_fd >= 0)
    saver->bstream = set_buffered_stream (bash_input.location.buffered_fd,
    					  (BUFFERED_STREAM *)NULL);
#endif /* BUFFERED_INPUT */

  saver->line = line_number;
  bash_input.name = (char *)NULL;
  saver->next = stream_list;
  stream_list = saver;
  EOF_Reached = 0;
  if (reset_lineno)
    line_number = 0;
}

void
pop_stream ()
{
  if (!stream_list)
    EOF_Reached = 1;
  else
    {
      STREAM_SAVER *saver = stream_list;

      EOF_Reached = 0;
      stream_list = stream_list->next;

      init_yy_io (saver->bash_input.getter,
		  saver->bash_input.ungetter,
		  saver->bash_input.type,
		  saver->bash_input.name,
		  saver->bash_input.location);

#if defined (BUFFERED_INPUT)
      /* If we have a buffered stream, restore buffers[fd]. */
      /* If the input file descriptor was changed while this was on the
	 save stack, update the buffered fd to the new file descriptor and
	 re-establish the buffer <-> bash_input fd correspondence. */
      if (bash_input.type == st_bstream && bash_input.location.buffered_fd >= 0)
	{
	  if (bash_input_fd_changed)
	    {
	      bash_input_fd_changed = 0;
	      if (default_buffered_input >= 0)
		{
		  bash_input.location.buffered_fd = default_buffered_input;
		  saver->bstream->b_fd = default_buffered_input;
		  SET_CLOSE_ON_EXEC (default_buffered_input);
		}
	    }
	  /* XXX could free buffered stream returned as result here. */
	  set_buffered_stream (bash_input.location.buffered_fd, saver->bstream);
	}
#endif /* BUFFERED_INPUT */

      line_number = saver->line;

      FREE (saver->bash_input.name);
      free (saver);
    }
}

/* Return 1 if a stream of type TYPE is saved on the stack. */
int
stream_on_stack (type)
     enum stream_type type;
{
  register STREAM_SAVER *s;

  for (s = stream_list; s; s = s->next)
    if (s->bash_input.type == type)
      return 1;
  return 0;
}

/* Save the current token state and return it in a malloced array. */
int *
save_token_state ()
{
  int *ret;

  ret = (int *)xmalloc (4 * sizeof (int));
  ret[0] = last_read_token;
  ret[1] = token_before_that;
  ret[2] = two_tokens_ago;
  ret[3] = current_token;
  return ret;
}

void
restore_token_state (ts)
     int *ts;
{
  if (ts == 0)
    return;
  last_read_token = ts[0];
  token_before_that = ts[1];
  two_tokens_ago = ts[2];
  current_token = ts[3];
}

/*
 * This is used to inhibit alias expansion and reserved word recognition
 * inside case statement pattern lists.  A `case statement pattern list' is:
 *
 *	everything between the `in' in a `case word in' and the next ')'
 *	or `esac'
 *	everything between a `;;' and the next `)' or `esac'
 */

#if defined (ALIAS) || defined (DPAREN_ARITHMETIC)

#define END_OF_ALIAS 0

/*
 * Pseudo-global variables used in implementing token-wise alias expansion.
 */

/*
 * Pushing and popping strings.  This works together with shell_getc to
 * implement alias expansion on a per-token basis.
 */

typedef struct string_saver {
  struct string_saver *next;
  int expand_alias;  /* Value to set expand_alias to when string is popped. */
  char *saved_line;
#if defined (ALIAS)
  alias_t *expander;   /* alias that caused this line to be pushed. */
#endif
  int saved_line_size, saved_line_index, saved_line_terminator;
} STRING_SAVER;

STRING_SAVER *pushed_string_list = (STRING_SAVER *)NULL;

/*
 * Push the current shell_input_line onto a stack of such lines and make S
 * the current input.  Used when expanding aliases.  EXPAND is used to set
 * the value of expand_next_token when the string is popped, so that the
 * word after the alias in the original line is handled correctly when the
 * alias expands to multiple words.  TOKEN is the token that was expanded
 * into S; it is saved and used to prevent infinite recursive expansion.
 */
static void
push_string (s, expand, ap)
     char *s;
     int expand;
     alias_t *ap;
{
  STRING_SAVER *temp = (STRING_SAVER *)xmalloc (sizeof (STRING_SAVER));

  temp->expand_alias = expand;
  temp->saved_line = shell_input_line;
  temp->saved_line_size = shell_input_line_size;
  temp->saved_line_index = shell_input_line_index;
  temp->saved_line_terminator = shell_input_line_terminator;
#if defined (ALIAS)
  temp->expander = ap;
#endif
  temp->next = pushed_string_list;
  pushed_string_list = temp;

#if defined (ALIAS)
  if (ap)
    ap->flags |= AL_BEINGEXPANDED;
#endif

  shell_input_line = s;
  shell_input_line_size = strlen (s);
  shell_input_line_index = 0;
  shell_input_line_terminator = '\0';
#if 0
  parser_state &= ~PST_ALEXPNEXT;	/* XXX */
#endif

  set_line_mbstate ();
}

/*
 * Make the top of the pushed_string stack be the current shell input.
 * Only called when there is something on the stack.  Called from shell_getc
 * when it thinks it has consumed the string generated by an alias expansion
 * and needs to return to the original input line.
 */
static void
pop_string ()
{
  STRING_SAVER *t;

  FREE (shell_input_line);
  shell_input_line = pushed_string_list->saved_line;
  shell_input_line_index = pushed_string_list->saved_line_index;
  shell_input_line_size = pushed_string_list->saved_line_size;
  shell_input_line_terminator = pushed_string_list->saved_line_terminator;

  if (pushed_string_list->expand_alias)
    parser_state |= PST_ALEXPNEXT;
  else
    parser_state &= ~PST_ALEXPNEXT;

  t = pushed_string_list;
  pushed_string_list = pushed_string_list->next;

#if defined (ALIAS)
  if (t->expander)
    t->expander->flags &= ~AL_BEINGEXPANDED;
#endif

  free ((char *)t);

  set_line_mbstate ();
}

static void
free_string_list ()
{
  register STRING_SAVER *t, *t1;

  for (t = pushed_string_list; t; )
    {
      t1 = t->next;
      FREE (t->saved_line);
#if defined (ALIAS)
      if (t->expander)
	t->expander->flags &= ~AL_BEINGEXPANDED;
#endif
      free ((char *)t);
      t = t1;
    }
  pushed_string_list = (STRING_SAVER *)NULL;
}

#endif /* ALIAS || DPAREN_ARITHMETIC */

void
free_pushed_string_input ()
{
#if defined (ALIAS) || defined (DPAREN_ARITHMETIC)
  free_string_list ();
#endif
}

/* Return a line of text, taken from wherever yylex () reads input.
   If there is no more input, then we return NULL.  If REMOVE_QUOTED_NEWLINE
   is non-zero, we remove unquoted \<newline> pairs.  This is used by
   read_secondary_line to read here documents. */
static char *
read_a_line (remove_quoted_newline)
     int remove_quoted_newline;
{
  static char *line_buffer = (char *)NULL;
  static int buffer_size = 0;
  int indx, c, peekc, pass_next;

#if defined (READLINE)
  if (no_line_editing && SHOULD_PROMPT ())
#else
  if (SHOULD_PROMPT ())
#endif
    print_prompt ();

  pass_next = indx = 0;
  while (1)
    {
      /* Allow immediate exit if interrupted during input. */
      QUIT;

      c = yy_getc ();

      /* Ignore null bytes in input. */
      if (c == 0)
	{
#if 0
	  internal_warning ("read_a_line: ignored null byte in input");
#endif
	  continue;
	}

      /* If there is no more input, then we return NULL. */
      if (c == EOF)
	{
	  if (interactive && bash_input.type == st_stream)
	    clearerr (stdin);
	  if (indx == 0)
	    return ((char *)NULL);
	  c = '\n';
	}

      /* `+2' in case the final character in the buffer is a newline. */
      RESIZE_MALLOCED_BUFFER (line_buffer, indx, 2, buffer_size, 128);

      /* IF REMOVE_QUOTED_NEWLINES is non-zero, we are reading a
	 here document with an unquoted delimiter.  In this case,
	 the line will be expanded as if it were in double quotes.
	 We allow a backslash to escape the next character, but we
	 need to treat the backslash specially only if a backslash
	 quoting a backslash-newline pair appears in the line. */
      if (pass_next)
	{
	  line_buffer[indx++] = c;
	  pass_next = 0;
	}
      else if (c == '\\' && remove_quoted_newline)
	{
	  QUIT;
	  peekc = yy_getc ();
	  if (peekc == '\n')
	    {
	      line_number++;
	      continue;	/* Make the unquoted \<newline> pair disappear. */
	    }
	  else
	    {
	      yy_ungetc (peekc);
	      pass_next = 1;
	      line_buffer[indx++] = c;		/* Preserve the backslash. */
	    }
	}
      else
	line_buffer[indx++] = c;

      if (c == '\n')
	{
	  line_buffer[indx] = '\0';
	  return (line_buffer);
	}
    }
}

/* Return a line as in read_a_line (), but insure that the prompt is
   the secondary prompt.  This is used to read the lines of a here
   document.  REMOVE_QUOTED_NEWLINE is non-zero if we should remove
   newlines quoted with backslashes while reading the line.  It is
   non-zero unless the delimiter of the here document was quoted. */
char *
read_secondary_line (remove_quoted_newline)
     int remove_quoted_newline;
{
  char *ret;
  int n, c;

  prompt_string_pointer = &ps2_prompt;
  if (SHOULD_PROMPT())
    prompt_again ();
  ret = read_a_line (remove_quoted_newline);
#if defined (HISTORY)
  if (ret && remember_on_history && (parser_state & PST_HEREDOC))
    {
      /* To make adding the the here-document body right, we need to rely
	 on history_delimiting_chars() returning \n for the first line of
	 the here-document body and the null string for the second and
	 subsequent lines, so we avoid double newlines.
	 current_command_line_count == 2 for the first line of the body. */

      current_command_line_count++;
      maybe_add_history (ret);
    }
#endif /* HISTORY */
  return ret;
}

/* **************************************************************** */
/*								    */
/*				YYLEX ()			    */
/*								    */
/* **************************************************************** */

/* Reserved words.  These are only recognized as the first word of a
   command. */
STRING_INT_ALIST word_token_alist[] = {
  { "if", IF },
  { "then", THEN },
  { "else", ELSE },
  { "elif", ELIF },
  { "fi", FI },
  { "case", CASE },
  { "esac", ESAC },
  { "for", FOR },
#if defined (SELECT_COMMAND)
  { "select", SELECT },
#endif
  { "while", WHILE },
  { "until", UNTIL },
  { "do", DO },
  { "done", DONE },
  { "in", IN },
  { "function", FUNCTION },
#if defined (COMMAND_TIMING)
  { "time", TIME },
#endif
  { "{", '{' },
  { "}", '}' },
  { "!", BANG },
#if defined (COND_COMMAND)
  { "[[", COND_START },
  { "]]", COND_END },
#endif
#if defined (COPROCESS_SUPPORT)
  { "coproc", COPROC },
#endif
  { (char *)NULL, 0}
};

/* other tokens that can be returned by read_token() */
STRING_INT_ALIST other_token_alist[] = {
  /* Multiple-character tokens with special values */
  { "--", TIMEIGN },
  { "-p", TIMEOPT },
  { "&&", AND_AND },
  { "||", OR_OR },
  { ">>", GREATER_GREATER },
  { "<<", LESS_LESS },
  { "<&", LESS_AND },
  { ">&", GREATER_AND },
  { ";;", SEMI_SEMI },
  { ";&", SEMI_AND },
  { ";;&", SEMI_SEMI_AND },
  { "<<-", LESS_LESS_MINUS },
  { "<<<", LESS_LESS_LESS },
  { "&>", AND_GREATER },
  { "&>>", AND_GREATER_GREATER },
  { "<>", LESS_GREATER },
  { ">|", GREATER_BAR },
  { "|&", BAR_AND },
  { "EOF", yacc_EOF },
  /* Tokens whose value is the character itself */
  { ">", '>' },
  { "<", '<' },
  { "-", '-' },
  { "{", '{' },
  { "}", '}' },
  { ";", ';' },
  { "(", '(' },
  { ")", ')' },
  { "|", '|' },
  { "&", '&' },
  { "newline", '\n' },
  { (char *)NULL, 0}
};

/* others not listed here:
	WORD			look at yylval.word
	ASSIGNMENT_WORD		look at yylval.word
	NUMBER			look at yylval.number
	ARITH_CMD		look at yylval.word_list
	ARITH_FOR_EXPRS		look at yylval.word_list
	COND_CMD		look at yylval.command
*/

/* These are used by read_token_word, but appear up here so that shell_getc
   can use them to decide when to add otherwise blank lines to the history. */

/* The primary delimiter stack. */
struct dstack dstack = {  (char *)NULL, 0, 0 };

/* A temporary delimiter stack to be used when decoding prompt strings.
   This is needed because command substitutions in prompt strings (e.g., PS2)
   can screw up the parser's quoting state. */
static struct dstack temp_dstack = { (char *)NULL, 0, 0 };

/* Macro for accessing the top delimiter on the stack.  Returns the
   delimiter or zero if none. */
#define current_delimiter(ds) \
  (ds.delimiter_depth ? ds.delimiters[ds.delimiter_depth - 1] : 0)

#define push_delimiter(ds, character) \
  do \
    { \
      if (ds.delimiter_depth + 2 > ds.delimiter_space) \
	ds.delimiters = (char *)xrealloc \
	  (ds.delimiters, (ds.delimiter_space += 10) * sizeof (char)); \
      ds.delimiters[ds.delimiter_depth] = character; \
      ds.delimiter_depth++; \
    } \
  while (0)

#define pop_delimiter(ds)	ds.delimiter_depth--

/* Return the next shell input character.  This always reads characters
   from shell_input_line; when that line is exhausted, it is time to
   read the next line.  This is called by read_token when the shell is
   processing normal command input. */

/* This implements one-character lookahead/lookbehind across physical input
   lines, to avoid something being lost because it's pushed back with
   shell_ungetc when we're at the start of a line. */
static int eol_ungetc_lookahead = 0;

static int
shell_getc (remove_quoted_newline)
     int remove_quoted_newline;
{
  register int i;
  int c;
  unsigned char uc;

  QUIT;

  if (sigwinch_received)
    {
      sigwinch_received = 0;
      get_new_window_size (0, (int *)0, (int *)0);
    }
      
  if (eol_ungetc_lookahead)
    {
      c = eol_ungetc_lookahead;
      eol_ungetc_lookahead = 0;
      return (c);
    }

#if defined (ALIAS) || defined (DPAREN_ARITHMETIC)
  /* If shell_input_line[shell_input_line_index] == 0, but there is
     something on the pushed list of strings, then we don't want to go
     off and get another line.  We let the code down below handle it. */

  if (!shell_input_line || ((!shell_input_line[shell_input_line_index]) &&
			    (pushed_string_list == (STRING_SAVER *)NULL)))
#else /* !ALIAS && !DPAREN_ARITHMETIC */
  if (!shell_input_line || !shell_input_line[shell_input_line_index])
#endif /* !ALIAS && !DPAREN_ARITHMETIC */
    {
      line_number++;

    restart_read:

      /* Allow immediate exit if interrupted during input. */
      QUIT;

      i = 0;
      shell_input_line_terminator = 0;

      /* If the shell is interatctive, but not currently printing a prompt
         (interactive_shell && interactive == 0), we don't want to print
         notifies or cleanup the jobs -- we want to defer it until we do
         print the next prompt. */
      if (interactive_shell == 0 || SHOULD_PROMPT())
	{
#if defined (JOB_CONTROL)
      /* This can cause a problem when reading a command as the result
	 of a trap, when the trap is called from flush_child.  This call
	 had better not cause jobs to disappear from the job table in
	 that case, or we will have big trouble. */
	  notify_and_cleanup ();
#else /* !JOB_CONTROL */
	  cleanup_dead_jobs ();
#endif /* !JOB_CONTROL */
	}

#if defined (READLINE)
      if (no_line_editing && SHOULD_PROMPT())
#else
      if (SHOULD_PROMPT())
#endif
	print_prompt ();

      if (bash_input.type == st_stream)
	clearerr (stdin);

      while (1)
	{
	  c = yy_getc ();

	  /* Allow immediate exit if interrupted during input. */
	  QUIT;

	  if (c == '\0')
	    {
#if 0
	      internal_warning ("shell_getc: ignored null byte in input");
#endif
	      continue;
	    }

	  RESIZE_MALLOCED_BUFFER (shell_input_line, i, 2, shell_input_line_size, 256);

	  if (c == EOF)
	    {
	      if (bash_input.type == st_stream)
		clearerr (stdin);

	      if (i == 0)
		shell_input_line_terminator = EOF;

	      shell_input_line[i] = '\0';
	      break;
	    }

	  shell_input_line[i++] = c;

	  if (c == '\n')
	    {
	      shell_input_line[--i] = '\0';
	      current_command_line_count++;
	      break;
	    }
	}

      shell_input_line_index = 0;
      shell_input_line_len = i;		/* == strlen (shell_input_line) */

      set_line_mbstate ();

#if defined (HISTORY)
      if (remember_on_history && shell_input_line && shell_input_line[0])
	{
	  char *expansions;
#  if defined (BANG_HISTORY)
	  int old_hist;

	  /* If the current delimiter is a single quote, we should not be
	     performing history expansion, even if we're on a different
	     line from the original single quote. */
	  old_hist = history_expansion_inhibited;
	  if (current_delimiter (dstack) == '\'')
	    history_expansion_inhibited = 1;
#  endif
	  expansions = pre_process_line (shell_input_line, 1, 1);
#  if defined (BANG_HISTORY)
	  history_expansion_inhibited = old_hist;
#  endif
	  if (expansions != shell_input_line)
	    {
	      free (shell_input_line);
	      shell_input_line = expansions;
	      shell_input_line_len = shell_input_line ?
					strlen (shell_input_line) : 0;
	      if (shell_input_line_len == 0)
		current_command_line_count--;

	      /* We have to force the xrealloc below because we don't know
		 the true allocated size of shell_input_line anymore. */
	      shell_input_line_size = shell_input_line_len;

	      set_line_mbstate ();
	    }
	}
      /* Try to do something intelligent with blank lines encountered while
	 entering multi-line commands.  XXX - this is grotesque */
      else if (remember_on_history && shell_input_line &&
	       shell_input_line[0] == '\0' &&
	       current_command_line_count > 1)
	{
	  if (current_delimiter (dstack))
	    /* We know shell_input_line[0] == 0 and we're reading some sort of
	       quoted string.  This means we've got a line consisting of only
	       a newline in a quoted string.  We want to make sure this line
	       gets added to the history. */
	    maybe_add_history (shell_input_line);
	  else
	    {
	      char *hdcs;
	      hdcs = history_delimiting_chars (shell_input_line);
	      if (hdcs && hdcs[0] == ';')
		maybe_add_history (shell_input_line);
	    }
	}

#endif /* HISTORY */

      if (shell_input_line)
	{
	  /* Lines that signify the end of the shell's input should not be
	     echoed. */
	  if (echo_input_at_read && (shell_input_line[0] ||
				     shell_input_line_terminator != EOF))
	    fprintf (stderr, "%s\n", shell_input_line);
	}
      else
	{
	  shell_input_line_size = 0;
	  prompt_string_pointer = &current_prompt_string;
	  if (SHOULD_PROMPT ())
	    prompt_again ();
	  goto restart_read;
	}

      /* Add the newline to the end of this string, iff the string does
	 not already end in an EOF character.  */
      if (shell_input_line_terminator != EOF)
	{
	  if (shell_input_line_len + 3 > shell_input_line_size)
	    shell_input_line = (char *)xrealloc (shell_input_line,
					1 + (shell_input_line_size += 2));

	  shell_input_line[shell_input_line_len] = '\n';
	  shell_input_line[shell_input_line_len + 1] = '\0';

	  set_line_mbstate ();
	}
    }

next_alias_char:
  uc = shell_input_line[shell_input_line_index];

  if (uc)
    shell_input_line_index++;

#if defined (ALIAS) || defined (DPAREN_ARITHMETIC)
  /* If UC is NULL, we have reached the end of the current input string.  If
     pushed_string_list is non-empty, it's time to pop to the previous string
     because we have fully consumed the result of the last alias expansion.
     Do it transparently; just return the next character of the string popped
     to. */
pop_alias:
  if (uc == 0 && (pushed_string_list != (STRING_SAVER *)NULL))
    {
      pop_string ();
      uc = shell_input_line[shell_input_line_index];
      if (uc)
	shell_input_line_index++;
    }
#endif /* ALIAS || DPAREN_ARITHMETIC */

  if MBTEST(uc == '\\' && remove_quoted_newline && shell_input_line[shell_input_line_index] == '\n')
    {
	if (SHOULD_PROMPT ())
	  prompt_again ();
	line_number++;
	/* What do we do here if we're expanding an alias whose definition
	   includes an escaped newline?  If that's the last character in the
	   alias expansion, we just pop the pushed string list (recall that
	   we inhibit the appending of a space in mk_alexpansion() if newline
	   is the last character).  If it's not the last character, we need
	   to consume the quoted newline and move to the next character in
	   the expansion. */
#if defined (ALIAS)
	if (expanding_alias () && shell_input_line[shell_input_line_index+1] == '\0')
	  {
	    uc = 0;
	    goto pop_alias;
	  }
	else if (expanding_alias () && shell_input_line[shell_input_line_index+1] != '\0')
	  {
	    shell_input_line_index++;	/* skip newline */
	    goto next_alias_char;	/* and get next character */
	  }
	else
#endif 
	  goto restart_read;
    }

  if (uc == 0 && shell_input_line_terminator == EOF)
    return ((shell_input_line_index != 0) ? '\n' : EOF);

  return (uc);
}

/* Put C back into the input for the shell.  This might need changes for
   HANDLE_MULTIBYTE around EOLs.  Since we (currently) never push back a
   character different than we read, shell_input_line_property doesn't need
   to change when manipulating shell_input_line.  The define for
   last_shell_getc_is_singlebyte should take care of it, though. */
static void
shell_ungetc (c)
     int c;
{
  if (shell_input_line && shell_input_line_index)
    shell_input_line[--shell_input_line_index] = c;
  else
    eol_ungetc_lookahead = c;
}

char *
parser_remaining_input ()
{
  if (shell_input_line == 0)
    return 0;
  if (shell_input_line_index < 0 || shell_input_line_index >= shell_input_line_len)
    return '\0';	/* XXX */
  return (shell_input_line + shell_input_line_index);
}

#ifdef INCLUDE_UNUSED
/* Back the input pointer up by one, effectively `ungetting' a character. */
static void
shell_ungetchar ()
{
  if (shell_input_line && shell_input_line_index)
    shell_input_line_index--;
}
#endif

/* Discard input until CHARACTER is seen, then push that character back
   onto the input stream. */
static void
discard_until (character)
     int character;
{
  int c;

  while ((c = shell_getc (0)) != EOF && c != character)
    ;

  if (c != EOF)
    shell_ungetc (c);
}

void
execute_variable_command (command, vname)
     char *command, *vname;
{
  char *last_lastarg;
  sh_parser_state_t ps;

  save_parser_state (&ps);
  last_lastarg = get_string_value ("_");
  if (last_lastarg)
    last_lastarg = savestring (last_lastarg);

  parse_and_execute (savestring (command), vname, SEVAL_NONINT|SEVAL_NOHIST);

  restore_parser_state (&ps);
  bind_variable ("_", last_lastarg, 0);
  FREE (last_lastarg);

  if (token_to_read == '\n')	/* reset_parser was called */
    token_to_read = 0;
}

/* Place to remember the token.  We try to keep the buffer
   at a reasonable size, but it can grow. */
static char *token = (char *)NULL;

/* Current size of the token buffer. */
static int token_buffer_size;

/* Command to read_token () explaining what we want it to do. */
#define READ 0
#define RESET 1
#define prompt_is_ps1 \
      (!prompt_string_pointer || prompt_string_pointer == &ps1_prompt)

/* Function for yyparse to call.  yylex keeps track of
   the last two tokens read, and calls read_token.  */
static int
yylex ()
{
  if (interactive && (current_token == 0 || current_token == '\n'))
    {
      /* Before we print a prompt, we might have to check mailboxes.
	 We do this only if it is time to do so. Notice that only here
	 is the mail alarm reset; nothing takes place in check_mail ()
	 except the checking of mail.  Please don't change this. */
      if (prompt_is_ps1 && parse_and_execute_level == 0 && time_to_check_mail ())
	{
	  check_mail ();
	  reset_mail_timer ();
	}

      /* Avoid printing a prompt if we're not going to read anything, e.g.
	 after resetting the parser with read_token (RESET). */
      if (token_to_read == 0 && SHOULD_PROMPT ())
	prompt_again ();
    }

  two_tokens_ago = token_before_that;
  token_before_that = last_read_token;
  last_read_token = current_token;
  current_token = read_token (READ);

  if ((parser_state & PST_EOFTOKEN) && current_token == shell_eof_token)
    {
      current_token = yacc_EOF;
      if (bash_input.type == st_string)
	rewind_input_string ();
    }
  parser_state &= ~PST_EOFTOKEN;

  return (current_token);
}

/* When non-zero, we have read the required tokens
   which allow ESAC to be the next one read. */
static int esacs_needed_count;

static void
push_heredoc (r)
     REDIRECT *r;
{
  if (need_here_doc >= HEREDOC_MAX)
    {
      last_command_exit_value = EX_BADUSAGE;
      need_here_doc = 0;
      report_syntax_error (_("maximum here-document count exceeded"));
      reset_parser ();
      exit_shell (last_command_exit_value);
    }
  redir_stack[need_here_doc++] = r;
}

void
gather_here_documents ()
{
  int r;

  r = 0;
  while (need_here_doc)
    {
      parser_state |= PST_HEREDOC;
      make_here_document (redir_stack[r++], line_number);
      parser_state &= ~PST_HEREDOC;
      need_here_doc--;
    }
}

/* When non-zero, an open-brace used to create a group is awaiting a close
   brace partner. */
static int open_brace_count;

#define command_token_position(token) \
  (((token) == ASSIGNMENT_WORD) || (parser_state&PST_REDIRLIST) || \
   ((token) != SEMI_SEMI && (token) != SEMI_AND && (token) != SEMI_SEMI_AND && reserved_word_acceptable(token)))

#define assignment_acceptable(token) \
  (command_token_position(token) && ((parser_state & PST_CASEPAT) == 0))

/* Check to see if TOKEN is a reserved word and return the token
   value if it is. */
#define CHECK_FOR_RESERVED_WORD(tok) \
  do { \
    if (!dollar_present && !quoted && \
	reserved_word_acceptable (last_read_token)) \
      { \
	int i; \
	for (i = 0; word_token_alist[i].word != (char *)NULL; i++) \
	  if (STREQ (tok, word_token_alist[i].word)) \
	    { \
	      if ((parser_state & PST_CASEPAT) && (word_token_alist[i].token != ESAC)) \
		break; \
	      if (word_token_alist[i].token == TIME && time_command_acceptable () == 0) \
		break; \
	      if (word_token_alist[i].token == ESAC) \
		parser_state &= ~(PST_CASEPAT|PST_CASESTMT); \
	      else if (word_token_alist[i].token == CASE) \
		parser_state |= PST_CASESTMT; \
	      else if (word_token_alist[i].token == COND_END) \
		parser_state &= ~(PST_CONDCMD|PST_CONDEXPR); \
	      else if (word_token_alist[i].token == COND_START) \
		parser_state |= PST_CONDCMD; \
	      else if (word_token_alist[i].token == '{') \
		open_brace_count++; \
	      else if (word_token_alist[i].token == '}' && open_brace_count) \
		open_brace_count--; \
	      return (word_token_alist[i].token); \
	    } \
      } \
  } while (0)

#if defined (ALIAS)

    /* OK, we have a token.  Let's try to alias expand it, if (and only if)
       it's eligible.

       It is eligible for expansion if EXPAND_ALIASES is set, and
       the token is unquoted and the last token read was a command
       separator (or expand_next_token is set), and we are currently
       processing an alias (pushed_string_list is non-empty) and this
       token is not the same as the current or any previously
       processed alias.

       Special cases that disqualify:
	 In a pattern list in a case statement (parser_state & PST_CASEPAT). */

static char *
mk_alexpansion (s)
     char *s;
{
  int l;
  char *r;

  l = strlen (s);
  r = xmalloc (l + 2);
  strcpy (r, s);
  /* If the last character in the alias is a newline, don't add a trailing
     space to the expansion.  Works with shell_getc above. */
  if (r[l - 1] != ' ' && r[l - 1] != '\n')
    r[l++] = ' ';
  r[l] = '\0';
  return r;
}

static int
alias_expand_token (tokstr)
     char *tokstr;
{
  char *expanded;
  alias_t *ap;

  if (((parser_state & PST_ALEXPNEXT) || command_token_position (last_read_token)) &&
	(parser_state & PST_CASEPAT) == 0)
    {
      ap = find_alias (tokstr);

      /* Currently expanding this token. */
      if (ap && (ap->flags & AL_BEINGEXPANDED))
	return (NO_EXPANSION);

      /* mk_alexpansion puts an extra space on the end of the alias expansion,
         so the lookahead by the parser works right.  If this gets changed,
         make sure the code in shell_getc that deals with reaching the end of
         an expanded alias is changed with it. */
      expanded = ap ? mk_alexpansion (ap->value) : (char *)NULL;

      if (expanded)
	{
	  push_string (expanded, ap->flags & AL_EXPANDNEXT, ap);
	  return (RE_READ_TOKEN);
	}
      else
	/* This is an eligible token that does not have an expansion. */
	return (NO_EXPANSION);
    }
  return (NO_EXPANSION);
}
#endif /* ALIAS */

static int
time_command_acceptable ()
{
#if defined (COMMAND_TIMING)
  int i;

  if (posixly_correct && shell_compatibility_level > 41)
    {
      /* Quick check of the rest of the line to find the next token.  If it
	 begins with a `-', Posix says to not return `time' as the token.
	 This was interp 267. */
      i = shell_input_line_index;
      while (i < shell_input_line_len && (shell_input_line[i] == ' ' || shell_input_line[i] == '\t'))
        i++;
      if (shell_input_line[i] == '-')
	return 0;
    }

  switch (last_read_token)
    {
    case 0:
    case ';':
    case '\n':
    case AND_AND:
    case OR_OR:
    case '&':
    case DO:
    case THEN:
    case ELSE:
    case '{':		/* } */
    case '(':		/* ) */
    case BANG:		/* ! time pipeline */
    case TIME:		/* time time pipeline */
    case TIMEOPT:	/* time -p time pipeline */
    case TIMEIGN:	/* time -p -- ... */
      return 1;
    default:
      return 0;
    }
#else
  return 0;
#endif /* COMMAND_TIMING */
}

/* Handle special cases of token recognition:
	IN is recognized if the last token was WORD and the token
	before that was FOR or CASE or SELECT.

	DO is recognized if the last token was WORD and the token
	before that was FOR or SELECT.

	ESAC is recognized if the last token caused `esacs_needed_count'
	to be set

	`{' is recognized if the last token as WORD and the token
	before that was FUNCTION, or if we just parsed an arithmetic
	`for' command.

	`}' is recognized if there is an unclosed `{' present.

	`-p' is returned as TIMEOPT if the last read token was TIME.
	`--' is returned as TIMEIGN if the last read token was TIMEOPT.

	']]' is returned as COND_END if the parser is currently parsing
	a conditional expression ((parser_state & PST_CONDEXPR) != 0)

	`time' is returned as TIME if and only if it is immediately
	preceded by one of `;', `\n', `||', `&&', or `&'.
*/

static int
special_case_tokens (tokstr)
     char *tokstr;
{
  if ((last_read_token == WORD) &&
#if defined (SELECT_COMMAND)
      ((token_before_that == FOR) || (token_before_that == CASE) || (token_before_that == SELECT)) &&
#else
      ((token_before_that == FOR) || (token_before_that == CASE)) &&
#endif
      (tokstr[0] == 'i' && tokstr[1] == 'n' && tokstr[2] == 0))
    {
      if (token_before_that == CASE)
	{
	  parser_state |= PST_CASEPAT;
	  esacs_needed_count++;
	}
      return (IN);
    }

  if (last_read_token == WORD &&
#if defined (SELECT_COMMAND)
      (token_before_that == FOR || token_before_that == SELECT) &&
#else
      (token_before_that == FOR) &&
#endif
      (tokstr[0] == 'd' && tokstr[1] == 'o' && tokstr[2] == '\0'))
    return (DO);

  /* Ditto for ESAC in the CASE case.
     Specifically, this handles "case word in esac", which is a legal
     construct, certainly because someone will pass an empty arg to the
     case construct, and we don't want it to barf.  Of course, we should
     insist that the case construct has at least one pattern in it, but
     the designers disagree. */
  if (esacs_needed_count)
    {
      esacs_needed_count--;
      if (STREQ (tokstr, "esac"))
	{
	  parser_state &= ~PST_CASEPAT;
	  return (ESAC);
	}
    }

  /* The start of a shell function definition. */
  if (parser_state & PST_ALLOWOPNBRC)
    {
      parser_state &= ~PST_ALLOWOPNBRC;
      if (tokstr[0] == '{' && tokstr[1] == '\0')		/* } */
	{
	  open_brace_count++;
	  function_bstart = line_number;
	  return ('{');					/* } */
	}
    }

  /* We allow a `do' after a for ((...)) without an intervening
     list_terminator */
  if (last_read_token == ARITH_FOR_EXPRS && tokstr[0] == 'd' && tokstr[1] == 'o' && !tokstr[2])
    return (DO);
  if (last_read_token == ARITH_FOR_EXPRS && tokstr[0] == '{' && tokstr[1] == '\0')	/* } */
    {
      open_brace_count++;
      return ('{');			/* } */
    }

  if (open_brace_count && reserved_word_acceptable (last_read_token) && tokstr[0] == '}' && !tokstr[1])
    {
      open_brace_count--;		/* { */
      return ('}');
    }

#if defined (COMMAND_TIMING)
  /* Handle -p after `time'. */
  if (last_read_token == TIME && tokstr[0] == '-' && tokstr[1] == 'p' && !tokstr[2])
    return (TIMEOPT);
  /* Handle -- after `time -p'. */
  if (last_read_token == TIMEOPT && tokstr[0] == '-' && tokstr[1] == '-' && !tokstr[2])
    return (TIMEIGN);
#endif

#if defined (COND_COMMAND) /* [[ */
  if ((parser_state & PST_CONDEXPR) && tokstr[0] == ']' && tokstr[1] == ']' && tokstr[2] == '\0')
    return (COND_END);
#endif

  return (-1);
}

/* Called from shell.c when Control-C is typed at top level.  Or
   by the error rule at top level. */
void
reset_parser ()
{
  dstack.delimiter_depth = 0;	/* No delimiters found so far. */
  open_brace_count = 0;

#if defined (EXTENDED_GLOB)
  /* Reset to global value of extended glob */
  if (parser_state & PST_EXTPAT)
    extended_glob = global_extglob;
#endif

  parser_state = 0;

#if defined (ALIAS) || defined (DPAREN_ARITHMETIC)
  if (pushed_string_list)
    free_string_list ();
#endif /* ALIAS || DPAREN_ARITHMETIC */

  if (shell_input_line)
    {
      free (shell_input_line);
      shell_input_line = (char *)NULL;
      shell_input_line_size = shell_input_line_index = 0;
    }

  FREE (word_desc_to_read);
  word_desc_to_read = (WORD_DESC *)NULL;

  eol_ungetc_lookahead = 0;

  current_token = '\n';		/* XXX */
  last_read_token = '\n';
  token_to_read = '\n';
}

/* Read the next token.  Command can be READ (normal operation) or
   RESET (to normalize state). */
static int
read_token (command)
     int command;
{
  int character;		/* Current character. */
  int peek_char;		/* Temporary look-ahead character. */
  int result;			/* The thing to return. */

  if (command == RESET)
    {
      reset_parser ();
      return ('\n');
    }

  if (token_to_read)
    {
      result = token_to_read;
      if (token_to_read == WORD || token_to_read == ASSIGNMENT_WORD)
	{
	  yylval.word = word_desc_to_read;
	  word_desc_to_read = (WORD_DESC *)NULL;
	}
      token_to_read = 0;
      return (result);
    }

#if defined (COND_COMMAND)
  if ((parser_state & (PST_CONDCMD|PST_CONDEXPR)) == PST_CONDCMD)
    {
      cond_lineno = line_number;
      parser_state |= PST_CONDEXPR;
      yylval.command = parse_cond_command ();
      if (cond_token != COND_END)
	{
	  cond_error ();
	  return (-1);
	}
      token_to_read = COND_END;
      parser_state &= ~(PST_CONDEXPR|PST_CONDCMD);
      return (COND_CMD);
    }
#endif

#if defined (ALIAS)
  /* This is a place to jump back to once we have successfully expanded a
     token with an alias and pushed the string with push_string () */
 re_read_token:
#endif /* ALIAS */

  /* Read a single word from input.  Start by skipping blanks. */
  while ((character = shell_getc (1)) != EOF && shellblank (character))
    ;

  if (character == EOF)
    {
      EOF_Reached = 1;
      return (yacc_EOF);
    }

  if MBTEST(character == '#' && (!interactive || interactive_comments))
    {
      /* A comment.  Discard until EOL or EOF, and then return a newline. */
      discard_until ('\n');
      shell_getc (0);
      character = '\n';	/* this will take the next if statement and return. */
    }

  if (character == '\n')
    {
      /* If we're about to return an unquoted newline, we can go and collect
	 the text of any pending here document. */
      if (need_here_doc)
	gather_here_documents ();

#if defined (ALIAS)
      parser_state &= ~PST_ALEXPNEXT;
#endif /* ALIAS */

      parser_state &= ~PST_ASSIGNOK;

      return (character);
    }

  if (parser_state & PST_REGEXP)
    goto tokword;

  /* Shell meta-characters. */
  if MBTEST(shellmeta (character) && ((parser_state & PST_DBLPAREN) == 0))
    {
#if defined (ALIAS)
      /* Turn off alias tokenization iff this character sequence would
	 not leave us ready to read a command. */
      if (character == '<' || character == '>')
	parser_state &= ~PST_ALEXPNEXT;
#endif /* ALIAS */

      parser_state &= ~PST_ASSIGNOK;

      peek_char = shell_getc (1);
      if (character == peek_char)
	{
	  switch (character)
	    {
	    case '<':
	      /* If '<' then we could be at "<<" or at "<<-".  We have to
		 look ahead one more character. */
	      peek_char = shell_getc (1);
	      if MBTEST(peek_char == '-')
		return (LESS_LESS_MINUS);
	      else if MBTEST(peek_char == '<')
		return (LESS_LESS_LESS);
	      else
		{
		  shell_ungetc (peek_char);
		  return (LESS_LESS);
		}

	    case '>':
	      return (GREATER_GREATER);

	    case ';':
	      parser_state |= PST_CASEPAT;
#if defined (ALIAS)
	      parser_state &= ~PST_ALEXPNEXT;
#endif /* ALIAS */

	      peek_char = shell_getc (1);
	      if MBTEST(peek_char == '&')
		return (SEMI_SEMI_AND);
	      else
		{
		  shell_ungetc (peek_char);
		  return (SEMI_SEMI);
		}

	    case '&':
	      return (AND_AND);

	    case '|':
	      return (OR_OR);

#if defined (DPAREN_ARITHMETIC) || defined (ARITH_FOR_COMMAND)
	    case '(':		/* ) */
	      result = parse_dparen (character);
	      if (result == -2)
	        break;
	      else
	        return result;
#endif
	    }
	}
      else if MBTEST(character == '<' && peek_char == '&')
	return (LESS_AND);
      else if MBTEST(character == '>' && peek_char == '&')
	return (GREATER_AND);
      else if MBTEST(character == '<' && peek_char == '>')
	return (LESS_GREATER);
      else if MBTEST(character == '>' && peek_char == '|')
	return (GREATER_BAR);
      else if MBTEST(character == '&' && peek_char == '>')
	{
	  peek_char = shell_getc (1);
	  if MBTEST(peek_char == '>')
	    return (AND_GREATER_GREATER);
	  else
	    {
	      shell_ungetc (peek_char);
	      return (AND_GREATER);
	    }
	}
      else if MBTEST(character == '|' && peek_char == '&')
	return (BAR_AND);
      else if MBTEST(character == ';' && peek_char == '&')
	{
	  parser_state |= PST_CASEPAT;
#if defined (ALIAS)
	  parser_state &= ~PST_ALEXPNEXT;
#endif /* ALIAS */
	  return (SEMI_AND);
	}

      shell_ungetc (peek_char);

      /* If we look like we are reading the start of a function
	 definition, then let the reader know about it so that
	 we will do the right thing with `{'. */
      if MBTEST(character == ')' && last_read_token == '(' && token_before_that == WORD)
	{
	  parser_state |= PST_ALLOWOPNBRC;
#if defined (ALIAS)
	  parser_state &= ~PST_ALEXPNEXT;
#endif /* ALIAS */
	  function_dstart = line_number;
	}

      /* case pattern lists may be preceded by an optional left paren.  If
	 we're not trying to parse a case pattern list, the left paren
	 indicates a subshell. */
      if MBTEST(character == '(' && (parser_state & PST_CASEPAT) == 0) /* ) */
	parser_state |= PST_SUBSHELL;
      /*(*/
      else if MBTEST((parser_state & PST_CASEPAT) && character == ')')
	parser_state &= ~PST_CASEPAT;
      /*(*/
      else if MBTEST((parser_state & PST_SUBSHELL) && character == ')')
	parser_state &= ~PST_SUBSHELL;

#if defined (PROCESS_SUBSTITUTION)
      /* Check for the constructs which introduce process substitution.
	 Shells running in `posix mode' don't do process substitution. */
      if MBTEST(posixly_correct || ((character != '>' && character != '<') || peek_char != '(')) /*)*/
#endif /* PROCESS_SUBSTITUTION */
	return (character);
    }

  /* Hack <&- (close stdin) case.  Also <&N- (dup and close). */
  if MBTEST(character == '-' && (last_read_token == LESS_AND || last_read_token == GREATER_AND))
    return (character);

tokword:
  /* Okay, if we got this far, we have to read a word.  Read one,
     and then check it against the known ones. */
  result = read_token_word (character);
#if defined (ALIAS)
  if (result == RE_READ_TOKEN)
    goto re_read_token;
#endif
  return result;
}

/*
 * Match a $(...) or other grouping construct.  This has to handle embedded
 * quoted strings ('', ``, "") and nested constructs.  It also must handle
 * reprompting the user, if necessary, after reading a newline, and returning
 * correct error values if it reads EOF.
 */
#define P_FIRSTCLOSE	0x0001
#define P_ALLOWESC	0x0002
#define P_DQUOTE	0x0004
#define P_COMMAND	0x0008	/* parsing a command, so look for comments */
#define P_BACKQUOTE	0x0010	/* parsing a backquoted command substitution */
#define P_ARRAYSUB	0x0020	/* parsing a [...] array subscript for assignment */
#define P_DOLBRACE	0x0040	/* parsing a ${...} construct */

/* Lexical state while parsing a grouping construct or $(...). */
#define LEX_WASDOL	0x001
#define LEX_CKCOMMENT	0x002
#define LEX_INCOMMENT	0x004
#define LEX_PASSNEXT	0x008
#define LEX_RESWDOK	0x010
#define LEX_CKCASE	0x020
#define LEX_INCASE	0x040
#define LEX_INHEREDOC	0x080
#define LEX_HEREDELIM	0x100		/* reading here-doc delimiter */
#define LEX_STRIPDOC	0x200		/* <<- strip tabs from here doc delim */
#define LEX_INWORD	0x400

#define COMSUB_META(ch)		((ch) == ';' || (ch) == '&' || (ch) == '|')

#define CHECK_NESTRET_ERROR() \
  do { \
    if (nestret == &matched_pair_error) \
      { \
	free (ret); \
	return &matched_pair_error; \
      } \
  } while (0)

#define APPEND_NESTRET() \
  do { \
    if (nestlen) \
      { \
	RESIZE_MALLOCED_BUFFER (ret, retind, nestlen, retsize, 64); \
	strcpy (ret + retind, nestret); \
	retind += nestlen; \
      } \
  } while (0)

static char matched_pair_error;

static char *
parse_matched_pair (qc, open, close, lenp, flags)
     int qc;	/* `"' if this construct is within double quotes */
     int open, close;
     int *lenp, flags;
{
  int count, ch, tflags;
  int nestlen, ttranslen, start_lineno;
  char *ret, *nestret, *ttrans;
  int retind, retsize, rflags;
  int dolbrace_state;

  dolbrace_state = (flags & P_DOLBRACE) ? DOLBRACE_PARAM : 0;

/*itrace("parse_matched_pair[%d]: open = %c close = %c flags = %d", line_number, open, close, flags);*/
  count = 1;
  tflags = 0;

  if ((flags & P_COMMAND) && qc != '`' && qc != '\'' && qc != '"' && (flags & P_DQUOTE) == 0)
    tflags |= LEX_CKCOMMENT;

  /* RFLAGS is the set of flags we want to pass to recursive calls. */
  rflags = (qc == '"') ? P_DQUOTE : (flags & P_DQUOTE);

  ret = (char *)xmalloc (retsize = 64);
  retind = 0;

  start_lineno = line_number;
  while (count)
    {
      ch = shell_getc (qc != '\'' && (tflags & (LEX_PASSNEXT)) == 0);

      if (ch == EOF)
	{
	  free (ret);
	  parser_error (start_lineno, _("unexpected EOF while looking for matching `%c'"), close);
	  EOF_Reached = 1;	/* XXX */
	  return (&matched_pair_error);
	}

      /* Possible reprompting. */
      if (ch == '\n' && SHOULD_PROMPT ())
	prompt_again ();

      /* Don't bother counting parens or doing anything else if in a comment
	 or part of a case statement */
      if (tflags & LEX_INCOMMENT)
	{
	  /* Add this character. */
	  RESIZE_MALLOCED_BUFFER (ret, retind, 1, retsize, 64);
	  ret[retind++] = ch;

	  if (ch == '\n')
	    tflags &= ~LEX_INCOMMENT;

	  continue;
	}

      /* Not exactly right yet, should handle shell metacharacters, too.  If
	 any changes are made to this test, make analogous changes to subst.c:
	 extract_delimited_string(). */
      else if MBTEST((tflags & LEX_CKCOMMENT) && (tflags & LEX_INCOMMENT) == 0 && ch == '#' && (retind == 0 || ret[retind-1] == '\n' || shellblank (ret[retind - 1])))
	tflags |= LEX_INCOMMENT;

      if (tflags & LEX_PASSNEXT)		/* last char was backslash */
	{
	  tflags &= ~LEX_PASSNEXT;
	  if (qc != '\'' && ch == '\n')	/* double-quoted \<newline> disappears. */
	    {
	      if (retind > 0)
		retind--;	/* swallow previously-added backslash */
	      continue;
	    }

	  RESIZE_MALLOCED_BUFFER (ret, retind, 2, retsize, 64);
	  if MBTEST(ch == CTLESC || ch == CTLNUL)
	    ret[retind++] = CTLESC;
	  ret[retind++] = ch;
	  continue;
	}
      /* If we're reparsing the input (e.g., from parse_string_to_word_list),
	 we've already prepended CTLESC to single-quoted results of $'...'.
	 We may want to do this for other CTLESC-quoted characters in
	 reparse, too. */
      else if MBTEST((parser_state & PST_REPARSE) && open == '\'' && (ch == CTLESC || ch == CTLNUL))
	{
	  RESIZE_MALLOCED_BUFFER (ret, retind, 1, retsize, 64);
	  ret[retind++] = ch;
	  continue;
	}
      else if MBTEST(ch == CTLESC || ch == CTLNUL)	/* special shell escapes */
	{
	  RESIZE_MALLOCED_BUFFER (ret, retind, 2, retsize, 64);
	  ret[retind++] = CTLESC;
	  ret[retind++] = ch;
	  continue;
	}
      else if MBTEST(ch == close)		/* ending delimiter */
	count--;
      /* handle nested ${...} specially. */
      else if MBTEST(open != close && (tflags & LEX_WASDOL) && open == '{' && ch == open) /* } */
	count++;
      else if MBTEST(((flags & P_FIRSTCLOSE) == 0) && ch == open)	/* nested begin */
	count++;

      /* Add this character. */
      RESIZE_MALLOCED_BUFFER (ret, retind, 1, retsize, 64);
      ret[retind++] = ch;

      /* If we just read the ending character, don't bother continuing. */
      if (count == 0)
	break;

      if (open == '\'')			/* '' inside grouping construct */
	{
	  if MBTEST((flags & P_ALLOWESC) && ch == '\\')
	    tflags |= LEX_PASSNEXT;
	  continue;
	}

      if MBTEST(ch == '\\')			/* backslashes */
	tflags |= LEX_PASSNEXT;

      /* Based on which dolstate is currently in (param, op, or word),
	 decide what the op is.  We're really only concerned if it's % or
	 #, so we can turn on a flag that says whether or not we should
	 treat single quotes as special when inside a double-quoted
	 ${...}. This logic must agree with subst.c:extract_dollar_brace_string
	 since they share the same defines. */
      if (flags & P_DOLBRACE)
        {
          /* ${param%[%]word} */
	  if MBTEST(dolbrace_state == DOLBRACE_PARAM && ch == '%' && retind > 1)
	    dolbrace_state = DOLBRACE_QUOTE;
          /* ${param#[#]word} */
	  else if MBTEST(dolbrace_state == DOLBRACE_PARAM && ch == '#' && retind > 1)
	    dolbrace_state = DOLBRACE_QUOTE;
          /* ${param/[/]pat/rep} */
	  else if MBTEST(dolbrace_state == DOLBRACE_PARAM && ch == '/' && retind > 1)
	    dolbrace_state = DOLBRACE_QUOTE;
          /* ${param^[^]pat} */
	  else if MBTEST(dolbrace_state == DOLBRACE_PARAM && ch == '^' && retind > 1)
	    dolbrace_state = DOLBRACE_QUOTE;
          /* ${param,[,]pat} */
	  else if MBTEST(dolbrace_state == DOLBRACE_PARAM && ch == ',' && retind > 1)
	    dolbrace_state = DOLBRACE_QUOTE;
	  else if MBTEST(dolbrace_state == DOLBRACE_PARAM && strchr ("#%^,~:-=?+/", ch) != 0)
	    dolbrace_state = DOLBRACE_OP;
	  else if MBTEST(dolbrace_state == DOLBRACE_OP && strchr ("#%^,~:-=?+/", ch) == 0)
	    dolbrace_state = DOLBRACE_WORD;
        }

      /* The big hammer.  Single quotes aren't special in double quotes.  The
         problem is that Posix used to say the single quotes are semi-special:
         within a double-quoted ${...} construct "an even number of
         unescaped double-quotes or single-quotes, if any, shall occur." */
      /* This was changed in Austin Group Interp 221 */
      if MBTEST(posixly_correct && shell_compatibility_level > 41 && dolbrace_state != DOLBRACE_QUOTE && (flags & P_DQUOTE) && (flags & P_DOLBRACE) && ch == '\'')
	continue;

      /* Could also check open == '`' if we want to parse grouping constructs
	 inside old-style command substitution. */
      if (open != close)		/* a grouping construct */
	{
	  if MBTEST(shellquote (ch))
	    {
	      /* '', ``, or "" inside $(...) or other grouping construct. */
	      push_delimiter (dstack, ch);
	      if MBTEST((tflags & LEX_WASDOL) && ch == '\'')	/* $'...' inside group */
		nestret = parse_matched_pair (ch, ch, ch, &nestlen, P_ALLOWESC|rflags);
	      else
		nestret = parse_matched_pair (ch, ch, ch, &nestlen, rflags);
	      pop_delimiter (dstack);
	      CHECK_NESTRET_ERROR ();

	      if MBTEST((tflags & LEX_WASDOL) && ch == '\'' && (extended_quote || (rflags & P_DQUOTE) == 0))
		{
		  /* Translate $'...' here. */
		  ttrans = ansiexpand (nestret, 0, nestlen - 1, &ttranslen);
		  xfree (nestret);

		  if ((rflags & P_DQUOTE) == 0)
		    {
		      nestret = sh_single_quote (ttrans);
		      free (ttrans);
		      nestlen = strlen (nestret);
		    }
		  else
		    {
		      nestret = ttrans;
		      nestlen = ttranslen;
		    }
		  retind -= 2;		/* back up before the $' */
		}
	      else if MBTEST((tflags & LEX_WASDOL) && ch == '"' && (extended_quote || (rflags & P_DQUOTE) == 0))
		{
		  /* Locale expand $"..." here. */
		  ttrans = localeexpand (nestret, 0, nestlen - 1, start_lineno, &ttranslen);
		  xfree (nestret);

		  nestret = sh_mkdoublequoted (ttrans, ttranslen, 0);
		  free (ttrans);
		  nestlen = ttranslen + 2;
		  retind -= 2;		/* back up before the $" */
		}

	      APPEND_NESTRET ();
	      FREE (nestret);
	    }
	  else if ((flags & P_ARRAYSUB) && (tflags & LEX_WASDOL) && (ch == '(' || ch == '{' || ch == '['))	/* ) } ] */
	    goto parse_dollar_word;
	}
      /* Parse an old-style command substitution within double quotes as a
	 single word. */
      /* XXX - sh and ksh93 don't do this - XXX */
      else if MBTEST(open == '"' && ch == '`')
	{
	  nestret = parse_matched_pair (0, '`', '`', &nestlen, rflags);

	  CHECK_NESTRET_ERROR ();
	  APPEND_NESTRET ();

	  FREE (nestret);
	}
      else if MBTEST(open != '`' && (tflags & LEX_WASDOL) && (ch == '(' || ch == '{' || ch == '['))	/* ) } ] */
	/* check for $(), $[], or ${} inside quoted string. */
	{
parse_dollar_word:
	  if (open == ch)	/* undo previous increment */
	    count--;
	  if (ch == '(')		/* ) */
	    nestret = parse_comsub (0, '(', ')', &nestlen, (rflags|P_COMMAND) & ~P_DQUOTE);
	  else if (ch == '{')		/* } */
	    nestret = parse_matched_pair (0, '{', '}', &nestlen, P_FIRSTCLOSE|P_DOLBRACE|rflags);
	  else if (ch == '[')		/* ] */
	    nestret = parse_matched_pair (0, '[', ']', &nestlen, rflags);

	  CHECK_NESTRET_ERROR ();
	  APPEND_NESTRET ();

	  FREE (nestret);
	}
      if MBTEST(ch == '$')
	tflags |= LEX_WASDOL;
      else
	tflags &= ~LEX_WASDOL;
    }

  ret[retind] = '\0';
  if (lenp)
    *lenp = retind;
/*itrace("parse_matched_pair[%d]: returning %s", line_number, ret);*/
  return ret;
}

/* Parse a $(...) command substitution.  This is messier than I'd like, and
   reproduces a lot more of the token-reading code than I'd like. */
static char *
parse_comsub (qc, open, close, lenp, flags)
     int qc;	/* `"' if this construct is within double quotes */
     int open, close;
     int *lenp, flags;
{
  int count, ch, peekc, tflags, lex_rwlen, lex_wlen, lex_firstind;
  int nestlen, ttranslen, start_lineno;
  char *ret, *nestret, *ttrans, *heredelim;
  int retind, retsize, rflags, hdlen;

  /* Posix interp 217 says arithmetic expressions have precedence, so
     assume $(( introduces arithmetic expansion and parse accordingly. */
  peekc = shell_getc (0);
  shell_ungetc (peekc);
  if (peekc == '(')
    return (parse_matched_pair (qc, open, close, lenp, 0));

/*itrace("parse_comsub: qc = `%c' open = %c close = %c", qc, open, close);*/
  count = 1;
  tflags = LEX_RESWDOK;

  if ((flags & P_COMMAND) && qc != '\'' && qc != '"' && (flags & P_DQUOTE) == 0)
    tflags |= LEX_CKCASE;
  if ((tflags & LEX_CKCASE) && (interactive == 0 || interactive_comments))
    tflags |= LEX_CKCOMMENT;

  /* RFLAGS is the set of flags we want to pass to recursive calls. */
  rflags = (flags & P_DQUOTE);

  ret = (char *)xmalloc (retsize = 64);
  retind = 0;

  start_lineno = line_number;
  lex_rwlen = lex_wlen = 0;

  heredelim = 0;
  lex_firstind = -1;

  while (count)
    {
comsub_readchar:
      ch = shell_getc (qc != '\'' && (tflags & (LEX_INCOMMENT|LEX_PASSNEXT)) == 0);

      if (ch == EOF)
	{
eof_error:
	  free (ret);
	  FREE (heredelim);
	  parser_error (start_lineno, _("unexpected EOF while looking for matching `%c'"), close);
	  EOF_Reached = 1;	/* XXX */
	  return (&matched_pair_error);
	}

      /* If we hit the end of a line and are reading the contents of a here
	 document, and it's not the same line that the document starts on,
	 check for this line being the here doc delimiter.  Otherwise, if
	 we're in a here document, mark the next character as the beginning
	 of a line. */
      if (ch == '\n')
	{
	  if ((tflags & LEX_HEREDELIM) && heredelim)
	    {
	      tflags &= ~LEX_HEREDELIM;
	      tflags |= LEX_INHEREDOC;
	      lex_firstind = retind + 1;
	    }
	  else if (tflags & LEX_INHEREDOC)
	    {
	      int tind;
	      tind = lex_firstind;
	      while ((tflags & LEX_STRIPDOC) && ret[tind] == '\t')
		tind++;
	      if (STREQN (ret + tind, heredelim, hdlen))
		{
		  tflags &= ~(LEX_STRIPDOC|LEX_INHEREDOC);
/*itrace("parse_comsub:%d: found here doc end `%s'", line_number, ret + tind);*/
		  free (heredelim);
		  heredelim = 0;
		  lex_firstind = -1;
		}
	      else
		lex_firstind = retind + 1;
	    }
	}

      /* Possible reprompting. */
      if (ch == '\n' && SHOULD_PROMPT ())
	prompt_again ();

      /* XXX -- possibly allow here doc to be delimited by ending right
	 paren. */
      if ((tflags & LEX_INHEREDOC) && ch == close && count == 1)
	{
	  int tind;
/*itrace("parse_comsub: in here doc, ch == close, retind - firstind = %d hdlen = %d retind = %d", retind-lex_firstind, hdlen, retind);*/
	  tind = lex_firstind;
	  while ((tflags & LEX_STRIPDOC) && ret[tind] == '\t')
	    tind++;
	  if (retind-tind == hdlen && STREQN (ret + tind, heredelim, hdlen))
	    {
	      tflags &= ~(LEX_STRIPDOC|LEX_INHEREDOC);
/*itrace("parse_comsub:%d: found here doc end `%s'", line_number, ret + tind);*/
	      free (heredelim);
	      heredelim = 0;
	      lex_firstind = -1;
	    }
	}

      /* Don't bother counting parens or doing anything else if in a comment */
      if (tflags & (LEX_INCOMMENT|LEX_INHEREDOC))
	{
	  /* Add this character. */
	  RESIZE_MALLOCED_BUFFER (ret, retind, 1, retsize, 64);
	  ret[retind++] = ch;

	  if ((tflags & LEX_INCOMMENT) && ch == '\n')
{
/*itrace("parse_comsub:%d: lex_incomment -> 0 ch = `%c'", line_number, ch);*/
	    tflags &= ~LEX_INCOMMENT;
}

	  continue;
	}

      if (tflags & LEX_PASSNEXT)		/* last char was backslash */
	{
/*itrace("parse_comsub:%d: lex_passnext -> 0 ch = `%c' (%d)", line_number, ch, __LINE__);*/
	  tflags &= ~LEX_PASSNEXT;
	  if (qc != '\'' && ch == '\n')	/* double-quoted \<newline> disappears. */
	    {
	      if (retind > 0)
		retind--;	/* swallow previously-added backslash */
	      continue;
	    }

	  RESIZE_MALLOCED_BUFFER (ret, retind, 2, retsize, 64);
	  if MBTEST(ch == CTLESC || ch == CTLNUL)
	    ret[retind++] = CTLESC;
	  ret[retind++] = ch;
	  continue;
	}

      /* If this is a shell break character, we are not in a word.  If not,
	 we either start or continue a word. */
      if MBTEST(shellbreak (ch))
	{
	  tflags &= ~LEX_INWORD;
/*itrace("parse_comsub:%d: lex_inword -> 0 ch = `%c' (%d)", line_number, ch, __LINE__);*/
	}
      else
	{
	  if (tflags & LEX_INWORD)
	    {
	      lex_wlen++;
/*itrace("parse_comsub:%d: lex_inword == 1 ch = `%c' lex_wlen = %d (%d)", line_number, ch, lex_wlen, __LINE__);*/
	    }	      
	  else
	    {
/*itrace("parse_comsub:%d: lex_inword -> 1 ch = `%c' (%d)", line_number, ch, __LINE__);*/
	      tflags |= LEX_INWORD;
	      lex_wlen = 0;
	    }
	}

      /* Skip whitespace */
      if MBTEST(shellblank (ch) && (tflags & LEX_HEREDELIM) == 0 && lex_rwlen == 0)
        {
	  /* Add this character. */
	  RESIZE_MALLOCED_BUFFER (ret, retind, 1, retsize, 64);
	  ret[retind++] = ch;
	  continue;
        }

      /* Either we are looking for the start of the here-doc delimiter
	 (lex_firstind == -1) or we are reading one (lex_firstind >= 0).
	 If this character is a shell break character and we are reading
	 the delimiter, save it and note that we are now reading a here
	 document.  If we've found the start of the delimiter, note it by
	 setting lex_firstind.  Backslashes can quote shell metacharacters
	 in here-doc delimiters. */
      if (tflags & LEX_HEREDELIM)
	{
	  if (lex_firstind == -1 && shellbreak (ch) == 0)
	    lex_firstind = retind;
#if 0
	  else if (heredelim && (tflags & LEX_PASSNEXT) == 0 && ch == '\n')
	    {
	      tflags |= LEX_INHEREDOC;
	      tflags &= ~LEX_HEREDELIM;
	      lex_firstind = retind + 1;
	    }
#endif
	  else if (lex_firstind >= 0 && (tflags & LEX_PASSNEXT) == 0 && shellbreak (ch))
	    {
	      if (heredelim == 0)
		{
		  nestret = substring (ret, lex_firstind, retind);
		  heredelim = string_quote_removal (nestret, 0);
		  free (nestret);
		  hdlen = STRLEN(heredelim);
/*itrace("parse_comsub:%d: found here doc delimiter `%s' (%d)", line_number, heredelim, hdlen);*/
		}
	      if (ch == '\n')
		{
		  tflags |= LEX_INHEREDOC;
		  tflags &= ~LEX_HEREDELIM;
		  lex_firstind = retind + 1;
		}
	      else
		lex_firstind = -1;
	    }
	}

      /* Meta-characters that can introduce a reserved word.  Not perfect yet. */
      if MBTEST((tflags & LEX_RESWDOK) == 0 && (tflags & LEX_CKCASE) && (tflags & LEX_INCOMMENT) == 0 && (shellmeta(ch) || ch == '\n'))
	{
	  /* Add this character. */
	  RESIZE_MALLOCED_BUFFER (ret, retind, 1, retsize, 64);
	  ret[retind++] = ch;
	  peekc = shell_getc (1);
	  if (ch == peekc && (ch == '&' || ch == '|' || ch == ';'))	/* two-character tokens */
	    {
	      RESIZE_MALLOCED_BUFFER (ret, retind, 1, retsize, 64);
	      ret[retind++] = peekc;
/*itrace("parse_comsub:%d: set lex_reswordok = 1, ch = `%c'", line_number, ch);*/
	      tflags |= LEX_RESWDOK;
	      lex_rwlen = 0;
	      continue;
	    }
	  else if (ch == '\n' || COMSUB_META(ch))
	    {
	      shell_ungetc (peekc);
/*itrace("parse_comsub:%d: set lex_reswordok = 1, ch = `%c'", line_number, ch);*/
	      tflags |= LEX_RESWDOK;
	      lex_rwlen = 0;
	      continue;
	    }
	  else if (ch == EOF)
	    goto eof_error;
	  else
	    {
	      /* `unget' the character we just added and fall through */
	      retind--;
	      shell_ungetc (peekc);
	    }
	}

      /* If we can read a reserved word, try to read one. */
      if (tflags & LEX_RESWDOK)
	{
	  if MBTEST(islower (ch))
	    {
	      /* Add this character. */
	      RESIZE_MALLOCED_BUFFER (ret, retind, 1, retsize, 64);
	      ret[retind++] = ch;
	      lex_rwlen++;
	      continue;
	    }
	  else if MBTEST(lex_rwlen == 4 && shellbreak (ch))
	    {
	      if (STREQN (ret + retind - 4, "case", 4))
{
		tflags |= LEX_INCASE;
/*itrace("parse_comsub:%d: found `case', lex_incase -> 1 lex_reswdok -> 0", line_number);*/
}
	      else if (STREQN (ret + retind - 4, "esac", 4))
{
		tflags &= ~LEX_INCASE;
/*itrace("parse_comsub:%d: found `esac', lex_incase -> 0 lex_reswdok -> 0", line_number);*/
}	        
	      tflags &= ~LEX_RESWDOK;
	    }
	  else if MBTEST((tflags & LEX_CKCOMMENT) && ch == '#' && (lex_rwlen == 0 || ((tflags & LEX_INWORD) && lex_wlen == 0)))
	    ;	/* don't modify LEX_RESWDOK if we're starting a comment */
	  else if MBTEST((tflags & LEX_INCASE) && ch != '\n')
	    /* If we can read a reserved word and we're in case, we're at the
	       point where we can read a new pattern list or an esac.  We
	       handle the esac case above.  If we read a newline, we want to
	       leave LEX_RESWDOK alone.  If we read anything else, we want to
	       turn off LEX_RESWDOK, since we're going to read a pattern list. */
{
	    tflags &= ~LEX_RESWDOK;
/*itrace("parse_comsub:%d: lex_incase == 1 found `%c', lex_reswordok -> 0", line_number, ch);*/
}
	  else if MBTEST(shellbreak (ch) == 0)
{
	    tflags &= ~LEX_RESWDOK;
/*itrace("parse_comsub:%d: found `%c', lex_reswordok -> 0", line_number, ch);*/
}
	}

      /* Might be the start of a here-doc delimiter */
      if MBTEST((tflags & LEX_INCOMMENT) == 0 && (tflags & LEX_CKCASE) && ch == '<')
	{
	  /* Add this character. */
	  RESIZE_MALLOCED_BUFFER (ret, retind, 1, retsize, 64);
	  ret[retind++] = ch;
	  peekc = shell_getc (1);
	  if (peekc == EOF)
	    goto eof_error;
	  if (peekc == ch)
	    {
	      RESIZE_MALLOCED_BUFFER (ret, retind, 1, retsize, 64);
	      ret[retind++] = peekc;
	      peekc = shell_getc (1);
	      if (peekc == EOF)
		goto eof_error;
	      if (peekc == '-')
		{
		  RESIZE_MALLOCED_BUFFER (ret, retind, 1, retsize, 64);
		  ret[retind++] = peekc;
		  tflags |= LEX_STRIPDOC;
		}
	      else
		shell_ungetc (peekc);
	      if (peekc != '<')
		{
		  tflags |= LEX_HEREDELIM;
		  lex_firstind = -1;
		}
	      continue;
	    }
	  else
	    ch = peekc;		/* fall through and continue XXX */
	}
      else if MBTEST((tflags & LEX_CKCOMMENT) && (tflags & LEX_INCOMMENT) == 0 && ch == '#' && (((tflags & LEX_RESWDOK) && lex_rwlen == 0) || ((tflags & LEX_INWORD) && lex_wlen == 0)))
{
/*itrace("parse_comsub:%d: lex_incomment -> 1 (%d)", line_number, __LINE__);*/
	tflags |= LEX_INCOMMENT;
}

      if MBTEST(ch == CTLESC || ch == CTLNUL)	/* special shell escapes */
	{
	  RESIZE_MALLOCED_BUFFER (ret, retind, 2, retsize, 64);
	  ret[retind++] = CTLESC;
	  ret[retind++] = ch;
	  continue;
	}
#if 0
      else if MBTEST((tflags & LEX_INCASE) && ch == close && close == ')')
        tflags &= ~LEX_INCASE;		/* XXX */
#endif
      else if MBTEST(ch == close && (tflags & LEX_INCASE) == 0)		/* ending delimiter */
{
	count--;
/*itrace("parse_comsub:%d: found close: count = %d", line_number, count);*/
}
      else if MBTEST(((flags & P_FIRSTCLOSE) == 0) && (tflags & LEX_INCASE) == 0 && ch == open)	/* nested begin */
{
	count++;
/*itrace("parse_comsub:%d: found open: count = %d", line_number, count);*/
}

      /* Add this character. */
      RESIZE_MALLOCED_BUFFER (ret, retind, 1, retsize, 64);
      ret[retind++] = ch;

      /* If we just read the ending character, don't bother continuing. */
      if (count == 0)
	break;

      if MBTEST(ch == '\\')			/* backslashes */
	tflags |= LEX_PASSNEXT;

      if MBTEST(shellquote (ch))
        {
          /* '', ``, or "" inside $(...). */
          push_delimiter (dstack, ch);
          if MBTEST((tflags & LEX_WASDOL) && ch == '\'')	/* $'...' inside group */
	    nestret = parse_matched_pair (ch, ch, ch, &nestlen, P_ALLOWESC|rflags);
	  else
	    nestret = parse_matched_pair (ch, ch, ch, &nestlen, rflags);
	  pop_delimiter (dstack);
	  CHECK_NESTRET_ERROR ();

	  if MBTEST((tflags & LEX_WASDOL) && ch == '\'' && (extended_quote || (rflags & P_DQUOTE) == 0))
	    {
	      /* Translate $'...' here. */
	      ttrans = ansiexpand (nestret, 0, nestlen - 1, &ttranslen);
	      xfree (nestret);

	      if ((rflags & P_DQUOTE) == 0)
		{
		  nestret = sh_single_quote (ttrans);
		  free (ttrans);
		  nestlen = strlen (nestret);
		}
	      else
		{
		  nestret = ttrans;
		  nestlen = ttranslen;
		}
	      retind -= 2;		/* back up before the $' */
	    }
	  else if MBTEST((tflags & LEX_WASDOL) && ch == '"' && (extended_quote || (rflags & P_DQUOTE) == 0))
	    {
	      /* Locale expand $"..." here. */
	      ttrans = localeexpand (nestret, 0, nestlen - 1, start_lineno, &ttranslen);
	      xfree (nestret);

	      nestret = sh_mkdoublequoted (ttrans, ttranslen, 0);
	      free (ttrans);
	      nestlen = ttranslen + 2;
	      retind -= 2;		/* back up before the $" */
	    }

	  APPEND_NESTRET ();
	  FREE (nestret);
	}
      else if MBTEST((tflags & LEX_WASDOL) && (ch == '(' || ch == '{' || ch == '['))	/* ) } ] */
	/* check for $(), $[], or ${} inside command substitution. */
	{
	  if ((tflags & LEX_INCASE) == 0 && open == ch)	/* undo previous increment */
	    count--;
	  if (ch == '(')		/* ) */
	    nestret = parse_comsub (0, '(', ')', &nestlen, (rflags|P_COMMAND) & ~P_DQUOTE);
	  else if (ch == '{')		/* } */
	    nestret = parse_matched_pair (0, '{', '}', &nestlen, P_FIRSTCLOSE|P_DOLBRACE|rflags);
	  else if (ch == '[')		/* ] */
	    nestret = parse_matched_pair (0, '[', ']', &nestlen, rflags);

	  CHECK_NESTRET_ERROR ();
	  APPEND_NESTRET ();

	  FREE (nestret);
	}
      if MBTEST(ch == '$')
	tflags |= LEX_WASDOL;
      else
	tflags &= ~LEX_WASDOL;
    }

  FREE (heredelim);
  ret[retind] = '\0';
  if (lenp)
    *lenp = retind;
/*itrace("parse_comsub:%d: returning `%s'", line_number, ret);*/
  return ret;
}

/* Recursively call the parser to parse a $(...) command substitution. */
char *
xparse_dolparen (base, string, indp, flags)
     char *base;
     char *string;
     int *indp;
     int flags;
{
  sh_parser_state_t ps;
  sh_input_line_state_t ls;
  int orig_ind, nc, sflags;
  char *ret, *s, *ep, *ostring;

  /*yydebug = 1;*/
  orig_ind = *indp;
  ostring = string;

/*itrace("xparse_dolparen: size = %d shell_input_line = `%s'", shell_input_line_size, shell_input_line);*/
  sflags = SEVAL_NONINT|SEVAL_NOHIST|SEVAL_NOFREE;
  if (flags & SX_NOLONGJMP)
    sflags |= SEVAL_NOLONGJMP;
  save_parser_state (&ps);
  save_input_line_state (&ls);

  /*(*/
  parser_state |= PST_CMDSUBST|PST_EOFTOKEN;	/* allow instant ')' */ /*(*/
  shell_eof_token = ')';
  parse_string (string, "command substitution", sflags, &ep);

  restore_parser_state (&ps);
  reset_parser ();
  /* reset_parser clears shell_input_line and associated variables */
  restore_input_line_state (&ls);

  token_to_read = 0;

  /* Need to find how many characters parse_and_execute consumed, update
     *indp, if flags != 0, copy the portion of the string parsed into RET
     and return it.  If flags & 1 (EX_NOALLOC) we can return NULL. */

  /*(*/
  if (ep[-1] != ')')
    {
#if DEBUG
      if (ep[-1] != '\n')
	itrace("xparse_dolparen:%d: ep[-1] != RPAREN (%d), ep = `%s'", line_number, ep[-1], ep);
#endif
      while (ep > ostring && ep[-1] == '\n') ep--;
    }

  nc = ep - ostring;
  *indp = ep - base - 1;

  /*(*/
#if DEBUG
  if (base[*indp] != ')')
    itrace("xparse_dolparen:%d: base[%d] != RPAREN (%d), base = `%s'", line_number, *indp, base[*indp], base);
#endif

  if (flags & SX_NOALLOC) 
    return (char *)NULL;

  if (nc == 0)
    {
      ret = xmalloc (1);
      ret[0] = '\0';
    }
  else
    ret = substring (ostring, 0, nc - 1);

  return ret;
}

#if defined (DPAREN_ARITHMETIC) || defined (ARITH_FOR_COMMAND)
/* Parse a double-paren construct.  It can be either an arithmetic
   command, an arithmetic `for' command, or a nested subshell.  Returns
   the parsed token, -1 on error, or -2 if we didn't do anything and
   should just go on. */
static int
parse_dparen (c)
     int c;
{
  int cmdtyp, sline;
  char *wval;
  WORD_DESC *wd;

#if defined (ARITH_FOR_COMMAND)
  if (last_read_token == FOR)
    {
      arith_for_lineno = line_number;
      cmdtyp = parse_arith_cmd (&wval, 0);
      if (cmdtyp == 1)
	{
	  wd = alloc_word_desc ();
	  wd->word = wval;
	  yylval.word_list = make_word_list (wd, (WORD_LIST *)NULL);
	  return (ARITH_FOR_EXPRS);
	}
      else
	return -1;		/* ERROR */
    }
#endif

#if defined (DPAREN_ARITHMETIC)
  if (reserved_word_acceptable (last_read_token))
    {
      sline = line_number;

      cmdtyp = parse_arith_cmd (&wval, 0);
      if (cmdtyp == 1)	/* arithmetic command */
	{
	  wd = alloc_word_desc ();
	  wd->word = wval;
	  wd->flags = W_QUOTED|W_NOSPLIT|W_NOGLOB|W_DQUOTE;
	  yylval.word_list = make_word_list (wd, (WORD_LIST *)NULL);
	  return (ARITH_CMD);
	}
      else if (cmdtyp == 0)	/* nested subshell */
	{
	  push_string (wval, 0, (alias_t *)NULL);
	  if ((parser_state & PST_CASEPAT) == 0)
	    parser_state |= PST_SUBSHELL;
	  return (c);
	}
      else			/* ERROR */
	return -1;
    }
#endif

  return -2;			/* XXX */
}

/* We've seen a `(('.  Look for the matching `))'.  If we get it, return 1.
   If not, assume it's a nested subshell for backwards compatibility and
   return 0.  In any case, put the characters we've consumed into a locally-
   allocated buffer and make *ep point to that buffer.  Return -1 on an
   error, for example EOF. */
static int
parse_arith_cmd (ep, adddq)
     char **ep;
     int adddq;
{
  int exp_lineno, rval, c;
  char *ttok, *tokstr;
  int ttoklen;

  exp_lineno = line_number;
  ttok = parse_matched_pair (0, '(', ')', &ttoklen, 0);
  rval = 1;
  if (ttok == &matched_pair_error)
    return -1;
  /* Check that the next character is the closing right paren.  If
     not, this is a syntax error. ( */
  c = shell_getc (0);
  if MBTEST(c != ')')
    rval = 0;

  tokstr = (char *)xmalloc (ttoklen + 4);

  /* if ADDDQ != 0 then (( ... )) -> "..." */
  if (rval == 1 && adddq)	/* arith cmd, add double quotes */
    {
      tokstr[0] = '"';
      strncpy (tokstr + 1, ttok, ttoklen - 1);
      tokstr[ttoklen] = '"';
      tokstr[ttoklen+1] = '\0';
    }
  else if (rval == 1)		/* arith cmd, don't add double quotes */
    {
      strncpy (tokstr, ttok, ttoklen - 1);
      tokstr[ttoklen-1] = '\0';
    }
  else				/* nested subshell */
    {
      tokstr[0] = '(';
      strncpy (tokstr + 1, ttok, ttoklen - 1);
      tokstr[ttoklen] = ')';
      tokstr[ttoklen+1] = c;
      tokstr[ttoklen+2] = '\0';
    }

  *ep = tokstr;
  FREE (ttok);
  return rval;
}
#endif /* DPAREN_ARITHMETIC || ARITH_FOR_COMMAND */

#if defined (COND_COMMAND)
static void
cond_error ()
{
  char *etext;

  if (EOF_Reached && cond_token != COND_ERROR)		/* [[ */
    parser_error (cond_lineno, _("unexpected EOF while looking for `]]'"));
  else if (cond_token != COND_ERROR)
    {
      if (etext = error_token_from_token (cond_token))
	{
	  parser_error (cond_lineno, _("syntax error in conditional expression: unexpected token `%s'"), etext);
	  free (etext);
	}
      else
	parser_error (cond_lineno, _("syntax error in conditional expression"));
    }
}

static COND_COM *
cond_expr ()
{
  return (cond_or ());  
}

static COND_COM *
cond_or ()
{
  COND_COM *l, *r;

  l = cond_and ();
  if (cond_token == OR_OR)
    {
      r = cond_or ();
      l = make_cond_node (COND_OR, (WORD_DESC *)NULL, l, r);
    }
  return l;
}

static COND_COM *
cond_and ()
{
  COND_COM *l, *r;

  l = cond_term ();
  if (cond_token == AND_AND)
    {
      r = cond_and ();
      l = make_cond_node (COND_AND, (WORD_DESC *)NULL, l, r);
    }
  return l;
}

static int
cond_skip_newlines ()
{
  while ((cond_token = read_token (READ)) == '\n')
    {
      if (SHOULD_PROMPT ())
	prompt_again ();
    }
  return (cond_token);
}

#define COND_RETURN_ERROR() \
  do { cond_token = COND_ERROR; return ((COND_COM *)NULL); } while (0)

static COND_COM *
cond_term ()
{
  WORD_DESC *op;
  COND_COM *term, *tleft, *tright;
  int tok, lineno;
  char *etext;

  /* Read a token.  It can be a left paren, a `!', a unary operator, or a
     word that should be the first argument of a binary operator.  Start by
     skipping newlines, since this is a compound command. */
  tok = cond_skip_newlines ();
  lineno = line_number;
  if (tok == COND_END)
    {
      COND_RETURN_ERROR ();
    }
  else if (tok == '(')
    {
      term = cond_expr ();
      if (cond_token != ')')
	{
	  if (term)
	    dispose_cond_node (term);		/* ( */
	  if (etext = error_token_from_token (cond_token))
	    {
	      parser_error (lineno, _("unexpected token `%s', expected `)'"), etext);
	      free (etext);
	    }
	  else
	    parser_error (lineno, _("expected `)'"));
	  COND_RETURN_ERROR ();
	}
      term = make_cond_node (COND_EXPR, (WORD_DESC *)NULL, term, (COND_COM *)NULL);
      (void)cond_skip_newlines ();
    }
  else if (tok == BANG || (tok == WORD && (yylval.word->word[0] == '!' && yylval.word->word[1] == '\0')))
    {
      if (tok == WORD)
	dispose_word (yylval.word);	/* not needed */
      term = cond_term ();
      if (term)
	term->flags |= CMD_INVERT_RETURN;
    }
  else if (tok == WORD && yylval.word->word[0] == '-' && yylval.word->word[2] == 0 && test_unop (yylval.word->word))
    {
      op = yylval.word;
      tok = read_token (READ);
      if (tok == WORD)
	{
	  tleft = make_cond_node (COND_TERM, yylval.word, (COND_COM *)NULL, (COND_COM *)NULL);
	  term = make_cond_node (COND_UNARY, op, tleft, (COND_COM *)NULL);
	}
      else
	{
	  dispose_word (op);
	  if (etext = error_token_from_token (tok))
	    {
	      parser_error (line_number, _("unexpected argument `%s' to conditional unary operator"), etext);
	      free (etext);
	    }
	  else
	    parser_error (line_number, _("unexpected argument to conditional unary operator"));
	  COND_RETURN_ERROR ();
	}

      (void)cond_skip_newlines ();
    }
  else if (tok == WORD)		/* left argument to binary operator */
    {
      /* lhs */
      tleft = make_cond_node (COND_TERM, yylval.word, (COND_COM *)NULL, (COND_COM *)NULL);

      /* binop */
      tok = read_token (READ);
      if (tok == WORD && test_binop (yylval.word->word))
	{
	  op = yylval.word;
	  if (op->word[0] == '=' && (op->word[1] == '\0' || (op->word[1] == '=' && op->word[2] == '\0')))
	    parser_state |= PST_EXTPAT;
	  else if (op->word[0] == '!' && op->word[1] == '=' && op->word[2] == '\0')
	    parser_state |= PST_EXTPAT;
	}
#if defined (COND_REGEXP)
      else if (tok == WORD && STREQ (yylval.word->word, "=~"))
	{
	  op = yylval.word;
	  parser_state |= PST_REGEXP;
	}
#endif
      else if (tok == '<' || tok == '>')
	op = make_word_from_token (tok);  /* ( */
      /* There should be a check before blindly accepting the `)' that we have
	 seen the opening `('. */
      else if (tok == COND_END || tok == AND_AND || tok == OR_OR || tok == ')')
	{
	  /* Special case.  [[ x ]] is equivalent to [[ -n x ]], just like
	     the test command.  Similarly for [[ x && expr ]] or
	     [[ x || expr ]] or [[ (x) ]]. */
	  op = make_word ("-n");
	  term = make_cond_node (COND_UNARY, op, tleft, (COND_COM *)NULL);
	  cond_token = tok;
	  return (term);
	}
      else
	{
	  if (etext = error_token_from_token (tok))
	    {
	      parser_error (line_number, _("unexpected token `%s', conditional binary operator expected"), etext);
	      free (etext);
	    }
	  else
	    parser_error (line_number, _("conditional binary operator expected"));
	  dispose_cond_node (tleft);
	  COND_RETURN_ERROR ();
	}

      /* rhs */
      if (parser_state & PST_EXTPAT)
	extended_glob = 1;
      tok = read_token (READ);
      if (parser_state & PST_EXTPAT)
	extended_glob = global_extglob;
      parser_state &= ~(PST_REGEXP|PST_EXTPAT);

      if (tok == WORD)
	{
	  tright = make_cond_node (COND_TERM, yylval.word, (COND_COM *)NULL, (COND_COM *)NULL);
	  term = make_cond_node (COND_BINARY, op, tleft, tright);
	}
      else
	{
	  if (etext = error_token_from_token (tok))
	    {
	      parser_error (line_number, _("unexpected argument `%s' to conditional binary operator"), etext);
	      free (etext);
	    }
	  else
	    parser_error (line_number, _("unexpected argument to conditional binary operator"));
	  dispose_cond_node (tleft);
	  dispose_word (op);
	  COND_RETURN_ERROR ();
	}

      (void)cond_skip_newlines ();
    }
  else
    {
      if (tok < 256)
	parser_error (line_number, _("unexpected token `%c' in conditional command"), tok);
      else if (etext = error_token_from_token (tok))
	{
	  parser_error (line_number, _("unexpected token `%s' in conditional command"), etext);
	  free (etext);
	}
      else
	parser_error (line_number, _("unexpected token %d in conditional command"), tok);
      COND_RETURN_ERROR ();
    }
  return (term);
}      

/* This is kind of bogus -- we slip a mini recursive-descent parser in
   here to handle the conditional statement syntax. */
static COMMAND *
parse_cond_command ()
{
  COND_COM *cexp;

  global_extglob = extended_glob;
  cexp = cond_expr ();
  return (make_cond_command (cexp));
}
#endif

#if defined (ARRAY_VARS)
/* When this is called, it's guaranteed that we don't care about anything
   in t beyond i.  We do save and restore the chars, though. */
static int
token_is_assignment (t, i)
     char *t;
     int i;
{
  unsigned char c, c1;
  int r;

  c = t[i]; c1 = t[i+1];
  t[i] = '='; t[i+1] = '\0';
  r = assignment (t, (parser_state & PST_COMPASSIGN) != 0);
  t[i] = c; t[i+1] = c1;
  return r;
}

/* XXX - possible changes here for `+=' */
static int
token_is_ident (t, i)
     char *t;
     int i;
{
  unsigned char c;
  int r;

  c = t[i];
  t[i] = '\0';
  r = legal_identifier (t);
  t[i] = c;
  return r;
}
#endif

static int
read_token_word (character)
     int character;
{
  /* The value for YYLVAL when a WORD is read. */
  WORD_DESC *the_word;

  /* Index into the token that we are building. */
  int token_index;

  /* ALL_DIGITS becomes zero when we see a non-digit. */
  int all_digit_token;

  /* DOLLAR_PRESENT becomes non-zero if we see a `$'. */
  int dollar_present;

  /* COMPOUND_ASSIGNMENT becomes non-zero if we are parsing a compound
     assignment. */
  int compound_assignment;

  /* QUOTED becomes non-zero if we see one of ("), ('), (`), or (\). */
  int quoted;

  /* Non-zero means to ignore the value of the next character, and just
     to add it no matter what. */
 int pass_next_character;

  /* The current delimiting character. */
  int cd;
  int result, peek_char;
  char *ttok, *ttrans;
  int ttoklen, ttranslen;
  intmax_t lvalue;

  if (token_buffer_size < TOKEN_DEFAULT_INITIAL_SIZE)
    token = (char *)xrealloc (token, token_buffer_size = TOKEN_DEFAULT_INITIAL_SIZE);

  token_index = 0;
  all_digit_token = DIGIT (character);
  dollar_present = quoted = pass_next_character = compound_assignment = 0;

  for (;;)
    {
      if (character == EOF)
	goto got_token;

      if (pass_next_character)
	{
	  pass_next_character = 0;
	  goto got_escaped_character;
	}

      cd = current_delimiter (dstack);

      /* Handle backslashes.  Quote lots of things when not inside of
	 double-quotes, quote some things inside of double-quotes. */
      if MBTEST(character == '\\')
	{
	  peek_char = shell_getc (0);

	  /* Backslash-newline is ignored in all cases except
	     when quoted with single quotes. */
	  if (peek_char == '\n')
	    {
	      character = '\n';
	      goto next_character;
	    }
	  else
	    {
	      shell_ungetc (peek_char);

	      /* If the next character is to be quoted, note it now. */
	      if (cd == 0 || cd == '`' ||
		  (cd == '"' && peek_char >= 0 && (sh_syntaxtab[peek_char] & CBSDQUOTE)))
		pass_next_character++;

	      quoted = 1;
	      goto got_character;
	    }
	}

      /* Parse a matched pair of quote characters. */
      if MBTEST(shellquote (character))
	{
	  push_delimiter (dstack, character);
	  ttok = parse_matched_pair (character, character, character, &ttoklen, (character == '`') ? P_COMMAND : 0);
	  pop_delimiter (dstack);
	  if (ttok == &matched_pair_error)
	    return -1;		/* Bail immediately. */
	  RESIZE_MALLOCED_BUFFER (token, token_index, ttoklen + 2,
				  token_buffer_size, TOKEN_DEFAULT_GROW_SIZE);
	  token[token_index++] = character;
	  strcpy (token + token_index, ttok);
	  token_index += ttoklen;
	  all_digit_token = 0;
	  quoted = 1;
	  dollar_present |= (character == '"' && strchr (ttok, '$') != 0);
	  FREE (ttok);
	  goto next_character;
	}

#ifdef COND_REGEXP
      /* When parsing a regexp as a single word inside a conditional command,
	 we need to special-case characters special to both the shell and
	 regular expressions.  Right now, that is only '(' and '|'. */ /*)*/
      if MBTEST((parser_state & PST_REGEXP) && (character == '(' || character == '|'))		/*)*/
	{
	  if (character == '|')
	    goto got_character;

	  push_delimiter (dstack, character);
	  ttok = parse_matched_pair (cd, '(', ')', &ttoklen, 0);
	  pop_delimiter (dstack);
	  if (ttok == &matched_pair_error)
	    return -1;		/* Bail immediately. */
	  RESIZE_MALLOCED_BUFFER (token, token_index, ttoklen + 2,
				  token_buffer_size, TOKEN_DEFAULT_GROW_SIZE);
	  token[token_index++] = character;
	  strcpy (token + token_index, ttok);
	  token_index += ttoklen;
	  FREE (ttok);
	  dollar_present = all_digit_token = 0;
	  goto next_character;
	}
#endif /* COND_REGEXP */

#ifdef EXTENDED_GLOB
      /* Parse a ksh-style extended pattern matching specification. */
      if MBTEST(extended_glob && PATTERN_CHAR (character))
	{
	  peek_char = shell_getc (1);
	  if MBTEST(peek_char == '(')		/* ) */
	    {
	      push_delimiter (dstack, peek_char);
	      ttok = parse_matched_pair (cd, '(', ')', &ttoklen, 0);
	      pop_delimiter (dstack);
	      if (ttok == &matched_pair_error)
		return -1;		/* Bail immediately. */
	      RESIZE_MALLOCED_BUFFER (token, token_index, ttoklen + 2,
				      token_buffer_size,
				      TOKEN_DEFAULT_GROW_SIZE);
	      token[token_index++] = character;
	      token[token_index++] = peek_char;
	      strcpy (token + token_index, ttok);
	      token_index += ttoklen;
	      FREE (ttok);
	      dollar_present = all_digit_token = 0;
	      goto next_character;
	    }
	  else
	    shell_ungetc (peek_char);
	}
#endif /* EXTENDED_GLOB */

      /* If the delimiter character is not single quote, parse some of
	 the shell expansions that must be read as a single word. */
      if (shellexp (character))
	{
	  peek_char = shell_getc (1);
	  /* $(...), <(...), >(...), $((...)), ${...}, and $[...] constructs */
	  if MBTEST(peek_char == '(' || \
		((peek_char == '{' || peek_char == '[') && character == '$'))	/* ) ] } */
	    {
	      if (peek_char == '{')		/* } */
		ttok = parse_matched_pair (cd, '{', '}', &ttoklen, P_FIRSTCLOSE|P_DOLBRACE);
	      else if (peek_char == '(')		/* ) */
		{
		  /* XXX - push and pop the `(' as a delimiter for use by
		     the command-oriented-history code.  This way newlines
		     appearing in the $(...) string get added to the
		     history literally rather than causing a possibly-
		     incorrect `;' to be added. ) */
		  push_delimiter (dstack, peek_char);
		  ttok = parse_comsub (cd, '(', ')', &ttoklen, P_COMMAND);
		  pop_delimiter (dstack);
		}
	      else
		ttok = parse_matched_pair (cd, '[', ']', &ttoklen, 0);
	      if (ttok == &matched_pair_error)
		return -1;		/* Bail immediately. */
	      RESIZE_MALLOCED_BUFFER (token, token_index, ttoklen + 2,
				      token_buffer_size,
				      TOKEN_DEFAULT_GROW_SIZE);
	      token[token_index++] = character;
	      token[token_index++] = peek_char;
	      strcpy (token + token_index, ttok);
	      token_index += ttoklen;
	      FREE (ttok);
	      dollar_present = 1;
	      all_digit_token = 0;
	      goto next_character;
	    }
	  /* This handles $'...' and $"..." new-style quoted strings. */
	  else if MBTEST(character == '$' && (peek_char == '\'' || peek_char == '"'))
	    {
	      int first_line;

	      first_line = line_number;
	      push_delimiter (dstack, peek_char);
	      ttok = parse_matched_pair (peek_char, peek_char, peek_char,
					 &ttoklen,
					 (peek_char == '\'') ? P_ALLOWESC : 0);
	      pop_delimiter (dstack);
	      if (ttok == &matched_pair_error)
		return -1;
	      if (peek_char == '\'')
		{
		  ttrans = ansiexpand (ttok, 0, ttoklen - 1, &ttranslen);
		  free (ttok);

		  /* Insert the single quotes and correctly quote any
		     embedded single quotes (allowed because P_ALLOWESC was
		     passed to parse_matched_pair). */
		  ttok = sh_single_quote (ttrans);
		  free (ttrans);
		  ttranslen = strlen (ttok);
		  ttrans = ttok;
		}
	      else
		{
		  /* Try to locale-expand the converted string. */
		  ttrans = localeexpand (ttok, 0, ttoklen - 1, first_line, &ttranslen);
		  free (ttok);

		  /* Add the double quotes back */
		  ttok = sh_mkdoublequoted (ttrans, ttranslen, 0);
		  free (ttrans);
		  ttranslen += 2;
		  ttrans = ttok;
		}

	      RESIZE_MALLOCED_BUFFER (token, token_index, ttranslen + 2,
				      token_buffer_size,
				      TOKEN_DEFAULT_GROW_SIZE);
	      strcpy (token + token_index, ttrans);
	      token_index += ttranslen;
	      FREE (ttrans);
	      quoted = 1;
	      all_digit_token = 0;
	      goto next_character;
	    }
	  /* This could eventually be extended to recognize all of the
	     shell's single-character parameter expansions, and set flags.*/
	  else if MBTEST(character == '$' && peek_char == '$')
	    {
	      ttok = (char *)xmalloc (3);
	      ttok[0] = ttok[1] = '$';
	      ttok[2] = '\0';
	      RESIZE_MALLOCED_BUFFER (token, token_index, 3,
				      token_buffer_size,
				      TOKEN_DEFAULT_GROW_SIZE);
	      strcpy (token + token_index, ttok);
	      token_index += 2;
	      dollar_present = 1;
	      all_digit_token = 0;
	      FREE (ttok);
	      goto next_character;
	    }
	  else
	    shell_ungetc (peek_char);
	}

#if defined (ARRAY_VARS)
      /* Identify possible array subscript assignment; match [...].  If
	 parser_state&PST_COMPASSIGN, we need to parse [sub]=words treating
	 `sub' as if it were enclosed in double quotes. */
      else if MBTEST(character == '[' &&		/* ] */
		     ((token_index > 0 && assignment_acceptable (last_read_token) && token_is_ident (token, token_index)) ||
		      (token_index == 0 && (parser_state&PST_COMPASSIGN))))
        {
	  ttok = parse_matched_pair (cd, '[', ']', &ttoklen, P_ARRAYSUB);
	  if (ttok == &matched_pair_error)
	    return -1;		/* Bail immediately. */
	  RESIZE_MALLOCED_BUFFER (token, token_index, ttoklen + 2,
				  token_buffer_size,
				  TOKEN_DEFAULT_GROW_SIZE);
	  token[token_index++] = character;
	  strcpy (token + token_index, ttok);
	  token_index += ttoklen;
	  FREE (ttok);
	  all_digit_token = 0;
	  goto next_character;
        }
      /* Identify possible compound array variable assignment. */
      else if MBTEST(character == '=' && token_index > 0 && (assignment_acceptable (last_read_token) || (parser_state & PST_ASSIGNOK)) && token_is_assignment (token, token_index))
	{
	  peek_char = shell_getc (1);
	  if MBTEST(peek_char == '(')		/* ) */
	    {
	      ttok = parse_compound_assignment (&ttoklen);

	      RESIZE_MALLOCED_BUFFER (token, token_index, ttoklen + 4,
				      token_buffer_size,
				      TOKEN_DEFAULT_GROW_SIZE);

	      token[token_index++] = '=';
	      token[token_index++] = '(';
	      if (ttok)
		{
		  strcpy (token + token_index, ttok);
		  token_index += ttoklen;
		}
	      token[token_index++] = ')';
	      FREE (ttok);
	      all_digit_token = 0;
	      compound_assignment = 1;
#if 1
	      goto next_character;
#else
	      goto got_token;		/* ksh93 seems to do this */
#endif
	    }
	  else
	    shell_ungetc (peek_char);
	}
#endif

      /* When not parsing a multi-character word construct, shell meta-
	 characters break words. */
      if MBTEST(shellbreak (character))
	{
	  shell_ungetc (character);
	  goto got_token;
	}

    got_character:

      if (character == CTLESC || character == CTLNUL)
	token[token_index++] = CTLESC;

    got_escaped_character:

      all_digit_token &= DIGIT (character);
      dollar_present |= character == '$';

      token[token_index++] = character;

      RESIZE_MALLOCED_BUFFER (token, token_index, 1, token_buffer_size,
			      TOKEN_DEFAULT_GROW_SIZE);

    next_character:
      if (character == '\n' && SHOULD_PROMPT ())
	prompt_again ();

      /* We want to remove quoted newlines (that is, a \<newline> pair)
	 unless we are within single quotes or pass_next_character is
	 set (the shell equivalent of literal-next). */
      cd = current_delimiter (dstack);
      character = shell_getc (cd != '\'' && pass_next_character == 0);
    }	/* end for (;;) */

got_token:

  token[token_index] = '\0';

  /* Check to see what thing we should return.  If the last_read_token
     is a `<', or a `&', or the character which ended this token is
     a '>' or '<', then, and ONLY then, is this input token a NUMBER.
     Otherwise, it is just a word, and should be returned as such. */
  if MBTEST(all_digit_token && (character == '<' || character == '>' || \
		    last_read_token == LESS_AND || \
		    last_read_token == GREATER_AND))
      {
	if (legal_number (token, &lvalue) && (int)lvalue == lvalue)
	  yylval.number = lvalue;
	else
	  yylval.number = -1;
	return (NUMBER);
      }

  /* Check for special case tokens. */
  result = (last_shell_getc_is_singlebyte) ? special_case_tokens (token) : -1;
  if (result >= 0)
    return result;

#if defined (ALIAS)
  /* Posix.2 does not allow reserved words to be aliased, so check for all
     of them, including special cases, before expanding the current token
     as an alias. */
  if MBTEST(posixly_correct)
    CHECK_FOR_RESERVED_WORD (token);

  /* Aliases are expanded iff EXPAND_ALIASES is non-zero, and quoting
     inhibits alias expansion. */
  if (expand_aliases && quoted == 0)
    {
      result = alias_expand_token (token);
      if (result == RE_READ_TOKEN)
	return (RE_READ_TOKEN);
      else if (result == NO_EXPANSION)
	parser_state &= ~PST_ALEXPNEXT;
    }

  /* If not in Posix.2 mode, check for reserved words after alias
     expansion. */
  if MBTEST(posixly_correct == 0)
#endif
    CHECK_FOR_RESERVED_WORD (token);

  the_word = (WORD_DESC *)xmalloc (sizeof (WORD_DESC));
  the_word->word = (char *)xmalloc (1 + token_index);
  the_word->flags = 0;
  strcpy (the_word->word, token);
  if (dollar_present)
    the_word->flags |= W_HASDOLLAR;
  if (quoted)
    the_word->flags |= W_QUOTED;		/*(*/
  if (compound_assignment && token[token_index-1] == ')')
    the_word->flags |= W_COMPASSIGN;
  /* A word is an assignment if it appears at the beginning of a
     simple command, or after another assignment word.  This is
     context-dependent, so it cannot be handled in the grammar. */
  if (assignment (token, (parser_state & PST_COMPASSIGN) != 0))
    {
      the_word->flags |= W_ASSIGNMENT;
      /* Don't perform word splitting on assignment statements. */
      if (assignment_acceptable (last_read_token) || (parser_state & PST_COMPASSIGN) != 0)
	the_word->flags |= W_NOSPLIT;
    }

  if (command_token_position (last_read_token))
    {
      struct builtin *b;
      b = builtin_address_internal (token, 0);
      if (b && (b->flags & ASSIGNMENT_BUILTIN))
	parser_state |= PST_ASSIGNOK;
      else if (STREQ (token, "eval") || STREQ (token, "let"))
	parser_state |= PST_ASSIGNOK;
    }

  yylval.word = the_word;

  if (token[0] == '{' && token[token_index-1] == '}' &&
      (character == '<' || character == '>'))
    {
      /* can use token; already copied to the_word */
      token[token_index-1] = '\0';
      if (legal_identifier (token+1))
	{
	  strcpy (the_word->word, token+1);
/*itrace("read_token_word: returning REDIR_WORD for %s", the_word->word);*/
	  return (REDIR_WORD);
	}
    }

  result = ((the_word->flags & (W_ASSIGNMENT|W_NOSPLIT)) == (W_ASSIGNMENT|W_NOSPLIT))
		? ASSIGNMENT_WORD : WORD;

  switch (last_read_token)
    {
    case FUNCTION:
      parser_state |= PST_ALLOWOPNBRC;
      function_dstart = line_number;
      break;
    case CASE:
    case SELECT:
    case FOR:
      if (word_top < MAX_CASE_NEST)
	word_top++;
      word_lineno[word_top] = line_number;
      break;
    }

  return (result);
}

/* Return 1 if TOKSYM is a token that after being read would allow
   a reserved word to be seen, else 0. */
static int
reserved_word_acceptable (toksym)
     int toksym;
{
  switch (toksym)
    {
    case '\n':
    case ';':
    case '(':
    case ')':
    case '|':
    case '&':
    case '{':
    case '}':		/* XXX */
    case AND_AND:
    case BANG:
    case BAR_AND:
    case DO:
    case DONE:
    case ELIF:
    case ELSE:
    case ESAC:
    case FI:
    case IF:
    case OR_OR:
    case SEMI_SEMI:
    case SEMI_AND:
    case SEMI_SEMI_AND:
    case THEN:
    case TIME:
    case TIMEOPT:
    case TIMEIGN:
    case COPROC:
    case UNTIL:
    case WHILE:
    case 0:
      return 1;
    default:
#if defined (COPROCESS_SUPPORT)
      if (last_read_token == WORD && token_before_that == COPROC)
	return 1;
#endif
      if (last_read_token == WORD && token_before_that == FUNCTION)
	return 1;
      return 0;
    }
}
    
/* Return the index of TOKEN in the alist of reserved words, or -1 if
   TOKEN is not a shell reserved word. */
int
find_reserved_word (tokstr)
     char *tokstr;
{
  int i;
  for (i = 0; word_token_alist[i].word; i++)
    if (STREQ (tokstr, word_token_alist[i].word))
      return i;
  return -1;
}

#if 0
#if defined (READLINE)
/* Called after each time readline is called.  This insures that whatever
   the new prompt string is gets propagated to readline's local prompt
   variable. */
static void
reset_readline_prompt ()
{
  char *temp_prompt;

  if (prompt_string_pointer)
    {
      temp_prompt = (*prompt_string_pointer)
			? decode_prompt_string (*prompt_string_pointer)
			: (char *)NULL;

      if (temp_prompt == 0)
	{
	  temp_prompt = (char *)xmalloc (1);
	  temp_prompt[0] = '\0';
	}

      FREE (current_readline_prompt);
      current_readline_prompt = temp_prompt;
    }
}
#endif /* READLINE */
#endif /* 0 */

#if defined (HISTORY)
/* A list of tokens which can be followed by newlines, but not by
   semi-colons.  When concatenating multiple lines of history, the
   newline separator for such tokens is replaced with a space. */
static const int no_semi_successors[] = {
  '\n', '{', '(', ')', ';', '&', '|',
  CASE, DO, ELSE, IF, SEMI_SEMI, SEMI_AND, SEMI_SEMI_AND, THEN, UNTIL,
  WHILE, AND_AND, OR_OR, IN,
  0
};

/* If we are not within a delimited expression, try to be smart
   about which separators can be semi-colons and which must be
   newlines.  Returns the string that should be added into the
   history entry.  LINE is the line we're about to add; it helps
   make some more intelligent decisions in certain cases. */
char *
history_delimiting_chars (line)
     const char *line;
{
  static int last_was_heredoc = 0;	/* was the last entry the start of a here document? */
  register int i;

  if ((parser_state & PST_HEREDOC) == 0)
    last_was_heredoc = 0;

  if (dstack.delimiter_depth != 0)
    return ("\n");

  /* We look for current_command_line_count == 2 because we are looking to
     add the first line of the body of the here document (the second line
     of the command).  We also keep LAST_WAS_HEREDOC as a private sentinel
     variable to note when we think we added the first line of a here doc
     (the one with a "<<" somewhere in it) */
  if (parser_state & PST_HEREDOC)
    {
      if (last_was_heredoc)
	{
	  last_was_heredoc = 0;
	  return "\n";
	}
      return (current_command_line_count == 2 ? "\n" : "");
    }

  if (parser_state & PST_COMPASSIGN)
    return (" ");

  /* First, handle some special cases. */
  /*(*/
  /* If we just read `()', assume it's a function definition, and don't
     add a semicolon.  If the token before the `)' was not `(', and we're
     not in the midst of parsing a case statement, assume it's a
     parenthesized command and add the semicolon. */
  /*)(*/
  if (token_before_that == ')')
    {
      if (two_tokens_ago == '(')	/*)*/	/* function def */
	return " ";
      /* This does not work for subshells inside case statement
	 command lists.  It's a suboptimal solution. */
      else if (parser_state & PST_CASESTMT)	/* case statement pattern */
	return " ";
      else	
	return "; ";				/* (...) subshell */
    }
  else if (token_before_that == WORD && two_tokens_ago == FUNCTION)
    return " ";		/* function def using `function name' without `()' */

  /* If we're not in a here document, but we think we're about to parse one,
     and we would otherwise return a `;', return a newline to delimit the
     line with the here-doc delimiter */
  else if ((parser_state & PST_HEREDOC) == 0 && current_command_line_count > 1 && last_read_token == '\n' && strstr (line, "<<"))
    {
      last_was_heredoc = 1;
      return "\n";
    }

  else if (token_before_that == WORD && two_tokens_ago == FOR)
    {
      /* Tricky.  `for i\nin ...' should not have a semicolon, but
	 `for i\ndo ...' should.  We do what we can. */
      for (i = shell_input_line_index; whitespace (shell_input_line[i]); i++)
	;
      if (shell_input_line[i] && shell_input_line[i] == 'i' && shell_input_line[i+1] == 'n')
	return " ";
      return ";";
    }
  else if (two_tokens_ago == CASE && token_before_that == WORD && (parser_state & PST_CASESTMT))
    return " ";

  for (i = 0; no_semi_successors[i]; i++)
    {
      if (token_before_that == no_semi_successors[i])
	return (" ");
    }

  return ("; ");
}
#endif /* HISTORY */

/* Issue a prompt, or prepare to issue a prompt when the next character
   is read. */
static void
prompt_again ()
{
  char *temp_prompt;

  if (interactive == 0 || expanding_alias ())	/* XXX */
    return;

  ps1_prompt = get_string_value ("PS1");
  ps2_prompt = get_string_value ("PS2");

  if (!prompt_string_pointer)
    prompt_string_pointer = &ps1_prompt;

  temp_prompt = *prompt_string_pointer
			? decode_prompt_string (*prompt_string_pointer)
			: (char *)NULL;

  if (temp_prompt == 0)
    {
      temp_prompt = (char *)xmalloc (1);
      temp_prompt[0] = '\0';
    }

  current_prompt_string = *prompt_string_pointer;
  prompt_string_pointer = &ps2_prompt;

#if defined (READLINE)
  if (!no_line_editing)
    {
      FREE (current_readline_prompt);
      current_readline_prompt = temp_prompt;
    }
  else
#endif	/* READLINE */
    {
      FREE (current_decoded_prompt);
      current_decoded_prompt = temp_prompt;
    }
}

int
get_current_prompt_level ()
{
  return ((current_prompt_string && current_prompt_string == ps2_prompt) ? 2 : 1);
}

void
set_current_prompt_level (x)
     int x;
{
  prompt_string_pointer = (x == 2) ? &ps2_prompt : &ps1_prompt;
  current_prompt_string = *prompt_string_pointer;
}
      
static void
print_prompt ()
{
  fprintf (stderr, "%s", current_decoded_prompt);
  fflush (stderr);
}

/* Return a string which will be printed as a prompt.  The string
   may contain special characters which are decoded as follows:

	\a	bell (ascii 07)
	\d	the date in Day Mon Date format
	\e	escape (ascii 033)
	\h	the hostname up to the first `.'
	\H	the hostname
	\j	the number of active jobs
	\l	the basename of the shell's tty device name
	\n	CRLF
	\r	CR
	\s	the name of the shell
	\t	the time in 24-hour hh:mm:ss format
	\T	the time in 12-hour hh:mm:ss format
	\@	the time in 12-hour hh:mm am/pm format
	\A	the time in 24-hour hh:mm format
	\D{fmt}	the result of passing FMT to strftime(3)
	\u	your username
	\v	the version of bash (e.g., 2.00)
	\V	the release of bash, version + patchlevel (e.g., 2.00.0)
	\w	the current working directory
	\W	the last element of $PWD
	\!	the history number of this command
	\#	the command number of this command
	\$	a $ or a # if you are root
	\nnn	character code nnn in octal
	\\	a backslash
	\[	begin a sequence of non-printing chars
	\]	end a sequence of non-printing chars
*/
#define PROMPT_GROWTH 48
char *
decode_prompt_string (string)
     char *string;
{
  WORD_LIST *list;
  char *result, *t;
  struct dstack save_dstack;
  int last_exit_value, last_comsub_pid;
#if defined (PROMPT_STRING_DECODE)
  int result_size, result_index;
  int c, n, i;
  char *temp, octal_string[4];
  struct tm *tm;  
  time_t the_time;
  char timebuf[128];
  char *timefmt;

  result = (char *)xmalloc (result_size = PROMPT_GROWTH);
  result[result_index = 0] = 0;
  temp = (char *)NULL;

  while (c = *string++)
    {
      if (posixly_correct && c == '!')
	{
	  if (*string == '!')
	    {
	      temp = savestring ("!");
	      goto add_string;
	    }
	  else
	    {
#if !defined (HISTORY)
		temp = savestring ("1");
#else /* HISTORY */
		temp = itos (history_number ());
#endif /* HISTORY */
		string--;	/* add_string increments string again. */
		goto add_string;
	    }
	}
      if (c == '\\')
	{
	  c = *string;

	  switch (c)
	    {
	    case '0':
	    case '1':
	    case '2':
	    case '3':
	    case '4':
	    case '5':
	    case '6':
	    case '7':
	      strncpy (octal_string, string, 3);
	      octal_string[3] = '\0';

	      n = read_octal (octal_string);
	      temp = (char *)xmalloc (3);

	      if (n == CTLESC || n == CTLNUL)
		{
		  temp[0] = CTLESC;
		  temp[1] = n;
		  temp[2] = '\0';
		}
	      else if (n == -1)
		{
		  temp[0] = '\\';
		  temp[1] = '\0';
		}
	      else
		{
		  temp[0] = n;
		  temp[1] = '\0';
		}

	      for (c = 0; n != -1 && c < 3 && ISOCTAL (*string); c++)
		string++;

	      c = 0;		/* tested at add_string: */
	      goto add_string;

	    case 'd':
	    case 't':
	    case 'T':
	    case '@':
	    case 'A':
	      /* Make the current time/date into a string. */
	      (void) time (&the_time);
#if defined (HAVE_TZSET)
	      sv_tz ("TZ");		/* XXX -- just make sure */
#endif
	      tm = localtime (&the_time);

	      if (c == 'd')
		n = strftime (timebuf, sizeof (timebuf), "%a %b %d", tm);
	      else if (c == 't')
		n = strftime (timebuf, sizeof (timebuf), "%H:%M:%S", tm);
	      else if (c == 'T')
		n = strftime (timebuf, sizeof (timebuf), "%I:%M:%S", tm);
	      else if (c == '@')
		n = strftime (timebuf, sizeof (timebuf), "%I:%M %p", tm);
	      else if (c == 'A')
		n = strftime (timebuf, sizeof (timebuf), "%H:%M", tm);

	      if (n == 0)
		timebuf[0] = '\0';
	      else
		timebuf[sizeof(timebuf) - 1] = '\0';

	      temp = savestring (timebuf);
	      goto add_string;

	    case 'D':		/* strftime format */
	      if (string[1] != '{')		/* } */
		goto not_escape;

	      (void) time (&the_time);
	      tm = localtime (&the_time);
	      string += 2;			/* skip { */
	      timefmt = xmalloc (strlen (string) + 3);
	      for (t = timefmt; *string && *string != '}'; )
		*t++ = *string++;
	      *t = '\0';
	      c = *string;	/* tested at add_string */
	      if (timefmt[0] == '\0')
		{
		  timefmt[0] = '%';
		  timefmt[1] = 'X';	/* locale-specific current time */
		  timefmt[2] = '\0';
		}
	      n = strftime (timebuf, sizeof (timebuf), timefmt, tm);
	      free (timefmt);

	      if (n == 0)
		timebuf[0] = '\0';
	      else
		timebuf[sizeof(timebuf) - 1] = '\0';

	      if (promptvars || posixly_correct)
		/* Make sure that expand_prompt_string is called with a
		   second argument of Q_DOUBLE_QUOTES if we use this
		   function here. */
		temp = sh_backslash_quote_for_double_quotes (timebuf);
	      else
		temp = savestring (timebuf);
	      goto add_string;
	      
	    case 'n':
	      temp = (char *)xmalloc (3);
	      temp[0] = no_line_editing ? '\n' : '\r';
	      temp[1] = no_line_editing ? '\0' : '\n';
	      temp[2] = '\0';
	      goto add_string;

	    case 's':
	      temp = base_pathname (shell_name);
	      temp = savestring (temp);
	      goto add_string;

	    case 'v':
	    case 'V':
	      temp = (char *)xmalloc (16);
	      if (c == 'v')
		strcpy (temp, dist_version);
	      else
		sprintf (temp, "%s.%d", dist_version, patch_level);
	      goto add_string;

	    case 'w':
	    case 'W':
	      {
		/* Use the value of PWD because it is much more efficient. */
		char t_string[PATH_MAX];
		int tlen;

		temp = get_string_value ("PWD");

		if (temp == 0)
		  {
		    if (getcwd (t_string, sizeof(t_string)) == 0)
		      {
			t_string[0] = '.';
			tlen = 1;
		      }
		    else
		      tlen = strlen (t_string);
		  }
		else
		  {
		    tlen = sizeof (t_string) - 1;
		    strncpy (t_string, temp, tlen);
		  }
		t_string[tlen] = '\0';

#if defined (MACOSX)
		/* Convert from "fs" format to "input" format */
		temp = fnx_fromfs (t_string, strlen (t_string));
		if (temp != t_string)
		  strcpy (t_string, temp);
#endif

#define ROOT_PATH(x)	((x)[0] == '/' && (x)[1] == 0)
#define DOUBLE_SLASH_ROOT(x)	((x)[0] == '/' && (x)[1] == '/' && (x)[2] == 0)
		/* Abbreviate \W as ~ if $PWD == $HOME */
		if (c == 'W' && (((t = get_string_value ("HOME")) == 0) || STREQ (t, t_string) == 0))
		  {
		    if (ROOT_PATH (t_string) == 0 && DOUBLE_SLASH_ROOT (t_string) == 0)
		      {
			t = strrchr (t_string, '/');
			if (t)
			  memmove (t_string, t + 1, strlen (t));	/* strlen(t) to copy NULL */
		      }
		  }
#undef ROOT_PATH
#undef DOUBLE_SLASH_ROOT
		else
		  /* polite_directory_format is guaranteed to return a string
		     no longer than PATH_MAX - 1 characters. */
		  strcpy (t_string, polite_directory_format (t_string));

		temp = trim_pathname (t_string, PATH_MAX - 1);
		/* If we're going to be expanding the prompt string later,
		   quote the directory name. */
		if (promptvars || posixly_correct)
		  /* Make sure that expand_prompt_string is called with a
		     second argument of Q_DOUBLE_QUOTES if we use this
		     function here. */
		  temp = sh_backslash_quote_for_double_quotes (t_string);
		else
		  temp = savestring (t_string);

		goto add_string;
	      }

	    case 'u':
	      if (current_user.user_name == 0)
		get_current_user_info ();
	      temp = savestring (current_user.user_name);
	      goto add_string;

	    case 'h':
	    case 'H':
	      temp = savestring (current_host_name);
	      if (c == 'h' && (t = (char *)strchr (temp, '.')))
		*t = '\0';
	      goto add_string;

	    case '#':
	      temp = itos (current_command_number);
	      goto add_string;

	    case '!':
#if !defined (HISTORY)
	      temp = savestring ("1");
#else /* HISTORY */
	      temp = itos (history_number ());
#endif /* HISTORY */
	      goto add_string;

	    case '$':
	      t = temp = (char *)xmalloc (3);
	      if ((promptvars || posixly_correct) && (current_user.euid != 0))
		*t++ = '\\';
	      *t++ = current_user.euid == 0 ? '#' : '$';
	      *t = '\0';
	      goto add_string;

	    case 'j':
	      temp = itos (count_all_jobs ());
	      goto add_string;

	    case 'l':
#if defined (HAVE_TTYNAME)
	      temp = (char *)ttyname (fileno (stdin));
	      t = temp ? base_pathname (temp) : "tty";
	      temp = savestring (t);
#else
	      temp = savestring ("tty");
#endif /* !HAVE_TTYNAME */
	      goto add_string;

#if defined (READLINE)
	    case '[':
	    case ']':
	      if (no_line_editing)
		{
		  string++;
		  break;
		}
	      temp = (char *)xmalloc (3);
	      n = (c == '[') ? RL_PROMPT_START_IGNORE : RL_PROMPT_END_IGNORE;
	      i = 0;
	      if (n == CTLESC || n == CTLNUL)
		temp[i++] = CTLESC;
	      temp[i++] = n;
	      temp[i] = '\0';
	      goto add_string;
#endif /* READLINE */

	    case '\\':
	    case 'a':
	    case 'e':
	    case 'r':
	      temp = (char *)xmalloc (2);
	      if (c == 'a')
#ifndef __MVS__
		temp[0] = '\07';
#else
		temp[0] = '\57';
#endif
	      else if (c == 'e')
#ifndef __MVS__
		temp[0] = '\033';
#else
		temp[0] = '\47';
#endif
	      else if (c == 'r')
		temp[0] = '\r';
	      else			/* (c == '\\') */
	        temp[0] = c;
	      temp[1] = '\0';
	      goto add_string;

	    default:
not_escape:
	      temp = (char *)xmalloc (3);
	      temp[0] = '\\';
	      temp[1] = c;
	      temp[2] = '\0';

	    add_string:
	      if (c)
		string++;
	      result =
		sub_append_string (temp, result, &result_index, &result_size);
	      temp = (char *)NULL; /* Freed in sub_append_string (). */
	      result[result_index] = '\0';
	      break;
	    }
	}
      else
	{
	  RESIZE_MALLOCED_BUFFER (result, result_index, 3, result_size, PROMPT_GROWTH);
	  result[result_index++] = c;
	  result[result_index] = '\0';
	}
    }
#else /* !PROMPT_STRING_DECODE */
  result = savestring (string);
#endif /* !PROMPT_STRING_DECODE */

  /* Save the delimiter stack and point `dstack' to temp space so any
     command substitutions in the prompt string won't result in screwing
     up the parser's quoting state. */
  save_dstack = dstack;
  dstack = temp_dstack;
  dstack.delimiter_depth = 0;

  /* Perform variable and parameter expansion and command substitution on
     the prompt string. */
  if (promptvars || posixly_correct)
    {
      last_exit_value = last_command_exit_value;
      last_comsub_pid = last_command_subst_pid;
      list = expand_prompt_string (result, Q_DOUBLE_QUOTES, 0);
      free (result);
      result = string_list (list);
      dispose_words (list);
      last_command_exit_value = last_exit_value;
      last_command_subst_pid = last_comsub_pid;
    }
  else
    {
      t = dequote_string (result);
      free (result);
      result = t;
    }

  dstack = save_dstack;

  return (result);
}

/************************************************
 *						*
 *		ERROR HANDLING			*
 *						*
 ************************************************/

/* Report a syntax error, and restart the parser.  Call here for fatal
   errors. */
int
yyerror (msg)
     const char *msg;
{
  report_syntax_error ((char *)NULL);
  reset_parser ();
  return (0);
}

static char *
error_token_from_token (tok)
     int tok;
{
  char *t;

  if (t = find_token_in_alist (tok, word_token_alist, 0))
    return t;

  if (t = find_token_in_alist (tok, other_token_alist, 0))
    return t;

  t = (char *)NULL;
  /* This stuff is dicy and needs closer inspection */
  switch (current_token)
    {
    case WORD:
    case ASSIGNMENT_WORD:
      if (yylval.word)
	t = savestring (yylval.word->word);
      break;
    case NUMBER:
      t = itos (yylval.number);
      break;
    case ARITH_CMD:
      if (yylval.word_list)
        t = string_list (yylval.word_list);
      break;
    case ARITH_FOR_EXPRS:
      if (yylval.word_list)
	t = string_list_internal (yylval.word_list, " ; ");
      break;
    case COND_CMD:
      t = (char *)NULL;		/* punt */
      break;
    }

  return t;
}

static char *
error_token_from_text ()
{
  char *msg, *t;
  int token_end, i;

  t = shell_input_line;
  i = shell_input_line_index;
  token_end = 0;
  msg = (char *)NULL;

  if (i && t[i] == '\0')
    i--;

  while (i && (whitespace (t[i]) || t[i] == '\n'))
    i--;

  if (i)
    token_end = i + 1;

  while (i && (member (t[i], " \n\t;|&") == 0))
    i--;

  while (i != token_end && (whitespace (t[i]) || t[i] == '\n'))
    i++;

  /* Return our idea of the offending token. */
  if (token_end || (i == 0 && token_end == 0))
    {
      if (token_end)
	msg = substring (t, i, token_end);
      else	/* one-character token */
	{
	  msg = (char *)xmalloc (2);
	  msg[0] = t[i];
	  msg[1] = '\0';
	}
    }

  return (msg);
}

static void
print_offending_line ()
{
  char *msg;
  int token_end;

  msg = savestring (shell_input_line);
  token_end = strlen (msg);
  while (token_end && msg[token_end - 1] == '\n')
    msg[--token_end] = '\0';

  parser_error (line_number, "`%s'", msg);
  free (msg);
}

/* Report a syntax error with line numbers, etc.
   Call here for recoverable errors.  If you have a message to print,
   then place it in MESSAGE, otherwise pass NULL and this will figure
   out an appropriate message for you. */
static void
report_syntax_error (message)
     char *message;
{
  char *msg, *p;

  if (message)
    {
      parser_error (line_number, "%s", message);
      if (interactive && EOF_Reached)
	EOF_Reached = 0;
      last_command_exit_value = parse_and_execute_level ? EX_BADSYNTAX : EX_BADUSAGE;
      return;
    }

  /* If the line of input we're reading is not null, try to find the
     objectionable token.  First, try to figure out what token the
     parser's complaining about by looking at current_token. */
  if (current_token != 0 && EOF_Reached == 0 && (msg = error_token_from_token (current_token)))
    {
      if (ansic_shouldquote (msg))
	{
	  p = ansic_quote (msg, 0, NULL);
	  free (msg);
	  msg = p;
	}
      parser_error (line_number, _("syntax error near unexpected token `%s'"), msg);
      free (msg);

      if (interactive == 0)
	print_offending_line ();

      last_command_exit_value = parse_and_execute_level ? EX_BADSYNTAX : EX_BADUSAGE;
      return;
    }

  /* If looking at the current token doesn't prove fruitful, try to find the
     offending token by analyzing the text of the input line near the current
     input line index and report what we find. */
  if (shell_input_line && *shell_input_line)
    {
      msg = error_token_from_text ();
      if (msg)
	{
	  parser_error (line_number, _("syntax error near `%s'"), msg);
	  free (msg);
	}

      /* If not interactive, print the line containing the error. */
      if (interactive == 0)
        print_offending_line ();
    }
  else
    {
      msg = EOF_Reached ? _("syntax error: unexpected end of file") : _("syntax error");
      parser_error (line_number, "%s", msg);
      /* When the shell is interactive, this file uses EOF_Reached
	 only for error reporting.  Other mechanisms are used to
	 decide whether or not to exit. */
      if (interactive && EOF_Reached)
	EOF_Reached = 0;
    }

  last_command_exit_value = parse_and_execute_level ? EX_BADSYNTAX : EX_BADUSAGE;
}

/* ??? Needed function. ??? We have to be able to discard the constructs
   created during parsing.  In the case of error, we want to return
   allocated objects to the memory pool.  In the case of no error, we want
   to throw away the information about where the allocated objects live.
   (dispose_command () will actually free the command.) */
static void
discard_parser_constructs (error_p)
     int error_p;
{
}

/************************************************
 *						*
 *		EOF HANDLING			*
 *						*
 ************************************************/

/* Do that silly `type "bye" to exit' stuff.  You know, "ignoreeof". */

/* A flag denoting whether or not ignoreeof is set. */
int ignoreeof = 0;

/* The number of times that we have encountered an EOF character without
   another character intervening.  When this gets above the limit, the
   shell terminates. */
int eof_encountered = 0;

/* The limit for eof_encountered. */
int eof_encountered_limit = 10;

/* If we have EOF as the only input unit, this user wants to leave
   the shell.  If the shell is not interactive, then just leave.
   Otherwise, if ignoreeof is set, and we haven't done this the
   required number of times in a row, print a message. */
static void
handle_eof_input_unit ()
{
  if (interactive)
    {
      /* shell.c may use this to decide whether or not to write out the
	 history, among other things.  We use it only for error reporting
	 in this file. */
      if (EOF_Reached)
	EOF_Reached = 0;

      /* If the user wants to "ignore" eof, then let her do so, kind of. */
      if (ignoreeof)
	{
	  if (eof_encountered < eof_encountered_limit)
	    {
	      fprintf (stderr, _("Use \"%s\" to leave the shell.\n"),
		       login_shell ? "logout" : "exit");
	      eof_encountered++;
	      /* Reset the parsing state. */
	      last_read_token = current_token = '\n';
	      /* Reset the prompt string to be $PS1. */
	      prompt_string_pointer = (char **)NULL;
	      prompt_again ();
	      return;
	    }
	}

      /* In this case EOF should exit the shell.  Do it now. */
      reset_parser ();
      exit_builtin ((WORD_LIST *)NULL);
    }
  else
    {
      /* We don't write history files, etc., for non-interactive shells. */
      EOF_Reached = 1;
    }
}

/************************************************
 *						*
 *	STRING PARSING FUNCTIONS		*
 *						*
 ************************************************/

/* It's very important that these two functions treat the characters
   between ( and ) identically. */

static WORD_LIST parse_string_error;

/* Take a string and run it through the shell parser, returning the
   resultant word list.  Used by compound array assignment. */
WORD_LIST *
parse_string_to_word_list (s, flags, whom)
     char *s;
     int flags;
     const char *whom;
{
  WORD_LIST *wl;
  int tok, orig_current_token, orig_line_number, orig_input_terminator;
  int orig_line_count;
  int old_echo_input, old_expand_aliases;
#if defined (HISTORY)
  int old_remember_on_history, old_history_expansion_inhibited;
#endif

#if defined (HISTORY)
  old_remember_on_history = remember_on_history;
#  if defined (BANG_HISTORY)
  old_history_expansion_inhibited = history_expansion_inhibited;
#  endif
  bash_history_disable ();
#endif

  orig_line_number = line_number;
  orig_line_count = current_command_line_count;
  orig_input_terminator = shell_input_line_terminator;
  old_echo_input = echo_input_at_read;
  old_expand_aliases = expand_aliases;

  push_stream (1);
  last_read_token = WORD;		/* WORD to allow reserved words here */
  current_command_line_count = 0;
  echo_input_at_read = expand_aliases = 0;

  with_input_from_string (s, whom);
  wl = (WORD_LIST *)NULL;

  if (flags & 1)
    parser_state |= PST_COMPASSIGN|PST_REPARSE;

  while ((tok = read_token (READ)) != yacc_EOF)
    {
      if (tok == '\n' && *bash_input.location.string == '\0')
	break;
      if (tok == '\n')		/* Allow newlines in compound assignments */
	continue;
      if (tok != WORD && tok != ASSIGNMENT_WORD)
	{
	  line_number = orig_line_number + line_number - 1;
	  orig_current_token = current_token;
	  current_token = tok;
	  yyerror (NULL);	/* does the right thing */
	  current_token = orig_current_token;
	  if (wl)
	    dispose_words (wl);
	  wl = &parse_string_error;
	  break;
	}
      wl = make_word_list (yylval.word, wl);
    }
  
  last_read_token = '\n';
  pop_stream ();

#if defined (HISTORY)
  remember_on_history = old_remember_on_history;
#  if defined (BANG_HISTORY)
  history_expansion_inhibited = old_history_expansion_inhibited;
#  endif /* BANG_HISTORY */
#endif /* HISTORY */

  echo_input_at_read = old_echo_input;
  expand_aliases = old_expand_aliases;

  current_command_line_count = orig_line_count;
  shell_input_line_terminator = orig_input_terminator;

  if (flags & 1)
    parser_state &= ~(PST_COMPASSIGN|PST_REPARSE);

  if (wl == &parse_string_error)
    {
      last_command_exit_value = EXECUTION_FAILURE;
      if (interactive_shell == 0 && posixly_correct)
	jump_to_top_level (FORCE_EOF);
      else
	jump_to_top_level (DISCARD);
    }

  return (REVERSE_LIST (wl, WORD_LIST *));
}

static char *
parse_compound_assignment (retlenp)
     int *retlenp;
{
  WORD_LIST *wl, *rl;
  int tok, orig_line_number, orig_token_size, orig_last_token, assignok;
  char *saved_token, *ret;

  saved_token = token;
  orig_token_size = token_buffer_size;
  orig_line_number = line_number;
  orig_last_token = last_read_token;

  last_read_token = WORD;	/* WORD to allow reserved words here */

  token = (char *)NULL;
  token_buffer_size = 0;

  assignok = parser_state&PST_ASSIGNOK;		/* XXX */

  wl = (WORD_LIST *)NULL;	/* ( */
  parser_state |= PST_COMPASSIGN;

  while ((tok = read_token (READ)) != ')')
    {
      if (tok == '\n')			/* Allow newlines in compound assignments */
	{
	  if (SHOULD_PROMPT ())
	    prompt_again ();
	  continue;
	}
      if (tok != WORD && tok != ASSIGNMENT_WORD)
	{
	  current_token = tok;	/* for error reporting */
	  if (tok == yacc_EOF)	/* ( */
	    parser_error (orig_line_number, _("unexpected EOF while looking for matching `)'"));
	  else
	    yyerror(NULL);	/* does the right thing */
	  if (wl)
	    dispose_words (wl);
	  wl = &parse_string_error;
	  break;
	}
      wl = make_word_list (yylval.word, wl);
    }

  FREE (token);
  token = saved_token;
  token_buffer_size = orig_token_size;

  parser_state &= ~PST_COMPASSIGN;

  if (wl == &parse_string_error)
    {
      last_command_exit_value = EXECUTION_FAILURE;
      last_read_token = '\n';	/* XXX */
      if (interactive_shell == 0 && posixly_correct)
	jump_to_top_level (FORCE_EOF);
      else
	jump_to_top_level (DISCARD);
    }

  last_read_token = orig_last_token;		/* XXX - was WORD? */

  if (wl)
    {
      rl = REVERSE_LIST (wl, WORD_LIST *);
      ret = string_list (rl);
      dispose_words (rl);
    }
  else
    ret = (char *)NULL;

  if (retlenp)
    *retlenp = (ret && *ret) ? strlen (ret) : 0;

  if (assignok)
    parser_state |= PST_ASSIGNOK;

  return ret;
}

/************************************************
 *						*
 *   SAVING AND RESTORING PARTIAL PARSE STATE   *
 *						*
 ************************************************/

sh_parser_state_t *
save_parser_state (ps)
     sh_parser_state_t *ps;
{
  if (ps == 0)
    ps = (sh_parser_state_t *)xmalloc (sizeof (sh_parser_state_t));
  if (ps == 0)
    return ((sh_parser_state_t *)NULL);

  ps->parser_state = parser_state;
  ps->token_state = save_token_state ();

  ps->input_line_terminator = shell_input_line_terminator;
  ps->eof_encountered = eof_encountered;

  ps->prompt_string_pointer = prompt_string_pointer;

  ps->current_command_line_count = current_command_line_count;

#if defined (HISTORY)
  ps->remember_on_history = remember_on_history;
#  if defined (BANG_HISTORY)
  ps->history_expansion_inhibited = history_expansion_inhibited;
#  endif
#endif

  ps->last_command_exit_value = last_command_exit_value;
#if defined (ARRAY_VARS)
  ps->pipestatus = save_pipestatus_array ();
#endif
    
  ps->last_shell_builtin = last_shell_builtin;
  ps->this_shell_builtin = this_shell_builtin;

  ps->expand_aliases = expand_aliases;
  ps->echo_input_at_read = echo_input_at_read;

  ps->token = token;
  ps->token_buffer_size = token_buffer_size;
  /* Force reallocation on next call to read_token_word */
  token = 0;
  token_buffer_size = 0;

  return (ps);
}

void
restore_parser_state (ps)
     sh_parser_state_t *ps;
{
  if (ps == 0)
    return;

  parser_state = ps->parser_state;
  if (ps->token_state)
    {
      restore_token_state (ps->token_state);
      free (ps->token_state);
    }

  shell_input_line_terminator = ps->input_line_terminator;
  eof_encountered = ps->eof_encountered;

  prompt_string_pointer = ps->prompt_string_pointer;

  current_command_line_count = ps->current_command_line_count;

#if defined (HISTORY)
  remember_on_history = ps->remember_on_history;
#  if defined (BANG_HISTORY)
  history_expansion_inhibited = ps->history_expansion_inhibited;
#  endif
#endif

  last_command_exit_value = ps->last_command_exit_value;
#if defined (ARRAY_VARS)
  restore_pipestatus_array (ps->pipestatus);
#endif

  last_shell_builtin = ps->last_shell_builtin;
  this_shell_builtin = ps->this_shell_builtin;

  expand_aliases = ps->expand_aliases;
  echo_input_at_read = ps->echo_input_at_read;

  FREE (token);
  token = ps->token;
  token_buffer_size = ps->token_buffer_size;
}

sh_input_line_state_t *
save_input_line_state (ls)
     sh_input_line_state_t *ls;
{
  if (ls == 0)
    ls = (sh_input_line_state_t *)xmalloc (sizeof (sh_input_line_state_t));
  if (ls == 0)
    return ((sh_input_line_state_t *)NULL);

  ls->input_line = shell_input_line;
  ls->input_line_size = shell_input_line_size;
  ls->input_line_len = shell_input_line_len;
  ls->input_line_index = shell_input_line_index;

  /* force reallocation */
  shell_input_line = 0;
  shell_input_line_size = shell_input_line_len = shell_input_line_index = 0;
}

void
restore_input_line_state (ls)
     sh_input_line_state_t *ls;
{
  FREE (shell_input_line);
  shell_input_line = ls->input_line;
  shell_input_line_size = ls->input_line_size;
  shell_input_line_len = ls->input_line_len;
  shell_input_line_index = ls->input_line_index;

  set_line_mbstate ();
}

/************************************************
 *						*
 *	MULTIBYTE CHARACTER HANDLING		*
 *						*
 ************************************************/

#if defined (HANDLE_MULTIBYTE)
static void
set_line_mbstate ()
{
  int i, previ, len, c;
  mbstate_t mbs, prevs;
  size_t mbclen;

  if (shell_input_line == NULL)
    return;
  len = strlen (shell_input_line);	/* XXX - shell_input_line_len ? */
  FREE (shell_input_line_property);
  shell_input_line_property = (char *)xmalloc (len + 1);

  memset (&prevs, '\0', sizeof (mbstate_t));
  for (i = previ = 0; i < len; i++)
    {
      mbs = prevs;

      c = shell_input_line[i];
      if (c == EOF)
	{
	  int j;
	  for (j = i; j < len; j++)
	    shell_input_line_property[j] = 1;
	  break;
	}

      mbclen = mbrlen (shell_input_line + previ, i - previ + 1, &mbs);
      if (mbclen == 1 || mbclen == (size_t)-1)
	{
	  mbclen = 1;
	  previ = i + 1;
	}
      else if (mbclen == (size_t)-2)
        mbclen = 0;
      else if (mbclen > 1)
	{
	  mbclen = 0;
	  previ = i + 1;
	  prevs = mbs;
	}
      else
	{
	  /* XXX - what to do if mbrlen returns 0? (null wide character) */
	  int j;
	  for (j = i; j < len; j++)
	    shell_input_line_property[j] = 1;
	  break;
	}

      shell_input_line_property[i] = mbclen;
    }
}
#endif /* HANDLE_MULTIBYTE */

 
yyparse()
{
 register short    *yyp;       /* for table lookup */
 register short    *yyq;
 register short    yyi;
 register short    *yyps;      /* top of state stack */
 register short    yystate;    /* current state */
 register YYSTYPE  *yypv;      /* top of value stack */
 register int      yyj;
 
#if YYDEBUG
  yyTraceItems     yyx;        /* trace block */
  short            *yytp;
  int              yyruletype = 0;
#endif
 
#ifdef YYSTATIC
  static short     yys[YYSSIZE + 1];
  static YYSTYPE   yyv[YYSSIZE + 1];
 
  #if YYDEBUG
     static short  yytypev[YYSSIZE+1];  /* type assignments */
  #endif
#else /* ! YYSTATIC */
  #ifdef YYALLOC
  YYSTYPE        *yyv;
  short          *yys;
    #if YYDEBUG
      short      *yytypev;
    #endif
    YYSTYPE save_yylval;
    YYSTYPE save_yyval;
    YYSTYPE *save_yypvt;
    int save_yychar, save_yyerrflag, save_yynerrs;
    int retval;          /* return value holder */
  #else
    short    yys[YYSSIZE + 1];
    static YYSTYPE yyv[YYSSIZE + 1]; /* historically static */
    #if YYDEBUG
      short yytypev[YYSSIZE+1];     /* mirror type table */
    #endif
    #endif /* ! YYALLOC */
#endif /* ! YYSTATIC */
 
#ifdef YYDYNAMIC
  char *envp;
#endif
 

#ifdef YYDYNAMIC
  if ((envp = getenv("YYSTACKSIZE")) != (char *)0)
    {
     yyssize = atoi(envp);
     if (yyssize <= 0)
       yyssize = YYSSIZE;
    }
  if ((envp = getenv("YYSTACKINC")) != (char *)0)
    yysinc = atoi(envp);
#endif
 
#ifdef YYALLOC
  yys = (short *) malloc((yyssize + 1) * sizeof(short));
  yyv = (YYSTYPE *) malloc((yyssize + 1) * sizeof(YYSTYPE));
 
  #if YYDEBUG
    yytypev = (short *) malloc((yyssize + 1) * sizeof(short));
  #endif
 
  if (yys == (short *)0 || yyv == (YYSTYPE *)0
  #if YYDEBUG
      || yytypev == (short *) 0
  #endif
   ) {
      yyerror(m_textmsg(4967, "Not enough space for parser stacks",
              "E"));
      return 1;
   }
 
  save_yylval = yylval;
  save_yyval = yyval;
  save_yypvt = yypvt;
  save_yychar = yychar;
  save_yyerrflag = yyerrflag;
  save_yynerrs = yynerrs;
#endif
 
  yynerrs = 0;
  yyerrflag = 0;
  yyclearin;
  yyps = yys;
  yypv = yyv;
  *yyps = yystate = YYS0;    /* start state */
 
#if YYDEBUG
  yytp = yytypev;
  yyi = yyj = 0;       /* silence compiler warnings */
#endif
 
yyStack:
   yyassert((unsigned)yystate < yynstate, m_textmsg(587, "state %d\n", ""), yystate);
 
#ifdef YYDYNAMIC
  if (++yyps > &yys[yyssize])
    {
     int yynewsize;
     long yysindex = yyps - yys;
     long yyvindex = yypv - yyv;
 
  #if YYDEBUG
     int yytindex = yytp - yytypev;
  #endif
 
     if (yysinc == 0)        /* no increment */
       {
        yyerror(m_textmsg(4968, "Parser stack overflow", "E"));
        YYABORT;
       }
      else if (yysinc < 0)                /* binary-exponential */
             yynewsize = yyssize * 2;
            else                          /* fixed increment */
             yynewsize = yyssize + yysinc;
     if (yynewsize < yyssize)
       {
        yyerror(m_textmsg(4967, "Not enough space for parser stacks", "E"));
        YYABORT;
       }
     yyssize = yynewsize;
     yys = (short *) realloc(yys, (yyssize + 1) * sizeof(short));
     yyps = yys + yysindex;
     yyv = (YYSTYPE *) realloc(yyv, (yyssize + 1) * sizeof(YYSTYPE));
     yypv = yyv + yyvindex;
 
#if YYDEBUG
     yytypev = (short *)realloc(yytypev,(yyssize + 1)*sizeof(short));
     yytp = yytypev + yytindex;
#endif
 
     if (yys == (short *)0 || yyv == (YYSTYPE *)0
#if YYDEBUG
     || yytypev == (short *) 0
#endif
      ) {
         yyerror(m_textmsg(4967, "Not enough space for parser stacks", "E"));
         YYABORT;
        }
    }
 
#else  /* ! YYDYNAMIC */
  if (++yyps > &yys[YYSSIZE])
    {
     yyerror(m_textmsg(4968, "Parser stack overflow", "E"));
     YYABORT;
    }
#endif /* !YYDYNAMIC */
 
  *yyps = yystate;  /* stack current state */
  *++yypv = yyval;  /* ... and value */
 
#if YYDEBUG
  *++yytp = yyruletype;   /* ... and type */
  if (yydebug)
    YY_TRACE(yyShowState)
#endif
 
/*--------------------------------------------------------------------*/
/* Look up next action in action table.                               */
/*--------------------------------------------------------------------*/
yyEncore:
 
#ifdef YYSYNC
  YYREAD;
#endif
 
  if (yystate >= sizeof yypact/sizeof yypact[0])  /* simple state */
     yyi = yystate - YYDELTA;   /* reduce in any case */
   else
    {
     if (*(yyp = &yyact[yypact[yystate]]) >= 0)
       {
        /* Look for a shift on yychar */
#ifndef YYSYNC
        YYREAD;
#endif
        yyq = yyp;
        yyi = yychar;
        while (yyi < *yyp++)
            ;
        if (yyi == yyp[-1])
          {
           yystate = yyneg(YYQYYP);
#if YYDEBUG
           if (yydebug)
             {
              yyruletype = yyGetType(yychar);
              YY_TRACE(yyShowShift)
             }
#endif
           yyval = yylval;   /* stack what yylex() set */
           yyclearin;     /* clear token */
           if (yyerrflag)
             yyerrflag--;   /* successful shift */
           goto yyStack;
          }
       }
 
     /*-----------------------------------------*/
     /*  Fell through - take default action     */
     /*-----------------------------------------*/
     if (yystate >= sizeof yydef /sizeof yydef[0])
       goto yyError;
     if ((yyi = yydef[yystate]) < 0)    /* default == reduce? */
       {
        /* Search exception table */
        yyassert((unsigned)yyneg(yyi) < sizeof yyex/sizeof yyex[0],
                 m_textmsg(2825, "exception %d\n", "I num"), yystate);
        yyp = &yyex[yyneg(yyi)];
 
#ifndef YYSYNC
        YYREAD;
#endif
 
        while ((yyi = *yyp) >= 0 && yyi != yychar)
             yyp += 2;
        yyi = yyp[1];
        yyassert(yyi >= 0,
                 m_textmsg(2826, "Ex table not reduce %d\n", "I num"), yyi);
       }
    }
 
  yyassert((unsigned)yyi < yynrule, m_textmsg(2827, "reduce %d\n", "I num"), yyi);
  yyj = yyrlen[yyi];
 
#if YYDEBUG
  if (yydebug)
    YY_TRACE(yyShowReduce)
  yytp -= yyj;
#endif
 
  yyps -= yyj;      /* pop stacks */
  yypvt = yypv;     /* save top */
  yypv -= yyj;
  yyval = yypv[1];  /* default action $ = $1 */
 
#if YYDEBUG
   yyruletype = yyRules[yyrmap[yyi]].type;
#endif
 
   switch (yyi)     /* perform semantic action */
     {
      
case YYr1: {	/* inputunit :  simple_list simple_list_terminator */
#line 382 "./parse.y"

			  
			  global_command = yypvt[-1].command;
			  eof_encountered = 0;
			  
			  if (parser_state & PST_CMDSUBST)
			    parser_state |= PST_EOFTOKEN;
			  YYACCEPT;
			
} break;

case YYr2: {	/* inputunit :  '\n' */
#line 393 "./parse.y"

			  
			  global_command = (COMMAND *)NULL;
			  if (parser_state & PST_CMDSUBST)
			    parser_state |= PST_EOFTOKEN;
			  YYACCEPT;
			
} break;

case YYr3: {	/* inputunit :  error '\n' */
#line 402 "./parse.y"

			  
			  global_command = (COMMAND *)NULL;
			  eof_encountered = 0;
			  
			  if (interactive && parse_and_execute_level == 0)
			    {
			      YYACCEPT;
			    }
			  else
			    {
			      YYABORT;
			    }
			
} break;

case YYr4: {	/* inputunit :  yacc_EOF */
#line 417 "./parse.y"

			  
			  global_command = (COMMAND *)NULL;
			  handle_eof_input_unit ();
			  YYACCEPT;
			
} break;

case YYr5: {	/* word_list :  WORD */
#line 427 "./parse.y"
 yyval.word_list = make_word_list (yypvt[0].word, (WORD_LIST *)NULL); 
} break;

case YYr6: {	/* word_list :  word_list WORD */
#line 429 "./parse.y"
 yyval.word_list = make_word_list (yypvt[0].word, yypvt[-1].word_list); 
} break;

case YYr7: {	/* redirection :  '>' WORD */
#line 433 "./parse.y"

			  source.dest = 1;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_output_direction, redir, 0);
			
} break;

case YYr8: {	/* redirection :  '<' WORD */
#line 439 "./parse.y"

			  source.dest = 0;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_input_direction, redir, 0);
			
} break;

case YYr9: {	/* redirection :  NUMBER '>' WORD */
#line 445 "./parse.y"

			  source.dest = yypvt[-2].number;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_output_direction, redir, 0);
			
} break;

case YYr10: {	/* redirection :  NUMBER '<' WORD */
#line 451 "./parse.y"

			  source.dest = yypvt[-2].number;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_input_direction, redir, 0);
			
} break;

case YYr11: {	/* redirection :  REDIR_WORD '>' WORD */
#line 457 "./parse.y"

			  source.filename = yypvt[-2].word;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_output_direction, redir, REDIR_VARASSIGN);
			
} break;

case YYr12: {	/* redirection :  REDIR_WORD '<' WORD */
#line 463 "./parse.y"

			  source.filename = yypvt[-2].word;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_input_direction, redir, REDIR_VARASSIGN);
			
} break;

case YYr13: {	/* redirection :  GREATER_GREATER WORD */
#line 469 "./parse.y"

			  source.dest = 1;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_appending_to, redir, 0);
			
} break;

case YYr14: {	/* redirection :  NUMBER GREATER_GREATER WORD */
#line 475 "./parse.y"

			  source.dest = yypvt[-2].number;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_appending_to, redir, 0);
			
} break;

case YYr15: {	/* redirection :  REDIR_WORD GREATER_GREATER WORD */
#line 481 "./parse.y"

			  source.filename = yypvt[-2].word;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_appending_to, redir, REDIR_VARASSIGN);
			
} break;

case YYr16: {	/* redirection :  GREATER_BAR WORD */
#line 487 "./parse.y"

			  source.dest = 1;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_output_force, redir, 0);
			
} break;

case YYr17: {	/* redirection :  NUMBER GREATER_BAR WORD */
#line 493 "./parse.y"

			  source.dest = yypvt[-2].number;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_output_force, redir, 0);
			
} break;

case YYr18: {	/* redirection :  REDIR_WORD GREATER_BAR WORD */
#line 499 "./parse.y"

			  source.filename = yypvt[-2].word;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_output_force, redir, REDIR_VARASSIGN);
			
} break;

case YYr19: {	/* redirection :  LESS_GREATER WORD */
#line 505 "./parse.y"

			  source.dest = 0;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_input_output, redir, 0);
			
} break;

case YYr20: {	/* redirection :  NUMBER LESS_GREATER WORD */
#line 511 "./parse.y"

			  source.dest = yypvt[-2].number;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_input_output, redir, 0);
			
} break;

case YYr21: {	/* redirection :  REDIR_WORD LESS_GREATER WORD */
#line 517 "./parse.y"

			  source.filename = yypvt[-2].word;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_input_output, redir, REDIR_VARASSIGN);
			
} break;

case YYr22: {	/* redirection :  LESS_LESS WORD */
#line 523 "./parse.y"

			  source.dest = 0;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_reading_until, redir, 0);
			  push_heredoc (yyval.redirect);
			
} break;

case YYr23: {	/* redirection :  NUMBER LESS_LESS WORD */
#line 530 "./parse.y"

			  source.dest = yypvt[-2].number;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_reading_until, redir, 0);
			  push_heredoc (yyval.redirect);
			
} break;

case YYr24: {	/* redirection :  REDIR_WORD LESS_LESS WORD */
#line 537 "./parse.y"

			  source.filename = yypvt[-2].word;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_reading_until, redir, REDIR_VARASSIGN);
			  push_heredoc (yyval.redirect);
			
} break;

case YYr25: {	/* redirection :  LESS_LESS_MINUS WORD */
#line 544 "./parse.y"

			  source.dest = 0;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_deblank_reading_until, redir, 0);
			  push_heredoc (yyval.redirect);
			
} break;

case YYr26: {	/* redirection :  NUMBER LESS_LESS_MINUS WORD */
#line 551 "./parse.y"

			  source.dest = yypvt[-2].number;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_deblank_reading_until, redir, 0);
			  push_heredoc (yyval.redirect);
			
} break;

case YYr27: {	/* redirection :  REDIR_WORD LESS_LESS_MINUS WORD */
#line 558 "./parse.y"

			  source.filename = yypvt[-2].word;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_deblank_reading_until, redir, REDIR_VARASSIGN);
			  push_heredoc (yyval.redirect);
			
} break;

case YYr28: {	/* redirection :  LESS_LESS_LESS WORD */
#line 565 "./parse.y"

			  source.dest = 0;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_reading_string, redir, 0);
			
} break;

case YYr29: {	/* redirection :  NUMBER LESS_LESS_LESS WORD */
#line 571 "./parse.y"

			  source.dest = yypvt[-2].number;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_reading_string, redir, 0);
			
} break;

case YYr30: {	/* redirection :  REDIR_WORD LESS_LESS_LESS WORD */
#line 577 "./parse.y"

			  source.filename = yypvt[-2].word;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_reading_string, redir, REDIR_VARASSIGN);
			
} break;

case YYr31: {	/* redirection :  LESS_AND NUMBER */
#line 583 "./parse.y"

			  source.dest = 0;
			  redir.dest = yypvt[0].number;
			  yyval.redirect = make_redirection (source, r_duplicating_input, redir, 0);
			
} break;

case YYr32: {	/* redirection :  NUMBER LESS_AND NUMBER */
#line 589 "./parse.y"

			  source.dest = yypvt[-2].number;
			  redir.dest = yypvt[0].number;
			  yyval.redirect = make_redirection (source, r_duplicating_input, redir, 0);
			
} break;

case YYr33: {	/* redirection :  REDIR_WORD LESS_AND NUMBER */
#line 595 "./parse.y"

			  source.filename = yypvt[-2].word;
			  redir.dest = yypvt[0].number;
			  yyval.redirect = make_redirection (source, r_duplicating_input, redir, REDIR_VARASSIGN);
			
} break;

case YYr34: {	/* redirection :  GREATER_AND NUMBER */
#line 601 "./parse.y"

			  source.dest = 1;
			  redir.dest = yypvt[0].number;
			  yyval.redirect = make_redirection (source, r_duplicating_output, redir, 0);
			
} break;

case YYr35: {	/* redirection :  NUMBER GREATER_AND NUMBER */
#line 607 "./parse.y"

			  source.dest = yypvt[-2].number;
			  redir.dest = yypvt[0].number;
			  yyval.redirect = make_redirection (source, r_duplicating_output, redir, 0);
			
} break;

case YYr36: {	/* redirection :  REDIR_WORD GREATER_AND NUMBER */
#line 613 "./parse.y"

			  source.filename = yypvt[-2].word;
			  redir.dest = yypvt[0].number;
			  yyval.redirect = make_redirection (source, r_duplicating_output, redir, REDIR_VARASSIGN);
			
} break;

case YYr37: {	/* redirection :  LESS_AND WORD */
#line 619 "./parse.y"

			  source.dest = 0;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_duplicating_input_word, redir, 0);
			
} break;

case YYr38: {	/* redirection :  NUMBER LESS_AND WORD */
#line 625 "./parse.y"

			  source.dest = yypvt[-2].number;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_duplicating_input_word, redir, 0);
			
} break;

case YYr39: {	/* redirection :  REDIR_WORD LESS_AND WORD */
#line 631 "./parse.y"

			  source.filename = yypvt[-2].word;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_duplicating_input_word, redir, REDIR_VARASSIGN);
			
} break;

case YYr40: {	/* redirection :  GREATER_AND WORD */
#line 637 "./parse.y"

			  source.dest = 1;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_duplicating_output_word, redir, 0);
			
} break;

case YYr41: {	/* redirection :  NUMBER GREATER_AND WORD */
#line 643 "./parse.y"

			  source.dest = yypvt[-2].number;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_duplicating_output_word, redir, 0);
			
} break;

case YYr42: {	/* redirection :  REDIR_WORD GREATER_AND WORD */
#line 649 "./parse.y"

			  source.filename = yypvt[-2].word;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_duplicating_output_word, redir, REDIR_VARASSIGN);
			
} break;

case YYr43: {	/* redirection :  GREATER_AND '-' */
#line 655 "./parse.y"

			  source.dest = 1;
			  redir.dest = 0;
			  yyval.redirect = make_redirection (source, r_close_this, redir, 0);
			
} break;

case YYr44: {	/* redirection :  NUMBER GREATER_AND '-' */
#line 661 "./parse.y"

			  source.dest = yypvt[-2].number;
			  redir.dest = 0;
			  yyval.redirect = make_redirection (source, r_close_this, redir, 0);
			
} break;

case YYr45: {	/* redirection :  REDIR_WORD GREATER_AND '-' */
#line 667 "./parse.y"

			  source.filename = yypvt[-2].word;
			  redir.dest = 0;
			  yyval.redirect = make_redirection (source, r_close_this, redir, REDIR_VARASSIGN);
			
} break;

case YYr46: {	/* redirection :  LESS_AND '-' */
#line 673 "./parse.y"

			  source.dest = 0;
			  redir.dest = 0;
			  yyval.redirect = make_redirection (source, r_close_this, redir, 0);
			
} break;

case YYr47: {	/* redirection :  NUMBER LESS_AND '-' */
#line 679 "./parse.y"

			  source.dest = yypvt[-2].number;
			  redir.dest = 0;
			  yyval.redirect = make_redirection (source, r_close_this, redir, 0);
			
} break;

case YYr48: {	/* redirection :  REDIR_WORD LESS_AND '-' */
#line 685 "./parse.y"

			  source.filename = yypvt[-2].word;
			  redir.dest = 0;
			  yyval.redirect = make_redirection (source, r_close_this, redir, REDIR_VARASSIGN);
			
} break;

case YYr49: {	/* redirection :  AND_GREATER WORD */
#line 691 "./parse.y"

			  source.dest = 1;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_err_and_out, redir, 0);
			
} break;

case YYr50: {	/* redirection :  AND_GREATER_GREATER WORD */
#line 697 "./parse.y"

			  source.dest = 1;
			  redir.filename = yypvt[0].word;
			  yyval.redirect = make_redirection (source, r_append_err_and_out, redir, 0);
			
} break;

case YYr51: {	/* simple_command_element :  WORD */
#line 705 "./parse.y"
 yyval.element.word = yypvt[0].word; yyval.element.redirect = 0; 
} break;

case YYr52: {	/* simple_command_element :  ASSIGNMENT_WORD */
#line 707 "./parse.y"
 yyval.element.word = yypvt[0].word; yyval.element.redirect = 0; 
} break;

case YYr53: {	/* simple_command_element :  redirection */
#line 709 "./parse.y"
 yyval.element.redirect = yypvt[0].redirect; yyval.element.word = 0; 
} break;

case YYr54: {	/* redirection_list :  redirection */
#line 713 "./parse.y"

			  yyval.redirect = yypvt[0].redirect;
			
} break;

case YYr55: {	/* redirection_list :  redirection_list redirection */
#line 717 "./parse.y"

			  register REDIRECT *t;

			  for (t = yypvt[-1].redirect; t->next; t = t->next)
			    ;
			  t->next = yypvt[0].redirect;
			  yyval.redirect = yypvt[-1].redirect;
			
} break;

case YYr56: {	/* simple_command :  simple_command_element */
#line 728 "./parse.y"
 yyval.command = make_simple_command (yypvt[0].element, (COMMAND *)NULL); 
} break;

case YYr57: {	/* simple_command :  simple_command simple_command_element */
#line 730 "./parse.y"
 yyval.command = make_simple_command (yypvt[0].element, yypvt[-1].command); 
} break;

case YYr58: {	/* command :  simple_command */
#line 734 "./parse.y"
 yyval.command = clean_simple_command (yypvt[0].command); 
} break;

case YYr59: {	/* command :  shell_command */
#line 736 "./parse.y"
 yyval.command = yypvt[0].command; 
} break;

case YYr60: {	/* command :  shell_command redirection_list */
#line 738 "./parse.y"

			  COMMAND *tc;

			  tc = yypvt[-1].command;
			  if (tc->redirects)
			    {
			      register REDIRECT *t;
			      for (t = tc->redirects; t->next; t = t->next)
				;
			      t->next = yypvt[0].redirect;
			    }
			  else
			    tc->redirects = yypvt[0].redirect;
			  yyval.command = yypvt[-1].command;
			
} break;

case YYr61: {	/* command :  function_def */
#line 754 "./parse.y"
 yyval.command = yypvt[0].command; 
} break;

case YYr62: {	/* command :  coproc */
#line 756 "./parse.y"
 yyval.command = yypvt[0].command; 
} break;

case YYr63: {	/* shell_command :  for_command */
#line 760 "./parse.y"
 yyval.command = yypvt[0].command; 
} break;

case YYr64: {	/* shell_command :  case_command */
#line 762 "./parse.y"
 yyval.command = yypvt[0].command; 
} break;

case YYr65: {	/* shell_command :  WHILE compound_list DO compound_list DONE */
#line 764 "./parse.y"
 yyval.command = make_while_command (yypvt[-3].command, yypvt[-1].command); 
} break;

case YYr66: {	/* shell_command :  UNTIL compound_list DO compound_list DONE */
#line 766 "./parse.y"
 yyval.command = make_until_command (yypvt[-3].command, yypvt[-1].command); 
} break;

case YYr67: {	/* shell_command :  select_command */
#line 768 "./parse.y"
 yyval.command = yypvt[0].command; 
} break;

case YYr68: {	/* shell_command :  if_command */
#line 770 "./parse.y"
 yyval.command = yypvt[0].command; 
} break;

case YYr69: {	/* shell_command :  subshell */
#line 772 "./parse.y"
 yyval.command = yypvt[0].command; 
} break;

case YYr70: {	/* shell_command :  group_command */
#line 774 "./parse.y"
 yyval.command = yypvt[0].command; 
} break;

case YYr71: {	/* shell_command :  arith_command */
#line 776 "./parse.y"
 yyval.command = yypvt[0].command; 
} break;

case YYr72: {	/* shell_command :  cond_command */
#line 778 "./parse.y"
 yyval.command = yypvt[0].command; 
} break;

case YYr73: {	/* shell_command :  arith_for_command */
#line 780 "./parse.y"
 yyval.command = yypvt[0].command; 
} break;

case YYr74: {	/* for_command :  FOR WORD newline_list DO compound_list DONE */
#line 784 "./parse.y"

			  yyval.command = make_for_command (yypvt[-4].word, add_string_to_list ("\"$@\"", (WORD_LIST *)NULL), yypvt[-1].command, word_lineno[word_top]);
			  if (word_top > 0) word_top--;
			
} break;

case YYr75: {	/* for_command :  FOR WORD newline_list '{' compound_list '}' */
#line 789 "./parse.y"

			  yyval.command = make_for_command (yypvt[-4].word, add_string_to_list ("\"$@\"", (WORD_LIST *)NULL), yypvt[-1].command, word_lineno[word_top]);
			  if (word_top > 0) word_top--;
			
} break;

case YYr76: {	/* for_command :  FOR WORD ';' newline_list DO compound_list DONE */
#line 794 "./parse.y"

			  yyval.command = make_for_command (yypvt[-5].word, add_string_to_list ("\"$@\"", (WORD_LIST *)NULL), yypvt[-1].command, word_lineno[word_top]);
			  if (word_top > 0) word_top--;
			
} break;

case YYr77: {	/* for_command :  FOR WORD ';' newline_list '{' compound_list '}' */
#line 799 "./parse.y"

			  yyval.command = make_for_command (yypvt[-5].word, add_string_to_list ("\"$@\"", (WORD_LIST *)NULL), yypvt[-1].command, word_lineno[word_top]);
			  if (word_top > 0) word_top--;
			
} break;

case YYr78: {	/* for_command :  FOR WORD newline_list IN word_list list_terminator newline_list DO compound_list DONE */
#line 804 "./parse.y"

			  yyval.command = make_for_command (yypvt[-8].word, REVERSE_LIST (yypvt[-5].word_list, WORD_LIST *), yypvt[-1].command, word_lineno[word_top]);
			  if (word_top > 0) word_top--;
			
} break;

case YYr79: {	/* for_command :  FOR WORD newline_list IN word_list list_terminator newline_list '{' compound_list '}' */
#line 809 "./parse.y"

			  yyval.command = make_for_command (yypvt[-8].word, REVERSE_LIST (yypvt[-5].word_list, WORD_LIST *), yypvt[-1].command, word_lineno[word_top]);
			  if (word_top > 0) word_top--;
			
} break;

case YYr80: {	/* for_command :  FOR WORD newline_list IN list_terminator newline_list DO compound_list DONE */
#line 814 "./parse.y"

			  yyval.command = make_for_command (yypvt[-7].word, (WORD_LIST *)NULL, yypvt[-1].command, word_lineno[word_top]);
			  if (word_top > 0) word_top--;
			
} break;

case YYr81: {	/* for_command :  FOR WORD newline_list IN list_terminator newline_list '{' compound_list '}' */
#line 819 "./parse.y"

			  yyval.command = make_for_command (yypvt[-7].word, (WORD_LIST *)NULL, yypvt[-1].command, word_lineno[word_top]);
			  if (word_top > 0) word_top--;
			
} break;

case YYr82: {	/* arith_for_command :  FOR ARITH_FOR_EXPRS list_terminator newline_list DO compound_list DONE */
#line 826 "./parse.y"

				  yyval.command = make_arith_for_command (yypvt[-5].word_list, yypvt[-1].command, arith_for_lineno);
				  if (word_top > 0) word_top--;
				
} break;

case YYr83: {	/* arith_for_command :  FOR ARITH_FOR_EXPRS list_terminator newline_list '{' compound_list '}' */
#line 831 "./parse.y"

				  yyval.command = make_arith_for_command (yypvt[-5].word_list, yypvt[-1].command, arith_for_lineno);
				  if (word_top > 0) word_top--;
				
} break;

case YYr84: {	/* arith_for_command :  FOR ARITH_FOR_EXPRS DO compound_list DONE */
#line 836 "./parse.y"

				  yyval.command = make_arith_for_command (yypvt[-3].word_list, yypvt[-1].command, arith_for_lineno);
				  if (word_top > 0) word_top--;
				
} break;

case YYr85: {	/* arith_for_command :  FOR ARITH_FOR_EXPRS '{' compound_list '}' */
#line 841 "./parse.y"

				  yyval.command = make_arith_for_command (yypvt[-3].word_list, yypvt[-1].command, arith_for_lineno);
				  if (word_top > 0) word_top--;
				
} break;

case YYr86: {	/* select_command :  SELECT WORD newline_list DO list DONE */
#line 848 "./parse.y"

			  yyval.command = make_select_command (yypvt[-4].word, add_string_to_list ("\"$@\"", (WORD_LIST *)NULL), yypvt[-1].command, word_lineno[word_top]);
			  if (word_top > 0) word_top--;
			
} break;

case YYr87: {	/* select_command :  SELECT WORD newline_list '{' list '}' */
#line 853 "./parse.y"

			  yyval.command = make_select_command (yypvt[-4].word, add_string_to_list ("\"$@\"", (WORD_LIST *)NULL), yypvt[-1].command, word_lineno[word_top]);
			  if (word_top > 0) word_top--;
			
} break;

case YYr88: {	/* select_command :  SELECT WORD ';' newline_list DO list DONE */
#line 858 "./parse.y"

			  yyval.command = make_select_command (yypvt[-5].word, add_string_to_list ("\"$@\"", (WORD_LIST *)NULL), yypvt[-1].command, word_lineno[word_top]);
			  if (word_top > 0) word_top--;
			
} break;

case YYr89: {	/* select_command :  SELECT WORD ';' newline_list '{' list '}' */
#line 863 "./parse.y"

			  yyval.command = make_select_command (yypvt[-5].word, add_string_to_list ("\"$@\"", (WORD_LIST *)NULL), yypvt[-1].command, word_lineno[word_top]);
			  if (word_top > 0) word_top--;
			
} break;

case YYr90: {	/* select_command :  SELECT WORD newline_list IN word_list list_terminator newline_list DO list DONE */
#line 868 "./parse.y"

			  yyval.command = make_select_command (yypvt[-8].word, REVERSE_LIST (yypvt[-5].word_list, WORD_LIST *), yypvt[-1].command, word_lineno[word_top]);
			  if (word_top > 0) word_top--;
			
} break;

case YYr91: {	/* select_command :  SELECT WORD newline_list IN word_list list_terminator newline_list '{' list '}' */
#line 873 "./parse.y"

			  yyval.command = make_select_command (yypvt[-8].word, REVERSE_LIST (yypvt[-5].word_list, WORD_LIST *), yypvt[-1].command, word_lineno[word_top]);
			  if (word_top > 0) word_top--;
			
} break;

case YYr92: {	/* case_command :  CASE WORD newline_list IN newline_list ESAC */
#line 880 "./parse.y"

			  yyval.command = make_case_command (yypvt[-4].word, (PATTERN_LIST *)NULL, word_lineno[word_top]);
			  if (word_top > 0) word_top--;
			
} break;

case YYr93: {	/* case_command :  CASE WORD newline_list IN case_clause_sequence newline_list ESAC */
#line 885 "./parse.y"

			  yyval.command = make_case_command (yypvt[-5].word, yypvt[-2].pattern, word_lineno[word_top]);
			  if (word_top > 0) word_top--;
			
} break;

case YYr94: {	/* case_command :  CASE WORD newline_list IN case_clause ESAC */
#line 890 "./parse.y"

			  yyval.command = make_case_command (yypvt[-4].word, yypvt[-1].pattern, word_lineno[word_top]);
			  if (word_top > 0) word_top--;
			
} break;

case YYr95: {	/* function_def :  WORD '(' ')' newline_list function_body */
#line 897 "./parse.y"
 yyval.command = make_function_def (yypvt[-4].word, yypvt[0].command, function_dstart, function_bstart); 
} break;

case YYr96: {	/* function_def :  FUNCTION WORD '(' ')' newline_list function_body */
#line 900 "./parse.y"
 yyval.command = make_function_def (yypvt[-4].word, yypvt[0].command, function_dstart, function_bstart); 
} break;

case YYr97: {	/* function_def :  FUNCTION WORD newline_list function_body */
#line 903 "./parse.y"
 yyval.command = make_function_def (yypvt[-2].word, yypvt[0].command, function_dstart, function_bstart); 
} break;

case YYr98: {	/* function_body :  shell_command */
#line 907 "./parse.y"
 yyval.command = yypvt[0].command; 
} break;

case YYr99: {	/* function_body :  shell_command redirection_list */
#line 909 "./parse.y"

			  COMMAND *tc;

			  tc = yypvt[-1].command;
			  
			  
			  if (tc->redirects)
			    {
			      register REDIRECT *t;
			      for (t = tc->redirects; t->next; t = t->next)
				;
			      t->next = yypvt[0].redirect;
			    }
			  else
			    tc->redirects = yypvt[0].redirect;
			  yyval.command = yypvt[-1].command;
			
} break;

case YYr100: {	/* subshell :  '(' compound_list ')' */
#line 940 "./parse.y"

			  yyval.command = make_subshell_command (yypvt[-1].command);
			  yyval.command->flags |= CMD_WANT_SUBSHELL;
			
} break;

case YYr101: {	/* coproc :  COPROC shell_command */
#line 947 "./parse.y"

			  yyval.command = make_coproc_command ("COPROC", yypvt[0].command);
			  yyval.command->flags |= CMD_WANT_SUBSHELL|CMD_COPROC_SUBSHELL;
			
} break;

case YYr102: {	/* coproc :  COPROC shell_command redirection_list */
#line 952 "./parse.y"

			  COMMAND *tc;

			  tc = yypvt[-1].command;
			  if (tc->redirects)
			    {
			      register REDIRECT *t;
			      for (t = tc->redirects; t->next; t = t->next)
				;
			      t->next = yypvt[0].redirect;
			    }
			  else
			    tc->redirects = yypvt[0].redirect;
			  yyval.command = make_coproc_command ("COPROC", yypvt[-1].command);
			  yyval.command->flags |= CMD_WANT_SUBSHELL|CMD_COPROC_SUBSHELL;
			
} break;

case YYr103: {	/* coproc :  COPROC WORD shell_command */
#line 969 "./parse.y"

			  yyval.command = make_coproc_command (yypvt[-1].word->word, yypvt[0].command);
			  yyval.command->flags |= CMD_WANT_SUBSHELL|CMD_COPROC_SUBSHELL;
			
} break;

case YYr104: {	/* coproc :  COPROC WORD shell_command redirection_list */
#line 974 "./parse.y"

			  COMMAND *tc;

			  tc = yypvt[-1].command;
			  if (tc->redirects)
			    {
			      register REDIRECT *t;
			      for (t = tc->redirects; t->next; t = t->next)
				;
			      t->next = yypvt[0].redirect;
			    }
			  else
			    tc->redirects = yypvt[0].redirect;
			  yyval.command = make_coproc_command (yypvt[-2].word->word, yypvt[-1].command);
			  yyval.command->flags |= CMD_WANT_SUBSHELL|CMD_COPROC_SUBSHELL;
			
} break;

case YYr105: {	/* coproc :  COPROC simple_command */
#line 991 "./parse.y"

			  yyval.command = make_coproc_command ("COPROC", clean_simple_command (yypvt[0].command));
			  yyval.command->flags |= CMD_WANT_SUBSHELL|CMD_COPROC_SUBSHELL;
			
} break;

case YYr106: {	/* if_command :  IF compound_list THEN compound_list FI */
#line 998 "./parse.y"
 yyval.command = make_if_command (yypvt[-3].command, yypvt[-1].command, (COMMAND *)NULL); 
} break;

case YYr107: {	/* if_command :  IF compound_list THEN compound_list ELSE compound_list FI */
#line 1000 "./parse.y"
 yyval.command = make_if_command (yypvt[-5].command, yypvt[-3].command, yypvt[-1].command); 
} break;

case YYr108: {	/* if_command :  IF compound_list THEN compound_list elif_clause FI */
#line 1002 "./parse.y"
 yyval.command = make_if_command (yypvt[-4].command, yypvt[-2].command, yypvt[-1].command); 
} break;

case YYr109: {	/* group_command :  '{' compound_list '}' */
#line 1007 "./parse.y"
 yyval.command = make_group_command (yypvt[-1].command); 
} break;

case YYr110: {	/* arith_command :  ARITH_CMD */
#line 1011 "./parse.y"
 yyval.command = make_arith_command (yypvt[0].word_list); 
} break;

case YYr111: {	/* cond_command :  COND_START COND_CMD COND_END */
#line 1015 "./parse.y"
 yyval.command = yypvt[-1].command; 
} break;

case YYr112: {	/* elif_clause :  ELIF compound_list THEN compound_list */
#line 1019 "./parse.y"
 yyval.command = make_if_command (yypvt[-2].command, yypvt[0].command, (COMMAND *)NULL); 
} break;

case YYr113: {	/* elif_clause :  ELIF compound_list THEN compound_list ELSE compound_list */
#line 1021 "./parse.y"
 yyval.command = make_if_command (yypvt[-4].command, yypvt[-2].command, yypvt[0].command); 
} break;

case YYr114: {	/* elif_clause :  ELIF compound_list THEN compound_list elif_clause */
#line 1023 "./parse.y"
 yyval.command = make_if_command (yypvt[-3].command, yypvt[-1].command, yypvt[0].command); 
} break;

case YYr116: {	/* case_clause :  case_clause_sequence pattern_list */
#line 1028 "./parse.y"
 yypvt[0].pattern->next = yypvt[-1].pattern; yyval.pattern = yypvt[0].pattern; 
} break;

case YYr117: {	/* pattern_list :  newline_list pattern ')' compound_list */
#line 1032 "./parse.y"
 yyval.pattern = make_pattern_list (yypvt[-2].word_list, yypvt[0].command); 
} break;

case YYr118: {	/* pattern_list :  newline_list pattern ')' newline_list */
#line 1034 "./parse.y"
 yyval.pattern = make_pattern_list (yypvt[-2].word_list, (COMMAND *)NULL); 
} break;

case YYr119: {	/* pattern_list :  newline_list '(' pattern ')' compound_list */
#line 1036 "./parse.y"
 yyval.pattern = make_pattern_list (yypvt[-2].word_list, yypvt[0].command); 
} break;

case YYr120: {	/* pattern_list :  newline_list '(' pattern ')' newline_list */
#line 1038 "./parse.y"
 yyval.pattern = make_pattern_list (yypvt[-2].word_list, (COMMAND *)NULL); 
} break;

case YYr121: {	/* case_clause_sequence :  pattern_list SEMI_SEMI */
#line 1042 "./parse.y"
 yyval.pattern = yypvt[-1].pattern; 
} break;

case YYr122: {	/* case_clause_sequence :  case_clause_sequence pattern_list SEMI_SEMI */
#line 1044 "./parse.y"
 yypvt[-1].pattern->next = yypvt[-2].pattern; yyval.pattern = yypvt[-1].pattern; 
} break;

case YYr123: {	/* case_clause_sequence :  pattern_list SEMI_AND */
#line 1046 "./parse.y"
 yypvt[-1].pattern->flags |= CASEPAT_FALLTHROUGH; yyval.pattern = yypvt[-1].pattern; 
} break;

case YYr124: {	/* case_clause_sequence :  case_clause_sequence pattern_list SEMI_AND */
#line 1048 "./parse.y"
 yypvt[-1].pattern->flags |= CASEPAT_FALLTHROUGH; yypvt[-1].pattern->next = yypvt[-2].pattern; yyval.pattern = yypvt[-1].pattern; 
} break;

case YYr125: {	/* case_clause_sequence :  pattern_list SEMI_SEMI_AND */
#line 1050 "./parse.y"
 yypvt[-1].pattern->flags |= CASEPAT_TESTNEXT; yyval.pattern = yypvt[-1].pattern; 
} break;

case YYr126: {	/* case_clause_sequence :  case_clause_sequence pattern_list SEMI_SEMI_AND */
#line 1052 "./parse.y"
 yypvt[-1].pattern->flags |= CASEPAT_TESTNEXT; yypvt[-1].pattern->next = yypvt[-2].pattern; yyval.pattern = yypvt[-1].pattern; 
} break;

case YYr127: {	/* pattern :  WORD */
#line 1056 "./parse.y"
 yyval.word_list = make_word_list (yypvt[0].word, (WORD_LIST *)NULL); 
} break;

case YYr128: {	/* pattern :  pattern '|' WORD */
#line 1058 "./parse.y"
 yyval.word_list = make_word_list (yypvt[0].word, yypvt[-2].word_list); 
} break;

case YYr129: {	/* list :  newline_list list0 */
#line 1067 "./parse.y"

			  yyval.command = yypvt[0].command;
			  if (need_here_doc)
			    gather_here_documents ();
			 
} break;

case YYr131: {	/* compound_list :  newline_list list1 */
#line 1076 "./parse.y"

			  yyval.command = yypvt[0].command;
			
} break;

case YYr133: {	/* list0 :  list1 '&' newline_list */
#line 1083 "./parse.y"

			  if (yypvt[-2].command->type == cm_connection)
			    yyval.command = connect_async_list (yypvt[-2].command, (COMMAND *)NULL, '&');
			  else
			    yyval.command = command_connect (yypvt[-2].command, (COMMAND *)NULL, '&');
			
} break;

case YYr135: {	/* list1 :  list1 AND_AND newline_list list1 */
#line 1094 "./parse.y"
 yyval.command = command_connect (yypvt[-3].command, yypvt[0].command, AND_AND); 
} break;

case YYr136: {	/* list1 :  list1 OR_OR newline_list list1 */
#line 1096 "./parse.y"
 yyval.command = command_connect (yypvt[-3].command, yypvt[0].command, OR_OR); 
} break;

case YYr137: {	/* list1 :  list1 '&' newline_list list1 */
#line 1098 "./parse.y"

			  if (yypvt[-3].command->type == cm_connection)
			    yyval.command = connect_async_list (yypvt[-3].command, yypvt[0].command, '&');
			  else
			    yyval.command = command_connect (yypvt[-3].command, yypvt[0].command, '&');
			
} break;

case YYr138: {	/* list1 :  list1 ';' newline_list list1 */
#line 1105 "./parse.y"
 yyval.command = command_connect (yypvt[-3].command, yypvt[0].command, ';'); 
} break;

case YYr139: {	/* list1 :  list1 '\n' newline_list list1 */
#line 1107 "./parse.y"
 yyval.command = command_connect (yypvt[-3].command, yypvt[0].command, ';'); 
} break;

case YYr140: {	/* list1 :  pipeline_command */
#line 1109 "./parse.y"
 yyval.command = yypvt[0].command; 
} break;

case YYr143: {	/* list_terminator :  '\n' */
#line 1117 "./parse.y"
 yyval.number = '\n'; 
} break;

case YYr144: {	/* list_terminator :  ';' */
#line 1119 "./parse.y"
 yyval.number = ';'; 
} break;

case YYr145: {	/* list_terminator :  yacc_EOF */
#line 1121 "./parse.y"
 yyval.number = yacc_EOF; 
} break;

case YYr148: {	/* simple_list :  simple_list1 */
#line 1135 "./parse.y"

			  yyval.command = yypvt[0].command;
			  if (need_here_doc)
			    gather_here_documents ();
			  if ((parser_state & PST_CMDSUBST) && current_token == shell_eof_token)
			    {
			      global_command = yypvt[0].command;
			      eof_encountered = 0;
			      rewind_input_string ();
			      YYACCEPT;
			    }
			
} break;

case YYr149: {	/* simple_list :  simple_list1 '&' */
#line 1148 "./parse.y"

			  if (yypvt[-1].command->type == cm_connection)
			    yyval.command = connect_async_list (yypvt[-1].command, (COMMAND *)NULL, '&');
			  else
			    yyval.command = command_connect (yypvt[-1].command, (COMMAND *)NULL, '&');
			  if (need_here_doc)
			    gather_here_documents ();
			  if ((parser_state & PST_CMDSUBST) && current_token == shell_eof_token)
			    {
			      global_command = yypvt[-1].command;
			      eof_encountered = 0;
			      rewind_input_string ();
			      YYACCEPT;
			    }
			
} break;

case YYr150: {	/* simple_list :  simple_list1 ';' */
#line 1164 "./parse.y"

			  yyval.command = yypvt[-1].command;
			  if (need_here_doc)
			    gather_here_documents ();
			  if ((parser_state & PST_CMDSUBST) && current_token == shell_eof_token)
			    {
			      global_command = yypvt[-1].command;
			      eof_encountered = 0;
			      rewind_input_string ();
			      YYACCEPT;
			    }
			
} break;

case YYr151: {	/* simple_list1 :  simple_list1 AND_AND newline_list simple_list1 */
#line 1179 "./parse.y"
 yyval.command = command_connect (yypvt[-3].command, yypvt[0].command, AND_AND); 
} break;

case YYr152: {	/* simple_list1 :  simple_list1 OR_OR newline_list simple_list1 */
#line 1181 "./parse.y"
 yyval.command = command_connect (yypvt[-3].command, yypvt[0].command, OR_OR); 
} break;

case YYr153: {	/* simple_list1 :  simple_list1 '&' simple_list1 */
#line 1183 "./parse.y"

			  if (yypvt[-2].command->type == cm_connection)
			    yyval.command = connect_async_list (yypvt[-2].command, yypvt[0].command, '&');
			  else
			    yyval.command = command_connect (yypvt[-2].command, yypvt[0].command, '&');
			
} break;

case YYr154: {	/* simple_list1 :  simple_list1 ';' simple_list1 */
#line 1190 "./parse.y"
 yyval.command = command_connect (yypvt[-2].command, yypvt[0].command, ';'); 
} break;

case YYr155: {	/* simple_list1 :  pipeline_command */
#line 1193 "./parse.y"
 yyval.command = yypvt[0].command; 
} break;

case YYr156: {	/* pipeline_command :  pipeline */
#line 1197 "./parse.y"
 yyval.command = yypvt[0].command; 
} break;

case YYr157: {	/* pipeline_command :  BANG pipeline_command */
#line 1199 "./parse.y"

			  if (yypvt[0].command)
			    yypvt[0].command->flags ^= CMD_INVERT_RETURN;	
			  yyval.command = yypvt[0].command;
			
} break;

case YYr158: {	/* pipeline_command :  timespec pipeline_command */
#line 1205 "./parse.y"

			  if (yypvt[0].command)
			    yypvt[0].command->flags |= yypvt[-1].number;
			  yyval.command = yypvt[0].command;
			
} break;

case YYr159: {	/* pipeline_command :  timespec list_terminator */
#line 1211 "./parse.y"

			  ELEMENT x;

			  
			  x.word = 0;
			  x.redirect = 0;
			  yyval.command = make_simple_command (x, (COMMAND *)NULL);
			  yyval.command->flags |= yypvt[-1].number;
			  
			  if (yypvt[0].number == '\n')
			    token_to_read = '\n';
			
} break;

case YYr160: {	/* pipeline_command :  BANG list_terminator */
#line 1228 "./parse.y"

			  ELEMENT x;

			  
			  x.word = 0;
			  x.redirect = 0;
			  yyval.command = make_simple_command (x, (COMMAND *)NULL);
			  yyval.command->flags |= CMD_INVERT_RETURN;
			  
			  if (yypvt[0].number == '\n')
			    token_to_read = '\n';
			
} break;

case YYr161: {	/* pipeline :  pipeline '|' newline_list pipeline */
#line 1248 "./parse.y"
 yyval.command = command_connect (yypvt[-3].command, yypvt[0].command, '|'); 
} break;

case YYr162: {	/* pipeline :  pipeline BAR_AND newline_list pipeline */
#line 1250 "./parse.y"

			  
			  COMMAND *tc;
			  REDIRECTEE rd, sd;
			  REDIRECT *r;

			  tc = yypvt[-3].command->type == cm_simple ? (COMMAND *)yypvt[-3].command->value.Simple : yypvt[-3].command;
			  sd.dest = 2;
			  rd.dest = 1;
			  r = make_redirection (sd, r_duplicating_output, rd, 0);
			  if (tc->redirects)
			    {
			      register REDIRECT *t;
			      for (t = tc->redirects; t->next; t = t->next)
				;
			      t->next = r;
			    }
			  else
			    tc->redirects = r;

			  yyval.command = command_connect (yypvt[-3].command, yypvt[0].command, '|');
			
} break;

case YYr163: {	/* pipeline :  command */
#line 1273 "./parse.y"
 yyval.command = yypvt[0].command; 
} break;

case YYr164: {	/* timespec :  TIME */
#line 1277 "./parse.y"
 yyval.number = CMD_TIME_PIPELINE; 
} break;

case YYr165: {	/* timespec :  TIME TIMEOPT */
#line 1279 "./parse.y"
 yyval.number = CMD_TIME_PIPELINE|CMD_TIME_POSIX; 
} break;

case YYr166: {	/* timespec :  TIME TIMEOPT TIMEIGN */
#line 1281 "./parse.y"
 yyval.number = CMD_TIME_PIPELINE|CMD_TIME_POSIX; 
} break;
#line 493 "/etc/yyparse.c"
      case YYrACCEPT: YYACCEPT;
      case YYrERROR:  goto yyError;
     }
 
   /*-------------------------------------------*/
   /* Look up next state in goto table.         */
   /*-------------------------------------------*/
   yyp = &yygo[yypgo[yyi]];
   yyq = yyp++;
   yyi = *yyps;
   while (yyi < *yyp++)
        ;
   yystate = yyneg(yyi == *--yyp? YYQYYP: *yyq);
 
#if YYDEBUG
   if (yydebug)
     YY_TRACE(yyShowGoto)
#endif
 
   goto yyStack;
 
yyerrlabel: ;     /* come here from YYERROR  */
/*
#pragma used yyerrlabel
*/
 yyerrflag = 1;
 if (yyi == YYrERROR)
   {
    yyps--;
    yypv--;
#if YYDEBUG
    yytp--;
#endif
   }
 
yyError:
   switch (yyerrflag)
     {
      /*----------------------------------------------*/
      /* new error                                    */
      /*----------------------------------------------*/
      case 0:
        yynerrs++;
        yyi = yychar;
        yyerror(m_textmsg(4969, "Syntax error", "E"));
        if (yyi != yychar)
          {
           /* user has changed the current token */
           /* try again */
           yyerrflag++;   /* avoid loops */
           goto yyEncore;
          }
 
      /*----------------------------------------------*/
      /* partially recovered                          */
      /*----------------------------------------------*/
      case 1:
      case 2:
         yyerrflag = 3;           /* need 3 valid shifts to recover */
 
         /*
          * Pop states, looking for a
          * shift on `error'.
          */
 
         for ( ; yyps > yys; yyps--, yypv--
#if YYDEBUG
                  , yytp--
#endif
         ) {
            if (*yyps >= sizeof yypact/sizeof yypact[0])
               continue;
            yyp = &yyact[yypact[*yyps]];
            yyq = yyp;
            do {
                if (YYERRCODE == *yyp)
                  {
                   yyp++;
                   yystate = yyneg(YYQYYP);
                   goto yyStack;
                  }
               } while (*yyp++ > YYTOKEN_BASE);
 
            /* no shift in this state */
#if YYDEBUG
            if (yydebug && yyps > yys+1)
              YY_TRACE(yyShowErrRecovery)
#endif
            /* pop stacks; try again */
           }
         /* no shift on error - abort */
         break;
 
      /*----------------------------------------------*/
      /* Erroneous token after an error - discard it. */
      /*----------------------------------------------*/
      case 3:
         if (yychar == 0)  /* but not EOF */
            break;
#if YYDEBUG
         if (yydebug)
            YY_TRACE(yyShowErrDiscard)
#endif
         yyclearin;
         goto yyEncore; /* try again in same state */
     }
 YYABORT;
 
#ifdef YYALLOC
yyReturn:
  yylval    = save_yylval;
  yyval     = save_yyval;
  yypvt     = save_yypvt;
  yychar    = save_yychar;
  yyerrflag = save_yyerrflag;
  yynerrs   = save_yynerrs;
  free((char *)yys);
  free((char *)yyv);
 
  #if YYDEBUG
    free((char *)yytypev);
  #endif
  return(retval);
#endif
 
}
 
/*=====================================================================*/
/* DEFINE DEBUG/TRACE FUNCTIONS.                                       */
/*=====================================================================*/
 
#if YYDEBUG
/*----------------------------------------------------*/
/* Return type of token                               */
/*----------------------------------------------------*/
int yyGetType(int tok)
 {
   yyNamedType * tp;
   for (tp = &yyTokenTypes[yyntoken-1]; tp > yyTokenTypes; tp--)
      if (tp->token == tok)
         return tp->type;
   return 0;
 }
 
/*----------------------------------------------------*/
/* Print a token legibly.                             */
/*----------------------------------------------------*/
char * yyptok(int tok)
 {
   yyNamedType * tp;
   for (tp = &yyTokenTypes[yyntoken-1]; tp > yyTokenTypes; tp--)
      if (tp->token == tok)
         return tp->name;
   return "";
 }
 
/*----------------------------------------------------*/
/* Show current state of yyparse                      */
/*----------------------------------------------------*/
void yyShowState(yyTraceItems * tp)
 {
  short * p;
  YYSTYPE * q;
 
  printf(m_textmsg(2828, "state %d (%d), char %s (%d)\n",
                   "I num1 num2 char num3"),
         yysmap[tp->state], tp->state,
         yyptok(tp->lookahead), tp->lookahead);
 }
 
/*----------------------------------------------------*/
/* show results of reduction                          */
/*----------------------------------------------------*/
void yyShowReduce(yyTraceItems * tp)
 {
  printf("reduce %d (%d), pops %d (%d)\n",
          yyrmap[tp->rule], tp->rule,
          tp->states[tp->nstates - tp->npop],
          yysmap[tp->states[tp->nstates - tp->npop]]);
 }
 
/*----------------------------------------------------*/
/*                                                    */
/*----------------------------------------------------*/
void yyShowRead(int val)
 {
   printf(m_textmsg(2829, "read %s (%d)\n", "I token num"), yyptok(val), val);
 }
 
/*----------------------------------------------------*/
/*                                                    */
/*----------------------------------------------------*/
void yyShowGoto(yyTraceItems * tp)
 {
   printf(m_textmsg(2830, "goto %d (%d)\n", "I num1 num2"),
          yysmap[tp->state], tp->state);
 }
 
/*----------------------------------------------------*/
/*                                                    */
/*----------------------------------------------------*/
void yyShowShift(yyTraceItems * tp)
 {
   printf(m_textmsg(2831, "shift %d (%d)\n", "I num1 num2"),
          yysmap[tp->state], tp->state);
 }
 
/*----------------------------------------------------*/
/*                                                    */
/*----------------------------------------------------*/
void yyShowErrRecovery(yyTraceItems * tp)
 {
  short * top = tp->states + tp->nstates - 1;
 
  printf(m_textmsg(2832, "Error recovery pops state %d (%d), uncovers %d (%d)\n",
                  "I num1 num2 num3 num4"),
         yysmap[*top], *top, yysmap[*(top-1)], *(top-1));
 }
 
/*----------------------------------------------------*/
/*                                                    */
/*----------------------------------------------------*/
void yyShowErrDiscard(yyTraceItems * tp)
 {
  printf(m_textmsg(2833, "Error recovery discards %s (%d), ", "I token num"),
         yyptok(tp->lookahead), tp->lookahead);
 }
 
#endif   /* YYDEBUG */
