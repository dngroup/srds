#ifndef SGXSRDSENCLAVE_U_H__
#define SGXSRDSENCLAVE_U_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include <string.h>
#include "sgx_edger8r.h" /* for sgx_satus_t etc. */


#include <stdlib.h> /* for size_t */

#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif

void SGX_UBRIDGE(SGX_CDECL, emit_debug, (const char* str));
void SGX_UBRIDGE(SGX_CDECL, emit_debug_int, (int a));
void SGX_UBRIDGE(SGX_CDECL, ocall_startClient, (int* csock, char* address));
void SGX_UBRIDGE(SGX_CDECL, ocall_sendToClient, (int sock, char* request, int size, char* finalbuffer));
void SGX_UBRIDGE(SGX_CDECL, ocall_receiveFromClient, (int sock, char* finalbuffer));
void SGX_UBRIDGE(SGX_CDECL, ocall_sendanswer, (int csock, char* msg, int size));
void SGX_UBRIDGE(SGX_CDECL, ocall_int_to_string, (int a, char* chr));
void SGX_UBRIDGE(SGX_CDECL, ocall_string_to_int, (const char* a, int size, int* out));
void SGX_UBRIDGE(SGX_CDECL, ocall_closesocket, (int sock));
int SGX_UBRIDGE(SGX_CDECL, sgx_thread_wait_untrusted_event_ocall, (const void* self));
int SGX_UBRIDGE(SGX_CDECL, sgx_thread_set_untrusted_event_ocall, (const void* waiter));

sgx_status_t decryptMessage(sgx_enclave_id_t eid, char* encMessageIn, size_t len, char* decMessageOut, size_t lenOut);
sgx_status_t encryptMessage(sgx_enclave_id_t eid, char* decMessageIn, size_t len, char* encMessageOut, size_t lenOut);
sgx_status_t ecall_handlemessage(sgx_enclave_id_t eid, int csock, int type, char* msg, int size);
sgx_status_t ecall_init(sgx_enclave_id_t eid);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
