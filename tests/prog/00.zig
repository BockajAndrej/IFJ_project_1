const ifj = @import("ifj24.zig");

var a : i32 = 12;
var b : i32 = 20;

pub fn main() void {
    const c : i32 = a + b;
    if(c > 0)
    {
        ifj.write("Vysledek: ");
        ifj.write(c);
    }
}