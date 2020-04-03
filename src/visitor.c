typedef struct
{
  AST** variable_definitions;
  uint64_t variable_definitions_count;
} Visitor;

AST* visitor_visit(Visitor* visitor, AST* node);

Visitor* create_visitor()
{
  Visitor* visitor = (Visitor*)calloc(1, sizeof(Visitor));
  visitor->variable_definitions = NULL;
  visitor->variable_definitions_count = 0;
  return visitor;
}

AST* visitor_visit_variable(Visitor* visitor, AST* node)
{
  for(uint64_t i = 0; i < visitor->variable_definitions_count; ++i)
  {
    AST* var = visitor->variable_definitions[i];
    if(strcmp(var->variable_name, node->variable_name) == 0)
      return visitor_visit(visitor, var->variable_value);
  }

  fprintf(stderr, "variable does not exist %s\n", node->variable_name);
  return create_ast(AST_NOOP);
}

AST* visitor_visit_variable_defenition(Visitor* visitor, AST* node)
{
  visitor->variable_definitions_count++;

  if(visitor->variable_definitions == NULL)
  {
      visitor->variable_definitions = (AST**)calloc(1, sizeof(AST*));
      LOG_FAIL(visitor->variable_definitions);
      visitor->variable_definitions[0] = node;
  }
  else
  {
    visitor->variable_definitions = realloc(visitor->variable_definitions, visitor->variable_definitions_count * sizeof(AST*));
    visitor->variable_definitions[visitor->variable_definitions_count - 1] = node;
  }

  return node;
}

static AST* builtin_function_write(Visitor* visitor, AST** args, uint64_t args_count)
{
  for(uint64_t i = 0; i < args_count; ++i)
  {
    AST* visited = visitor_visit(visitor, args[i]);
    switch(visited->type)
    {
      case AST_STRING : printf("%s\n", visited->string_value); break;
      default : printf("%p\n", visited);
    }
  }

  return create_ast(AST_NOOP);
}

AST* visitor_visit_function_call(Visitor* visitor, AST* node)
{
  if(strcmp(node->function_call_name, "write") == 0)
  {
    return builtin_function_write(visitor, node->function_call_arguments, node->function_call_arguments_count);
  }

  fprintf(stderr, "undefined method %s\n", node->function_call_name);
  exit(EXIT_FAILURE);
  return create_ast(AST_NOOP);
}

AST* visitor_visit_string(Visitor* visitor, AST* node)
{
  return node;
}

AST* visitor_visit_compound(Visitor* visitor, AST* node)
{
  for(uint64_t i = 0; i < node->compound_count; ++i)
  {
    visitor_visit(visitor, node->compound_value[i]);
  }

  return create_ast(AST_NOOP);
}

AST* visitor_visit(Visitor* visitor, AST* node)
{
  switch(node->type)
  {
    case AST_VARIABLE : return visitor_visit_variable(visitor, node);
    case AST_VARIABLE_DEFINITION : return visitor_visit_variable_defenition(visitor, node);
    case AST_FUNCTION_CALL : return visitor_visit_function_call(visitor, node);
    case AST_STRING : return visitor_visit_string(visitor, node);
    case AST_COMPOUND : return visitor_visit_compound(visitor, node);
    case AST_NOOP : return node;
    default : break;
  }

  fprintf(stderr, "uncaught statement of type %d\n", node->type);
  exit(EXIT_FAILURE);
  return create_ast(AST_NOOP);
}
