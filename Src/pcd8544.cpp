////////////////////////////////////////////////////////////////////////////////
// PCD8544 Library
// Copyright (C) 2022 Ryan Clarke <kj6msg@icloud.com>
////////////////////////////////////////////////////////////////////////////////

#include "pcd8544.hpp"

#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_spi.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <string_view>


////////////////////////////////////////////////////////////////////////////////
// Static Data
////////////////////////////////////////////////////////////////////////////////

// clang-format off
////////////////////////////////////////////////////////////////////////////////
static constexpr std::array<std::uint8_t, 1536> font{
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x3eu, 0x45u, 0x51u, 0x45u, 0x3eu,
    0x00u, 0x3eu, 0x7bu, 0x6fu, 0x7bu, 0x3eu,
    0x00u, 0x1cu, 0x3eu, 0x7cu, 0x3eu, 0x1cu,
    0x00u, 0x18u, 0x3cu, 0x7eu, 0x3cu, 0x18u,
    0x00u, 0x18u, 0x5eu, 0x6eu, 0x5eu, 0x18u,
    0x00u, 0x18u, 0x5cu, 0x6eu, 0x5cu, 0x18u,
    0x00u, 0x00u, 0x18u, 0x18u, 0x00u, 0x00u,
    0xffu, 0xffu, 0xe7u, 0xe7u, 0xffu, 0xffu,
    0x00u, 0x18u, 0x24u, 0x24u, 0x18u, 0x00u,
    0xffu, 0xe7u, 0xdbu, 0xdbu, 0xe7u, 0xffu,
    0x70u, 0x88u, 0x88u, 0x8du, 0x73u, 0x07u,
    0x00u, 0x0eu, 0x51u, 0xf1u, 0x51u, 0x0eu,
    0x00u, 0x60u, 0x60u, 0x3fu, 0x02u, 0x04u,
    0x60u, 0x60u, 0x3fu, 0xc5u, 0xcau, 0x7cu,
    0x00u, 0x2au, 0x1cu, 0x36u, 0x1cu, 0x2au,
    0x00u, 0x3eu, 0x3eu, 0x1cu, 0x1cu, 0x08u,
    0x00u, 0x08u, 0x1cu, 0x1cu, 0x3eu, 0x3eu,
    0x00u, 0x14u, 0x36u, 0x7fu, 0x36u, 0x14u,
    0x00u, 0x00u, 0x5fu, 0x00u, 0x5fu, 0x00u,
    0x00u, 0x06u, 0x09u, 0x7fu, 0x01u, 0x7fu,
    0x40u, 0x9au, 0xa5u, 0xa5u, 0x59u, 0x02u,
    0x00u, 0xe0u, 0xe0u, 0xe0u, 0xe0u, 0xe0u,
    0x00u, 0x94u, 0xb6u, 0xffu, 0xb6u, 0x94u,
    0x00u, 0x08u, 0x0cu, 0xfeu, 0x0cu, 0x08u,
    0x00u, 0x10u, 0x30u, 0x7fu, 0x30u, 0x10u,
    0x08u, 0x08u, 0x08u, 0x3eu, 0x1cu, 0x08u,
    0x08u, 0x1cu, 0x3eu, 0x08u, 0x08u, 0x08u,
    0x00u, 0x0fu, 0x08u, 0x08u, 0x08u, 0x08u,
    0x08u, 0x1cu, 0x08u, 0x08u, 0x1cu, 0x08u,
    0x00u, 0x60u, 0x78u, 0x7eu, 0x78u, 0x60u,
    0x00u, 0x06u, 0x1eu, 0x7eu, 0x1eu, 0x06u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x5fu, 0x00u, 0x00u,
    0x00u, 0x00u, 0x07u, 0x00u, 0x07u, 0x00u,
    0x00u, 0x14u, 0x7fu, 0x14u, 0x7fu, 0x14u,
    0x00u, 0x24u, 0x2au, 0x6bu, 0x2au, 0x12u,
    0x00u, 0x22u, 0x15u, 0x2au, 0x54u, 0x22u,
    0x00u, 0x36u, 0x49u, 0x56u, 0x20u, 0x50u,
    0x00u, 0x00u, 0x0bu, 0x07u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x3eu, 0x41u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x41u, 0x3eu, 0x00u,
    0x00u, 0x08u, 0x2au, 0x1cu, 0x2au, 0x08u,
    0x00u, 0x08u, 0x08u, 0x3eu, 0x08u, 0x08u,
    0x00u, 0x00u, 0xa0u, 0x60u, 0x00u, 0x00u,
    0x00u, 0x08u, 0x08u, 0x08u, 0x08u, 0x08u,
    0x00u, 0x00u, 0x60u, 0x60u, 0x00u, 0x00u,
    0x00u, 0x60u, 0x30u, 0x18u, 0x0cu, 0x06u,
    0x00u, 0x3eu, 0x51u, 0x49u, 0x45u, 0x3eu,
    0x00u, 0x00u, 0x42u, 0x7fu, 0x40u, 0x00u,
    0x00u, 0x62u, 0x51u, 0x49u, 0x49u, 0x46u,
    0x00u, 0x22u, 0x49u, 0x49u, 0x49u, 0x36u,
    0x00u, 0x18u, 0x14u, 0x52u, 0x7fu, 0x50u,
    0x00u, 0x27u, 0x45u, 0x45u, 0x45u, 0x39u,
    0x00u, 0x3cu, 0x4au, 0x49u, 0x49u, 0x30u,
    0x00u, 0x01u, 0x01u, 0x79u, 0x05u, 0x03u,
    0x00u, 0x36u, 0x49u, 0x49u, 0x49u, 0x36u,
    0x00u, 0x06u, 0x49u, 0x49u, 0x29u, 0x1eu,
    0x00u, 0x00u, 0x6cu, 0x6cu, 0x00u, 0x00u,
    0x00u, 0x00u, 0xacu, 0x6cu, 0x00u, 0x00u,
    0x00u, 0x08u, 0x14u, 0x22u, 0x41u, 0x00u,
    0x00u, 0x14u, 0x14u, 0x14u, 0x14u, 0x14u,
    0x00u, 0x00u, 0x41u, 0x22u, 0x14u, 0x08u,
    0x00u, 0x06u, 0x01u, 0x51u, 0x09u, 0x06u,
    0x00u, 0x3eu, 0x41u, 0x5du, 0x55u, 0x5eu,
    0x00u, 0x7eu, 0x11u, 0x11u, 0x11u, 0x7eu,
    0x00u, 0x7fu, 0x49u, 0x49u, 0x49u, 0x36u,
    0x00u, 0x3eu, 0x41u, 0x41u, 0x41u, 0x22u,
    0x00u, 0x7fu, 0x41u, 0x41u, 0x22u, 0x1cu,
    0x00u, 0x7fu, 0x49u, 0x49u, 0x49u, 0x41u,
    0x00u, 0x7fu, 0x09u, 0x09u, 0x09u, 0x01u,
    0x00u, 0x3eu, 0x41u, 0x41u, 0x51u, 0x72u,
    0x00u, 0x7fu, 0x08u, 0x08u, 0x08u, 0x7fu,
    0x00u, 0x00u, 0x41u, 0x7fu, 0x41u, 0x00u,
    0x00u, 0x30u, 0x40u, 0x40u, 0x40u, 0x3fu,
    0x00u, 0x7fu, 0x08u, 0x14u, 0x22u, 0x41u,
    0x00u, 0x7fu, 0x40u, 0x40u, 0x40u, 0x40u,
    0x00u, 0x7fu, 0x06u, 0x18u, 0x06u, 0x7fu,
    0x00u, 0x7fu, 0x06u, 0x08u, 0x30u, 0x7fu,
    0x00u, 0x3eu, 0x41u, 0x41u, 0x41u, 0x3eu,
    0x00u, 0x7fu, 0x09u, 0x09u, 0x09u, 0x06u,
    0x00u, 0x3eu, 0x41u, 0x51u, 0x21u, 0x5eu,
    0x00u, 0x7fu, 0x09u, 0x09u, 0x19u, 0x66u,
    0x00u, 0x26u, 0x49u, 0x49u, 0x49u, 0x32u,
    0x00u, 0x01u, 0x01u, 0x7fu, 0x01u, 0x01u,
    0x00u, 0x3fu, 0x40u, 0x40u, 0x40u, 0x3fu,
    0x00u, 0x07u, 0x18u, 0x60u, 0x18u, 0x07u,
    0x00u, 0x7fu, 0x20u, 0x18u, 0x20u, 0x7fu,
    0x00u, 0x63u, 0x14u, 0x08u, 0x14u, 0x63u,
    0x00u, 0x03u, 0x0cu, 0x78u, 0x0cu, 0x03u,
    0x00u, 0x61u, 0x51u, 0x49u, 0x45u, 0x43u,
    0x00u, 0x00u, 0x7fu, 0x41u, 0x41u, 0x00u,
    0x00u, 0x06u, 0x0cu, 0x18u, 0x30u, 0x60u,
    0x00u, 0x00u, 0x41u, 0x41u, 0x7fu, 0x00u,
    0x00u, 0x04u, 0x02u, 0x01u, 0x02u, 0x04u,
    0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u,
    0x00u, 0x00u, 0x00u, 0x01u, 0x02u, 0x00u,
    0x00u, 0x20u, 0x54u, 0x54u, 0x54u, 0x78u,
    0x00u, 0x7fu, 0x28u, 0x44u, 0x44u, 0x38u,
    0x00u, 0x38u, 0x44u, 0x44u, 0x44u, 0x28u,
    0x00u, 0x38u, 0x44u, 0x44u, 0x28u, 0x7fu,
    0x00u, 0x38u, 0x54u, 0x54u, 0x54u, 0x18u,
    0x00u, 0x08u, 0x7eu, 0x09u, 0x01u, 0x02u,
    0x00u, 0x18u, 0xa4u, 0xa4u, 0xa8u, 0x7cu,
    0x00u, 0x7fu, 0x08u, 0x04u, 0x04u, 0x78u,
    0x00u, 0x00u, 0x44u, 0x7du, 0x40u, 0x00u,
    0x00u, 0x40u, 0x80u, 0x84u, 0x7du, 0x00u,
    0x00u, 0x7fu, 0x10u, 0x28u, 0x44u, 0x00u,
    0x00u, 0x00u, 0x41u, 0x7fu, 0x40u, 0x00u,
    0x00u, 0x7cu, 0x04u, 0x78u, 0x04u, 0x78u,
    0x00u, 0x7cu, 0x08u, 0x04u, 0x04u, 0x78u,
    0x00u, 0x38u, 0x44u, 0x44u, 0x44u, 0x38u,
    0x00u, 0xfcu, 0x28u, 0x44u, 0x44u, 0x38u,
    0x00u, 0x38u, 0x44u, 0x44u, 0x28u, 0xfcu,
    0x00u, 0x44u, 0x78u, 0x44u, 0x04u, 0x08u,
    0x00u, 0x48u, 0x54u, 0x54u, 0x54u, 0x24u,
    0x00u, 0x04u, 0x3fu, 0x44u, 0x40u, 0x20u,
    0x00u, 0x3cu, 0x40u, 0x40u, 0x20u, 0x7cu,
    0x00u, 0x1cu, 0x20u, 0x40u, 0x20u, 0x1cu,
    0x00u, 0x3cu, 0x40u, 0x30u, 0x40u, 0x3cu,
    0x00u, 0x44u, 0x28u, 0x10u, 0x28u, 0x44u,
    0x00u, 0x1cu, 0xa0u, 0xa0u, 0xa0u, 0x7cu,
    0x00u, 0x44u, 0x64u, 0x54u, 0x4cu, 0x44u,
    0x00u, 0x08u, 0x36u, 0x41u, 0x41u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x7fu, 0x00u, 0x00u,
    0x00u, 0x00u, 0x41u, 0x41u, 0x36u, 0x08u,
    0x00u, 0x02u, 0x01u, 0x02u, 0x04u, 0x02u,
    0x00u, 0x78u, 0x44u, 0x42u, 0x44u, 0x78u,
    0x00u, 0xbeu, 0xc1u, 0xc1u, 0x41u, 0x22u,
    0x00u, 0x3cu, 0x41u, 0x40u, 0x21u, 0x7cu,
    0x00u, 0x38u, 0x54u, 0x56u, 0x55u, 0x18u,
    0x00u, 0x20u, 0x56u, 0x55u, 0x56u, 0x78u,
    0x00u, 0x20u, 0x55u, 0x54u, 0x55u, 0x78u,
    0x00u, 0x20u, 0x55u, 0x56u, 0x54u, 0x78u,
    0x00u, 0x20u, 0x54u, 0x55u, 0x54u, 0x78u,
    0x00u, 0xb8u, 0xc4u, 0xc4u, 0x44u, 0x28u,
    0x00u, 0x38u, 0x56u, 0x55u, 0x56u, 0x18u,
    0x00u, 0x38u, 0x55u, 0x54u, 0x55u, 0x18u,
    0x00u, 0x38u, 0x55u, 0x56u, 0x54u, 0x18u,
    0x00u, 0x00u, 0x45u, 0x7cu, 0x41u, 0x00u,
    0x00u, 0x00u, 0x46u, 0x7du, 0x42u, 0x00u,
    0x00u, 0x00u, 0x45u, 0x7eu, 0x40u, 0x00u,
    0x00u, 0x7cu, 0x13u, 0x12u, 0x13u, 0x7cu,
    0x00u, 0x7cu, 0x12u, 0x13u, 0x12u, 0x7cu,
    0x00u, 0x7eu, 0x4au, 0x4bu, 0x4bu, 0x43u,
    0x00u, 0x74u, 0x54u, 0x78u, 0x54u, 0x5cu,
    0x00u, 0x7eu, 0x09u, 0x7eu, 0x49u, 0x49u,
    0x00u, 0x38u, 0x46u, 0x45u, 0x46u, 0x38u,
    0x00u, 0x38u, 0x45u, 0x44u, 0x45u, 0x38u,
    0x00u, 0x38u, 0x45u, 0x46u, 0x44u, 0x38u,
    0x00u, 0x3cu, 0x42u, 0x41u, 0x22u, 0x7cu,
    0x00u, 0x3cu, 0x41u, 0x42u, 0x20u, 0x7cu,
    0x00u, 0x1cu, 0xa1u, 0xa0u, 0xa1u, 0x7cu,
    0x00u, 0x3cu, 0x43u, 0x42u, 0x43u, 0x3cu,
    0x00u, 0x3eu, 0x41u, 0x40u, 0x41u, 0x3eu,
    0x00u, 0x38u, 0x44u, 0xc6u, 0x44u, 0x28u,
    0x00u, 0x48u, 0x7eu, 0x49u, 0x49u, 0x42u,
    0x00u, 0x29u, 0x2au, 0xfcu, 0x2au, 0x29u,
    0x00u, 0x7fu, 0x09u, 0x29u, 0xf6u, 0xa0u,
    0x00u, 0x40u, 0x88u, 0x7eu, 0x09u, 0x02u,
    0x00u, 0x20u, 0x54u, 0x56u, 0x55u, 0x78u,
    0x00u, 0x00u, 0x44u, 0x7eu, 0x41u, 0x00u,
    0x00u, 0x38u, 0x44u, 0x46u, 0x45u, 0x38u,
    0x00u, 0x3cu, 0x40u, 0x42u, 0x21u, 0x7cu,
    0x00u, 0x7cu, 0x09u, 0x05u, 0x05u, 0x78u,
    0x00u, 0x7eu, 0x0du, 0x19u, 0x31u, 0x7eu,
    0x00u, 0x26u, 0x29u, 0x29u, 0x27u, 0x28u,
    0x00u, 0x26u, 0x29u, 0x29u, 0x26u, 0x00u,
    0x00u, 0x30u, 0x48u, 0x45u, 0x40u, 0x30u,
    0x00u, 0x78u, 0x08u, 0x08u, 0x08u, 0x08u,
    0x08u, 0x08u, 0x08u, 0x08u, 0x78u, 0x00u,
    0x00u, 0x17u, 0x08u, 0x04u, 0x6au, 0x58u,
    0x00u, 0x17u, 0x08u, 0x34u, 0x22u, 0x70u,
    0x00u, 0x00u, 0x00u, 0x7du, 0x00u, 0x00u,
    0x08u, 0x14u, 0x22u, 0x08u, 0x14u, 0x22u,
    0x22u, 0x14u, 0x08u, 0x22u, 0x14u, 0x08u,
    0x11u, 0x44u, 0x11u, 0x44u, 0x11u, 0x44u,
    0x55u, 0xaau, 0x55u, 0xaau, 0x55u, 0xaau,
    0xeeu, 0xbbu, 0xeeu, 0xbbu, 0xeeu, 0xbbu,
    0x00u, 0x00u, 0x00u, 0xffu, 0x00u, 0x00u,
    0x08u, 0x08u, 0x08u, 0xffu, 0x00u, 0x00u,
    0x14u, 0x14u, 0x14u, 0xffu, 0x00u, 0x00u,
    0x08u, 0x08u, 0xffu, 0x00u, 0xffu, 0x00u,
    0x08u, 0x08u, 0xf8u, 0x08u, 0xf8u, 0x00u,
    0x14u, 0x14u, 0x14u, 0xfcu, 0x00u, 0x00u,
    0x14u, 0x14u, 0xf7u, 0x00u, 0xffu, 0x00u,
    0x00u, 0x00u, 0xffu, 0x00u, 0xffu, 0x00u,
    0x14u, 0x14u, 0xf4u, 0x04u, 0xfcu, 0x00u,
    0x14u, 0x14u, 0x17u, 0x10u, 0x1fu, 0x00u,
    0x08u, 0x08u, 0x0fu, 0x08u, 0x0fu, 0x00u,
    0x14u, 0x14u, 0x14u, 0x1fu, 0x00u, 0x00u,
    0x08u, 0x08u, 0x08u, 0xf8u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x0fu, 0x08u, 0x08u,
    0x08u, 0x08u, 0x08u, 0x0fu, 0x08u, 0x08u,
    0x08u, 0x08u, 0x08u, 0xf8u, 0x08u, 0x08u,
    0x00u, 0x00u, 0x00u, 0xffu, 0x08u, 0x08u,
    0x08u, 0x08u, 0x08u, 0x08u, 0x08u, 0x08u,
    0x08u, 0x08u, 0x08u, 0xffu, 0x08u, 0x08u,
    0x00u, 0x00u, 0x00u, 0xffu, 0x14u, 0x14u,
    0x00u, 0x00u, 0xffu, 0x00u, 0xffu, 0x08u,
    0x00u, 0x00u, 0x1fu, 0x10u, 0x17u, 0x14u,
    0x00u, 0x00u, 0xfcu, 0x04u, 0xf4u, 0x14u,
    0x14u, 0x14u, 0x17u, 0x10u, 0x17u, 0x14u,
    0x14u, 0x14u, 0xf4u, 0x04u, 0xf4u, 0x14u,
    0x00u, 0x00u, 0xffu, 0x00u, 0xf7u, 0x14u,
    0x14u, 0x14u, 0x14u, 0x14u, 0x14u, 0x14u,
    0x14u, 0x14u, 0xf7u, 0x00u, 0xf7u, 0x14u,
    0x14u, 0x14u, 0x14u, 0x17u, 0x14u, 0x14u,
    0x08u, 0x08u, 0x0fu, 0x08u, 0x0fu, 0x08u,
    0x14u, 0x14u, 0x14u, 0xf4u, 0x14u, 0x14u,
    0x08u, 0x08u, 0xf8u, 0x08u, 0xf8u, 0x08u,
    0x00u, 0x00u, 0x0fu, 0x08u, 0x0fu, 0x08u,
    0x00u, 0x00u, 0x00u, 0x1fu, 0x14u, 0x14u,
    0x00u, 0x00u, 0x00u, 0xfcu, 0x14u, 0x14u,
    0x00u, 0x00u, 0xf8u, 0x08u, 0xf8u, 0x08u,
    0x08u, 0x08u, 0xffu, 0x08u, 0xffu, 0x08u,
    0x14u, 0x14u, 0x14u, 0xffu, 0x14u, 0x14u,
    0x08u, 0x08u, 0x08u, 0x0fu, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0xf8u, 0x08u, 0x08u,
    0xffu, 0xffu, 0xffu, 0xffu, 0xffu, 0xffu,
    0xf0u, 0xf0u, 0xf0u, 0xf0u, 0xf0u, 0xf0u,
    0xffu, 0xffu, 0xffu, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0xffu, 0xffu, 0xffu,
    0x0fu, 0x0fu, 0x0fu, 0x0fu, 0x0fu, 0x0fu,
    0x00u, 0x38u, 0x44u, 0x44u, 0x38u, 0x44u,
    0x40u, 0x7cu, 0x02u, 0x4au, 0x4au, 0x34u,
    0x00u, 0x7fu, 0x01u, 0x01u, 0x01u, 0x01u,
    0x04u, 0x7cu, 0x04u, 0x04u, 0x7cu, 0x04u,
    0x00u, 0x63u, 0x55u, 0x49u, 0x41u, 0x41u,
    0x00u, 0x38u, 0x44u, 0x44u, 0x4cu, 0x34u,
    0x00u, 0xfcu, 0x40u, 0x40u, 0x40u, 0x7cu,
    0x00u, 0x08u, 0x04u, 0x7cu, 0x08u, 0x04u,
    0x00u, 0x1cu, 0x63u, 0x7fu, 0x63u, 0x1cu,
    0x00u, 0x3eu, 0x49u, 0x49u, 0x49u, 0x3eu,
    0x00u, 0x5eu, 0x61u, 0x01u, 0x61u, 0x5eu,
    0x00u, 0x38u, 0x46u, 0x45u, 0x45u, 0x3au,
    0x38u, 0x44u, 0x44u, 0x38u, 0x44u, 0x38u,
    0x00u, 0x38u, 0xc4u, 0x7cu, 0x46u, 0x38u,
    0x00u, 0x1cu, 0x2au, 0x49u, 0x49u, 0x00u,
    0x00u, 0x7eu, 0x01u, 0x01u, 0x01u, 0x7eu,
    0x00u, 0x2au, 0x2au, 0x2au, 0x2au, 0x2au,
    0x00u, 0x48u, 0x48u, 0x7eu, 0x48u, 0x48u,
    0x00u, 0x00u, 0xc1u, 0xa2u, 0x94u, 0x88u,
    0x00u, 0x88u, 0x94u, 0xa2u, 0xc1u, 0x00u,
    0x00u, 0x00u, 0x00u, 0xfcu, 0x02u, 0x0cu,
    0x00u, 0x30u, 0x40u, 0x3fu, 0x00u, 0x00u,
    0x00u, 0x08u, 0x08u, 0x2au, 0x08u, 0x08u,
    0x00u, 0x24u, 0x12u, 0x24u, 0x48u, 0x24u,
    0x00u, 0x06u, 0x09u, 0x09u, 0x06u, 0x00u,
    0x00u, 0x00u, 0x18u, 0x18u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x08u, 0x00u, 0x00u, 0x00u,
    0x10u, 0x20u, 0x40u, 0xffu, 0x01u, 0x01u,
    0x00u, 0x00u, 0x0fu, 0x02u, 0x01u, 0x0eu,
    0x00u, 0x00u, 0x09u, 0x0du, 0x0au, 0x00u,
    0x00u, 0x3cu, 0x3cu, 0x3cu, 0x3cu, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u
};
// clang-format on


////////////////////////////////////////////////////////////////////////////////
// Public Member Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
PCD8544::PCD8544(SPI_TypeDef* spi_port, GPIO_TypeDef* sce_port,
    unsigned int sce_pin, GPIO_TypeDef* rst_port, unsigned int rst_pin,
    GPIO_TypeDef* dc_port, unsigned int dc_pin)
    : m_spi_port(spi_port), m_sce_port(sce_port), m_sce_pin(sce_pin),
      m_rst_port(rst_port), m_rst_pin(rst_pin), m_dc_port(dc_port),
      m_dc_pin(dc_pin)
{
    LL_SPI_Enable(m_spi_port);

    LL_GPIO_SetOutputPin(m_sce_port, m_sce_pin);

    LL_GPIO_ResetOutputPin(m_rst_port, m_rst_pin);
    LL_GPIO_SetOutputPin(m_rst_port, m_rst_pin);

    send(WriteType::command, FUNC_SET | EXTEND);
    send(WriteType::command, SET_VOP | static_cast<std::uint8_t>(m_vop));
    send(WriteType::command, TEMP_CTRL | TEMP0);

    send(WriteType::command, SET_BIAS | static_cast<std::uint8_t>(bias));

    send(WriteType::command, FUNC_SET | BASIC);
    send(WriteType::command, DISP_CTRL | NORMAL);

    clear();
}


////////////////////////////////////////////////////////////////////////////////
PCD8544::~PCD8544()
{
    LL_SPI_Disable(m_spi_port);

    m_spi_port = nullptr;
    m_sce_port = nullptr;
    m_rst_port = nullptr;
    m_dc_port  = nullptr;
}


////////////////////////////////////////////////////////////////////////////////
void PCD8544::set_contrast(int level)
{
    // datasheet specifies Vop should be less than 8.5V for low temperatures
    if(level > max_vop)
        level = max_vop;   // Vop = 3.06V + 0.06V * 90 = 8.46V

    m_vop = level;

    send(WriteType::command, FUNC_SET | EXTEND);
    send(WriteType::command, SET_VOP | static_cast<std::uint8_t>(m_vop));
    send(WriteType::command, FUNC_SET | BASIC);
}


////////////////////////////////////////////////////////////////////////////////
void PCD8544::clear()
{
    set_cursor(0, 0);

    for(int n{0}; n != screen_width * rows; ++n)
        send(WriteType::data, 0U);
}


////////////////////////////////////////////////////////////////////////////////
void PCD8544::set_cursor(int column, int row)
{
    m_x_addr = (column % columns) * font_width;
    m_y_addr = row % rows;

    send(WriteType::command, SET_X_ADDR | static_cast<std::uint8_t>(m_x_addr));
    send(WriteType::command, SET_Y_ADDR | static_cast<std::uint8_t>(m_y_addr));
}


////////////////////////////////////////////////////////////////////////////////
void PCD8544::print(char c)
{
    if((c < ' ') || (c == '\x7F'))
    {
        switch(c)
        {
        case '\n':
            set_cursor(0, m_y_addr + 1);
            break;

        case '\f':
            clear();
            break;

        case '\r':
            set_cursor(0, m_y_addr);
            break;

        default:
            break;
        }
    }
    else
    {
        write(static_cast<std::uint8_t>(c));
    }
}


////////////////////////////////////////////////////////////////////////////////
void PCD8544::print(std::string_view s)
{
    for(const auto c : s)
        print(c);
}


////////////////////////////////////////////////////////////////////////////////
void PCD8544::write(unsigned char c)
{
    auto font_it = std::next(font.begin(), font_width * static_cast<int>(c));
    std::for_each_n(font_it, font_width,
        [&](const auto f) { send(WriteType::data, f); });

    m_x_addr = (m_x_addr + font_width) % screen_width;

    if(m_x_addr == 0)
        m_y_addr = (m_y_addr + 1) % rows;
}


////////////////////////////////////////////////////////////////////////////////
void PCD8544::set_ram_addr(int x, int y)
{
    m_x_addr = x % screen_width;
    m_y_addr = y % rows;

    send(WriteType::command, SET_X_ADDR | static_cast<std::uint8_t>(m_x_addr));
    send(WriteType::command, SET_Y_ADDR | static_cast<std::uint8_t>(m_y_addr));
}


////////////////////////////////////////////////////////////////////////////////
void PCD8544::set_pixels(std::uint8_t pixels)
{
    send(WriteType::data, pixels);

    m_x_addr = (m_x_addr + 1) % screen_width;

    if(m_x_addr == 0)
        m_y_addr = (m_y_addr + 1) % rows;
}


//////////////////////////////////////////////////////////////////////////////..
void PCD8544::draw_bitmap(
    const std::array<std::uint8_t, screen_width * banks>& bmp)
{
    set_ram_addr(0, 0);

    for(const auto pixels : bmp)
        send(WriteType::data, pixels);
}


////////////////////////////////////////////////////////////////////////////////
// Private Member Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
void PCD8544::send(WriteType type, std::uint8_t data)
{
    if(type == WriteType::command)
        LL_GPIO_ResetOutputPin(m_dc_port, m_dc_pin);
    else
        LL_GPIO_SetOutputPin(m_dc_port, m_dc_pin);

    LL_GPIO_ResetOutputPin(m_sce_port, m_sce_pin);

    LL_SPI_TransmitData8(m_spi_port, data);
    while(!LL_SPI_IsActiveFlag_TXE(m_spi_port)) {}

    while(LL_SPI_IsActiveFlag_BSY(m_spi_port)) {}

    LL_GPIO_SetOutputPin(m_sce_port, m_sce_pin);
}
