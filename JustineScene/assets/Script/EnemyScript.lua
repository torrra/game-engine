EnemyScript = ScriptObject:_new()

EnemyScript.hasDetected = false

local State = 
{
    Moving = 0,
    PauseBeforeTurn = 1,
    Rotating = 2,
    PauseAfterTurn = 3
}

local function Length(a, b)
    local dx = a.x - b.x
    local dz = a.z - b.z
    return math.sqrt(dx * dx + dz * dz)
end

local function Atan2(y, x)
    if x > 0 then
        return math.atan(y / x)
    elseif x < 0 and y >= 0 then
        return math.atan(y / x) + math.pi
    elseif x < 0 and y < 0 then
        return math.atan(y / x) - math.pi
    elseif x == 0 and y > 0 then
        return math.pi / 2
    elseif x == 0 and y < 0 then
        return -math.pi / 2
    else
        return 0
    end
end

local function Lerp(a, b, t)
    
    return a + (b - a) * t

end

local function LerpAngle(a, b, t)
    local delta = (b - a + math.pi) % (2 * math.pi) - math.pi
    return a + delta * t
end

local function NormalizeAngle(angle)
    angle = (angle + math.pi) % (2 * math.pi)
    if angle < 0 then
        angle = angle + 2 * math.pi
    end
    return angle - math.pi
end

function EnemyScript:Initialize()

    self.transform = GetTransformComponent(self.entity.handle)
    self.navPoint1 = GetNavigationPointComponent(GetEntity("NavPoint.NavPoint1").handle)
    self.navPoint2 = GetNavigationPointComponent(GetEntity("NavPoint.NavPoint2").handle)
    self.navPoint1:RefreshRef()
    self.navPoint2:RefreshRef()
    self.transform:RefreshRef()

    local _, yHeight, _ = self.transform:GetPosition()
    self.height = yHeight
    local x, _, z = self.navPoint1:GetPosition()
    self.transform:SetPosition(x, self.height, z)

    self.speed = 3.0
    self.goingToPoint1 = false

    self.state = State.Moving

    self.pauseBeforeDuration = 1.0
    self.pauseAfterDuration = 1.0
    self.pauseTimer = 0

    -- Rotation
    local w, x, y, z = self.transform:GetRotation()
    self.currentRotation = {x=x, y=y, z=z}
    self.targetRotation = {x=x, y=y, z=z}
    self.rotationProgress = 0
    self.rotationSpeed = 1.5 -- rotation duration in second

end

function EnemyScript:Move(deltaTime)

    local currentPos = Vector3.new(self.transform:GetPosition())
    local targetPoint = self.goingToPoint1 and self.navPoint1 or self.navPoint2
    local targetPos = Vector3.new(targetPoint:GetPosition())
    local direction = Vector3.new(targetPos.x - currentPos.x, currentPos.y, targetPos.z - currentPos.z)
    local distance = Length(targetPos, currentPos)

    if self.state == State.Moving then
        if distance < 0.1 then
            -- Pause before rotation 
            self.state = State.PauseBeforeTurn
            self.pauseTimer = self.pauseBeforeDuration
        else
            -- Move to the target 
            direction:Normalize()
            local move = Vector3.new(direction.x * self.speed * deltaTime, currentPos.y, direction.z * self.speed * deltaTime)
            local newPosition = currentPos + move
            self.transform:SetPosition(newPosition.x, currentPos.y, newPosition.z)
        end

    elseif self.state == State.PauseBeforeTurn then
        self.pauseTimer = self.pauseTimer - deltaTime
        if self.pauseTimer <= 0 then
            self.goingToPoint1 = not self.goingToPoint1
            local newTargetPoint = self.goingToPoint1 and self.navPoint1 or self.navPoint2
            local newTargetPos = Vector3.new(newTargetPoint:GetPosition())
            local newDir = Vector3.new(newTargetPos.x - currentPos.x, currentPos.y, newTargetPos.z - currentPos.z)
            newDir:Normalize()

            local angleY = -Atan2(newDir.z, newDir.x)

            self.targetRotation = {
                x = 0,
                y = angleY,
                z = 0
            }

            -- Récupérer la rotation actuelle du transform pour bien commencer l'interpolation
            local currentY = NormalizeAngle(self.currentRotation.y)
            angleY = NormalizeAngle(angleY)

            self.currentRotation = {
                x = 0,
                y = currentY,
                z = 0
            }

            self.rotationProgress = 0
            self.state = State.Rotating
        end


    elseif self.state == State.Rotating then
        self.rotationProgress = self.rotationProgress + deltaTime / self.rotationSpeed
        if self.rotationProgress >= 1 then
            self.rotationProgress = 1
            self.state = State.PauseAfterTurn
            self.pauseTimer = self.pauseAfterDuration
            -- Fixer la rotation pour éviter erreurs d'interpolation
            self.currentRotation = {
                x = self.targetRotation.x,
                y = self.targetRotation.y,
                z = self.targetRotation.z
            }
        end
        -- Interpoler la rotation entre currentRotation (début) et targetRotation (fin)
        local newX = Lerp(self.currentRotation.x, self.targetRotation.x, self.rotationProgress)
        local newY = LerpAngle(self.currentRotation.y, self.targetRotation.y, self.rotationProgress)
        local newZ = Lerp(self.currentRotation.z, self.targetRotation.z, self.rotationProgress)
        
        self.transform:SetRotation(newX, newY, newZ)

    elseif self.state == State.PauseAfterTurn then
        self.pauseTimer = self.pauseTimer - deltaTime
        if self.pauseTimer <= 0 then
            self.state = State.Moving
        end
    end
end

function EnemyScript:Attack(deltaTime)

    local currentPosition = Vector3.new(self.transform:GetPosition())
    local targetPosition = Vector3.new(self.playerTransform:GetPosition())
    local direction = Vector3.new(targetPosition.x - currentPosition.x, currentPosition.y, targetPosition.z - currentPosition.z)
    local distance = Length(targetPosition, currentPosition)

    local targetDistance = 4.0

    if self.hasDetected then
        if direction.x ~= 0 or direction.z ~= 0 then

            direction:Normalize()
            local angleY = Atan2(direction.z, direction.x)
            self.transform:SetRotation(0.0, -angleY, 0.0)

        end
        local moveDirection = Vector3.new(0, 0, 0)

        if distance > targetDistance + 0.2 then
            moveDirection = direction
        elseif distance < targetDistance - 0.2 then
            moveDirection = Vector3.new(-direction.x, -direction.y, -direction.z)
        else 
            moveDirection = Vector3.new(0, 0, 0)
        end

        local move = Vector3.new(moveDirection.x * self.speed * deltaTime, currentPosition.y, moveDirection.z * self.speed * deltaTime)
        local newPosition = currentPosition + move
        self.transform:SetPosition(newPosition.x, currentPosition.y, newPosition.z)
    end
    
end

function EnemyScript:Start()
    
    self:Initialize()

    self.playerTransform = GetTransformComponent(GetEntity("Player").handle)
end

function EnemyScript:Update(deltaTime)
    
    if self.hasDetected then
        self:Attack(deltaTime)
    else
        self:Move(deltaTime)
    end


end

ScriptObjectTypes.EnemyScript = EnemyScript
return EnemyScript
