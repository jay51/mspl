typedef struct AST_STRUCT AST;
struct AST_STRUCT
{
  enum
  {
    AST_VARIABLE,
    AST_VARIABLE_DEFENITION,
    AST_FUNCTION_CALL,
    AST_STRING,
    AST_COMPOUND,
  } type;

  // AST_VARIABLE
  char* variable_name;

  // AST_VARIABLE_DEFENITION
  AST* variable_value;

  // AST_FUNCTION_CALL
  char* function_call_name;
  AST** function_call_arguments;
  uint64_t function_call_arguments_count;

  // AST_STRING
  char* string_value;

  // AST_COMPOUND
  AST** compound_value;
  uint64_t compound_count;
};

AST* create_ast(int type)
{
  AST* ast = (AST*)calloc(1, sizeof(AST));
  LOG_FAIL(ast);

  ast->type = type;

  ast->variable_name = NULL;

  ast->variable_value = NULL;

  ast->function_call_name = NULL;
  ast->function_call_arguments = NULL;
  ast->function_call_arguments_count = 0;

  ast->string_value = NULL;

  ast->compound_value = NULL;
  ast->compound_count = 0;

  return ast;
}
