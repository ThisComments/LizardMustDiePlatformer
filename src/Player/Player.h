#pragma once
#include <SFML/Graphics.hpp>
#include "../KeyboardHandler/PlayerInput.h"

struct MovementModifiers
{
	float gravityScale = 1.f;
	float movementSpeedScale = 1.f;
	bool allowsJump = true;
	bool allowsDash = true;
};

struct EnvironmentState
{
	bool isGrounded = false;
	float gravity = 0.f;
	float groundFriction = 0.f;
	MovementModifiers movementModifiers;
};

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
    float m_dashCooldown;
    void Move(const PlayerInput& input,  const float movementSpeedScale, const float friction, const float dt);
    void UpdateDirection();
    void Jump(bool& isJump, const bool allowsJump);
    void ApplyGravity(const float gravity, const float gravityScale, const bool isJumpHeld, const float dt);
    void StartDash(bool& isDash, const bool allowsDash);
    void UpdateDash(const float dt);
    void SetIsGrounded(const bool newIsGrounded);

public:
    Player();
    void Update(
        PlayerInput& input, 
        EnvironmentState& environmentState, 
        const float dt
    );
    void Draw(sf::RenderTarget& window) const;
    sf::FloatRect GetBounds() const;
    sf::Vector2f GetVelocity() const;
    sf::Vector2f GetPosition() const;
    bool IsGrounded() const;
    void SetVelocity(const sf::Vector2f newVelocity);
    void SetPosition(const sf::Vector2f newPosition);
    void StopDash();

};
