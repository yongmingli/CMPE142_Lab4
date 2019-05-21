//
// CMPE142 Lab4 
// Yongming Li
// 010215699
// May/2019
//
****** READ ME ******

IMPORTANT: This program is coded and tested only in Clion application. 
	   If your run this program in other environment, there is NO guarantee you would get the expected results. 

Questions
1. Provide the name of the github repository (including the owning account, eg “mlarkin2015/142-
assignment-4”)
	https://github.com/yongmingli/CMPE142_Lab4
	Note: It is a private repository. 

2. Provide instructions I need to follow to build your assignment.
	When running this program in Linux, I have encountered some unsolved technical difficulties, and therefore, this code can NOT fully run in Linux environment. This code, however, can be successfully run in Both Clion application and a online complier (paiza.io)

	For Clion: 
		Open Clion and create a project with C++ language (C++ 14 as default).
		Import the codes into main. (Copy and paste)
		Run

	For online complier (paiza.io):
		Open paisa.io website and choose C++ language (or go to https://paiza.io/en/languages/online-cpp-compiler)
		Click "Start Online C++ complier" (Free)
		Import the codes into main. (Copy and paste)
		Run

	Note: You may encounter a Timeout error in both Clion and paisa.io, just re-run the program until success. (See Error Analysis)

3. Comment on the distribution of how many times each customer got to eat. Was the distribution equal? Why, or why not?
	 No, the distribution is not equal. In the program, the two foods are producted randomly, and therefore, they may not be served to each customer equally according to their different needs. In this case, whatever kind of food is produced, it would be serve to a customer who wants to eat it.   
    
4. I did the project by myself.

Error Analysis:
	As I was debugging my codes for Linux, I noticed that when a pthread signal is sent out to each pthread, it may not be received in each pthread function successfully. So, the program can not continue. I tried all the solutions I could find online, and they required to go deep into 'make' function or deeper into the environment itself. So far, I am not capable of solving this problem. 
