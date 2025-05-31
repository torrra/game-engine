RigidBodyDynamic = Component:_new()

local ExistingRigidBodyDynamicComponents = {}

ForceMode = {

	FORCE			= 0,
	IMPULSE			= 1,
	VELOCITY_CHANGE = 2,
	ACCELERATION	= 3
}

function RigidBodyDynamic:_RegisterRigidBodyDynamicComponent(handle)

	local newComponent = {entity = handle}
	self.__index = self
	setmetatable(newComponent, self)
	ExistingRigidBodyDynamicComponents[handle] = newComponent
end

function _NewRigidBodyDynamicComponent(handle)

	RigidBodyDynamic:_RegisterRigidBodyDynamicComponent(handle)
end

function _RemoveRigidBodyDynamicComponent(handle)

	ExistingRigidBodyDynamicComponents[handle] = nil
end

function GetRigidBodyDynamicComponent(handle)

	return ExistingRigidBodyDynamicComponents[handle]
end

function RigidBodyDynamic:RefreshRef()

	self.ref = RigidBodyDynamicRef.GetRigidBodyDynamicRef(self.entity)
end

function RigidBodyDynamic:GetIsGravityDisabled()

	return RigidBodyDynamicRef.GetIsGravityDisabled(self.ref)
end

function RigidBodyDynamic:GetBoxHalfExtents()

	return RigidBodyDynamicRef.GetBoxHalfExtents(self.ref)
end

function RigidBodyDynamic:GetSphereRadius()

	return RigidBodyDynamicRef.GetSphereRadius(self.ref)
end

function RigidBodyDynamic:GetCapsuleFormat()

	return RigidBodyDynamicRef.GetCapsuleFormat(self.ref)
end

function RigidBodyDynamic:GetLinearVelocity()

	return RigidBodyDynamicRef.GetLinearVelocity(self.ref)
end

function RigidBodyDynamic:GetAngularVelocity()

	return RigidBodyDynamicRef.GetAngularVelocity(self.ref)
end

function RigidBodyDynamic:GetIsTrigger()

	return RigidBodyDynamicRef.GetIsTrigger(self.ref)
end

function RigidBodyDynamic:GetIsXAxisLock()

	return RigidBodyDynamicRef.GetIsXAxisLock(self.ref)
end

function RigidBodyDynamic:GetIsYAxisLock()

	return RigidBodyDynamicRef.GetIsYAxisLock(self.ref)
end

function RigidBodyDynamic:GetIsZAxisLock()

	return RigidBodyDynamicRef.GetIsZAxisLock(self.ref)
end

function RigidBodyDynamic:SetGravityDisabled(isGravityDisabled)

	RigidBodyDynamicRef.SetGravityDisabled(self.ref, isGravityDisabled)
end

function RigidBodyDynamic:SetBoxHalfExtents(x, y, z)

	RigidBodyDynamicRef.SetBoxHalfExtents(self.ref, x, y, z)
end

function RigidBodyDynamic:SetSphereRadius(radius)

	RigidBodyDynamicRef.SetSphereRadius(self.ref, radius)
end

function RigidBodyDynamic:SetCapsuleFormat(x, y)

	RigidBodyDynamicRef.SetCapsuleFormat(self.ref, x, y)
end

function RigidBodyDynamic:SetTrigger(isTrigger)

	RigidBodyDynamicRef.SetTrigger(self.ref, isTrigger)
end

function RigidBodyDynamic:SetLinearVelocity(x, y, z)

	RigidBodyDynamicRef.SetLinearVelocity(self.ref, x, y, z)
end

function RigidBodyDynamic:SetAngularVelocity(x, y, z)

	RigidBodyDynamicRef.SetAngularVelocity(self.ref, x, y, z)
end

function RigidBodyDynamic:SetXAxisLock(isXAxisLock)

	RigidBodyDynamicRef.SetXAxisLock(self.ref, isXAxisLock)
end

function RigidBodyDynamic:SetYAxisLock(isYAxisLock)

	RigidBodyDynamicRef.SetYAxisLock(self.ref, isYAxisLock)
end

function RigidBodyDynamic:SetZAxisLock(isZAxisLock)

	RigidBodyDynamicRef.SetZAxisLock(self.ref, isZAxisLock)
end

function RigidBodyDynamic:AddForce(x, y, z, forceMode, autoWake)

	force = forceMode == nil and FORCE or forceMode
	wake = autoWake == nil and true or autoWake
	RigidBodyDynamicRef.AddForce(self.ref, x, y, z, force, wake)
end

function RigidBodyDynamic:AddTorque(x, y, z, forceMode, autoWake)

	RigidBodyDynamicRef.AddTorque(self.ref, x, y, z, forceMode, autoWake)
end