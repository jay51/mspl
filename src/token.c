typedef struct
{
  enum
  {
    TOKEN_ID,
    TOKEN_EQUALS,
    TOKEN_STRING,
    TOKEN_COMMA,
    TOKEN_SEMICOLON,
    TOKEN_LEFT_PARENTHESIS,
    TOKEN_RIGHT_PARENTHESIS,
    TOKEN_EOF,
  } type;
  char* value;
} Token;

Token* create_token(int type, const char* value)
{
  Token* token = (Token*)calloc(1, sizeof(Token));
  LOG_FAIL(token);
  token->type = type;
  token->value = (char*)calloc(strlen(value) + 1, sizeof(char));
  LOG_FAIL(token->value);
  strcpy(token->value, value);
  return token;
}

void destroy_token(Token* token)
{
  free(token->value);
  free(token);
}

void print_token(const Token* token)
{
  printf("TOKEN:\n\ttype: ");
  switch(token->type)
  {
    case TOKEN_ID : printf("TOKEN_ID"); break;
    case TOKEN_EQUALS : printf("TOKEN_EQUALS"); break;
    case TOKEN_STRING : printf("TOKEN_STRING"); break;
    case TOKEN_SEMICOLON : printf("TOKEN_SEMICOLON"); break;
    case TOKEN_COMMA : printf("TOKEN_COMMA"); break;
    case TOKEN_LEFT_PARENTHESIS : printf("TOKEN_LEFT_PARENTHESIS"); break;
    case TOKEN_RIGHT_PARENTHESIS : printf("TOKEN_RIGHT_PARENTHESIS"); break;
    case TOKEN_EOF : printf("TOKEN_EOF"); break;
  }
  printf("\n\tvalue: %s\n", token->value);
}
