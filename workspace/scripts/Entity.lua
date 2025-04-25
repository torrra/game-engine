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
    return newEntity
end

function Entity:Activate(active)

    if type(active) ~= "bool" then
        error("Expecting bool as argument")
    else
        EntityRef.Activate(self.ref, active)
    end
end  

-- Get an existing entity reference
function GetEntity(entityName)
 
    return ExistingEntities[entityName]
end

-- Register a new unique entity reference
function _AddEntityHandle(newHandle, entityName)

    ExistingEntities[entityName] = Entity:_NewNativeEntity(newHandle)
    print("\n[Entity script] successfully registered "..entityName.."\n")
end


function _UnregisterEntity(entityName)

    ExistingEntities[entityName] = nil;
    print("\n[Entity script] unregistered "..entityName.."\n")
end

return Entity