#include <string>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#define Block 1

void Decode(char* dst)
{
    char cipherBlockTemp[Block];
    memset(cipherBlockTemp, 'S', Block);

    for (int i = 0; i < Block; i++)
    {
        dst[i] = dst[i] ^ cipherBlockTemp[i];
    }
    return;
}

int Decryption(const char* inputfile, const char* outputfile)
{
    std::ifstream ifs(inputfile, std::ios::binary);
    if (ifs.fail())
    {
        std::cerr << "Failed to open inputfile." << std::endl;

        return -1;
    }

    std::ofstream ofs(outputfile, std::ios::binary);
    if (ofs.fail())
    {
        std::cerr << "Failed to open outputfile." << std::endl;

        return -1;
    }

    int decoding_size;
    ifs.read((char*)&decoding_size, sizeof(decoding_size));

    int file_size;
    ifs.read((char*)&file_size, sizeof(file_size));

    std::vector<char> enginedata(file_size);
    ifs.read(enginedata.data(), file_size);

    ifs.seekg(2*sizeof(int), std::ifstream::beg);

    char data[Block];
    char initialData[Block];
    char dataTemp[Block];
    char cipherBlockPre[Block];
    char decodeBlock[Block];

    memset(initialData, 'I', Block);

    int pos = ifs.tellg();
    ifs.read(data, Block);

    memcpy(cipherBlockPre, data, Block);

    Decode(data);

    for (int i = 0; i < Block; i++)
    {
        decodeBlock[i] = data[i] ^ initialData[i];
    }

    ofs.write(decodeBlock, Block);

    int counter = 1;
    do {
        if (counter == decoding_size) break;

        pos = ifs.tellg();
        ifs.read(data, Block);

        memcpy(dataTemp, data, Block);

        Decode(data);

        for (int i = 0; i < Block; i++)
        {
            decodeBlock[i] = data[i] ^ cipherBlockPre[i];
        }

        ofs.write(decodeBlock, Block);
        memcpy(cipherBlockPre, dataTemp, Block);

        counter++;
    }

    while (true);

    std::vector<char> dst;
    std::copy(enginedata.begin() + decoding_size, enginedata.end(), std::back_inserter(dst));

    int dst_size = file_size - decoding_size;
    ofs.write(&dst[0], dst_size);

    return 0;
}