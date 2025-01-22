const std = @import("std");

pub const Token_Type = enum {
    LEFT_PAREN,
    RIGHT_PAREN,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SLASH,
    STAR,


    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,

    TRUE,
    FALSE,

    IF,
    DEF,
    LET,
    FN,
    NIL,
    DO,
    QUOTE,
    EVAL,
    CONST,
    FIRST,
    REST,
    LIST,
    PRINTLN,
    NOT,

    IDENTIFIER,
    STRING,
    NUMBER,

    EOF,
};

pub const Literal = union(enum) { Float: f64, String: []const u8, Bool: bool, None: void };

pub const Token = struct {
    const Self = @This();
    type: Token_Type,
    lexeme: []const u8,
    literal: Literal,

    pub fn init(self: *Self, token_type: Token_Type, lexeme: *std.ArrayList(u8), lit: Literal) void {
        self.type = token_type;
        self.lexeme = lexeme;
        self.literal = lit;
    }

    fn append_literal(buffer: *std.ArrayList(u8), lit: Literal) !void {
        switch (lit) {
            Literal.Float => |val| {
                var float_val: [std.fmt.format_float.min_buffer_size]u8 = undefined;
                const float_str = try std.fmt.formatFloat(&float_val, val, .{.mode = .decimal});
                try buffer.appendSlice(float_str);
            },
            Literal.String => |str| {
                try buffer.appendSlice(str);
            },
            Literal.Bool => |b| {
                if(b) {
                    try buffer.appendSlice("true");
                } else {
                    try buffer.appendSlice("false");
                }
            },
            Literal.None => try buffer.appendSlice("null")
        }
    }

    pub fn to_string(self: *const Self, allocator: std.mem.Allocator) ![]const u8 {
        var buffer = std.ArrayList(u8).init(allocator);
        defer buffer.deinit();

        try buffer.appendSlice("Token { type: ");
        try buffer.appendSlice(@tagName(self.type));
        try buffer.appendSlice(", lexeme: ");
        try buffer.appendSlice(self.lexeme);
        try buffer.appendSlice(", Literal: ");
        try append_literal(&buffer, self.literal);
        try buffer.appendSlice(" }\n");
        return try buffer.toOwnedSlice();
    }
};