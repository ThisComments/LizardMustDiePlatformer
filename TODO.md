Ниже по каждому пункту из аудита: **что реально не так → что делать → мой вердикт по приоритету**. Я отдельно отмечаю места, где рекомендация аудитора слишком категорична или её стоит изменить под твою архитектуру.

## P1-3 — загрузка `hero.png`

**Проблема:** `Player` сам знает путь:

```cpp
"../assets/hero.png"
```

Это зависит от текущей рабочей директории. При запуске из другой директории загрузка может упасть. А исключение не перехватывается. 

**Исправление:**

Минимально:

```cpp
int main()
{
	try
	{
		Game game;
		game.Run();
	}
	catch (const sf::Exception& exception)
	{
		std::cerr << exception.what() << '\n';
		return 1;
	}

	return 0;
}
```

А путь лучше убрать из `Player`. В перспективе ресурсы должны загружаться не из entity.

**Приоритет: исправить.**

---

# P1-4 — позиция и hitbox внутри `sf::Sprite`

Это **один из самых важных пунктов аудита**.

Сейчас физика фактически зависит от картинки:

```cpp
m_sprite.getGlobalBounds()
```

Если поменяешь `hero.png`, изменится hitbox. Если сделаешь анимацию — разные кадры потенциально будут иметь разные bounds. 

### Правильно

`Player` должен иметь:

```cpp
sf::Vector2f m_position;
sf::Vector2f m_velocity;
sf::Vector2f m_size;
```

А sprite — только визуальное представление.

Например:

```text
Player
├── position
├── velocity
├── hitbox size
├── movement state
└── Sprite
```

`GetBounds()`:

```cpp
sf::FloatRect Player::GetBounds() const
{
	return sf::FloatRect{
		m_position - m_size / 2.f,
		m_size
	};
}
```

А перед рисованием:

```cpp
m_sprite.setPosition(m_position);
```

**Это надо сделать до серьёзной переработки CollisionSystem.**

**Приоритет: очень высокий.**

---

# P1-6 — начало dash

Аудитор здесь прав.

Сейчас:

```text
StartDash()
↓
Move()
↓
Gravity()
↓
Jump()
```

Поэтому dash не является отдельным состоянием.

Из-за этого `Move()` может изменить скорость dash с `1000` до `MAX_SPEED = 300`. 

### Правильно

```cpp
if (StartDash(input))
{
	UpdateDash(dt);
	return;
}
```

И перед стартом dash сначала определить направление:

```text
input
 ↓
UpdateDirection
 ↓
StartDash
 ↓
return
```

То есть если одновременно:

```text
A + Shift
```

dash должен идти влево.

**Приоритет: высокий.**

---

# P1-7 — dash и столкновения

Тоже реальная проблема.

Сейчас `UpdateDash()` каждый кадр снова делает:

```cpp
m_velocity.x = m_dashDirection * DASH_SPEED;
```

Поэтому если CollisionSystem поставил:

```cpp
velocity.x = 0;
```

следующий кадр dash снова выставит `1000`.



### Правильно

При горизонтальном столкновении:

```cpp
player.StopDash();
```

То есть:

```text
Dash
 ↓
Wall
 ↓
CollisionSystem
 ↓
StopDash()
```

И `m_canDash`/cooldown должны работать независимо от того, закончился dash естественно или столкновением.

**Приоритет: высокий.**

---

# P1-8 — модель горизонтального движения

Здесь сразу несколько проблем.

### 1. Friction работает только без input

Получается:

```text
D → ускорение
нет клавиши → friction
A → ускорение
```

То есть лёд фактически влияет только на торможение. 

Если это не задумано — исправить.

### 2. `groundFriction = 0` означает одновременно

```text
нет поверхности
```

и

```text
нулевое трение
```

Это разные состояния.

Лучше:

```cpp
bool isGrounded;
float groundFriction;
```

а friction использовать только если `isGrounded`.

### 3. A + D

Сейчас:

```text
A = true
D = true
```

может дать `isMoving = true`, хотя движение нулевое.

Нужно сначала вычислить направление:

```cpp
float moveDirection = 0.f;

if (input.moveLeft)
{
	moveDirection -= 1.f;
}

if (input.moveRight)
{
	moveDirection += 1.f;
}
```

И уже:

```cpp
if (moveDirection != 0.f)
```

### 4. `MAX_SPEED`

Аудитор прав: нельзя одним `MAX_SPEED` ограничивать вообще любую горизонтальную скорость.

Иначе:

```text
knockback
↓
velocity.x = 800
↓
MAX_SPEED
↓
300
```

Импульс уничтожен.

**Нужно разделить input velocity и external velocity.**

Но это я бы делал, когда появится первый knockback/пружина/ветер.

### 5. `drag`

Если `drag` пока не используется — либо подключить, либо удалить.

**Приоритет: высокий, особенно перед следующими механиками.**

---

# P1-9 — вода и прыжок

Это не баг, а **неопределённое игровое решение**.

Аудитор заметил:

```text
gravity × 0.3
```

при сохранении:

```text
jumpSpeed = -600
```

→ прыжок в воде становится значительно выше. 

Если это не задумано — исправить.

Я бы не делал автоматически `sqrt(gravityScale)`. Это математически сохраняет высоту прыжка, но не обязательно соответствует дизайну.

У тебя уже есть:

```cpp
jumpSpeedMultiplier
```

Поэтому лучше явно определить:

```cpp
WATER_PROPERTIES
{
	gravityScale = ...,
	moveSpeedScale = ...,
	jumpSpeedScale = ...
}
```

Например вода может:

```text
gravity ↓
speed ↓
jump ↓
```

И это уже нормальная игровая модель.

---

# P1-10 — выбор поверхности

Это реальный баг.

Сейчас если игрок стоит одновременно на:

```text
ICE | GROUND
```

то последняя подходящая поверхность в `vector` перезапишет предыдущую. 

### Правильно

`CollisionResult` должен выбрать **конкретный контакт с землёй**, а не просто последний найденный obstacle.

Для платформера достаточно правила:

> поверхность, которая находится непосредственно под нижней гранью игрока и имеет наибольшее горизонтальное пересечение.

После step-up нужно заново определить ground contact.

**Это стоит исправить вместе с новой axis-separated CollisionSystem.**

---

# P1-11 — копирование `Player`

Реальная проблема SFML.

`sf::Sprite` связан с `sf::Texture`. Поэтому бездумно копировать `Player` опасно. 

Минимальное решение:

```cpp
Player(const Player&) = delete;
Player& operator=(const Player&) = delete;
```

Но я бы ещё подумал о move.

С твоей текущей архитектурой `Player` вообще не должен копироваться.

**Приоритет: сделать сейчас, занимает минуты.**

---

# P1-12 — input

Здесь аудит предлагает несколько правильных вещей.

### `PlayerInput&`

Сейчас:

```cpp
Jump(PlayerInput& input)
```

и функция может сделать:

```cpp
input.jump = false;
```

Это действительно скрытый протокол потребления события. 

У тебя `KeyboardHandler` уже делает edge detection:

```text
Space pressed
↓
input.jump = true
```

Поэтому `Player` не должен менять input.

Лучше:

```cpp
void Player::Update(
	const PlayerInput& input,
	...
)
```

### Но есть важный нюанс

Тебе тогда надо убрать:

```cpp
input.jump = false;
```

и использовать внутреннее состояние/локальную переменную, если одно событие нельзя обработать дважды.

В твоей текущей структуре это вообще не проблема: `jump` уже создаётся как одноразовое событие.

### `isKeyPressed`

Да, лучше проверять:

```cpp
m_window.hasFocus()
```

чтобы игра не реагировала на клавиши, когда окно неактивно.

### Scancode

Это низкий приоритет.

**Приоритет: средний.**

---

# P1-13 — setters

Аудитор здесь прав.

Сейчас:

```cpp
SetIsGrounded(true);
```

делает больше, чем следует из названия: ещё восстанавливает air jump и dash. 

Это плохой API.

Лучше событие:

```cpp
void Player::Land(float groundFriction)
```

Внутри:

```text
grounded = true
canDoubleJump = true
canAirDash = true
groundFriction = ...
```

Но ещё лучше в твоей новой архитектуре:

```text
CollisionResult
 ↓
Player::ApplyCollisionResult(...)
```

И Player сам решает, какие игровые состояния изменить.

То есть CollisionSystem говорит **что произошло**, а не меняет внутреннюю игровую логику Player.

---

# P1-15 — инициализация

Аудитор прав на 100%.

Вот это:

```cpp
WATER_PROPERTIES = {0.3f, 0.6f, 5.0f, true, false}
```

плохо читается.

Непонятно:

```text
0.3 — что?
0.6 — что?
5.0 — что?
true — что?
false — что?
```

Если используется C++20, делай designated initialization:

```cpp
const LiquidProperties WATER_PROPERTIES{
	.gravityScale = 0.3f,
	.moveSpeedScale = 0.6f,
	.jumpSpeedScale = 5.f,
	.allowsDash = true,
	.allowsAirDash = false
};
```

А вот:

```cpp
friction: 100.f
```

действительно убрать. Это не стандартный C++.

**Приоритет: исправить.**

---

# P1-16 — границы World и камера

Здесь две отдельные проблемы.

### Дублирование

У тебя:

```text
World bounds
+
стены Obstacle
```

описывают одно и то же.

Это действительно потенциальный источник рассинхронизации. 

Но **я бы пока не удалял стены**.

`World::GetBounds()` отвечает за:

```text
камера
respawn
уровень
```

`Obstacle` отвечает за:

```text
физическую стену
```

Это разные системы, даже если сейчас значения совпадают.

### `std::clamp`

Если:

```text
world < camera
```

то:

```cpp
std::clamp(value, larger, smaller)
```

некорректен.

Нужно обработать случай:

```cpp
if (worldSize <= cameraSize)
{
	cameraCenter = worldCenter;
}
else
{
	cameraCenter.x = std::clamp(...);
}
```

**Приоритет: средний.**

---

# P1-17 — rendering

Здесь в основном оптимизация.

### Копирование

Было:

```cpp
for (const Obstacle obstacle : m_obstacles)
```

Нужно:

```cpp
for (const Obstacle& obstacle : m_obstacles)
```

### Создание `RectangleShape` каждый кадр

Да, лучше не создавать новый объект каждый кадр.

Но **это сейчас не важно** при 26 obstacles.

### Culling

Пока не нужен.

### Вода перекрывает игрока

Вот это уже не оптимизация, а потенциальный визуальный баг:

```text
Player
↓
Liquid
```

Если вода полностью непрозрачная, ноги пропадают.

Можно либо менять порядок, либо сделать жидкость полупрозрачной.

**Приоритет: низкий, кроме визуального слоя воды.**

---

# P1-18 — окно

Тут три вещи.

### 1500×1200

Для ноутбуков действительно может быть неудобно.

Я бы не зашивал размер окна как две отдельные константы.

```cpp
const sf::Vector2u WINDOW_SIZE{1500, 900};
```

### Resized

Да, нужно будет реагировать на `Resized`, особенно если окно можно менять.

### Closed

Если после:

```cpp
window.close();
```

цикл всё ещё делает:

```text
Update
Draw
```

— это надо исправить.

```cpp
while (m_window.isOpen())
```

и обработка `Closed`.

**Приоритет: средний.**

---

# P1-19 — include hygiene

Это в основном поддерживаемость.

### Вместо:

```cpp
#include "Player.h"
```

из любого места лучше:

```cpp
#include "Player/Player.h"
```

и сделать:

```text
src/
```

единственным include root.

### `World.h`

Если использует:

```cpp
std::vector
```

должен сам подключать:

```cpp
#include <vector>
```

Не полагаться на SFML.

### `Game.cpp`

Если используется:

```cpp
std::optional
```

должен быть:

```cpp
#include <optional>
```

### SFML

Вместо:

```cpp
#include <SFML/Graphics.hpp>
```

везде, где нужен только:

```cpp
sf::Vector2f
sf::FloatRect
```

лучше включать конкретные заголовки.

**Приоритет: низкий.**

---

# P1-20 — naming/style

Тут есть несколько действительно полезных исправлений.

### `position`

Здесь действительно важно договориться:

```text
Player::position = center
Obstacle::position = center
Liquid::position = center
```

а:

```cpp
getGlobalBounds().position
```

— top-left.

Это должно быть явно единообразно.

---

# P1-22 — `RenderTarget`

Сейчас:

```cpp
void Draw(sf::RenderWindow& window)
```

лучше:

```cpp
void Draw(sf::RenderTarget& target)
```

Потому что `RenderWindow` — конкретный тип окна, а `RenderTarget` — абстракция поверхности, куда можно рисовать.

Это даст возможность потом рисовать в:

```text
RenderWindow
RenderTexture
```

без изменения `Draw()`.

**Исправить. Стоимость почти нулевая.**

---

# Теперь FUTURE — F1–F12

Здесь уже не всё надо делать сейчас.

## F1 — `Body`

**Согласен. Это важнейшая архитектурная рекомендация из всей второй части.**

Сейчас:

```text
CollisionSystem → Player
```

Из-за этого система коллизий не может нормально работать с:

```text
Enemy
Projectile
MovingPlatform
```

без привязки к `Player`. 

Лучше:

```cpp
struct Body
{
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2f size;

	bool isGrounded;
	float groundFriction;
};
```

А:

```cpp
class Player
{
	Body m_body;
	...
};
```

Тогда:

```cpp
CollisionResult Resolve(
	Body& body,
	const std::vector<Obstacle>& obstacles
);
```

Это я бы делал **вместе с P1-4 и новой системой коллизий**.

---

# F2 — `World` станет God Object

Пока **не проблема**.

У тебя один игрок, один уровень, немного систем.

Не надо сейчас создавать:

```text
LevelManager
EntityManager
SceneManager
WorldManager
```

Когда появятся:

```text
Enemy
Projectile
второй уровень
```

тогда уже разделить данные уровня и runtime-состояние.

**Сейчас оставить.**

---

# F3 — состояние Player и velocity

Вот это **очень важный пункт на будущее**.

Сейчас velocity одновременно меняют:

```text
Move
Dash
Gravity
Collision
MAX_SPEED
```

Добавишь:

```text
knockback
wind
moving platform
bounce
```

и начнётся конфликт между системами. 

Я бы не вводил полноценный FSM прямо сейчас.

Но уже сейчас разделил бы:

```text
movement velocity
+
external velocity / impulse
```

А когда появятся 3+ сложных состояния:

```cpp
enum class MovementState
{
	Grounded,
	Airborne,
	Dashing,
	...
};
```

---

# F4 — `MovementContext`

Это хорошая идея **на следующий этап**.

Сейчас:

```cpp
Update(
	input,
	friction,
	gravity,
	liquidProperties,
	dt
);
```

будет постепенно превращаться в:

```cpp
Update(
	input,
	friction,
	gravity,
	liquidProperties,
	wind,
	slope,
	bounce,
	...
);
```

Поэтому при появлении следующего environmental effect:

```cpp
struct MovementContext
{
	float gravity;
	float groundFriction;
	MovementModifiers movementModifiers;
};
```

И передавать один объект.

**Сейчас не надо создавать ради одной жидкости.**

---

# F5 — Surface

Сейчас:

```cpp
Surface
{
	friction;
	visualId;
}
```

нормально.

Когда добавятся:

```text
bounce
damage
sound
particles
```

лучше:

```cpp
enum class SurfaceType
{
	Ground,
	Ice,
	Stone
};
```

и таблица свойств.

**Сейчас не надо.**

---

# F7 — GameState

Пока не нужен.

Когда появятся:

```text
Menu
Playing
Pause
GameOver
```

ввести:

```cpp
enum class GameState
{
	Menu,
	Playing,
	Paused,
	GameOver
};
```

До этого — лишняя абстракция.

---

# F8 — Rendering / assets

Аудитор предлагает texture cache только когда появится несколько пользователей одной текстуры. Это разумно. 

Сейчас **не создавать Renderer**.

Твоё текущее:

```text
Game
World
Player::Draw()
Obstacle::Draw()
```

достаточно.

Но загрузку ресурсов из `Player` я бы постепенно вынес.

---

# F9 — enemies / attacks

Пока ничего делать.

Но **до появления attack/damage** надо определить правила:

```text
dash + attack?
dash + invulnerability?
attack во время dash?
knockback во время dash?
```

Это важнее создания отдельных классов.

---

# F10 — respawn

А вот это я бы сделал уже сейчас.

Потому что ты всё равно добавляешь:

```text
MAX_FALL_SPEED
kill plane
```

Нужен:

```cpp
void Reset(sf::Vector2f position);
```

И spawn не должен быть захардкожен внутри `Player`.

Например `World` хранит:

```cpp
sf::Vector2f m_playerSpawn;
```

---

# F11 — PlayerTuning

Сейчас нормально, что константы разбросаны.

Но когда начнёшь активно настраивать:

```text
acceleration
max speed
jump
dash
gravity
friction
```

сделать:

```cpp
struct PlayerTuning
{
	float acceleration;
	float maxSpeed;
	float jumpSpeed;
	float dashSpeed;
	float dashDuration;
	...
};
```

И передавать его в `Player`.

**Сейчас можно не трогать.**

---

# F12 — тесты

Здесь я согласен особенно сильно.

После того как ты вынесешь `Body` и сделаешь нормальный `CollisionSystem`, его можно тестировать **без SFML окна**.

Минимум тестов:

```text
standing on ground
landing
hitting left wall
hitting right wall
hitting ceiling
step up
step too high
two adjacent platforms
ice friction
ground friction
ground selection at seam
```

Именно CollisionSystem — первое место, куда тесты реально принесут пользу.

---

# Что я бы реально сделал сейчас

Не надо выполнять все 34 пункта подряд. Получится бессмысленный рефакторинг.

Я бы изменил архитектуру в таком порядке:

### Этап 1 — фундамент

1. `dt` clamp + frame limit.
2. `MAX_FALL_SPEED`.
3. respawn/kill plane.
4. обработка исключения загрузки ресурсов.
5. запрет копирования `Player`.

### Этап 2 — физическое тело

6. Убрать позицию и hitbox из `sf::Sprite`.
7. Создать `Body`.
8. `Player` владеет `Body`.
9. `CollisionSystem` работает с `Body`, а не напрямую со `Player`.

### Этап 3 — полностью переделать CollisionSystem

10. Перейти с MTV:

```text
intersection size.x < size.y
```

на:

```text
Move X
→ Resolve X
→ Step-up

Move Y
→ Resolve Y
→ Ground
```

11. Нормально определять `groundFriction`.
12. Сделать `CollisionResult`.
13. После collision передавать результат игроку.

### Этап 4 — Player

14. Исправить dash.
15. `StartDash()` → успешный старт → `return`.
16. `CollisionSystem` может `StopDash()`.
17. Убрать изменение `PlayerInput`.
18. Убрать `SetIsGrounded()` с игровой логикой.

### Этап 5 — жидкости

19. Переименовать `LiquidProperties` в более общий тип **только если действительно планируются другие зоны**.
20. Сделать безопасный `LiquidResult`.
21. Определить, как именно вода влияет на jump.
22. Добавить приоритет/правило пересекающихся жидкостей, если они могут пересекаться.

### Этап 6 — после этого

23. `RenderTarget&`.
24. include hygiene.
25. naming.
26. camera edge cases.
27. тесты CollisionSystem.

---

Самая существенная рекомендация из всего аудита: **не продолжай сейчас навешивать новые механики на существующую физику**. У тебя уже появилась вода, dash, step-up, friction и variable jump — именно сейчас нужно стабилизировать `Body → movement → collision → environment result → Player state`.

Иначе следующая механика будет не добавляться, а заставлять переписывать предыдущие. Аудит в этом месте попал точно: текущая архитектура **не требует переписывания проекта**, но `Player + CollisionSystem + порядок `World::Update`` сейчас действительно являются тем местом, которое надо привести в порядок до дальнейшего расширения. 

Переделать колизию на ту которая в лекции
Переделать карту на тайловую систему
stateManager у игрока реализовать