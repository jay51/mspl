const char* DATA_TYPE_VAR = "var";

// Use dinamic list on Compound
typedef struct
{
  Lexer* lexer;
  Token* previous_token;
  Token* current_token;
} Parser;

AST* parser_parse_id(Parser* parser);

Parser* create_parser(Lexer* lexer)
{
  Parser* parser = (Parser*)calloc(1, sizeof(Parser));
  LOG_FAIL(parser);
  parser->lexer = lexer;
  parser->current_token = lexer_get_next_token(parser->lexer);
  parser->previous_token = parser->current_token;
  return parser;
}

void destroy_parser(Parser* parser)
{
  destroy_lexer(parser->lexer);
  free(parser);
}

void parser_eat(Parser* parser, int token_type)
{
  if(parser->current_token->type == token_type)
  {
    parser->previous_token = parser->current_token;
    parser->current_token = lexer_get_next_token(parser->lexer);
  }
  else
  {
    fprintf(stderr, "unexpected token <%s> with type <%d>\n", parser->current_token->value, parser->current_token->type);
    exit(EXIT_FAILURE);
  }
}

AST* parser_parse_string(Parser* parser)
{
  AST* string = create_ast(AST_STRING);
  string->string_value = parser->current_token->value;
  parser_eat(parser, TOKEN_STRING);
  return string;
}

AST* parser_parse_expression(Parser* parser)
{
  switch(parser->current_token->type)
  {
    case TOKEN_STRING : return parser_parse_string(parser);
    case TOKEN_ID : return parser_parse_id(parser);
    default : return create_ast(AST_NOOP);
  }
}

AST* parser_parse_variable_defenition(Parser* parser)
{
  parser_eat(parser, TOKEN_ID);
  char* variable_name = parser->current_token->value;
  parser_eat(parser, TOKEN_ID);
  parser_eat(parser, TOKEN_EQUALS);
  AST* variable_value = parser_parse_expression(parser);
  AST* variable_defenition = create_ast(AST_VARIABLE_DEFINITION);
  variable_defenition->variable_name = variable_name;
  variable_defenition->variable_value = variable_value;
  return variable_defenition;
}

AST* parser_parse_function_call(Parser* parser)
{
  AST* function_call = create_ast(AST_FUNCTION_CALL);
  function_call->function_call_name = parser->previous_token->value;
  function_call->function_call_arguments = (AST**)calloc(1, sizeof(AST*));

  parser_eat(parser, TOKEN_LEFT_PARENTHESIS);

  LOG_FAIL(function_call->function_call_arguments);
  function_call->function_call_arguments[0] = parser_parse_expression(parser);
  function_call->function_call_arguments_count++;

  while(parser->current_token->type == TOKEN_COMMA)
  {
    parser_eat(parser, TOKEN_COMMA);
    function_call->function_call_arguments_count++;
    function_call->function_call_arguments = realloc(function_call->function_call_arguments, function_call->function_call_arguments_count * sizeof(AST*));
    function_call->function_call_arguments[function_call->function_call_arguments_count - 1] = parser_parse_expression(parser);
  }

  parser_eat(parser, TOKEN_RIGHT_PARENTHESIS);

  return function_call;
}

AST* parser_parse_variable(Parser* parser)
{
  char* token_value = parser->current_token->value;

  parser_eat(parser, TOKEN_ID);

  if(parser->current_token->type == TOKEN_LEFT_PARENTHESIS)
  {
    return parser_parse_function_call(parser);
  }

  AST* variable = create_ast(AST_VARIABLE);
  variable->variable_name = token_value;

  return variable;
}

AST* parser_parse_id(Parser* parser)
{
  if(strcmp(parser->current_token->value, DATA_TYPE_VAR) == 0)
  {
    return parser_parse_variable_defenition(parser);
  }
  else
  {
    return parser_parse_variable(parser);
  }
}

AST* parser_parse_statement(Parser* parser)
{
  switch(parser->current_token->type)
  {
    case TOKEN_ID : return parser_parse_id(parser);
    default : return create_ast(AST_NOOP);
  }
}

AST* parser_parse_statements(Parser* parser)
{
  AST* compound = create_ast(AST_COMPOUND);

  compound->compound_value = (AST**)calloc(1, sizeof(AST*));
  LOG_FAIL(compound->compound_value);
  compound->compound_value[0] = parser_parse_statement(parser);
  compound->compound_count++;

  while(parser->current_token->type == TOKEN_SEMICOLON)
  {
    parser_eat(parser, TOKEN_SEMICOLON);
    AST* statement = parser_parse_statement(parser);
    if(statement)
    {
      compound->compound_count++;
      compound->compound_value = realloc(compound->compound_value, compound->compound_count * sizeof(AST*));
      compound->compound_value[compound->compound_count - 1] = statement;
    }
  }

  return compound;
}

AST* parser_parse(Parser* parser)
{
  return parser_parse_statements(parser);
}

AST* parser_parse_factor(Parser* parser)
{

  return NULL;
}

AST* parser_parse_term(Parser* parser)
{

  return NULL;
}
