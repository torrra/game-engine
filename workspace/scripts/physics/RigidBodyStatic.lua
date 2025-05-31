RigidBodyStatic = Component:_new()

local ExistingRigidBodyStaticComponents = {}

function RigidBodyStatic:_RegisterRigidBodyStaticComponent(handle)

	local newComponent = {entity = handle}
	self.__index = self
	setmetatable(newComponent, self)
	ExistingRigidBodyStaticComponents[handle] = newComponent
end

function _NewRigidBodyStaticComponent(handle)

	RigidBodyStatic:_RegisterRigidBodyStaticComponent(handle)
end

function _RemoveRigidBodyStaticComponent(handle)

	ExistingRigidBodyStaticComponents[handle] = nil
end

function GetRigidBodyStaticComponent(handle)

	return ExistingRigidBodyStaticComponents[handle]
end

function RigidBodyStatic:RefreshRef()

	self.ref = RigidBodyStaticRef.GetRigidBodyStaticRef(self.entity)
end

function RigidBodyStatic:GetBoxHalfExtents()

	return RigidBodyStaticRef.GetBoxHalfExtents(self.ref)
end

function RigidBodyStatic:GetSphereRadius()

	return RigidBodyStaticRef.GetSphereRadius(self.ref)
end

function RigidBodyStatic:GetCapsuleFormat()

	return RigidBodyStaticRef.GetCapsuleFormat(self.ref)
end

function RigidBodyStatic:GetIsTrigger()

	return RigidBodyStaticRef.GetIsTrigger(self.ref)
end

function RigidBodyStatic:SetBoxHalfExtents(x, y, z)

	RigidBodyStaticRef.SetBoxHalfExtents(self.ref, x, y, z)
end

function RigidBodyStatic:SetSphereRadius(radius)

	RigidBodyStaticRef.SetSphereRadius(self.ref, radius)
end

function RigidBodyStatic:SetCapsuleFormat(x, y)

	RigidBodyStaticRef.SetCapsuleFormat(self.ref, x, y)
end

function RigidBodyStatic:SetTrigger(isTrigger)

	RigidBodyStaticRef.SetTrigger(self.ref, isTrigger)
end