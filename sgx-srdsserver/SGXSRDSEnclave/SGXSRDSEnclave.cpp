#include "SGXSRDSEnclave_t.h"

#include "sgx_trts.h"
#include "sgx_tcrypto.h"
#include "string.h"
#include <string>
#include "stdlib.h"
#include <map>
#include "sgx_thread.h"

sgx_thread_mutex_t mutex;

bool encrypt_IPs = true;
bool encrypt = true;
bool enable_TE_encryption = true;

const std::string proxyPort("8081");
const std::string proxyAddr = "localhost:" + proxyPort;
const std::string trackerAddr("192.168.1.120:8888");
const std::string serverAddr("localhost:8080");
const std::string mpdAddr("192.168.1.109:8081");
const std::string mpdRes = "/api/mpd/srds";

int numberOfTokens = 4;

int extractSize(char * msg) {
	int size = ((unsigned char)msg[0] << 24) + ((unsigned char)msg [1] << 16) + ((unsigned char)msg[2] << 8) + (unsigned char)msg[3];
	return size;
}

void extractBuffer(char * msg, int size, char * bufferOut) {
	int sizeIntinChar = 4;
	memset(bufferOut, 0, size);

	for(int i = 0; i< size; i++) {
		bufferOut[i] = msg [i + sizeIntinChar];
	}
}

namespace blockchain_values {

	// BODY must be: application/x-www-form-urlencoded
	
	// read balance: 'GET http://' + server_addr + '/api/getBalance/' + public_base64_key
	
	// assign coins: 'POST http://' + server_addr + '/api/assignCoin'
	// -> Parameters: "key" : public_base64_key ; "amount" : amount
	
	// spend coins 'POST http://' + server_addr + '/api/spendCoin'
	// -> Parameters: "key_from" : public_base64_key ; "key_to" : public_base64_key_dst ; "amount" : amount ; "signature" : hex_sign 
	
	const char * server_addr = "174.138.12.115:8124";
	const char * hex_sign = "207bedf94ad5378a8a483538296409bcf273ef764679758e2354a56efd7433ba9ed137f1077c65233054a1d79e3fc829308c8405a1841f0d3da079bd7a75b85fe93f42aecd984513e6c003695ca94d7d3bffaf32ad6a8ca7c5b299a13c07a2e5c2093c0787eaa9057016b0b0e27717cad2859fd4319659713e90b64d4d8dbba28ae125a1eded5d2584945423382f802debcbdc5206b3f1d38147550b03b5c5c72211a1c997fe89821a8a4c06d61efc41ccd3a25991e6800804d8d2fa87ab69ef9c96a1f0c8341e8fc63ea3b186702cc48d670ca4f65ce7f36d6d03983e4dd436c68ab46b67b6f5567e786ec17e09b59918c71bd86aaf68a3dd487165a1e65d18b6f55c7cba55e6fc56bbbee812bcfa71ac555ca83f472a106683bc51a76a018554bd7bf515c5584645e6b3b31a9255532afce7e09fa82e8b2c8d69f0763548adba33812ba490bf8bef3bf4d122e96f6be23d57b68fd6797154b92098059c6d6f5421ef33c9e5ea745bef4fb3ec40b6d5f08175ab71e82673044cda5418483e9cd61124caecfb245ebf249b86cfba1b66ba6ad914bc5538226a2ac5d85c52154bacd79fb07425e8b819c749fa47b9c8b3d40256cd0c985d5ff4a191129839a9d62131c95b4309e275a26e13955c70ef85680e7e361aa8850c34d670035b93185763ccadff7f80286d69b73ecb0002ec0dd55789499b12017fb341f642d50a8b3f";
	const char * private_key = "-----BEGIN RSA PRIVATE KEY-----\n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\n-----END RSA PRIVATE KEY-----";
	const char * private8_key = "-----BEGIN PRIVATE KEY-----\n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\nT259sEiUQ+DsJbrTJeIZZsSEly3R\n-----END PRIVATE KEY-----\n"; 
	const char public_key[] = "-----BEGIN PUBLIC KEY-----\nMIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEApGMpRdjgraVusaxfHYEa\nnpRfMQSd6xi2H2F224BoR8RsxQEnvbTJX+kFMEVjJkV+TfcoOhKyT0v3MMSMVdLM\njil6eDrc+H0xnwKErb2Vvu3yazGuKdWEWF8aq3yhUstyqVdtWlSIAVpbARIHbueC\nM6gS1qTZVOxEo0SHtsbBHghAo24hOwXAfczslttIFvcKg0qyP8nmCaAZYPSBFUfE\nwZdxWLOjh48lmrtZtGFWWjO4QKLfg6UDNGg2FdFfnJamx9B5BHrHJdQIsStCFlZW\nkJ6bEv498qZByf1SH4aXkC/UXTTDV5i47PTr17+QO7YIh6SJni8xSzly8FEASBKc\neRfeiWNdMDEFrjfyHhr0WUMjzT+ZuVFNNMu72owSLbfkpLwtMieB5Tr07zG3gkDv\nDEmubmBHvRy4sGklOW4arejS1w1ZHyo3FfDFEO4dfKXLF/w8OXwH/wJk5ytt1Fz6\n5J5iSDd553UJSxaJIb79610qBPLhjGtyqXIWlesRjOgkOk2si2LkCjPxG7XwklyU\nxmb8XV9DSvrDqJ+3QaD/BweD5oIlhTbjzICd8t+uFjL9f4NvWKehjZCsiM3WMbau\nRdfbremTGKAgFwEd2VAJMMjD1tQsxrJQyvruVbzdRF8c3jpKit2GlsBlbtEUYUZ4\niEOyvw1YmSFb+MIj4YMjsRcCAwEAAQ==\n-----END PUBLIC KEY-----\n";
	const char public_base64_key[] = "LS0tLS1CRUdJTiBQVUJMSUMgS0VZLS0tLS0NCk1JSUNJakFOQmdrcWhraUc5dzBCQVFFRkFBT0NBZzhBTUlJQ0NnS0NBZ0VBcEdNcFJkamdyYVZ1c2F4ZkhZRWENCm5wUmZNUVNkNnhpMkgyRjIyNEJvUjhSc3hRRW52YlRKWCtrRk1FVmpKa1YrVGZjb09oS3lUMHYzTU1TTVZkTE0NCmppbDZlRHJjK0gweG53S0VyYjJWdnUzeWF6R3VLZFdFV0Y4YXEzeWhVc3R5cVZkdFdsU0lBVnBiQVJJSGJ1ZUMNCk02Z1MxcVRaVk94RW8wU0h0c2JCSGdoQW8yNGhPd1hBZmN6c2x0dElGdmNLZzBxeVA4bm1DYUFaWVBTQkZVZkUNCndaZHhXTE9qaDQ4bG1ydFp0R0ZXV2pPNFFLTGZnNlVETkdnMkZkRmZuSmFteDlCNUJIckhKZFFJc1N0Q0ZsWlcNCmtKNmJFdjQ5OHFaQnlmMVNINGFYa0MvVVhUVERWNWk0N1BUcjE3K1FPN1lJaDZTSm5pOHhTemx5OEZFQVNCS2MNCmVSZmVpV05kTURFRnJqZnlIaHIwV1VNanpUK1p1VkZOTk11NzJvd1NMYmZrcEx3dE1pZUI1VHIwN3pHM2drRHYNCkRFbXVibUJIdlJ5NHNHa2xPVzRhcmVqUzF3MVpIeW8zRmZERkVPNGRmS1hMRi93OE9Yd0gvd0prNXl0dDFGejYNCjVKNWlTRGQ1NTNVSlN4YUpJYjc5NjEwcUJQTGhqR3R5cVhJV2xlc1JqT2drT2syc2kyTGtDalB4RzdYd2tseVUNCnhtYjhYVjlEU3ZyRHFKKzNRYUQvQndlRDVvSWxoVGJqeklDZDh0K3VGakw5ZjROdldLZWhqWkNzaU0zV01iYXUNClJkZmJyZW1UR0tBZ0Z3RWQyVkFKTU1qRDF0UXN4ckpReXZydVZiemRSRjhjM2pwS2l0Mkdsc0JsYnRFVVlVWjQNCmlFT3l2dzFZbVNGYitNSWo0WU1qc1JjQ0F3RUFBUT09DQotLS0tLUVORCBQVUJMSUMgS0VZLS0tLS0=";

	int getBalance() {
		int socket;
		int balance = -1;
		char responseBuffer[1028];
		int sizeAnswerFromClient;
		int find = 0;
		int find2 = 0;
		std::string request = "GET /api/getBalance/" + std::string(blockchain_values::public_base64_key) + "\r\nAccept: */*\r\n\r\n";
		ocall_startClient(&socket, (char *) blockchain_values::server_addr);
		memset(responseBuffer, 0, 1028);
		ocall_sendToClient(socket, (char *) request.c_str(), request.size(), responseBuffer);
		sizeAnswerFromClient = extractSize(responseBuffer);
		char finalanswer[sizeAnswerFromClient];
		memset(finalanswer, 0, (sizeAnswerFromClient));
		extractBuffer(responseBuffer, sizeAnswerFromClient, finalanswer);
		std::string responseString(finalanswer);
		find = responseString.find("\"result\":\"") + 10;
		if (find > 10) {
			find2 = responseString.find('"', find + 1);
			if (find2 > find) {
				std::string balanceString = responseString.substr(find, find2);
				ocall_string_to_int(balanceString.c_str(), balanceString.size(), &balance);
			}
		}
		emit_debug("Tokens balance:");
		emit_debug_int(balance);
		ocall_closesocket(socket);
		return balance;
	}
	
	int assignCoin (int coinsToAssign) {
		int socket;
		int coinsAssigned = -1;
		char responseBuffer[1028];
		int sizeAnswerFromClient;
		int find = 0;
		char coinsToAssignStr[1024];
		memset(coinsToAssignStr, 0, 1024);
		ocall_int_to_string(coinsToAssign, coinsToAssignStr);
		std::string body = "key=\"" + std::string(blockchain_values::public_base64_key) + "\"&amount=\"" + std::string(coinsToAssignStr) + "\"";
		char bodySizeStr[1024];
		memset(bodySizeStr, 0, 1024);
		ocall_int_to_string(body.size(), bodySizeStr);
		std::string requestheader = "POST /api/assignCoin\r\nAccept: */*\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: " + std::string(bodySizeStr) + "\r\n\r\n";
		std::string request = requestheader + body;
		ocall_startClient(&socket, (char *) blockchain_values::server_addr);
		memset(responseBuffer, 0, 1028);
		ocall_sendToClient(socket, (char *) request.c_str(), request.size(), responseBuffer);
		sizeAnswerFromClient = extractSize(responseBuffer);
		char finalanswer[sizeAnswerFromClient];
		memset(finalanswer, 0, (sizeAnswerFromClient));
		extractBuffer(responseBuffer, sizeAnswerFromClient, finalanswer);
		std::string responseString(finalanswer);
		find = responseString.find("\"status\":\"") + 10;
		if (find > 10) {
			if (responseString.substr(find+1, find+2).compare("OK")) {
				coinsAssigned = coinsToAssign;
			}
		}
		emit_debug("Tokens assigned:");
		emit_debug_int(coinsAssigned);
		ocall_closesocket(socket);
		return coinsAssigned;
	}

}

// assume that there exist a 128 bits symmetric key priorly provisioned to the enclaves
char * sgx_provisioned_key = (char *)"1234567890123456";

void set_ctr_bytes(uint32_t val, uint8_t *ctr, size_t ctr_size)
{
	// within our simulation counters do not exceed 2^32 values, meaning that they can
	// be represented on 4 bytes.
	ctr[ctr_size - 4] = (val & 0xff000000) >> 24;
	ctr[ctr_size - 3] = (val & 0x00ff0000) >> 16;
	ctr[ctr_size - 2] = (val & 0x0000ff00) >>  8;
	ctr[ctr_size - 1] = (val & 0x000000ff);
}

sgx_status_t decryptMessage(uint8_t * in, size_t in_size, uint8_t * out, uint32_t counter)
{
	uint8_t ctr_bytes[16] = {0};
	set_ctr_bytes(counter, ctr_bytes, 16);
	return sgx_aes_ctr_decrypt((sgx_aes_ctr_128bit_key_t*) sgx_provisioned_key,
		(uint8_t*) in, in_size, ctr_bytes, 128,
		(uint8_t*) out);
}

sgx_status_t encryptMessage(uint8_t * in, size_t in_size, uint8_t * out, uint32_t counter)
{
	uint8_t ctr_bytes[16] = {0};
	set_ctr_bytes(counter, ctr_bytes, 16);
	return sgx_aes_ctr_encrypt(
		(sgx_aes_ctr_128bit_key_t*) sgx_provisioned_key,
		(uint8_t*) in, in_size, ctr_bytes, 128,
		(uint8_t*) out);
}

const char alpha32[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

int GetEncode32Length(int bytes)
{
   int bits = bytes * 8;
   int length = bits / 5;
   if((bits % 5) > 0)
   {
	  length++;
   }
   return length;
}

int GetDecode32Length(int bytes)
{
   int bits = bytes * 5;
   int length = bits / 8;
   return length;
}

static bool Encode32Block(unsigned char * in5, unsigned char * out8)
{
	  // pack 5 bytes
	  uint64_t buffer = 0;
	  for(int i = 0; i < 5; i++)
	  {
		  if(i != 0)
		  {
			  buffer = (buffer << 8);
		  }
		  buffer = buffer | in5[i];
	  }
	  // output 8 bytes
	  for(int j = 7; j >= 0; j--)
	  {
		  buffer = buffer << (24 + (7 - j) * 5);
		  buffer = buffer >> (24 + (7 - j) * 5);
		  unsigned char c = (unsigned char)(buffer >> (j * 5));
		  // self check
		  if(c >= 32) return false;
		  out8[7 - j] = c;
	  }
	  return true;
}

bool Encode32(unsigned char * in, int inLen, unsigned char * out)
{
   if((in == 0) || (inLen <= 0) || (out == 0)) return false;

   int d = inLen / 5;
   int r = inLen % 5;

   unsigned char outBuff[8];

   for(int j = 0; j < d; j++)
   {
	  if(!Encode32Block(&in[j * 5], &outBuff[0])) return false;
	  memmove(&out[j * 8], &outBuff[0], sizeof(unsigned char) * 8);
   }

   unsigned char padd[5];
   memset(padd, 0, sizeof(unsigned char) * 5);
   for(int i = 0; i < r; i++)
   {
	  padd[i] = in[inLen - r + i];
   }
   if(!Encode32Block(&padd[0], &outBuff[0])) return false;
   memmove(&out[d * 8], &outBuff[0], sizeof(unsigned char) * GetEncode32Length(r));

   return true;
}

static bool Decode32Block(unsigned char * in8, unsigned char * out5)
{
	  // pack 8 bytes
	  uint64_t buffer = 0;
	  for(int i = 0; i < 8; i++)
	  {
		  // input check
		  if(in8[i] >= 32) return false;
		  if(i != 0)
		  {
			  buffer = (buffer << 5);
		  }
		  buffer = buffer | in8[i];
	  }
	  // output 5 bytes
	  for(int j = 4; j >= 0; j--)
	  {
		  out5[4 - j] = (unsigned char)(buffer >> (j * 8));
	  }
	  return true;
}

bool Decode32(unsigned char * in, int inLen, unsigned char * out)
{
   if((in == 0) || (inLen <= 0) || (out == 0)) return false;

   int d = inLen / 8;
   int r = inLen % 8;

   unsigned char outBuff[5];

   for(int j = 0; j < d; j++)
   {
	  if(!Decode32Block(&in[j * 8], &outBuff[0])) return false;
	  memmove(&out[j * 5], &outBuff[0], sizeof(unsigned char) * 5);
   }

   unsigned char padd[8];
   memset(padd, 0, sizeof(unsigned char) * 8);
   for(int i = 0; i < r; i++)
   {
	  padd[i] = in[inLen - r + i];
   }
   if(!Decode32Block(&padd[0], &outBuff[0])) return false;
   memmove(&out[d * 5], &outBuff[0], sizeof(unsigned char) * GetDecode32Length(r));

   return true;
}

bool Map32(unsigned char * inout32, int inout32Len, unsigned char * alpha32)
{
	if((inout32 == 0) || (inout32Len <= 0) || (alpha32 == 0)) return false;
	for(int i = 0; i < inout32Len; i++)
	{
		if(inout32[i] >=32) return false;
		inout32[i] = alpha32[inout32[i]];
	}
	return true;
}

static void ReverseMap(unsigned char * inAlpha32, unsigned char * outMap)
{
	memset(outMap, 0, sizeof(unsigned char) * 256);
	for(int i = 0; i < 32; i++)
	{
		outMap[(int)inAlpha32[i]] = i;
	}
}

bool Unmap32(unsigned char * inout32, int inout32Len, unsigned char * alpha32)
{
	if((inout32 == 0) || (inout32Len <= 0) || (alpha32 == 0)) return false;
	unsigned char rmap[256];
	ReverseMap(alpha32, rmap);
	for(int i = 0; i < inout32Len; i++)
	{
		inout32[i] = rmap[(int)inout32[i]];
	}
	return true;
}

void T2B32 (std::string ipToChange2, char * targetEncrypted) {
	char target0[ipToChange2.size()];
	memcpy(target0, ipToChange2.c_str(), ipToChange2.size());
	target0[ipToChange2.size()] = '\0';
	if (encrypt_IPs) {
		encryptMessage((unsigned char *) target0, ipToChange2.size(), (unsigned char *) targetEncrypted, 0);
	} else {
		memcpy(targetEncrypted, target0, ipToChange2.length());
	}
	targetEncrypted[ipToChange2.size()] = '\0';
	int encodeLength = GetEncode32Length(ipToChange2.size());
	char data32[encodeLength];
	Encode32((unsigned char *) targetEncrypted, ipToChange2.size(), (unsigned char *) data32);
	Map32((unsigned char *) data32, encodeLength, (unsigned char *) alpha32);
	memcpy(targetEncrypted, data32, encodeLength);
	targetEncrypted[encodeLength] = '\0';
}

void B322T (char * targetEncrypted, char * target) {
	char targetDecrypted[strlen(targetEncrypted)];
	int decodeLength = GetDecode32Length(strlen(targetEncrypted));
	int encodeLength = GetEncode32Length(decodeLength);
	Unmap32((unsigned char *) targetEncrypted, encodeLength, (unsigned char *) alpha32);
	char decode256[decodeLength];
	Decode32((unsigned char *) targetEncrypted, encodeLength, (unsigned char *) decode256);
	decode256[decodeLength] = '\0';
	if (encrypt_IPs) {
		decryptMessage((uint8_t *) decode256, decodeLength, (uint8_t *) targetDecrypted, 0);
	} else {
		memcpy(targetDecrypted, decode256, decodeLength);
	}
	targetDecrypted[decodeLength] = '\0';
	memcpy(target, targetDecrypted, strlen(targetDecrypted));
	target[strlen(targetDecrypted)] = '\0';
}

void printT2B32(char * str2) {
	std::string str(str2);
	emit_debug(str.c_str());
	char targetEncrypted[GetEncode32Length(str.size())];
	T2B32(str, targetEncrypted);
	emit_debug(targetEncrypted);
	char target[strlen(targetEncrypted)];
	B322T(targetEncrypted, target);
	if (strcmp(str.c_str(),target) != 0) {
		emit_debug("WARNING - STRINGS DO NOT MATCH!");
		emit_debug("v v v");
		emit_debug(str.c_str());
		emit_debug("^ ^ ^");
		emit_debug("v v v");
		emit_debug(target);
		emit_debug("^ ^ ^");
	}
}

struct map_element {
	char * key;
	char * value;
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
	std::memcpy(y, string.c_str(), string.length());
	y[string.length()] = '\0';
	std::string string2(y);
	return string2;
}

char * copystring2char(std::string string2) {
	int length = (int)string2.length();
	char * st = (char *) malloc((length+1)*sizeof(char));
	std::memcpy(st, string2.c_str(), length+1);
	st[length] = '\0';
	return st;
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
	struct map_element * current;
	struct map_element * current_old;
	if (map && map->first && map->first != NULL) {
		current = map->first;
	} else {
		current = NULL;
	}
	while(current && current != NULL) {
		current_old = current;
		current = map_get_next(current_old);
		if (current_old && current_old != NULL) {
			if (current_old->key && current_old->key != NULL) {
				free(current_old->key);
			}
			if (current_old->value && current_old->value != NULL) {
				free(current_old->value);
			}
			if (current_old->inmap && current_old->inmap != NULL) {
				map_destroy(current_old->inmap);
			}
			if (current_old && current_old != NULL) {
				free(current_old);
			}
		}
	}
	if (map && map != NULL) {
		free(map);
	}	
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

int isBiggerThan(std::string value, char * valueInMem) {
	char * value2 = (char *)value.c_str();
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

int isHttp(char * msg) {
	std::string beginning(msg, 0, 4);
	if (beginning == "GET " || beginning == "POST" || beginning == "PUT " || beginning == "DELE" || beginning == "HTTP") {
		return 0;
	} else {
		return 1;
	}
}
int isOption(char * msg) {
	std::string beginning(msg, 0, 4);
	if (beginning == "OPTI") {
		return 0;
	} else {
		return 1;
	}
}

char * substr(char * arr, int begin, int len) {
	char * res = new char[len];
	for (int i = 0; i < len; i++)
		res[i] = *(arr + begin + i);
	res[len] = 0;
	return res;
}

int testEndTransferEncoding(char * msg, int size) {
	int result = 1;
	int sizeOfEndBufferMarker = 5;
	if (size < sizeOfEndBufferMarker) {
		return 1;
	}
	char * test = substr(msg,size-sizeOfEndBufferMarker, sizeOfEndBufferMarker);
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

char * createNewHeader(char * msg, std::string address, int size) {
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
		header.insert(posEnd + 2, "Connection: Close\r\n");
	}
	header.replace(posHost, posEnd-posHost, address);
	posForward = header.find("X-Forwarded-Host: ") + 18;
	posEndForward = header.find("\r\n", posForward);
	header.replace(posForward, posEndForward-posForward, "localhost:8080");
	if (header.find("From-SGX") == std::string::npos) {
		posHost = header.find("Host: ") + 6;
		posEnd = header.find("\r\n", posHost);
		header.insert(posEnd + 2, "From-SGX: fromsgx\r\n");
	} else {
		posHost = header.find("From-SGX: ");
		posEnd = header.find("\r\n", posHost);
		header.replace(posHost, posEnd+2-posHost, "");
	}
	char *y = new char[header.length() + 1];
	std::memcpy(y, header.c_str(), header.length());
	y[header.length()] = '\0';
	return y;
}

char * addContentToAnswer(std::string header, std::string content) {
	//std::string header(answer);
	int pos = header.find("Content-Length: ") + 16;
	int posEnd = header.find("\r\n", pos);
	int size = content.length();
	char * chr = (char *) malloc(1024);
	ocall_int_to_string(size, chr);
	std::string s2(chr);
	header.replace(pos, posEnd-pos, s2);
	header += content;
	char *y = new char[header.length()];
	std::memcpy(y, header.c_str(), header.length());
	y[header.length()] = '\0';
	free(chr);
	return y;
}

struct map* parse_headers(char * msg2, int headersSize) {
	struct map* headers = map_init();
	int endPos = 0;
	int pos = 0;
	int oldPos = 0;
	std::string allmsg(msg2);
	int posmiddle = 0;
	int firstSpace = 0;
	int secondSpace = 0;
	//std::map<std::string, std::string> headers;
	endPos = allmsg.find("\r\n\r\n");
	std::string sSize1("HeaderSize");
	char chr[1024];
	ocall_int_to_string((endPos + 4), chr);
	std::string sSize2(chr);
	//headers[copystring(sSize1)] = copystring(sSize2);
	map_add(headers, sSize1, sSize2);
	pos = allmsg.find("\r\n");
	firstSpace = allmsg.find(" ");
	std::string sMethod1("Method");
	std::string sMethod2(msg2, oldPos, firstSpace);
	//headers[copystring(sMethod1)] = copystring(sMethod2);
	map_add(headers, sMethod1, sMethod2);
	secondSpace = allmsg.find(" ", firstSpace + 1);
	std::string sPath1("Path");
	std::string sPath2(msg2, firstSpace + 1, secondSpace - firstSpace - 1);
	//headers[copystring(sPath1)] = copystring(sPath2);
	map_add(headers, sPath1, sPath2);
	std::string sProto1("Protocol");
	std::string sProto2(msg2, secondSpace + 1, pos - secondSpace - 1);
	//headers[copystring(sProto1)] = copystring(sProto2);
	map_add(headers, sProto1, sProto2);
	while (pos < endPos && pos > 0) {
		oldPos = pos;
		pos = allmsg.find("\r\n", pos + 1);
		posmiddle = allmsg.find(":", oldPos + 1);
		std::string s1(msg2, oldPos + 2, posmiddle - oldPos - 2);
		std::string s2(msg2, posmiddle + 2, pos - posmiddle - 2);
		//headers[copystring(s1)] = copystring(s2);
		map_add(headers, s1, s2);

	}
	return headers;
}

int cutInto16BytesMultiple(char * bufferIn, char * bufferOut, int totalSize) {
	int remainingSize;
	if (totalSize % 16 == 0) {
		remainingSize = 0;
	} else {
		remainingSize = (totalSize % 16);
		memcpy(bufferOut, bufferIn + (16 * (totalSize/16)), remainingSize);
	}
	return remainingSize;
}

void sendTokensToPlayer(int csock) {
	std::string answer = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Methods: GET, POST, DELETE, OPTIONS\r\nAccess-Control-Allow-Headers: Origin, Content-Type, Accept, x-forwarded-host\r\nContent-Length: 0\r\nContent-Type: text/plain\r\nConnection: Close\r\n\r\n";
	char chr[1024];
	ocall_int_to_string(numberOfTokens, chr);
	std::string content(chr);
	char * finalAnswer = addContentToAnswer(answer, content);
	ocall_sendanswer(csock, finalAnswer, strlen(finalAnswer));
	if (finalAnswer != NULL) {
		free(finalAnswer);
	}
}

void sendAddressesToPlayer(int csock) {
	std::string answer = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Methods: GET, POST, DELETE, OPTIONS\r\nAccess-Control-Allow-Headers: Origin, Content-Type, Accept, x-forwarded-host\r\nContent-Length: 0\r\nContent-Type: text/plain\r\nConnection: Close\r\n\r\n";
	std::string content;
	char address[500];
	memset(address, 0, 500);
	T2B32(trackerAddr, address);
	content += std::string(address) + ",";
	memset(address, 0, 500);
	T2B32(serverAddr, address);
	content += std::string(address) + ",";
	memset(address, 0, 500);
	T2B32(mpdAddr, address);
	content += std::string(address) + ",";
	content += std::string("http://") + std::string(address) + mpdRes;
	char * finalAnswer = addContentToAnswer(answer, content);
	ocall_sendanswer(csock, finalAnswer, strlen(finalAnswer));
	if (finalAnswer != NULL) {
		free(finalAnswer);
	}
}

void cleanup_memory(int client_sock, struct map* headersRequest, char * finalanswer) {
	if (client_sock > 0) {
		ocall_closesocket(client_sock);
	}
	if (headersRequest != NULL) {
		map_destroy(headersRequest);
	}
	if (finalanswer != NULL) {
		free(finalanswer);
	}
}

void do_encryption(bool enable, bool fromSGX, char * buffIn, char * buffOut, int buffSize, uint32_t counter) {
	memset(buffOut, 0, buffSize);
	if (encrypt && enable) {
		if (fromSGX) {
			decryptMessage((uint8_t *) buffIn, buffSize, (uint8_t *) buffOut, counter);
		} else {
			encryptMessage((uint8_t *) buffIn, buffSize, (uint8_t *) buffOut, counter);
		}
	} else {
		memcpy(buffOut, buffIn, buffSize);
	}
}

void handle_encryption(bool fromSGX, char * finalBuff, int buffSize, uint32_t counter) {
	int offset = getPosEndOfHeader(finalBuff) < 0 ? 0 : getPosEndOfHeader(finalBuff) + 4;
	int payloadSize = buffSize - offset;
	if (payloadSize > 0) {
		char fullBuff[buffSize];
		char payload[payloadSize];
		char codedBuff[payloadSize];
		memcpy(fullBuff, finalBuff, offset);
		memcpy(payload, finalBuff + offset, payloadSize);
		do_encryption(true, fromSGX, payload, codedBuff, payloadSize, counter);
		memcpy(fullBuff + offset, codedBuff, payloadSize);
		memcpy(finalBuff, fullBuff, buffSize);
	}
}

void content_encoding_loop(int csock, int client_sock, bool fromSGX, char * finalanswer, int sizeAnswerFromClient) {

	int previous_subpacket_tail_size = 0;
	int sub_packet_size = 0;
	int data_sent = 0;
	int loops = 0;
	int testEndTransfer = -1;
	char last16[16];
	uint32_t counter_16bytes = 0;
	char * answerFromClient = (char *) malloc(1028 * sizeof(char));
	char * previous_subpacket_tail = (char *) malloc(16 * sizeof(char));
	
	char out[sizeAnswerFromClient];
	do_encryption(true, fromSGX, finalanswer, out, sizeAnswerFromClient, counter_16bytes);
	ocall_sendanswer(csock, out, sizeAnswerFromClient);
	
	// unsigned char * buff_dest = (unsigned char *) buff_orig;
	// unsigned char * pixels = static_cast<unsigned char *>(memory_buffer);
	
	while (testEndTransfer != 0) {
		memset(answerFromClient, 0, 1028 * sizeof(char));
		ocall_receiveFromClient(client_sock, answerFromClient);
		sub_packet_size = extractSize(answerFromClient);
		if (sub_packet_size > 0) {
			char buff[sub_packet_size];
			extractBuffer(answerFromClient, sub_packet_size, buff);
			char sub_packet[(previous_subpacket_tail_size + sub_packet_size)];
			memcpy(sub_packet, previous_subpacket_tail, previous_subpacket_tail_size);
			memcpy(sub_packet + previous_subpacket_tail_size, buff, sub_packet_size);
			sub_packet_size += previous_subpacket_tail_size;
			int valid_packet_size = 16 * (sub_packet_size / 16);
			if (valid_packet_size > 0) {
				char out[valid_packet_size];
				testEndTransfer = !fromSGX ? testEndTransferEncoding(sub_packet, valid_packet_size) : testEndTransfer;
				do_encryption(enable_TE_encryption, fromSGX, sub_packet, out, valid_packet_size, counter_16bytes);
				testEndTransfer = fromSGX ? testEndTransferEncoding(out, valid_packet_size) : testEndTransfer;
				ocall_sendanswer(csock, out, valid_packet_size);
				counter_16bytes += valid_packet_size / 16;
				memcpy(last16, sub_packet + valid_packet_size - 16, 16);
				previous_subpacket_tail_size = sub_packet_size - valid_packet_size;
				previous_subpacket_tail = (char *) realloc(previous_subpacket_tail, previous_subpacket_tail_size * sizeof(char));
				memcpy(previous_subpacket_tail, sub_packet + valid_packet_size, previous_subpacket_tail_size);
				data_sent += valid_packet_size;
				loops++;
			}
		}
		if (previous_subpacket_tail_size > 0) {
			char out[previous_subpacket_tail_size+16];
			char buff16[previous_subpacket_tail_size+16];
			memcpy(buff16, last16, 16);
			memcpy(buff16 + 16, previous_subpacket_tail, previous_subpacket_tail_size);
			testEndTransfer = !fromSGX ? testEndTransferEncoding(buff16, previous_subpacket_tail_size + 16) : testEndTransfer;
			do_encryption(enable_TE_encryption, fromSGX, buff16, out, previous_subpacket_tail_size + 16, counter_16bytes - 1);
			testEndTransfer = fromSGX ? testEndTransferEncoding(out, previous_subpacket_tail_size + 16) : testEndTransfer;
			if (testEndTransfer == 0) {
				data_sent += previous_subpacket_tail_size;
				loops++;
				ocall_sendanswer(csock, out + 16, previous_subpacket_tail_size);
			}
		}
	}
	display_TE(csock, loops, data_sent/1000);
	//blockchain	
}

void proxy_loop(int csock, int client_sock, bool fromSGX, char * finalanswer, int sizeAnswerFromClient, char * answerFromClient) {

	int out;
	int totalSizeAnswer = 0;
	uint32_t counter = 0;
	char finalanswer_backup[sizeAnswerFromClient];
	
	memset(finalanswer_backup, 0, sizeAnswerFromClient * sizeof(char));
	memcpy(finalanswer_backup, finalanswer, sizeAnswerFromClient);
	handle_encryption(fromSGX, finalanswer, sizeAnswerFromClient, counter);
	// finalanswer -> if fromSGX: encrypt / else: decrypt
	if (sizeAnswerFromClient > 0) {
		if (isHttp(finalanswer) == 0) {
			struct map* headersAnswer = NULL;
			headersAnswer = parse_headers(finalanswer, getPosEndOfHeader(finalanswer) + 4);
			std::string contentLength = map_find(headersAnswer, "Content-Length") > 0 ? "Content-Length" : "content-length";
			std::string transferEncoding = map_find(headersAnswer, "Transfer-Encoding") > 0 ? "Transfer-Encoding" : "transfer-encoding";
			if (map_find(headersAnswer, contentLength) > 0) {
				ocall_string_to_int(map_get(headersAnswer, "HeaderSize"), (int) strlen(map_get(headersAnswer, "HeaderSize")), &out);
				totalSizeAnswer += sizeAnswerFromClient - out;
				ocall_string_to_int(map_get(headersAnswer, contentLength), (int) strlen(map_get(headersAnswer, contentLength)), &out);
				while (testContentLength(out, totalSizeAnswer) != 0 && sizeAnswerFromClient != 0) {
					ocall_sendanswer(csock, finalanswer, sizeAnswerFromClient);
					memset(answerFromClient, 0, 1028);
					ocall_receiveFromClient(client_sock, answerFromClient);
					sizeAnswerFromClient = extractSize(answerFromClient);
					finalanswer = (char *) realloc(finalanswer, sizeAnswerFromClient * sizeof(char));
					memset(finalanswer, 0, sizeAnswerFromClient * sizeof(char));
					extractBuffer(answerFromClient, sizeAnswerFromClient, finalanswer);
					handle_encryption(fromSGX, finalanswer, sizeAnswerFromClient, counter);
					totalSizeAnswer += sizeAnswerFromClient;
				}
				ocall_sendanswer(csock, finalanswer, sizeAnswerFromClient);
			} else if (map_find(headersAnswer, transferEncoding) > 0) {
				content_encoding_loop(csock, client_sock, fromSGX, finalanswer_backup, sizeAnswerFromClient);
			} else {
				ocall_sendanswer(csock, finalanswer, sizeAnswerFromClient);
			}
			map_destroy(headersAnswer);
		} else {
			ocall_sendanswer(csock, finalanswer, sizeAnswerFromClient);
		}
	}
}

void handleProxy(int csock, char * msg, int msgsize) {

	bool fromSGX = true;
	int client_sock;
	int sizeAnswerFromClient;
	char answerFromClient[1028];
	char * answer;
	char * finalanswer = (char *) malloc(1028 * sizeof(char));
	struct map* headersRequest = parse_headers(msg, getPosEndOfHeader(msg)+4);	
	char * target2 = map_get(headersRequest, "X-Forwarded-Host");
	
	if (target2 == NULL) {
		sendTokensToPlayer(csock);
	} else if (strcmp(target2, "addr") == 0) {
		sendAddressesToPlayer(csock); // trackerAddr,serverAddr,mpdAddr,mpdURL
	} else {
		char target[strlen(target2)];
		if (strcmp(target2, serverAddr.c_str()) == 0) {
			memcpy(target, target2, strlen(target2));
			emit_debug(target);
		} else {
			B322T(target2, target);
			printT2B32(target);
		}
		fromSGX = (map_find(headersRequest, "From-SGX") > 0);
		ocall_startClient(&client_sock, target);
		if (client_sock < 1) {
			display_msg(csock,"Start client failed! Dropping packet.");
		} else {
			sizeAnswerFromClient = msgsize;
			answer = createNewHeader(msg, target, sizeAnswerFromClient);
			finalanswer = (char *) realloc(finalanswer, sizeAnswerFromClient * sizeof(char));
			memset(finalanswer, 0, sizeAnswerFromClient * sizeof(char));
			memcpy(finalanswer, msg, sizeAnswerFromClient);
			handle_encryption(fromSGX, finalanswer, sizeAnswerFromClient, 0);
			ocall_sendToClient(client_sock, answer, (int) strlen(answer), answerFromClient);
			sizeAnswerFromClient = extractSize(answerFromClient);
			finalanswer = (char *) realloc(finalanswer, sizeAnswerFromClient * sizeof(char));
			memset(finalanswer, 0, sizeAnswerFromClient * sizeof(char));
			extractBuffer(answerFromClient, sizeAnswerFromClient, finalanswer);
			proxy_loop(csock, client_sock, fromSGX, finalanswer, sizeAnswerFromClient, answerFromClient);
		}
	}
	cleanup_memory(client_sock, headersRequest, finalanswer);
}

/*
// blockchain
numberOfTokens = blockchain_values::getBalance();
if (fromSGX) {
	blockchain_values::assignCoin(+1);
} else {
	if (numberOfTokens > 1) {
		blockchain_values::assignCoin(-1);
	}
}
numberOfTokens = blockchain_values::getBalance();
*/

/*
std::string str(finalanswer);
size_t pos = str.find("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
if (pos > 0) {
	char baseURLEnc[32];
	memset(baseURLEnc, 0, 32);
	T2B32(mpdAddr, baseURLEnc);
	str.replace(pos, 32, std::string(baseURLEnc));
	memcpy(finalanswer, str.c_str(), str.size());
}
*/

/*
char * returnMPD(void) {
	char mpd[] = "MPD-HERE";
	memset(mpd, 0, (strlen(mpd))*sizeof(char));
	char encryptedMPD[strlen(mpd)];
	memset(encryptedMPD, 0, strlen(mpd)*sizeof(char));
	encryptMessage(mpd, strlen(mpd), encryptedMPD, 0);
	return encryptedMPD;
}
*/

void handleTracker(int csock, char * msg, int size, int debug) {

	// Decryption: msg -> fullDecryptedMessage
	uint32_t counter = 0;
	
	int endPos = getPosEndOfHeader(msg)+4;
	int msgSize = size-endPos;
	char fullDecryptedMessage[size];
	char decryptedMessage[msgSize];
	memcpy(fullDecryptedMessage, msg, endPos);
	if (endPos < size) {
		char messageToDecrypt[msgSize];
		memcpy(messageToDecrypt, msg+endPos, msgSize);
		bool encrypt_decrypt = debug == 0 ? true : false;
		do_encryption(true, encrypt_decrypt, messageToDecrypt, decryptedMessage, msgSize, counter);
		counter = msgSize / 16;
		memcpy(fullDecryptedMessage+endPos, decryptedMessage, msgSize);
	}
	// fullDecryptedMessage
	
	std::string answer = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Methods: GET, POST, DELETE, OPTIONS\r\nAccess-Control-Allow-Headers: Origin, Content-Type, Accept, x-forwarded-host\r\nContent-Length: 0\r\nContent-Type: text/plain\r\nConnection: Close\r\n\r\n";

	char * finalanswer;
	struct map* headersRequest = parse_headers(fullDecryptedMessage, getPosEndOfHeader(fullDecryptedMessage)+4);
	std::string value = map_get(headersRequest, "Method");
	std::string content;
	struct map* ipmap;
	struct map_element * current;
	
	if (debug == 0) {
		if (value == "POST") {
			sgx_thread_mutex_lock(&mutex);
			content = "";
			std::string msgContent(decryptedMessage);
			int firstComa = msgContent.find(",");
			int secondComa = msgContent.find(",", firstComa + 1);
			std::string videoID = msgContent.substr(0, firstComa);
			std::string ipToChange2 = msgContent.substr(firstComa+1, secondComa-firstComa-1);
			std::string numberOfSegment = msgContent.substr(secondComa+1);
			
			char clientip[30];
			ocall_getSocketIP(csock, clientip);
			std::string ipToChange(clientip);
			ipToChange += ":" + proxyPort;
			memset(clientip, 0, 30);
			T2B32(ipToChange, clientip);
			ipToChange = std::string(clientip);

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
				} else {
					map_replace(ipmap, ipToChange, numberOfSegment);
				}
			}
			finalanswer = addContentToAnswer(answer, content);
			sgx_thread_mutex_unlock(&mutex);
		} else if (value == "GET") {
			sgx_thread_mutex_lock(&mutex);
			std::string msgContent(decryptedMessage);
			int begin = msgContent.find("/?");
			int firstComa = msgContent.find(",", begin);
			int endpath = msgContent.find(" ", begin);
			std::string  videoID = msgContent.substr(begin + 2, firstComa-begin - 2);
			std::string  numberOfSegments = msgContent.substr(firstComa+1, endpath-firstComa-1);
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
	char fullEncryptedMessage[answer.length()+msgSize];
	memcpy(fullEncryptedMessage, finalanswer, endPos);
	if (endPos < (int) strlen(finalanswer)) {
		char messageToEncrypt[msgSize];
		char encryptedMessage[msgSize];
		memcpy(messageToEncrypt, finalanswer+endPos, msgSize);
		bool encrypt_decrypt = debug == 1 ? true : false;
		do_encryption(true, encrypt_decrypt, messageToEncrypt, encryptedMessage, msgSize, counter);
		counter = msgSize / 16;
		memcpy(fullEncryptedMessage+endPos, encryptedMessage, msgSize);
	}
	// fullEncryptedMessage
	
	ocall_sendanswer(csock, fullEncryptedMessage, strlen(fullEncryptedMessage));
	map_destroy(headersRequest);
}

void handleOption(int csock) {
	char answer[1024] = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Methods: GET, POST, DELETE, OPTIONS\r\nAccess-Control-Allow-Headers: Origin, Content-Type, Accept, x-forwarded-host, access-control-allow-origin\r\nConnection: Closed\r\n\r\n\0";
	//char test[1024] = "GET / HTTP/1.1\r\nHost: lacaud.fr\r\nUser-Agent: curl/7.55.1\r\nConnection: close\r\nAccept: */*\r\n\r\n";
	ocall_sendanswer(csock, answer, strlen(answer));
}

void ecall_handlemessage(int csock, int type, char * msg, int size){
	int http = isHttp(msg);
	if (http == 0) {
		if (type == 0) {
			display_msg(csock,"Forwarding (proxy) request...");
			handleProxy(csock, msg, size);
		}
		if (type == 1) {
			display_msg(csock,"Tracker request...");
			handleTracker(csock, msg, size, 0);
		}
		if (type == 11) {
			display_msg(csock,"Tracker encryption test...");
			handleTracker(csock, msg, size, 1);
		}
		if (type == 2) {
			display_msg(csock,"Blockchain test...");
			blockchain_values::getBalance();
			blockchain_values::assignCoin(999999999);
			blockchain_values::getBalance();
		}
		if (type == 32) {
			// base32 test
			display_msg(csock,"Base32 translation test...");
			std::string str("tracker");
			display_msg(csock,str.c_str());
			char targetEncrypted[GetEncode32Length(str.size())];
			T2B32(str, targetEncrypted);
			display_msg(csock,targetEncrypted);
			char target[strlen(targetEncrypted)];
			B322T(targetEncrypted, target);
			display_msg(csock,target);
		}
		if (type == 22) {
			display_msg(csock,"Addresses translation...");
			printT2B32((char *) "tracker");
			printT2B32((char *) "mpdserver");
			printT2B32((char *) "localhost:8080");
			printT2B32((char *) "147.210.128.146:8081");
		}
	} else {
		int option = isOption(msg);
		if (option == 0) {
			display_msg(csock,"Options request...");
			handleOption(csock);
		} else {
			display_msg(csock,"Request type not detected!");
		}
	}
}

