// CryptoTestingApp.cpp : Defines the entry point for the console application.
//

#include <string.h>
#include <time.h>

#include <iostream>
#include <cstring>
#include <unistd.h>

#include "sgx_urts.h"
#include "SGXSRDSEnclave_u.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "server.h"
#include "common_socket.h"

#define BUFLEN 2048
#define SGX_AESGCM_MAC_SIZE 16
#define SGX_AESGCM_IV_SIZE 12

#define ENCLAVE_FILE "SGXSRDSEnclave.signed.so"

void emit_debug(const char *buf)

{
    // printf("DEBUG: %s\n", buf);
}

int main(int argc, char ** argv) {

	printf("Starting app...\n");

	/* Setup enclave */
	sgx_enclave_id_t eid;
	sgx_status_t ret;
	sgx_launch_token_t token = { 0 };
	int token_updated = 0;

	ret = sgx_create_enclave(ENCLAVE_FILE, SGX_DEBUG_FLAG, &token, &token_updated, &eid, NULL);
	if (ret != SGX_SUCCESS)
	{
		printf("sgx_create_enclave failed: %#x\n", ret);
		getchar();
		return 1;
	}

	char * arg;
	int port;
	if (argc != 2) {
		port = 8080;
	} else {
		port = atoi(argv[1]);
	}

	for (int i = 1; i <= argc; i++) {
		arg = argv[i];
		std::cout << arg << std::endl;
	}

	startServer(port);

	return 0;
}

/*

int main()
{
	printf("Starting app...\n");
	
	// Setup enclave
	sgx_enclave_id_t eid;
	sgx_status_t ret;
	sgx_launch_token_t token = { 0 };
	int token_updated = 0;
	
	ret = sgx_create_enclave(ENCLAVE_FILE, SGX_DEBUG_FLAG, &token, &token_updated, &eid, NULL);
	if (ret != SGX_SUCCESS)
	{
		printf("sgx_create_enclave failed: %#x\n", ret);
		getchar();
		return 1;
	}

	float startTime, endTime, timeElapsed;
	int N = 1024;
	char message[N];
	message[N-1] = 0;
	static const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	for (int i = 1; i < N; ++i) {
		message[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
	}
	message[0] = 'A';
	message[N-1] = 0;

	// printf("Original message: %s\n", message);

	int IT = 100000;
	float averageE = 0;
	float averageD = 0;

	for (int i = 0; i < IT; i++){

		// The encrypted message will contain the MAC, the IV, and the encrypted message itself.
		size_t encMessageLen = (SGX_AESGCM_MAC_SIZE + SGX_AESGCM_IV_SIZE + strlen(message)); 
		char *encMessage = (char *) malloc((encMessageLen+1)*sizeof(char));

		printf("Encrypting...\n");
		startTime = (float)clock()/CLOCKS_PER_SEC;
		ret = encryptMessage(eid, message, strlen(message), encMessage, encMessageLen);
		encMessage[encMessageLen] = '\0';
		endTime = (float)clock()/CLOCKS_PER_SEC;
		timeElapsed = endTime - startTime;
		printf("[ENCRYPT] Time elapsed: %f ms\n", timeElapsed*1000);
		printf("[ENCRYPT] Bitrate: %f Mbps\n", 8*strlen(message)/(1000000*timeElapsed));
		// printf("Encrypted message: %s\n", encMessage);

		averageE += 8*strlen(message)/(1000000*timeElapsed);
	
		// The decrypted message will contain the same message as the original one.
		size_t decMessageLen = strlen(message);
		char *decMessage = (char *) malloc((decMessageLen+1)*sizeof(char));

		printf("Decrypting...\n");
		startTime = (float)clock()/CLOCKS_PER_SEC;
		ret = decryptMessage(eid,encMessage,encMessageLen,decMessage,decMessageLen);
		decMessage[decMessageLen] = '\0';
		endTime = (float)clock()/CLOCKS_PER_SEC;
		timeElapsed = endTime - startTime;
		printf("[DECRYPT] Time elapsed: %f ms\n", timeElapsed*1000);
		printf("[DECRYPT] Bitrate: %f Mbps\n", 8*strlen(message)/(1000000*timeElapsed));
		// printf("Decrypted message: %s", decMessage);

		averageD += 8*strlen(message)/(1000000*timeElapsed);

	}

	printf("Finalizing...\n");

	printf("Buffer size: %i bytes\n", N);
	printf("Iterations: %i\n", IT);

	averageE /= IT;
	averageD /= IT;

	printf("[ENCRYPT] Average bitrate: %f Mbps (%f MBps)\n", averageE, averageE/8);
	printf("[DECRYPT] Average bitrate: %f Mbps (%f MBps)\n", averageD, averageD/8);

	getchar();

	return 0;
}

*/

