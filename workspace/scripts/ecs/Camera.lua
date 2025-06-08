Camera = Component:_new()

local ExistingCameraComponents = {}

function Camera:_RegisterCameraComponent(handle)

	local newComponent = {entity = handle}
	self.__index = self
	setmetatable(newComponent, self)
	ExistingCameraComponents[handle] = newComponent
end

function _NewCameraComponent(handle)

	Camera:_RegisterCameraComponent(handle)
end

function _RemoveCameraComponent(handle)
	
	ExistingCameraComponents[handle] = nil
end

function Camera:RefreshRef()

	self.ref = CamRef.GetCamRef(self.entity)
end

function Camera:Move(x, y, z, speed, deltaTime)

	CamRef.Move(self.ref, x, y, z, speed, deltaTime)
end

function Camera:Rotate(x, y, z)

	CamRef.Rotate(self.ref, x, y, z)
end

function Camera:GetPosition()

	return CamRef.GetPosition(self.ref)
end

function Camera:GetRotation()

	return CamRef.GetRotation(self.ref)
end

function Camera:GetFOV()

	return CamRef.GetFOV(self.ref)
end

function Camera:GetNearPlane()

	return CamRef.GetNearPlane(self.ref)
end

function Camera:GetFarPlane()

	return CamRef.GetFarPlane(self.ref)
end

function Camera:SetPosition(x, y, z)

	CamRef.SetPosition(self.ref, x, y, z)
end

function Camera:SetRotation(x, y, z)

	CamRef.SetRotation(self.ref, x, y, z)
end

function Camera:SetFOV(fov)

	CamRef.SetFOV(self.ref, fov)
end

function Camera:SetNearPlane(near)

	CamRef.SetNearPlane(self.ref, near)
end

function Camera:SetFarPlane(far)

	CamRef.SetFarPlane(self.ref, far)
end

function GetCameraComponent(ownerHandle)

	return ExistingCameraComponents[ownerHandle]
end