# Remote Desktop

## Introduction

 Here is our source code for a Remote Desktop program - a program that allows you to control a computer from another computer on the same WiFi network or LAN.

<p align="center">
  This is our demo video
</p>

<div style="display: flex; justify-content: space-around;">
<p align="center">
  <img src="./Video Demo/Video Demo/Phía-Client.gif" width=600><br/>
  <i>Client</i>
</p>

<p align="center">
  <img src="./Video Demo/Video Demo/Phía-Server.gif" width=600><br/>
  <i>Server</i>
</p>

 </div>

## Programming language, Libraries, Frameworks
**Languages:**
* C++

**Libraries:**
* winsock2: Establishes connection between two computers, transmits data (screenshot images, mouse events, keyboard events)
* opencv2: Processes images, captures screenshots, resizes images
* SFML: Displays images in windows, handles keyboard and mouse events
* Thread: Creates new threads to open the server

## How to use my program

**To open the program:**
Go to x64 -> Release -> Run the Remote Desktop.exe file

**Note:** 
* The program only works when both computers are using the same WiFi network. Disable all unrelated networks except the WiFi network shared with the other computer to ensure stability.
* We have deleted the file 'Remote Desktop\x64\Release\opencv_world480d.dll' because its large size prevents it from being uploaded to GitHub. You can download it from the internet. We apologize for this inconvenience.

**To connect, follow these steps:**

* Server (the controlled computer): Click the "Open Server" button.
* Client (the controlling computer): Enter the Server's IP address and click "CONNECT NOW" (or press the Enter key).

## About our team
We are a group of students from the University of Science consisting of 3 members:
* **Nguyen Chi Cong - me**
* **Ho Dinh Hoang**
* **Phan Van Phuc** 


 











