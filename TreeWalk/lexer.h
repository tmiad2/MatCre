#ifndef LEXER_H
#define LEXER_H
#include <QString>
#include <QVector>

enum class TokenType {
  Error,
  Identifier,
  True,
  False,
  Number,
  Character,
  String,
  LeftParen,
  RightParen,
  HashLeftParen,
  HashU8LeftParen,
  SingleQuote,
  BackTick,
  Comma,
  CommaAt,
  Period,
  Increment,
  Decrement,
  Plus,
  Minus,
  Star,
  LeftBrace,
  RightBrace,
  Divide,
  Equal,
  DotAssign,
  Assign,
  Semicolon,
  GreaterOrEqual,
  LesserOrEqual,
  BangEqual,
  Greater,
  Lesser,
  And,
  Or,
  Not,
  // Keywords
  FILE,
  LINE,
  Break,
  Case,
  Class,
  Continue,
  Do,
  Else,
  Elseif,
  End,
  For,
  Function,
  Global,
  If,
  Nil,
  Print,
  Return,
  Switch,
  Super,
  This,
  Until,
  While,

  // End Keywords.

  Unknown
};

class Token {
public:
  Token()
      : type(TokenType::Error), representation(QString()), location(0),
        line(0) {}
  Token(TokenType type, QString representation, size_t location, size_t line)
      : type(type), representation(representation), location(location),
        line(line) {}
  Token(const Token &t) = default;
  Token(Token &&) = default;
  const TokenType type;
  const QString representation;
  const size_t location;
  const size_t line;
};

QVector<Token> lex(const QString &string);

inline QString toString(const Token &t) {
  QString out;
  out += QString("Type: %1\n").arg(static_cast<int>(t.type));
  out += QString("Representation: %1\n").arg(t.representation);
  out += QString("At line: %1, position: %2").arg(t.line).arg(t.location);
  return out;
}

#endif // LEXER_H
