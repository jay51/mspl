int main(int argc, char const *argv[])
{
  char source[] =
  "var name = \"mspl!\";\
   print(name);";

  Lexer* lexer = create_lexer(source);

  Token* token;
  while((token = lexer_get_next_token(lexer))->type != TOKEN_EOF)
    print_token(token);

  destroy_lexer(lexer);

  return 0;
}
