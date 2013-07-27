function test_type(self, json)
end

function test_component(self, ctype)
end

function test_step(self, entity, dt)
	if forcekey > 0.5 then
		local phys = Physics.Get(entity)
		Physics.ApplyForce(phys, 0, 18, 0)
	end
end