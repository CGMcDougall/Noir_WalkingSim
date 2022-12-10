COMP 3501 Walking Sim -- Noir Walking Sim

Authors: 
	Connor McDougall
	Carter 


Compiling Instructions:

	Two libraries have been added to this project, and will need to be linked in order for the project to function. The Two libraris, alut and OpenAl32, have been included in a zip file named Libs. 
	Change the path.h file and the library path in cmakeList.txt file and generate. From there you should be able to run it.  

Scene:
	
	The Scene features quite a few object:

	On an enclosed road, buidlings line eihter side
		There are 4 types of buildings	
		These buildings have colision detection
	Ahead and between two building on the right is a car in an alley	
	Periodically on both sides of the street there are street lights
		They are light sources for the scene

	Attached to the player model are two things
		A Cigarette
			Tracks the player
			Has a smoke particle effect 
		Rain effect
			Particle effect that tracks the player	

	Ahead and to the right there is a tree blowing in the wind
	
a

	While audio does function, it requires the inclusion of two audio libs
		alut and OpenAL32
					
	
