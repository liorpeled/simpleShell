%{
# define 	YY_DECL		 char 	**yylex() 
int _numargs = 1000;
char *_args[1000];
int _argcount = 0;
%}

WORD 		[a-zA-Z0-9\/\.-]+ 
SPECIAL 	[()><|&;*]

%%
		_argcount = 0; _args[0] = NULL;

{WORD}|{SPECIAL} {
		if(_argcount < _numargs) {
			_args[_argcount++] = (char *)strdup(yytext);
			_args[_argcount] = NULL; 
		}
      }
\n    return _args;
[ \t]+

.


%%
char **getcommand() { return yylex(); }