#include "Entity.h"

const float Entity::GRAVITY = 0.15;

Entity::Entity(Level& level, EntityType entityType, float x, float y, float w, float h):
    level(level),
    entityType(entityType),
    x(x),
    y(y),
    dx(0),
    dy(0),
    w(w),
    h(h),
    shield(false),
    enemy(false),
    landedDown(false),
    landedLeft(false),
    landedRight(false),
    landedUp(false),
    palpable(true),
    die(false)
{
    //ctor
}

Entity::~Entity()
{
    //dtor
}

void Entity::step()
{
}

void Entity::draw(sf::RenderWindow& window)
{
}

void Entity::drawShield(sf::RenderWindow& window)
{
    sf::CircleShape circle(h/1.5, 10);
    circle.setOrigin(h/1.5, h/1.5);
    circle.setFillColor(sf::Color(100, 100, 255, 50));
    circle.setPosition(x+w/2, y+h/2);
    window.draw(circle);
}

void Entity::collideEntity(Entity* other)
{
}

bool Entity::isOutOfBounds()
{
    return x + w < 0 ||
            y + h < 0 ||
            x > level.w * Tile::TILE_SIZE ||
            y > level.h * Tile::TILE_SIZE;
}
bool Entity::isCollidingWith(Entity* other)
{
    if(!palpable || !(other->palpable))
    {
        return false;
    }
    return !(x + w < other->x ||
             x > other->x + other->w ||
             y + h < other->y ||
             y > other->y + other->h);
}
void Entity::separateFromEntity(Entity* other)
{
    float minDist = 1.0 / 0.f;
    bool horizontal = true;

    if(x < other->x)
    {
        float dist = other->x - (x + w);
        if(fabs(dist) < fabs(minDist))
        {
            minDist = dist;
            horizontal = true;
        }
    }
    if(x+w > other->x+other->w)
    {
        float dist = other->x + other->w - x;
        if(fabs(dist) < fabs(minDist))
        {
            minDist = dist;
            horizontal = true;
        }
    }
    if(y < other->y)
    {
        float dist = other->y - (y + h);
        if(fabs(dist) < fabs(minDist))
        {
            minDist = dist;
            horizontal = false;
        }
    }
    if(y+h > other->y+other->h)
    {
        float dist = other->y + other->h - y;
        if(fabs(dist) < fabs(minDist))
        {
            minDist = dist;
            horizontal = false;
        }
    }
    if(horizontal)
    {
        dx += minDist / 4;
        other->dx -= minDist/4;
    }
    else
    {
        dy += minDist/4;
        other->dy -= minDist/4;
    }
}
void Entity::drawBoundingBox(sf::RenderWindow& window)
{
    sf::RectangleShape box({w, h});
    box.setPosition(x, y);
    box.setFillColor(sf::Color(255, 0, 0, 30));
    window.draw(box);
}

void Entity::collideLevel()
{
    int startX = max(0, (int)(x / Tile::TILE_SIZE));
    int endX = min(level.w-1, (unsigned )((x+w) / Tile::TILE_SIZE));
    int startY = max(0, (int)(y / Tile::TILE_SIZE));
    int endY = min(level.h-1, (unsigned)((y+h) / Tile::TILE_SIZE));


    /// this will crash sometimes

    float minDistX = 1.0/0.f;
    float minDistY = 1.0/0.f;

    landedDown = false;
    landedUp = false;
    landedLeft = false;
    landedRight = false;
    if(startX > endX ||
       startY > endY ||
       endX < 0 ||
       endY < 0 ||
       startX >= (int)(level.w) ||
       startY >= (int)(level.h))
    {
        return;
    }
    for(int sx = startX; sx <= endX; sx++)
    {
        for(int sy = startY; sy <= endY; sy++)
        {
            if(!level.getTile(sx, sy).solid)
                continue;
            float minDist = 1.0/0.f; /// INFINITY
            bool horizontal;

            if(level.getTile(sx, sy).left)// && x < sx * Tile::TILE_SIZE)
            {
                float dist = (sx * Tile::TILE_SIZE) - (x + w);
                if(fabs(dist) < fabs(minDist))
                {
                    minDist = dist;
                    horizontal = true;
                }
            }
            if(level.getTile(sx, sy).right)// && (x+w) > (sx+1) * Tile::TILE_SIZE)
            {
                float dist = ((sx+1) * Tile::TILE_SIZE) - x;

                if(fabs(dist) < fabs(minDist))
                {
                    minDist = dist;
                    horizontal = true;
                }
            }
            if(level.getTile(sx, sy).up)// && y < sy * Tile::TILE_SIZE)
            {
                float dist = (sy * Tile::TILE_SIZE) - (y + h);
                if(fabs(dist) < fabs(minDist))
                {
                    minDist = dist;
                    horizontal = false;
                }
            }
            if(level.getTile(sx, sy).down)// && (y+h) > (sy+1) * Tile::TILE_SIZE)
            {
                float dist = ((sy+1) * Tile::TILE_SIZE) - y;
                if(fabs(dist) < fabs(minDist))
                {
                    minDist = dist;
                    horizontal = false;
                }
            }
            if(horizontal)
            {
                if(fabs(minDist) < fabs(minDistX))
                {
                    minDistX = minDist;
                }
            }
            else
            {
                if(fabs(minDist) < fabs(minDistY))
                {
                    minDistY = minDist;
                }
            }
        }
    }

    if(fabs(minDistX) < 1000)
    {
        if(minDistX < 0)
        {
            if(dx >= 0)
            {
                x += minDistX;
                landedRight = true;
                dx = 0;
            }
        }
        else
        {
            if(dx <= 0)
            {
                x += minDistX;
                landedLeft = true;
                dx = 0;
            }
        }

    }
    if(fabs(minDistY) < 1000)
    {
        if(minDistY < 0)
        {
            if(dy >= 0)
            {
                y += minDistY;
                landedDown = true;
                dy = 0;
            }
        }
        else
        {
            if(dy <= 0)
            {
                y += minDistY;
                landedUp = true;
                dy = 0;
            }
        }
    }
}
