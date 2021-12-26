// declaring global constants
const int REF_LIGHT_GREEN = 18;
const int REF_LIGHT_BLACK = 5;
const int MIN_SPACE_TO_PARK = 40;
const int ENC_FACTOR = 180/(PI*2.75);
const int DIST_BETWEEN_PARKING = 40;

//function to configure sensors
void configureSensors (tSensors ultrasonicport, tSensors colorport,
tSensors gyroport, tSensors touchport)
{
	SensorType[ultrasonicport] = sensorEV3_Ultrasonic;
	wait1Msec(50);
	SensorType[touchport] = sensorEV3_Touch;
	wait1Msec(50);
	SensorType[colorport] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode [colorport] = modeEV3Color_Reflected;
	wait1Msec(50);
	SensorType[gyroport] = sensorEV3_Gyro;
	wait1Msec(50);
	SensorMode[gyroport]= modeEV3Gyro_Calibration;
	wait1Msec(50);
	SensorMode[gyroport]= modeEV3Gyro_RateAndAngle;
	wait1Msec(50);
}

//function to follow the green line
void followLine(tSensors colorport)
{
	bool trackline = true;
	while (trackline== true)
	{
		if (SensorValue[colorport] < REF_LIGHT_GREEN)
		{
			motor[motorA] = 30;
			motor[motorD] = 5;
			wait1Msec(5);
		}

		else
		{
			motor[motorA] = -5;
			motor[motorD] = 20;
			wait1Msec(50);
		}

		if (SensorValue[colorport] < REF_LIGHT_BLACK)
		{
			trackline = false;
		}
		wait1Msec(50);
	}

}

//function to rotate the robot
void rotateRobot(tSensors gyroport, int angle, int motorpower)
{
	if (angle < 0)
	{
		motor[motorA] = motorpower;
		motor[motorD] = -motorpower;
	}
	else
	{
		motor[motorA] = -motorpower;
		motor[motorD] = motorpower;
	}
	angle = abs(angle);

	while (abs((getGyroDegrees(gyroport)))<angle)
	{}
	motor[motorA]= motor[motorD] = 0;
}

// function to find an available parking spot and return if it is on the right or left side
bool checkAvailability(tSensors ultrasonicport, int motorpower)
{
	float obst1 = 0;
	float obst2 = 0;

	while (obst1 < MIN_SPACE_TO_PARK && obst2 < MIN_SPACE_TO_PARK)
	{
		nMotorEncoder[motorA] = 0;
		motor[motorA]= motor[motorD] = 20;
		float dist_to_drive = (DIST_BETWEEN_PARKING*ENC_FACTOR);
		while (nMotorEncoder [motorA] < dist_to_drive)
		{}
		motor[motorA]= motor[motorD] =0;

		nMotorEncoder[motorB]=0;
		motor[motorB]= 10;
		while (nMotorEncoder [motorB] < (90))
		{}
		motor[motorB]= 0;
		wait1Msec(500);
		obst1 = SensorValue[S2];
		wait1Msec(500);
		nMotorEncoder[motorB]=0;
		motor[motorB]= -10;
		while (nMotorEncoder [motorB] > (-180))
		{}
		motor[motorB]= 0;
		wait1Msec(500);
		obst2 = SensorValue[S2];
		wait1Msec(200);
		nMotorEncoder[motorB]=0;
		motor[motorB]= 10;
		while (nMotorEncoder [motorB] < (90))
		{}
		motor[motorB]= 0;
		wait1Msec(500);
	}

	if ( obst1 > 90 )
	{
		return true;
	}
	else
	{
		return false;
	}
}

/* function to turn the robot the appropriate direction depending on if the parking spot is on
the right or left side of the parking lot
*/
void park(tSensors gyroport,int motorpower)
{
	bool left = checkAvailability(S2,10);
	wait1Msec(50);
	SensorMode[gyroport]= modeEV3Gyro_Calibration;
	wait1Msec(50);
	if ( left == true)
	{
		rotateRobot(S1,90,10);
	}
	else
	{
		rotateRobot(S1,-90,10);
	}
}

//function to check if the robot has reached the end of the parking spot
bool parked(tSensors touchport)
{

	if(SensorValue[touchport] == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//function to display a message for ten seconds
void display ()
{

	displayString(7,"thank you for using the");
	displayString(8,"snow cleaning robot");
	time1 [T1] = 0;
	while (time1[T1] < 10000)
	{}
	eraseDisplay();
}


task main()
{
	configureSensors (S2,S3,S1,S4);

	while (!getButtonPress(buttonEnter))
	{}
	while (getButtonPress(buttonEnter))
	{}
	followLine(S3);
	rotateRobot(S1,180,20);
	wait1Msec(2000);

	park(S1,30);
	parked(S4);
	motor[motorA] = motor[motorD] = 30;
	while ( parked(S4) == false)
	{
		parked(S4);
	}
	motor[motorA] = motor[motorD] = 0;
	display ();

}
