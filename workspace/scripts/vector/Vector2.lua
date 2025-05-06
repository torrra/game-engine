Vector2 = {x = 0.0, y = 0.0}

function Vector2.new(xVal, yVal)

	if (type(xVal) ~= "number") or (type(yVal) ~= "number") then
		error("Invalid argument type")
	end

	local result = {x = xVal, y = yVal}

	setmetatable(result, Vector2)
	return result
end


function Vector2.copy(vector)

	return Vector2.new(vector.x, vector.y)
end


function Vector2.Add(vectorA, vectorB)

	local result = Vector2.new(0.0, 0.0)

	result.x = vectorA.x + vectorB.x
	result.y = vectorA.y + vectorB.y
	return result
end

function Vector2.Subtract(vectorA, vectorB)

	local result = Vector2.new(0.0, 0.0)

	result.x = vectorA.x - vectorB.x
	result.y = vectorA.y - vectorB.y
	return result
end

function Vector2.Multiply(vectorA, vectorB)

	local result = Vector2.new(0.0, 0.0)

	result.x = vectorA.x * vectorB.x
	result.y = vectorA.y * vectorB.y
	return result
end


function Vector2.Divide(vectorA, vectorB)

	if Utils.AlmostEqual(vectorB.x, 0.0) or Utils.AlmostEqual(vectorB.y, 0.0) then
		error("Cannot perform division by zero")
	end

	local result = Vector2.new(0.0, 0.0)

	result.x = vectorA.x / vectorB.x
	result.y = vectorA.y / vectorB.y
	return result
end


function Vector2.EqualTo(vectorA, vectorB)

	if (Utils.AlmostEqual(vectorA.x , vectorB.x) and AlmostEqual(vectorA.y, vectorB.y)) then
		return true;

	else
		return false

	end
end

function Vector2.ToString(vector)

	return "{ "..vector.x..", "..vector.y.." }"
end

function Vector2.Concatenate(string, vector)

	return string..Vector2.ToString(vector)
end

function Vector2:AngleFrom(vectorB)

	return Vec2C.AngleFrom(self.x, self.y, vectorB.x, vectorB.y)
end

function Vector2:Cross(vectorB)

	return Vec2C.Cross(self.x, self.y, vectorB.x, vectorB.y)
end

function Vector2:DistanceFrom(vectorB)

	return Vec2C.DistanceFrom(self.x, self.y, vectorB.x, vectorB.y)
end

function Vector2:DistanceSquaredFrom(vectorB)

	return Vec2C.DistanceSquaredFrom(self.x, self.y, vectorB.x, vectorB.y)
end

function Vector2:Dot(vectorB)

	return Vec2C.Dot(self.x, self.y, vectorB.x, vectorB.y)
end

function Vector2:IsLongerThan(vectorB)

	return Vec2C.IsLongerThan(self.x, self.y, vectorB.x, vectorB.y)
end


function Vector2:IsShorterThan(vectorB)

	return Vec2C.IsShorterThan(self.x, self.y, vectorB.x, vectorB.y)
end

function Vector2:IsUnitVector()
	
	return Vec2C.IsUnitVector(self.x, self.y)
end

function Vector2:Magnitude()

	return Vec2C.Magnitude(self.x, self.y)
end

function Vector2:MagnitudeSquared()

	return Vec2C.MagnitudeSquared(self.x, self.y)
end

function Vector2:Normalize()

	if Utils.AlmostEqual(self:MagnitudeSquared(), 0.0) then
		error("Cannot divide by zero magnitude")
	end

	self.x, self.y = Vec2C.Normalize(self.x, self.y)
end

function Vector2:NormalizedCopy()

	local copy = Vector2.copy(self)

	copy:Normalize()
	return copy
end


function Vector2:NormalVector()

	return Vector2.new(Vec2C.NormalVector(self.x, self.y))
end

function Vector2:ProjectOnto(vectorB)

	return Vector2.new(Vec2C.ProjectOnto(self.x, self.y, vectorB.x, vectorB.y))
end

function Vector2:ReflectOnto()

	return Vector2.new(Vec2C.ReflectOnto(self.x, self.y, vectorB.x, vectorB.y))
end

function Vector2:Rotate(angle)

	return Vector2.new(Vec2C.Rotate(self.x, self.y, angle))
end

function Vector2:Scale(scaleVector)

	return Vector2.new(Vec2C.Scale(self.x, self.y, scaleVector.x, scaleVector.y))
end

Vector2.__index = Vector2

Vector2.__add = Vector2.Add
Vector2.__sub = Vector2.Subtract
Vector2.__mul = Vector2.Multiply
Vector2.__div = Vector2.Divide

Vector2.__eq = Vector2.EqualTo

Vector2.__tostring = Vector2.ToString
Vector2.__concat = Vector2.Concatenate

Vector2.__len = Vector2.Magnitude

Vector2.__newindex = function(_, val) error("[Vector2] Accessing invalid index "..tostring(val)) end
Vector2.__metatable = "Protected metatable"

return Vector2