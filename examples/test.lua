function test(entity, dt)
	local a = cvec3.create()
	local b = cvec3.create(-2, 0, 0)
	local c = a+b
	local phys = Physics.Get(entity)
	Physics.ApplyForce(phys, c)
end