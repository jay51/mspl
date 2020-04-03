int main(int argc, char const *argv[])
{
  char source[] =
  "var name = \"mspl!\";\
   print(name);";

  Lexer* lexer = create_lexer(source);
  Parser* parser = create_parser(lexer);
  AST* root = parser_parse(parser);

  printf("%d %ld\n", root->type, root->compound_count);

  destroy_lexer(lexer);

  return 0;
}
