SGX_SDK ?= /opt/intel/sgxsdk
SGX_MODE ?= SIM
SGX_ARCH ?= x64

ifeq ($(shell getconf LONG_BIT), 32)
	SGX_ARCH := x86
else ifeq ($(findstring -m32, $(CXXFLAGS)), -m32)
	SGX_ARCH := x86
endif

ifeq ($(SGX_ARCH), x86)
	SGX_COMMON_CFLAGS := -m32
	SGX_LIBRARY_PATH := $(SGX_SDK)/lib
	SGX_ENCLAVE_SIGNER := $(SGX_SDK)/bin/x86/sgx_sign
	SGX_EDGER8R := $(SGX_SDK)/bin/x86/sgx_edger8r
else
	SGX_COMMON_CFLAGS := -m64
	SGX_LIBRARY_PATH := $(SGX_SDK)/lib64
	SGX_ENCLAVE_SIGNER := $(SGX_SDK)/bin/x64/sgx_sign
	SGX_EDGER8R := $(SGX_SDK)/bin/x64/sgx_edger8r
endif

ifeq ($(SGX_DEBUG), 1)
ifeq ($(SGX_PRERELEASE), 1)
$(error Cannot set SGX_DEBUG and SGX_PRERELEASE at the same time!!)
endif
endif

ifeq ($(SGX_DEBUG), 1)
        SGX_COMMON_CFLAGS += -O0 -g
else
        SGX_COMMON_CFLAGS += -O2
endif

######## SGXSRDSServer Settings ########

ifneq ($(SGX_MODE), HW)
	Urts_Library_Name := sgx_urts_sim
else
	Urts_Library_Name := sgx_urts
endif

App_Cpp_Files := SGXSRDSServer/SGXSRDSServer.cpp SGXSRDSServer/trackerdb.cpp SGXSRDSServer/server.cpp SGXSRDSServer/common_socket.cpp SGXSRDSServer/http_header.cpp SGXSRDSServer/message_management.cpp SGXSRDSServer/client.cpp
App_Include_Paths := -IInclude -IApp -I$(SGX_SDK)/include

App_C_Flags := $(SGX_COMMON_CFLAGS) -fPIC -Wno-attributes $(App_Include_Paths)

# Three configuration modes - Debug, prerelease, release
#   Debug - Macro DEBUG enabled.
#   Prerelease - Macro NDEBUG and EDEBUG enabled.
#   Release - Macro NDEBUG enabled.
ifeq ($(SGX_DEBUG), 1)
        App_C_Flags += -DDEBUG -UNDEBUG -UEDEBUG
else ifeq ($(SGX_PRERELEASE), 1)
        App_C_Flags += -DNDEBUG -DEDEBUG -UDEBUG
else
        App_C_Flags += -DNDEBUG -UEDEBUG -UDEBUG
endif

App_Cpp_Flags := $(App_C_Flags) -std=c++11
App_Link_Flags := $(SGX_COMMON_CFLAGS) -L$(SGX_LIBRARY_PATH) -l$(Urts_Library_Name) -lpthread 

ifneq ($(SGX_MODE), HW)
	App_Link_Flags += -lsgx_uae_service_sim
else
	App_Link_Flags += -lsgx_uae_service
endif

App_Cpp_Objects := $(App_Cpp_Files:.cpp=.o)

App_Name := SGXSRDSServerApp

######## SGXSRDSEnclave Settings ########

ifneq ($(SGX_MODE), HW)
	Trts_Library_Name := sgx_trts_sim
	Service_Library_Name := sgx_tservice_sim
else
	Trts_Library_Name := sgx_trts
	Service_Library_Name := sgx_tservice
endif
Crypto_Library_Name := sgx_tcrypto

SGXSRDSEnclave_Cpp_Files := SGXSRDSEnclave/SGXSRDSEnclave.cpp
SGXSRDSEnclave_Include_Paths := -IInclude -ISGXSRDSEnclave -I$(SGX_SDK)/include -I$(SGX_SDK)/include/tlibc -I$(SGX_SDK)/include/stlport

SGXSRDSEnclave_C_Flags := $(SGX_COMMON_CFLAGS) -nostdinc -fvisibility=hidden -fpie -fstack-protector $(SGXSRDSEnclave_Include_Paths)
SGXSRDSEnclave_Cpp_Flags := $(SGXSRDSEnclave_C_Flags) -std=c++03 -nostdinc++
SGXSRDSEnclave_Link_Flags := $(SGX_COMMON_CFLAGS) -Wl,--no-undefined -nostdlib -nodefaultlibs -nostartfiles -L$(SGX_LIBRARY_PATH) \
	-Wl,--whole-archive -l$(Trts_Library_Name) -Wl,--no-whole-archive \
	-Wl,--start-group -lsgx_tstdc -lsgx_tstdcxx -l$(Crypto_Library_Name) -l$(Service_Library_Name) -Wl,--end-group \
	-Wl,-Bstatic -Wl,-Bsymbolic -Wl,--no-undefined \
	-Wl,-pie,-eenclave_entry -Wl,--export-dynamic  \
	-Wl,--defsym,__ImageBase=0 \
	-Wl,--version-script=SGXSRDSEnclave/SGXSRDSEnclave.lds

SGXSRDSEnclave_Cpp_Objects := $(SGXSRDSEnclave_Cpp_Files:.cpp=.o)

SGXSRDSEnclave_Name := SGXSRDSEnclave.so
Signed_SGXSRDSEnclave_Name := SGXSRDSEnclave.signed.so
SGXSRDSEnclave_Config_File := SGXSRDSEnclave/SGXSRDSEnclave.config.xml

ifeq ($(SGX_MODE), HW)
ifneq ($(SGX_DEBUG), 1)
ifneq ($(SGX_PRERELEASE), 1)
Build_Mode = HW_RELEASE
endif
endif
endif


.PHONY: all run

ifeq ($(Build_Mode), HW_RELEASE)
all: $(App_Name) $(SGXSRDSEnclave_Name)
	@echo "The project has been built in release hardware mode."
	@echo "Please sign the $(SGXSRDSEnclave_Name) first with your signing key before you run the $(App_Name) to launch and access the enclave."
	@echo "To sign the enclave use the command:"
	@echo "   $(SGX_ENCLAVE_SIGNER) sign -key <your key> -enclave $(SGXSRDSEnclave_Name) -out <$(Signed_SGXSRDSEnclave_Name)> -config $(SGXSRDSEnclave_Config_File)"
	@echo "You can also sign the enclave using an external signing tool. See User's Guide for more details."
	@echo "To build the project in simulation mode set SGX_MODE=SIM. To build the project in prerelease mode set SGX_PRERELEASE=1 and SGX_MODE=HW."
else
all: $(App_Name) $(Signed_SGXSRDSEnclave_Name)
endif

run: all
ifneq ($(Build_Mode), HW_RELEASE)
	@$(CURDIR)/$(App_Name)
	@echo "RUN  =>  $(App_Name) [$(SGX_MODE)|$(SGX_ARCH), OK]"
endif

######## SGXSRDSServer Objects ########

SGXSRDSServer/SGXSRDSEnclave_u.c: $(SGX_EDGER8R) SGXSRDSEnclave/SGXSRDSEnclave.edl
	@cd SGXSRDSServer && $(SGX_EDGER8R) --untrusted ../SGXSRDSEnclave/SGXSRDSEnclave.edl --search-path ../SGXSRDSEnclave --search-path $(SGX_SDK)/include
	@echo "GEN  =>  $@"

SGXSRDSServer/SGXSRDSEnclave_u.o: SGXSRDSServer/SGXSRDSEnclave_u.c
	@$(CC) $(App_C_Flags) -c $< -o $@
	@echo "CC   <=  $<"

SGXSRDSServer/%.o: SGXSRDSServer/%.cpp
	@$(CXX) $(App_Cpp_Flags) -c $< -o $@
	@echo "CXX  <=  $<"

$(App_Name): SGXSRDSServer/SGXSRDSEnclave_u.o $(App_Cpp_Objects)
	@$(CXX) $^ -o $@ $(App_Link_Flags)
	@echo "LINK =>  $@"


######## SGXSRDSEnclave Objects ########

SGXSRDSEnclave/SGXSRDSEnclave_t.c: $(SGX_EDGER8R) SGXSRDSEnclave/SGXSRDSEnclave.edl
	@cd SGXSRDSEnclave && $(SGX_EDGER8R) --trusted ../SGXSRDSEnclave/SGXSRDSEnclave.edl --search-path ../SGXSRDSEnclave --search-path $(SGX_SDK)/include
	@echo "GEN  =>  $@"

SGXSRDSEnclave/SGXSRDSEnclave_t.o: SGXSRDSEnclave/SGXSRDSEnclave_t.c
	@$(CC) $(SGXSRDSEnclave_C_Flags) -c $< -o $@
	@echo "CC   <=  $<"

SGXSRDSEnclave/%.o: SGXSRDSEnclave/%.cpp
	@$(CXX) $(SGXSRDSEnclave_Cpp_Flags) -c $< -o $@
	@echo "CXX  <=  $<"

$(SGXSRDSEnclave_Name): SGXSRDSEnclave/SGXSRDSEnclave_t.o $(SGXSRDSEnclave_Cpp_Objects)
	@$(CXX) $^ -o $@ $(SGXSRDSEnclave_Link_Flags)
	@echo "LINK =>  $@"

$(Signed_SGXSRDSEnclave_Name): $(SGXSRDSEnclave_Name)
	@$(SGX_ENCLAVE_SIGNER) sign -key SGXSRDSEnclave/SGXSRDSEnclave_private.pem -enclave $(SGXSRDSEnclave_Name) -out $@ -config $(SGXSRDSEnclave_Config_File)
	@echo "SIGN =>  $@"

.PHONY: clean

clean:
	@rm -f $(App_Name) $(SGXSRDSEnclave_Name) $(Signed_SGXSRDSEnclave_Name) $(App_Cpp_Objects) SGXSRDSServer/SGXSRDSEnclave_u.* $(SGXSRDSEnclave_Cpp_Objects) SGXSRDSEnclave/SGXSRDSEnclave_t.*
