#ifndef APIC_H
#define APIC_H

#include "common.h"

typedef struct RSDPDescriptor {
    char Signature[8];
    uint8_t Checksum;
    char OEMID[6];
    uint8_t Revision;
    uint32_t RsdtAddress;
} __attribute__ ((packed)) RSDPDescriptor;
#define RSDPDescriptorSize (sizeof(RSDPDescriptor))


//typedef struct RSDPDescriptor20 {
//    RSDPDescriptor firstPart;

//    uint32_t Length;
//    uint64_t XsdtAddress;
//    uint8_t ExtendedChecksum;
//    uint8_t reserved[3];
//} __attribute__ ((packed)) RSDPDescriptor20;
//#define RSDPDescriptor20Size (sizeof(RSDPDescriptor20))



RSDPDescriptor *find_rsdp();


typedef struct ACPISDTHeader {
  char Signature[4];
  uint32_t Length;
  uint8_t Revision;
  uint8_t Checksum;
  char OEMID[6];
  char OEMTableID[8];
  uint32_t OEMRevision;
  uint32_t CreatorID;
  uint32_t CreatorRevision;
} __attribute__((packed)) ACPISDTHeader;


typedef struct RSDT {
  ACPISDTHeader h;
//  uint32_t PointerToOtherSDT[(h.Length - sizeof(h)) / 4];
  uint32_t PointerToOtherSDT[];
} RSDT;


ACPISDTHeader *find_apic(void *root_sdt);


#endif // APIC_H

