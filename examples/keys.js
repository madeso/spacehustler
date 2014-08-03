[
	{
		"name": "keyboard",
		"units": [
			{
				"type":"keyboard",
				"binds":
				[
					  {"action":"enable_tweak", "type":"button", "key":"Tab"}
					, {"action":"yaw+",          "type":"button", "key":"Q"}
					, {"action":"yaw-",          "type":"button", "key":"E"}
					, {"action":"pitch+",        "type":"button", "key":"S"}
					, {"action":"pitch-",        "type":"button", "key":"W"}
					, {"action":"roll+",         "type":"button", "key":"A"}
					, {"action":"roll-",         "type":"button", "key":"D"}
					, {"action":"reset",        "type":"button", "key":"R"}
					, {"action":"quit",         "type":"button", "key":"Escape"}
				]
			},
			{
				"type":"mouse",
				"binds":
				[
					  {"action":"camhorizontal", "type":"axis", "sign":"+", "axis":"x", "invert":false, "scale":1.2}
					, {"action":"camvertical",    "type":"axis", "sign":"-", "axis":"y", "scale":1.2}
					, {"action":"force",        "type":"button", "key":"left"}
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
					  {"action":"force-axis",   "axis":2, "type":"axis", "invert":true}
					, {"action":"yaw",     "axis":3, "type":"axis", "invert": true}
					, {"action":"pitch",   "axis":1, "type":"axis"}
					, {"action":"roll",    "axis":0, "type":"axis", "invert": true}
					, {"action":"camhorizontal", "axis":"x", "type":"hat", "hat":0}
					, {"action":"camvertical", "axis":"y", "type":"hat", "hat":0}
				]
			},
			{
				"type": "keyboard",
				"binds":
				[
					  {"action":"reset",        "type":"button", "key":"R"}
					, {"action":"quit",         "type":"button", "key":"Escape"}
					, {"action":"enable_tweak", "type":"button", "key":"Tab"}
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
					  {"action":"force",    "type":"axis", "sign":"-", "axis":4, "invert":true}
					, {"action":"yaw+",     "type":"axis", "axis":2}
					, {"action":"pitch+",   "type":"axis", "axis":1}
					, {"action":"roll+",    "type":"axis", "axis":0}
					, {"action":"reset",    "type":"button", "button":5}
					, {"action":"camhorizontal+", "type":"button", "button":3}
					, {"action":"camhorizontal-", "type":"button", "button":4}
					, {"action":"camvertical+", "type":"button", "button":2}
					, {"action":"camvertical-", "type":"button", "button":1}
				]
			},
			{
				"type": "keyboard",
				"binds":
				[
					  {"action":"reset",        "type":"button", "key":"R"}
					, {"action":"quit",         "type":"button", "key":"Escape"}
					, {"action":"enable_tweak", "type":"button", "key":"Tab"}
				]
			}
		]
	}
]