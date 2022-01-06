# RobotC-SelfParkingRobot
LEGO EV3 Color Detecting and Self-Parking Robot 



Introduction
During the winter, the roads can quickly become slippery due to snow and ice. When people are getting in or out of their car, they can slip and fall, resulting in significant injury, making this a substantial safety concern. This issue is particularly prevalent in parking lots, as many people exit their vehicles and walk away. To solve this issue, we decided to build an EV3 Lego robot that could shovel snow in a parking lot and park itself in an open spot afterwards.


Description of software design


The overall program was broken up into smaller sections by creating a function for each of the main actions the robot takes while executing this program (Refer to Appendix A for all the functions and main code).
 
 
 The main actions the robot must do for the program to successfully run are:  


1.	Configure all the sensors:
-	Void function (returns nothing)
-	Used parameters: tSensors ultrasonicport, tSensors colorport, tSensors gyroport, tSensors touchport
-	Ultrasonic sensor: must be configured to be able to detect a free parking spot/ to check if a care is currently parked in a specific spot. 
-	Color sensor: must be configured to be able to detect and move along the green line for parking spots. 
-	Gyro sensor: must be configured so that, after the robot shovels the snow, it is able to turn 180 degrees, and then look for a free parking spot. 
-	Touch sensor: must be configured so the robot knows when it is parked, as in, once the touch sensor is activated (returns 1 or true), it means the shovel has made contact with the curb at the end of a parking spot, and so the robot has successfully parked itself.


2.	Following the line:
-	Void function (returns nothing)
-	Used parameters: tSensors colorport
-	Uses the color sensor (configured in the first function) to detect the amount of reflected light when following the green line.
-	Decisions: the robot is able to follow the line by differentiating whether it is detecting the green tape (path of the robot) or the white paper (the ground of the parking lot).
-	When detecting the green line, the color sensor will return a reflected light value between 5 and 18, and when detecting the white parking lot, the reflecting light will be greater than 18. Therefore, we programmed our robot to turn slightly counterclockwise whenever it is detecting a reflected light value between 5 and 18, and then turn slightly clockwise when detecting a reflected light value greater than 18. The robot will continue to do this until it detects a reflected light value of less than 5 indicating that it has detected the black tape at the end of the path.  


3.	Rotating the robot:
-	Void function (returns nothing) 
-	Used parameters: tSensors gyroport, int angle (constant), int motorpower (constant)
-	Decisions: checks if angle (constant of 180 degrees) is less than 0, then rotate counterclockwise if the angle is less than 0, or clockwise if the angle is greater than 0. 
-	The robot will keep turning while the gyro sensor returns a value that is less than the absolute value of the angle that was inputted into the function.  
-	Once robot reaches the end of the parking lot, it will use the gyro sensor to turn 180 degrees and start detecting for an open parking spot. 


4.	Check for free space:
-	Boolean function (returns true or false)
-	Used parameters: tSensors ultrasonicport, int motorpower (constant) 
-	Uses the ultrasonic sensor (configured in the first function) to detect distance from the robot to the closest surface in the direction of the ultrasonic sensor.
-	Given the robot is in the center of the parking lot and distance between each adjacent parking spot is 40 cm, the robot will move forward 40 cm, turn the ultrasonic sensor 90 degrees clockwise (to check if there is a car on the right side) and then 180 degrees counterclockwise (to check if there is a car on the left side). The values detected from the ultrasonic sensor will be stores in variables called obst1 and obst2 respectively. 
-	If the values of obst1 and obst2 are both less than 40, that means there are cars parked in both spots, and the robot will repeat the steps in the previous point. However, if either obst1 or obst2 is greater than 40, the robot will consider that parking spot to be free.  
-	Decisions: if obst1 is greater than 90, the function will return true else the function will return false. This return will be used in the park function and will help the robot determine if it should turn left or right. 


5.	Parking the robot after detecting free space:
-	Void function (returns nothing)
-	Used parameters: tSensors gyroport,int motorpower
-	This function will call the checkAvailability() function and use the return from that function to decide if the robot will turn into the left parking space or the right parking space and then call the rotateRobot() function to actually rotate the robot.
-	If the return from the checkAvailability() function is true, the will turn 90 clockwise, else if it is false the robot will turn 90 degrees counterclockwise.


6.	Checks if the robot successfully parked:
-	Boolean function (returns true or false)
-	Parameters used: tSensors touchport
-	The robot will drive into the parking spot until the shovel hits the curb.
