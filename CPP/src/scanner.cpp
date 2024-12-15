#include "../include/scanner.hpp"
#include <cctype>
#include <optional>
#include <string>
#include <vector>

Scanner::Scanner(const std::string &src) : m_src(src) {}

std::vector<Token> Scanner::scan_tokens() & {
    while (!is_at_end()) {
        m_start = m_current;
        scan_token();
    }
    m_tokens.emplace_back(Token{EndOfFile, std::nullopt, ""});
    return m_tokens;
}

void Scanner::scan_token() & {
    char c = advance();
    switch (c) {
    case '(':
      add_null_token(LEFT_PAREN);
      break;
    case ')':
      add_null_token(RIGHT_PAREN);
      break;
    case '{':
      add_null_token(LEFT_BRACE);
      break;
    case '}':
        add_null_token(RIGHT_BRACE);
        break;
    case ',':
        add_null_token(COMMA);
        break;
    case '.':
        add_null_token(DOT);
        break;
    case '-':
        add_null_token(MINUS);
        break;
    case '+':
        add_null_token(PLUS);
        break;
    case ';': // COMMENT
        while (peek() != '\n' && peek() != '\0') {
            advance();
        }
        add_null_token(SEMICOLON);
        break;
    case '/':
        add_null_token(SLASH);
        break;
    case '*':
        add_null_token(STAR);
        break;

    case ' ':
    case '\t':
    case '\n':
    case '\r':
        break;
      
    case '\"':
        str();
        break;

    default:
      if (isdigit(c)) {
          number();
      } else if (isalpha(c)) {
          identifier();
      } else {
          std::cerr << "Unexpected character: " << c << '\n';
          return;
      }
    }
}

inline bool Scanner::is_at_end() const& noexcept { return m_current >= m_src.size(); }
inline char Scanner::advance() & { return m_src.at(m_current++); }

void Scanner::add_null_token(const Token_Type type) & noexcept {
    add_token(type, std::nullopt);
}

void Scanner::add_token(const Token_Type type, const lit &literal) & {
    const std::string str = m_src.substr(m_start, m_current - m_start);
    m_tokens.push_back({type, literal, str});
}

bool Scanner::match(const char expected) & {
    if (is_at_end()) {
        return false;
    } else if (m_src.at(m_current) != expected) {
        return false;
    } else {
        m_current++;
        return true;
    }
}

char Scanner::peek() const& {
    if (is_at_end()) {
        return '\0';
    } else {
        return m_src.at(m_current);
    }
}

char Scanner::peek_next() const& {
    if (m_current + 1 >= m_src.size()) {
        return '\0';
    } else {
        return m_src.at(m_current + 1);
    }
}

void Scanner::str() & {
    while (peek() != '\"' && !is_at_end()) {
        advance();
    }
    if (is_at_end()) {
        std::cerr << "Error: Unterminated string!\n";
        return;
    } else {
        advance();
        std::string str{m_src.substr(m_start + 1, m_current - m_start - 2)};
        add_token(STRING, str);
    }
}

void Scanner::number() & {
    while (isdigit(peek())) {
        advance();
    }
    if (peek() == '.' && isdigit(peek_next())) {
      advance();
    }
    while (isdigit(peek())) {
        advance();
    }
    std::string str_num{m_src.substr(m_start, m_current - m_start)};
    double number{std::stod(str_num)};
    add_token(NUMBER, number);
}

void Scanner::identifier() & {
    while (isalnum(peek())) {
        advance();
    }
    std::string text{m_src.substr(m_start, m_current - m_start)};
    auto it = m_keywords.find(text);
    if (it != m_keywords.end()) {
        add_null_token(it->second);
    } else {
        add_null_token(IDENTIFIER);
    }
}
