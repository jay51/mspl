#define LOG_FAIL(CONDITION) (void)((CONDITION) || (__log_fail( __FILE__, __LINE__, __func__, #CONDITION), 0))

void __log_fail(const char *file, int line, const char *function, const char *message)
{
  fprintf(stderr, "%s:%d: %s: Assertion [%s] failed\n", file, line, function, message);
  exit(EXIT_FAILURE);
}

#define LOG_ERROR(message) (__log_error(message))

void __log_error(const char *message)
{
  fprintf(stderr, "%s", message);
  exit(EXIT_FAILURE);
}
