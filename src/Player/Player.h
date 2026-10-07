#pragma once
#include <SFML/Graphics.hpp>
#include "PlayerInput.h"
#include "Liquid.h"

class Player
{
private:
    sf::Texture m_texture;
    sf::Sprite m_sprite;
    sf::Vector2f m_velocity;
    float m_facingDirection;
    bool m_isGrounded;
    bool m_canAirJump;
    bool m_isDashing;
	float m_dashTimer;
	float m_dashDirection;
    bool m_canAirDash;
    float m_cooldownDash;
    void Move(const PlayerInput& input,  const float movementSpeedScale, const float friction, const float dt);
    void UpdateDirection();
    void Jump(bool& isJump, const bool allowsJump);
    void ApplyGravity(const float gravity, const float gravityScale, const bool isJumpHeld, const float dt);
    void StartDash(bool& isDash, const bool allowsDash);
    void UpdateDash(const float dt);

public:
    Player();
    void Update(
        PlayerInput& input, 
        const float friction, 
        const float gravity, 
        const LiquidProperties liquidResult, 
        const float dt
    );
    void Draw(sf::RenderWindow& window) const;
    sf::FloatRect GetBounds() const;
    sf::Vector2f GetVelocity() const;
    sf::Vector2f GetPosition() const;
    bool GetIsGrounded() const;
    void SetVelocity(const sf::Vector2f newVelocity);
    void SetPosition(const sf::Vector2f newPosition);
    void SetIsGrounded(const bool newIsGrounded);
    void StopDash();

};
