struct Entry
    {
        uint32_t key;         // 4 bytes
        uint64_t value;       // 8 bytes
        uint8_t occupied : 1; // 1 bit
        uint8_t padding : 7;  // 7 bits para completar 1 byte
        Entry() : key(0), value(0), occupied(0) {}
        Entry(uint32_t k, uint64_t v, uint8_t occ) : key(k), value(v), occupied(occ) {}
    };