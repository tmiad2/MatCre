#include "Parser.h"
namespace parse {

const Token &Parser::consume(TokenType type, const QString &message) {
  if (check(type))
    return advance();
  emit error(message);
  throw std::runtime_error(message.toStdString());
}
} // namespace parse
