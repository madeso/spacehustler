[
	{
		"name": "keyboard",
		"units": [
			{
				"type":"keyboard",
				"binds":
				[
					  {"action":"enable_tweak", "type":"button", "key":"Tab"}
					, {"action":"force",        "type":"button", "key":"Space"}
					, {"action":"yaw+",         "type":"button", "key":"Q"}
					, {"action":"yaw-",         "type":"button", "key":"E"}
					, {"action":"pitch+",       "type":"button", "key":"S"}
					, {"action":"pitch-",       "type":"button", "key":"W"}
					, {"action":"roll",         "type":"axiskeys", "min":"D", "max":"A"}
					, {"action":"reset",        "type":"button", "key":"R"}
					, {"action":"quit",         "type":"button", "key":"Escape"}
				]
			},
			{
				"type":"mouse",
				"binds":
				[
					  {"action":"camhorizontal", "type":"axis", "sign":"+", "axis":"x"}
					, {"action":"camvertical",    "type":"axis", "sign":"-", "axis":"y"}
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
					  {"action":"force",    "sign": "",  "axis":2, "type":"axis", "invert":true}
					, {"action":"yaw+",     "sign": "+", "axis":3, "type":"axis"}
					, {"action":"yaw-",     "sign": "-", "axis":3, "type":"axis"}
					, {"action":"pitch+",   "sign": "-", "axis":1, "type":"axis"}
					, {"action":"pitch-",   "sign": "+", "axis":1, "type":"axis"}
					, {"action":"roll+",    "sign": "-", "axis":0, "type":"axis"}
					, {"action":"roll-",    "sign": "+", "axis":0, "type":"axis"}
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
					, {"action":"yaw+",     "type":"axis", "sign":"-", "axis":2}
					, {"action":"yaw-",     "type":"axis", "sign":"+", "axis":2}
					, {"action":"pitch+",   "type":"axis", "sign":"+", "axis":1}
					, {"action":"pitch-",   "type":"axis", "sign":"-", "axis":1}
					, {"action":"roll+",    "type":"axis", "sign":"-", "axis":0}
					, {"action":"roll-",    "type":"axis", "sign":"+", "axis":0}
					, {"action":"reset",    "type":"button", "button":5}
					, {"action":"camhorizontal", "type":"button", "button":3}
					, {"action":"camleft",  "type":"button", "button":4}
					, {"action":"camvertical", "type":"button", "button":2}
					, {"action":"camdown",  "type":"button", "button":1}
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