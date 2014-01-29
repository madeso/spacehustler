[
	{
		"name": "keyboard",
		"units": [
			{
				"type":"keyboard",
				"binds":
				[
					  {"action":"enable_tweak", "key":"Tab"}
					, {"action":"force",        "key":"Space"}
					, {"action":"yaw+",         "key":"Q"}
					, {"action":"yaw-",         "key":"E"}
					, {"action":"pitch+",       "key":"S"}
					, {"action":"pitch-",       "key":"W"}
					, {"action":"roll+",        "key":"A"}
					, {"action":"roll-",        "key":"D"}
					, {"action":"reset",        "key":"R"}
					, {"action":"quit",         "key":"Escape"}
				]
			},
			{
				"type":"mouse",
				"binds":
				[
					  {"action":"camhorizontal", "type":"axis", "sign":"+", "axis":"x"}
					, {"action":"camleft",  "type":"axis", "sign":"-", "axis":"x"}
					, {"action":"camvertical",    "type":"axis", "sign":"-", "axis":"y"}
					, {"action":"camdown",  "type":"axis", "sign":"+", "axis":"y"}
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
					, {"action":"camhorizontal", "sign": "+", "axis":"x", "type":"hat", "hat":0}
					, {"action":"camleft",  "sign": "-", "axis":"x", "type":"hat", "hat":0}
					, {"action":"camvertical",    "sign": "+", "axis":"y", "type":"hat", "hat":0}
					, {"action":"camdown",  "sign": "-", "axis":"y", "type":"hat", "hat":0}
				]
			},
			{
				"type": "keyboard",
				"binds":
				[
					  {"action":"reset",        "key":"R"}
					, {"action":"quit",         "key":"Escape"}
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
					, {"action":"camvertical",    "type":"button", "button":2}
					, {"action":"camdown",  "type":"button", "button":1}
				]
			},
			{
				"type": "keyboard",
				"binds":
				[
					  {"action":"reset",        "key":"R"}
					, {"action":"quit",         "key":"Escape"}
					, {"action":"enable_tweak", "key":"Tab"}
				]
			}
		]
	}
]