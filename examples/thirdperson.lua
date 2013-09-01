function thirdperson_type(self, json)
	self.distance = Json.Get(json, "distance", 50)
end

function thirdperson_component(self, ctype)
	self.distance = ctype.distance
	self.rotx = 0
	self.roty = 0
end

function thirdperson_step(self, entity, dt)
	self.rotx = self.rotx + (camright - camleft) * 0.1
	self.roty = self.roty + (camdown - camup) * 0.1
	local player = Names.FromName("Player")
	local pos = Entity.GetPosition(player)
	local rot = cquat.yawpitchroll(self.rotx, self.roty, 0)
	--rot = rot * Entity.GetRotation(player)
	Entity.SetRotation(entity, rot)
	Entity.SetPosition(entity, pos - rot:getz() * self.distance)
end