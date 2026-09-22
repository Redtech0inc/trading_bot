#include <iostream>
#include <curl/curl.h>
#include "../secrets/secrets.hpp"

size_t callback(char* data, size_t size, size_t numberOfElements, void* userdata){
    std::cout.write(data, size*numberOfElements);
    return size * numberOfElements;
}

int main(){

    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL* curl = curl_easy_init();
    if(curl == nullptr) {
        std::cout << "could not open curl client" << std::endl;
        return 1;
    }

    //give it it's certification
    curl_easy_setopt(curl, CURLOPT_CAINFO, secrets::CURL_AC_CERT_PATH);
    //configure request to request from example.com
    curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");
    //and call callback when success
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);

    //perform request
    CURLcode result = curl_easy_perform(curl);

    if(result != CURLE_OK){
        std::cout << "request failed:" << std::endl;
        std::cout << curl_easy_strerror(result) << std::endl;
    }else std::cout << "request success!" << std::endl;

    //cleanup all curl memory
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}