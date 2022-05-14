////////////////////////////////////////////////////////////////////////////////
// PCD8544 Library
// Copyright 2022 Ryan Clarke
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
////////////////////////////////////////////////////////////////////////////////

#ifndef PCD8544_HPP
#define PCD8544_HPP

#include "stm32f411xe.h"

#include <array>
#include <cstdint>
#include <string_view>


////////////////////////////////////////////////////////////////////////////////
/// @brief PCD8544 LCD display.
////////////////////////////////////////////////////////////////////////////////
class PCD8544
{
  public:
    static constexpr int screen_width{84};
    static constexpr int screen_height{48};
    static constexpr int banks{6};
    static constexpr int pixels_per_bank{screen_height / banks};

    static constexpr int font_width{6};
    static constexpr int font_height{8};

    static constexpr int columns{screen_width / font_width};
    static constexpr int rows{screen_height / font_height};

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Constructor.
    /// @param spi_port SPI port
    /// @param sce_port chip enable port
    /// @param sce_pin  chip enable pin
    /// @param rst_port reset port
    /// @param rst_pin  reset pin
    /// @param dc_port  mode select port
    /// @param dc_pin   mode select pin
    ////////////////////////////////////////////////////////////////////////////
    PCD8544(SPI_TypeDef* spi_port, GPIO_TypeDef* sce_port, unsigned int sce_pin,
        GPIO_TypeDef* rst_port, unsigned int rst_pin, GPIO_TypeDef* dc_port,
        unsigned int dc_pin);

    PCD8544(const PCD8544&)            = delete;
    PCD8544& operator=(const PCD8544&) = delete;
    PCD8544(PCD8544&&)                 = delete;
    PCD8544&& operator=(PCD8544&&)     = delete;

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Destructor.
    ////////////////////////////////////////////////////////////////////////////
    ~PCD8544();

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Set display contrast level.
    /// @param level contrast level [49-90]
    ////////////////////////////////////////////////////////////////////////////
    void set_contrast(int level) noexcept;

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Clears the display.
    ////////////////////////////////////////////////////////////////////////////
    void clear() noexcept;

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Set the cursor position.
    /// @param column horizontal coordinate [0-13]
    /// @param row    vertical coordinate [0-5]
    ////////////////////////////////////////////////////////////////////////////
    void set_cursor(int column, int row) noexcept;

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Print a character. Processes NL, FF, and CR.
    /// @param c character
    ////////////////////////////////////////////////////////////////////////////
    void print(char c);

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Print a string. Processes NL, FF, and CR.
    /// @param s string
    ////////////////////////////////////////////////////////////////////////////
    void print(std::string_view s);

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Print a character. No processing of control codes.
    /// @param c character
    ////////////////////////////////////////////////////////////////////////////
    void write(unsigned char c);

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Set RAM address.
    /// @param x horizontal coordinate [0-83]
    /// @param y vertical coordinate [0-5]
    ////////////////////////////////////////////////////////////////////////////
    void set_ram_addr(int x, int y) noexcept;

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Set a column of eight pixels at current RAM address.
    /// @param pixels pixel data
    ////////////////////////////////////////////////////////////////////////////
    void set_pixels(std::uint8_t pixels) noexcept;

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Draw a bitmap image.
    /// @param bmp bitmap
    ////////////////////////////////////////////////////////////////////////////
    void draw_bitmap(
        const std::array<std::uint8_t, screen_width * banks>& bmp) noexcept;

  private:
    ////////////////////////////////////////////////////////////////////////////
    /// @brief PCD8544 write mode.
    ////////////////////////////////////////////////////////////////////////////
    enum class WriteType
    {
        command,
        data
    };

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Send a byte to the display.
    /// @param type command or data
    /// @param data byte to send
    ////////////////////////////////////////////////////////////////////////////
    void send(WriteType type, std::uint8_t data) const noexcept;

    SPI_TypeDef* m_spi_port{nullptr};

    GPIO_TypeDef* m_sce_port{nullptr};
    unsigned int m_sce_pin{0};

    GPIO_TypeDef* m_rst_port{nullptr};
    unsigned int m_rst_pin{0};

    GPIO_TypeDef* m_dc_port{nullptr};
    unsigned int m_dc_pin{0};

    int m_vop{69};   // m_vop = 3.06V + 0.06V * 69 = 7.2V
    int m_x_addr{0};
    int m_y_addr{0};

    // commands and flags
    static constexpr std::uint8_t NOP{0x00U};
    static constexpr std::uint8_t FUNC_SET{0x20U};

    static constexpr std::uint8_t DISP_CTRL{0x08U};
    static constexpr std::uint8_t SET_Y_ADDR{0x40U};
    static constexpr std::uint8_t SET_X_ADDR{0x80U};

    static constexpr std::uint8_t TEMP_CTRL{0x04U};
    static constexpr std::uint8_t SET_BIAS{0x10U};
    static constexpr std::uint8_t SET_VOP{0x80U};

    static constexpr std::uint8_t BASIC{0x00U};
    static constexpr std::uint8_t EXTEND{0x01U};

    static constexpr std::uint8_t HORIZONTAL{0x00U};
    static constexpr std::uint8_t VERTICAL{0x02U};

    static constexpr std::uint8_t ACTIVE{0x00U};
    static constexpr std::uint8_t POWERDOWN{0x40U};

    static constexpr std::uint8_t BLANK{0x00U};
    static constexpr std::uint8_t ALL_ON{0x01U};
    static constexpr std::uint8_t NORMAL{0x04U};
    static constexpr std::uint8_t INVERSE{0x05U};

    static constexpr std::uint8_t TEMP0{0x00U};
    static constexpr std::uint8_t TEMP1{0x01U};
    static constexpr std::uint8_t TEMP2{0x10U};
    static constexpr std::uint8_t TEMP3{0x11U};
};


#endif   // PCD8544_HPP
