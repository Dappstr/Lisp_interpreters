const std = @import("std");
const Token = @import("token.zig").Token;
const Token_Type = @import("token.zig").Token_Type;
const Literal = @import("token.zig").Literal;

var gpa = std.heap.GeneralPurposeAllocator(.{}){};
var allocator = gpa.allocator();

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();
    const stdin = std.io.getStdIn().reader();

    while(true) {
        try stdout.print("zisp> ", .{});
        var buff = std.ArrayList(u8).init(allocator);
        defer buff.deinit();

        _ = stdin.streamUntilDelimiter(buff.writer(), '\n', null) catch |err| switch (err) {
            error.StreamTooLong => return std.debug.print("Stream too long.\n", .{}),
            error.EndOfStream => return,
            else => return err,
        };
        //try stdout.print("{s}\n", .{buff.items});
    }
}
