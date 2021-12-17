#include "Encryption.h"
#include "Decryption.h"

#include <iostream>
#include <time.h>

int main()
{
    clock_t start = clock();

    const char* engine_file = "data\\fcn-resnet101.engine";
    const char* encrypted_file = "output\\encrypted.bin";
    const char* decryption_file = "output\\decryption.engine";
    const int encrypt_size = 128;

    //Encryption(engine_file, encrypted_file, encrypt_size);

    Decryption(encrypted_file, decryption_file);

    clock_t end = clock();

    const double time = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    printf("time %lf[s]\n", time);

    return 0;
}
