[
	{
		"name": "keyboard",
		"units": [
			{
				"type":"keyboard",
				"binds":
				[
					  {"action":"enable_tweak", "key":"Tab"}
					, {"action":"force", "key":"Space"}
					, {"action":"yaw+", "key":"Q"}
					, {"action":"yaw-", "key":"E"}
					, {"action":"pitch+", "key":"S"}
					, {"action":"pitch-", "key":"W"}
					, {"action":"roll+","key":"A"}
					, {"action":"roll-","key":"D"}
					, {"action":"reset","key":"R"}
					, {"action":"quit", "key":"Escape"}
				]
			},
			{
				"type":"mouse",
				"binds":
				[
					  {"action":"camright", "key":"MouseX+"}
					, {"action":"camleft", "key":"MouseX-"}
					, {"action":"camup", "key":"MouseY-"}
					, {"action":"camdown", "key":"MouseY+"}
				]
			}
		]
	}
	,{
		"name": "joystick"
		, "units": [
			{
				"type": "joystick",
				"binds":
				[
					  {"action":"force", "key":"JoystickZ", "device":0, "invert":true}
					, {"action":"yaw+", "key":"JoystickR+", "device":0}
					, {"action":"yaw-", "key":"JoystickR-", "device":0}
					, {"action":"pitch+", "key":"JoystickY-", "device":0}
					, {"action":"pitch-", "key":"JoystickY+", "device":0}
					, {"action":"roll+","key":"JoystickX-", "device":0}
					, {"action":"roll-","key":"JoystickX+", "device":0}
					, {"action":"camright", "key":"JoystickPovX+", "device":0}
					, {"action":"camleft", "key":"JoystickPovX-", "device":0}
					, {"action":"camup", "key":"JoystickPovY+", "device":0}
					, {"action":"camdown", "key":"JoystickPovY-", "device":0}
				]
			},
			{
				"type": "keyboard",
				"binds":
				[
					  {"action":"reset","key":"R"}
					, {"action":"quit", "key":"Escape"}
					, {"action":"enable_tweak", "key":"Tab"}
				]
			}
		]
	}
	
	,{
		"name": "360"
		, "units": [
			{
				"type": "joystick",
				"binds":
				[
					  {"action":"force", "key":"JoystickU-", "invert":true}
					, {"action":"yaw+", "key":"JoystickZ-"}
					, {"action":"yaw-", "key":"JoystickZ+"}
					, {"action":"pitch+", "key":"JoystickY+"}
					, {"action":"pitch-", "key":"JoystickY-"}
					, {"action":"roll+","key":"JoystickX-"}
					, {"action":"roll-","key":"JoystickX+"}
					, {"action":"reset","key":"JoystickButton5"}
					, {"action":"camright", "key":"JoystickButton3"}
					, {"action":"camleft", "key":"JoystickButton4"}
					, {"action":"camup", "key":"JoystickButton2"}
					, {"action":"camdown", "key":"JoystickButton1"}
				]
			},
			{
				"type": "keyboard",
				"binds":
				[
					  {"action":"reset","key":"R"}
					, {"action":"quit", "key":"Escape"}
					, {"action":"enable_tweak", "key":"Tab"}
				]
			}
		]
	}
]