RenderableAlignment = {
    world = 0,
    topLeft = 1,
    top = 2,
    topRight = 3,
    left = 4,
    center = 5,
    right = 6,
    bottomLeft = 7,
    bottom = 8,
    bottomRight = 9
}
TextAlignment = {
    left = 0,
    center = 1,
    right = 2,
    fit = 3
}
VerticalTextAlignment = {
    top = 0,
    mid = 1,
    bottom = 2
}
RenderableLevel = {
    background = -1000,
    gameObjects = 1000,
    monsters = 2000,
    player = 3000,
    bullets = 4000,
    objectGUI = 5000,
    foreground = 6000,
    GUI = 7000
}

CollisionFlags = {
    NoPlayerCollision = 1,
    NoMonsterCollision = 2,
    NoBulletCollision = 4,
}

function logTable(myTable)
    if myTable == nil then
        log("nil")
    else 
        for k, v in pairs( myTable ) do
           log(k, v)
        end
    end
 end
 
local seen={}

 function dumpTable(t)
    seen={}
    dump(t, "")
 end
 
 

function dump(t,i)
    seen[t]=true
    local s={}
    local n=0
    for k in pairs(t) do
        n=n+1 s[n]=k
    end
    for k,v in ipairs(s) do
        log(v .. " " .. tostring(t[v]))
        v=t[v]
        if type(v)=="table" and not seen[v] then
            dump(v,i.."\t")
        --elseif type(v)=="userdata" and not seen[v] then
        --    dump(getmetatable(v),i.."\t")
        end
    end
end

function clamp(t)
    if t < 0.0 then
        return 0.0
    elseif t > 1.0 then
        return 1.0
    end
    return t
end


function lerp (a, b, t)
    return a + (b-a) * t
end

function fixAngle(a)
    while a > math.pi do
        a = a - 2.0 * math.pi
    end
    while a < -math.pi do
        a = a + 2.0 * math.pi
    end
    return a
end

function shallowcopy(orig)
    local orig_type = type(orig)
    local copy
    if orig_type == 'table' then
        copy = {}
        for orig_key, orig_value in pairs(orig) do
            copy[orig_key] = orig_value
        end
    else 
        copy = orig
    end
    return copy
end

FadeOutImage = {}

function FadeOutImage.init(gameObject)
    if gameObject.data.fadeOutStartTime == nil then
        gameObject.data.fadeOutStartTime = 1.0
    end
    if gameObject.data.fadeOutDuration == nil then
        gameObject.data.fadeOutDuration = 1.0
    end
    if gameObject.data.fadeInDuration == nil then
        gameObject.data.fadeInDuration = 0.0
    end
    if gameObject.data.startAlpha == nil then
        gameObject.data.startAlpha = 1.0
    end
end


function FadeOutImage.onTick(gameObject)
    local timeDiff = time - gameObject.data.startTime
    local alpha
    if timeDiff < gameObject.data.fadeInDuration then
        alpha = timeDiff / gameObject.data.fadeInDuration
    elseif gameObject.data.fadeOutStartTime < 0.0 then
        alpha = 1.0
    else 
        alpha = 1.0 - (timeDiff - gameObject.data.fadeOutStartTime) / gameObject.data.fadeOutDuration
        if alpha < 0.0 then 
            gameObject.toBeRemoved = true
            alpha = 0.0
        elseif alpha > 1.0 then
            alpha = 1.0
        end
    end
    gameObject.data.renderable:setColor(Vec4.new(1.0, 1.0, 1.0, alpha * gameObject.data.startAlpha))
end

function FadeOutImage.clear(gameObjectId)

end
DummyProfiler = {}
DummyProfiler.start = function()
end
DummyProfiler.stop = function()
end
if createSumProfiler == nil then
    createSumProfiler = function()
        return DummyProfiler
    end
end
if createProfiler == nil then
    createProfiler = function()
        return DummyProfiler
    end
end

function deleteAll()
    for k in pairs (_G) do
        if k ~= "" and k ~= nil and _G[k] ~= _G and _G[k] ~= doNotDelete and doNotDelete[k] ~= true then
            _G[k] = nil
        end
    end
end

function createWeightedRandomList()
    local l = {}
    l.list = {}
    l.total = 0.0
    
    l.add = function(element, weight)
        l.list[element] = weight
        l.total = l.total + weight
        l.last = element
    end
    
    l.get = function()
        local t = math.random() * l.total
        for k,v in ipairs(l.list) do
            t = t - v
            if t < 0.001 then
                return k
            end
        end
        return l.last
    end
    return l
end

function printStack()
    print(debug.traceback())
end

function fillDoNotDelete()
    doNotDelete = {}
    for k in pairs (_G) do
        doNotDelete[k] = true
    end
end
