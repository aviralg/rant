/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "gram.y" /* yacc.c:339  */

/*
 *  R : A Computer Langage for Statistical Data Analysis
 *  Copyright (C) 1995, 1996, 1997  Robert Gentleman and Ross Ihaka
 *  Copyright (C) 1997--2015  The R Core Team
 *  Copyright (C) 2009--2011  Romain Francois
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, a copy is available at
 *  https://www.R-project.org/Licenses/
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "IOStuff.h"		/*-> Defn.h */
#include "Fileio.h"
#include "Parse.h"
#include <R_ext/Print.h>

#if !defined(__STDC_ISO_10646__) && (defined(__APPLE__) || defined(__FreeBSD__))
/* This may not be 100% true (see the comment in rlocale.h),
   but it seems true in normal locales */
# define __STDC_ISO_10646__
#endif


#define YYERROR_VERBOSE 1
#define PARSE_ERROR_SIZE 256	    /* Parse error messages saved here */
#define PARSE_CONTEXT_SIZE 256	    /* Recent parse context kept in a circular buffer */

static Rboolean busy = FALSE;

static int identifier ;
static void incrementId(void);
static void initData(void);
static void initId(void);
static void record_( int, int, int, int, int, int, char* ) ;

static void yyerror(const char *);
static int yylex();
int yyparse(void);

static FILE *fp_parse;
static int (*ptr_getc)(void);

static int	SavedToken;
static SEXP	SavedLval;

#define yyconst const

typedef struct yyltype
{
  int first_line;
  int first_column;
  int first_byte;

  int last_line;
  int last_column;
  int last_byte;
  
  int first_parsed;
  int last_parsed;
  
  int id;
} yyltype;


#define INIT_DATA_COUNT 16384    	/* init parser data to this size */
#define MAX_DATA_COUNT   65536		/* release it at the end if it is this size or larger*/

#define DATA_COUNT  (ParseState.data ? length( ParseState.data ) / DATA_ROWS : 0)
#define ID_COUNT    ((ParseState.ids ? length( ParseState.ids ) / 2 : 0) - 1)

static void finalizeData( ) ;
static void growData( ) ;
static void growID( int ) ;

#define DATA_ROWS 8

#define _FIRST_PARSED( i ) INTEGER( ParseState.data )[ DATA_ROWS*(i)     ]
#define _FIRST_COLUMN( i ) INTEGER( ParseState.data )[ DATA_ROWS*(i) + 1 ]
#define _LAST_PARSED( i )  INTEGER( ParseState.data )[ DATA_ROWS*(i) + 2 ]
#define _LAST_COLUMN( i )  INTEGER( ParseState.data )[ DATA_ROWS*(i) + 3 ]
#define _TERMINAL( i ) 	   INTEGER( ParseState.data )[ DATA_ROWS*(i) + 4 ]
#define _TOKEN( i )        INTEGER( ParseState.data )[ DATA_ROWS*(i) + 5 ]
#define _ID( i )           INTEGER( ParseState.data )[ DATA_ROWS*(i) + 6 ]
#define _PARENT(i)         INTEGER( ParseState.data )[ DATA_ROWS*(i) + 7 ]

#define ID_ID( i )      INTEGER(ParseState.ids)[ 2*(i) ]
#define ID_PARENT( i )  INTEGER(ParseState.ids)[ 2*(i) + 1 ]

static void modif_token( yyltype*, int ) ;
static void recordParents( int, yyltype*, int) ;

static int _current_token ;

/**
 * Records an expression (non terminal symbol 'expr') and gives it an id
 *
 * @param expr expression we want to record and flag with the next id
 * @param loc the location of the expression
 */   
static void setId( SEXP expr, yyltype loc){
    record_( 
	    (loc).first_parsed, (loc).first_column, (loc).last_parsed, (loc).last_column, 
	    _current_token, (loc).id, 0 ) ;
}

# define YYLTYPE yyltype
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do	{ 								\
	if (N){								\
	    (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	    (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	    (Current).first_byte   = YYRHSLOC (Rhs, 1).first_byte;	\
	    (Current).last_line    = YYRHSLOC (Rhs, N).last_line;	\
	    (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	    (Current).last_byte    = YYRHSLOC (Rhs, N).last_byte;	\
	    (Current).first_parsed = YYRHSLOC (Rhs, 1).first_parsed;    \
	    (Current).last_parsed  = YYRHSLOC (Rhs, N).last_parsed;	\
	    incrementId( ) ; 						\
	    (Current).id = identifier ; 				\
	    _current_token = yyr1[yyn] ; 				\
	    yyltype childs[N] ;						\
	    int ii = 0; 						\
	    for( ii=0; ii<N; ii++){ 					\
		      childs[ii] = YYRHSLOC (Rhs, (ii+1) ) ; 		\
	    } 								\
	    recordParents( identifier, childs, N) ; 			\
	} else	{							\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = YYRHSLOC (Rhs, 0).last_column;	\
	  (Current).last_column = (Current).first_column - 1;		\
	  (Current).first_byte = YYRHSLOC (Rhs, 0).last_byte;		\
	  (Current).last_byte = (Current).first_byte - 1;		\
	  (Current).id = NA_INTEGER;                                    \
	} 								\
    } while (0)

		
# define YY_LOCATION_PRINT(Loc)					\
 fprintf ( stderr, "%d.%d.%d-%d.%d.%d (%d)",				\
 	(Loc).first_line, (Loc).first_column,	(Loc).first_byte, 	\
 	(Loc).last_line,  (Loc).last_column, 	(Loc).last_byte, 	\
	(Loc).id )

/* Useful defines so editors don't get confused ... */

#define LBRACE	'{'
#define RBRACE	'}'

/* Functions used in the parsing process */

static void	CheckFormalArgs(SEXP, SEXP, YYLTYPE *);
static SEXP	FirstArg(SEXP, SEXP);
static SEXP	GrowList(SEXP, SEXP);
static void	IfPush(void);
static int	KeywordLookup(const char *);
static SEXP	NewList(void);
static SEXP	NextArg(SEXP, SEXP, SEXP);
static SEXP	TagArg(SEXP, SEXP, YYLTYPE *);
static int 	processLineDirective();

/* These routines allocate constants */

static SEXP	mkComplex(const char *);
SEXP		mkFalse(void);
static SEXP     mkFloat(const char *);
static SEXP 	mkInt(const char *); 
static SEXP	mkNA(void);
SEXP		mkTrue(void);

/* Internal lexer / parser state variables */

static int	EatLines = 0;
static int	GenerateCode = 0;
static int	EndOfFile = 0;
static int	xxgetc();
static int	xxungetc(int);
static int	xxcharcount, xxcharsave;
static int	xxlinesave, xxbytesave, xxcolsave, xxparsesave;

static SEXP	SrcRefs;
static SrcRefState ParseState;
static PROTECT_INDEX srindex;

#include <rlocale.h>
#ifdef HAVE_LANGINFO_CODESET
# include <langinfo.h>
#endif


static int mbcs_get_next(int c, wchar_t *wc)
{
    int i, res, clen = 1; char s[9];
    mbstate_t mb_st;

    s[0] = (char) c;
    /* This assumes (probably OK) that all MBCS embed ASCII as single-byte
       lead bytes, including control chars */
    if((unsigned int) c < 0x80) {
	*wc = (wchar_t) c;
	return 1;
    }
    if(utf8locale) {
	clen = utf8clen((char) c);
	for(i = 1; i < clen; i++) {
	    s[i] = (char) xxgetc();
	    if(s[i] == R_EOF) error(_("EOF whilst reading MBCS char at line %d"), ParseState.xxlineno);
	}
	s[clen] ='\0'; /* x86 Solaris requires this */
	res = (int) mbrtowc(wc, s, clen, NULL);
	if(res == -1) error(_("invalid multibyte character in parser at line %d"), ParseState.xxlineno);
    } else {
	/* This is not necessarily correct for stateful MBCS */
	while(clen <= MB_CUR_MAX) {
	    mbs_init(&mb_st);
	    res = (int) mbrtowc(wc, s, clen, &mb_st);
	    if(res >= 0) break;
	    if(res == -1)
		error(_("invalid multibyte character in parser at line %d"), ParseState.xxlineno);
	    /* so res == -2 */
	    c = xxgetc();
	    if(c == R_EOF) error(_("EOF whilst reading MBCS char at line %d"), ParseState.xxlineno);
	    s[clen++] = (char) c;
	} /* we've tried enough, so must be complete or invalid by now */
    }
    for(i = clen - 1; i > 0; i--) xxungetc(s[i]);
    return clen;
}

/* Soon to be defunct entry points */

void		R_SetInput(int);
int		R_fgetc(FILE*);
static int colon ;

/* Routines used to build the parse tree */

static SEXP	xxnullformal(void);
static SEXP	xxfirstformal0(SEXP);
static SEXP	xxfirstformal1(SEXP, SEXP);
static SEXP	xxaddformal0(SEXP, SEXP, YYLTYPE *);
static SEXP	xxaddformal1(SEXP, SEXP, SEXP, YYLTYPE *);
static SEXP	xxexprlist0();
static SEXP	xxexprlist1(SEXP, YYLTYPE *);
static SEXP	xxexprlist2(SEXP, SEXP, YYLTYPE *);
static SEXP	xxsub0(void);
static SEXP	xxsub1(SEXP, YYLTYPE *);
static SEXP	xxsymsub0(SEXP, YYLTYPE *);
static SEXP	xxsymsub1(SEXP, SEXP, YYLTYPE *);
static SEXP	xxnullsub0(YYLTYPE *);
static SEXP	xxnullsub1(SEXP, YYLTYPE *);
static SEXP	xxsublist1(SEXP);
static SEXP	xxsublist2(SEXP, SEXP);
static SEXP	xxcond(SEXP);
static SEXP	xxifcond(SEXP);
static SEXP	xxif(SEXP, SEXP, SEXP);
static SEXP	xxifelse(SEXP, SEXP, SEXP, SEXP);
static SEXP	xxforcond(SEXP, SEXP);
static SEXP	xxfor(SEXP, SEXP, SEXP);
static SEXP	xxwhile(SEXP, SEXP, SEXP);
static SEXP	xxrepeat(SEXP, SEXP);
static SEXP	xxnxtbrk(SEXP);
static SEXP	xxfuncall(SEXP, SEXP);
static SEXP	xxdefun(SEXP, SEXP, SEXP, YYLTYPE *);
static SEXP	xxunary(SEXP, SEXP);
static SEXP	xxbinary(SEXP, SEXP, SEXP);
static SEXP	xxparen(SEXP, SEXP);
static SEXP	xxsubscript(SEXP, SEXP, SEXP);
static SEXP	xxexprlist(SEXP, YYLTYPE *, SEXP);
static int	xxvalue(SEXP, int, YYLTYPE *);
static SEXP xxannotate(SEXP, SEXP);
static SEXP xxannotation(SEXP);


#define YYSTYPE		SEXP


#line 359 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    END_OF_INPUT = 258,
    ERROR = 259,
    STR_CONST = 260,
    NUM_CONST = 261,
    NULL_CONST = 262,
    SYMBOL = 263,
    FUNCTION = 264,
    INCOMPLETE_STRING = 265,
    LEFT_ASSIGN = 266,
    EQ_ASSIGN = 267,
    RIGHT_ASSIGN = 268,
    LBB = 269,
    FOR = 270,
    IN = 271,
    IF = 272,
    ELSE = 273,
    WHILE = 274,
    NEXT = 275,
    BREAK = 276,
    REPEAT = 277,
    GT = 278,
    GE = 279,
    LT = 280,
    LE = 281,
    EQ = 282,
    NE = 283,
    AND = 284,
    OR = 285,
    AND2 = 286,
    OR2 = 287,
    NS_GET = 288,
    NS_GET_INT = 289,
    COMMENT = 290,
    LINE_DIRECTIVE = 291,
    SYMBOL_FORMALS = 292,
    EQ_FORMALS = 293,
    EQ_SUB = 294,
    SYMBOL_SUB = 295,
    SYMBOL_FUNCTION_CALL = 296,
    SYMBOL_PACKAGE = 297,
    COLON_ASSIGN = 298,
    SLOT = 299,
    ANNOTATION_OPERATOR = 300,
    DELIMITED_ANNOTATION_BEGIN = 301,
    DELIMITED_ANNOTATION_END = 302,
    FUNCTION_OPERATOR = 303,
    LOW = 304,
    TILDE = 305,
    UNOT = 306,
    NOT = 307,
    SPECIAL = 308,
    UMINUS = 309,
    UPLUS = 310
  };
#endif
/* Tokens.  */
#define END_OF_INPUT 258
#define ERROR 259
#define STR_CONST 260
#define NUM_CONST 261
#define NULL_CONST 262
#define SYMBOL 263
#define FUNCTION 264
#define INCOMPLETE_STRING 265
#define LEFT_ASSIGN 266
#define EQ_ASSIGN 267
#define RIGHT_ASSIGN 268
#define LBB 269
#define FOR 270
#define IN 271
#define IF 272
#define ELSE 273
#define WHILE 274
#define NEXT 275
#define BREAK 276
#define REPEAT 277
#define GT 278
#define GE 279
#define LT 280
#define LE 281
#define EQ 282
#define NE 283
#define AND 284
#define OR 285
#define AND2 286
#define OR2 287
#define NS_GET 288
#define NS_GET_INT 289
#define COMMENT 290
#define LINE_DIRECTIVE 291
#define SYMBOL_FORMALS 292
#define EQ_FORMALS 293
#define EQ_SUB 294
#define SYMBOL_SUB 295
#define SYMBOL_FUNCTION_CALL 296
#define SYMBOL_PACKAGE 297
#define COLON_ASSIGN 298
#define SLOT 299
#define ANNOTATION_OPERATOR 300
#define DELIMITED_ANNOTATION_BEGIN 301
#define DELIMITED_ANNOTATION_END 302
#define FUNCTION_OPERATOR 303
#define LOW 304
#define TILDE 305
#define UNOT 306
#define NOT 307
#define SPECIAL 308
#define UMINUS 309
#define UPLUS 310

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);



/* Copy the second part of user declarations.  */

#line 531 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  67
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1522

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  77
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  25
/* YYNRULES -- Number of rules.  */
#define YYNRULES  127
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  243

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   310

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      68,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    73,     2,     2,    64,    74,     2,     2,
      66,    72,    57,    55,    76,    56,     2,    58,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    60,    69,
       2,     2,     2,    49,    65,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    67,     2,    75,    63,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    70,     2,    71,    51,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    50,    52,    53,    54,    59,    61,
      62
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   342,   342,   343,   344,   345,   346,   349,   350,   353,
     354,   355,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,   390,   391,   393,   395,   396,   397,
     398,   399,   400,   401,   402,   403,   404,   405,   406,   407,
     408,   409,   410,   411,   412,   413,   414,   415,   416,   417,
     418,   422,   425,   428,   432,   433,   434,   435,   436,   437,
     440,   441,   444,   445,   446,   447,   448,   449,   450,   451,
     454,   455,   456,   457,   458,   459,   460,   461,   463,   467,
     471,   472,   475,   477,   479,   481,   485,   486,   489,   490,
     491,   492,   493,   496,   499,   500,   501,   502,   505,   508,
     511,   512,   515,   518,   522,   523,   524,   525
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "END_OF_INPUT", "ERROR", "STR_CONST",
  "NUM_CONST", "NULL_CONST", "SYMBOL", "FUNCTION", "INCOMPLETE_STRING",
  "LEFT_ASSIGN", "EQ_ASSIGN", "RIGHT_ASSIGN", "LBB", "FOR", "IN", "IF",
  "ELSE", "WHILE", "NEXT", "BREAK", "REPEAT", "GT", "GE", "LT", "LE", "EQ",
  "NE", "AND", "OR", "AND2", "OR2", "NS_GET", "NS_GET_INT", "COMMENT",
  "LINE_DIRECTIVE", "SYMBOL_FORMALS", "EQ_FORMALS", "EQ_SUB", "SYMBOL_SUB",
  "SYMBOL_FUNCTION_CALL", "SYMBOL_PACKAGE", "COLON_ASSIGN", "SLOT",
  "ANNOTATION_OPERATOR", "DELIMITED_ANNOTATION_BEGIN",
  "DELIMITED_ANNOTATION_END", "FUNCTION_OPERATOR", "'?'", "LOW", "'~'",
  "TILDE", "UNOT", "NOT", "'+'", "'-'", "'*'", "'/'", "SPECIAL", "':'",
  "UMINUS", "UPLUS", "'^'", "'$'", "'@'", "'('", "'['", "'\\n'", "';'",
  "'{'", "'}'", "')'", "'!'", "'%'", "']'", "','", "$accept", "prog",
  "expr_or_assign", "equal_assign", "expr", "cond", "ifcond", "forcond",
  "exprlist", "sublist", "sub", "formlist", "cr", "preanno",
  "annotated_symbol", "annotation", "datatype", "atomictype",
  "compositetype", "arraytype", "listtype", "functiontype", "unittype",
  "uniontype", "datatypelist", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,    63,
     304,   126,   305,   306,   307,    43,    45,    42,    47,   308,
      58,   309,   310,    94,    36,    64,    40,    91,    10,    59,
     123,   125,    41,    33,    37,    93,    44
};
# endif

#define YYPACT_NINF -170

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-170)))

#define YYTABLE_NINF -114

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-114)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     252,  -170,  -170,    81,  -170,  -170,   101,   -49,   -16,    -8,
       4,  -170,  -170,   528,    -7,   597,   666,   666,   666,   666,
     528,  -170,   528,   666,   119,   -40,  -170,   924,    11,   144,
     179,   181,    -3,    67,   528,   666,   528,   666,   528,  -170,
    -170,     2,    -5,   321,  -170,  -170,  -170,  -170,  -170,  -170,
    -170,   -22,   459,   735,  -170,    38,   -21,    -7,   597,  1266,
    1374,   108,   108,    52,  -170,    85,  1394,  -170,  -170,  -170,
     666,   528,   666,   804,   666,   666,   666,   666,   666,   666,
     666,   666,   666,   666,   666,   666,   666,   666,   666,   666,
     666,   666,   666,   186,   187,   804,   804,   666,  -170,  -170,
    -170,  -170,  -170,  -170,  -170,  -170,   126,    -7,   597,   -12,
      72,   127,   132,  -170,   981,   125,  1038,  -170,  -170,  -170,
     -21,   106,   107,    36,     5,    -7,    -7,   804,  -170,  1152,
      -7,   597,   528,   390,   133,  -170,   528,   528,  -170,  1266,
    -170,  1320,   111,   178,   130,  1209,   123,  -170,  1448,  1448,
    1448,  1448,  1448,  1448,  1394,   102,  1394,   102,  1266,  1374,
     163,   163,   217,   217,   249,   108,   108,  -170,  -170,  -170,
    -170,   128,   129,  1209,   666,   -21,   154,  -170,    -1,  -170,
      75,  -170,   666,   666,  -170,   528,  -170,  -170,  -170,   155,
      -5,  -170,   -46,   -46,   131,   321,   158,  -170,  1152,  1266,
     666,  -170,  -170,   666,   666,   666,   135,   140,  -170,  -170,
    1209,  -170,   873,   190,   195,  -170,  -170,  1209,  1095,  -170,
      -7,  -170,   -21,  -170,   528,  1266,  1209,  1209,  1209,  -170,
     804,    -7,  -170,   666,   666,  -170,   -46,  -170,   321,  1209,
    1209,   528,  -170
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     6,     2,    13,    12,    14,    15,     0,     0,     0,
       0,    67,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     3,    74,     0,     0,     0,     8,     7,     0,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,    52,
     113,     0,     0,     0,   108,   109,   114,   115,   116,   112,
     117,    15,     0,    74,   107,     0,   106,     0,     0,    22,
      21,    19,    18,     0,    75,     0,    20,     1,     4,     5,
       0,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,    82,     0,    58,    57,
      62,    61,    56,    55,    60,    59,    91,     0,     0,     0,
       0,    92,     0,    50,     0,    48,     0,    51,   122,   125,
     124,   108,   109,     0,     0,     0,     0,    82,    10,    69,
       0,     0,     0,     0,     0,    17,    79,    77,    16,    43,
       9,    44,    13,    14,    15,    83,    99,    80,    38,    37,
      33,    34,    35,    36,    39,    40,    41,    42,    32,    31,
      24,    25,    26,    27,    29,    23,    28,    64,    63,    66,
      65,    99,    99,    30,     0,   100,     0,    99,     0,   102,
       0,   103,     0,     0,    72,     0,    71,   110,   111,     0,
       0,   119,   123,   121,    99,   100,     0,    11,    70,    69,
       0,    78,    76,    86,    88,    84,     0,     0,    47,    54,
      93,   101,     0,    95,    96,   104,   105,    94,     0,    49,
       0,   127,   126,   118,   101,    70,    87,    89,    85,    53,
      82,     0,    45,     0,     0,    73,   120,    81,     0,    97,
      98,     0,    46
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -170,  -170,     0,   -25,    14,  -170,  -170,  -170,  -170,   -72,
     -19,  -170,  -169,  -170,     3,   -52,    25,   -37,   -27,  -170,
    -170,  -170,  -170,  -170,    20
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    24,    54,    26,    27,    38,    36,    34,    65,   146,
     147,   109,   207,   110,   119,    55,    56,    44,    45,    46,
      47,    48,    49,    50,   123
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      25,    40,   126,    40,   121,   106,   134,   213,   212,   125,
      40,    30,    31,    39,   122,   121,    98,    32,   128,    99,
      63,   127,    64,   171,   172,   122,  -113,   126,    68,    69,
      59,    60,    61,    62,   113,   111,   115,    66,   117,    43,
     107,   108,   107,   108,   107,   108,   127,   107,   108,   114,
      33,   116,    63,    64,  -113,   194,   176,   129,    35,    41,
     177,    41,   124,    42,   178,    42,   120,   120,    41,   241,
      37,   140,    42,   124,   118,   112,   191,   120,   120,   196,
     179,   190,   133,   215,   139,   132,   141,   145,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   197,   189,   145,
     145,   173,   190,   181,    28,    29,    73,   107,   108,    67,
     107,   108,    73,   203,   135,    74,    75,    76,    77,    78,
      79,    80,   175,    82,    30,    31,   201,   202,   174,   182,
     180,   145,   205,   185,    28,    29,   198,   199,   183,   100,
     192,   193,   101,   136,   137,   195,   138,    86,    87,    88,
      89,    90,    91,    30,    31,    92,    93,    94,    95,    96,
     128,    92,    93,    94,    95,    96,    97,    73,   187,   188,
     200,   214,    97,   216,   102,   219,   104,   103,   210,   105,
     204,   167,   169,   221,   168,   170,   217,   218,   206,   197,
     208,   211,   233,   220,   209,   224,   223,   234,     0,   129,
     229,   237,   232,   128,   225,   222,   230,   226,   227,   228,
      88,    89,    90,    91,     0,     0,    92,    93,    94,    95,
      96,    73,     0,     0,     0,     0,     0,    97,   198,     0,
       0,   242,     0,     0,   145,   236,     0,   239,   240,     0,
       0,     0,   129,     1,     0,     2,   238,     3,     4,     5,
       6,     7,     0,    73,     0,     0,     0,     8,     0,     9,
       0,    10,    11,    12,    13,     0,    90,    91,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,    14,    15,     0,
       0,    16,     0,    17,     0,     0,     0,    18,    19,    91,
       0,     0,    92,    93,    94,    95,    96,     0,    20,     0,
      21,     0,    22,    97,     0,    23,     3,     4,     5,     6,
       7,     0,     0,     0,     0,     0,     8,     0,     9,     0,
      10,    11,    12,    13,     0,     0,     0,     0,     0,     0,
       0,   125,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    14,    15,     0,   126,
      16,     0,    17,     0,     0,     0,    18,    19,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    20,   127,     0,
       0,    22,     0,     0,    23,     3,     4,     5,     6,     7,
       0,     0,     0,     0,     0,     8,     0,     9,     0,    10,
      11,    12,    13,     0,     0,     0,     0,     0,     0,     0,
     125,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,     0,   126,    16,
       0,    17,     0,     0,     0,    18,    19,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    20,   127,     0,     0,
      22,     0,     0,    23,     3,     4,     5,    51,     7,     0,
       0,     0,     0,     0,     8,     0,     9,     0,    10,    11,
      12,    13,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   130,   131,     0,     0,    16,     0,
      17,     0,     0,     0,    18,    19,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,     0,     0,     0,    53,
       0,   118,    23,     3,     4,     5,     6,     7,     0,     0,
       0,     0,     0,     8,     0,     9,     0,    10,    11,    12,
      13,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    14,    15,     0,     0,    16,     0,    17,
       0,     0,     0,    18,    19,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    20,     0,     0,     0,    22,     0,
       0,    23,     3,     4,     5,    51,     7,     0,     0,     0,
       0,     0,     8,     0,     9,     0,    10,    11,    12,    13,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    14,    15,     0,     0,    16,     0,    17,     0,
       0,     0,    18,    19,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,     0,     0,     0,    53,     0,     0,
      23,     3,     4,     5,     6,     7,     0,     0,     0,     0,
       0,     8,     0,     9,     0,    10,    11,    12,    13,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,    16,     0,    17,     0,     0,
       0,    18,    19,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    20,     0,     0,     0,    22,     0,     0,    23,
       3,     4,     5,    51,     7,     0,     0,     0,     0,     0,
       8,     0,     9,     0,    10,    11,    12,    13,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     130,   131,     0,     0,    16,     0,    17,     0,     0,     0,
      18,    19,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,     0,     0,     0,    53,     0,     0,    23,   142,
       4,   143,   144,     7,     0,     0,     0,     0,     0,     8,
       0,     9,     0,    10,    11,    12,    13,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,     0,     0,    16,     0,    17,     0,     0,     0,    18,
      19,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      20,     0,     0,     0,    22,     0,     0,    23,     3,     4,
       5,     6,     7,     0,     0,     0,     0,     0,     8,     0,
       9,     0,    10,    11,    12,    13,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   231,    15,
       0,     0,    16,     0,    17,     0,     0,     0,    18,    19,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    20,
       0,     0,     0,    22,     0,     0,    23,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    84,     0,    85,     0,     0,     0,    86,
      87,    88,    89,    90,    91,     0,     0,    92,    93,    94,
      95,    96,    70,     0,    72,    73,     0,     0,    97,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      84,     0,    85,     0,     0,     0,    86,    87,    88,    89,
      90,    91,     0,     0,    92,    93,    94,    95,    96,    70,
       0,    72,    73,   184,     0,    97,     0,     0,     0,     0,
       0,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    84,     0,    85,
       0,     0,     0,    86,    87,    88,    89,    90,    91,     0,
       0,    92,    93,    94,    95,    96,    70,     0,    72,    73,
     186,     0,    97,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    84,     0,    85,     0,     0,     0,
      86,    87,    88,    89,    90,    91,     0,     0,    92,    93,
      94,    95,    96,    70,    71,    72,    73,   235,     0,    97,
       0,     0,     0,     0,     0,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    85,     0,     0,     0,    86,    87,    88,
      89,    90,    91,     0,     0,    92,    93,    94,    95,    96,
      70,     0,    72,    73,     0,     0,    97,     0,     0,     0,
       0,     0,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    84,     0,
      85,     0,     0,     0,    86,    87,    88,    89,    90,    91,
       0,     0,    92,    93,    94,    95,    96,    70,     0,    72,
      73,     0,     0,    97,     0,     0,     0,     0,     0,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
       0,    86,    87,    88,    89,    90,    91,     0,     0,    92,
      93,    94,    95,    96,    73,     0,     0,     0,     0,     0,
      97,     0,     0,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,    86,    87,    88,    89,    90,
      91,     0,     0,    92,    93,    94,    95,    96,    73,     0,
       0,     0,     0,     0,    97,     0,     0,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,     0,    73,     0,
       0,     0,     0,     0,     0,     0,     0,    74,    75,    76,
      77,    78,    79,     0,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,    91,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,     0,     0,    97,    86,
      87,    88,    89,    90,    91,     0,     0,    92,    93,    94,
      95,    96,    73,     0,     0,     0,     0,     0,    97,     0,
       0,  -114,  -114,  -114,  -114,  -114,  -114,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,    88,    89,    90,    91,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
       0,     0,    97
};

static const yytype_int16 yycheck[] =
{
       0,     8,    48,     8,    41,     8,    58,     8,   177,    30,
       8,    33,    34,    13,    41,    52,     5,    66,    43,     8,
      20,    67,    22,    95,    96,    52,    48,    48,    68,    69,
      16,    17,    18,    19,    34,    32,    36,    23,    38,    14,
      45,    46,    45,    46,    45,    46,    67,    45,    46,    35,
      66,    37,    52,    53,    76,   127,   108,    43,    66,    66,
      72,    66,    42,    70,    76,    70,    41,    42,    66,   238,
      66,    71,    70,    53,    72,     8,    71,    52,    53,   131,
       8,    76,    57,     8,    70,    47,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,   132,    72,    95,
      96,    97,    76,   110,    33,    34,    14,    45,    46,     0,
      45,    46,    14,    12,    72,    23,    24,    25,    26,    27,
      28,    29,   107,    31,    33,    34,   136,   137,    12,    12,
      68,   127,    12,    18,    33,    34,   132,   133,    16,     5,
     125,   126,     8,    68,    69,   130,    71,    55,    56,    57,
      58,    59,    60,    33,    34,    63,    64,    65,    66,    67,
     195,    63,    64,    65,    66,    67,    74,    14,    72,    72,
      47,   178,    74,   180,     5,   185,     5,     8,   174,     8,
      12,     5,     5,   190,     8,     8,   182,   183,    75,   224,
      72,    47,    12,    48,    75,    47,    75,    12,    -1,   195,
      75,   230,   212,   238,   200,   190,    76,   203,   204,   205,
      57,    58,    59,    60,    -1,    -1,    63,    64,    65,    66,
      67,    14,    -1,    -1,    -1,    -1,    -1,    74,   224,    -1,
      -1,   241,    -1,    -1,   230,   220,    -1,   233,   234,    -1,
      -1,    -1,   238,     1,    -1,     3,   231,     5,     6,     7,
       8,     9,    -1,    14,    -1,    -1,    -1,    15,    -1,    17,
      -1,    19,    20,    21,    22,    -1,    59,    60,    -1,    -1,
      63,    64,    65,    66,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    45,    46,    -1,
      -1,    49,    -1,    51,    -1,    -1,    -1,    55,    56,    60,
      -1,    -1,    63,    64,    65,    66,    67,    -1,    66,    -1,
      68,    -1,    70,    74,    -1,    73,     5,     6,     7,     8,
       9,    -1,    -1,    -1,    -1,    -1,    15,    -1,    17,    -1,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    45,    46,    -1,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    67,    -1,
      -1,    70,    -1,    -1,    73,     5,     6,     7,     8,     9,
      -1,    -1,    -1,    -1,    -1,    15,    -1,    17,    -1,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    45,    46,    -1,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,
      70,    -1,    -1,    73,     5,     6,     7,     8,     9,    -1,
      -1,    -1,    -1,    -1,    15,    -1,    17,    -1,    19,    20,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    49,    -1,
      51,    -1,    -1,    -1,    55,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,    70,
      -1,    72,    73,     5,     6,     7,     8,     9,    -1,    -1,
      -1,    -1,    -1,    15,    -1,    17,    -1,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    45,    46,    -1,    -1,    49,    -1,    51,
      -1,    -1,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,    70,    -1,
      -1,    73,     5,     6,     7,     8,     9,    -1,    -1,    -1,
      -1,    -1,    15,    -1,    17,    -1,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    45,    46,    -1,    -1,    49,    -1,    51,    -1,
      -1,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    -1,    70,    -1,    -1,
      73,     5,     6,     7,     8,     9,    -1,    -1,    -1,    -1,
      -1,    15,    -1,    17,    -1,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    45,    46,    -1,    -1,    49,    -1,    51,    -1,    -1,
      -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    -1,    -1,    -1,    70,    -1,    -1,    73,
       5,     6,     7,     8,     9,    -1,    -1,    -1,    -1,    -1,
      15,    -1,    17,    -1,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      45,    46,    -1,    -1,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    -1,    -1,    70,    -1,    -1,    73,     5,
       6,     7,     8,     9,    -1,    -1,    -1,    -1,    -1,    15,
      -1,    17,    -1,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,
      46,    -1,    -1,    49,    -1,    51,    -1,    -1,    -1,    55,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    -1,    -1,    -1,    70,    -1,    -1,    73,     5,     6,
       7,     8,     9,    -1,    -1,    -1,    -1,    -1,    15,    -1,
      17,    -1,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    46,
      -1,    -1,    49,    -1,    51,    -1,    -1,    -1,    55,    56,
      -1,    -1,    -1,    -1,    -1,    11,    12,    13,    14,    66,
      -1,    -1,    -1,    70,    -1,    -1,    73,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    -1,    51,    -1,    -1,    -1,    55,
      56,    57,    58,    59,    60,    -1,    -1,    63,    64,    65,
      66,    67,    11,    -1,    13,    14,    -1,    -1,    74,    -1,
      -1,    -1,    -1,    -1,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      49,    -1,    51,    -1,    -1,    -1,    55,    56,    57,    58,
      59,    60,    -1,    -1,    63,    64,    65,    66,    67,    11,
      -1,    13,    14,    72,    -1,    74,    -1,    -1,    -1,    -1,
      -1,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,    51,
      -1,    -1,    -1,    55,    56,    57,    58,    59,    60,    -1,
      -1,    63,    64,    65,    66,    67,    11,    -1,    13,    14,
      72,    -1,    74,    -1,    -1,    -1,    -1,    -1,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    57,    58,    59,    60,    -1,    -1,    63,    64,
      65,    66,    67,    11,    12,    13,    14,    72,    -1,    74,
      -1,    -1,    -1,    -1,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    55,    56,    57,
      58,    59,    60,    -1,    -1,    63,    64,    65,    66,    67,
      11,    -1,    13,    14,    -1,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      51,    -1,    -1,    -1,    55,    56,    57,    58,    59,    60,
      -1,    -1,    63,    64,    65,    66,    67,    11,    -1,    13,
      14,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    55,    56,    57,    58,    59,    60,    -1,    -1,    63,
      64,    65,    66,    67,    14,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    55,    56,    57,    58,    59,
      60,    -1,    -1,    63,    64,    65,    66,    67,    14,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    24,    25,
      26,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      56,    57,    58,    59,    60,    -1,    -1,    63,    64,    65,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    74,    55,
      56,    57,    58,    59,    60,    -1,    -1,    63,    64,    65,
      66,    67,    14,    -1,    -1,    -1,    -1,    -1,    74,    -1,
      -1,    23,    24,    25,    26,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    56,    57,    58,    59,    60,    -1,
      -1,    63,    64,    65,    66,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    74
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,     5,     6,     7,     8,     9,    15,    17,
      19,    20,    21,    22,    45,    46,    49,    51,    55,    56,
      66,    68,    70,    73,    78,    79,    80,    81,    33,    34,
      33,    34,    66,    66,    84,    66,    83,    66,    82,    79,
       8,    66,    70,    93,    94,    95,    96,    97,    98,    99,
     100,     8,    66,    70,    79,    92,    93,    45,    46,    81,
      81,    81,    81,    79,    79,    85,    81,     0,    68,    69,
      11,    12,    13,    14,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    49,    51,    55,    56,    57,    58,
      59,    60,    63,    64,    65,    66,    67,    74,     5,     8,
       5,     8,     5,     8,     5,     8,     8,    45,    46,    88,
      90,    91,     8,    79,    81,    79,    81,    79,    72,    91,
      93,    94,    95,   101,   101,    30,    48,    67,    80,    81,
      45,    46,    47,    93,    92,    72,    68,    69,    71,    81,
      79,    81,     5,     7,     8,    81,    86,    87,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,     5,     8,     5,
       8,    86,    86,    81,    12,    93,    92,    72,    76,     8,
      68,    91,    12,    16,    72,    18,    72,    72,    72,    72,
      76,    71,    93,    93,    86,    93,    92,    80,    81,    81,
      47,    79,    79,    12,    12,    12,    75,    89,    72,    75,
      81,    47,    89,     8,    91,     8,    91,    81,    81,    79,
      48,    91,    93,    75,    47,    81,    81,    81,    81,    75,
      76,    45,    79,    12,    12,    72,    93,    87,    93,    81,
      81,    89,    79
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    77,    78,    78,    78,    78,    78,    79,    79,    80,
      80,    80,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    82,    83,    84,    85,    85,    85,    85,    85,    85,
      86,    86,    87,    87,    87,    87,    87,    87,    87,    87,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    89,
      90,    90,    91,    91,    91,    91,    92,    92,    93,    93,
      93,    93,    93,    94,    95,    95,    95,    95,    96,    97,
      98,    98,    99,   100,   101,   101,   101,   101
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     2,     1,     1,     1,     3,
       3,     4,     1,     1,     1,     1,     3,     3,     2,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     6,     9,     4,     3,     5,
       3,     3,     2,     5,     4,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     3,
       4,     3,     3,     5,     0,     1,     3,     2,     3,     2,
       1,     4,     0,     1,     2,     3,     2,     3,     2,     3,
       0,     1,     1,     3,     3,     3,     3,     5,     5,     0,
       2,     3,     2,     2,     3,     3,     1,     1,     1,     1,
       3,     3,     1,     1,     1,     1,     1,     1,     4,     3,
       5,     3,     2,     3,     1,     1,     3,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 342 "gram.y" /* yacc.c:1646  */
    { YYACCEPT; }
#line 2135 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 343 "gram.y" /* yacc.c:1646  */
    { yyresult = xxvalue(NULL,2,NULL);	goto yyreturn; }
#line 2141 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 344 "gram.y" /* yacc.c:1646  */
    { yyresult = xxvalue((yyvsp[-1]),3,&(yylsp[-1]));	goto yyreturn; }
#line 2147 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 345 "gram.y" /* yacc.c:1646  */
    { yyresult = xxvalue((yyvsp[-1]),4,&(yylsp[-1]));	goto yyreturn; }
#line 2153 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 346 "gram.y" /* yacc.c:1646  */
    { YYABORT; }
#line 2159 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 349 "gram.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 2165 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 350 "gram.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 2171 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 353 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0])); }
#line 2177 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 354 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxannotate((yyvsp[0]), (yyvsp[-1])); }
#line 2183 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 355 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxannotate((yyvsp[0]), (yyvsp[-2])); }
#line 2189 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 358 "gram.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]);	setId( (yyval), (yyloc)); }
#line 2195 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 359 "gram.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]);	setId( (yyval), (yyloc)); }
#line 2201 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 360 "gram.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]);	setId( (yyval), (yyloc)); }
#line 2207 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 361 "gram.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]);	setId( (yyval), (yyloc)); }
#line 2213 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 362 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxexprlist((yyvsp[-2]),&(yylsp[-2]),(yyvsp[-1])); setId( (yyval), (yyloc)); }
#line 2219 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 363 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxparen((yyvsp[-2]),(yyvsp[-1]));	setId( (yyval), (yyloc)); }
#line 2225 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 364 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxunary((yyvsp[-1]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2231 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 365 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxunary((yyvsp[-1]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2237 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 366 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxunary((yyvsp[-1]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2243 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 367 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxunary((yyvsp[-1]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2249 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 368 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxunary((yyvsp[-1]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2255 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 369 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2261 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 370 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2267 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 371 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2273 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 372 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2279 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 373 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2285 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 374 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2291 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 375 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2297 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 376 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2303 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 377 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2309 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 378 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2315 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 379 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2321 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 380 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2327 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 381 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2333 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 382 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2339 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 383 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2345 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 384 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2351 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 385 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2357 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 386 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2363 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 387 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2369 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 388 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2375 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 389 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2381 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 390 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[0]),(yyvsp[-2]));	setId( (yyval), (yyloc)); }
#line 2387 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 392 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxdefun((yyvsp[-5]),(yyvsp[-3]),(yyvsp[0]),&(yyloc)); 	setId( (yyval), (yyloc)); }
#line 2393 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 394 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxdefun((yyvsp[-8]), (yyvsp[-6]), xxannotate((yyvsp[0]), (yyvsp[-2])), &(yyloc)); 	setId( (yyval), (yyloc)); }
#line 2399 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 395 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxfuncall((yyvsp[-3]),(yyvsp[-1]));  setId( (yyval), (yyloc)); modif_token( &(yylsp[-3]), SYMBOL_FUNCTION_CALL ) ; }
#line 2405 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 396 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxif((yyvsp[-2]),(yyvsp[-1]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2411 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 397 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxifelse((yyvsp[-4]),(yyvsp[-3]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2417 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 398 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxfor((yyvsp[-2]),(yyvsp[-1]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2423 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 399 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxwhile((yyvsp[-2]),(yyvsp[-1]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2429 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 400 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxrepeat((yyvsp[-1]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2435 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 401 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxsubscript((yyvsp[-4]),(yyvsp[-3]),(yyvsp[-2]));	setId( (yyval), (yyloc)); }
#line 2441 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 402 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxsubscript((yyvsp[-3]),(yyvsp[-2]),(yyvsp[-1]));	setId( (yyval), (yyloc)); }
#line 2447 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 403 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));      setId( (yyval), (yyloc)); modif_token( &(yylsp[-2]), SYMBOL_PACKAGE ) ; }
#line 2453 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 404 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));      setId( (yyval), (yyloc)); modif_token( &(yylsp[-2]), SYMBOL_PACKAGE ) ; }
#line 2459 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 405 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2465 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 406 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2471 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 407 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));      setId( (yyval), (yyloc)); modif_token( &(yylsp[-2]), SYMBOL_PACKAGE ) ;}
#line 2477 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 408 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));      setId( (yyval), (yyloc)); modif_token( &(yylsp[-2]), SYMBOL_PACKAGE ) ;}
#line 2483 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 409 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2489 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 410 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2495 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 411 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2501 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 412 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2507 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 413 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));      setId( (yyval), (yyloc)); modif_token( &(yylsp[0]), SLOT ) ; }
#line 2513 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 414 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2519 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 415 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxnxtbrk((yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2525 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 416 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxnxtbrk((yyvsp[0]));	setId( (yyval), (yyloc)); }
#line 2531 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 417 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxannotate((yyvsp[0]), (yyvsp[-1])); }
#line 2537 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 418 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxannotate((yyvsp[0]), (yyvsp[-2])); }
#line 2543 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 422 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxcond((yyvsp[-1]));   }
#line 2549 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 425 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxifcond((yyvsp[-1])); }
#line 2555 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 428 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxforcond((yyvsp[-3]),(yyvsp[-1]));	setId( (yyval), (yyloc)); }
#line 2561 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 432 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxexprlist0();	setId( (yyval), (yyloc)); }
#line 2567 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 433 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxexprlist1((yyvsp[0]), &(yylsp[0])); }
#line 2573 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 434 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxexprlist2((yyvsp[-2]), (yyvsp[0]), &(yylsp[0])); }
#line 2579 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 435 "gram.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-1]);		setId( (yyval), (yyloc)); }
#line 2585 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 436 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxexprlist2((yyvsp[-2]), (yyvsp[0]), &(yylsp[0])); }
#line 2591 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 437 "gram.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-1]);}
#line 2597 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 440 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxsublist1((yyvsp[0]));	  }
#line 2603 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 441 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxsublist2((yyvsp[-3]),(yyvsp[0])); }
#line 2609 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 444 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxsub0();	 }
#line 2615 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 445 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxsub1((yyvsp[0]), &(yylsp[0]));  }
#line 2621 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 446 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxsymsub0((yyvsp[-1]), &(yylsp[-1])); 	modif_token( &(yylsp[0]), EQ_SUB ) ; modif_token( &(yylsp[-1]), SYMBOL_SUB ) ; }
#line 2627 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 447 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxsymsub1((yyvsp[-2]),(yyvsp[0]), &(yylsp[-2])); 	modif_token( &(yylsp[-1]), EQ_SUB ) ; modif_token( &(yylsp[-2]), SYMBOL_SUB ) ; }
#line 2633 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 448 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxsymsub0((yyvsp[-1]), &(yylsp[-1])); 	modif_token( &(yylsp[0]), EQ_SUB ) ; }
#line 2639 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 449 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxsymsub1((yyvsp[-2]),(yyvsp[0]), &(yylsp[-2])); 	modif_token( &(yylsp[-1]), EQ_SUB ) ; }
#line 2645 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 450 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxnullsub0(&(yylsp[-1])); 	modif_token( &(yylsp[0]), EQ_SUB ) ; }
#line 2651 "y.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 451 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxnullsub1((yyvsp[0]), &(yylsp[-2])); 	modif_token( &(yylsp[-1]), EQ_SUB ) ; }
#line 2657 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 454 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxnullformal(); }
#line 2663 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 455 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxfirstformal0((yyvsp[0])); 	modif_token( &(yylsp[0]), SYMBOL_FORMALS ) ; }
#line 2669 "y.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 456 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxfirstformal0((yyvsp[0])); 	modif_token( &(yylsp[0]), SYMBOL_FORMALS ); }
#line 2675 "y.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 457 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxfirstformal1((yyvsp[-2]),(yyvsp[0])); 	modif_token( &(yylsp[-2]), SYMBOL_FORMALS ) ; modif_token( &(yylsp[-1]), EQ_FORMALS ) ; }
#line 2681 "y.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 458 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxfirstformal1((yyvsp[-2]),(yyvsp[0])); modif_token( &(yylsp[-2]), SYMBOL_FORMALS ) ; modif_token( &(yylsp[-1]), EQ_FORMALS ) ; }
#line 2687 "y.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 459 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxaddformal0((yyvsp[-2]),(yyvsp[0]), &(yylsp[0]));   modif_token( &(yylsp[0]), SYMBOL_FORMALS ) ; }
#line 2693 "y.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 460 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxaddformal0((yyvsp[-2]),(yyvsp[0]), &(yylsp[0]));   modif_token( &(yylsp[0]), SYMBOL_FORMALS ) ; }
#line 2699 "y.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 462 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxaddformal1((yyvsp[-4]),(yyvsp[-2]),(yyvsp[0]),&(yylsp[-2])); modif_token( &(yylsp[-2]), SYMBOL_FORMALS ) ; modif_token( &(yylsp[-1]), EQ_FORMALS ) ;}
#line 2705 "y.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 464 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxaddformal1((yyvsp[-4]),(yyvsp[-2]),(yyvsp[0]),&(yylsp[-2])); modif_token( &(yylsp[-2]), SYMBOL_FORMALS ) ; modif_token( &(yylsp[-1]), EQ_FORMALS ) ;}
#line 2711 "y.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 467 "gram.y" /* yacc.c:1646  */
    { EatLines = 1; }
#line 2717 "y.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 471 "gram.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 2723 "y.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 472 "gram.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-1]); }
#line 2729 "y.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 476 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxannotate((yyvsp[0]), (yyvsp[-1])); setId((yyval), (yyloc)); }
#line 2735 "y.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 478 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxannotate((yyvsp[0]), (yyvsp[-1])); setId((yyval), (yyloc)); }
#line 2741 "y.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 480 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxannotate((yyvsp[-1]), (yyvsp[-2])); setId((yyval), (yyloc)); }
#line 2747 "y.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 482 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxannotate((yyvsp[-1]), (yyvsp[-2])); setId((yyval), (yyloc)); }
#line 2753 "y.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 485 "gram.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]);	setId((yyval), (yyloc)); }
#line 2759 "y.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 486 "gram.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]);	setId((yyval), (yyloc)); }
#line 2765 "y.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 489 "gram.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]);	setId((yyval), (yyloc)); }
#line 2771 "y.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 490 "gram.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]);	setId((yyval), (yyloc)); }
#line 2777 "y.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 491 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxparen((yyvsp[-2]),(yyvsp[-1]));	setId( (yyval), (yyloc)); }
#line 2783 "y.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 492 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxparen((yyvsp[-2]),(yyvsp[-1]));	setId( (yyval), (yyloc)); }
#line 2789 "y.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 496 "gram.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]);	setId((yyval), (yyloc)); }
#line 2795 "y.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 499 "gram.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]);	setId((yyval), (yyloc)); }
#line 2801 "y.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 500 "gram.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]);	setId((yyval), (yyloc)); }
#line 2807 "y.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 501 "gram.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]);	setId((yyval), (yyloc)); }
#line 2813 "y.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 502 "gram.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]);	setId((yyval), (yyloc)); }
#line 2819 "y.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 505 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxsubscript((yyvsp[-3]),(yyvsp[-2]),(yyvsp[-1]));	setId((yyval), (yyloc)); }
#line 2825 "y.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 508 "gram.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-1]); setId((yyval), (yyloc)); }
#line 2831 "y.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 511 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]), (yyvsp[-3]), (yyvsp[0]));	setId((yyval), (yyloc)); }
#line 2837 "y.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 512 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]), (yyvsp[-2]), (yyvsp[0]));	setId((yyval), (yyloc)); }
#line 2843 "y.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 515 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxnullformal(); }
#line 2849 "y.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 518 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]), (yyvsp[-2]), (yyvsp[0])); setId((yyval), (yyloc)); }
#line 2855 "y.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 522 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxexprlist1((yyvsp[0]), &(yylsp[0])); setId((yyval), (yyloc)); }
#line 2861 "y.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 523 "gram.y" /* yacc.c:1646  */
    { (yyval) = xxexprlist1((yyvsp[0]), &(yylsp[0])); setId((yyval), (yyloc)); }
#line 2867 "y.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 524 "gram.y" /* yacc.c:1646  */
    { xxexprlist2((yyvsp[-2]), (yyvsp[0]), &(yylsp[0])); }
#line 2873 "y.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 525 "gram.y" /* yacc.c:1646  */
    { xxexprlist2((yyvsp[-2]), (yyvsp[0]), &(yylsp[0])); }
#line 2879 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2883 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 527 "gram.y" /* yacc.c:1906  */



/*----------------------------------------------------------------------------*/

static int (*ptr_getc)(void);

/* Private pushback, since file ungetc only guarantees one byte.
   We need up to one MBCS-worth */
#define DECLARE_YYTEXT_BUFP(bp) char *bp = yytext ;
#define YYTEXT_PUSH(c, bp) do { \
    if ((bp) - yytext >= sizeof(yytext) - 1){ \
		error(_("input buffer overflow at line %d"), ParseState.xxlineno); \
	} \
    *(bp)++ = ((char)c);			\
} while(0) ;

#define PUSHBACK_BUFSIZE 16
static int pushback[PUSHBACK_BUFSIZE];
static unsigned int npush = 0;

static int prevpos = 0;
static int prevlines[PUSHBACK_BUFSIZE];
static int prevcols[PUSHBACK_BUFSIZE];
static int prevbytes[PUSHBACK_BUFSIZE];
static int prevparse[PUSHBACK_BUFSIZE];

static int xxgetc(void)
{
    int c, oldpos;

    if(npush) c = pushback[--npush]; else  c = ptr_getc();

    oldpos = prevpos;
    prevpos = (prevpos + 1) % PUSHBACK_BUFSIZE;
    prevbytes[prevpos] = ParseState.xxbyteno;
    prevlines[prevpos] = ParseState.xxlineno;  
    prevparse[prevpos] = ParseState.xxparseno;

    /* We only advance the column for the 1st byte in UTF-8, so handle later bytes specially */
    if (0x80 <= (unsigned char)c && (unsigned char)c <= 0xBF && known_to_be_utf8)  {
    	ParseState.xxcolno--;   
    	prevcols[prevpos] = prevcols[oldpos];
    } else 
    	prevcols[prevpos] = ParseState.xxcolno;
    	
    if (c == EOF) {
	EndOfFile = 1;
	return R_EOF;
    }
    R_ParseContextLast = (R_ParseContextLast + 1) % PARSE_CONTEXT_SIZE;
    R_ParseContext[R_ParseContextLast] = (char) c;

    if (c == '\n') {
	ParseState.xxlineno += 1;
	ParseState.xxcolno = 0;
    	ParseState.xxbyteno = 0;
    	ParseState.xxparseno += 1;
    } else {
        ParseState.xxcolno++;
    	ParseState.xxbyteno++;
    }

    if (c == '\t') ParseState.xxcolno = ((ParseState.xxcolno + 7) & ~7);
    
    R_ParseContextLine = ParseState.xxlineno;    

    xxcharcount++;
    return c;
}

static int xxungetc(int c)
{
    /* this assumes that c was the result of xxgetc; if not, some edits will be needed */
    ParseState.xxlineno = prevlines[prevpos];
    ParseState.xxbyteno = prevbytes[prevpos];
    ParseState.xxcolno  = prevcols[prevpos];
    ParseState.xxparseno = prevparse[prevpos];
    
    prevpos = (prevpos + PUSHBACK_BUFSIZE - 1) % PUSHBACK_BUFSIZE;

    R_ParseContextLine = ParseState.xxlineno;

    xxcharcount--;
    R_ParseContext[R_ParseContextLast] = '\0';
    /* precaution as to how % is implemented for < 0 numbers */
    R_ParseContextLast = (R_ParseContextLast + PARSE_CONTEXT_SIZE -1) % PARSE_CONTEXT_SIZE;
    if(npush >= PUSHBACK_BUFSIZE) return EOF;
    pushback[npush++] = c;
    return c;
}

/*
 * Increments/inits the token/grouping counter
 */
static void incrementId(void){
	identifier++; 
}

static void initId(void){
	identifier = 0 ;
}

static SEXP makeSrcref(YYLTYPE *lloc, SEXP srcfile)
{
    SEXP val;

    PROTECT(val = allocVector(INTSXP, 8));
    INTEGER(val)[0] = lloc->first_line;
    INTEGER(val)[1] = lloc->first_byte;
    INTEGER(val)[2] = lloc->last_line;
    INTEGER(val)[3] = lloc->last_byte;
    INTEGER(val)[4] = lloc->first_column;
    INTEGER(val)[5] = lloc->last_column;
    INTEGER(val)[6] = lloc->first_parsed;
    INTEGER(val)[7] = lloc->last_parsed;
    setAttrib(val, R_SrcfileSymbol, srcfile);
    setAttrib(val, R_ClassSymbol, mkString("srcref"));
    UNPROTECT(1);
    return val;
}

static SEXP attachSrcrefs(SEXP val)
{
    SEXP srval;

    PROTECT(val);
    PROTECT(srval = PairToVectorList(SrcRefs));
    
    setAttrib(val, R_SrcrefSymbol, srval);
    setAttrib(val, R_SrcfileSymbol, ParseState.SrcFile);
    {
	YYLTYPE wholeFile;
	wholeFile.first_line = 1;
	wholeFile.first_byte = 0;
	wholeFile.first_column = 0;
	wholeFile.last_line = ParseState.xxlineno;
	wholeFile.last_byte = ParseState.xxbyteno;
	wholeFile.last_column = ParseState.xxcolno;
	wholeFile.first_parsed = 1;
	wholeFile.last_parsed = ParseState.xxparseno;
	setAttrib(val, R_WholeSrcrefSymbol, makeSrcref(&wholeFile, ParseState.SrcFile));
    }
    REPROTECT(SrcRefs = R_NilValue, srindex);
    ParseState.didAttach = TRUE;
    UNPROTECT(2);
    return val;
}

static int xxvalue(SEXP v, int k, YYLTYPE *lloc)
{
    if (k > 2) {
	if (ParseState.keepSrcRefs)
	    REPROTECT(SrcRefs = listAppend(SrcRefs, list1(makeSrcref(lloc, ParseState.SrcFile))), srindex);
	UNPROTECT_PTR(v);
    }
    R_CurrentExpr = v;
    return k;
}

static SEXP xxnullformal()
{
  SEXP ans, annotations;
    PROTECT(ans = R_NilValue);
    PROTECT(annotations = R_NilValue);
    setAttrib(ans, R_AnnotationsSymbol, annotations);
    return ans;
}

static SEXP xxfirstformal0(SEXP sym)
{
	SEXP ans, annotations, annotation;
	UNPROTECT_PTR(sym);
	if (GenerateCode) {
    PROTECT(annotation = getAttrib(sym, R_AnnotationsSymbol));
    ATTRIB(sym) = R_NilValue;
		PROTECT(ans = FirstArg(R_MissingArg, sym));
		PROTECT(annotations = list1(annotation));
    SET_TAG(annotations, sym);
		setAttrib(ans, R_AnnotationsSymbol, annotations);

	}
	else
		PROTECT(ans = R_NilValue);
	return ans;
}

static SEXP xxfirstformal1(SEXP sym, SEXP expr)
{
  SEXP ans, annotation, annotations;
    if (GenerateCode) {
      PROTECT(annotation = getAttrib(sym, R_AnnotationsSymbol));
      ATTRIB(sym) = R_NilValue;
      PROTECT(ans = FirstArg(expr, sym));
      PROTECT(annotations = list1(annotation));
      SET_TAG(annotations, sym);
      setAttrib(ans, R_AnnotationsSymbol, annotations);
    }
    else {
      PROTECT(ans = R_NilValue);
    }
    UNPROTECT_PTR(expr);
    UNPROTECT_PTR(sym);
    return ans;
}

static SEXP xxaddformal0(SEXP formlist, SEXP sym, YYLTYPE *lloc)
{
  SEXP ans, annotation, annotations;
    if (GenerateCode) {
	CheckFormalArgs(formlist, sym, lloc);
  PROTECT(annotation = getAttrib(sym, R_AnnotationsSymbol));
  ATTRIB(sym) = R_NilValue;
  PROTECT(annotations = CONS(annotation, getAttrib(formlist, R_AnnotationsSymbol)));
  SET_TAG(annotations, sym);
  PROTECT(ans = NextArg(formlist, R_MissingArg, sym));
  setAttrib(ans, R_AnnotationsSymbol, annotations);
    }
    else
	PROTECT(ans = R_NilValue);
    UNPROTECT_PTR(sym);
    UNPROTECT_PTR(formlist);
    return ans;
}

static SEXP xxaddformal1(SEXP formlist, SEXP sym, SEXP expr, YYLTYPE *lloc)
{
  SEXP ans, annotation, annotations;
    if (GenerateCode) {
      CheckFormalArgs(formlist, sym, lloc);

      PROTECT(annotation = getAttrib(sym, R_AnnotationsSymbol));
      ATTRIB(sym) = R_NilValue;
      PROTECT(annotations = CONS(annotation, getAttrib(formlist, R_AnnotationsSymbol)));
      SET_TAG(annotations, sym);
      PROTECT(ans = NextArg(formlist, expr, sym));
      setAttrib(ans, R_AnnotationsSymbol, annotations);
    }
    else
	PROTECT(ans = R_NilValue);
    UNPROTECT_PTR(expr);
    UNPROTECT_PTR(sym);
    UNPROTECT_PTR(formlist);
    return ans;
}

static SEXP xxexprlist0(void)
{
    SEXP ans;
    if (GenerateCode) {
	PROTECT(ans = NewList());
	if (ParseState.keepSrcRefs) {
	    setAttrib(ans, R_SrcrefSymbol, SrcRefs);
	    REPROTECT(SrcRefs = R_NilValue, srindex);
	}
    }
    else
	PROTECT(ans = R_NilValue);
    return ans;
}

static SEXP xxexprlist1(SEXP expr, YYLTYPE *lloc)
{
    SEXP ans,tmp;
    if (GenerateCode) {
	PROTECT(tmp = NewList());
	if (ParseState.keepSrcRefs) {
	    setAttrib(tmp, R_SrcrefSymbol, SrcRefs);
	    REPROTECT(SrcRefs = list1(makeSrcref(lloc, ParseState.SrcFile)), srindex);
	}
	PROTECT(ans = GrowList(tmp, expr));
	UNPROTECT_PTR(tmp);
    }
    else
	PROTECT(ans = R_NilValue);
    UNPROTECT_PTR(expr);
    return ans;
}

static SEXP xxexprlist2(SEXP exprlist, SEXP expr, YYLTYPE *lloc)
{
    SEXP ans;
    if (GenerateCode) {
	if (ParseState.keepSrcRefs)
	    REPROTECT(SrcRefs = listAppend(SrcRefs, list1(makeSrcref(lloc, ParseState.SrcFile))), srindex);
	PROTECT(ans = GrowList(exprlist, expr));
    }
    else
	PROTECT(ans = R_NilValue);
    UNPROTECT_PTR(expr);
    UNPROTECT_PTR(exprlist);
    return ans;
}

static SEXP xxsub0(void)
{
    SEXP ans;
    if (GenerateCode)
	PROTECT(ans = lang2(R_MissingArg,R_NilValue));
    else
	PROTECT(ans = R_NilValue);
    return ans;
}

static SEXP xxsub1(SEXP expr, YYLTYPE *lloc)
{
    SEXP ans;
    if (GenerateCode)
	PROTECT(ans = TagArg(expr, R_NilValue, lloc));
    else
	PROTECT(ans = R_NilValue);
    UNPROTECT_PTR(expr);
    return ans;
}

static SEXP xxsymsub0(SEXP sym, YYLTYPE *lloc)
{
    SEXP ans;
    if (GenerateCode)
	PROTECT(ans = TagArg(R_MissingArg, sym, lloc));
    else
	PROTECT(ans = R_NilValue);
    UNPROTECT_PTR(sym);
    return ans;
}

static SEXP xxsymsub1(SEXP sym, SEXP expr, YYLTYPE *lloc)
{
    SEXP ans;
    if (GenerateCode)
	PROTECT(ans = TagArg(expr, sym, lloc));
    else
	PROTECT(ans = R_NilValue);
    UNPROTECT_PTR(expr);
    UNPROTECT_PTR(sym);
    return ans;
}

static SEXP xxnullsub0(YYLTYPE *lloc)
{
    SEXP ans;
    UNPROTECT_PTR(R_NilValue);
    if (GenerateCode)
	PROTECT(ans = TagArg(R_MissingArg, install("NULL"), lloc));
    else
	PROTECT(ans = R_NilValue);
    return ans;
}

static SEXP xxnullsub1(SEXP expr, YYLTYPE *lloc)
{
    SEXP ans = install("NULL");
    UNPROTECT_PTR(R_NilValue);
    if (GenerateCode)
	PROTECT(ans = TagArg(expr, ans, lloc));
    else
	PROTECT(ans = R_NilValue);
    UNPROTECT_PTR(expr);
    return ans;
}


static SEXP xxsublist1(SEXP sub)
{
    SEXP ans;
    if (GenerateCode)
	PROTECT(ans = FirstArg(CAR(sub),CADR(sub)));
    else
	PROTECT(ans = R_NilValue);
    UNPROTECT_PTR(sub);
    return ans;
}

static SEXP xxsublist2(SEXP sublist, SEXP sub)
{
    SEXP ans;
    if (GenerateCode)
	PROTECT(ans = NextArg(sublist, CAR(sub), CADR(sub)));
    else
	PROTECT(ans = R_NilValue);
    UNPROTECT_PTR(sub);
    UNPROTECT_PTR(sublist);
    return ans;
}

static SEXP xxcond(SEXP expr)
{
    EatLines = 1;
    return expr;
}

static SEXP xxifcond(SEXP expr)
{
    EatLines = 1;
    return expr;
}

static SEXP xxif(SEXP ifsym, SEXP cond, SEXP expr)
{
    SEXP ans;
    if (GenerateCode)
	PROTECT(ans = lang3(ifsym, cond, expr));
    else
	PROTECT(ans = R_NilValue);
    UNPROTECT_PTR(expr);
    UNPROTECT_PTR(cond);
    return ans;
}

static SEXP xxifelse(SEXP ifsym, SEXP cond, SEXP ifexpr, SEXP elseexpr)
{
    SEXP ans;
    if( GenerateCode)
	PROTECT(ans = lang4(ifsym, cond, ifexpr, elseexpr));
    else
	PROTECT(ans = R_NilValue);
    UNPROTECT_PTR(elseexpr);
    UNPROTECT_PTR(ifexpr);
    UNPROTECT_PTR(cond);
    return ans;
}

static SEXP xxforcond(SEXP sym, SEXP expr)
{
    SEXP ans;
    EatLines = 1;
    if (GenerateCode)
	PROTECT(ans = LCONS(sym, expr));
    else
	PROTECT(ans = R_NilValue);
    UNPROTECT_PTR(expr);
    UNPROTECT_PTR(sym);
    return ans;
}

static SEXP xxfor(SEXP forsym, SEXP forcond, SEXP body)
{
    SEXP ans;
    if (GenerateCode)
	PROTECT(ans = lang4(forsym, CAR(forcond), CDR(forcond), body));
    else
	PROTECT(ans = R_NilValue);
    UNPROTECT_PTR(body);
    UNPROTECT_PTR(forcond);
    return ans;
}

static SEXP xxwhile(SEXP whilesym, SEXP cond, SEXP body)
{
    SEXP ans;
    if (GenerateCode)
	PROTECT(ans = lang3(whilesym, cond, body));
    else
	PROTECT(ans = R_NilValue);
    UNPROTECT_PTR(body);
    UNPROTECT_PTR(cond);
    return ans;
}

static SEXP xxrepeat(SEXP repeatsym, SEXP body)
{
    SEXP ans;
    if (GenerateCode)
	PROTECT(ans = lang2(repeatsym, body));
    else
	PROTECT(ans = R_NilValue);
    UNPROTECT_PTR(body);
    return ans;
}

static SEXP xxnxtbrk(SEXP keyword)
{
    if (GenerateCode)
	PROTECT(keyword = lang1(keyword));
    else
	PROTECT(keyword = R_NilValue);
    return keyword;
}

static SEXP xxfuncall(SEXP expr, SEXP args)
{
    SEXP ans, sav_expr = expr;
    if(GenerateCode) {
	if (isString(expr))
	    expr = installTrChar(STRING_ELT(expr, 0));
	PROTECT(expr);
	if (length(CDR(args)) == 1 && CADR(args) == R_MissingArg && TAG(CDR(args)) == R_NilValue )
	    ans = lang1(expr);
	else
	    ans = LCONS(expr, CDR(args));
	UNPROTECT(1);
	PROTECT(ans);
    }
    else {
	PROTECT(ans = R_NilValue);
    }
    UNPROTECT_PTR(args);
    UNPROTECT_PTR(sav_expr);
    return ans;
}

static SEXP mkString2(const char *s, size_t len, Rboolean escaped)
{
    SEXP t;
    cetype_t enc = CE_NATIVE;

    if(known_to_be_latin1) enc= CE_LATIN1;
    else if(!escaped && known_to_be_utf8) enc = CE_UTF8;

    PROTECT(t = allocVector(STRSXP, 1));
    SET_STRING_ELT(t, 0, mkCharLenCE(s, (int) len, enc));
    UNPROTECT(1);
    return t;
}

static SEXP xxdefun(SEXP fname, SEXP formals, SEXP body, YYLTYPE *lloc)
{
    SEXP ans, srcref;

    if (GenerateCode) {
    	if (ParseState.keepSrcRefs) {
    	    srcref = makeSrcref(lloc, ParseState.SrcFile);
    	    ParseState.didAttach = TRUE;
    	} else
    	    srcref = R_NilValue;
      setAttrib(CDR(formals), R_AnnotationsSymbol, getAttrib(formals, R_AnnotationsSymbol));
	PROTECT(ans = lang4(fname, CDR(formals), body, srcref));
    } else
    PROTECT(ans = R_NilValue);
    UNPROTECT_PTR(body);
    UNPROTECT_PTR(formals);
    return ans;
}

static SEXP xxunary(SEXP op, SEXP arg)
{
    SEXP ans;
    if (GenerateCode)
	PROTECT(ans = lang2(op, arg));
    else
	PROTECT(ans = R_NilValue);
    UNPROTECT_PTR(arg);
    return ans;
}

static SEXP xxbinary(SEXP n1, SEXP n2, SEXP n3)
{
    SEXP ans;
    if (GenerateCode)
	PROTECT(ans = lang3(n1, n2, n3));
    else
	PROTECT(ans = R_NilValue);
    UNPROTECT_PTR(n2);
    UNPROTECT_PTR(n3);
    return ans;
}

static SEXP xxparen(SEXP n1, SEXP n2)
{
    SEXP ans;
    if (GenerateCode)
	PROTECT(ans = lang2(n1, n2));
    else
	PROTECT(ans = R_NilValue);
    UNPROTECT_PTR(n2);
    return ans;
}


/* This should probably use CONS rather than LCONS, but
   it shouldn't matter and we would rather not meddle
   See PR#7055 */

static SEXP xxsubscript(SEXP a1, SEXP a2, SEXP a3)
{
    SEXP ans;
    if (GenerateCode)
	PROTECT(ans = LCONS(a2, CONS(a1, CDR(a3))));
    else
	PROTECT(ans = R_NilValue);
    UNPROTECT_PTR(a3);
    UNPROTECT_PTR(a1);
    return ans;
}

static SEXP xxexprlist(SEXP a1, YYLTYPE *lloc, SEXP a2)
{
    SEXP ans;
    SEXP prevSrcrefs;

    EatLines = 0;
    if (GenerateCode) {
	SET_TYPEOF(a2, LANGSXP);
	SETCAR(a2, a1);
	if (ParseState.keepSrcRefs) {
	    PROTECT(prevSrcrefs = getAttrib(a2, R_SrcrefSymbol));
	    REPROTECT(SrcRefs = CONS(makeSrcref(lloc, ParseState.SrcFile), SrcRefs), srindex);
	    PROTECT(ans = attachSrcrefs(a2));
	    REPROTECT(SrcRefs = prevSrcrefs, srindex);
	    /* SrcRefs got NAMED by being an attribute... */
	    SET_NAMED(SrcRefs, 0);
	    UNPROTECT_PTR(prevSrcrefs);
	}
	else
	    PROTECT(ans = a2);
    }
    else
	PROTECT(ans = R_NilValue);
    UNPROTECT_PTR(a2);
    return ans;
}

static SEXP xxannotate(SEXP expr, SEXP annotation) {
  SEXP ans;
  SEXP annotations;
  SEXP previousAnnotations;
  if (GenerateCode) {
    PROTECT(annotations = list1(annotation));
    PROTECT(previousAnnotations = getAttrib(expr, R_AnnotationsSymbol));
    PROTECT(annotations = CONS(annotation, previousAnnotations));
    setAttrib(expr, R_AnnotationsSymbol, annotations);
    PROTECT(ans = expr);
  }
  else {
    PROTECT(ans = R_NilValue);
  }
  //UNPROTECT_PTR(expr);
  return ans;
}

static SEXP xxannotation(SEXP annotation) {
  SEXP ans;
  if (GenerateCode) {
    PROTECT(ans = lang2(R_QuoteSymbol, annotation));
  }
  else {
    PROTECT(ans = R_NilValue);
  }
  //UNPROTECT_PTR(attributes);
  return ans;
}


/*--------------------------------------------------------------------------*/

static SEXP TagArg(SEXP arg, SEXP tag, YYLTYPE *lloc)
{
    switch (TYPEOF(tag)) {
    case STRSXP:
	tag = installTrChar(STRING_ELT(tag, 0));
    case NILSXP:
    case SYMSXP:
	return lang2(arg, tag);
    default:
	error(_("incorrect tag type at line %d"), lloc->first_line); return R_NilValue/* -Wall */;
    }
}


/* Stretchy List Structures : Lists are created and grown using a special */
/* dotted pair.  The CAR of the list points to the last cons-cell in the */
/* list and the CDR points to the first.  The list can be extracted from */
/* the pair by taking its CDR, while the CAR gives fast access to the end */
/* of the list. */


/* Create a stretchy-list dotted pair */

static SEXP NewList(void)
{
    SEXP s = CONS(R_NilValue, R_NilValue);
    SETCAR(s, s);
    return s;
}

/* Add a new element at the end of a stretchy list */

static SEXP GrowList(SEXP l, SEXP s)
{
    SEXP tmp;
    PROTECT(s);
    tmp = CONS(s, R_NilValue);
    UNPROTECT(1);
    SETCDR(CAR(l), tmp);
    SETCAR(l, tmp);
    return l;
}

static SEXP FirstArg(SEXP s, SEXP tag)
{
    SEXP tmp;
    PROTECT(s);
    PROTECT(tag);
    PROTECT(tmp = NewList());
    tmp = GrowList(tmp, s);
    SET_TAG(CAR(tmp), tag);
    UNPROTECT(3);
    return tmp;
}

static SEXP NextArg(SEXP l, SEXP s, SEXP tag)
{
    PROTECT(tag);
    PROTECT(l);
    l = GrowList(l, s);
    SET_TAG(CAR(l), tag);
    UNPROTECT(2);
    return l;
}



/*--------------------------------------------------------------------------*/

/*
 *  Parsing Entry Points:
 *
 *  The Following entry points provide language parsing facilities.
 *  Note that there are separate entry points for parsing IoBuffers
 *  (i.e. interactve use), files and R character strings.
 *
 *  The entry points provide the same functionality, they just
 *  set things up in slightly different ways.
 *
 *  The following routines parse a single expression:
 *
 *
 *	SEXP R_Parse1File(FILE *fp, int gencode, ParseStatus *status, Rboolean first)
 *   (used for R_ReplFile in main.c)
 *
 *	SEXP R_Parse1Buffer(IoBuffer *buffer, int gencode, ParseStatus *status, Rboolean first)
 *   (used for ReplIteration and R_ReplDLLdo1 in main.c)
 *
 *  The success of the parse is indicated as folllows:
 *
 *
 *	status = PARSE_NULL       - there was no statement to parse
 *		 PARSE_OK	  - complete statement
 *		 PARSE_INCOMPLETE - incomplete statement
 *		 PARSE_ERROR      - syntax error
 *		 PARSE_EOF	  - end of file
 *
 *
 *  The following routines parse several expressions and return
 *  their values in a single expression vector.
 *
 *	SEXP R_ParseFile(FILE *fp, int n, ParseStatus *status, SEXP srcfile)
 *    (used for do_edit in file edit.c)
 *
 *	SEXP R_ParseVector(SEXP *text, int n, ParseStatus *status, SEXP srcfile)
 *    (public, and used by parse(text=) in file source.c)
 *
 *	SEXP R_ParseBuffer(IoBuffer *buffer, int n, ParseStatus *status, SEXP prompt, SEXP srcfile)
 *    (used by parse(file="") in file source.c)
 *
 *      SEXP R_ParseConn(Rconnection con, int n, ParseStatus *status, SEXP srcfile)
 *    (used by parse(file=) in file source.c)
 *
 *  Here, status is 1 for a successful parse and 0 if parsing failed
 *  for some reason.
 */

#define CONTEXTSTACK_SIZE 50
static int	SavedToken;
static SEXP	SavedLval;
static char	contextstack[CONTEXTSTACK_SIZE], *contextp;

static void PutSrcRefState(SrcRefState *state);
static void UseSrcRefState(SrcRefState *state);

/* This is called once when R starts up. */
attribute_hidden
void InitParser(void)
{
    ParseState.data = NULL;
    ParseState.ids = NULL;
}

/* This is called each time a new parse sequence begins */
attribute_hidden
void R_InitSrcRefState(void)
{
    if (busy) {
    	SrcRefState *prev = malloc(sizeof(SrcRefState));
    	PutSrcRefState(prev);
	ParseState.prevState = prev;
	ParseState.data = NULL;
	ParseState.ids = NULL;
    } else
        ParseState.prevState = NULL;
    ParseState.keepSrcRefs = FALSE;
    ParseState.didAttach = FALSE;
    PROTECT_WITH_INDEX(ParseState.SrcFile = R_NilValue, &(ParseState.SrcFileProt));
    PROTECT_WITH_INDEX(ParseState.Original = R_NilValue, &(ParseState.OriginalProt));
    ParseState.data_count = 0;
    ParseState.xxlineno = 1;
    ParseState.xxcolno = 0;
    ParseState.xxbyteno = 0;
    ParseState.xxparseno = 1;
    busy = TRUE;
}

attribute_hidden
void R_FinalizeSrcRefState(void)
{
    UNPROTECT_PTR(ParseState.SrcFile);
    UNPROTECT_PTR(ParseState.Original);
    /* Free the data, text and ids if we are restoring a previous state,
       or if they have grown too large */
    if (ParseState.data) {
    	if (ParseState.prevState || DATA_COUNT > MAX_DATA_COUNT) {
	    R_ReleaseObject(ParseState.data);
	    R_ReleaseObject(ParseState.text);
	    ParseState.data = NULL;
	} else /* Remove all the strings from the text vector so they don't take up memory, and clean up data */
	    for (int i=0; i < ParseState.data_count; i++) {
	    	SET_STRING_ELT(ParseState.text, i, R_BlankString);
		_PARENT(i) = 0;
	    }
    } 
    if (ParseState.ids) {
	if (ParseState.prevState || ID_COUNT > MAX_DATA_COUNT) {
	    R_ReleaseObject(ParseState.ids);
	    ParseState.ids = NULL;
        } else {/* Remove the parent records */
            if (identifier > ID_COUNT) identifier = ID_COUNT;
            for (int i=0; i < identifier; i++)
	        ID_PARENT(i) = 0;
	}
    }
    ParseState.SrcFileProt = NA_INTEGER;
    ParseState.OriginalProt = NA_INTEGER;
    ParseState.data_count = NA_INTEGER;
    if (ParseState.prevState) {
        SrcRefState *prev = ParseState.prevState;
    	UseSrcRefState(prev);
    	free(prev);
    } else
        busy = FALSE;
}

static void UseSrcRefState(SrcRefState *state)
{
    ParseState.keepSrcRefs = state->keepSrcRefs;
    ParseState.SrcFile = state->SrcFile;
    ParseState.Original = state->Original;
    ParseState.SrcFileProt = state->SrcFileProt;
    ParseState.OriginalProt = state->OriginalProt;
    ParseState.data = state->data;
    ParseState.text = state->text;
    ParseState.ids = state->ids;
    ParseState.data_count = state->data_count;
    ParseState.xxlineno = state->xxlineno;
    ParseState.xxcolno = state->xxcolno;
    ParseState.xxbyteno = state->xxbyteno;
    ParseState.xxparseno = state->xxparseno;
    ParseState.prevState = state->prevState;
    busy = TRUE;
}

static void PutSrcRefState(SrcRefState *state)
{
    if (state) {
	state->keepSrcRefs = ParseState.keepSrcRefs;
	state->SrcFile = ParseState.SrcFile;
	state->Original = ParseState.Original;
	state->SrcFileProt = ParseState.SrcFileProt;
	state->OriginalProt = ParseState.OriginalProt;
	state->data = ParseState.data;
	state->text = ParseState.text;
	state->ids = ParseState.ids;
	state->data_count = ParseState.data_count;
	state->xxlineno = ParseState.xxlineno;
	state->xxcolno = ParseState.xxcolno;
	state->xxbyteno = ParseState.xxbyteno;
	state->xxparseno = ParseState.xxparseno;
	state->prevState = ParseState.prevState;
    } else 
    	R_FinalizeSrcRefState();
}

static void ParseInit(void)
{
    contextp = contextstack;
    *contextp = ' ';
    SavedToken = 0;
    SavedLval = R_NilValue;
    EatLines = 0;
    EndOfFile = 0;
    xxcharcount = 0;
    npush = 0;
}

static void initData(void)
{
    ParseState.data_count = 0 ;
    for (int i = 1; i <= ID_COUNT; i++)
	ID_ID( i ) = 0;
}


static void ParseContextInit(void)
{
    R_ParseContextLast = 0;
    R_ParseContext[0] = '\0';
    
    colon = 0 ;

    /* starts the identifier counter*/
    initId();

    initData();
}

static SEXP R_Parse1(ParseStatus *status)
{
    switch(yyparse()) {
    case 0:                     /* End of file */
	*status = PARSE_EOF;
	if (EndOfFile == 2) *status = PARSE_INCOMPLETE;
	break;
    case 1:                     /* Syntax error / incomplete */
	*status = PARSE_ERROR;
	if (EndOfFile) *status = PARSE_INCOMPLETE;
	break;
    case 2:                     /* Empty Line */
	*status = PARSE_NULL;
	break;
    case 3:                     /* Valid expr '\n' terminated */
    case 4:                     /* Valid expr ';' terminated */
	*status = PARSE_OK;
	break;
    }
    return R_CurrentExpr;
}

static FILE *fp_parse;

static int file_getc(void)
{
    return R_fgetc(fp_parse);
}

/* used in main.c */
attribute_hidden
SEXP R_Parse1File(FILE *fp, int gencode, ParseStatus *status)
{
    int savestack;
    savestack = R_PPStackTop;    
    ParseInit();
    ParseContextInit();
    GenerateCode = gencode;
    fp_parse = fp;
    ptr_getc = file_getc;
    R_Parse1(status);
    R_PPStackTop = savestack;
    return R_CurrentExpr;
}

static IoBuffer *iob;

static int buffer_getc(void)
{
    return R_IoBufferGetc(iob);
}

/* Used only in main.c */
attribute_hidden
SEXP R_Parse1Buffer(IoBuffer *buffer, int gencode, ParseStatus *status)
{
    Rboolean keepSource = FALSE; 
    int savestack;    

    R_InitSrcRefState();
    savestack = R_PPStackTop;       
    if (gencode) {
    	keepSource = asLogical(GetOption1(install("keep.source")));
    	if (keepSource) {
    	    ParseState.keepSrcRefs = TRUE;
    	    REPROTECT(ParseState.SrcFile = NewEnvironment(R_NilValue, R_NilValue, R_EmptyEnv), ParseState.SrcFileProt);
	    REPROTECT(ParseState.Original = ParseState.SrcFile, ParseState.OriginalProt);
	    PROTECT_WITH_INDEX(SrcRefs = R_NilValue, &srindex);
	}
    }
    ParseInit();
    ParseContextInit();
    GenerateCode = gencode;
    iob = buffer;
    ptr_getc = buffer_getc;
    R_Parse1(status);
    if (gencode && keepSource) {
    	if (ParseState.didAttach) {
   	    int buflen = R_IoBufferReadOffset(buffer);
   	    char buf[buflen+1];
   	    SEXP class;
   	    R_IoBufferReadReset(buffer);
   	    for (int i=0; i<buflen; i++)
   	    	buf[i] = (char) R_IoBufferGetc(buffer);

   	    buf[buflen] = 0;
	    SEXP s_filename = install("filename");
	    defineVar(s_filename, ScalarString(mkChar("")), ParseState.Original);
	    SEXP s_lines = install("lines");
	    defineVar(s_lines, ScalarString(mkChar(buf)), ParseState.Original);
    	    PROTECT(class = allocVector(STRSXP, 2));
            SET_STRING_ELT(class, 0, mkChar("srcfilecopy"));
            SET_STRING_ELT(class, 1, mkChar("srcfile"));
	    setAttrib(ParseState.Original, R_ClassSymbol, class);
	    UNPROTECT(1);
	}
    }
    R_PPStackTop = savestack;
    R_FinalizeSrcRefState();
    return R_CurrentExpr;
}

static TextBuffer *txtb;

static int text_getc(void)
{
    return R_TextBufferGetc(txtb);
}

static SEXP R_Parse(int n, ParseStatus *status, SEXP srcfile)
{
    int savestack;
    int i;
    SEXP t, rval;

    R_InitSrcRefState();
    savestack = R_PPStackTop;
    
    ParseContextInit();
    PROTECT(t = NewList());

    REPROTECT(ParseState.SrcFile = srcfile, ParseState.SrcFileProt);
    REPROTECT(ParseState.Original = srcfile, ParseState.OriginalProt);
    
    if (isEnvironment(ParseState.SrcFile)) {
    	ParseState.keepSrcRefs = TRUE;
	PROTECT_WITH_INDEX(SrcRefs = R_NilValue, &srindex);
    }
    
    for(i = 0; ; ) {
	if(n >= 0 && i >= n) break;
	ParseInit();
	rval = R_Parse1(status);
	switch(*status) {
	case PARSE_NULL:
	    break;
	case PARSE_OK:
	    t = GrowList(t, rval);
	    i++;
	    break;
	case PARSE_INCOMPLETE:
	case PARSE_ERROR:
	    if (ParseState.keepSrcRefs) 
	        finalizeData();
	    R_PPStackTop = savestack;
	    R_FinalizeSrcRefState();	    
	    return R_NilValue;
	    break;
	case PARSE_EOF:
	    goto finish;
	    break;
	}
    }

finish:

    t = CDR(t);
    PROTECT(rval = allocVector(EXPRSXP, length(t)));
    for (n = 0 ; n < LENGTH(rval) ; n++, t = CDR(t))
	SET_VECTOR_ELT(rval, n, CAR(t));
    if (ParseState.keepSrcRefs) {
	finalizeData();
	rval = attachSrcrefs(rval);
    }
    R_PPStackTop = savestack;    /* UNPROTECT lots! */
    R_FinalizeSrcRefState();
    *status = PARSE_OK;
    return rval;
}

/* used in edit.c */
attribute_hidden
SEXP R_ParseFile(FILE *fp, int n, ParseStatus *status, SEXP srcfile)
{
    GenerateCode = 1;
    fp_parse = fp;
    ptr_getc = file_getc;
    return R_Parse(n, status, srcfile);
}

#include "Rconnections.h"
static Rconnection con_parse;

/* need to handle incomplete last line */
static int con_getc(void)
{
    int c;
    static int last=-1000;

    c = Rconn_fgetc(con_parse);
    if (c == EOF && last != '\n') c = '\n';
    return (last = c);
}

/* used in source.c */
attribute_hidden
SEXP R_ParseConn(Rconnection con, int n, ParseStatus *status, SEXP srcfile)
{
    GenerateCode = 1;
    con_parse = con;
    ptr_getc = con_getc;
    return R_Parse(n, status, srcfile);
}

/* This one is public, and used in source.c */
SEXP R_ParseVector(SEXP text, int n, ParseStatus *status, SEXP srcfile)
{
    SEXP rval;
    TextBuffer textb;
    R_TextBufferInit(&textb, text);
    txtb = &textb;
    GenerateCode = 1;
    ptr_getc = text_getc;
    rval = R_Parse(n, status, srcfile);
    R_TextBufferFree(&textb);
    return rval;
}

static const char *Prompt(SEXP prompt, int type)
{
    if(type == 1) {
	if(length(prompt) <= 0) {
	    return CHAR(STRING_ELT(GetOption1(install("prompt")), 0));
	}
	else
	    return CHAR(STRING_ELT(prompt, 0));
    }
    else {
	return CHAR(STRING_ELT(GetOption1(install("continue")), 0));
    }
}

/* used in source.c */
attribute_hidden
SEXP R_ParseBuffer(IoBuffer *buffer, int n, ParseStatus *status, SEXP prompt, 
		   SEXP srcfile)
{
    SEXP rval, t;
    char *bufp, buf[CONSOLE_BUFFER_SIZE];
    int c, i, prompt_type = 1;
    int savestack;

    R_IoBufferWriteReset(buffer);
    buf[0] = '\0';
    bufp = buf;
    R_InitSrcRefState();    
    savestack = R_PPStackTop;
    PROTECT(t = NewList());
    
    GenerateCode = 1;
    iob = buffer;
    ptr_getc = buffer_getc;

    REPROTECT(ParseState.SrcFile = srcfile, ParseState.SrcFileProt);
    REPROTECT(ParseState.Original = srcfile, ParseState.OriginalProt);
    
    if (isEnvironment(ParseState.SrcFile)) {
    	ParseState.keepSrcRefs = TRUE;
	PROTECT_WITH_INDEX(SrcRefs = R_NilValue, &srindex);
    }
    
    for(i = 0; ; ) {
	if(n >= 0 && i >= n) break;
	if (!*bufp) {
	    if(R_ReadConsole((char *) Prompt(prompt, prompt_type),
			     (unsigned char *)buf, CONSOLE_BUFFER_SIZE, 1) == 0)
		goto finish;
	    bufp = buf;
	}
	while ((c = *bufp++)) {
	    R_IoBufferPutc(c, buffer);
	    if (c == ';' || c == '\n') break;
	}

	/* Was a call to R_Parse1Buffer, but we don't want to reset
	   xxlineno and xxcolno */
	ParseInit();
	ParseContextInit();
	R_Parse1(status);
	rval = R_CurrentExpr;

	switch(*status) {
	case PARSE_NULL:
	    break;
	case PARSE_OK:
	    t = GrowList(t, rval);
	    i++;
	    break;
	case PARSE_INCOMPLETE:
	case PARSE_ERROR:
	    R_IoBufferWriteReset(buffer);
	    R_PPStackTop = savestack;
	    R_FinalizeSrcRefState();
	    return R_NilValue;
	    break;
	case PARSE_EOF:
	    goto finish;
	    break;
	}
    }
finish:
    R_IoBufferWriteReset(buffer);
    t = CDR(t);
    PROTECT(rval = allocVector(EXPRSXP, length(t)));
    for (n = 0 ; n < LENGTH(rval) ; n++, t = CDR(t))
	SET_VECTOR_ELT(rval, n, CAR(t));
    if (ParseState.keepSrcRefs) {
	finalizeData();
	rval = attachSrcrefs(rval);
    }
    R_PPStackTop = savestack; /* UNPROTECT lots! */
    R_FinalizeSrcRefState();    
    *status = PARSE_OK;
    return rval;
}


/*----------------------------------------------------------------------------
 *
 *  The Lexical Analyzer:
 *
 *  Basic lexical analysis is performed by the following
 *  routines.  Input is read a line at a time, and, if the
 *  program is in batch mode, each input line is echoed to
 *  standard output after it is read.
 *
 *  The function yylex() scans the input, breaking it into
 *  tokens which are then passed to the parser.  The lexical
 *  analyser maintains a symbol table (in a very messy fashion).
 *
 *  The fact that if statements need to parse differently
 *  depending on whether the statement is being interpreted or
 *  part of the body of a function causes the need for ifpop
 *  and IfPush.  When an if statement is encountered an 'i' is
 *  pushed on a stack (provided there are parentheses active).
 *  At later points this 'i' needs to be popped off of the if
 *  stack.
 *
 */

static void IfPush(void)
{
    if (*contextp==LBRACE ||
	*contextp=='['    ||
	*contextp=='('    ||
	*contextp == 'i') {
	if(contextp - contextstack >= CONTEXTSTACK_SIZE)
	    error(_("contextstack overflow"));
	*++contextp = 'i';
    }

}

static void ifpop(void)
{
    if (*contextp=='i')
	*contextp-- = 0;
}

/* This is only called following ., so we only care if it is
   an ANSI digit or not */
static int typeofnext(void)
{
    int k, c;

    c = xxgetc();
    if (isdigit(c)) k = 1; else k = 2;
    xxungetc(c);
    return k;
}

static int nextchar(int expect)
{
    int c = xxgetc();
    if (c == expect)
	return 1;
    else
	xxungetc(c);
    return 0;
}

/* Special Symbols */
/* Syntactic Keywords + Symbolic Constants */

struct {
    char *name;
    int token;
}
static keywords[] = {
    { "NULL",	    NULL_CONST },
    { "NA",	    NUM_CONST  },
    { "TRUE",	    NUM_CONST  },
    { "FALSE",	    NUM_CONST  },
    { "Inf",	    NUM_CONST  },
    { "NaN",	    NUM_CONST  },
    { "NA_integer_", NUM_CONST  },
    { "NA_real_",    NUM_CONST  },
    { "NA_character_", NUM_CONST  },
    { "NA_complex_", NUM_CONST  },
    { "function",   FUNCTION   },
    { "while",	    WHILE      },
    { "repeat",	    REPEAT     },
    { "for",	    FOR	       },
    { "if",	    IF	       },
    { "in",	    IN	       },
    { "else",	    ELSE       },
    { "next",	    NEXT       },
    { "break",	    BREAK      },
    { "...",	    SYMBOL     },
    { 0,	    0	       }
};

/* KeywordLookup has side effects, it sets yylval */

static int KeywordLookup(const char *s)
{
    int i;
    for (i = 0; keywords[i].name; i++) {
	if (strcmp(keywords[i].name, s) == 0) {
	    switch (keywords[i].token) {
	    case NULL_CONST:
		PROTECT(yylval = R_NilValue);
		break;
	    case NUM_CONST:
		if(GenerateCode) {
		    switch(i) {
		    case 1:
			PROTECT(yylval = mkNA());
			break;
		    case 2:
			PROTECT(yylval = mkTrue());
			break;
		    case 3:
			PROTECT(yylval = mkFalse());
			break;
		    case 4:
			PROTECT(yylval = allocVector(REALSXP, 1));
			REAL(yylval)[0] = R_PosInf;
			break;
		    case 5:
			PROTECT(yylval = allocVector(REALSXP, 1));
			REAL(yylval)[0] = R_NaN;
			break;
		    case 6:
			PROTECT(yylval = allocVector(INTSXP, 1));
			INTEGER(yylval)[0] = NA_INTEGER;
			break;
		    case 7:
			PROTECT(yylval = allocVector(REALSXP, 1));
			REAL(yylval)[0] = NA_REAL;
			break;
		    case 8:
			PROTECT(yylval = allocVector(STRSXP, 1));
			SET_STRING_ELT(yylval, 0, NA_STRING);
			break;
		    case 9:
			PROTECT(yylval = allocVector(CPLXSXP, 1));
			COMPLEX(yylval)[0].r = COMPLEX(yylval)[0].i = NA_REAL;
			break;
		    }
		} else
		    PROTECT(yylval = R_NilValue);
		break;
	    case FUNCTION:
	    case WHILE:
	    case REPEAT:
	    case FOR:
	    case IF:
	    case NEXT:
	    case BREAK:
		yylval = install(s);
		break;
	    case IN:
	    case ELSE:
		break;
	    case SYMBOL:
		PROTECT(yylval = install(s));
		break;
	    }
	    return keywords[i].token;
	}
    }
    return 0;
}

static SEXP mkFloat(const char *s)
{
    return ScalarReal(R_atof(s));
}

static SEXP mkInt(const char *s)
{
    double f = R_atof(s);  /* or R_strtol? */
    return ScalarInteger((int) f);
}

static SEXP mkComplex(const char *s)
{
    SEXP t = R_NilValue;
    double f;
    f = R_atof(s); /* FIXME: make certain the value is legitimate. */

    if(GenerateCode) {
       t = allocVector(CPLXSXP, 1);
       COMPLEX(t)[0].r = 0;
       COMPLEX(t)[0].i = f;
    }

    return t;
}

static SEXP mkNA(void)
{
    SEXP t = allocVector(LGLSXP, 1);
    LOGICAL(t)[0] = NA_LOGICAL;
    return t;
}

attribute_hidden
SEXP mkTrue(void)
{
    SEXP s = allocVector(LGLSXP, 1);
    LOGICAL(s)[0] = 1;
    return s;
}

SEXP mkFalse(void)
{
    SEXP s = allocVector(LGLSXP, 1);
    LOGICAL(s)[0] = 0;
    return s;
}

static void yyerror(const char *s)
{
    static const char *const yytname_translations[] =
    {
    /* the left column are strings coming from bison, the right
       column are translations for users.
       The first YYENGLISH from the right column are English to be translated,
       the rest are to be copied literally.  The #if 0 block below allows xgettext
       to see these.
    */
#define YYENGLISH 8
	"$undefined",	"input",
	"END_OF_INPUT",	"end of input",
	"ERROR",	"input",
	"STR_CONST",	"string constant",
	"NUM_CONST",	"numeric constant",
	"SYMBOL",	"symbol",
	"LEFT_ASSIGN",	"assignment",
	"'\\n'",	"end of line",
	"NULL_CONST",	"'NULL'",
	"FUNCTION",	"'function'",
	"EQ_ASSIGN",	"'='",
	"RIGHT_ASSIGN",	"'->'",
	"LBB",		"'[['",
	"FOR",		"'for'",
	"IN",		"'in'",
	"IF",		"'if'",
	"ELSE",		"'else'",
	"WHILE",	"'while'",
	"NEXT",		"'next'",
	"BREAK",	"'break'",
	"REPEAT",	"'repeat'",
	"GT",		"'>'",
	"GE",		"'>='",
	"LT",		"'<'",
	"LE",		"'<='",
	"EQ",		"'=='",
	"NE",		"'!='",
	"AND",		"'&'",
	"OR",		"'|'",
	"AND2",		"'&&'",
	"OR2",		"'||'",
	"NS_GET",	"'::'",
	"NS_GET_INT",	"':::'",
  "ANNOTATION_OPERATOR", "'@:'",
  "DELIMITED_ANNOTATION_BEGIN", "'@|'",
  "DELIMITED_ANNOTATION_END", "'|@'",
  "FUNCTION_OPERATOR", "'=>'",
	0
    };
    static char const yyunexpected[] = "syntax error, unexpected ";
    static char const yyexpecting[] = ", expecting ";
    char *expecting;

    R_ParseError     = yylloc.first_line;
    R_ParseErrorCol  = yylloc.first_column;
    R_ParseErrorFile = ParseState.SrcFile;

    if (!strncmp(s, yyunexpected, sizeof yyunexpected -1)) {
	int i;
	/* Edit the error message */
	expecting = strstr(s + sizeof yyunexpected -1, yyexpecting);
	if (expecting) *expecting = '\0';
	for (i = 0; yytname_translations[i]; i += 2) {
	    if (!strcmp(s + sizeof yyunexpected - 1, yytname_translations[i])) {
                switch(i/2)
                {
                case 0:
                        snprintf(R_ParseErrorMsg, PARSE_ERROR_SIZE, _("unexpected input"));
                                break;
                case 1:
                        snprintf(R_ParseErrorMsg, PARSE_ERROR_SIZE, _("unexpected end of input"));
                                break;
                case 2:
                        snprintf(R_ParseErrorMsg, PARSE_ERROR_SIZE, _("unexpected input"));
                                break;
                case 3:
                        snprintf(R_ParseErrorMsg, PARSE_ERROR_SIZE, _("unexpected string constant"));
                                break;
                case 4:
                        snprintf(R_ParseErrorMsg, PARSE_ERROR_SIZE, _("unexpected numeric constant"));
                                break;
                case 5:
                        snprintf(R_ParseErrorMsg, PARSE_ERROR_SIZE, _("unexpected symbol"));
                                break;
                case 6:
                        snprintf(R_ParseErrorMsg, PARSE_ERROR_SIZE, _("unexpected assignment"));
                                break;
                case 7:
                        snprintf(R_ParseErrorMsg, PARSE_ERROR_SIZE, _("unexpected end of line"));
                                break;
                default:
                  snprintf(R_ParseErrorMsg, PARSE_ERROR_SIZE, _("unexpected %s"),
                           yytname_translations[i+1]);
                                break;
                }
                
		return;
	    }
	}
	snprintf(R_ParseErrorMsg, PARSE_ERROR_SIZE - 1, _("unexpected %s"),
                 s + sizeof yyunexpected - 1);
    } else {
	strncpy(R_ParseErrorMsg, s, PARSE_ERROR_SIZE - 1);
        R_ParseErrorMsg[PARSE_ERROR_SIZE - 1] = '\0';
    }
}

static void CheckFormalArgs(SEXP formlist, SEXP _new, YYLTYPE *lloc)
{
    while (formlist != R_NilValue) {
	if (TAG(formlist) == _new) {
	    error(_("repeated formal argument '%s' on line %d"), EncodeChar(PRINTNAME(_new)),
								 lloc->first_line);
	}
	formlist = CDR(formlist);
    }
}

/* This is used as the buffer for NumericValue, SpecialValue and
   SymbolValue.  None of these could conceivably need 8192 bytes.

   It has not been used as the buffer for input character strings
   since Oct 2007 (released as 2.7.0), and for comments since 2.8.0
 */
static char yytext[MAXELTSIZE];

static int SkipSpace(void)
{
    int c;

#ifdef Win32
    if(!mbcslocale) { /* 0xa0 is NBSP in all 8-bit Windows locales */
	while ((c = xxgetc()) == ' ' || c == '\t' || c == '\f' ||
	       (unsigned int) c == 0xa0) ;
	return c;
    } else {
	int i, clen;
	wchar_t wc;
	while (1) {
	    c = xxgetc();
	    if (c == ' ' || c == '\t' || c == '\f') continue;
	    if (c == '\n' || c == R_EOF) break;
	    if ((unsigned int) c < 0x80) break;
	    clen = mbcs_get_next(c, &wc);  /* always 2 */
	    if(! Ri18n_iswctype(wc, Ri18n_wctype("blank")) ) break;
	    for(i = 1; i < clen; i++) c = xxgetc();
	}
	return c;
    }
#endif
#if defined(__STDC_ISO_10646__)
    if(mbcslocale) { /* wctype functions need Unicode wchar_t */
	int i, clen;
	wchar_t wc;
	while (1) {
	    c = xxgetc();
	    if (c == ' ' || c == '\t' || c == '\f') continue;
	    if (c == '\n' || c == R_EOF) break;
	    if ((unsigned int) c < 0x80) break;
	    clen = mbcs_get_next(c, &wc);
	    if(! Ri18n_iswctype(wc, Ri18n_wctype("blank")) ) break;
	    for(i = 1; i < clen; i++) c = xxgetc();
	}
    } else
#endif
	while ((c = xxgetc()) == ' ' || c == '\t' || c == '\f') ;
    return c;
}

/* Note that with interactive use, EOF cannot occur inside */
/* a comment.  However, semicolons inside comments make it */
/* appear that this does happen.  For this reason we use the */
/* special assignment EndOfFile=2 to indicate that this is */
/* going on.  This is detected and dealt with in Parse1Buffer. */

static int SkipComment(void)
{
    int c='#', i;
    
    /* locations before the # character was read */
    int _first_column = ParseState.xxcolno ;
    int _first_parsed = ParseState.xxparseno ;
    int type = COMMENT ;

    Rboolean maybeLine = (ParseState.xxcolno == 1);
    Rboolean doSave;

    DECLARE_YYTEXT_BUFP(yyp);
    
    if (maybeLine) {
    	char lineDirective[] = "#line";
    	YYTEXT_PUSH(c, yyp);
    	for (i=1; i<5; i++) {
    	    c = xxgetc();
  	    if (c != (int)(lineDirective[i])) {
  	    	maybeLine = FALSE;
  	    	break;
  	    }
            YYTEXT_PUSH(c, yyp);
  	}
  	if (maybeLine)     
	    c = processLineDirective(&type);
    }
    // we want to track down the character
    // __before__ the new line character
    int _last_column  = ParseState.xxcolno ;
    int _last_parsed  = ParseState.xxparseno ;
    
    if (c == '\n') {
        _last_column = prevcols[prevpos];
        _last_parsed = prevparse[prevpos];
    }
    
    doSave = !maybeLine;
    
    while (c != '\n' && c != R_EOF) {
        // Comments can be any length; we only record the ones that fit in yytext.
        if (doSave) {
            YYTEXT_PUSH(c, yyp);
            doSave = (yyp - yytext) < sizeof(yytext) - 2;
        }
 	_last_column = ParseState.xxcolno ;
	_last_parsed = ParseState.xxparseno ;
	c = xxgetc();
    }
    if (c == R_EOF) EndOfFile = 2;
    incrementId( ) ;
    YYTEXT_PUSH('\0', yyp);
    record_( _first_parsed, _first_column, _last_parsed, _last_column,
	     type, identifier, doSave ? yytext : 0 ) ;
    return c;
}

static int NumericValue(int c)
{
    int seendot = (c == '.');
    int seenexp = 0;
    int last = c;
    int nd = 0;
    int asNumeric = 0;
    int count = 1; /* The number of characters seen */

    DECLARE_YYTEXT_BUFP(yyp);
    YYTEXT_PUSH(c, yyp);
    /* We don't care about other than ASCII digits */
    while (isdigit(c = xxgetc()) || c == '.' || c == 'e' || c == 'E'
	   || c == 'x' || c == 'X' || c == 'L')
    {
	count++;
	if (c == 'L') /* must be at the end.  Won't allow 1Le3 (at present). */
	{   YYTEXT_PUSH(c, yyp);
	    break;
	}
	
	if (c == 'x' || c == 'X') {
	    if (count > 2 || last != '0') break;  /* 0x must be first */
	    YYTEXT_PUSH(c, yyp);
	    while(isdigit(c = xxgetc()) || ('a' <= c && c <= 'f') ||
		  ('A' <= c && c <= 'F') || c == '.') {
		if (c == '.') {
		    if (seendot) return ERROR;
		    seendot = 1;
		}
		YYTEXT_PUSH(c, yyp);
		nd++;
	    }
	    if (nd == 0) return ERROR;
	    if (c == 'p' || c == 'P') {
	        seenexp = 1;
		YYTEXT_PUSH(c, yyp);
		c = xxgetc();
		if (!isdigit(c) && c != '+' && c != '-') return ERROR;
		if (c == '+' || c == '-') {
		    YYTEXT_PUSH(c, yyp);
		    c = xxgetc();
		}
		for(nd = 0; isdigit(c); c = xxgetc(), nd++)
		    YYTEXT_PUSH(c, yyp);
		if (nd == 0) return ERROR;
	    }
            if (seendot && !seenexp) return ERROR;
	    break;
	}
	if (c == 'E' || c == 'e') {
	    if (seenexp)
		break;
	    seenexp = 1;
	    seendot = seendot == 1 ? seendot : 2;
	    YYTEXT_PUSH(c, yyp);
	    c = xxgetc();
	    if (!isdigit(c) && c != '+' && c != '-') return ERROR;
	    if (c == '+' || c == '-') {
		YYTEXT_PUSH(c, yyp);
		c = xxgetc();
		if (!isdigit(c)) return ERROR;
	    }
	}
	if (c == '.') {
	    if (seendot)
		break;
	    seendot = 1;
	}
	YYTEXT_PUSH(c, yyp);
	last = c;
    }
    
    if(c == 'i')
	YYTEXT_PUSH(c, yyp); /* for getParseData */
	
    YYTEXT_PUSH('\0', yyp);    
    /* Make certain that things are okay. */
    if(c == 'L') {
	double a = R_atof(yytext);
	int b = (int) a;
	/* We are asked to create an integer via the L, so we check that the
	   double and int values are the same. If not, this is a problem and we
	   will not lose information and so use the numeric value.
	*/
	if(a != (double) b) {
	    if(GenerateCode) {
		if(seendot == 1 && seenexp == 0)
		    warning(_("integer literal %s contains decimal; using numeric value"), yytext);
		else {
		    /* hide the L for the warning message */
		    warning(_("non-integer value %s qualified with L; using numeric value"), yytext);
		}
	    }
	    asNumeric = 1;
	    seenexp = 1;
	}
    }

    if(c == 'i') {
	yylval = GenerateCode ? mkComplex(yytext) : R_NilValue;
    } else if(c == 'L' && asNumeric == 0) {
	if(GenerateCode && seendot == 1 && seenexp == 0)
	    warning(_("integer literal %s contains unnecessary decimal point"), yytext);
	yylval = GenerateCode ? mkInt(yytext) : R_NilValue;
#if 0  /* do this to make 123 integer not double */
    } else if(!(seendot || seenexp)) {
	if(c != 'L') xxungetc(c);
	if (GenerateCode) {
	    double a = R_atof(yytext);
	    int b = (int) a;
	    yylval = (a != (double) b) ? mkFloat(yytext) : mkInt(yytext);
	} else yylval = R_NilValue;
#endif
    } else {
	if(c != 'L')
	    xxungetc(c);
	yylval = GenerateCode ? mkFloat(yytext) : R_NilValue;
    }

    PROTECT(yylval);
    return NUM_CONST;
}

/* Strings may contain the standard ANSI escapes and octal */
/* specifications of the form \o, \oo or \ooo, where 'o' */
/* is an octal digit. */


#define STEXT_PUSH(c) do {                  \
	size_t nc = bp - stext;       \
	if (nc >= nstext - 1) {             \
	    char *old = stext;              \
	    nstext *= 2;                    \
	    stext = malloc(nstext);         \
	    if(!stext) error(_("unable to allocate buffer for long string at line %d"), ParseState.xxlineno);\
	    memmove(stext, old, nc);        \
	    if(old != st0) free(old);	    \
	    bp = stext+nc; }		    \
	*bp++ = ((char) c);		    \
} while(0)


/* The idea here is that if a string contains \u escapes that are not
   valid in the current locale, we should switch to UTF-8 for that
   string.  Needs Unicode wide-char support.

   Defining __STDC_ISO_10646__ is done by the OS (nor to) in wchar.t.
   Some (e.g. Solaris, FreeBSD) have Unicode wchar_t but do not define it.
*/

#if defined(Win32) || defined(__STDC_ISO_10646__)
typedef wchar_t ucs_t;
# define mbcs_get_next2 mbcs_get_next
#else
typedef unsigned int ucs_t;
# define WC_NOT_UNICODE 
static int mbcs_get_next2(int c, ucs_t *wc)
{
    int i, res, clen = 1; char s[9];

    s[0] = c;
    /* This assumes (probably OK) that all MBCS embed ASCII as single-byte
       lead bytes, including control chars */
    if((unsigned int) c < 0x80) {
	*wc = (wchar_t) c;
	return 1;
    }
    if(utf8locale) {
	clen = utf8clen(c);
	for(i = 1; i < clen; i++) {
	    s[i] = xxgetc();
	    if(s[i] == R_EOF) error(_("EOF whilst reading MBCS char at line %d"), ParseState.xxlineno);
	}
	s[clen] ='\0'; /* x86 Solaris requires this */
	res = mbtoucs(wc, s, clen);
	if(res == -1) error(_("invalid multibyte character in parser at line %d"), ParseState.xxlineno);
    } else {
	/* This is not necessarily correct for stateful MBCS */
	while(clen <= MB_CUR_MAX) {
	    res = mbtoucs(wc, s, clen);
	    if(res >= 0) break;
	    if(res == -1)
		error(_("invalid multibyte character in parser at line %d"), ParseState.xxlineno);
	    /* so res == -2 */
	    c = xxgetc();
	    if(c == R_EOF) error(_("EOF whilst reading MBCS char at line %d"), ParseState.xxlineno);
	    s[clen++] = c;
	} /* we've tried enough, so must be complete or invalid by now */
    }
    for(i = clen - 1; i > 0; i--) xxungetc(s[i]);
    return clen;
}
#endif

#define WTEXT_PUSH(c) do { if(wcnt < 10000) wcs[wcnt++] = c; } while(0)

static SEXP mkStringUTF8(const ucs_t *wcs, int cnt)
{
    SEXP t;
    int nb;

/* NB: cnt includes the terminator */
#ifdef Win32
    nb = cnt*4; /* UCS-2/UTF-16 so max 4 bytes per wchar_t */
#else
    nb = cnt*6;
#endif
    R_CheckStack2(nb);
    char s[nb];
    memset(s, 0, nb); /* safety */
#ifdef WC_NOT_UNICODE
    for(char *ss = s; *wcs; wcs++) ss += ucstoutf8(ss, *wcs);
#else
    wcstoutf8(s, wcs, nb);
#endif
    PROTECT(t = allocVector(STRSXP, 1));
    SET_STRING_ELT(t, 0, mkCharCE(s, CE_UTF8));
    UNPROTECT(1);
    return t;
}

#define CTEXT_PUSH(c) do { \
	if (ct - currtext >= 1000) { \
	    memmove(currtext, currtext+100, 901); memmove(currtext, "... ", 4); ct -= 100; \
	    currtext_truncated = TRUE; \
	} \
	*ct++ = ((char) c);  \
} while(0)
#define CTEXT_POP() ct--


/* forSymbol is true when parsing backticked symbols */
static int StringValue(int c, Rboolean forSymbol)
{
    int quote = c;
    char currtext[1010], *ct = currtext;
    char st0[MAXELTSIZE];
    unsigned int nstext = MAXELTSIZE;
    char *stext = st0, *bp = st0;
    int wcnt = 0;
    ucs_t wcs[10001];
    Rboolean oct_or_hex = FALSE, use_wcs = FALSE, currtext_truncated = FALSE;

    CTEXT_PUSH(c);
    while ((c = xxgetc()) != R_EOF && c != quote) {
	CTEXT_PUSH(c);
	if (c == '\n') {
	    xxungetc(c); CTEXT_POP();
	    /* Fix suggested by Mark Bravington to allow multiline strings
	     * by pretending we've seen a backslash. Was:
	     * return ERROR;
	     */
	    c = '\\';
	}
	if (c == '\\') {
	    c = xxgetc(); CTEXT_PUSH(c);
	    if ('0' <= c && c <= '7') {
		int octal = c - '0';
		if ('0' <= (c = xxgetc()) && c <= '7') {
		    CTEXT_PUSH(c);
		    octal = 8 * octal + c - '0';
		    if ('0' <= (c = xxgetc()) && c <= '7') {
			CTEXT_PUSH(c);
			octal = 8 * octal + c - '0';
		    } else {
			xxungetc(c);
			CTEXT_POP();
		    }
		} else {
		    xxungetc(c);
		    CTEXT_POP();
		}
		if (!octal)
		    error(_("nul character not allowed (line %d)"), ParseState.xxlineno);
		c = octal;
		oct_or_hex = TRUE;
	    }
	    else if(c == 'x') {
		int val = 0; int i, ext;
		for(i = 0; i < 2; i++) {
		    c = xxgetc(); CTEXT_PUSH(c);
		    if(c >= '0' && c <= '9') ext = c - '0';
		    else if (c >= 'A' && c <= 'F') ext = c - 'A' + 10;
		    else if (c >= 'a' && c <= 'f') ext = c - 'a' + 10;
		    else {
			xxungetc(c);
			CTEXT_POP();
			if (i == 0) { /* was just \x */
			    *ct = '\0';
			    errorcall(R_NilValue, _("'\\x' used without hex digits in character string starting \"%s\""), currtext);
			}
			break;
		    }
		    val = 16*val + ext;
		}
		if (!val)
		    error(_("nul character not allowed (line %d)"), ParseState.xxlineno);
		c = val;
		oct_or_hex = TRUE;
	    }
	    else if(c == 'u') {
		unsigned int val = 0; int i, ext; 
		Rboolean delim = FALSE;

		if(forSymbol) 
		    error(_("\\uxxxx sequences not supported inside backticks (line %d)"), ParseState.xxlineno);
		if((c = xxgetc()) == '{') {
		    delim = TRUE;
		    CTEXT_PUSH(c);
		} else xxungetc(c);
		for(i = 0; i < 4; i++) {
		    c = xxgetc(); CTEXT_PUSH(c);
		    if(c >= '0' && c <= '9') ext = c - '0';
		    else if (c >= 'A' && c <= 'F') ext = c - 'A' + 10;
		    else if (c >= 'a' && c <= 'f') ext = c - 'a' + 10;
		    else {
			xxungetc(c);
			CTEXT_POP();
			if (i == 0) { /* was just \u */
			    *ct = '\0';
			    errorcall(R_NilValue, _("'\\u' used without hex digits in character string starting \"%s\""), currtext);
			}
			break;
		    }
		    val = 16*val + ext;
		}
		if(delim) {
		    if((c = xxgetc()) != '}')
			error(_("invalid \\u{xxxx} sequence (line %d)"),
			      ParseState.xxlineno);
		    else CTEXT_PUSH(c);
		}
		if (!val)
		    error(_("nul character not allowed (line %d)"), ParseState.xxlineno);
		WTEXT_PUSH(val); /* this assumes wchar_t is Unicode */
		use_wcs = TRUE;
		continue;
	    }
	    else if(c == 'U') {
		unsigned int val = 0; int i, ext;
		Rboolean delim = FALSE;
		if(forSymbol) 
		    error(_("\\Uxxxxxxxx sequences not supported inside backticks (line %d)"), ParseState.xxlineno);
		if((c = xxgetc()) == '{') {
		    delim = TRUE;
		    CTEXT_PUSH(c);
		} else xxungetc(c);
		for(i = 0; i < 8; i++) {
		    c = xxgetc(); CTEXT_PUSH(c);
		    if(c >= '0' && c <= '9') ext = c - '0';
		    else if (c >= 'A' && c <= 'F') ext = c - 'A' + 10;
		    else if (c >= 'a' && c <= 'f') ext = c - 'a' + 10;
		    else {
			xxungetc(c);
			CTEXT_POP();
			if (i == 0) { /* was just \U */
			    *ct = '\0';
			    errorcall(R_NilValue, _("'\\U' used without hex digits in character string starting \"%s\""), currtext);
			}
			break;
		    }
		    val = 16*val + ext;
		}
		if(delim) {
		    if((c = xxgetc()) != '}')
			error(_("invalid \\U{xxxxxxxx} sequence (line %d)"), ParseState.xxlineno);
		    else CTEXT_PUSH(c);
		}
		if (!val)
		    error(_("nul character not allowed (line %d)"), ParseState.xxlineno);		
		WTEXT_PUSH(val);
		use_wcs = TRUE;
		continue;
	    }
	    else {
		switch (c) {
		case 'a':
		    c = '\a';
		    break;
		case 'b':
		    c = '\b';
		    break;
		case 'f':
		    c = '\f';
		    break;
		case 'n':
		    c = '\n';
		    break;
		case 'r':
		    c = '\r';
		    break;
		case 't':
		    c = '\t';
		    break;
		case 'v':
		    c = '\v';
		    break;
		case '\\':
		    c = '\\';
		    break;
		case '"':
		case '\'':
		case '`':
		case ' ':
		case '\n':
		    break;
		default:
		    *ct = '\0';
		    errorcall(R_NilValue, _("'\\%c' is an unrecognized escape in character string starting \"%s\""), c, currtext);
		}
	    }
	} else if(mbcslocale) {
	    int i, clen;
	    ucs_t wc;
	    clen = mbcs_get_next2(c, &wc);
	    WTEXT_PUSH(wc);
	    for(i = 0; i < clen - 1; i++){
		STEXT_PUSH(c);
		c = xxgetc();
		if (c == R_EOF) break;
		CTEXT_PUSH(c);
		if (c == '\n') {
		    xxungetc(c); CTEXT_POP();
		    c = '\\';
		}
	    }
	    if (c == R_EOF) break;
	    STEXT_PUSH(c);
	    continue;
	}
	STEXT_PUSH(c);
	if ((unsigned int) c < 0x80) WTEXT_PUSH(c);
	else { /* have an 8-bit char in the current encoding */
#ifdef WC_NOT_UNICODE
	    ucs_t wc;
	    char s[2] = " ";
	    s[0] = (char) c;
	    mbtoucs(&wc, s, 2);
#else
	    wchar_t wc;
	    char s[2] = " ";
	    s[0] = (char) c;
	    mbrtowc(&wc, s, 2, NULL);
#endif
	    WTEXT_PUSH(wc);
	}
    }
    STEXT_PUSH('\0');
    WTEXT_PUSH(0);
    yytext[0] = '\0';
    if (c == R_EOF) {
        if(stext != st0) free(stext);
        PROTECT(yylval = R_NilValue);
    	return INCOMPLETE_STRING;
    } else {
    	CTEXT_PUSH(c);
    	CTEXT_PUSH('\0');
    }
    if (!currtext_truncated)
    	strcpy(yytext, currtext);
    else if (forSymbol || !use_wcs) {
        size_t total = strlen(stext);
        snprintf(yytext, MAXELTSIZE, "[%u chars quoted with '%c']", (unsigned int)total, quote);
    } else 
        snprintf(yytext, MAXELTSIZE, "[%d wide chars quoted with '%c']", wcnt, quote);
    if(forSymbol) {
	PROTECT(yylval = install(stext));
	if(stext != st0) free(stext);
	return SYMBOL;
    } else {
	if(use_wcs) {
	    if(oct_or_hex)
		error(_("mixing Unicode and octal/hex escapes in a string is not allowed"));
	    if(wcnt < 10000)
		PROTECT(yylval = mkStringUTF8(wcs, wcnt)); /* include terminator */
	    else
		error(_("string at line %d containing Unicode escapes not in this locale\nis too long (max 10000 chars)"), ParseState.xxlineno);
	} else
	    PROTECT(yylval = mkString2(stext,  bp - stext - 1, oct_or_hex));
	if(stext != st0) free(stext);
	return STR_CONST;
    }
}

static int SpecialValue(int c)
{
    DECLARE_YYTEXT_BUFP(yyp);
    YYTEXT_PUSH(c, yyp);
    while ((c = xxgetc()) != R_EOF && c != '%') {
	if (c == '\n') {
	    xxungetc(c);
	    return ERROR;
	}
	YYTEXT_PUSH(c, yyp);
    }
    if (c == '%')
	YYTEXT_PUSH(c, yyp);
    YYTEXT_PUSH('\0', yyp);
    yylval = install(yytext);
    return SPECIAL;
}

/* return 1 if name is a valid name 0 otherwise */
attribute_hidden
int isValidName(const char *name)
{
    const char *p = name;
    int i;

    if(mbcslocale) {
	/* the only way to establish which chars are alpha etc is to
	   use the wchar variants */
	size_t n = strlen(name), used;
	wchar_t wc;
	used = Mbrtowc(&wc, p, n, NULL); p += used; n -= used;
	if(used == 0) return 0;
	if (wc != L'.' && !iswalpha(wc) ) return 0;
	if (wc == L'.') {
	    /* We don't care about other than ASCII digits */
	    if(isdigit(0xff & (int)*p)) return 0;
	    /* Mbrtowc(&wc, p, n, NULL); if(iswdigit(wc)) return 0; */
	}
	while((used = Mbrtowc(&wc, p, n, NULL))) {
	    if (!(iswalnum(wc) || wc == L'.' || wc == L'_')) break;
	    p += used; n -= used;
	}
	if (*p != '\0') return 0;
    } else {
	int c = 0xff & *p++;
	if (c != '.' && !isalpha(c) ) return 0;
	if (c == '.' && isdigit(0xff & (int)*p)) return 0;
	while ( c = 0xff & *p++, (isalnum(c) || c == '.' || c == '_') ) ;
	if (c != '\0') return 0;
    }

    if (strcmp(name, "...") == 0) return 1;

    for (i = 0; keywords[i].name != NULL; i++)
	if (strcmp(keywords[i].name, name) == 0) return 0;

    return 1;
}


static int SymbolValue(int c)
{
    int kw;
    DECLARE_YYTEXT_BUFP(yyp);
    if(mbcslocale) {
	wchar_t wc; int i, clen;
	clen = mbcs_get_next(c, &wc);
	while(1) {
	    /* at this point we have seen one char, so push its bytes
	       and get one more */
	    for(i = 0; i < clen; i++) {
		YYTEXT_PUSH(c, yyp);
		c = xxgetc();
	    }
	    if(c == R_EOF) break;
	    if(c == '.' || c == '_') {
		clen = 1;
		continue;
	    }
	    clen = mbcs_get_next(c, &wc);
	    if(!iswalnum(wc)) break;
	}
    } else
	do {
	    YYTEXT_PUSH(c, yyp);
	} while ((c = xxgetc()) != R_EOF &&
		 (isalnum(c) || c == '.' || c == '_'));
    xxungetc(c);
    YYTEXT_PUSH('\0', yyp);
    if ((kw = KeywordLookup(yytext))) 
	return kw;
    
    PROTECT(yylval = install(yytext));
    return SYMBOL;
}

static void setParseFilename(SEXP newname) {
    SEXP class;
    
    if (isEnvironment(ParseState.SrcFile)) {
    	SEXP oldname = findVar(install("filename"), ParseState.SrcFile);
    	if (isString(oldname) && length(oldname) > 0 &&
    	    strcmp(CHAR(STRING_ELT(oldname, 0)),
    	           CHAR(STRING_ELT(newname, 0))) == 0) return;
	REPROTECT(ParseState.SrcFile = NewEnvironment(R_NilValue, R_NilValue, R_EmptyEnv), ParseState.SrcFileProt);
	defineVar(install("filename"), newname, ParseState.SrcFile);
	defineVar(install("original"), ParseState.Original, ParseState.SrcFile);

	PROTECT(class = allocVector(STRSXP, 2));
	SET_STRING_ELT(class, 0, mkChar("srcfilealias"));
	SET_STRING_ELT(class, 1, mkChar("srcfile"));
	setAttrib(ParseState.SrcFile, R_ClassSymbol, class);
	UNPROTECT(1);
    } else {
    	REPROTECT(ParseState.SrcFile = duplicate(newname), ParseState.SrcFileProt);
    }
    UNPROTECT_PTR(newname);
}

static int processLineDirective(int *type)
{
    int c, tok, linenumber;
    c = SkipSpace();
    if (!isdigit(c)) return(c);
    tok = NumericValue(c);
    linenumber = atoi(yytext);
    c = SkipSpace();
    if (c == '"') 
	tok = StringValue(c, FALSE);
    else
    	xxungetc(c);
    if (tok == STR_CONST) 
	setParseFilename(yylval);
    while ((c = xxgetc()) != '\n' && c != R_EOF) /* skip */ ;
    ParseState.xxlineno = linenumber;
    *type = LINE_DIRECTIVE;
    /* we don't change xxparseno here:  it counts parsed lines, not official lines */
    R_ParseContext[R_ParseContextLast] = '\0';  /* Context report shouldn't show the directive */
    return(c);
}

/* Get the R symbol, and set yytext at the same time */
static SEXP install_and_save(char * text)
{
    strcpy(yytext, text);
    return install(text);
}

/* Get an R symbol, and set different yytext.  Used for translation of -> to <-. ->> to <<- */
static SEXP install_and_save2(char * text, char * savetext)
{
    strcpy(yytext, savetext);
    return install(text);
}


/* Split the input stream into tokens. */
/* This is the lowest of the parsing levels. */

static int token(void)
{
    int c;
    wchar_t wc;

    if (SavedToken) {
	c = SavedToken;
	yylval = SavedLval;
	SavedLval = R_NilValue;
	SavedToken = 0;
	yylloc.first_line = xxlinesave;
	yylloc.first_column = xxcolsave;
	yylloc.first_byte = xxbytesave;
	yylloc.first_parsed = xxparsesave;
	return c;
    }
    xxcharsave = xxcharcount; /* want to be able to go back one token */

    c = SkipSpace();
    if (c == '#') c = SkipComment();

    yylloc.first_line = ParseState.xxlineno;
    yylloc.first_column = ParseState.xxcolno;
    yylloc.first_byte = ParseState.xxbyteno;
    yylloc.first_parsed = ParseState.xxparseno;

    if (c == R_EOF) return END_OF_INPUT;

    /* Either digits or symbols can start with a "." */
    /* so we need to decide which it is and jump to  */
    /* the correct spot. */

    if (c == '.' && typeofnext() >= 2) goto symbol;

    /* literal numbers */

    if (c == '.') return NumericValue(c);
    /* We don't care about other than ASCII digits */
    if (isdigit(c)) return NumericValue(c);

    /* literal strings */

    if (c == '\"' || c == '\'')
	return StringValue(c, FALSE);

    /* special functions */

    if (c == '%')
	return SpecialValue(c);

    /* functions, constants and variables */

    if (c == '`')
	return StringValue(c, TRUE);
 symbol:

    if (c == '.') return SymbolValue(c);
    if(mbcslocale) {
	mbcs_get_next(c, &wc);
	if (iswalpha(wc)) return SymbolValue(c);
    } else
	if (isalpha(c)) return SymbolValue(c);

    /* compound tokens */

    switch (c) {
    case '<':
	if (nextchar('=')) {
	    yylval = install_and_save("<=");
	    return LE;
	}
	if (nextchar('-')) {
	    yylval = install_and_save("<-");
	    return LEFT_ASSIGN;
	}
	if (nextchar('<')) {
	    if (nextchar('-')) {
		yylval = install_and_save("<<-");
		return LEFT_ASSIGN;
	    }
	    else
		return ERROR;
	}
	yylval = install_and_save("<");
	return LT;
    case '-':
	if (nextchar('>')) {
	    if (nextchar('>')) {
		yylval = install_and_save2("<<-", "->>");
		return RIGHT_ASSIGN;
	    }
	    else {
		yylval = install_and_save2("<-", "->");
		return RIGHT_ASSIGN;
	    }
	}
	yylval = install_and_save("-");
	return '-';
    case '>':
	if (nextchar('=')) {
	    yylval = install_and_save(">=");
	    return GE;
	}
	yylval = install_and_save(">");
	return GT;
    case '!':
	if (nextchar('=')) {
	    yylval = install_and_save("!=");
	    return NE;
	}
	yylval = install_and_save("!");
	return '!';
    case '=':
      if (nextchar('=')) {
        yylval = install_and_save("==");
        return EQ;
      } else if (nextchar('>')) {
        yylval = install_and_save("=>");
        return FUNCTION_OPERATOR;
      }
      yylval = install_and_save("=");
	return EQ_ASSIGN;
    case ':':
	if (nextchar(':')) {
	    if (nextchar(':')) {
		yylval = install_and_save(":::");
		return NS_GET_INT;
	    }
	    else {
		yylval = install_and_save("::");
		return NS_GET;
	    }
	}
	if (nextchar('=')) {
	    yylval = install_and_save(":=");
	    return LEFT_ASSIGN;
	}
	yylval = install_and_save(":");
	return ':';
    case '&':
	if (nextchar('&')) {
	    yylval = install_and_save("&&");
	    return AND2;
	}
	yylval = install_and_save("&");
	return AND;
    case '|':
	if (nextchar('|')) {
	    yylval = install_and_save("||");
	    return OR2;
	} else if (nextchar('@')) {
    yylval = install_and_save("|@");
    return DELIMITED_ANNOTATION_END;
  }
	yylval = install_and_save("|");
	return OR;
    case LBRACE:
	yylval = install_and_save("{");
	return c;
    case RBRACE:
        strcpy(yytext, "}");
	return c;
    case '(':
	yylval = install_and_save("(");
	return c;
    case ')':
        strcpy(yytext, ")");
	return c;
    case '[':
	if (nextchar('[')) {
	    yylval = install_and_save("[[");
	    return LBB;
	}
	yylval = install_and_save("[");
	return c;
    case ']':
        strcpy(yytext, "]");
	return c;
    case '?':
	yylval = install_and_save("?");
	return c;
    case '*':
	/* Replace ** by ^.  This has been here since 1998, but is
	   undocumented (at least in the obvious places).  It is in
	   the index of the Blue Book with a reference to p. 431, the
	   help for 'Deprecated'.  S-PLUS 6.2 still allowed this, so
	   presumably it was for compatibility with S. */
	if (nextchar('*')) {
	    yylval = install_and_save2("^", "**");
	    return '^';
	} else
	    yylval = install_and_save("*");
	return c;
    case '+':
    case '/':
    case '^':
    case '~':
    case '$':
    case '@':
      if (nextchar(':')) {
        yylval = install_and_save("@:");
        return ANNOTATION_OPERATOR;
      } else if (nextchar('|')) {
        yylval = install_and_save("@|");
        return DELIMITED_ANNOTATION_BEGIN;
      }
	yytext[0] = (char) c;
	yytext[1] = '\0';
	yylval = install(yytext);
	return c;
    default:
        yytext[0] = (char) c;
        yytext[1] = '\0';
	return c;
    }
}

/**
 * Sets the first elements of the yyloc structure with current 
 * information
 */
static void setfirstloc(void)
{
    yylloc.first_line   = ParseState.xxlineno;
    yylloc.first_column = ParseState.xxcolno;
    yylloc.first_byte   = ParseState.xxbyteno;
    yylloc.first_parsed = ParseState.xxparseno;
}

static void setlastloc(void)
{
    yylloc.last_line = ParseState.xxlineno;
    yylloc.last_column = ParseState.xxcolno;
    yylloc.last_byte = ParseState.xxbyteno;
    yylloc.last_parsed = ParseState.xxparseno;
}

/**
 * Wrap around the token function. Returns the same result
 * but increments the identifier, after a call to token_, 
 * the identifier variable contains the id of the token
 * just returned
 *
 * @return the same as token
 */

static int token_(void){
    // capture the position before retrieving the token
    setfirstloc( ) ;

    // get the token
    int res = token( ) ;

    // capture the position after
    int _last_col  = ParseState.xxcolno ;
    int _last_parsed = ParseState.xxparseno ;

    _current_token = res ;
    incrementId( ) ;
    yylloc.id = identifier ;

    // record the position
    if( res != '\n' && res != END_OF_INPUT)
	record_( yylloc.first_parsed, yylloc.first_column, 
	         _last_parsed, _last_col,
		res, identifier, yytext );

    return res; 
}


static int yylex(void)
{
    int tok;

 again:

    tok = token_();

    /* Newlines must be handled in a context */
    /* sensitive way.  The following block of */
    /* deals directly with newlines in the */
    /* body of "if" statements. */

    if (tok == '\n') {

	if (EatLines || *contextp == '[' || *contextp == '(')
	    goto again;

	/* The essence of this is that in the body of */
	/* an "if", any newline must be checked to */
	/* see if it is followed by an "else". */
	/* such newlines are discarded. */

	if (*contextp == 'i') {

	    /* Find the next non-newline token */

	    while(tok == '\n')
		tok = token_();

	    /* If we encounter "}", ")" or "]" then */
	    /* we know that all immediately preceding */
	    /* "if" bodies have been terminated. */
	    /* The corresponding "i" values are */
	    /* popped off the context stack. */

	    if (tok == RBRACE || tok == ')' || tok == ']' ) {
		while (*contextp == 'i')
		    ifpop();
		*contextp-- = 0;
		setlastloc();
		return tok;
	    }

	    /* When a "," is encountered, it terminates */
	    /* just the immediately preceding "if" body */
	    /* so we pop just a single "i" of the */
	    /* context stack. */

	    if (tok == ',') {
		ifpop();
		setlastloc();
		return tok;
	    }

	    /* Tricky! If we find an "else" we must */
	    /* ignore the preceding newline.  Any other */
	    /* token means that we must return the newline */
	    /* to terminate the "if" and "push back" that */
	    /* token so that we will obtain it on the next */
	    /* call to token.  In either case sensitivity */
	    /* is lost, so we pop the "i" from the context */
	    /* stack. */

	    if(tok == ELSE) {
		EatLines = 1;
		ifpop();
		setlastloc();
		return ELSE;
	    }
	    else {
		ifpop();
		SavedToken = tok;
		xxlinesave = yylloc.first_line;
		xxcolsave  = yylloc.first_column;
		xxbytesave = yylloc.first_byte;
		xxparsesave = yylloc.first_parsed;
		SavedLval = yylval;
		setlastloc();
		if (yytext[0]) /* unrecord the pushed back token if not null */
		    ParseState.data_count--;
		return '\n';
	    }
	}
	else {
	    setlastloc();
	    return '\n';
	}
    }

    /* Additional context sensitivities */

    switch(tok) {

	/* Any newlines immediately following the */
	/* the following tokens are discarded. The */
	/* expressions are clearly incomplete. */

    case '+':
    case '-':
    case '*':
    case '/':
    case '^':
    case LT:
    case LE:
    case GE:
    case GT:
    case EQ:
    case NE:
    case OR:
    case AND:
    case OR2:
    case AND2:
    case SPECIAL:
    case FUNCTION:
    case WHILE:
    case REPEAT:
    case FOR:
    case IN:
    case '?':
    case '!':
    case '=':
    case ':':
    case '~':
    case '$':
    case '@':
    case LEFT_ASSIGN:
    case RIGHT_ASSIGN:
    case EQ_ASSIGN:
	EatLines = 1;
	break;

	/* Push any "if" statements found and */
	/* discard any immediately following newlines. */

    case IF:
	IfPush();
	EatLines = 1;
	break;

	/* Terminate any immediately preceding "if" */
	/* statements and discard any immediately */
	/* following newlines. */

    case ELSE:
	ifpop();
	EatLines = 1;
	break;

	/* These tokens terminate any immediately */
	/* preceding "if" statements. */

    case ';':
    case ',':
	ifpop();
	break;

	/* Any newlines following these tokens can */
	/* indicate the end of an expression. */

    case SYMBOL:
    case STR_CONST:
    case NUM_CONST:
    case NULL_CONST:
    case NEXT:
    case BREAK:
	EatLines = 0;
	break;

	/* Handle brackets, braces and parentheses */

    case LBB:
	if(contextp - contextstack >= CONTEXTSTACK_SIZE - 1)
	    error(_("contextstack overflow at line %d"), ParseState.xxlineno);
	*++contextp = '[';
	*++contextp = '[';
	break;

    case '[':
	if(contextp - contextstack >= CONTEXTSTACK_SIZE)
	    error(_("contextstack overflow at line %d"), ParseState.xxlineno);
	*++contextp = (char) tok;
	break;

    case LBRACE:
	if(contextp - contextstack >= CONTEXTSTACK_SIZE)
	    error(_("contextstack overflow at line %d"), ParseState.xxlineno);
	*++contextp = (char) tok;
	EatLines = 1;
	break;

    case '(':
	if(contextp - contextstack >= CONTEXTSTACK_SIZE)
	    error(_("contextstack overflow at line %d"), ParseState.xxlineno);
	*++contextp = (char) tok;
	break;

    case ']':
	while (*contextp == 'i')
	    ifpop();
	*contextp-- = 0;
	EatLines = 0;
	break;

    case RBRACE:
	while (*contextp == 'i')
	    ifpop();
	*contextp-- = 0;
	break;

    case ')':
	while (*contextp == 'i')
	    ifpop();
	*contextp-- = 0;
	EatLines = 0;
	break;

    }
    setlastloc();
    return tok;
}
/**
 * Records location information about a symbol. The information is
 * used to fill the data 
 * 
 */
static void record_( int first_parsed, int first_column, int last_parsed, int last_column,
	int token, int id, char* text_in ){
       
	
	if( token == LEFT_ASSIGN && colon == 1){
		token = COLON_ASSIGN ;
		colon = 0 ;
	}
	
	if (!ParseState.keepSrcRefs || id == NA_INTEGER) return;
	
	// don't care about zero sized things
	if( !yytext[0] ) return ;
	
	if (ParseState.data_count == DATA_COUNT)
	    growData();
	
	_FIRST_COLUMN( ParseState.data_count ) = first_column; 
	_FIRST_PARSED( ParseState.data_count ) = first_parsed;
	_LAST_COLUMN( ParseState.data_count )  = last_column;  
	_LAST_PARSED( ParseState.data_count )  = last_parsed; 
	_TOKEN( ParseState.data_count )        = token;        
	_ID( ParseState.data_count )           = id ;          
	_PARENT(ParseState.data_count)         = 0 ; 
	if ( text_in )
	    SET_STRING_ELT(ParseState.text, ParseState.data_count, mkChar(text_in));
	else
	    SET_STRING_ELT(ParseState.text, ParseState.data_count, mkChar(""));
	
	if( id > ID_COUNT ){
		growID(id) ;
	}
	ID_ID( id ) = ParseState.data_count ; 
	
	ParseState.data_count++ ;
}

/**
 * records parent as the parent of all its childs. This grows the 
 * parents list with a new vector. The first element of the new 
 * vector is the parent id, and other elements are childs id
 *
 * @param parent id of the parent expression
 * @param childs array of location information for all child symbols
 * @param nchilds number of childs
 */
static void recordParents( int parent, yyltype * childs, int nchilds){
	
	if( parent > ID_COUNT ){
		growID(parent) ;
	}
	
	/* some of the childs might be an empty token (like cr)
	   which we do not want to track */
	int ii;    /* loop index */
	yyltype loc ;
	for( ii=0; ii<nchilds; ii++){
		loc = childs[ii] ;
		if( loc.id == NA_INTEGER || (loc.first_line == loc.last_line && loc.first_byte > loc.last_byte) )
			continue ;
		/*  This shouldn't happen... */
		if (loc.id < 0 || loc.id > identifier) {
		    error(_("internal parser error at line %d"),  ParseState.xxlineno);
		}
		ID_PARENT( (childs[ii]).id ) = parent  ;
	}
	
}

/**
 * The token pointed by the location has the wrong token type, 
 * This updates the type
 *
 * @param loc location information for the token to track
 */ 
static void modif_token( yyltype* loc, int tok ){
	
	int id = loc->id ;
	
	if (!ParseState.keepSrcRefs || id < 0 || id > ID_COUNT) 
	    return;
	    
	if( tok == SYMBOL_FUNCTION_CALL ){
		// looking for first child of id
		int j = ID_ID( id ) ;
		int parent = id ;
		
		if (j < 0 || j > ID_COUNT)
	            return;
	            
		while( ID_PARENT( _ID(j) ) != parent ){
		    j-- ; 
		    if (j < 0)
	        	return;
		}
			
		if( _TOKEN(j) == SYMBOL ){
		    _TOKEN(j) = SYMBOL_FUNCTION_CALL ;
		}
		
	} else{
		_TOKEN( ID_ID(id) ) = tok ;
	}
	
}

/* this local version of lengthgets() always copies and doesn't fill with NA */
static SEXP lengthgets2(SEXP x, int len) {
    SEXP result;
    PROTECT(result = allocVector( TYPEOF(x), len ));
    
    len = (len < length(x)) ? len : length(x);
    switch(TYPEOF(x)) {
    	case INTSXP: 
    	    for (int i = 0; i < len; i++)
    	    	INTEGER(result)[i] = INTEGER(x)[i];
	    for (int i = len; i < length(result); i++)
		INTEGER(result)[i] = 0;
    	    break;
    	case STRSXP:
    	    for (int i = 0; i < len; i++)
    	    	SET_STRING_ELT(result, i, STRING_ELT(x, i));
    	    break;
    	default:
	    UNIMPLEMENTED_TYPE("lengthgets2", x);
    }
    UNPROTECT(1);
    return result;
}

static void finalizeData( ){
	
    int nloc = ParseState.data_count ;

    // int maxId = _ID(nloc-1) ;
    int i, j, id ;
    int parent ; 

    /* attach comments to closest enclosing symbol */
    int comment_line, comment_first_col;
    int this_first_parsed, this_last_parsed, this_first_col ;
    int orphan ;

    for( i=0; i<nloc; i++){
	if( _TOKEN(i) == COMMENT ){
	    comment_line = _FIRST_PARSED( i ) ;
	    comment_first_col = _FIRST_COLUMN( i ) ;

	    orphan = 1 ;
	    for( j=i+1; j<nloc; j++){
		this_first_parsed = _FIRST_PARSED( j ) ;
		this_first_col = _FIRST_COLUMN( j ) ;
		this_last_parsed  = _LAST_PARSED( j ) ;

		/* the comment needs to start after the current symbol */
		if( comment_line < this_first_parsed ) continue ;
		if( (comment_line == this_first_parsed) & (comment_first_col < this_first_col) ) continue ;

		/* the current symbol must finish after the comment */
		if( this_last_parsed <= comment_line ) continue ; 

		/* we have a match, record the parent and stop looking */
		ID_PARENT( _ID(i) ) = _ID(j) ;
		orphan = 0;
		break ;
	    }
	    if(orphan){
		ID_PARENT( _ID(i) ) = 0 ;
	    }
	}
    }

    int idp;
    /* store parents in the data */
    for( i=0; i<nloc; i++){
	id = _ID(i);
	parent = ID_PARENT( id ) ;
	if( parent == 0 ){
	    _PARENT(i)=parent;
	    continue;
	}
	while( 1 ){
	    idp = ID_ID( parent ) ;
	    if( idp > 0 ) break ;
	    if( parent == 0 ){
		break ;
	    }
	    parent = ID_PARENT( parent ) ;
	}
	_PARENT(i) = parent ;
    }

    /* now rework the parents of comments, we try to attach 
    comments that are not already attached (parent=0) to the next
    enclosing top-level expression */ 

    for( i=0; i<nloc; i++){
	int token = _TOKEN(i); 
	if( token == COMMENT && _PARENT(i) == 0 ){
	    for( j=i; j<nloc; j++){
		int token_j = _TOKEN(j); 
		if( token_j == COMMENT ) continue ;
		if( _PARENT(j) != 0 ) continue ;
		_PARENT(i) = - _ID(j) ;
		break ;
	    }
	}
    }

    /* attach the token names as an attribute so we don't need to switch to a dataframe, and decide on terminals */
    SEXP tokens;
    PROTECT(tokens = allocVector( STRSXP, nloc ) );
    for (int i=0; i<nloc; i++) {
        int token = _TOKEN(i);
        int xlat = yytranslate[token];
        if (xlat == 2) /* "unknown" */
            xlat = token;
        if (xlat < YYNTOKENS + YYNNTS)
    	    SET_STRING_ELT(tokens, i, mkChar(yytname[xlat]));
    	else { /* we have a token which doesn't have a name, e.g. an illegal character as in PR#15518 */
    	    char name[2];
    	    name[0] = (char) xlat;
    	    name[1] = 0;
    	    SET_STRING_ELT(tokens, i, mkChar(name));
    	}
    	_TERMINAL(i) = xlat < YYNTOKENS;
    }
    SEXP dims, newdata, newtext;
    if (nloc) {
	PROTECT( newdata = lengthgets2(ParseState.data, nloc * DATA_ROWS));
	PROTECT( newtext = lengthgets2(ParseState.text, nloc));
    } else {
	PROTECT( newdata = allocVector( INTSXP, 0));
	PROTECT( newtext = allocVector( STRSXP, 0));
    }
    PROTECT( dims = allocVector( INTSXP, 2 ) ) ;
    INTEGER(dims)[0] = DATA_ROWS ;
    INTEGER(dims)[1] = nloc ;
    setAttrib( newdata, install( "dim" ), dims ) ;
    setAttrib( newdata, install("tokens"), tokens );
    setAttrib( newdata, install("text"), newtext );
    
    setAttrib(newdata, R_ClassSymbol, mkString("parseData"));
    
    /* Put it into the srcfile environment */
    if (isEnvironment(ParseState.SrcFile)) 
    	defineVar(install("parseData"), newdata, ParseState.SrcFile);
    UNPROTECT(4);
}

/**
 * Grows the data
 */
static void growData(){
	
    SEXP bigger, biggertext ; 
    int new_data_count;	
    if (!ParseState.data) {
        new_data_count = INIT_DATA_COUNT;
    	R_PreserveObject(ParseState.data = allocVector(INTSXP, 0));
    	R_PreserveObject(ParseState.text = allocVector(STRSXP, 0));
    } else
        new_data_count = 2*DATA_COUNT;
	
    R_PreserveObject( bigger = lengthgets2(ParseState.data, new_data_count * DATA_ROWS ) ) ; 
    R_PreserveObject( biggertext = lengthgets2(ParseState.text, new_data_count ) );
    R_ReleaseObject( ParseState.data );
    R_ReleaseObject( ParseState.text );
    ParseState.data = bigger;
    ParseState.text = biggertext;
}

/**
 * Grows the ids vector so that ID_ID(target) can be called
 */
static void growID( int target ){
	
    SEXP bigger;
    int new_count;
    if (!ParseState.ids) {
        new_count = INIT_DATA_COUNT/2 - 1;
        R_PreserveObject(ParseState.ids = allocVector(INTSXP, 0));
    } else
    	new_count = ID_COUNT;
    	
    while (target > new_count)
    	new_count = 2*new_count + 1;
    	
    if (new_count <= ID_COUNT)
    	return;
    
    int new_size = (1 + new_count)*2;
    R_PreserveObject( bigger = lengthgets2(ParseState.ids, new_size ) );
    R_ReleaseObject( ParseState.ids );
    ParseState.ids = bigger;
}
