
function PiercingBullets.init()
end

function PiercingBullets.onAddGunBullet(gun, bullet)
    if math.random() < 0.2 and bullet.penetrateCount < 5 and bullet.penetrateCount >= 0 and bullet.data.cannotBeBuffed ~= true then
        bullet.penetrateCount = 5
        bullet:addTrailParticle("RailGunParticle", Vec2.new(0.0, 0.0), 7.0, {})
    end
end
