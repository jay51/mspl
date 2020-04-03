// were are using a single compilation unit
//  visit: https://en.wikipedia.org/wiki/Single_Compilation_Unit
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// sources
// make sure you include them by order of 'need'
// example: main uses lexer therefore lexer must be included before main
#include "log.c"
#include "token.c"
#include "lexer.c"
#include "ast.c"
#include "parser.c"
#include "visitor.c"
#include "main.c"
