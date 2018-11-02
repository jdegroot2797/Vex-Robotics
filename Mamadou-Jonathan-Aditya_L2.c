#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    lineFollowerRIGHT, sensorLineFollower)
#pragma config(Sensor, in2,    lineFollowerCENTER, sensorLineFollower)
#pragma config(Sensor, in3,    lineFollowerLEFT, sensorLineFollower)
#pragma config(Sensor, in4,    potentiometerSensor, sensorPotentiometer)
#pragma config(Sensor, dgtl8,  sonarSensor,    sensorSONAR_inch)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           rightMotor,    tmotorVex393_HBridge, PIDControl, encoderPort, I2C_1)
#pragma config(Motor,  port6,           clawMotor,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           armMotor,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          leftMotor,     tmotorVex393_HBridge, PIDControl, reversed, encoderPort, I2C_2)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*+++++++++++++++++++++++++++++++++++++++++++++| Notes |++++++++++++++++++++++++++++++++++++++++++++++
Forward for Target Distance

Robot Model(s): Modified Squarebot

[I/O Port]          [Name]              [Type]                [Description]
Motor Port 2        rightMotor          393 Motor             Right side motor, Reversed
Motor Port 3        leftMotor           393 Motor             Left side motor
I2C_1               rightIEM            Integrated Encoder    Encoder mounted on rightMotor
I2C_2               leftIEM             Integrated Encoder    Encoted mounted on leftMotor
----------------------------------------------------------------------------------------------------*/

	//directional variables for 'turnRobot' function
	const int left = 1;
	const int right = 0;

	//speed is the variable given for how much power and in turn speed is given to drive the robot
	int speed = 63;



	//light value for line trackers
  int threshold = 505;

	//1 full rotation = 12.56 inch = 1.04feet ( using 627 as target )
	const float rotation = 627;

	//Should move forward until a line is recognized
	/*void findLine()
	{
		if(SensorValue[lineTrackerCENTER] < 4000)
		{
	 		motor[leftMotor] = 63;
	 		motor[rightMotor] = 0;
	 	}
		else
		{
	 	 motor[leftMotor] = 0;
	 	 motor[rightMotor] = 0;
	  }
	}
	*/
	//follows line going at 1-arg power given until 2-arg distance
	//in inches is deteched by the sonar
	void followLineUntilObject(int power, int objDistance)
	{
	  /* surfaces, adding them together, then dividing by 2. */
	  while(SensorValue(sonarSensor) > 6)
	  {
	  	if(SensorValue(sonarSensor) < 16)
	  		power = power / 2;

	    // RIGHT sensor sees dark:
	    if(SensorValue(lineFollowerRIGHT) > threshold)
	    {
	      // counter-steer right:
	      motor[leftMotor]  = power;
	      motor[rightMotor] = 0;
	    }
	    // CENTER sensor sees dark:
	    if(SensorValue(lineFollowerCENTER) > threshold)
	    {
	      // go straight
	      motor[leftMotor]  = power;
	      motor[rightMotor] = power;
	    }
	    // LEFT sensor sees dark:
	    if(SensorValue(lineFollowerLEFT) > threshold)
	    {
	      // counter-steer left:
	      motor[leftMotor]  = 0;
	      motor[rightMotor] = power;
	    }
	    else
	    {
	    	motor[leftMotor]  = 0;
	      motor[rightMotor] = 0;
	    }
	  }
	}
  void raiseArm(int power)
	{
			//brief delay
			wait1Msec(2000);
			bool arm = true;
			int result = 0;
			while (arm)
			{
				//potentiometer value to stop arm movement must be higher due to
				//power delivered to the claw, momentum/gravity, and the weight of the object
				//will cause arm to cut off at potentiometer value but drift lower for a longer time
				if(SensorValue(potentiometerSensor) == 1600 || SensorValue(potentiometerSensor) > 1600)
				{
					arm = false;
					motor[armMotor] = 0;
				}
				else
				{

					motor[armMotor] = -power;
				}
			}
	}

	void openClaw()
	{
		wait1Msec(2000);
		motor[clawMotor] = 50;
		wait1Msec(1100);
	}

	void closeClaw()
	{
		wait1Msec(2000);
		motor[clawMotor] = -50;
		wait1Msec(1100);
	}

	void lowerArm(int power)
	{
			//brief delay
			wait1Msec(2000);
			bool arm = true;
			while (arm)
			{
				//potentiometer value to stop arm movement must be higher due to
				//power delivered to the claw, momentum/gravity, and the weight of the object
				//will cause arm to cut off at potentiometer value but drift lower for a longer time
				if(SensorValue(potentiometerSensor) == 1599 || SensorValue(potentiometerSensor) < 1599)
				{
					arm = false;
					motor[armMotor] = 0;
				}
				else
				{
					motor[armMotor] = 0;
					motor[armMotor] = power;
				}
			}
	}


	void forwardMovement(double target, int power)
	{
		resetMotorEncoder(rightMotor);
		resetMotorEncoder(leftMotor);

		//result holds number in feet
		double result = target * rotation;

		moveMotorTarget(rightMotor,(int)result,power,true);
		moveMotorTarget(leftMotor,(int)result,power,true);
		wait1Msec(10000);

		while((getMotorTargetCompleted(rightMotor) == false || getMotorTargetCompleted(leftMotor) == false)){}
	}

	// 90 degree turn = ~ 933 as target
	void turnRobot(double target, int power, int direction)
	{
		resetMotorEncoder(rightMotor);
		resetMotorEncoder(leftMotor);

		//result holds number in feet
		double result = target * rotation;

		//left turn
		if(direction == left)
		{
			moveMotorTarget(rightMotor,(int)result,power,true); //moveMotorTarget takes int for distance therefore the (int) casting
			moveMotorTarget(leftMotor,0,0,true);
			wait1Msec(10000);
		 	while((getMotorTargetCompleted(rightMotor) == false || getMotorTargetCompleted(leftMotor) == false)){}
		}

		//right turn
		else if(direction == right)
		{
			moveMotorTarget(rightMotor,0,0,true);
			moveMotorTarget(leftMotor,(int)result,power,true); //moveMotorTarget takes int for distance therefore the (int) casting
			wait1Msec(10000);
			while((getMotorTargetCompleted(rightMotor) == false || getMotorTargetCompleted(leftMotor) == false)){}
		}
	}

task main()
{
	wait1Msec(2000);
	openClaw();
	followLineUntilObject(15, 8);
	wait1Msec(4000);
	closeClaw();
	wait1Msec(2000);
	raiseArm(30);

	//motor[clawMotor] = -5;


	// || (or) it is '-1'.  (-1 is the value returned when nothing is in it's visable range)

			//motor[rightMotor] = 23;			// Motor on port2 is run at half (63) power forward
		//	motor[leftMotor]  = 23;			// Motor on port3 is run at half (63) power forward
	//	}
	//findObject();
	//findLine();

	//lowerArm(27);

  //forwardMovement(3.49, 63); //4ft straight
  /*turnRobot(1.68, 83, left); //2ft left turn

  forwardMovement(2.0, 63); //
  turnRobot(1.69, 83, left); //

  forwardMovement(2.75, 63);
  turnRobot(1.72, 83, right);
  forwardMovement(1.82,63);

*/
//	resetMotorEncoder(rightMotor);
//	resetMotorEncoder(leftMotor);
}
