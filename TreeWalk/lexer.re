/*!re2c re2c:flags:i = 1; */
/*!max:re2c*/

#include "lexer.h"
/*!re2c
    true = "true";
    false = "false";
    alpha = [a-zA-Z];
    digit = [0-9];
    lineending = [\r\n]|"\r\n";
    verticalline = "|";
    intralinewhitespace=[ \t];
    whitespace = intralinewhitespace | lineending;
    comment = ";"[^\n]* "\n"; atmosphere = whitespace | comment ;
    specialinitial = [_$];
    initial = alpha | specialinitial;
    subsequent = initial | digit;
    hexdigit = digit | [a-f];
    hexscalarvalue = hexdigit+;
    inlinehexescape = "\\x" hexscalarvalue;
    mnemonicescape = "\\a" | "\\b" | "\\t" | "\\n" | "\\r";
    dotsubsequent =   ".";
    stringelement = [^\n\\\x00] | mnemonicescape | "\\\"" |"\\\\" |( "\\" intralinewhitespace* lineending intralinewhitespace*) | inlinehexescape;
    string = "\"" stringelement* "\"";
    number =  digit+ dotsubsequent? digit*;
    character = ("#\\" .) | ("#\\" hexscalarvalue);
    identifier = (initial subsequent*);
*/

#define YYPEEK() YYCURSOR < YYLIMIT ? string[YYCURSOR].unicode() : 0
#define YYSKIP() ++YYCURSOR
#define YYBACKUP() YYMARKER = YYCURSOR
#define YYBACKUPCTX() YYCTXMARKER = YYCURSOR
#define YYRESTORE() YYCURSOR = YYMARKER
#define YYRESTORECTX() YYCURSOR = YYCTXMARKER
#define YYRESTORERAG(t) YYCURSOR = t
#define YYLESSTHAN(n) YYLIMIT - YYCURSOR < n
#define YYSTAGP(t) t = YYCURSOR
#define YYSTAGPD(t) t = YYCURSOR - 1
#define YYSTAGN(t) t = -1

QVector<Token> lex(const QString& string) {
  QVector<Token> tokens;
  /*!stags:re2c format = 'int @@;'; */
  /*!re2c
  re2c:define:YYCTYPE = uint16_t;
  re2c:yyfill:enable  = 0;
  */
  int YYCURSOR = 0;
  int YYMARKER, o1, o2;
  size_t last_line_loc = 0;
  size_t line = 1;
  const int YYLIMIT = string.size();
  do {
    o1 = 0;
    o2 = 0;

    /*!re2c

    * {
        tokens.push_back(Token{TokenType::Unknown,QString(""), YYCURSOR-last_line_loc,line});
        continue;
}

    intralinewhitespace{
        continue;
  }
    
    @o1 lineending @o2{
        line++;
        last_line_loc = o1;
        continue;
    }

    @o1 true @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::True,temp, o1-last_line_loc,line});
        continue;
    }
    
    @o1 false @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::False,temp, o1-last_line_loc,line});
        continue;
    }
    
    @o1 "__FILE__" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::FILE,temp, o1-last_line_loc,line});
        continue;
    }
    @o1 "__LINE__" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::LINE,temp, o1-last_line_loc,line});
        continue;
    }
    @o1 "break" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Break,temp, o1-last_line_loc,line});
        continue;
    }
    
    @o1 "case" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Case,temp, o1-last_line_loc,line});
        continue;
    }
    
    @o1 "class" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Class,temp, o1-last_line_loc,line});
        continue;
    }
    
    @o1 "continue" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Continue,temp, o1-last_line_loc,line});
        continue;
    }
    
    @o1 "do" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Do,temp, o1-last_line_loc,line});
        continue;
    }
    
    @o1 "else" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Else,temp, o1-last_line_loc,line});
        continue;
    }
    
    @o1 "elseif" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Elseif,temp, o1-last_line_loc,line});
        continue;
    }
    
    @o1 "end" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::End,temp, o1-last_line_loc,line});
        continue;
    }
    
    @o1 "for" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::For,temp, o1-last_line_loc,line});
        continue;
    }
    
    @o1 "function" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Function,temp, o1-last_line_loc,line});
        continue;
    }
    
    @o1 "global" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Global,temp, o1-last_line_loc,line});
        continue;
    }
    
    @o1 "if" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::If,temp, o1-last_line_loc,line});
        continue;
    }
    
    @o1 "return" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Return,temp, o1-last_line_loc,line});
        continue;
    }
    
    @o1 "switch" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Switch,temp, o1-last_line_loc,line});
        continue;
    }
    
    @o1 "until" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Until,temp, o1-last_line_loc,line});
        continue;
    }
    
    @o1 "while" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::While,temp, o1-last_line_loc,line});
        continue;
    }
    
    @o1 "super" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Super,temp, o1-last_line_loc,line});
        continue;
    }
    
    @o1 "this" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::This,temp, o1-last_line_loc,line});
        continue;
    }
    
    @o1 "print" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Print,temp, o1-last_line_loc,line});
        continue;
    }
    
    @o1 "nil" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Nil,temp, o1-last_line_loc,line});
        continue;
    }
    

    @o1 identifier @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Identifier,temp,
    o1-last_line_loc,line}); continue;
    }



    @o1 number @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Number,temp, o1-last_line_loc,line});
        continue;
    }

    @o1 character @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Character,temp,
    o1-last_line_loc,line}); continue;
    }

    @o1 string @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::String,temp, o1-last_line_loc,line});
        continue;
    }
    
    @o1 ".=" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::DotAssign,temp,
    o1-last_line_loc,line}); continue;
    }
    
    @o1 "&&" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::And,temp,
    o1-last_line_loc,line}); continue;
    }
    
    @o1 "||" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Or,temp,
    o1-last_line_loc,line}); continue;
    }
    
    
    @o1 "==" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Equal,temp,
    o1-last_line_loc,line}); continue;
    }
    
     @o1 ">=" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::GreaterOrEqual,temp,
    o1-last_line_loc,line}); continue;
    }
     @o1 "<=" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::LesserOrEqual,temp,
    o1-last_line_loc,line}); continue;
    }
     @o1 "!=" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::BangEqual,temp,
    o1-last_line_loc,line}); continue;
    }
    
    @o1 "=" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Assign,temp,
    o1-last_line_loc,line}); continue;
    }
    
    @o1 ">" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Greater,temp,
    o1-last_line_loc,line}); continue;
    }
    
    @o1 "<" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Lesser,temp,
    o1-last_line_loc,line}); continue;
    }
    

    @o1 ";" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Semicolon,temp,
    o1-last_line_loc,line}); continue;
    }
    
    @o1 "++" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Increment,temp,
    o1-last_line_loc,line}); continue;
    }

    @o1 "+" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Plus,temp,
    o1-last_line_loc,line}); continue;
    }
    
    @o1 "--" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Decrement,temp,
    o1-last_line_loc,line}); continue;
    }
    
    @o1 "-" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Minus,temp,
    o1-last_line_loc,line}); continue;
    }
    
    @o1 "*" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Star,temp,
    o1-last_line_loc,line}); continue;
    }
    
    @o1 "{" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::LeftBrace,temp,
    o1-last_line_loc,line}); continue;
    }
    @o1 "/" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Divide,temp,
    o1-last_line_loc,line}); continue;
    }
    
    @o1 "}" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::RightBrace,temp,
    o1-last_line_loc,line}); continue;
    }
    
    
    @o1 "(" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::LeftParen,temp,
    o1-last_line_loc,line}); continue;
    }

    @o1 ")" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::RightParen,temp,
    o1-last_line_loc,line}); continue;
    }

    @o1 "#(" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::HashLeftParen,temp,
    o1-last_line_loc,line}); continue;
    }

    @o1 "#u8(" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::HashU8LeftParen,temp,
    o1-last_line_loc,line}); continue;
    }

    @o1 "'" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::SingleQuote,temp,
    o1-last_line_loc,line}); continue;
    }

    @o1 "`" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::BackTick,temp,
    o1-last_line_loc,line}); continue;
    }

    @o1 "," @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Comma,temp, o1-last_line_loc,line});
        continue;
    }

    @o1 ",@" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::CommaAt,temp, o1-last_line_loc,line});
        continue;
    }


    @o1 "." @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Period,temp, o1-last_line_loc,line});
        continue;
    }
    
    @o1 "!" @o2 {
        auto len = o2-o1;
        auto temp = string.mid(o1,len);
        tokens.push_back(Token{TokenType::Not,temp,
    o1-last_line_loc,line}); continue;
    }

    */
  } while (YYCURSOR < YYLIMIT);
  return tokens;
}
