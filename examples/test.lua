function test(entity, dt)
	local p = cvec3.create()
	local phys = Physics.Get(entity)
	Physics.ApplyForce(phys, -2,0,0)
end