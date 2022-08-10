#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include "zlib.h"

void* myalloc OF((void*, unsigned, unsigned));
void myfree OF((void*, void*));

void* myalloc(void* q, unsigned n, unsigned m)
{
    (void)q;
    return calloc(n, m);
}

void myfree(void* q, void* p)
{
    (void)q;
    free(p);
}

int __fastcall uncompress2(Bytef* out_uncompressed_buffer, uInt* out_uncompressed_size, Bytef* compressed_buffer, uInt compressed_size, alloc_func alloc_func, free_func free_func, void* opaque)
{
    int v8;
    int v9;
    int result;
    z_stream z;

    z.next_in = compressed_buffer;
    z.avail_in = compressed_size;
    v8 = *out_uncompressed_size;
    z.next_out = out_uncompressed_buffer;
    z.zalloc = alloc_func;
    z.zfree = free_func;
    z.opaque = opaque;
    z.avail_out = v8;
    v9 = inflateInit_(&z, "1.1.4", 56);
    if (v9)
        goto LABEL_7;
    v9 = inflate(&z, 4);
    if (v9 == 1)
    {
        *out_uncompressed_size = z.total_out;
        v9 = inflateEnd(&z);
        goto LABEL_7;
    }
    inflateEnd(&z);
    if (v9)
    {
    LABEL_7:
        result = v9;
        goto LABEL_5;
    }
    result = -5;
LABEL_5:
    return result;
}

int main(int argc, char* argv[])
{
    std::cout << "Inflating file: ";
    std::cout << argv[1];
    std::cout << "\n";


    std::ifstream infile(argv[1], std::ios_base::binary);
    size_t compressedLength = 0;
    int unknown = 0;
    size_t uncompressedLength = 0;

    infile.read((char*)&compressedLength, 4);
    infile.read((char*)&unknown, 4);
    infile.read((char*)&uncompressedLength, 4);
    compressedLength = _byteswap_ulong(compressedLength);
    unknown = _byteswap_ulong(unknown);
    uncompressedLength = _byteswap_ulong(uncompressedLength);

    if (unknown != 1) {
        std::cout << "Unrecognized filetype.";
        return 1;
    }

    char* compressedBuffer = new char[compressedLength];
    char* uncompressedBuffer = new char[uncompressedLength];

    infile.read(compressedBuffer, compressedLength - 12);

    uncompress2(
        (Bytef*)uncompressedBuffer,
        &uncompressedLength,
        (Bytef*)compressedBuffer,
        compressedLength,
        myalloc,
        myfree,
        nullptr);

    FILE* file = fopen(argv[1], "wb");
    fwrite(uncompressedBuffer, 1, uncompressedLength, file);
}
