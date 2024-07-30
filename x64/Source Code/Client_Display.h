#pragma once
#include "Tools.h"
#include "Capture.h"
#include <queue>
#include "Display.h"

cv::Mat receiveMatFromSocket(SOCKET serverSocket) {
    int totalSize = 0;

    // Nhận kích thước dữ liệu
    if (recv(serverSocket, (char*)&totalSize, sizeof(totalSize), 0) == SOCKET_ERROR) {
        // Xử lý lỗi khi nhận kích thước dữ liệu
        return cv::Mat(); // Trả về một ma trận rỗng
    }

    std::vector<uchar> buffer;
    buffer.reserve(totalSize);

    int bytesReceived = 0;
    while (bytesReceived < totalSize) {
        int remaining = totalSize - bytesReceived;
        int chunkSize = remaining > 4096 ? 4096 : remaining; // Kích thước chunk mỗi lần nhận

        std::vector<uchar> tempBuffer(chunkSize);
        int received = recv(serverSocket, reinterpret_cast<char*>(tempBuffer.data()), chunkSize, 0);

        if (received <= 0) {
            // Xử lý lỗi hoặc kết thúc kết nối
            return cv::Mat(); // Trả về một ma trận rỗng
        }

        bytesReceived += received;
        buffer.insert(buffer.end(), tempBuffer.begin(), tempBuffer.begin() + received);
    }

    cv::Mat image = cv::imdecode(buffer, cv::IMREAD_COLOR); // Tạo ma trận hình ảnh từ dữ liệu nhận được

    return image; // Trả về ma trận hình ảnh đã nhận
}

cv::Mat receivedImage;

bool TheOne(sf::Event event)
{
    switch (event.type) {
    case sf::Event::KeyPressed: return 1;
    case sf::Event::KeyReleased: return 1;
    case sf::Event::MouseButtonPressed: return 1;
    case sf::Event::MouseButtonReleased: return 1;
    case sf::Event::MouseMoved: return 1;
    case sf::Event::MouseWheelScrolled: return 1;


    default: return 0; // Giá trị mặc định nếu không có ánh xạ
    }
    return 0;
}

int Recv_Screen(SOCKET serverSocket, char* addr)
{
    sf::RenderWindow window(sf::VideoMode(1920 * resize / 100, 1080 * resize / 100), "Partner PC");
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock.\n";
        return 1;
    }

    std::cerr << "Try to connect to port 312!\n";
    sf::TcpSocket socket;
    if (socket.connect(addr, 312) != sf::Socket::Done) {
        std::cerr << "Failed to connect to server.\n";
        WSACleanup();
        return 1;
    }

    std::cerr << "Connected!\n";

    std::queue<sf::Event> qu;

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (TheOne(event)) {
                qu.push(event);
            }
        }

        int numberToSend = qu.size();

        // Chuyển đổi số nguyên thành chuỗi ký tự
        std::string numberAsString = std::to_string(numberToSend);

        // Gửi dữ liệu qua socket
        if (socket.send(numberAsString.c_str(), numberAsString.size() + 1) != sf::Socket::Done) {
            std::cerr << "Error to send event!" << std::endl;
        }
        else {
            std::cout << "Sending number of event successfully : " << numberToSend << std::endl;
        }

        while (!qu.empty())
        {
            sf::Event event = qu.front();
            qu.pop();

            if (socket.send(&event, sizeof(event)) != sf::Socket::Done) {
                std::cerr << "Error sending event to server\n";
            }
        }



        window.clear();

        receivedImage = receiveMatFromSocket(serverSocket);

        //  std::cerr << "Image Received!\n";

        sf::Image image = matToImage(receivedImage);
        sf::Texture texture = imageToTexture(image);
        sf::Sprite sprite(texture);
        window.draw(sprite);
        window.display();
    }

    socket.disconnect();
    WSACleanup();

    return 1;
}

int Client(sf::RenderWindow& window, char* SERVER_IP_ADDRESS)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return 1;
    }

    // Create a socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create a socket." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345); // Sử dụng cùng một cổng (port 12345)

    // Connect to the server
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);
    if (connect(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect to the server." << std::endl;
        return 1;
    }
    else
    {      
        std::cout << "Connected to the server!" << std::endl;
    }

    //Receive data

    bool ClientConnected = true;
    // std::thread THREAD_SEND(Send_Event, std::ref(window), SERVER_IP_ADDRESS, std::ref(ClientConnected));
     //THREAD_SEND.detach();
    
    Recv_Screen(serverSocket, SERVER_IP_ADDRESS);

    // Close the socket
    closesocket(serverSocket);

    ClientConnected = false;
    WSACleanup();

    return 0;
}
