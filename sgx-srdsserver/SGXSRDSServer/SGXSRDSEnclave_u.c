#include "SGXSRDSEnclave_u.h"
#include <errno.h>

typedef struct ms_decryptMessage_t {
	char* ms_encMessageIn;
	size_t ms_len;
	char* ms_decMessageOut;
	size_t ms_lenOut;
} ms_decryptMessage_t;

typedef struct ms_encryptMessage_t {
	char* ms_decMessageIn;
	size_t ms_len;
	char* ms_encMessageOut;
	size_t ms_lenOut;
} ms_encryptMessage_t;

typedef struct ms_ecall_handlemessage_t {
	int ms_csock;
	int ms_type;
	char* ms_msg;
	int ms_size;
} ms_ecall_handlemessage_t;


typedef struct ms_emit_debug_t {
	char* ms_str;
} ms_emit_debug_t;

typedef struct ms_emit_debug_int_t {
	int ms_a;
} ms_emit_debug_int_t;

typedef struct ms_ocall_startClient_t {
	int* ms_csock;
	char* ms_address;
} ms_ocall_startClient_t;

typedef struct ms_ocall_sendToClient_t {
	int ms_sock;
	char* ms_request;
	int ms_size;
	char* ms_finalbuffer;
} ms_ocall_sendToClient_t;

typedef struct ms_ocall_receiveFromClient_t {
	int ms_sock;
	char* ms_finalbuffer;
} ms_ocall_receiveFromClient_t;

typedef struct ms_ocall_sendanswer_t {
	int ms_csock;
	char* ms_msg;
	int ms_size;
} ms_ocall_sendanswer_t;

typedef struct ms_ocall_int_to_string_t {
	int ms_a;
	char* ms_chr;
} ms_ocall_int_to_string_t;

typedef struct ms_ocall_string_to_int_t {
	char* ms_a;
	int ms_size;
	int* ms_out;
} ms_ocall_string_to_int_t;

typedef struct ms_ocall_closesocket_t {
	int ms_sock;
} ms_ocall_closesocket_t;

typedef struct ms_sgx_thread_wait_untrusted_event_ocall_t {
	int ms_retval;
	void* ms_self;
} ms_sgx_thread_wait_untrusted_event_ocall_t;

typedef struct ms_sgx_thread_set_untrusted_event_ocall_t {
	int ms_retval;
	void* ms_waiter;
} ms_sgx_thread_set_untrusted_event_ocall_t;

static sgx_status_t SGX_CDECL SGXSRDSEnclave_emit_debug(void* pms)
{
	ms_emit_debug_t* ms = SGX_CAST(ms_emit_debug_t*, pms);
	emit_debug((const char*)ms->ms_str);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL SGXSRDSEnclave_emit_debug_int(void* pms)
{
	ms_emit_debug_int_t* ms = SGX_CAST(ms_emit_debug_int_t*, pms);
	emit_debug_int(ms->ms_a);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL SGXSRDSEnclave_ocall_startClient(void* pms)
{
	ms_ocall_startClient_t* ms = SGX_CAST(ms_ocall_startClient_t*, pms);
	ocall_startClient(ms->ms_csock, ms->ms_address);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL SGXSRDSEnclave_ocall_sendToClient(void* pms)
{
	ms_ocall_sendToClient_t* ms = SGX_CAST(ms_ocall_sendToClient_t*, pms);
	ocall_sendToClient(ms->ms_sock, ms->ms_request, ms->ms_size, ms->ms_finalbuffer);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL SGXSRDSEnclave_ocall_receiveFromClient(void* pms)
{
	ms_ocall_receiveFromClient_t* ms = SGX_CAST(ms_ocall_receiveFromClient_t*, pms);
	ocall_receiveFromClient(ms->ms_sock, ms->ms_finalbuffer);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL SGXSRDSEnclave_ocall_sendanswer(void* pms)
{
	ms_ocall_sendanswer_t* ms = SGX_CAST(ms_ocall_sendanswer_t*, pms);
	ocall_sendanswer(ms->ms_csock, ms->ms_msg, ms->ms_size);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL SGXSRDSEnclave_ocall_int_to_string(void* pms)
{
	ms_ocall_int_to_string_t* ms = SGX_CAST(ms_ocall_int_to_string_t*, pms);
	ocall_int_to_string(ms->ms_a, ms->ms_chr);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL SGXSRDSEnclave_ocall_string_to_int(void* pms)
{
	ms_ocall_string_to_int_t* ms = SGX_CAST(ms_ocall_string_to_int_t*, pms);
	ocall_string_to_int((const char*)ms->ms_a, ms->ms_size, ms->ms_out);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL SGXSRDSEnclave_ocall_closesocket(void* pms)
{
	ms_ocall_closesocket_t* ms = SGX_CAST(ms_ocall_closesocket_t*, pms);
	ocall_closesocket(ms->ms_sock);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL SGXSRDSEnclave_sgx_thread_wait_untrusted_event_ocall(void* pms)
{
	ms_sgx_thread_wait_untrusted_event_ocall_t* ms = SGX_CAST(ms_sgx_thread_wait_untrusted_event_ocall_t*, pms);
	ms->ms_retval = sgx_thread_wait_untrusted_event_ocall((const void*)ms->ms_self);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL SGXSRDSEnclave_sgx_thread_set_untrusted_event_ocall(void* pms)
{
	ms_sgx_thread_set_untrusted_event_ocall_t* ms = SGX_CAST(ms_sgx_thread_set_untrusted_event_ocall_t*, pms);
	ms->ms_retval = sgx_thread_set_untrusted_event_ocall((const void*)ms->ms_waiter);

	return SGX_SUCCESS;
}

static const struct {
	size_t nr_ocall;
	void * table[11];
} ocall_table_SGXSRDSEnclave = {
	11,
	{
		(void*)SGXSRDSEnclave_emit_debug,
		(void*)SGXSRDSEnclave_emit_debug_int,
		(void*)SGXSRDSEnclave_ocall_startClient,
		(void*)SGXSRDSEnclave_ocall_sendToClient,
		(void*)SGXSRDSEnclave_ocall_receiveFromClient,
		(void*)SGXSRDSEnclave_ocall_sendanswer,
		(void*)SGXSRDSEnclave_ocall_int_to_string,
		(void*)SGXSRDSEnclave_ocall_string_to_int,
		(void*)SGXSRDSEnclave_ocall_closesocket,
		(void*)SGXSRDSEnclave_sgx_thread_wait_untrusted_event_ocall,
		(void*)SGXSRDSEnclave_sgx_thread_set_untrusted_event_ocall,
	}
};
sgx_status_t decryptMessage(sgx_enclave_id_t eid, char* encMessageIn, size_t len, char* decMessageOut, size_t lenOut)
{
	sgx_status_t status;
	ms_decryptMessage_t ms;
	ms.ms_encMessageIn = encMessageIn;
	ms.ms_len = len;
	ms.ms_decMessageOut = decMessageOut;
	ms.ms_lenOut = lenOut;
	status = sgx_ecall(eid, 0, &ocall_table_SGXSRDSEnclave, &ms);
	return status;
}

sgx_status_t encryptMessage(sgx_enclave_id_t eid, char* decMessageIn, size_t len, char* encMessageOut, size_t lenOut)
{
	sgx_status_t status;
	ms_encryptMessage_t ms;
	ms.ms_decMessageIn = decMessageIn;
	ms.ms_len = len;
	ms.ms_encMessageOut = encMessageOut;
	ms.ms_lenOut = lenOut;
	status = sgx_ecall(eid, 1, &ocall_table_SGXSRDSEnclave, &ms);
	return status;
}

sgx_status_t ecall_handlemessage(sgx_enclave_id_t eid, int csock, int type, char* msg, int size)
{
	sgx_status_t status;
	ms_ecall_handlemessage_t ms;
	ms.ms_csock = csock;
	ms.ms_type = type;
	ms.ms_msg = msg;
	ms.ms_size = size;
	status = sgx_ecall(eid, 2, &ocall_table_SGXSRDSEnclave, &ms);
	return status;
}

sgx_status_t ecall_init(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 3, &ocall_table_SGXSRDSEnclave, NULL);
	return status;
}

