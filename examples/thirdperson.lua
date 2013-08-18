function thirdperson_type(self, json)
	self.force = Json.Get(json, "force", 50)
end

function thirdperson_component(self, ctype)
	self.force = ctype.force
end

function thirdperson_step(self, entity, dt)
	local player = Names.FromName("Player")
	local playerpos = Entity.GetPosition(player)
	local playerrot = Entity.GetRotation(player)
	Entity.SetRotation(entity, -playerrot)
	Entity.SetPosition(entity, playerpos + playerrot:getz() * 50)
end