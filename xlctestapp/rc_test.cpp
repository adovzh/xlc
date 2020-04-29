//
// Created by Alexander Dovzhikov on 18/4/20.
//

#include <iostream>
#include <remote_client.h>

int main()
{
    std::cout << "Remote Client Test" << std::endl;

    using namespace xlc;

    try {
        remote_client client;
        std::cout << "Remote client open: " << std::boolalpha << client.isOpen() << std::endl;

        std::future<bool> result_future = client.connect();
        try {
            bool result = result_future.get();
            std::cout << "Connect success: " << std::boolalpha << result << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Error connecting: " << e.what() << std::endl;
        }
        std::cout << "Remote client open: " << std::boolalpha << client.isOpen() << std::endl;

        std::future<bool> dc_future = client.disconnect();
        try {
            bool result = dc_future.get();
            std::cout << "Disconnect success: " << std::boolalpha << result << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Error disconnecting: " << e.what() << std::endl;
        }
        std::cout << "Remote client open: " << std::boolalpha << client.isOpen() << std::endl;
    }
    catch (std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "Unknown error!" << std::endl;
    }


    return 0;
}