Raycast = { index = -1}
RayHit = { hasHit = false, hitEntity = nil, location = nil, distance = 0.0}

function Raycast.new()

	local newObj = {index = RayC.Create()}
	setmetatable(newObj, Raycast)
	return newObj
end


function Raycast:GetOrigin()

	return Vector3.new(RayC.GetOrigin(self.index))
end

function Raycast:GetDirection()

	return Vector3.new(RayC.GetDirection(self.index))
end

function Raycast:GetDistance()

	return RayC.GetDistance(self.index)
end

function Raycast:SetDirection(vector)

	RayC.SetDirection(self.index, vector.x, vector.y, vector.z)
end

function Raycast:SetOrigin(vector)

	RayC.SetOrigin(self.index, vector.x, vector.y, vector.z)
end

function Raycast:SetDistance(distance)

	RayC.SetDistance(self.index, distance)
end

function Raycast:HasHit()

	local hasHitBool, hitHandle, hitLocationX, hitLocationY, hitLocationZ, hitDistance = 
	RayC.HasHit(self.index)

	local hitData = 
	{
		hasHit = hasHitBool,
		hitEntity = Entity:_NewNativeEntity(hitHandle),
		location = Vector3.new(hitLocationX, hitLocationY, hitLocationZ),
		distance = hitDistance
	}

	return hitData
end

Raycast.__index = Raycast

return Raycast