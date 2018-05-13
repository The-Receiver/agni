#ifndef ACPI_H
#define ACPI_H

#include <klib.h>
#include <stdint.h>

typedef struct {
    uint64_t sig;
    uint8_t checksum;
    char oem[6];
    uint8_t rev;
    uint32_t rsdt_addr;
} rsdpdesc_t;

#endif
