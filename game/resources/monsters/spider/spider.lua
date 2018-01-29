
function Spider.init(monster)
    data = monster.data
    monster:setScale(math.random() * 0.3 + 0.6)

    local diff = player.position - monster.position
    
    monster.moveSpeed = 0
    monster.moveAngle = math.random() * math.pi * 2.0
    
    monster.pathCheckDistance = 300.0
    
    data.targetAngle = monster.moveAngle - 0.4 + math.random() * 0.8
    data.moving = true
    data.lastHitTime = 0.0
    
    monster.data.moveTimer = -0.5 + math.random() * 2.5
    if monster.data.moveTimer > 0.0 then
        monster:playAnimation("walk", math.random())
    else
        monster:playAnimation("stand", math.random())
    
    end
    
    StunController.init(monster)
    
    MonsterGroupHelper.init(monster)
    data.playerIgnoreDistance = 30.0
    
    MonsterMeleeHelper.init(monster)
    data.hitWaitTime = 0.1
    data.hitInterval = 0.9
    data.minDamage = 8
    data.maxDamage = 14
    
    data.maxMoveSpeed = 200.0 * (math.random() * 0.1 + 1.0)
    data.maxRotateSpeed = 0.08
    data.playerSeeRange = 250.0
    
    data.tickWaitTime = 0.0
    
    data.randomShiftAngle = 0.0
    
    data.tickWaitTime = 0.0
    
    BulletShooter.init(monster)
end

function Spider.spawnChanceInMission(missionData, min)
    return 0.15 + clamp(min * 0.3) * 0.10
end
 
function Spider.buffStats(monster, min)
    StunController.buffStats(monster, min)
    MonsterMeleeHelper.buffStats(monster, min)
    BulletShooter.buffStats(monster, min)
    
    monster.data.randomMove = (math.random() > (0.15 + clamp(min * 0.2) * 0.15))
    monster.data.playerSeeRange = monster.data.playerSeeRange * (1.0 +  clamp(min * 0.1) * 2.0)
    monster.data.maxMoveSpeed =  monster.data.maxMoveSpeed * (1.0 + clamp(min  / 10) * 0.20)
    monster.data.maxRotateSpeed =  monster.data.maxRotateSpeed * (1.0 + clamp(min * 0.05) * 1.0)
end

function Spider.makeBoss(monster, min)
    makeBossDefault(monster)
end

function Spider.onHit(monster, damage, dir, args)
    StunController.onHit(monster, damage, dir, args)
end

function Spider.onTick(monster)
    data = monster.data
    
    data.tickWaitTime = data.tickWaitTime - dt
    
    diffToPlayer = player.position - monster.position
    distanceToPlayer = diffToPlayer:length()
    angleToPlayer = diffToPlayer:getAngle()
    
    if data.tickWaitTime < 0.0 then
        data.tickWaitTime = 0.2 + math.random() * 0.2 + lerp(0.0, 1.2, clamp((distanceToPlayer - 100) / 1500))
        local posToMove = player.position
        
        if data.randomMove or player.isDead then
            posToMove = data.randomPos
            if posToMove == nil or posToMove:distanceSquared(monster.position) < 60 * 60 then
                posToMove = getRandomMapPosition()
                data.randomPos = posToMove
            end
                
            if distanceToPlayer < data.playerSeeRange and player.isDead == false then
                local c = (distanceToPlayer - data.playerSeeRange * 0.8) / data.playerSeeRange * 0.2
                if c < 0.0 then
                    c = 0.0
                end
                posToMove = posToMove * c + player.position * (1.0 - c)
            end
        end
        
        
        posToMove = monster:getPathPos(posToMove)
        
        diffToMovePos = posToMove - monster.position
        distanceToMovePos = diffToMovePos:length()
        angleToMovePos = diffToMovePos:getAngle()
        
        data.targetAngleToMovePos = angleToMovePos
        data.posToMove = posToMove
    end

    MonsterMeleeHelper.onTick(monster)
    BulletShooter.onTick(monster)
    
    if data.moveTimer > 0.0 then
        local moveNewAngle = MonsterGroupHelper.fixAngle(monster, data.targetAngleToMovePos)
        local cPlayer = clamp((distanceToPlayer - 100.0) / 100.0)
        monster.moveAngle = approachAngle(monster.moveAngle, moveNewAngle, (data.maxRotateSpeed * (1.1 - cPlayer) ) * timeScale)
        data.moveTimer = data.moveTimer - dt
        if data.moveTimer <= 0.0 then
            monster:playAnimation("stand", math.random())
        end
        
        if data.moving then
            monster.moveSpeed = data.maxMoveSpeed * StunController.getSlowAmount(monster);
        else
            monster.moveSpeed = 0.0;
        end
    else
        monster.moveSpeed = 0.0
        data.moveTimer = data.moveTimer - dt
        if data.moveTimer <= -1.0 then
            monster:playAnimation("walk", math.random())
            monster.data.moveTimer = 0.5 + math.random() * 2.5
            data.randomPos = getRandomMapPosition()
        end
    end
end
