function test_type(self, json)
	self.force = Json.Get(json, "force", 5)
end

function test_component(self, ctype)
	self.force = ctype.force
end

function test_step(self, entity, dt)
	if forcekey > 0.5 then
		local phys = Physics.Get(entity)
		Physics.ApplyForce(phys, 0, self.force, 0)
	end
end