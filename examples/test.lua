function test_type(self, json)
	self.force = Json.Get(json, "force", 50)
end

function test_component(self, ctype)
	self.force = ctype.force
end

function test_step(self, entity, dt)
	local phys = Physics.Get(entity)
	--Physics.ApplyForce(phys, -100*mousey*self.force, 0, -100*mousex*self.force)
	if yawpkey > 0.5 then
		Physics.ApplyTorque(phys, 0, self.force, 0)
	end
	if yawnkey > 0.5 then
		Physics.ApplyTorque(phys, 0, -self.force, 0)
	end
	
	if pitchpkey > 0.5 then
		Physics.ApplyTorque(phys, self.force, 0, 0)
	end
	if pitchnkey > 0.5 then
		Physics.ApplyTorque(phys, -self.force, 0, 0)
	end
	
	if rollpkey > 0.5 then
		Physics.ApplyTorque(phys, 0, 0, self.force)
	end
	if rollnkey > 0.5 then
		Physics.ApplyTorque(phys, 0, 0, -self.force)
	end
	
	if forcekey > 0.5 then
		Physics.ApplyForce(phys, 0, self.force, 0)
	end
	if quitkey > 0.5 then
		Game.Quit()
	end
end