# Key Listener  

Low-latency key-press & mouse-click listener modified from rustbell's keyasio using ASIO with [FMOD Core](https://www.fmod.com) \(Ver. 2.00.01\)  

## VK Code  
See [https://docs.microsoft.com/en-us/windows/desktop/inputdev/virtual-key-codes](https://docs.microsoft.com/en-us/windows/desktop/inputdev/virtual-key-codes)

## Compile  
Install MinGW and double click Compile.bat  

## Command Line  
Key_Listener.exe \[Buffer Size\] \[Use Key Listener\] \[Use Click Listener\] \[Key1 VK Code\] \[Key2 VK Code\] ...  

e.g. `Key_Listener.exe 64 1 0 68 70 74 75`

## Sound Files  
Create **Key\_[VK Code].wav** for each key, e.g. Key\_68.wav  
Create **Key\_Default.wav** for other keys  

Create **Click\_[L R M X].wav** for each button, e.g. Click\_L.wav  
Create **Click\_Default.wav** for other buttons  

## How to Use  
You need:   
​	2 audio outputs device, e.g. Onboard Audio with a USB Audio DAC  
​	2 headphones or speakers you can use at the same time  

Install ASIO4ALL   

Plug your headphones in  

In your windows audio control panel, set one of your audio output device as the default device for other programs  

Run Key_Listener.exe, select asio4all and then open the asio4all control panel, select your **Another** audio output device and deselect other devices (maybe you need to click the "Advanced Options" Botton on the bottom right corner first)  

Restart Key_Listener.exe and press keys  

If you don't hear anything, check the asio4all control panel and set a larger Buffer Size  