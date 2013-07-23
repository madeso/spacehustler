function test(entity, dt)
	local phys = GetPhysics(entity)
	ApplyForce(phys, -2,0,0)
end