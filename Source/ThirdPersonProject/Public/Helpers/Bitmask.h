#pragma  once

#define TEST_BIT(Bitmask, Bit) (((Bitmask) & (1 << static_cast<uint32>(Bit))) > 0)
#define SET_BIT(Bitmask, Bit) (Bitmask |= 1 << static_cast<uint32>(Bit))
#define CLEAR_BIT(Bitmask, Bit) (Bitmask &= ~(1 << static_cast<uint32>(Bit)))