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

struct map_element {
    char* key;
    char* value;
    struct map* inmap;
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

struct map* trackermap;

struct map* proxytable;

void ecall_init(int type) {
    sgx_thread_mutex_init(&mutex, NULL);
    if (type == 1) {
        trackermap = map_init();
    }
    if (type == 0) {
        proxytable = map_init();
    }
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



void map_add(struct map* map, std::string key, std::string value) {
    struct map_element* elt = (struct map_element*)malloc(sizeof(struct map_element));

    if (map->size == 0) {
        map->first = elt;
    }
    elt->key = copystring2char(key);
    elt->value = copystring2char(value);
    elt->inmap = NULL;
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

void map_replace(struct map* map, std::string key, std::string newvalue) {
    char * key2 = copystring2char(key);
    struct map_element * current = map->first;
    if (current != NULL && strcmp(current->key,key2) == 0){
        free(current->value);
        current->value = copystring2char(newvalue);
    }

    while(current != NULL && current->key != NULL){
        if(strcmp(current->key,key2) == 0){
            free(current->value);
            current->value = copystring2char(newvalue);
        }
        current=map_get_next(current);
    }
    free(key2);
}

void map_destroy(struct map* map) {
    struct map_element * current = map->first;
    struct map_element * current_old;
    while(current != NULL) {
        current_old=current;
        current = map_get_next(current_old);
        free(current_old->key);
        free(current_old->value);
        if (current_old->inmap != NULL) {
            map_destroy(current_old->inmap);
        }
        free(current_old);
    }
    free(map);
}

int map_find(struct map* map, std::string key) {

    char * key2 = copystring2char(key);
    struct map_element * current = map->first;
    if (current != NULL && strcmp(current->key,key2) == 0){
        free(key2);
        return 1;
    }

    while(current != NULL) {

        if (current->key != NULL) {
            if (strcmp(current->key, key2) == 0) {
                free(key2);
                return 1;
            }
        }

        current = map_get_next(current);

    }
    free(key2);
    return 0;
}

char * map_get(struct map* map, std::string key) {
    char * key2 = copystring2char(key);
    struct map_element * current = map->first;
    if (current != NULL && strcmp(current->key,key2) == 0){
        free(key2);
        return current->value;
    }

    while(current != NULL && current->key != NULL){
        if(strcmp(current->key,key2) == 0){
            free(key2);
            return current->value;
        }
        current=map_get_next(current);
    }
    free(key2);
    return NULL;
}

struct map * map_get_map(struct map* map, std::string key) {
    char * key2 = copystring2char(key);
    struct map_element * current = map->first;
    if (current != NULL && strcmp(current->key,key2) == 0){
        free(key2);
        return current->inmap;
    }

    while(current != NULL && current->key != NULL){
        if(strcmp(current->key,key2) == 0){
            free(key2);
            return current->inmap;
        }
        current=map_get_next(current);
    }
    free(key2);
    return NULL;
}

struct map_element * map_get_elem(struct map* map, std::string key) {
    char * key2 = copystring2char(key);
    struct map_element * current = map->first;
    if (current != NULL && strcmp(current->key,key2) == 0){
        free(key2);
        return current;
    }

    while(current != NULL && current->key != NULL){
        if(strcmp(current->key,key2) == 0){
            free(key2);
            return current;
        }
        current=map_get_next(current);
    }
    free(key2);
    return NULL;
}

int isBiggerThan(std::string value, char* valueInMem) {
    char * value2 = (char*)value.c_str();
    int sizeValue = strlen(value2);
    int sizeValueInMem = strlen(valueInMem);
    int returnvalue = 1;

    if (sizeValue > sizeValueInMem) {
        returnvalue = 0;
    } else if (sizeValue == sizeValueInMem) {
        for (int i = 0; i < sizeValueInMem; i++) {
            if (value2[i] > valueInMem[i]) {
                returnvalue = 0;
                break;
            }
            if (value2[i] < valueInMem[i]) {
                break;
            }
        }
    }

    return returnvalue;
}

std::string map_findKeysByValueBiggerThan(struct map* map, std::string value) {
    std:: string output = "";
    struct map_element * current = map->first;

    while(current != NULL && current->key != NULL){

        if (isBiggerThan(value, current->value) > 0) {
            std::string topush(current->key);
            if (output.length() > 0 && output[output.length()-1] != ',') {
                output += ",";
            }
            output += topush;
        }
        current=map_get_next(current);
    }

    return output;
}

// assume that there exist a 128 bits symmetric key priorly provisioned to the enclaves
char* sgx_provisioned_key = (char*)"1234567890123456";

void set_ctr_bytes(uint32_t val, uint8_t *ctr, size_t ctr_size)
{
    // within our simulation counters do not exceed 2^32 values, meaning that they can
    // be represented on 4 bytes.
    ctr[ctr_size - 4] = (val & 0xff000000) >> 24;
    ctr[ctr_size - 3] = (val & 0x00ff0000) >> 16;
    ctr[ctr_size - 2] = (val & 0x0000ff00) >>  8;
    ctr[ctr_size - 1] = (val & 0x000000ff);
}

sgx_status_t decryptMessage(char* in, size_t in_size, char* out, uint32_t counter)
{
    uint8_t ctr_bytes[16] = {0};
    set_ctr_bytes(counter, ctr_bytes, 16);
    return sgx_aes_ctr_decrypt((sgx_aes_ctr_128bit_key_t*) sgx_provisioned_key,
        (uint8_t*) in, in_size, ctr_bytes, 128,
        (uint8_t*) out);
}

sgx_status_t encryptMessage(char* in, size_t in_size, char* out, uint32_t counter)
{
    uint8_t ctr_bytes[16] = {0};
    set_ctr_bytes(counter, ctr_bytes, 16);
    return sgx_aes_ctr_encrypt(
        (sgx_aes_ctr_128bit_key_t*) sgx_provisioned_key,
        (uint8_t*) in, in_size, ctr_bytes, 128,
        (uint8_t*) out);
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
    int posForward;
    int posEndForward;
    if (posConnection != -1) {
        posConnection = posConnection + 12;
        int posEndConnection = header.find("\r\n", posConnection);
        header.replace(posConnection, posEndConnection-posConnection, "Close");
    } else {
        header.insert(posEnd + 3, "Connection: Close\r\n");
    }

    header.replace(posHost, posEnd-posHost, address);

    posForward = header.find("X-Forwarded-Host: ") + 18;
    posEndForward = header.find("\r\n", posForward);
    header.replace(posForward, posEndForward-posForward, "localhost:8080");

    /*posHost = header.find("Host: ") + 6;
    posEnd = header.find("\r\n", posHost);
    header.insert(posEnd + 3, "Encrypt: encrypt\r\n");*/

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

int cutInto16BytesMultiple(char * bufferIn, char * bufferOut, int totalSize) {
	int remainingSize;
	if (totalSize % 16 == 0) {
		remainingSize = 0;
	} else {
		remainingSize = (totalSize % 16);
		strncpy(bufferOut, bufferIn + (16 * (totalSize/16)), totalSize - remainingSize);
	}
	return remainingSize;
}

void handleProxy(int csock, char * msg, int msgsize) {
    //char target[1024] = "msstream.net";
    int targetPort = 8023;

	bool fromSGX = true;
	
	uint32_t counter = 0;
	int remainingSize = 0;
	char * remainingBuffer = (char *) malloc(16 * sizeof(char));
	
    char * answer;
    int client_sock;
    char * finalanswer;
    char answerFromClient[1028];
    int httpanswer;
    int testIsEnd = 0;
    int sizeAnswerFromClient = 0;
    int totalSizeAnswer = 0;
    char * target;
    struct map* headersAnswer = NULL;
    int return_send = 0;

    struct map* headersRequest;
    msg[msgsize] = '\0';
    headersRequest = parse_headers(msg);
    target = map_get(headersRequest, "X-Forwarded-Host");

    ocall_startClient(&client_sock, target);
    answer = createNewHeader(msg, target, msgsize);
    
	int endPos = getPosEndOfHeader(msg)+4;
	int msgSizeCnt = msgsize-endPos;
	if (endPos < msgsize) {
		char * fullDecryptedMessage = (char*) malloc(msgsize*sizeof(char));
		memset(fullDecryptedMessage, 0, msgsize*sizeof(char));
		char * decryptedMessage = (char*) malloc((msgSizeCnt+1)*sizeof(char));
		memset(decryptedMessage, 0, (msgSizeCnt+1)*sizeof(char));
		strncpy(fullDecryptedMessage, msg, endPos);
		char * messageToDecrypt = (char*) malloc((msgSizeCnt+1)*sizeof(char));
		memset(messageToDecrypt, 0, (msgSizeCnt+1)*sizeof(char));
		strncpy(messageToDecrypt, msg+endPos, msgSizeCnt);		
		messageToDecrypt[msgSizeCnt] = '\0';		
		if (fromSGX) {
			decryptMessage(messageToDecrypt, msgSizeCnt, decryptedMessage, 0);
		} else {
			encryptMessage(messageToDecrypt, msgSizeCnt, decryptedMessage, 0);
		}
		decryptedMessage[msgSizeCnt] = '\0';
		strncpy(fullDecryptedMessage+endPos, decryptedMessage, msgSizeCnt);
		strncpy(msg, fullDecryptedMessage, msgsize);
		free(messageToDecrypt);
		free(decryptedMessage);
		free(fullDecryptedMessage);
	}

    ocall_sendToClient(client_sock, answer, (int) strlen(answer), answerFromClient); 
    sizeAnswerFromClient = extractSize(answerFromClient);
    finalanswer = extractBuffer(answerFromClient, sizeAnswerFromClient); // finalanswer -> first (last?) subpacket
    
    endPos = getPosEndOfHeader(finalanswer)+4;
	msgSizeCnt = sizeAnswerFromClient-endPos;
	char * fullDecryptedMessage = (char*) malloc(sizeAnswerFromClient*sizeof(char));
	memset(fullDecryptedMessage, 0, sizeAnswerFromClient*sizeof(char));
	char * decryptedMessage = (char*) malloc((msgSizeCnt+1)*sizeof(char));
	memset(decryptedMessage, 0, (msgSizeCnt+1)*sizeof(char));
	strncpy(fullDecryptedMessage, finalanswer, endPos);
	char * messageToDecrypt = (char*) malloc((msgSizeCnt+1)*sizeof(char));
	memset(messageToDecrypt, 0, (msgSizeCnt+1)*sizeof(char));
	strncpy(messageToDecrypt, finalanswer+endPos, msgSizeCnt);
	messageToDecrypt[msgSizeCnt] = '\0';		
	if (fromSGX) {
		encryptMessage(messageToDecrypt, msgSizeCnt, decryptedMessage, counter);
	} else {
		decryptMessage(messageToDecrypt, msgSizeCnt, decryptedMessage, counter);
	}
	counter = msgSizeCnt / 16;
	decryptedMessage[msgSizeCnt] = '\0';
	strncpy(fullDecryptedMessage+endPos, decryptedMessage, msgSizeCnt);
	strncpy(finalanswer, fullDecryptedMessage, sizeAnswerFromClient);
	memset(remainingBuffer, 0, 16);
	remainingSize = cutInto16BytesMultiple(messageToDecrypt, remainingBuffer, msgSizeCnt);
	free(messageToDecrypt);
	free(decryptedMessage);
	free(fullDecryptedMessage);
    
    // finalanswer -> if fromSGX: encrypt / else: decrypt

    if (sizeAnswerFromClient > 0) {
        httpanswer = isHttp(finalanswer);
        if (httpanswer == 0) {
            headersAnswer = parse_headers(finalanswer);

            if (map_find(headersAnswer, "Content-Length") > 0 || map_find(headersAnswer, "content-length") > 0) {
                std::string contentLength;
                if (map_find(headersAnswer, "Content-Length") > 0) {
                    contentLength = "Content-Length";
                } else {
                    contentLength = "content-length";
                }
                int out;
                ocall_string_to_int(map_get(headersAnswer, "HeaderSize"),
                                    (int) strlen(map_get(headersAnswer, "HeaderSize")), &out);
                totalSizeAnswer += sizeAnswerFromClient - out;
                ocall_string_to_int(map_get(headersAnswer, contentLength),
                                    (int) strlen(map_get(headersAnswer, contentLength)), &out);

                while (testContentLength(out, totalSizeAnswer) != 0 && sizeAnswerFromClient != 0) {
                    ocall_sendanswer(&return_send, csock, finalanswer, sizeAnswerFromClient);
                    free(finalanswer);
                    ocall_receiveFromClient(client_sock, answerFromClient);
                    sizeAnswerFromClient = extractSize(answerFromClient);
                    finalanswer = extractBuffer(answerFromClient, sizeAnswerFromClient);
					
					endPos = getPosEndOfHeader(finalanswer)+4;
					msgSizeCnt = sizeAnswerFromClient-endPos+remainingSize;
					char * fullDecryptedMessage = (char*) malloc(sizeAnswerFromClient*sizeof(char));
					memset(fullDecryptedMessage, 0, sizeAnswerFromClient*sizeof(char));
					char * decryptedMessage = (char*) malloc((msgSizeCnt+1)*sizeof(char));
					memset(decryptedMessage, 0, (msgSizeCnt+1)*sizeof(char));
					strncpy(fullDecryptedMessage, finalanswer, endPos);
					char * messageToDecrypt = (char*) malloc((msgSizeCnt+1)*sizeof(char));
					memset(messageToDecrypt, 0, (msgSizeCnt+1)*sizeof(char));
					strncpy(messageToDecrypt, remainingBuffer, remainingSize);
					strncpy(messageToDecrypt+remainingSize, finalanswer+endPos, msgSizeCnt-remainingSize);
					messageToDecrypt[msgSizeCnt] = '\0';
					if (fromSGX) {
						encryptMessage(messageToDecrypt, msgSizeCnt, decryptedMessage, counter);
					} else {
						decryptMessage(messageToDecrypt, msgSizeCnt, decryptedMessage, counter);
					}
					counter += msgSizeCnt / 16;
					decryptedMessage[msgSizeCnt] = '\0';
					strncpy(fullDecryptedMessage+endPos+remainingSize, decryptedMessage, msgSizeCnt-remainingSize);
					strncpy(finalanswer, fullDecryptedMessage, sizeAnswerFromClient);
					memset(remainingBuffer, 0, 16);
					remainingSize = cutInto16BytesMultiple(messageToDecrypt, remainingBuffer, msgSizeCnt);
					free(messageToDecrypt);
					free(decryptedMessage);
					free(fullDecryptedMessage);
                    
                    totalSizeAnswer += sizeAnswerFromClient;
                }
                ocall_sendanswer(&return_send, csock, finalanswer, sizeAnswerFromClient);

            } else if (map_find(headersAnswer, "Transfer-Encoding") > 0) {
                //TODO Transfer-Encoding: chunked then look for the 0\r\n\r\n at the end of every packet. When found, close the socket
                //TODO Other idea: add a "Connection: close" header, so the connexion will be closed by the server

                while (testEndTransferEncoding(finalanswer, sizeAnswerFromClient) != 0 && sizeAnswerFromClient != 0) {
                    ocall_sendanswer(&return_send, csock, finalanswer, sizeAnswerFromClient);
                    if (return_send == 0) {
                        break;
                    }
                    free(finalanswer);
                    ocall_receiveFromClient(client_sock, answerFromClient);
                    sizeAnswerFromClient = extractSize(answerFromClient);
                    finalanswer = extractBuffer(answerFromClient, sizeAnswerFromClient);
                    
                    endPos = getPosEndOfHeader(finalanswer)+4;
					msgSizeCnt = sizeAnswerFromClient-endPos+remainingSize;
					char * fullDecryptedMessage = (char*) malloc(sizeAnswerFromClient*sizeof(char));
					memset(fullDecryptedMessage, 0, sizeAnswerFromClient*sizeof(char));
					char * decryptedMessage = (char*) malloc((msgSizeCnt+1)*sizeof(char));
					memset(decryptedMessage, 0, (msgSizeCnt+1)*sizeof(char));
					strncpy(fullDecryptedMessage, finalanswer, endPos);
					char * messageToDecrypt = (char*) malloc((msgSizeCnt+1)*sizeof(char));
					memset(messageToDecrypt, 0, (msgSizeCnt+1)*sizeof(char));
					strncpy(messageToDecrypt, remainingBuffer, remainingSize);
					strncpy(messageToDecrypt+remainingSize, finalanswer+endPos, msgSizeCnt-remainingSize);
					messageToDecrypt[msgSizeCnt] = '\0';
					if (fromSGX) {
						encryptMessage(messageToDecrypt, msgSizeCnt, decryptedMessage, counter);
					} else {
						decryptMessage(messageToDecrypt, msgSizeCnt, decryptedMessage, counter);
					}
					counter += msgSizeCnt / 16;
					decryptedMessage[msgSizeCnt] = '\0';
					strncpy(fullDecryptedMessage+endPos+remainingSize, decryptedMessage, msgSizeCnt-remainingSize);
					strncpy(finalanswer, fullDecryptedMessage, sizeAnswerFromClient);
					memset(remainingBuffer, 0, 16);
					remainingSize = cutInto16BytesMultiple(messageToDecrypt, remainingBuffer, msgSizeCnt);
					free(messageToDecrypt);
					free(decryptedMessage);
					free(fullDecryptedMessage);
                }
                ocall_sendanswer(&return_send, csock, finalanswer, sizeAnswerFromClient);

            } else {
                ocall_sendanswer(&return_send, csock, finalanswer, sizeAnswerFromClient);
            }
        } else {
            ocall_sendanswer(&return_send, csock, finalanswer, sizeAnswerFromClient);
        }
    }

    ocall_closesocket(client_sock);
    free(answer);
    free(finalanswer);
    map_destroy(headersRequest);
    if (headersAnswer != NULL) {
        map_destroy(headersAnswer);
    }
}

void handleTracker(int csock, char * msg, int size, int debug) {

	// Decryption: msg -> fullDecryptedMessage
	uint32_t counter = 0;
	
	int endPos = getPosEndOfHeader(msg)+4;
	int msgSize = size-endPos;
	char * fullDecryptedMessage = (char*) malloc(size*sizeof(char));
	memset(fullDecryptedMessage, 0, size*sizeof(char));
	char * decryptedMessage = (char*) malloc((msgSize+1)*sizeof(char));
	memset(decryptedMessage, 0, (msgSize+1)*sizeof(char));
	strncpy(fullDecryptedMessage, msg, endPos);
	if (endPos < size) {
		char * messageToDecrypt = (char*) malloc((msgSize+1)*sizeof(char));
		memset(messageToDecrypt, 0, (msgSize+1)*sizeof(char));
		strncpy(messageToDecrypt, msg+endPos, msgSize);
		messageToDecrypt[msgSize] = '\0';		
		if (debug == 0) {
			decryptMessage(messageToDecrypt, msgSize, decryptedMessage, counter);
		} else if (debug == 1) {
			encryptMessage(messageToDecrypt, msgSize, decryptedMessage, counter);
		}
		counter = msgSize / 16;
		decryptedMessage[msgSize] = '\0';
		strncpy(fullDecryptedMessage+endPos, decryptedMessage, msgSize);
		free(messageToDecrypt);
	}
	// fullDecryptedMessage
	
	std::string answer = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\nContent-Type: text/plain\r\nConnection: Close\r\n\r\n";

    char * finalanswer;
    struct map* headersRequest = parse_headers(fullDecryptedMessage);
    std::string value = map_get(headersRequest, "Method");
    std::string content;
    int return_send = 0;
    struct map* ipmap;
    struct map_element * current;
    
    if (debug == 0) {
		if (value == "POST") {
		    sgx_thread_mutex_lock(&mutex);
		    content = "";
            std::string msgContent(decryptedMessage);
            int firstComa = msgContent.find(",");
            int secondComa = msgContent.find(",", firstComa + 1);
            std::string  videoID = msgContent.substr(0, firstComa);
            std::string  ipToChange = msgContent.substr(firstComa+1, secondComa-firstComa-1);
            std::string  numberOfSegment = msgContent.substr(secondComa+1);

		    if (map_find(trackermap, videoID) == 0) {
		        map_add(trackermap, videoID, "");
		        ipmap = map_get_map(trackermap, videoID);
		        if (ipmap == NULL) {
		            ipmap = map_init();
		        }
		        map_add(ipmap, ipToChange, numberOfSegment);
		        current = map_get_elem(trackermap, videoID);
		        current->inmap = ipmap;
		    } else {
		        ipmap = map_get_map(trackermap, videoID);
                if (map_find(ipmap, ipToChange) == 0) {
                    map_add(ipmap, ipToChange, numberOfSegment);
                } else {                    map_replace(ipmap, ipToChange, numberOfSegment);
                }
		    }
		    finalanswer = addContentToAnswer(answer, content);
		    sgx_thread_mutex_unlock(&mutex);
		} else if (value == "GET") {
		    sgx_thread_mutex_lock(&mutex);
            std::string msgContent(decryptedMessage);
            int firstComa = msgContent.find(",");
            std::string  videoID = msgContent.substr(0, firstComa);
            std::string  numberOfSegments = msgContent.substr(firstComa+1);
		    if (map_find(trackermap, videoID) > 0) {
		        ipmap = map_get_map(trackermap, videoID);
                if (ipmap != NULL) {
                    std::string tosend = map_findKeysByValueBiggerThan(ipmap, numberOfSegments);
                    finalanswer = addContentToAnswer(answer, tosend);
                } else {
                    std::string tosend = "";
                    finalanswer = addContentToAnswer(answer, tosend);
                }

		    } else {
		        std::string tosend = "";
		        finalanswer = addContentToAnswer(answer, tosend);
		    }
		    sgx_thread_mutex_unlock(&mutex);
		} else if (value == "DELETE") {
		    sgx_thread_mutex_lock(&mutex);
		    content = "DELETE received";
		    finalanswer = addContentToAnswer(answer, content);
		    map_destroy(trackermap);
		    trackermap = map_init();
		    sgx_thread_mutex_unlock(&mutex);
		} else {
		    content = "";
		    finalanswer = addContentToAnswer(answer, content);
		}
	} else if (debug == 1) {
		std::string content(decryptedMessage);
		finalanswer = addContentToAnswer(answer, content);
	}
    
    // Encryption: answer -> fullEncryptedMessage
    counter = 0;
    endPos = getPosEndOfHeader(finalanswer)+4;
    msgSize = strlen(finalanswer) - endPos;
	char * fullEncryptedMessage = (char*) malloc((answer.length()+msgSize)*sizeof(char));
	memset(fullEncryptedMessage, 0, (answer.length()+msgSize)*sizeof(char));
	strncpy(fullEncryptedMessage, finalanswer, endPos);
	if (endPos < strlen(finalanswer)) {
		char * messageToEncrypt = (char*) malloc((msgSize+1)*sizeof(char));
		memset(messageToEncrypt, 0, (msgSize+1)*sizeof(char));
		char * encryptedMessage = (char*) malloc((msgSize+1)*sizeof(char));
		memset(encryptedMessage, 0, (msgSize+1)*sizeof(char));
		strncpy(messageToEncrypt, finalanswer+endPos, msgSize);
		messageToEncrypt[msgSize] = '\0';
		if (debug == 0) {
			encryptMessage(messageToEncrypt, msgSize, encryptedMessage, counter);
		} else if (debug == 1) {
			decryptMessage(messageToEncrypt, msgSize, encryptedMessage, counter);
		}
		counter = msgSize / 16;
		encryptedMessage[msgSize] = '\0';
		strncpy(fullEncryptedMessage+endPos, encryptedMessage, msgSize);
		free(messageToEncrypt);
		free(encryptedMessage);
	}
	// fullEncryptedMessage
	
    ocall_sendanswer(&return_send, csock, fullEncryptedMessage, strlen(fullEncryptedMessage));
    free(finalanswer);
    free(fullDecryptedMessage);
    free(fullEncryptedMessage);
    free(decryptedMessage);
    map_destroy(headersRequest);
}

void handleOption(int csock) {
    char answer[1024] = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Methods: GET, POST, DELETE, OPTIONS\r\nAccess-Control-Allow-Headers: Origin, Content-Type, Accept, x-forwarded-host\r\nConnection: Closed\r\n\r\n\0";
    //char test[1024] = "GET / HTTP/1.1\r\nHost: lacaud.fr\r\nUser-Agent: curl/7.55.1\r\nConnection: close\r\nAccept: */*\r\n\r\n";
    int return_send = 0;

    ocall_sendanswer(&return_send, csock, answer, strlen(answer));
}

void ecall_handlemessage(int csock, int type, char * msg, int size){
    int http = isHttp(msg);
    if (http == 0) {
        if (type == 0) {
            handleProxy(csock, msg, size);
        }
        if (type == 1) {
            handleTracker(csock, msg, size, 0);
        }
		if (type == 11) {
			// tracker encryption test
			handleTracker(csock, msg, size, 1);
        }
    } else {

        int option = isOption(msg);
        if (option == 0) {
            handleOption(csock);
        }
    }
}

