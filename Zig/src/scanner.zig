const std = @import("std");
const Token = @import("token.zig");

const Scanner_error = error {
    untermed_str,
    unknown_token,
};

pub const Scanner = struct {
    const Self = @This();
    start: usize = 0,
    current: usize = 0,

    source:[] u8,
    tokens: std.ArrayList(Token.Token),

    const keywords = std.StaticStringMap(Token.Token_Type).initComptime(.{
        .{"if", Token.Token_Type.IF},
        .{"def", Token.Token_Type.DEF},
        .{"let", Token.Token_Type.LET},
        .{"fn", Token.Token_Type.FN},
        .{"nil", Token.Token_Type.NIL},
        .{"do", Token.Token_Type.DO},
        .{"quote", Token.Token_Type.QUOTE},
        .{"eval", Token.Token_Type.EVAL},
        .{"const", Token.Token_Type.CONST},
        .{"first", Token.Token_Type.FIRST},
        .{"rest", Token.Token_Type.REST},
        .{"list", Token.Token_Type.LIST},
        .{"println", Token.Token_Type.PRINTLN},
        .{"not", Token.Token_Type.NOT},
    });

    pub fn init(alloc: std.mem.Allocator, src: []u8) Scanner {
        return .{
            .tokens = std.ArrayList(Token.Token).init(alloc),
            .source = src
        };
    }

    pub fn deinit(self: *Self) void { self.tokens.deinit(); }

    fn is_at_end(self: *Self) bool { return self.current >= self.source.len; }

    fn advance(self: *Self) u8 {
        self.current += 1;
        return self.source[self.current - 1];
    }

    fn add_null_token(self: *Self, t: Token.Token_Type) !void { try self.add_token(t, Token.Literal.None);}

    fn add_token(self: *Self, t: Token.Token_Type, lit: Token.Literal) !void {
        const text = self.source[self.start .. self.current];
        const token = Token.Token {
            .type = t,
            .lexeme = text,
            .literal = lit,
        };
        try self.tokens.append(token);
    }

    fn match(self: *Self, expected: u8)  bool {
        if(self.is_at_end()) {
            return false;
        } else if(self.source[self.current] != expected) {
            return false;
        }
        self.current += 1;
        return true;
    }

    fn match_next(self: *Self, expected: u8) bool {
        if(self.is_at_end()) {
            return false;
        } else if(self.peek_next() != expected) {
            return false;
        }
        self.current += 1;
        return true;
    }

    fn peek(self: *Self) ?u8 {
        if(self.is_at_end()) { return null; }
        return self.source[self.current];
    }

    fn peek_next(self: *Self) ?u8 {
        if(self.current + 1 >= self.source.len) { return null; }
        return self.source[self.current + 1];
    }

    fn string(self: *Self) !void {
        var c: u8 = self.peek().?;
        while(c != '\"' and !self.is_at_end()) {
            c = self.advance();
        }
        if(self.is_at_end()) {
            std.debug.print("Error! Unterminated string!\n", .{});
            return Scanner_error.untermed_str;
        }
        const value: []const u8 = self.source[self.start + 1.. self.current - 1];
        const literal = Token.Literal { .String = value };
        try self.add_token(Token.Token_Type.STRING, literal);
    }

    fn number(self: *Self) !void {
        while(std.ascii.isDigit(self.peek().?)) {
            _ = self.advance();
        }
        if(self.peek() == '.' and std.ascii.isDigit(self.peek_next().?)) {
            _ = self.advance();
        }

        const num_string = self.source[self.start .. self.current];
        const flt: f64 = try std.fmt.parseFloat(f64, num_string);
        const literal = Token.Literal{ .Float = flt };
        try self.add_token(Token.Token_Type.NUMBER, literal);
    }

    fn identifier(self: *Self) !void {
        while (self.peek() != null and (std.ascii.isAlphabetic(self.peek().?) or self.peek() == '_')) {
            _ = self.advance();
        }

        const text: []u8 = self.source[self.start .. self.current];
        const ttype_opt = keywords.get(text);
        const ttype = if (ttype_opt == null) Token.Token_Type.IDENTIFIER else ttype_opt.?;
        try self.add_null_token(ttype);
    }

    fn scan_token(self: *Self) !void {
        const c: u8 = self.advance();
        switch (c) {
            '(' => try self.add_null_token(Token.Token_Type.LEFT_PAREN),
            ')' => try self.add_null_token(Token.Token_Type.RIGHT_PAREN),
            ',' => try self.add_null_token(Token.Token_Type.COMMA),
            '.' => try self.add_null_token(Token.Token_Type.DOT),
            '-' => try self.add_null_token(Token.Token_Type.MINUS),
            '+' => try self.add_null_token(Token.Token_Type.PLUS),
            '*' => try self.add_null_token(Token.Token_Type.STAR),
            '/' => try self.add_null_token(Token.Token_Type.SLASH),

            '!' => if (self.match_next('=')) {
                try self.add_null_token(Token.Token_Type.BANG_EQUAL);
            } else {
                try self.add_null_token(Token.Token_Type.BANG);
            },

            '=' => if(self.match_next('=')) {
                try self.add_null_token(Token.Token_Type.EQUAL_EQUAL);
            } else {
                try self.add_null_token(Token.Token_Type.EQUAL);
            },

            '<' => if(self.match_next('=')) {
                try self.add_null_token(Token.Token_Type.LESS_EQUAL);
            } else {
                try self.add_null_token(Token.Token_Type.LESS);
            },

            '>' => if(self.match_next('=')) {
                try self.add_null_token(Token.Token_Type.GREATER_EQUAL);
            } else {
                try self.add_null_token(Token.Token_Type.GREATER);
            },

            '\"' => try self.string(),

            ' ' => {},
            '\r' => {},
            '\t' => {},
            '\n' => {},

            else => {
                if(std.ascii.isDigit(c)) {
                    try self.number();
                } else if(std.ascii.isAlphabetic(c)) {
                    try self.identifier();
                } else {
                    return Scanner_error.unknown_token;
                }
            }
        }
    }

    pub fn scan_tokens(self: *Self) ![]const Token.Token {
        while(!self.is_at_end()) {
            self.start = self.current;
            try self.scan_token();
        }
        const eof = Token.Token {
            .type = Token.Token_Type.EOF,
            .lexeme = "EOF",
            .literal = Token.Literal.None
        };

        try self.tokens.append(eof);
        return self.tokens.items;
    }
};