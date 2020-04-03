char* read_file(const char* filePath)
{
  FILE* fp = fopen(filePath, "r");
  LOG_FAIL(fp);
  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);
  char* buffer = calloc(size, sizeof(char));
  LOG_FAIL(buffer);
  rewind(fp);
  fread(buffer, size, sizeof(char), fp);
  fclose(fp);
  return buffer;
}

int main(int argc, char const *argv[])
{
  char* source = read_file(argv[1]);
  Lexer* lexer = create_lexer(source);
  Parser* parser = create_parser(lexer);
  AST* root = parser_parse(parser);
  Visitor* visitor = create_visitor();
  visitor_visit(visitor, root);

  destroy_parser(parser);

  return 0;
}
