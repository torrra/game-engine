Component = {entity = nil}
ComponentTypes = {Transform = 1, Script = 2}

function Component:IsValid()

	return ComponentRef.IsValid(self.self)
end

function Component:Activate(active)

	if type(active) ~= "bool" then
		error("Expecting bool as argument")
	else
		ComponentRef.Activate(self.ref, active)
	end
end

function Component:IsActive()

	return ComponentRef.IsActive(self.ref)
end

function Component:Invalidate()

	return ComponentRef.Invalidate(self.ref)
end

function Component:_new(object)
	object = object or {}
	self.__index = self
	setmetatable(object, self)
	return object
end