PlatformScript = ScriptObject:_new()

PlatformScript.isTriggered = false

-- Is executed once when the object becomes active
function PlatformScript:Start()

    self.rigidbody = GetRigidBodyDynamicComponent(self.entity.handle)
    self.transform = GetTransformComponent(self.entity.handle)

    self.transform:RefreshRef()

    -- Position de base
    local x, y, z = self.transform:GetPosition()
    self.startPosition = Vector3.new(x, y, z)
    self.endPosition = Vector3.new(x, 24.150, -48.1)

    self.isGoingUp = false     -- etat : monte ou descend
    self.movementSpeed = 5.0   -- vitesse en unités par seconde

    -- On initialise la position en bas
    self.transform:SetPosition(self.startPosition.x, self.startPosition.y, self.startPosition.z)

end

local function Sign(x)
    if x > 0 then return 1
    elseif x < 0 then return -1
    else return 0
    end
end

local function MoveTowards(current, target, maxDelta)
    local delta = target - current
    if math.abs(delta) <= maxDelta then
        return target
    else
        return current + Sign(delta) * maxDelta
    end
end

-- Is executed every tick
function PlatformScript:Update(deltaTime)

    if self.isTriggered then
        -- Si la plateforme est déclenchée, on déplace
        local x, y, z = self.transform:GetPosition()

        local targetY
        local targetZ

        if self.isGoingUp then
            targetY = self.endPosition.y
            targetZ = self.endPosition.z
        else
            targetY = self.startPosition.y
            targetZ = self.startPosition.z
        end

        -- Interpolation vers la cible Y/Z
        local maxDelta = self.movementSpeed * deltaTime
        local newY = MoveTowards(y, targetY, maxDelta)
        local newZ = MoveTowards(z, targetZ, maxDelta)

        -- --- Correction ici : utiliser MoveKinematic ---
        -- Si la plateforme est kinematic → c’est MoveKinematic qu’il faut utiliser
        -- On prend la rotation actuelle pour ne pas la changer
        local _, rotY, _ = self.transform:GetRotation()

        self.rigidbody:MoveKinematic(self.startPosition.x, newY, newZ, 0.0, rotY, 0.0)

        -- Vérification si on a atteint la cible
        if math.abs(newY - targetY) < 0.01 and math.abs(newZ - targetZ) < 0.01 then
            -- On inverse la direction après avoir atteint la cible
            self.isGoingUp = not self.isGoingUp
            -- Si tu veux que la plateforme ne fasse qu’un aller-retour, tu peux désactiver le trigger ici :
            -- self.isTriggered = false
        end
    end

end

-- Engine definitions
ScriptObjectTypes.PlatformScript = PlatformScript
return PlatformScript