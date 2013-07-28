function test_type(self, json)
	self.force = Json.Get(json, "force", 5)
end

function test_component(self, ctype)
	self.force = ctype.force
end

function test_step(self, entity, dt)
	local phys = Physics.Get(entity)
	Physics.ApplyForce(phys, -100*mousey*self.force, 0, -100*mousex*self.force)
	if forcekey > 0.5 then
		Physics.ApplyForce(phys, 0, self.force, 0)
	end
	if quitkey > 0.5 then
		Game.Quit()
	end
end