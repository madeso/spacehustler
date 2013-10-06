function test_type(self, json)
	self.force = Json.Get(json, "force", 50)
end

function test_component(self, object, ctype)
	self.force = ctype.force
end

function test_step(self, object, entity, dt)
	local phys = Physics.Get(entity)
	--Physics.ApplyForce(phys, -100*mousey*self.force, 0, -100*mousex*self.force)
	if object.yawpkey > 0.5 then
		local q = Physics.GetOrientation(phys)
		local a = q:gety()
		Physics.ApplyTorque(phys, a*self.force*object.yawpkey)
	end
	if object.yawnkey > 0.0 then
		local q = Physics.GetOrientation(phys)
		local a = q:gety()
		Physics.ApplyTorque(phys, a*-self.force*object.yawnkey)
	end
	
	if object.pitchpkey > 0.5 then
		local q = Physics.GetOrientation(phys)
		local a = q:getx()
		Physics.ApplyTorque(phys, a*self.force*object.pitchpkey)
	end
	if object.pitchnkey > 0.0 then
		local q = Physics.GetOrientation(phys)
		local a = q:getx()
		Physics.ApplyTorque(phys, a*-self.force*object.pitchnkey)
	end
	
	if object.rollpkey > 0.0 then
		local q = Physics.GetOrientation(phys)
		local a = q:getz()
		Physics.ApplyTorque(phys, a*self.force*object.rollpkey)
	end
	if object.rollnkey > 0.0 then
		local q = Physics.GetOrientation(phys)
		local a = q:getz()
		Physics.ApplyTorque(phys, a*-self.force*object.rollnkey)
	end
	
	if object.forcekey > 0.0 then
		local q = Physics.GetOrientation(phys)
		local a = q:gety()
		Physics.ApplyForce(phys, a*self.force*object.forcekey)
	end
	
	if object.resetkey > 0.5 then
		Physics.SetOrientation(phys, cquat.identity())
	end
	
	if object.quitkey > 0.5 then
		Game.Quit()
	end
end