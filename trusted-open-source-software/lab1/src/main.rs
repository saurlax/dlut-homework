use bitflags::bitflags;

// The `bitflags!` macro generates `struct`s that manage a set of flags.
bitflags! {
    /// Represents a set of flags.
    #[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
    struct Flags: u32 {
        /// The value `A`, at bit position `0`.
        const A = 0b00000001;
        /// The value `B`, at bit position `1`.
        const B = 0b00000010;
        /// The value `C`, at bit position `2`.
        const C = 0b00000100;

        /// The combination of `A`, `B`, and `C`.
        const ABC = Self::A.bits() | Self::B.bits() | Self::C.bits();
    }
}

fn main() {
    let e1 = Flags::A | Flags::C;
    let e2 = Flags::B | Flags::C;
    assert_eq!((e1 | e2), Flags::ABC); // union
    assert_eq!((e1 & e2), Flags::C); // intersection
    assert_eq!((e1 - e2), Flags::A); // set difference
    assert_eq!(!e2, Flags::A); // set complement
}
