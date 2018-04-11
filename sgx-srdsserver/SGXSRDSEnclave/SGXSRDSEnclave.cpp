#include "SGXSRDSEnclave_t.h"

#include "sgx_trts.h"
#include "sgx_tcrypto.h"
#include "string.h"
#include <string>
#include "stdlib.h"
#include <map>
#include "sgx_thread.h"

#define BUFLEN 2048
static sgx_aes_gcm_128bit_key_t key = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf };

sgx_thread_mutex_t mutex;

void ecall_init() {
    sgx_thread_mutex_init(&mutex, NULL);
}

std::string copystring(std::string string) {
    char *y = new char[string.length() + 1];
    std::strncpy(y, string.c_str(), string.length());
    y[string.length()] = '\0';
    std::string string2(y);
    return string2;
}

char * copystring2char(std::string string) {
    char *y = (char*)malloc(string.length() + 1);
    std::strncpy(y, string.c_str(), string.length());
    y[string.length()] = '\0';
    return y;
}

struct map_element {
    char* key;
    char* value;
    struct map_element* next;
};

struct map {
    struct map_element* first;
    struct map_element* last;
    int size;
};

struct map* map_init() {
    struct map* map = (struct map*)malloc(sizeof(struct map));
    map->first = NULL;
    map->last = NULL;
    map->size = 0;
    return map;
}

void map_add(struct map* map, std::string key, std::string value) {
    struct map_element* elt = (struct map_element*)malloc(sizeof(struct map_element));

    if (map->size == 0) {
        map->first = elt;
    }
    elt->key = copystring2char(key);
    elt->value = copystring2char(value);
    elt->next = NULL;
    if(map->last!=NULL){
        map->last->next = elt;
    }
    map->last = elt;
    map->size++;
}
struct map_element * map_get_next(struct map_element * element){
    return element->next;
}

void map_destroy(struct map* map) {
    struct map_element * current = map->first;
    struct map_element * current_old;
    while(current != NULL) {
        current_old=current;
        current = map_get_next(current_old);
        free(current_old->key);
        free(current_old->value);
        free(current_old);
    }
    free(map);
}

int map_find(struct map* map, std::string key) {

    char * key2 = copystring2char(key);
    struct map_element * current = map->first;
    if (current != NULL && strcmp(current->key,key2) == 0){
        return 1;
    }

    while(current != NULL) {

        if (current->key != NULL) {
            if (strcmp(current->key, key2) == 0) {
                return 1;
            }
        }

        current = map_get_next(current);

    }
    return 0;
}

char * map_get(struct map* map, std::string key) {
    char * key2 = copystring2char(key);
    struct map_element * current = map->first;
    if (current != NULL && strcmp(current->key,key2) == 0){
        return current->value;
    }

    while(current != NULL && current->key != NULL){
        if(strcmp(current->key,key2) == 0){
            return current->value;
        }
        current=map_get_next(current);
    }
    return NULL;
}


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
int isOption(char* msg) {
    std::string beginning(msg, 0, 4);
    if (beginning == "OPTI") {
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
    int result = 1;
    int sizeOfEndBufferMarker = 5;
    if (size < sizeOfEndBufferMarker) {
        return 1;
    }
    char* test = substr(msg,size-sizeOfEndBufferMarker, sizeOfEndBufferMarker);
    if (strcmp(test, "0\r\n\r\n")==0) {
        result = 0;
    } else {
        result =  1;
    }
    free(test);
    return result;
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



char* createNewHeader(char* msg, std::string address, int size) {
    std::string header(msg, 0, size);
    int posHost = header.find("Host: ") + 6;
    int posEnd = header.find("\r\n", posHost);
    int posConnection = header.find("Connection: ");
    if (posConnection != -1) {
        posConnection = posConnection + 12;
        int posEndConnection = header.find("\r\n", posConnection);
        header.replace(posConnection, posEndConnection-posConnection, "Close");
    } else {
        header.insert(posEnd + 3, "Connection: Close\r\n");
    }

    header.replace(posHost, posEnd-posHost, address);

    char *y = new char[header.length() + 1];
    std::strncpy(y, header.c_str(), header.length());
    y[header.length()] = '\0';
    return y;
}

char * addContentToAnswer(std::string header, std::string content) {
    //std::string header(answer);
    int pos = header.find("Content-Length: ") + 16;
    int posEnd = header.find("\r\n", pos);
    int size = content.length();

    char * chr = (char*) malloc(1024);
    ocall_int_to_string(size, chr);
    std::string s2(chr);


    header.replace(pos, posEnd-pos, s2);
    header += content;
;
    char *y = new char[header.length()];
    std::strncpy(y, header.c_str(), header.length());
    y[header.length()] = '\0';

    free(chr);
    return y;
}

struct map* parse_headers(char * msg) {
	struct map* headers = map_init();
    int endPos = 0;
    int pos = 0;
    int oldPos = 0;
    std::string allmsg(msg);
    int posmiddle = 0;
    int firstSpace = 0;
    int secondSpace = 0;
    //std::map<std::string, std::string> headers;

    endPos = allmsg.find("\r\n\r\n");
    std::string sSize1("HeaderSize");
    char * chr = (char*) malloc(1024);
    ocall_int_to_string((endPos + 4), chr);
    std::string sSize2(chr);
    //headers[copystring(sSize1)] = copystring(sSize2);
    map_add(headers, sSize1, sSize2);


    pos = allmsg.find("\r\n");
    firstSpace = allmsg.find(" ");
    std::string sMethod1("Method");
    std::string sMethod2(msg, oldPos, firstSpace);
    //headers[copystring(sMethod1)] = copystring(sMethod2);
    map_add(headers, sMethod1, sMethod2);
    secondSpace = allmsg.find(" ", firstSpace + 1);
    std::string sPath1("Path");
    std::string sPath2(msg, firstSpace + 1, secondSpace - firstSpace - 1);
    //headers[copystring(sPath1)] = copystring(sPath2);
    map_add(headers, sPath1, sPath2);

    std::string sProto1("Protocol");
    std::string sProto2(msg, secondSpace + 1, pos - secondSpace - 1);
    //headers[copystring(sProto1)] = copystring(sProto2);
    map_add(headers, sProto1, sProto2);

    while (pos < endPos && pos > 0) {
        oldPos = pos;
        pos = allmsg.find("\r\n", pos + 1);
        posmiddle = allmsg.find(":", oldPos + 1);
        std::string s1(msg, oldPos + 2, posmiddle - oldPos - 2);
        std::string s2(msg, posmiddle + 2, pos - posmiddle - 2);
        //headers[copystring(s1)] = copystring(s2);
        map_add(headers, s1, s2);

    }
    free(chr);

    return headers;
}

void handleProxy(int csock, char * msg, int msgsize) {
    //char target[1024] = "msstream.net";
    int targetPort = 8023;

    char * answer;
    int client_sock;
    char * finalanswer;
    char answerFromClient[1028];
    int httpanswer;
    int testIsEnd = 0;
    int sizeAnswerFromClient = 0;
    int totalSizeAnswer = 0;
    struct map* headersAnswer;

    struct map* headersRequest;
    msg[msgsize]='\0';
    headersRequest = parse_headers(msg);
    char * target = map_get(headersRequest,"X-Forwarded-Host");

    ocall_startClient(&client_sock, target);
    answer = createNewHeader(msg, target, msgsize);

    ocall_sendToClient(client_sock, answer, (int)strlen(answer), answerFromClient);
    sizeAnswerFromClient = extractSize(answerFromClient);
    finalanswer = extractBuffer(answerFromClient, sizeAnswerFromClient);


    httpanswer = isHttp(finalanswer);
    if(httpanswer == 0) {
        headersAnswer = parse_headers(finalanswer);

        if (map_find(headersAnswer, "Content-Length") > 0 || map_find(headersAnswer, "content-length") > 0) {
            std::string contentLength;
            if (map_find(headersAnswer, "Content-Length")>0) {
                contentLength = "Content-Length";
            } else {
                contentLength = "content-length";
            }
            int out;
            ocall_string_to_int(map_get(headersAnswer,"HeaderSize"),(int)strlen(map_get(headersAnswer,"HeaderSize")), &out);
            totalSizeAnswer += sizeAnswerFromClient - out;
            ocall_string_to_int(map_get(headersAnswer, contentLength),(int)strlen(map_get(headersAnswer,contentLength)), &out);
            while (testContentLength(out, totalSizeAnswer) != 0 && sizeAnswerFromClient != 0) {
                ocall_sendanswer(csock, finalanswer, sizeAnswerFromClient);
                free(finalanswer);
                ocall_receiveFromClient(client_sock, answerFromClient);
                sizeAnswerFromClient = extractSize(answerFromClient);
                finalanswer = extractBuffer(answerFromClient, sizeAnswerFromClient);
                totalSizeAnswer += sizeAnswerFromClient;
            }
            ocall_sendanswer(csock, finalanswer, sizeAnswerFromClient);
        } else if (map_find(headersAnswer, "Transfer-Encoding")>0) {
            //TODO Transfer-Encoding: chunked then look for the 0\r\n\r\n at the end of every packet. When found, close the socket
            //TODO Other idea: add a "Connection: close" header, so the connexion will be closed by the server
            while (testEndTransferEncoding(finalanswer, sizeAnswerFromClient) != 0 && sizeAnswerFromClient != 0) {
                ocall_sendanswer(csock, finalanswer, sizeAnswerFromClient);
                free(finalanswer);
                ocall_receiveFromClient(client_sock, answerFromClient);
                sizeAnswerFromClient = extractSize(answerFromClient);
                finalanswer = extractBuffer(answerFromClient, sizeAnswerFromClient);
            }
            ocall_sendanswer(csock, finalanswer, sizeAnswerFromClient);

        } else {
            ocall_sendanswer(csock, finalanswer, sizeAnswerFromClient);
        }
    } else {
        ocall_sendanswer(csock, finalanswer, sizeAnswerFromClient);
    }

    ocall_closesocket(client_sock);
    free(answer);
    free(finalanswer);
    map_destroy(headersRequest);
    map_destroy(headersAnswer);
}

void handleTracker(int csock, char * msg, int size) {
    std::string answer = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\nContent-Type: text/plain\r\nConnection: Close\r\n\r\n";
    char * finalanswer;
    struct map* headersRequest = parse_headers(msg);
    std::string value = map_get(headersRequest, "Method");
    std::string content;

    if (value == "POST") {
        sgx_thread_mutex_lock(&mutex);
        content = "POST received";
        finalanswer = addContentToAnswer(answer, content);
        sgx_thread_mutex_unlock(&mutex);
    } else if (value == "GET") {
        sgx_thread_mutex_lock(&mutex);
        content = "GET received";
        finalanswer = addContentToAnswer(answer, content);
        sgx_thread_mutex_unlock(&mutex);
    } else if (value == "DELETE") {
        sgx_thread_mutex_lock(&mutex);
        content = "DELETE received";
        finalanswer = addContentToAnswer(answer, content);
        sgx_thread_mutex_unlock(&mutex);
    } else {
        content = "";
        finalanswer = addContentToAnswer(answer, content);
    }

    ocall_sendanswer(csock, finalanswer, strlen(finalanswer));
    free(finalanswer);
    map_destroy(headersRequest);
}

void handleOption(int csock) {
    char answer[1024] = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Methods: GET, POST, DELETE, OPTIONS\r\nAccess-Control-Allow-Headers: Origin, Content-Type, Accept, x-forwarded-host\r\nConnection: Closed\r\n\r\n\0";
    //char test[1024] = "GET / HTTP/1.1\r\nHost: lacaud.fr\r\nUser-Agent: curl/7.55.1\r\nConnection: close\r\nAccept: */*\r\n\r\n";

    ocall_sendanswer(csock, answer, strlen(answer));
}

void ecall_handlemessage(int csock, int type, char * msg, int size){
    int http = isHttp(msg);
    if (http == 0) {
        if (type == 0) {
            handleProxy(csock, msg, size);
        }
        if (type == 1) {
            handleTracker(csock, msg, size);
        }
    } else {

        int option = isOption(msg);
        if (option == 0) {
            handleOption(csock);
        }
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
