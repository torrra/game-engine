Vector3 = {x = 0.0, y = 0.0, z = 0.0}

function Vector3.new(xVal, yVal, zVal)

	if (type(xVal) ~= "number") or (type(yVal) ~= "number") or (type(zVal) ~= "number") then
		error("Invalid argument type")
	end

	local result = {x = xVal, y = yVal, z = zVal}

	setmetatable(result, Vector3)
	return result
end


function Vector3.copy(vector)

	return Vector3.new(vector.x, vector.y, vector.z)
end


function Vector3.Add(vectorA, vectorB)

	local result = Vector3.new(0.0, 0.0, 0.0)

	result.x = vectorA.x + vectorB.x
	result.y = vectorA.y + vectorB.y
	result.z = vectorA.z + vectorB.z
	return result
end

function Vector3.Subtract(vectorA, vectorB)

	local result = Vector3.new(0.0, 0.0, 0.0)

	result.x = vectorA.x - vectorB.x
	result.y = vectorA.y - vectorB.y
	result.z = vectorA.z - vectorB.z
	return result
end

function Vector3.Multiply(vectorA, vectorB)

	local result = Vector3.new(0.0, 0.0, 0.0)

	result.x = vectorA.x * vectorB.x
	result.y = vectorA.y * vectorB.y
	result.z = vectorA.z * vectorB.z
	return result
end


function Vector3.Divide(vectorA, vectorB)

	if Utils.AlmostEqual(vectorB.x, 0.0) or Utils.AlmostEqual(vectorB.y, 0.0) or
		Utils.AlmostEqual(vectorB.z, 0.0) then

		error("Cannot perform division by zero")
	end

	local result = Vector3.new(0.0, 0.0, 0.0)

	result.x = vectorA.x / vectorB.x
	result.y = vectorA.y / vectorB.y
	result.z = vectorA.z / vectorB.z
	return result
end


function Vector3.EqualTo(vectorA, vectorB)

	if  Utils.AlmostEqual(vectorA.x , vectorB.x) and AlmostEqual(vectorA.y, vectorB.y) and 
		Utils.AlmostEqual(vectorA.z, vectorB.z) then

		return true

	else
		return false

	end
end

function Vector3.ToString(vector)

	return "{ "..vector.x..", "..vector.y..", "..vector.z.." }"
end

function Vector3.Concatenate(string, vector)

	return string..Vector3.ToString(vector)
end

function Vector3:AngleFrom(vectorB)

	return Vec3C.AngleFrom(self.x, self.y, self.z, vectorB.x, vectorB.y, vectorB.z)
end

function Vector3:Cross(vectorB)

	return Vec3C.Cross(self.x, self.y, self.z, vectorB.x, vectorB.y, vectorB.z)
end

function Vector3:DistanceFrom(vectorB)

	return Vec3C.DistanceFrom(self.x, self.y, self.z, vectorB.x, vectorB.y, vectorB.z)
end

function Vector3:DistanceSquaredFrom(vectorB)

	return Vec3C.DistanceSquaredFrom(self.x, self.y, self.z, vectorB.x, vectorB.y, vectorB.z)
end

function Vector3:Distance2DFrom(vectorB)

	return Vec3C.Distance2DFrom(self.x, self.y, self.z, vectorB.x, vectorB.y, vectorB.z)
end

function Vector3:Distance2DSquaredFrom(vectorB)

	return Vec3C.Distance2DSquaredFrom(self.x, self.y, self.z, vectorB.x, vectorB.y, vectorB.z)
end

function Vector3:Dot(vectorB)

	return Vec3C.Dot(self.x, self.y, self.z, vectorB.x, vectorB.y, vectorB.z)
end

function Vector3:IsLongerThan(vectorB)

	return Vec3C.IsLongerThan(self.x, self.y, self.z, vectorB.x, vectorB.y, vectorB.z)
end


function Vector3:IsShorterThan(vectorB)

	return Vec3C.IsShorterThan(self.x, self.y, self.z, vectorB.x, vectorB.y, vectorB.z)
end

function Vector3:IsUnitVector()
	
	return Vec3C.IsUnitVector(self.x, self.y, self.z)
end

function Vector3:Magnitude()

	return Vec3C.Magnitude(self.x, self.y, self.z)
end

function Vector3:MagnitudeSquared()

	return Vec3C.MagnitudeSquared(self.x, self.y, self.z)
end

function Vector3:Normalize()

	if Utils.AlmostEqual(self:MagnitudeSquared(), 0.0) then
		error("Cannot divide by zero magnitude")
	end

	self.x, self.y, self.z = Vec3C.Normalize(self.x, self.y, self.z)
end

function Vector3:NormalizedCopy()

	local copy = Vector3.copy(self)

	copy:Normalize()
	return copy
end

function Vector3:ProjectOnto(vectorB)

	return Vector3.new(Vec3C.ProjectOnto(self.x, self.y, self.z, vectorB.x, vectorB.y, vectorB.z))
end

function Vector3:ReflectOnto(vectorB)

	return Vector3.new(Vec3C.ReflectOnto(self.x, self.y, self.z, vectorB.x, vectorB.y, vectorB.z))
end

function Vector3:Rotate(angleX, angleY, angleZ)

	return Vector3.new(Vec3C.Rotate(self.x, self.y, self.z, angleX, angleY, angleZ))
end

function Vector3:Scale(scaleVector)

	return Vector3.new(Vec3C.Scale(self.x, self.y, self.z, scaleVector.x, scaleVector.y, scaleVector.z))
end

function Vector3:Translate(translation)

	return Vector3.new(Vec3C.Translate(self.x, self.y, self.z, translation.x, translation.y, translation.z))
end

function Vector3.Lerp(vectorA, vectorB, ratio)

	return Vector3.new(Vec3C.Lerp(vectorA.x, vectorA.y, vectorA.z,
						vectorB.x, vectorB.y, vectorB.z, ratio))
end

Vector3.__index = Vector3

Vector3.__add = Vector3.Add
Vector3.__sub = Vector3.Subtract
Vector3.__mul = Vector3.Multiply
Vector3.__div = Vector3.Divide

Vector3.__eq = Vector3.EqualTo

Vector3.__tostring = Vector3.ToString
Vector3.__concat = Vector3.Concatenate

Vector3.__len = Vector3.Magnitude

Vector3.__newindex = function(_, val) error("[Vector3] Accessing invalid index "..tostring(val)) end
Vector3.__metatable = "Protected metatable"

return Vector3