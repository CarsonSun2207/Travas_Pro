#ifndef MFRC522_H
#define MFRC522_H

#include <cstdint>

class CMFRC522 {
public:
    virtual void PCD_Init() = 0;
    virtual int isNewCardPresent() = 0;
    virtual void RC522_Anticoll(unsigned char *id) = 0;

    unsigned char _id[8];  // Ensure this matches the implementation
};

#endif // MFRC522_H
