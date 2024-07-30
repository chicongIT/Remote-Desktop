#pragma once
#include <SFML/Graphics.hpp>
#include <thread>
#include "Header.h"
#include "Capture.h"
#include "Client_Display.h"
#include "Server_Display.h"
#include "Tools.h"
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

bool isButtonClicked_Server = false;
bool isButtonClicked_Client = false;
bool vip = false;
bool Server_On = false;
bool Server_Connected = false;
bool Client_On = false;
bool Client_Connected = false;
bool Have_Port = false;
bool isButtonClicked_Open_Server = false;

bool DisplayStatus = true; // true : Server    | false : Client
bool is_Done = 0;
bool Port_On = false;
string bg = "Resources/Image/bg1.png";



void Server_cornectted_display(sf::RenderWindow& window, sf::Event event, bool &button_connected)
{

    
   
    window.clear();
    Draw_Background(window, "Resources/Image/Cornectted_bg.png");
    bool isClickedDiscornect = false;
    Draw_Button(window, event, isClickedDiscornect, 400, 370, 6);
    Draw_Button(window, event, vip, 25, 593, 7);
    if (isClickedDiscornect)
    {
        button_connected = false;
        Server_On = false;
 
         
    }               

}

std::string GetLastIPAddress() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock\n";
        return "";
    }

    PIP_ADAPTER_INFO pAdapterInfo = nullptr;
    ULONG buflen = sizeof(IP_ADAPTER_INFO);

    pAdapterInfo = (IP_ADAPTER_INFO*)malloc(buflen);
    if (pAdapterInfo == nullptr) {
        std::cerr << "Memory allocation failure\n";
        return "";
    }

    if (GetAdaptersInfo(pAdapterInfo, &buflen) == ERROR_BUFFER_OVERFLOW) {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO*)malloc(buflen);
        if (pAdapterInfo == nullptr) {
            std::cerr << "Memory allocation failure\n";
            return "";
        }
    }

    if (GetAdaptersInfo(pAdapterInfo, &buflen) == NO_ERROR) {
        for (PIP_ADAPTER_INFO pAdapter = pAdapterInfo; pAdapter != nullptr; pAdapter = pAdapter->Next) {
            if (strstr(pAdapter->Description, "Wi-Fi") != nullptr || strstr(pAdapter->Description, "WiFi") != nullptr) {
                return pAdapter->IpAddressList.IpAddress.String;
            }
        }
    }
    else {
        std::cerr << "GetAdaptersInfo failed\n";
    }

    free(pAdapterInfo);
    WSACleanup();

    return "";
}


void Normal_Mode(sf::RenderWindow& window, sf::Event event)
{
    sf::Font font;
    if (!font.loadFromFile("Resources/Fonts/arial.ttf")) {
        std::cerr << "Error loading font\n";
        return;
    }

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setPosition(620, 278);
    text.setFillColor(sf::Color::Black); // Đặt màu cho văn bản
    text.setString("");

    std::string ipAddress;

    bool is_Done = 0;
    // Lấy địa chỉ IP của mạng
    sf::IpAddress publicIpAddress = sf::IpAddress::getPublicAddress();


    // In địa chỉ IP
    std::string ipAddressString = GetLastIPAddress();


    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.text.unicode == 13)
            {
                window.clear();
                is_Done = 1;
                break;
            }
            else
                if (event.type == sf::Event::TextEntered) {
                    if (event.text.unicode < 128 && event.text.unicode != 8) {
                        // Kiểm tra ký tự nhập vào, loại bỏ ký tự backspace (8)
                        ipAddress += static_cast<char>(event.text.unicode);
                    }
                    else if (event.text.unicode == 8 && !ipAddress.empty()) {
                        // Xóa ký tự cuối cùng nếu là backspace và xâu không rỗng
                        ipAddress.pop_back();
                    }
                }

            window.clear();

            //std::cerr << is_Done << '\n';
            if (ipAddress == "")
                bg = "Resources/Image/bg1.png";
            else
                bg = "Resources/Image/bg2.png";
                    
               
            Draw_Background(window, bg);
            Draw_Text(window, ipAddressString, 30, sf::Color::Black, 80, 320);
            //   2 button client & server
            //Draw_Button(window, event, isButtonClicked_Server, 600, 100, 1);
            Draw_Button(window, event, is_Done, 625, 337, 3);
            int tam;
            
            if (Server_On)
                tam = 5;
            else
            {
                tam = 4; 
                Have_Port = 0;
          
            }
                     
            isButtonClicked_Server = false;
            Draw_Button(window, event, isButtonClicked_Server, 110, 337, tam);
            if (isButtonClicked_Server)
                Server_On = (Server_On == true)?false:true;
            if (Server_On && !Server_Connected && !Have_Port)
            {
                std::thread THREAD_S(Server, std::ref(window), std::ref(Server_Connected));
                THREAD_S.detach();
                // Server(window, Server_Connected);
                Server_On = 1;
                Have_Port = 1;
            }
               

            
            // Hiển thị IP đã nhập
            text.setString(ipAddress);

            window.draw(text);
            if (Server_Connected)
            {
                Server_cornectted_display(window, event, Server_Connected);
            } 
            
            window.display();

            if (is_Done)
                break;
        }
        if (is_Done)
            break;
    }
    // Chuyển đổi std::string sang char*
    char* ipAddressChar = new char[16];
    for (int i = 0; i < 16; ++i)ipAddressChar[i] = '\0';
    std::strcpy(ipAddressChar, ipAddress.c_str());

    window.clear();

    printf("%s\n", ipAddressChar);
    
    Client(window, ipAddressChar );

    delete[] ipAddressChar;

    Client_On = 0;

    return;
}

void Main_Display_Screen()
{
    sf::RenderWindow window(sf::VideoMode(1000, 616), "Remote Desktop Client"); // Resolution 1920x1080
    sf::View view(sf::FloatRect(0, 0, 1000, 616));
    window.setView(view);

    while (window.isOpen()) {      

        window.clear();       
        sf::Event event = Get_Event(window);
        Normal_Mode(window, event);
       
        window.display();
    }
}