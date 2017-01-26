#include "apic.h"
#include "monitor.h"



static int validate_first_rsdp(RSDPDescriptor *rsdp10) {
    uint32_t acc = 0;

    int i;
    for (i = 0; i < (int)sizeof(RSDPDescriptor); ++i) {
        acc += ((uint8_t *)rsdp10)[i];
    }
    return acc & 7;
}

//static int validate_second_rsdp(RSDPDescriptor20 *rsdp20) {
//    int acc = 0;

//    int i;
//    for (i = 0; i < (int)sizeof(RSDPDescriptor20); ++i) {
//        acc += *((uint8_t *)(rsdp20) + i);
//    }

//    int first_rsdp_validator = validate_first_rsdp(&rsdp20->firstPart);
//    if (first_rsdp_validator || acc & 7) {
//        monitor_write("ERROR: RSDP 2.0 is broken");
//    }
//    else {
//        monitor_write("RSDP 2.0 is valid");
//    }
//    return (acc & 7) | first_rsdp_validator;
//}


//static const uint8_t RSDP_SIGNATURE[8] = {'R', 'S', 'D', ' ', 'P', 'T', 'R', ' '};
static const uint8_t RSDP_SIGNATURE[8] = "RSD PTR ";
#define RSDP_SIGNATURE_SIZE (sizeof(RSDP_SIGNATURE))


static RSDPDescriptor *check_for_rsdp(uint8_t *ptr) {
    const int memcmp_result = memcmp(ptr, RSDP_SIGNATURE, RSDP_SIGNATURE_SIZE);
    if (memcmp_result) return 0;
    RSDPDescriptor *probable_rsdp = (RSDPDescriptor *)(ptr);

    if (validate_first_rsdp(probable_rsdp)) return 0;
//    monitor_debug("version: ", probable_rsdp->Revision);
    assert(probable_rsdp->Revision == 0, "ERROR: Only ACPI 1.0 is supported!");
    return probable_rsdp;
}


RSDPDescriptor *find_rsdp()
{
    uint8_t *p;
    uint8_t *ebda = (void *)((*(uint16_t *)(0x040e)) * 16);
    const uint8_t *ebda_min = (uint8_t *)0x80000;
    const uint8_t *ebda_max = (uint8_t *)(0xa0000) - RSDPDescriptorSize;
    //        void *ebda = (void *)(((uint32_t) * (uint16_t *)(EBDA_PTR_LOC)) << 4);
    if (ebda >= ebda_min && ebda < ebda_max && (uint32_t)(ebda) % 16 == 0) {
        for (p = ebda; p <= ebda_max; p += 16) {
            RSDPDescriptor *rsdp = check_for_rsdp(p);
            if (rsdp) return rsdp;
        }
    }
    for (p = (uint8_t *)0xe0000; (uint8_t *)p <= (uint8_t *)0xfffff - RSDPDescriptorSize; p += 1) {
        RSDPDescriptor *rsdp = check_for_rsdp(p);
        if (rsdp) return rsdp;
    }
    return 0;
}


static int acpi_header_checksum(ACPISDTHeader *table_header)
{
    uint8_t sum = 0;
    uint8_t i;
//    monitor_write_decimal_number(table_header->Length);
    for (i = 0; i < table_header->Length; i++) {
        sum += ((uint8_t*) table_header)[i];
    }
    return sum;
}


ACPISDTHeader *find_apic(void *root_sdt)
{
    RSDT *rsdt = (RSDT *) root_sdt;
    uint32_t i, entries = (rsdt->h.Length - sizeof(rsdt->h)) / 4;

    for (i = 0; i < entries; i++)
    {
        ACPISDTHeader *h = (ACPISDTHeader *)(rsdt->PointerToOtherSDT[i]);
        if (memcmp(h->Signature, "APIC", 4) == 0 && acpi_header_checksum(h) == 0) {
            return h;
        }
    }

    assert(0, "No APIC found.");

    return 0;
}
