#ifndef EXPRESSION_H
#define EXPRESSION_H
#include "lexer.h"
#include <QString>
#include <QVariant>
#include <memory>

namespace Expr {

static inline QString
toString(const std::vector<double>
             &v) { // Maybe print variable names too. Eventually.
  return QString("Unimplemented\n");
}

struct Object {
  std::variant<QString, double, bool, std::vector<double>, nullptr_t> value;
  auto operator==(const Object &o) const {
    if (this->value.index() != o.value.index()) {
      return false;
    }
    switch (this->value.index()) {
#define ___CMP(TYPE) std::get<TYPE>(this->value) == std::get<TYPE>(o.value)
    case 0:
      return ___CMP(QString);
    case 1:
      return ___CMP(double);
    case 2:
      return ___CMP(bool);
    case 3:
      return ___CMP(std::vector<double>);
    case 4:
      return true;
    default:
      break;
#undef ___CMP
    }
    return false;
  }
  auto operator!=(const Object &o) const { return !(operator==(o)); }
  template <typename T> Object(T arg) : value(arg) {}
  Object(const Object &o) : value(o.value) {}
  Object(Object &&o) : value(o.value) {}
  ~Object() {}
  QString toString() const {
    switch (value.index()) {
    case 0:
      return std::get<QString>(value);
      break;
    case 1:
      return QString::number(std::get<double>(value));
      break;
    case 2:
      return QString(std::get<bool>(value) ? "true" : "false");
      break;
    case 3:
      return Expr::toString(std::get<std::vector<double>>(value));
    case 4:
      return QString("Nil");
      break;
    }
    return QString("   ouch! ");
  }
};

struct Expression {
  virtual QString toString() const = 0;
  virtual Object evaluate() const = 0;
  virtual ~Expression() = 0;
};

struct Binary : virtual Expression {
  const std::unique_ptr<Expression> Left;
  const std::unique_ptr<Expression> Right;
  const Token op;

  Binary(std::unique_ptr<Expression> Left, std::unique_ptr<Expression> Right,
         const Token &op)
      : Left(std::move(Left)), Right(std::move(Right)), op(op) {}

  QString toString() const override {
    QString out;
    out += QString("(%1 %2 %3)")
               .arg(op.representation)
               .arg(Left->toString())
               .arg(Right->toString());
    return out;
  }

  Object evaluate() const override {
    auto left = Left->evaluate();
    auto right = Right->evaluate();
    if (op.type == TokenType::Equal) {
      return Object(left == right);
    }
    if (op.type == TokenType::BangEqual) {
      return Object(left != right);
    }
    auto index = left.value.index();
    if (index != right.value.index()) {
      throw std::runtime_error("Could not evaluate expression.");
      return Object(nullptr);
    }
    switch (index) {
    case 1: {
      auto l = std::get<double>(left.value);
      auto r = std::get<double>(right.value);
      switch (op.type) {
      case TokenType::Plus:
        return l + r;
      case TokenType::Star:
        return l * r;
      case TokenType::Divide:
        return l / r;
      case TokenType::Minus:
        return l - r;
      case TokenType::GreaterOrEqual:
        return l >= r;
      case TokenType::LesserOrEqual:
        return l <= r;
      case TokenType::Greater:
        return l > r;
      case TokenType::Lesser:
        return l < r;
      default:
        break;
      }
      break;
    }
    case 0: {
      auto l = std::get<QString>(left.value);
      auto r = std::get<QString>(right.value);

      switch (op.type) {

      case TokenType::Plus:
        return l + r;
      default:
        break;
      }
      break;
    }
    }
    throw std::runtime_error("Could not evaluate expression.");
    return Object(nullptr);
  }
};

struct Grouping : virtual Expression {
  const std::unique_ptr<Expression> expression;
  explicit Grouping(std::unique_ptr<Expression> expression)
      : expression(std::move(expression)) {}
  QString toString() const override {
    QString out;
    out += QString("(group %1)").arg(expression->toString());
    return out;
  }

  Object evaluate() const override { return expression->evaluate(); }
};

struct Literal : virtual Expression {
  const Object object;
  explicit Literal(const Object &object) : object(object) {}
  QString toString() const override {
    QString out;
    out += object.toString();
    return out;
  }
  Object evaluate() const override { return object; }
};

struct Unary : virtual Expression {
  const Token op;
  const std::unique_ptr<Expression> right;
  Unary(const Token op, std::unique_ptr<Expression> right)
      : op(op), right(std::move(right)) {}
  QString toString() const override {
    QString out;
    out += QString("(%1 %2)").arg(op.representation).arg(right->toString());
    return out;
  }

  Object evaluate() const override {
    auto r = right->evaluate();
    switch (r.value.index()) {
    case 1: // double number.
    {
      auto val = std::get<double>(r.value);
      switch (op.type) {
      case TokenType::Minus:
        return -val;
      case TokenType::Plus:
        return val;
      default:
        return nullptr;
      }

    } break;

    case 2: // boolean.
    {
      auto val = std::get<bool>(r.value);
      switch (op.type) {
      case TokenType::Not:
        return !val;

      default:
        return nullptr;
      }

    } break;

    default:
      return nullptr;
    }

    return nullptr;
  }
};

} // namespace Expr
#endif // EXPRESSION_H
