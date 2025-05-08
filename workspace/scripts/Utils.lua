Utils = {}

function Utils.AlmostEqual(valA, valB)

    return math.abs(valA - valB) < 0.00001
end

function Utils.Clamp(val, low, high)

    if val < low then 
        return low

    elseif val > high then
        return high

    else
        return val
    end
end

function Utils.DenyAccess(_, val)
    error("[Utils] Cannot access index "..tostring(val).." in this table")
end

Utils.__index = Utils.DenyAccess
Utils.__newindex = Utils.DenyAccess
Utils.__metatable = "Protected metatable"
setmetatable(Utils, Utils)

return Utils