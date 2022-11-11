#ifndef PICIDAE_PARSE_TOKEN_BASE_HPP__
#define PICIDAE_PARSE_TOKEN_BASE_HPP__

#include <cstdint>
#include <memory>

namespace picidae {

namespace parse {

using tok_type = uint8_t;
using op_id = uint64_t;

enum class TokenType {
  NONE,
  OP,
  UNARY,
  VAR,
  STR,
  FUNC,
  NUM = 0x20,
  REAL = 0x21,
  INT = 0x22,
  BOOL = 0x23,
  IT = 0x40,  // iterator
  LIST = 0x41,
  TUPLE = 0x42,
  STUPLE = 0x43,  // argtuple
  MAP = 0x44,
  REF = 0x80,
  ANY_TYPE = 0xFF
};

#define ANY_OP ""

struct TokenBase {
  TokenType _t;
  TokenBase() {}
  TokenBase(TokenType t) : _t(t) {}
  virtual ~TokenBase() {}
  virtual std::shared_ptr<TokenBase> clone() const = 0;
};

template <class T>
class Token : public TokenBase {
 public:
  Token(T t, TokenType type) : TokenBase(t), _val(t) {}

  virtual std::shared_ptr<TokenBase> clone() const {
    return std::make_shared<Token>(*this);
  }

  T _val;
};

struct TokenNone : public TokenBase {
  TokenNone() : TokenBase(TokenType::NONE) {}
  virtual std::shared_ptr<TokenBase> clone() const {
    return std::make_shared<TokenNone>(*this);
  }
};

struct TokenUnary : public TokenBase {
  TokenUnary() : TokenBase(TokenType::UNARY) {}
  virtual std::shared_ptr<TokenBase> clone() const {
    return std::make_shared<TokenUnary>(*this);
  }
};

}  // namespace parse

}  // namespace picidae

#endif  // PICIDAE_PARSE_TOKEN_BASE_HPP__