#include <iostream>
#include <thread>
#include <mutex>
#include "../secrets/api_keys.hpp"

int main(){
    std::cout<<"hello world!"<<std::endl;

    std::cout<<"key: "<<secrets::SOME_API_KEY<<std::endl;

    return 0;
}