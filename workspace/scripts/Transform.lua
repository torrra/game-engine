Transform = Component:_new()

local ExistingTransformComponents = {}

function Transform:_RegisterTransformComponent(handle)

	local newComponent = {entity = handle}
	self.__index = self
	setmetatable(newComponent, self)
	ExistingTransformComponents[handle] = newComponent
end

function _NewTransformComponent(handle)

	Transform:_RegisterTransformComponent(handle)
end

function _RemoveTransformComponent(handle)

	ExistingTransformComponents[handle] = nil
end

function GetTransformComponent(handle)

	return ExistingTransformComponents[handle]
end


function Transform:RefreshRef()

	self.ref = TransformRef.GetTransformRef(self.entity)
end

function Transform:GetPosition()
	
	return TransformRef.GetTransformPosition(self.ref)
end


function Transform:GetRotation()

	return TransformRef.GetRotation(self.ref)
end

function Transform:GetScale()

	return TransformRef.GetScale(self.ref)
end


function Transform:SetPosition(x, y, z)

	TransformRef.SetPosition(self.ref, x, y, z)
end

function Transform:SetRotation(w, x, y, z)

	TransformRef.SetRotation(self.ref, w, x, y, z)
end

function Transform:SetScale(x, y, z)

	TransformRef.SetScale(self.ref, x, y, z)
end


function Transform:AddTranslation(x, y, z)

	TransformRef.AddTranslation(self.ref, x, y, z)
end

function Transform:AddEulerRotation(x, y, z)

	TransformRef.AddEulerRotation(self.ref, x, y, z)
end

function Transform:AddScale(x, y, z)

	TransformRef.AddScale(self.ref, x, y, z)
end
