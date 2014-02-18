function test_type(self, json)
	self.force = Json.Get(json, "force", 50)
end

function test_component(self, object, ctype)
	self.force = ctype.force
end

function test_step(self, object, entity, dt)
	local phys = Physics.Get(entity)
	--Physics.ApplyForce(phys, -100*mousey*self.force, 0, -100*mousex*self.force)
	
	Physics.ApplyTorque(phys, Physics.GetOrientation(phys):gety()*self.force*object.yawkey)
	Physics.ApplyTorque(phys, Physics.GetOrientation(phys):getx()*self.force*object.pitchkey)
	Physics.ApplyTorque(phys, Physics.GetOrientation(phys):getz()*self.force*object.rollkey)
	
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