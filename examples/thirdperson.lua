function thirdperson_type(self, json)
	self.distance = Json.Get(json, "distance", 100)
end

function thirdperson_component(self, object, ctype)
	self.distance = ctype.distance
	self.rotx = 0
	self.roty = 0
end

function thirdperson_step(self, object, entity, dt)
	self.rotx = self.rotx + object.camhorizontal * 0.001
	self.roty = self.roty + object.camvertical * 0.001
	local player = Names.FromName("Player")
	local pos = Entity.GetPosition(player)
	local rot = cquat.yawpitchroll(-self.rotx, -self.roty, 0)
	--rot = rot * Entity.GetRotation(player)
	Entity.SetRotation(entity, rot)
	Entity.SetPosition(entity, pos + rot:getz() * self.distance)
end