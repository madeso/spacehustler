function test(entity, dt)
	if forcekey > 0.5 then
		local phys = Physics.Get(entity)
		Physics.ApplyForce(phys, 0, 18, 0)
	end
end