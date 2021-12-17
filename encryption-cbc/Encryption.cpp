#include <string>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#define Block 1

void Cipher(char* dst)
{
    char cipherBlockTemp[Block];
    memset(cipherBlockTemp, 'S', Block);

    for (int i = 0; i < Block; i++)
    {
        dst[i] = dst[i] ^ cipherBlockTemp[i];
    }

    return;
}

int Encryption(const char* inputfile, const char* outputfile, const int encryptsize)
{
    std::ifstream ifs(inputfile, std::ios::binary);
    if (ifs.fail())
    {
        std::cerr << "Failed to open inputfile." << std::endl;

        return -1;
    }

    ifs.seekg(0, std::ifstream::end);
    const int file_size = ifs.tellg();
    ifs.seekg(0, std::ifstream::beg);

    std::ofstream ofs(outputfile, std::ios::binary);

    std::vector<int> vec_int{ encryptsize, file_size };
    ofs.write(reinterpret_cast<char*>(&vec_int[0]), sizeof(int) * 2);

    std::vector<char> enginedata(file_size);
    ifs.read(enginedata.data(), file_size);

    ifs.seekg(0, std::ifstream::beg);

    char data[Block];
    char initialData[Block];
    char cipherBlockPre[Block];
    char cipherBlock[Block];

    memset(initialData, 'I', Block);

    ifs.read(data, Block);

    for (int i = 0; i < Block; i++)
    {
        cipherBlock[i] = data[i] ^ initialData[i];
    }

    Cipher(cipherBlock);

    ofs.write(cipherBlock, Block);

    memcpy(cipherBlockPre, cipherBlock, Block);

    int counter = 1;
    do {
        if (counter == encryptsize) break;

        ifs.read(data, Block);

        for (int i = 0; i < Block; i++)
        {
            cipherBlock[i] = data[i] ^ cipherBlockPre[i];
        }

        Cipher(cipherBlock);

        ofs.write(cipherBlock, Block);

        memcpy(cipherBlockPre, cipherBlock, Block);

        counter++;
    }

    while (true);

    std::vector<char> dst;
    std::copy(enginedata.begin() + encryptsize, enginedata.end(), std::back_inserter(dst));

    int dst_size = file_size - encryptsize;
    ofs.write(&dst[0], dst_size);

    return 0;
}