#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif                          // Da se ukinu neka upozorenja za Visual Studio

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <immintrin.h>      // AVX2 instruction set


bool isPrime(int num);


// gcc -O3 -msse2 -o your_program your_code.c
int main(int argc, char** argv) {

    const char* inputFileName = (char*)calloc(30, sizeof(char));
    const char* outputFileName = (char*)calloc(30, sizeof(char));

    inputFileName = argv[1];
    outputFileName = argv[2];

    if (argc != 3) { inputFileName = "input_0-1000.bin"; outputFileName = "output.txt"; }
    // Ucitavanje ulaznog i izlaznog fajla


    int first = 0, last = 0;
    int numOfRanges = 0, alltogetherPrimes = 0;

    FILE* infp = fopen(inputFileName, "rb");
    FILE* outfp = fopen(outputFileName, "w");


    fread(&numOfRanges, sizeof(int), 1, infp);

    for (int i = 0; i < numOfRanges; i++)
    {
        int count = 0;
        fread(&first, sizeof(int), 1, infp);
        fread(&last, sizeof(int), 1, infp);

        // Brojanje prostih brojeva
        for (int i = first; i <= last; i++) {
            if (isPrime(i)) {
                count++;
            }
        }
        alltogetherPrimes += count;

        // Upis u izlazni fajl
        fprintf(outfp, "The number of prime numbers between %d and %d is: %d\n", first, last, count);
    }

    fprintf(outfp, "\n     --All together: %d--\n", alltogetherPrimes);



    fclose(outfp);
    fclose(infp);
    return 0;
}



bool isPrime(int num)
{
    if (num <= 1 || num == 4 || num == 6)
        return false;

    if (num == 2 || num == 3 || num == 5)
        return true;


    __m128 number = _mm_setr_ps((float)num, (float)num, (float)num, (float)num);


    for (int i = 2; i < num/2; i+=4)
    {
        __m128 iterationPair = _mm_setr_ps(i + 1, i + 2, i + 3, i + 4);
        __m128 result = _mm_div_ps(number, iterationPair);
        
        __m128i integerResult = _mm_cvttps_epi32(result);

        __m128 floatResult = _mm_cvtepi32_ps(integerResult);
        
        result = _mm_sub_ps(result, floatResult);
        result = _mm_cmpeq_ps(result, _mm_setzero_ps());

        int mask = _mm_movemask_ps(result);

        if (mask != 0) return false;
    }

    return true;
}

/*
* 
* Logicnija implementacija ali, iz nekog razloga, na Linux-u nece da
* se kompajlira funkcija _mm_rem_epi32 !
* 
* 
int isPrime(int num) {
    if (num <= 1) {
        return 0;
    }


    __m128i num_vector = _mm_set1_epi32(num);

    // Check if num is divisible by any number from 2 to num/2 in parallel    -- Ako se koristi sqrt(num) preskoci se poneki broj !!!
    for (int i = 2; i <= num / 2; i += 4) {
        __m128i i_vector = _mm_set_epi32(i, i + 1, i + 2, i + 3);
        __m128i remainder = _mm_rem_epi32(num_vector, i_vector);

        // Poredi remainder sa nulama i ako je neki remainder == 0, broj nije prost
        __m128i cmp_result = _mm_cmpeq_epi32(remainder, _mm_setzero_si128());

        // Kastovanje iz int u float
        __m128 result = _mm_castsi128_ps(cmp_result);

        // Smjestanje u masku
        int mask = _mm_movemask_ps(result);

        // ako je neki remainder == 0, broj nije prost
        if (mask != 0) {
            return 0;
        }
    }

    return 1;
}

*/

