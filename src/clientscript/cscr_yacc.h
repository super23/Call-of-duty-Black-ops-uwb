#pragma once

int __cdecl yyparse();
unsigned int __cdecl LowerCase(unsigned int stringValue);
void __cdecl _yy_memcpy(char *to, char *from, unsigned int count);
int __cdecl yylex();
void __cdecl TextValue(char *str, int len);
sval_u __cdecl HashValue(char *str, int len);
int __cdecl StringValue(char *str, int len);
void __cdecl IntegerValue(char *str);
void __cdecl FloatValue(char *str);
int __cdecl yy_get_next_buffer();
int __cdecl yy_get_previous_state();
int __cdecl yy_try_NUL_trans(int yy_current_state);
void __cdecl yyrestart(_iobuf *input_file);
void __cdecl yy_load_buffer_state();
yy_buffer_state *__cdecl yy_create_buffer(_iobuf *file, unsigned int size);
void __cdecl yy_init_buffer(yy_buffer_state *b, _iobuf *file);
void __cdecl yy_flush_buffer(yy_buffer_state *b);
void __cdecl  yy_fatal_error(const char *msg);
void *__cdecl yy_flex_alloc(unsigned int size);
void *__cdecl yy_flex_realloc(void *ptr, unsigned int size);
int __cdecl yyerror();
void __cdecl ScriptParse(scriptInstance_t inst, sval_u *parseData, unsigned __int8 user);
