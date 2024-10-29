#define LSS_Baud		115200
#define LSS_ID_Left		1
#define LSS_ID_Right	0

#define LSS_State_Duration_Pause		1000
#define LSS_State_Duration_Forward		3000
#define LSS_State_Duration_TurnLeft		5000
#define LSS_State_Duration_TurnRight	5000
#define LSS_State_Duration_Backward		3000

typedef enum LSS_state
{
	LSS_State_Nothing,
	LSS_State_InitStuff,
	LSS_State_Pause,
	LSS_State_Forward,
	LSS_State_TurnLeft,
	LSS_State_TurnRight,
	LSS_State_Backward,
	
	last
} LSS_state_t;

uint8_t LSS_State_SetTime(void);
uint8_t LSS_State_CheckTime(void);
uint8_t LSS_State_ResetTime(void);

void setup()
{
	Serial.begin(LSS_Baud);
}

// Variables
LSS_state_t state = LSS_State_InitStuff;
LSS_state_t newState = LSS_State_InitStuff;
uint8_t timeStartSet = 0;
uint32_t timeNow = 0;
uint32_t timeDuration = 0;
uint32_t timeStart = 0;

void loop()
{
	// Update state & time
	state = newState;
	timeNow = millis();

	// Perform action based on state
	switch(state)
	{
		case LSS_State_Nothing:
		{
			// Do nothing
			break;
		}
		case LSS_State_InitStuff:
		{
			// Move to next step
			LSS_State_ResetTime();
			newState = LSS_State_Forward;
			timeDuration = LSS_State_Duration_Forward;
			break;
		}
		
		case LSS_State_Forward:
		{
			// Check if task was started
			if(LSS_State_SetTime())
			{
				// Check if duration is expired
				if(LSS_State_CheckTime())
				{
					// Move to next step
					LSS_State_ResetTime();
					newState = LSS_State_TurnLeft;
					timeDuration = LSS_State_Duration_TurnLeft;
				}
				else
				{
					// Nothing to do
				}
			}
			else
			{
				// Perform task (first part)
				Serial.print("#255 MD900 T2000\r");
			}
			break;
		}
		
		case LSS_State_TurnLeft:
		{
			// Check if task was started
			if(LSS_State_SetTime())
			{
				// Check if duration is expired
				if(LSS_State_CheckTime())
				{
					// Move to next step
					LSS_State_ResetTime();
					newState = LSS_State_TurnRight;
					timeDuration = LSS_State_Duration_TurnRight;
				}
				else
				{
					// Nothing to do
				}
			}
			else
			{
				// Perform task (first part)
				Serial.print("#"); Serial.print(LSS_ID_Left, DEC); Serial.print(" MD-900 T3000\r");
				Serial.print("#"); Serial.print(LSS_ID_Right, DEC); Serial.print(" MD900 T3000\r");
			}
			break;
		}
		
		case LSS_State_TurnRight:
		{
			// Check if task was started
			if(LSS_State_SetTime())
			{
				// Check if duration is expired
				if(LSS_State_CheckTime())
				{
					// Move to next step
					LSS_State_ResetTime();
					newState = LSS_State_Backward;
					timeDuration = LSS_State_Duration_Backward;
				}
				else
				{
					// Nothing to do
				}
			}
			else
			{
				// Perform task (first part)
				Serial.print("#"); Serial.print(LSS_ID_Left, DEC); Serial.print(" MD900 T3000\r");
				Serial.print("#"); Serial.print(LSS_ID_Right, DEC); Serial.print(" MD-900 T3000\r");
			}
			break;
		}
		
		case LSS_State_Backward:
		{
			// Check if task was started
			if(LSS_State_SetTime())
			{
				// Check if duration is expired
				if(LSS_State_CheckTime())
				{
					// Move to next step
					LSS_State_ResetTime();
					newState = LSS_State_Pause;
					timeDuration = LSS_State_Duration_Pause;
				}
				else
				{
					// Nothing to do
				}
			}
			else
			{
				// Perform task (first part)
				Serial.print("#255 MD-900 T2000\r");
			}
			break;
		}
		
		case LSS_State_Pause:
		{
			// Check if task was started
			if(LSS_State_SetTime())
			{
				// Check if duration is expired
				if(LSS_State_CheckTime())
				{
					// Move to next step
					LSS_State_ResetTime();
					newState = LSS_State_Forward;
					timeDuration = LSS_State_Duration_Forward;
				}
				else
				{
					// Nothing to do
				}
			}
			else
			{
					// Nothing to do
			}
			break;
		}
	}
}

uint8_t LSS_State_SetTime(void)
{
	if(timeStartSet)
	{
		return(1);
	}
	else
	{
		// Set starting time
		timeStart = timeNow;
		timeStartSet = 1;
		return(0);
	}
}

uint8_t LSS_State_CheckTime(void)
{
	if(timeStartSet)
	{
		// Check if time elapsed
		return(timeNow > (timeStart + timeDuration));
	}
	else
	{
		// Start time not set, return false
		return(0);
	}
}

uint8_t LSS_State_ResetTime(void)
{
	timeStart = 0;
	timeStartSet = 0;
	timeDuration = 0;
}
