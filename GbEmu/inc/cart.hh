#pragma once

#include "common.hh"
#include <cstdint>
#include <stdlib.h>
#include <stdio.h>


namespace DMG01
{

typedef struct {
    std::uint8_t entry[4];
    std::uint8_t logo[0x30];

    char title[16];
    std::uint16_t new_lic_code;
    std::uint8_t sgb_flag;
    std::uint8_t type;
    std::uint8_t rom_size;
    std::uint8_t ram_size;
    std::uint8_t dest_code;
    std::uint8_t lic_code;
    std::uint8_t version;
    std::uint8_t checksum;
    std::uint16_t global_checksum;
} rom_header;

class Cart
{
public:
    Cart(){}

    bool        load        (char* fname);
    word_t      read        (address_t addr);
    void        write       (address_t addr, word_t val);

    const char* licenceName ();
    const char* typeName    ();
    void        printHeader ();

private:

    rom_header    header         {};
    char          filename[1024] {};
    std::uint32_t rom_size       {};
    word_t        *rom_data;

};
}