[
	 { "name" : "craft",  "data": [
		 {"system": "Rendering",
			"data" : {"file": "fighter1.obj"}}
		,{"system": "Physics",
			"data" : {"mass":1, "width":25, "height":4, "depth":18}}
		,{"system": "test", "data": {"force": 50}}
		,{"system": "Name", "data": {"name":"Player"}}
		,{"system": "Player", "data": {"name":"Player"}}
	] }
	
	,{ "name" : "camera",  "data": [
		 {"system": "Camera",
			"data" : {"fov": 45, "near": 5, "far": 1500}}
		,{"system": "3rdperson", "data":{}}
		,{"system": "Player", "data": {"name":"Player"}}
	] }
]