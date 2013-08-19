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
		local q = Physics.GetOrientation(phys)
		local a = q:gety()
		Physics.ApplyTorque(phys, a*self.force*yawpkey)
	end
	if yawnkey > 0.0 then
		local q = Physics.GetOrientation(phys)
		local a = q:gety()
		Physics.ApplyTorque(phys, a*-self.force*yawnkey)
	end
	
	if pitchpkey > 0.5 then
		local q = Physics.GetOrientation(phys)
		local a = q:getx()
		Physics.ApplyTorque(phys, a*self.force*pitchpkey)
	end
	if pitchnkey > 0.0 then
		local q = Physics.GetOrientation(phys)
		local a = q:getx()
		Physics.ApplyTorque(phys, a*-self.force*pitchnkey)
	end
	
	if rollpkey > 0.0 then
		local q = Physics.GetOrientation(phys)
		local a = q:getz()
		Physics.ApplyTorque(phys, a*self.force*rollpkey)
	end
	if rollnkey > 0.0 then
		local q = Physics.GetOrientation(phys)
		local a = q:getz()
		Physics.ApplyTorque(phys, a*-self.force*rollnkey)
	end
	
	if forcekey > 0.0 then
		local q = Physics.GetOrientation(phys)
		local a = q:gety()
		Physics.ApplyForce(phys, a*self.force*forcekey)
	end
	
	if resetkey > 0.5 then
		Physics.SetOrientation(phys, cquat.identity())
	end
	
	if quitkey > 0.5 then
		Game.Quit()
	end
end