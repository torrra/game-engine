EnemyScript = ScriptObject:_new()

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

local function QuaternionSlerp(q1, q2, t)
    local dot = q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z
    if dot < 0 then
        q2 = {w = -q2.w, x = -q2.x, y = -q2.y, z = -q2.z}
        dot = -dot
    end
    if dot > 0.9995 then
        local w = q1.w + t * (q2.w - q1.w)
        local x = q1.x + t * (q2.x - q1.x)
        local y = q1.y + t * (q2.y - q1.y)
        local z = q1.z + t * (q2.z - q1.z)
        local len = math.sqrt(w*w + x*x + y*y + z*z)
        return {w = w/len, x = x/len, y = y/len, z = z/len}
    end
    local theta0 = math.acos(dot)
    local theta = theta0 * t
    local sinTheta = math.sin(theta)
    local sinTheta0 = math.sin(theta0)
    local s0 = math.cos(theta) - dot * sinTheta / sinTheta0
    local s1 = sinTheta / sinTheta0
    return {
        w = s0 * q1.w + s1 * q2.w,
        x = s0 * q1.x + s1 * q2.x,
        y = s0 * q1.y + s1 * q2.y,
        z = s0 * q1.z + s1 * q2.z
    }
end

function EnemyScript:Start()
    
    self.transform = GetTransformComponent(self.entity.handle)
    self.navPoint1 = GetNavigationPointComponent(GetEntity("NavPoint.NavPoint1").handle)
    self.navPoint2 = GetNavigationPointComponent(GetEntity("NavPoint.NavPoint2").handle)
    self.navPoint1:RefreshRef()
    self.navPoint2:RefreshRef()
    self.transform:RefreshRef()

    self.height = Vector3.new(self.transform:GetPosition())
    local x, y, z = self.navPoint1:GetPosition()
    self.transform:SetPosition(x, self.height.y, z)

    self.speed = 3.0
    self.goingToPoint1 = false

    -- Nouveaux états : "moving", "pauseBeforeTurn", "rotating", "pauseAfterTurn"
    self.state = "moving"

    self.pauseBeforeDuration = 1.0
    self.pauseAfterDuration = 1.0
    self.pauseTimer = 0

    -- Rotation
    local w, x, y, z = self.transform:GetRotation()
    self.currentRotation = {w=w, x=x, y=y, z=z}
    self.targetRotation = {w=w, x=x, y=y, z=z}
    self.rotationProgress = 0
    self.rotationSpeed = 1.5 -- durée rotation en secondes
end

function EnemyScript:Update(deltaTime)
    local currentPos = Vector3.new(self.transform:GetPosition())
    local targetPoint = self.goingToPoint1 and self.navPoint1 or self.navPoint2
    local targetPos = Vector3.new(targetPoint:GetPosition())
    local direction = Vector3.new(targetPos.x - currentPos.x, 0, targetPos.z - currentPos.z)
    local distance = Length(targetPos, currentPos)

    if self.state == "moving" then
        if distance < 0.1 then
            -- Arrivé au point, on passe en pause avant rotation
            self.state = "pauseBeforeTurn"
            self.pauseTimer = self.pauseBeforeDuration
        else
            -- Avancer vers la cible
            direction:Normalize()
            local move = Vector3.new(direction.x * self.speed * deltaTime, 0, direction.z * self.speed * deltaTime)
            local newPosition = currentPos + move
            self.transform:SetPosition(newPosition.x, self.height.y, newPosition.z)
        end

    elseif self.state == "pauseBeforeTurn" then
        self.pauseTimer = self.pauseTimer - deltaTime
        if self.pauseTimer <= 0 then
            -- Calcul rotation cible
            self.goingToPoint1 = not self.goingToPoint1
            local newTargetPoint = self.goingToPoint1 and self.navPoint1 or self.navPoint2
            local newTargetPos = Vector3.new(newTargetPoint:GetPosition())
            local newDir = Vector3.new(newTargetPos.x - currentPos.x, 0, newTargetPos.z - currentPos.z)
            newDir:Normalize()

            local angleY = math.deg(Atan2(newDir.z, newDir.x))
            if angleY < 0 then angleY = angleY + 360 end
            local angleRad = math.rad(angleY)
            local halfAngle = angleRad / 2

            self.targetRotation = {
                w = math.cos(halfAngle),
                x = 0,
                y = math.sin(halfAngle),
                z = 0
            }

            local w,x,y,z = self.transform:GetRotation()
            self.currentRotation = {w=w, x=x, y=y, z=z}
            self.rotationProgress = 0

            self.state = "rotating"
        end

    elseif self.state == "rotating" then
        self.rotationProgress = self.rotationProgress + deltaTime / self.rotationSpeed
        if self.rotationProgress >= 1 then
            self.rotationProgress = 1
            self.state = "pauseAfterTurn"
            self.pauseTimer = self.pauseAfterDuration
        end
        local newRot = QuaternionSlerp(self.currentRotation, self.targetRotation, self.rotationProgress)
        self.transform:SetRotation(newRot.w, newRot.x, newRot.y, newRot.z)

    elseif self.state == "pauseAfterTurn" then
        self.pauseTimer = self.pauseTimer - deltaTime
        if self.pauseTimer <= 0 then
            self.state = "moving"
        end
    end
end

ScriptObjectTypes.EnemyScript = EnemyScript
return EnemyScript
