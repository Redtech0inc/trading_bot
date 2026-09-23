#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include "../secrets/secrets.hpp"

#define IS_TEST_RUN true
#define COLLECT_DATA false

enum class Exchange{
    Frankfurt,
    Xetra,
    Stuttgart
};

std::string getSuffix(Exchange* market){
    switch (*market){
        case Exchange::Frankfurt:
            return ".F";
            break;
        case Exchange::Xetra:
            return ".DE";
            break;
        case Exchange::Stuttgart:
            return ".SG";
            break;
    }

    return ".DE"; //say Xetra is the default one you use (since it used the default .DE)
}

/**/
size_t callback(char* data, size_t size, size_t numberOfElements, void* userdata){
    std::cout.write(data, size*numberOfElements);
    if(COLLECT_DATA){
        std::ofstream dataCollector("C:/Users/flori/Downloads/programs/C++/trading_bot/collected_data/collected.json");
        dataCollector.write(data, size*numberOfElements);
        dataCollector.close();
    }
    return size * numberOfElements;
}


int main(){

    if(!IS_TEST_RUN || COLLECT_DATA){
        curl_global_init(CURL_GLOBAL_DEFAULT);
        CURL* curl = curl_easy_init();
        if(curl == nullptr) {
            std::cout << "could not open curl client" << std::endl;
            return 1;
        }

        //give it its certification
        curl_easy_setopt(curl, CURLOPT_CAINFO, secrets::CURL_AC_CERT_PATH);

        /*
        std::string ticker = "BK1"; //Deutsche Bank AG stock (the stock i want to view)
        Exchange market = Exchange::Xetra;
        */
        
        std::string url =
            "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=AAPL&apikey="
            +std::string(secrets::ALPHA_VANTAGE_API_KEY);
        ;
            /*"https://finnhub.io/api/v1/quote?symbol="
            + ticker + getSuffix(&market)
            +"&token="+secrets::FINNHUB_API_KEY
        ;*/

        //std::cout << "URL: >>>" << url << "<<<" << std::endl;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); //tell curl to ask finnhub about the wanted stock's last transaction
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback); //TODO: chnage the callback method to save and go through a json decoder

        //perform request
        CURLcode result = curl_easy_perform(curl);

        long responseCode;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);

        if(result != CURLE_OK){
            std::cout << "request failed:\n";
            std::cout << curl_easy_strerror(result) << "\n";
        }else std::cout << "request success!\n";

        std::cout << "HTTP Status: " << responseCode << std::endl;

        

        //cleanup all curl memory
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }else if(IS_TEST_RUN){
        std::cout << "bridged curl call due to test run" << std::endl;
    }

    return 0;
}