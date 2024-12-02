#pragma once

#include <optional>
#include <ostream>
#include <variant>
#include <string>
#include <iostream>

enum Token_Type {
  // Single-character tokens
  LEFT_PAREN,      // (
  RIGHT_PAREN,     // )
  LEFT_BRACE,      // {
  RIGHT_BRACE,     // }
  COMMA,           // ,
  DOT,             // .
  MINUS,           // -
  PLUS,            // +
  SEMICOLON,       // ;
  SLASH,           // /
  STAR,            // *

  // One or two character tokens
  BANG,            // !
  BANG_EQUAL,      // !=
  EQUAL,           // =
  EQUAL_EQUAL,     // ==
  GREATER,         // >
  GREATER_EQUAL,   // >=
  LESS,            // <
  LESS_EQUAL,      // <=

  // Literals
  IDENTIFIER,      // Variable names, symbols
  STRING,          // "hello world"
  NUMBER,          // 42, 3.14

  // Keywords
  IF,              // if
  DEF,             // def!
  LET,             // let*
  FN,              // fn*
  TRUE,            // true
  FALSE,           // false
  NIL,             // nil
  DO,              // do
  QUOTE,           // quote
  EVAL,            // eval
  CONS,            // cons
  FIRST,           // first
  REST,            // rest
  LIST,            // list
  PRINTLN,         // println
  NOT,             // not

  EndOfFile        // End of input
};

using lit = std::optional<std::variant<std::string, double>>;

class Token {
private:
    Token_Type m_type {};
    lit m_literal {};
    std::string m_lexeme {};

public:
  Token(const Token_Type type, const lit& literal, const std::string &lexeme)
      : m_type(type), m_literal(literal), m_lexeme(lexeme) {}

    
    Token_Type type() const { return m_type; }
    const lit &literal() const { return m_literal; }
    const std::string& lexeme() const { return m_lexeme; }
    
    friend std::ostream &operator<<(std::ostream &out, const Token &t) {
        out << "Token { type: " << t.m_type << ", lexeme: " << t.m_lexeme
            << ", literal: ";
        if (!t.m_literal.has_value()) {
            std::cout << "null";
        } else if (auto str = std::get_if<std::string>(&t.m_literal.value())) {
            out << *str;
        } else if (auto f = std::get_if<double>(&t.m_literal.value())) {
            out << *f;
        }
        out << "}\n";
        return out;
    }
};

