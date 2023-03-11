#include "game_object.h"


GameObject::GameObject()
    : Position(0.0f, 0.0f), Size(1.0f, 1.0f), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite(), IsSolid(false), Destroyed(false), acceleration(0.0f), upacceleration(0.0f), Rotate(0.0f), ImageSize(1.0f, 1.0f) { }

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite,float Rotation, float Rotate, glm::vec3 color, glm::vec2 velocity)
    : Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(Rotation), Sprite(sprite), IsSolid(false), Destroyed(false), acceleration(0.0f), upacceleration(0.0f), Rotate(Rotate), ImageSize(Size) { }

void GameObject::Draw(SpriteRenderer& renderer)
{
    this->Rotation += this->Rotate;
    if (this->Rotation >= 360)
        this->Rotation -= 360;
    /*glm::vec2 size = glm::vec2(200.0f, 50.0f);

    if (Rotation > 45 && Rotation < 135)
        size = glm::vec2(50.0f, 200.0f);

    this->Size = size;*/

    //if(this->Position.x > -size.x && this->Position.x < 1200 + size.x)

    renderer.DrawSprite(this->Sprite, this->Position, this->ImageSize, this->Rotation, this->Color);
}
