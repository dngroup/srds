#include "SGXSRDSEnclave_t.h"

#include "sgx_trts.h"
#include "sgx_tcrypto.h"
#include "string.h"
#include <string>
#include "stdlib.h"
#include <map>

#define BUFLEN 2048
static sgx_aes_gcm_128bit_key_t key = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf };

int extractSize(char * msg) {
	int size = ((unsigned char)msg[0] << 24) + ((unsigned char)msg [1] << 16) + ((unsigned char)msg[2] << 8) + (unsigned char)msg[3];
	return size;
}

char * extractBuffer(char * msg, int size) {
    int sizeIntinChar = 4;
    char * buffer = (char *)malloc(sizeof(char) * size);
    buffer = (char*)memset(buffer, '\0', size);

    for(int i = 0; i< size; i++) {
        buffer[i] = msg [i + sizeIntinChar];
    }
    return buffer;
}

int isHttp(char* msg) {
    std::string beginning(msg, 0, 4);
    if (beginning == "GET " || beginning == "POST" || beginning == "PUT " || beginning == "DELE" || beginning == "HTTP") {
        return 0;
    } else {
        return 1;
    }
}

char* substr(char* arr, int begin, int len)
{
    char* res = new char[len];
    for (int i = 0; i < len; i++)
        res[i] = *(arr + begin + i);
    res[len] = 0;
    return res;
}

int testEndTransferEncoding(char* msg, int size) {
    int sizeOfEndBufferMarker = 5;
    if (size < sizeOfEndBufferMarker) {
        return 1;
    }

    char* test = substr(msg,size-sizeOfEndBufferMarker, sizeOfEndBufferMarker);

    if (strcmp(test, "0\r\n\r\n")==0) {
        return 0;
    } else {
        return 1;
    }
}

int testContentLength(int contentLength, int totalSize) {
    if (totalSize >= contentLength) {
        return 0;
    } else {
        return 1;
    }
}

int getPosEndOfHeader(char * msg) {
    std::string allmsg(msg);
    return allmsg.find("\r\n\r\n");
}

std::string copystring(std::string string) {
    char *y = new char[string.length() + 1];
    std::strncpy(y, string.c_str(), string.length());
    std::string string2(y);
    return string2;
}

char* createNewHeader(char* msg, std::string address, int size) {
    std::string header(msg, 0, size);
    int posHost = header.find("Host: ") + 6;
    int posEnd = header.find("\r\n", posHost);

    header.replace(posHost, posEnd-posHost, address);
    header.insert(posEnd, "Connection: Close\r\n");

    char *y = new char[header.length() + 1];
    std::strncpy(y, header.c_str(), header.length());
    return y;
}

std::map<std::string, std::string> parse_headers(char * msg) {
	emit_debug(msg);
    int endPos = 0;
    int pos = 0;
    int oldPos = 0;
    std::string allmsg(msg);
    int posmiddle = 0;
    int firstSpace = 0;
    int secondSpace = 0;
    std::map<std::string, std::string> headers;

    endPos = allmsg.find("\r\n\r\n");
    std::string sSize1("HeaderSize");
    char * chr = (char*) malloc(1024);
    ocall_int_to_string((endPos + 4), chr);
    std::string sSize2(chr);
    headers[copystring(sSize1)] = copystring(sSize2);
    emit_debug(chr);

    pos = allmsg.find("\r\n");
    firstSpace = allmsg.find(" ");
    std::string sMethod1("Method");
    std::string sMethod2(msg, oldPos, firstSpace);
    headers[copystring(sMethod1)] = copystring(sMethod2);
    secondSpace = allmsg.find(" ", firstSpace + 1);
    std::string sPath1("Path");
    std::string sPath2(msg, firstSpace + 1, secondSpace - firstSpace - 1);
    headers[copystring(sPath1)] = copystring(sPath2);

    std::string sProto1("Protocol");
    std::string sProto2(msg, secondSpace + 1, pos - secondSpace - 1);
    headers[copystring(sProto1)] = copystring(sProto2);

    while (pos < endPos && pos > 0) {
        oldPos = pos;
        pos = allmsg.find("\r\n", pos + 1);
        posmiddle = allmsg.find(":", oldPos + 1);
        std::string s1(msg, oldPos + 2, posmiddle - oldPos - 2);
        std::string s2(msg, posmiddle + 2, pos - posmiddle - 2);
        headers[copystring(s1)] = copystring(s2);
    }

    return headers;
}

void handleProxy(int csock, char * msg, int msgsize) {
    char target[1024] = "msstream.net";
    int targetPort = 8023;

    char * answer;
    int client_sock;
    char * finalanswer;
    char answerFromClient[1028];
    char answerFromClient2[1028];
    int httpanswer;
    int testIsEnd = 0;
    int sizeAnswerFromClient = 0;
    int totalSizeAnswer = 0;
    std::map<std::string,std::string> headersAnswer;

    ocall_startClient(&client_sock, target, targetPort);
    answer = createNewHeader(msg, target, msgsize);
    //answerFromClient = (char*) malloc(1028);
    ocall_sendToClient(client_sock, answer, (int)strlen(answer), answerFromClient2);
    memcpy(answerFromClient,answerFromClient2,1028);
    sizeAnswerFromClient = extractSize(answerFromClient);
    finalanswer = extractBuffer(answerFromClient, sizeAnswerFromClient);
    //emit_debug_int(sizeAnswerFromClient);
    //emit_debug(answerFromClient);
    //emit_debug(finalanswer);

    httpanswer = isHttp(finalanswer);
    if(httpanswer == 0) {
        headersAnswer = parse_headers(finalanswer);
        emit_debug(headersAnswer["HeaderSize"].c_str());
        emit_debug(headersAnswer["Transfer-Encoding"].c_str());
        if (headersAnswer.count("Content-Length")>0) {
            //TODO Content-Length, then read data until the end and close socket
            int out;
            ocall_string_to_int(headersAnswer["HeaderSize"].c_str(),(int)strlen(headersAnswer["HeaderSize"].c_str()), &out);
            totalSizeAnswer += sizeAnswerFromClient - out;
            ocall_string_to_int(headersAnswer["Content-Length"].c_str(),(int)strlen(headersAnswer["HeaderSize"].c_str()), &out);
            while (testContentLength(out, totalSizeAnswer) != 0) {
                ocall_sendanswer(csock, finalanswer, sizeAnswerFromClient);
                free(finalanswer);
                ocall_receiveFromClient(client_sock, answerFromClient);
                sizeAnswerFromClient = extractSize(answerFromClient);
                finalanswer = extractBuffer(answerFromClient, sizeAnswerFromClient);
                //free(answerFromClient);
                totalSizeAnswer += sizeAnswerFromClient;
                ocall_string_to_int(headersAnswer["Content-Length"].c_str(),(int)strlen(headersAnswer["HeaderSize"].c_str()), &out);
            }
            ocall_sendanswer(csock, finalanswer, sizeAnswerFromClient);
            free(finalanswer);
        } else if (headersAnswer.count("Transfer-Encoding")>0) {
            //TODO Transfer-Encoding: chunked then look for the 0\r\n\r\n at the end of every packet. When found, close the socket
            //TODO Other idea: add a "Connection: close" header, so the connexion will be closed by the server

            while (testEndTransferEncoding(finalanswer, sizeAnswerFromClient) != 0 && sizeAnswerFromClient != 0) {
                ocall_sendanswer(csock, finalanswer, sizeAnswerFromClient);
                free(finalanswer);
                ocall_receiveFromClient(client_sock, answerFromClient);
                sizeAnswerFromClient = extractSize(answerFromClient);
                finalanswer = extractBuffer(answerFromClient, sizeAnswerFromClient);
                //free(answerFromClient);
            }
            ocall_sendanswer(csock, finalanswer, sizeAnswerFromClient);
            free(finalanswer);

        } else {
            ocall_sendanswer(csock, finalanswer, sizeAnswerFromClient);
            free(finalanswer);
        }
    } else {
        ocall_sendanswer(csock, finalanswer, sizeAnswerFromClient);
        free(finalanswer);
    }
}

void handleTracker(int csock, char * msg) {
    char answer[1024] = "HTTP/1.1 200 OK\r\nContent-Length: 17\r\nContent-Type: application/json\r\nConnection: Close\r\n\r\n[\"peer1\",\"peer2\"]\0";
    //char test[1024] = "GET / HTTP/1.1\r\nHost: lacaud.fr\r\nUser-Agent: curl/7.55.1\r\nConnection: close\r\nAccept: */*\r\n\r\n";

    ocall_sendanswer(csock, answer, strlen(answer));
}

void ecall_handlemessage(int csock, char * msg, int size){
    int http = isHttp(msg);
    if (http == 0) {
        handleProxy(csock, msg, size);
    }
}


void decryptMessage(char *encMessageIn, size_t len, char *decMessageOut, size_t lenOut)
{

	uint8_t *encMessage = (uint8_t *) encMessageIn;
	uint8_t p_dst[BUFLEN] = {0};

	sgx_rijndael128GCM_decrypt(
		&key,
		encMessage + SGX_AESGCM_MAC_SIZE + SGX_AESGCM_IV_SIZE,
		lenOut,
		p_dst,
		encMessage + SGX_AESGCM_MAC_SIZE, SGX_AESGCM_IV_SIZE,
		NULL, 0,
		(sgx_aes_gcm_128bit_tag_t *) encMessage);
	memcpy(decMessageOut, p_dst, lenOut);
        emit_debug((char *) p_dst);

}

void encryptMessage(char *decMessageIn, size_t len, char *encMessageOut, size_t lenOut)
{

	uint8_t *origMessage = (uint8_t *) decMessageIn;
	uint8_t p_dst[BUFLEN] = {0};

	// Generate the IV (nonce)
	sgx_read_rand(p_dst + SGX_AESGCM_MAC_SIZE, SGX_AESGCM_IV_SIZE);

	sgx_rijndael128GCM_encrypt(
		&key,
		origMessage, len, 
		p_dst + SGX_AESGCM_MAC_SIZE + SGX_AESGCM_IV_SIZE,
		p_dst + SGX_AESGCM_MAC_SIZE, SGX_AESGCM_IV_SIZE,
		NULL, 0,
		(sgx_aes_gcm_128bit_tag_t *) (p_dst));	
	memcpy(encMessageOut,p_dst,lenOut);

}
