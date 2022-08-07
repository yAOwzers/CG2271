# CG2271
Real-Time Operating Systems (Year 2 Semester 1, 2020)

This projects is to create a robot that is able to manage multiple threads while navigating a race course. The goal is to simulate a real-life situation of a test driving car, where there are multiple features that has to be handled by the microcontroller, but prioritizing the main threads such as movement of the car in the event of a sudden stop (to avoid an obstacle for example).

## Learning points:
1. Different ways of handling communications with the memory to avoid conflicts during the sharing of resources 
  -  Some examples would be Mutex (Mutually Exclusive), Semaphores, MessageManagers, Flags
2. Deadlocking, Priority Invesion 
  - Cases where Priotization might affect the functionality of the microcontroller
  
## Hardware and Software
1. FRDM-KL25Z Freedom board
2. Utilized uVision IDE Keil, coded in C.
3. Coded a simple UI for movement for Bluetooth communication with the Robot

## Image used in UI
<img src="https://imgur.com/AruRSrV.jpg" />
