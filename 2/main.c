// main.c -- Kernel entry point, monitor initialization routines.

#include "common.h"
#include "monitor.h"
#include "multiboot.h"
#include "apic.h"


static void write_memory_map_entry(multiboot_memory_map_t *mmap) {
    uint64_t finish_addr = mmap->addr + mmap->len;
    monitor_write("memory-range: 0x");
    monitor_write_big_number(mmap->addr);
    monitor_write("-0x");
    monitor_write_big_number(finish_addr - 1);
    monitor_write(", type ");
    monitor_write_decimal_number(mmap->type);
    monitor_write("\n");
}


typedef struct ProcessorLocalAPIC {
    uint8_t acpi_processor_id;
    uint8_t apic_id;
    uint32_t flags;
} __attribute__ ((packed)) ProcessorLocalAPIC;

typedef struct MADTEntryHeader {
    uint8_t entry_type;
    uint8_t record_length;
} __attribute__ ((packed)) MADTEntryHeader;


static inline uint8_t uint8_by_offset(const uint8_t *start, uint32_t offset) {
    return *(start + offset);
}

static inline uint16_t uint16_by_offset(const uint8_t *start, uint32_t offset) {
    return *((uint16_t *)(start + offset));
}

static inline uint32_t uint32_by_offset(const uint8_t *start, uint32_t offset) {
    return *((uint32_t *)(start + offset));
}

static inline uint64_t uint64_by_offset(const uint8_t *start, uint32_t offset) {
    return *((uint64_t *)(start + offset));
}

const char *RESERVED_MUST_BE_ZERO_ERROR = "ERROR: reserved field must be zero.";

static void write_apic_info(ACPISDTHeader *h) {
    const uint32_t local_controller_address = uint32_by_offset((uint8_t *)h, 36);
    const uint32_t flags =  uint32_by_offset((uint8_t *)h, 40);
    const uint8_t *start = (uint8_t *)(h) + 44;
    assert((start - (uint8_t *)(h)) >= 0, 0);
    int local_apic_address_override_entry_exists = 0;
    while ((uint32_t)(start - (uint8_t *)(h)) < h->Length) {
        const uint8_t entry_type = uint8_by_offset(start, 0);
        const uint8_t record_length = uint8_by_offset(start, 1);

        switch(entry_type) {
        case 0: { // Processor Local APIC.
            uint8_t acpi_processor_id = uint8_by_offset(start, 2);
            (void)acpi_processor_id;
            uint8_t apic_id = uint8_by_offset(start, 3);
            uint32_t flags = uint32_by_offset(start, 4);
            assert(flags == 0 || flags == 1, RESERVED_MUST_BE_ZERO_ERROR);
            monitor_write("Local APIC ");
            monitor_write_decimal_number(apic_id);
            monitor_write("\n");
            break;
        }
        case 1: { // I/O APIC.
            uint8_t io_apic_id = uint8_by_offset(start, 2);
            uint8_t reserved = uint8_by_offset(start, 3);
            assert(reserved == 0, RESERVED_MUST_BE_ZERO_ERROR);
            uint32_t io_apic_address = uint32_by_offset(start, 4);
            uint32_t global_system_interrupt_base = uint32_by_offset(start, 8);

            monitor_write("IOAPIC ");
            monitor_write_decimal_number(io_apic_id);
            monitor_write(" at 0x");
            monitor_write_number(io_apic_address);
            monitor_write(" IRQs from ");
            monitor_write_decimal_number(global_system_interrupt_base);
            monitor_write("\n");
            break;
        }
        case 5: { // Local APIC Address Override.
            local_apic_address_override_entry_exists = 1;
            uint16_t reserved = uint16_by_offset(start, 2);
            assert(reserved == 0, RESERVED_MUST_BE_ZERO_ERROR);
            uint64_t local_apic_address = uint64_by_offset(start, 4);
            monitor_write("Local APICs accessible at 0x");
            monitor_write_big_number(local_apic_address);
            monitor_write("\n");
            break;
        }
        case 9: { // Processor Local x2APIC.
            monitor_write("Processor Local x2APIC.\n");
            uint16_t reserved = uint16_by_offset(start, 2);
            assert(reserved == 0, RESERVED_MUST_BE_ZERO_ERROR);
            uint32_t x2apic_id = uint32_by_offset(start, 4);
            monitor_write("Local x2APIC ");
            monitor_write_decimal_number(x2apic_id);
            monitor_write("\n");
            break;
        }
        default: break;
        }

        start += record_length;
    }
    if (local_apic_address_override_entry_exists == 0) {
        monitor_write("Local APICs accessible at 0x");
        monitor_write_number(local_controller_address);
        monitor_write("\n");
    }

    if (flags == 1) {
//        monitor_write("Dual 8259 Legacy PICs Installed\n");
        monitor_write("PC/AT dual PIC supported\n");
    }
    else if (flags) {
        monitor_write("ERROR: flags must be zero");
    }
}


//extern state_t common_thread;
int main(uint32_t magic, uint32_t addr) {
    monitor_clear();
    multiboot_info_t *mbi;

    // Check if booted by a Multiboot-compliant boot loader.
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        monitor_write("Invalid magic number: 0x");
        monitor_write_number(magic);
        return -1;
    }

    mbi = (multiboot_info_t *) addr;

    if (bit(mbi->flags, 2)) {
        monitor_write("cmdline: ");
        monitor_write((char *) mbi->cmdline);
        monitor_write("\n");
    }
    else {
        monitor_write("Failed to take command line from multiboot.");
    }

    if (bit(mbi->flags, 4) && bit(mbi->flags, 5)) {
        monitor_write("ERROR: Both bits 4 and 5 are set.\n");
        return -1;
    }

    if (bit(mbi->flags, 6))
    {
        multiboot_memory_map_t *mmap;

//        monitor_debug("mmap_addr = 0x", mbi->mmap_addr);
//        monitor_debug("mmap_length = 0x", mbi->mmap_length);
        for (mmap = (multiboot_memory_map_t *) mbi->mmap_addr;
             (uint32_t) mmap < mbi->mmap_addr + mbi->mmap_length;
             mmap = (multiboot_memory_map_t *) ((uint32_t)mmap + mmap->size + 4))
        {
            write_memory_map_entry(mmap);
        }
    }
    else {
        monitor_write("Failed to take memory map from multiboot.");
    }

    RSDPDescriptor *rsdp = find_rsdp();
//    monitor_debug("rsdp = ", (uint32_t)rsdp);

    if (!rsdp) {
        monitor_write("ERROR: Unable to find RSDP. \n");
        return -1;
    }


    ACPISDTHeader *apic = find_apic((void *)rsdp->RsdtAddress);

//    monitor_debug("apic = ", (uint32_t)apic);

    write_apic_info(apic);

//    monitor_write("Bye!\n");

    return 0;
}
