#ifndef PARSER_H
#define PARSER_H

#include "Expression.h"
#include "lexer.h"
namespace parse {
using namespace Expr;
const QString Unknown = "Unknown";
class Parser : public QObject {
  Q_OBJECT
  inline const static Token EOF_Token =
      Token(TokenType::Unknown, Unknown, 0, 0);
  const QVector<Token> &tokens;
  int index = 0;

  std::unique_ptr<Expression> expression() { return equality(); }

  std::unique_ptr<Expression> equality() {
    auto expr = comparison();
    while (match(TokenType::BangEqual, TokenType::Equal)) {
      auto op = previous();
      auto right = comparison();
      expr = std::make_unique<Binary>(std::move(expr), std::move(right), op);
    }
    return expr;
  }

  const Token &advance() {
    if (index < tokens.length()) {
      index++;
    }
    return previous();
  }
  const Token &previous() const { return tokens.at(index - 1); }
  const Token &peek() const {
    if (index >= tokens.length()) {
      return EOF_Token;
    }
    return tokens.at(index);
  }

  bool isAtEnd() const { return index >= tokens.length(); }

  bool check(TokenType type) const {
    if (isAtEnd())
      return false;
    return peek().type == type;
  }

  template <typename Arg, typename... Args>
  bool match(Arg const &first, Args const &... types) {
    if (check(first)) {
      advance();
      return true;
    }

    return match(types...);
  }

  bool match(TokenType const &type) {
    if (check(type)) {
      advance();
      return true;
    }
    return false;
  }

  bool match() { return false; }

  std::unique_ptr<Expression> comparison() {
    auto expr = addition();
    while (match(TokenType::Greater, TokenType::GreaterOrEqual,
                 TokenType::Lesser, TokenType::LesserOrEqual)) {
      auto op = previous();
      auto right = addition();
      expr = std::make_unique<Binary>(std::move(expr), std::move(right), op);
    }
    return expr;
  }

  std::unique_ptr<Expression> addition() {
    auto expr = multiplication();
    while (match(TokenType::Minus, TokenType::Plus)) {
      auto op = previous();
      auto right = multiplication();
      expr = std::make_unique<Binary>(std::move(expr), std::move(right), op);
    }
    return expr;
  }

  std::unique_ptr<Expression> multiplication() {
    auto expr = unary();
    while (match(TokenType::Star, TokenType::Divide)) {
      auto op = previous();
      auto right = unary();
      expr = std::make_unique<Binary>(std::move(expr), std::move(right), op);
    }
    return expr;
  }

  std::unique_ptr<Expression> unary() {
    if (match(TokenType::Not, TokenType::Minus)) {
      auto op = previous();
      auto right = unary();
      return std::make_unique<Unary>(op, std::move(right));
    }
    return primary();
  }

  std::unique_ptr<Expression> primary() {
    if (match(TokenType::True)) {
      return std::make_unique<Literal>(true);
    }
    if (match(TokenType::False)) {
      return std::make_unique<Literal>(false);
    }
    if (match(TokenType::Nil)) {
      return std::make_unique<Literal>(nullptr);
    }
    if (match(TokenType::Number)) {
      return std::make_unique<Literal>(previous().representation.toDouble());
    }
    if (match(TokenType::String)) {
      return std::make_unique<Literal>(previous().representation);
    }

    if (match(TokenType::LeftParen)) {
      auto expr = expression();
      consume(TokenType::RightParen, QString("Expected  closing ')' at %1:%2.")
                                         .arg(peek().line)
                                         .arg(peek().location));
      return std::make_unique<Grouping>(std::move(expr));
    }
    auto s = QString("Expected Literal at %1:%2, instead found : %3")
                 .arg(peek().line)
                 .arg(peek().location)
                 .arg(peek().representation);
    emit error(s);
    throw std::runtime_error(s.toStdString());
  }

  const Token &consume(TokenType type, const QString &message);

  void synchronize() {
    advance();

    while (!isAtEnd()) {
      if (previous().type == TokenType::Semicolon)
        return;

      switch (peek().type) {
      case TokenType::Class:
      case TokenType::Function:
      case TokenType::For:
      case TokenType::If:
      case TokenType::While:
      case TokenType::Print:
      case TokenType::Return:
        return;
      default:
        break;
      }

      advance();
    }
  }

public:
  explicit Parser(const QVector<Token> &tokens) : tokens(tokens) {}

  std::unique_ptr<Expression> parse() {
    try {
      return expression();
    } catch (const std::exception &e) {
      return nullptr;
    }
  }

signals:
  void error(QString error_string);
};

} // namespace parse

#endif // PARSER_H
