/* Copyright 2014, Kenneth MacKay. Licensed under the BSD 2-clause license. */

#include "uECC.h"
#include "sha3.h"

#include <stdio.h>
#include <string.h>


void print_uint8 (const char * title, uint8_t * data, int32_t dataLen) 
{
	printf("%s", title) ;
	int i ;
	for (i = 0; i < dataLen; i++) {
		printf("%02X",data[i]) ;
	}
	printf("\n") ;
}


int main() 
{
	int i, c;
    	const struct uECC_Curve_t * curve = uECC_secp256k1();
 
    	uint8_t privMaster[32] = {0xce,0xed,0x45,0x2a,0x28,0x79,0xec,0xb6,0x63,0x8f,0x8a,0x62,0xbe,0xbb,0x10,0xfc,0x47,0x11,0xf2,0x8a,0x40,0xc0,0x8e,0x68,0xed,0x78,0x88,0x53,0x02,0x0a,0x1b,0x5a} ;
	uint8_t privObj   [32] = {0xf6,0x2a,0x0c,0x39,0xa3,0x44,0xe8,0xb1,0x00,0xc6,0x0c,0xf9,0xc1,0xac,0x9f,0xc0,0xea,0xbf,0x61,0xea,0x20,0x32,0x4d,0xb0,0x69,0xfb,0xef,0x72,0xe7,0xde,0x2e,0xe4} ;

/*    	uint8_t public[64]  = {0xc7,0x9e,0xa9,0xbb,0xd5,0x89,0x75,0xad,0x57,0x0c,0x3b,0x5c,0x27,0x6a,0x4f,0xf0,
			       0xb1,0x1a,0x42,0x9d,0x78,0xd2,0xb5,0x12,0x57,0x55,0xe8,0x72,0xa5,0x03,0x5f,0xc7,
			       0x55,0xe4,0x8a,0x29,0x26,0xfa,0x9d,0x12,0xd9,0xbc,0xba,0xf3,0x2a,0x5e,0x81,0xbe,
			       0x10,0x23,0x27,0x3d,0x5c,0x38,0xeb,0x3c,0x4c,0xee,0x91,0x0f,0xe7,0x61,0x43,0xd8} ;
		
*/	
	uint8_t pub[64] = {0} ;
	uint8_t pubMaster [64] = {0x2b,0x2f,0xd9,0x2c,0x45,0x10,0x59,0xaf,0xa2,0x47,0x33,0xc8,0x6a,0x1f,0xd8,0xbf,0x1c,0x1b,0xe5,0xe8,0x92,0x9f,0x39,0xa3,0xd6,0x70,0xb0,0xcb,0x57,0x41,0x81,0x00,0x7f,0x22,0x97,0x11,0xd8,0x93,0xe1,0x8d,0xe4,0x41,0xea,0x54,0x3e,0x0a,0x6a,0x0c,0x81,0xd8,0x5e,0xdd,0x64,0x2b,0xe8,0x3b,0x79,0xb7,0x05,0xeb,0x7c,0x7a,0xa1,0x57} ;
    	uint8_t hash[32] = {0} ;
    	uint8_t sig[64]  = {0} ;


        if (!uECC_compute_public_key(privObj, pub, curve)) {
	        printf("uECC_make_key() failed\n");
                return 1;
        }

	print_uint8 ("pub:",pub, 64) ;

	sha3_ctx ctx ;
	rhash_sha3_256_init(&ctx) ;
	rhash_sha3_update(&ctx, (const unsigned char *) pub, 64) ;
	rhash_keccak_final(&ctx, hash) ;

	if (hash == NULL) {
                printf("hash == NULL\n");
                return 1;
	}

	print_uint8 ("address :",hash, 32) ;



    	printf("\n\nTesting 256 signatures\n");


	uint8_t data [1+1+20] = {0x0a,0x02,0x8c,0xf2,0xc2,0x4f,0xf8,0x18,0x44,0x5a,0x4f,0x65,0x27,0x2a,0xe4,0xad,0x4c,0x88,0x0c,0xd9,0xfe,0x05} ;


	// memcpy(data+2,hash+12, 20) ;

	sha3_ctx ctx2 ;
	rhash_sha3_256_init(&ctx2) ;
	rhash_sha3_update(&ctx2, (const unsigned char *) data, 28) ;
	rhash_keccak_final(&ctx2, hash) ;
	
	if (hash == NULL) {
                printf("hash == NULL\n") ;
                return 1;
	}

	uint8_t h [32] = {0x74,0x6a,0xf8,0xa6,0xb4,0x61,0x74,0x7c,0x3f,0xf0,0x64,0x2f,0x26,0xf5,0x9e,0x3b,0xf1,0x5a,0x78,0xa2,0x6a,0x6c,0x32,0x52,0xed,0x5d,0xda,0x61,0xc4,0x2d,0x3c,0x26} ;
	
	print_uint8 ("hash: ",h, 32) ;

           // memcpy(hash, public, sizeof(hash));
            
        if (!uECC_sign(privMaster, h, sizeof(h), sig, curve)) {
        	printf("uECC_sign() failed\n");
                return 1;
        }

	print_uint8 ("sig:",sig, 64) ;

        if (!uECC_verify(pubMaster, h, sizeof(h), sig, curve)) {
        	printf("uECC_verify() failed\n");
        	return 1;
        }




        printf("signature verification finished with success\n");

    
    return 0;
}
