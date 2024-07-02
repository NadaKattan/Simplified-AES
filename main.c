/*
 *@author:Nada
 */
/*Some Examples for testing:
 * key: 	        A73B
 * plaintext:   	6F6B
 * ciphertext:  	0738
 *
 * key: 	    	BBFF
 * plaintext:       1238
 * ciphertext: 	    720E
 * 
 * key: 	     	AB89
 * plaintext:  	    04B0
 * ciphertext:  	89A8
 */
#include <stdio.h>
typedef unsigned char uint8;
uint8 SBox[16] = {0x9, 0x4, 0xA, 0xB, 0xD, 0x1, 0x8, 0x5, 0x6, 0x2, 0x0, 0x3, 0xC, 0xE, 0xF, 0x7};
uint8 invSBox[16] = {0xA, 0x5, 0x9, 0xB, 0x1, 0x7, 0x8, 0xF, 0x6, 0x0, 0x2, 0x3, 0xC, 0x4, 0xD, 0xE};
uint8 Mult_table_y4[16] = {0x0, 0x4, 0x8, 0xC, 0x3, 0x7, 0xB, 0xF, 0x6, 0x2, 0xE, 0xA, 0x5, 0x1, 0xD, 0x9};
uint8 Mult_table_y2[16] = {0x0, 0x2, 0x4, 0x6, 0x8, 0xA, 0xC, 0xE, 0x3, 0x1, 0x7, 0x5, 0xB, 0x9, 0xF, 0xD};
uint8 Mult_table_y9[16] = {0x0, 0x9, 0x1, 0x8, 0x2, 0xB, 0x3, 0xA, 0x4, 0xD, 0x5, 0xC, 0x6, 0xF, 0x7, 0xE};
uint8 mix_arr[4] = {1, 4, 4, 1}, mix_res[4], temp_arr[2], res[4];
uint8 w0[2], w1[2], w2[2], w3[2], w4[2], w5[2];
uint8 k0[4], k1[4], k2[4];
uint8 RC1[2] = {0x8, 0x0}, RC2[2] = {0x3, 0x0};
uint8 ct[4], pt[4];
uint8 temp, process;
/**Fn prototypes**/
void concat(uint8 a[], uint8 b[], uint8 k[]);
void cpy_arr(uint8 a[], uint8 b[], uint8 size);
void swap4(uint8 a[]);
void swap2(uint8 b[]);
void sub(uint8 c[], uint8 size);
void invSub(uint8 c[], uint8 size);
void xor_arr(uint8 a[], uint8 b[], uint8 c[], uint8 size);
void mix_cols(uint8 res[]);
void invMix_cols(uint8 res[]);
void print_arr(uint8 arr[], uint8 size);
void expand();
void enc();
void dec();
void input_arr(uint8 arr[], uint8 size);
void split(uint8 a[], uint8 b[], uint8 k[]);

int main()
{
    do
    {
        printf("Please, Enter hex [0-9A-F]\n");
        printf("input 0: encryption, 1: decryption--> ");
        scanf("%d", &process);
        if (process == 0)
        {
            printf("Enter key:");
            input_arr(k0, 4);
            printf("Enter plaintext:");
            input_arr(pt, 4);
            expand();
            enc();
        }
        else if (process == 1)
        {
            printf("Enter key:");
            input_arr(k0, 4);
            printf("Enter ciphertext:");
            input_arr(ct, 4);

            expand();
            dec();
        }
        else
        {
            printf("Please Enter valid process\n");
        }
    } while ((!(process == 0 || process == 1)));

    return 0;
}

void input_arr(uint8 arr[], uint8 size)
{
    for (uint8 i = 0; i < size; i++)
    {
        scanf("%1X", &arr[i]);
    }
}
void split(uint8 a[], uint8 b[], uint8 k[])
{
    for (uint8 i = 0; i < 2; i++)
    {
        a[i] = k[i];
    }
    for (uint8 i = 2; i < 4; i++)
    {
        b[i - 2] = k[i];
    }
}
void concat(uint8 a[], uint8 b[], uint8 k[])
{
    for (uint8 i = 0; i < 2; i++)
    {
        k[i] = a[i];
    }
    for (uint8 i = 2; i < 4; i++)
    {
        k[i] = b[i - 2];
    }
}
void cpy_arr(uint8 a[], uint8 b[], uint8 size)
{
    for (uint8 i = 0; i < size; i++)
    {
        a[i] = b[i];
    }
}
void swap4(uint8 a[])
{
    temp = a[1];
    a[1] = a[3];
    a[3] = temp;
}
void swap2(uint8 b[])
{
    temp = b[0];
    b[0] = b[1];
    b[1] = temp;
}
void sub(uint8 c[], uint8 size)
{
    for (uint8 i = 0; i < size; i++)
    {
        c[i] = SBox[c[i]];
    }
}
void invSub(uint8 c[], uint8 size)
{
    for (uint8 i = 0; i < size; i++)
    {
        c[i] = invSBox[c[i]];
    }
}
void xor_arr(uint8 a[], uint8 b[], uint8 c[], uint8 size)
{
    for (uint8 i = 0; i < size; i++)
    {
        c[i] = a[i] ^ b[i];
    }
}
void mix_cols(uint8 res[])
{
    mix_res[0] = (res[0]) ^ (Mult_table_y4[res[1]]);
    mix_res[2] = (res[2]) ^ (Mult_table_y4[res[3]]);
    mix_res[1] = (Mult_table_y4[res[0]]) ^ (res[1]);
    mix_res[3] = (Mult_table_y4[res[2]]) ^ (res[3]);
}
void invMix_cols(uint8 res[])
{
    mix_res[0] = (Mult_table_y9[res[0]]) ^ (Mult_table_y2[res[1]]);
    mix_res[2] = (Mult_table_y9[res[2]]) ^ (Mult_table_y2[res[3]]);
    mix_res[1] = (Mult_table_y2[res[0]]) ^ (Mult_table_y9[res[1]]);
    mix_res[3] = (Mult_table_y2[res[2]]) ^ (Mult_table_y9[res[3]]);
}

void print_arr(uint8 arr[], uint8 size)
{
    for (uint8 i = 0; i < size; i++)
    {
        printf("%X", arr[i]);
    }
    printf("\n");
}
void expand()
{
    split(w0, w1, k0);
    // RN
    cpy_arr(temp_arr, w1, 2);
    swap2(temp_arr);
    // SN
    sub(temp_arr, 2);
    xor_arr(temp_arr, RC1, temp_arr, 2);
    xor_arr(temp_arr, w0, w2, 2);
    xor_arr(w2, w1, w3, 2);
    cpy_arr(temp_arr, w3, 2);
    swap2(temp_arr);
    sub(temp_arr, 2);
    xor_arr(temp_arr, RC2, temp_arr, 2);
    xor_arr(temp_arr, w2, w4, 2);
    xor_arr(w3, w4, w5, 2);
    concat(w2, w3, k1);
    concat(w4, w5, k2);
}
void enc()
{
    // pt xor key0
    xor_arr(pt, k0, res, 4);
    /*Round 1*/
    // Nibble Sub
    sub(res, 4);
    // shifting
    swap4(res);
    // Mix columns
    mix_cols(res);
    xor_arr(mix_res, k1, mix_res, 4);
    /*Final Round*/
    // Nibble Sub
    sub(mix_res, 4);
    // shifting
    swap4(mix_res);
    xor_arr(mix_res, k2, ct, 4);
    printf("ciphertext:");
    print_arr(ct, 4);
}
void dec()
{
    xor_arr(ct, k2, res, 4);
    /**Round 1**/
    // inverse shifting
    swap4(res);
    // inverse Nibble Sub
    invSub(res, 4);
    xor_arr(res, k1, res, 4);
    // inverse Mix columns
    invMix_cols(res);
    /**Final Round**/
    // inverse shifting
    swap4(mix_res);
    // inverse Nibble Sub
    invSub(mix_res, 4);
    xor_arr(mix_res, k0, pt, 4);
    printf("plaintext:");
    print_arr(pt, 4);
}