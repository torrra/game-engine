Components = {Transform = 1}
Entity = {handle = 0, ref = nil}

local ExistingEntities = {}

currentEntity = {}

function Entity:IsValid()
	return EntityRef.IsValid(self.ref)
end

function Entity:RefreshRef()
	self.ref = EntityRef.GetEntityRef(self.handle)
end

function Entity:NewNativeEntity(newHandle)
	local newEntity = {handle = newHandle, ref = nil }
	self.__index = self
	setmetatable(newEntity, self)
	return newEntity
end

function Entity:Activate(active)

	if type(active) ~= "bool" then
		error("Expecting bool as argument")
	else
		EntityRef.Activate(self.ref, active)
	end
end

function AddEntityHandle(newHandle)
	ExistingEntities[newHandle] = Entity:NewNativeEntity(newHandle)
	currentEntity = ExistingEntities[newHandle]
end

