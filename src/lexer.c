typedef struct
{
  char* source_code;
  char current_char;
  uint64_t index;
  uint64_t source_code_length;
} Lexer;

Lexer* create_lexer(char* source_code)
{
  Lexer* lexer = (Lexer*)calloc(1, sizeof(Lexer));
  LOG_FAIL(lexer);
  lexer->source_code = source_code;
  lexer->index = 0;
  lexer->current_char = lexer->source_code[lexer->index];
  lexer->source_code_length = strlen(lexer->source_code);
  return lexer;
}

void destroy_lexer(Lexer* lexer)
{
  // free(lexer->source_code);
  free(lexer);
}

void lexer_advance(Lexer* lexer)
{
  if(lexer->current_char != '\0' && lexer->index < lexer->source_code_length)
  {
    lexer->index++;
    lexer->current_char = lexer->source_code[lexer->index];
  }
}

void lexer_skip_whitespace(Lexer* lexer)
{
  while(isspace(lexer->current_char))
  {
    lexer_advance(lexer);
  }
}

static inline Token* lexer_advance_with_token(Lexer* lexer, Token* token)
{
  lexer_advance(lexer);
  return token;
}

char* lexer_get_current_char_as_string(Lexer* lexer)
{
  char* str = calloc(2, sizeof(char));
  str[0] = lexer->current_char;
  str[1] = '\0';
  return str;
}

Token* lexer_collect_string(Lexer* lexer)
{
  lexer_advance(lexer);

  uint64_t initial_index = lexer->index;

  while(lexer->current_char != '"')
  {
    if(lexer->current_char == '\0')
    {
      LOG_ERROR("missing closing quotation mark\n");
    }
    lexer_advance(lexer);
  }

  uint64_t len = lexer->index - initial_index;
  char str[len];
  memcpy(str, &lexer->source_code[initial_index], len);
  str[len] = '\0';

  lexer_advance(lexer);

  return create_token(TOKEN_STRING, str);
}

Token* lexer_collect_id(Lexer* lexer)
{
  uint64_t initial_index = lexer->index;

  while(isalnum(lexer->current_char))
  {
    lexer_advance(lexer);
  }

  uint64_t len = lexer->index - initial_index;
  char str[len];
  memcpy(str, &lexer->source_code[initial_index], len);
  str[len] = '\0';

  return create_token(TOKEN_ID, str);
}

Token* lexer_get_next_token(Lexer* lexer)
{
  while(lexer->current_char != '\0' && lexer->index < lexer->source_code_length)
  {
    if(isspace(lexer->current_char))
    {
      lexer_skip_whitespace(lexer);
    }

    if(isalnum(lexer->current_char))
    {
      return lexer_collect_id(lexer);
    }

    switch(lexer->current_char)
    {
      case '"' : return lexer_collect_string(lexer);
      case '=' : return lexer_advance_with_token(lexer, create_token(TOKEN_EQUALS, lexer_get_current_char_as_string(lexer)));
      case ';' : return lexer_advance_with_token(lexer, create_token(TOKEN_SEMICOLON, lexer_get_current_char_as_string(lexer)));
      case '(' : return lexer_advance_with_token(lexer, create_token(TOKEN_LEFT_PARENTHESIS, lexer_get_current_char_as_string(lexer)));
      case ')' : return lexer_advance_with_token(lexer, create_token(TOKEN_RIGHT_PARENTHESIS, lexer_get_current_char_as_string(lexer)));
    }
  }

  return create_token(TOKEN_EOF, "\0");
}
