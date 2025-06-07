Entity = {handle = 0, ref = nil}

local ExistingEntities = {}


function Entity:IsValid()

    return EntityRef.IsValid(self.ref)
end

function Entity:IsActive()

    return EntityRef.IsActive(self.ref)
end

function Entity:Invalidate()

    return EntityRef.Invalidate(self.ref)
end

function Entity:RefreshRef()

    self.ref = EntityRef.GetEntityRef(self.handle)
end

-- Create new instance of entity reference
function Entity:_NewNativeEntity(newHandle)

    local newEntity = {handle = newHandle, ref = nil }
    self.__index = self
    setmetatable(newEntity, self)
    newEntity:RefreshRef()
    return newEntity
end

function Entity:Activate(active)

    if type(active) ~= "bool" then
        error("Expecting bool as argument")
    else
        EntityRef.Activate(self.ref, active)
    end
end  

function Entity:GetParent()

    return EntityRef.GetParent(self.ref)
end

function Entity:HasParent()

    return EntityRef.HasParent(self.ref)
end

-- Get an existing entity reference
function GetEntity(entityName)
 
    local entity = ExistingEntities[entityName]

    if entity then
        entity:RefreshRef()
    end

    return entity
end

-- Register a new unique entity reference
function _AddEntityHandle(newHandle, entityName)

    ExistingEntities[entityName] = Entity:_NewNativeEntity(newHandle)
end


function _UnregisterEntity(entityName)

    ExistingEntities[entityName] = nil;
end

return Entity