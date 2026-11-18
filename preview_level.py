import sys
from pathlib import Path

SOLID_TILES = {'G', 'S', 'P', 'I', 'N', 'R', 'T'}
LIQUID_TILES = {'W', 'L'}
BG_TILES = {'b', 'r'}
ALL_TILES = SOLID_TILES | LIQUID_TILES | BG_TILES | {'.'}

ANSI_COLORS = {
    '@': '\033[96m',
    'F': '\033[92m',
    'c': '\033[93m',
    'g': '\033[95m',
    'L': '\033[91m',
    'W': '\033[94m',
    'I': '\033[36m',
    'b': '\033[90m',
    'r': '\033[90m',
}
RESET = '\033[0m'


def parse_level(filepath: Path):
    lines = filepath.read_text(encoding='utf-8').splitlines()
    grid = []
    object_lines = []
    in_objects = False

    for line_num, raw_line in enumerate(lines, start=1):
        line = raw_line.rstrip('\r')
        if not line:
            continue
        if line == '[objects]':
            in_objects = True
            continue
        if in_objects:
            object_lines.append((line_num, line))
        else:
            for ch in line:
                if ch not in ALL_TILES:
                    raise ValueError(f"Строка {line_num}: неизвестный символ тайла '{ch}'")
            grid.append(list(line))

    if not grid:
        raise ValueError("Карта пустая")

    width = len(grid[0])
    height = len(grid)
    for idx, row in enumerate(grid, start=1):
        if len(row) != width:
            raise ValueError(f"Строка {idx}: длина {len(row)}, ожидалось {width}")

    player = None
    finish = None
    collectibles = []

    for line_num, line in object_lines:
        parts = line.split()
        if not parts or parts[0].startswith('#'):
            continue

        obj_type = parts[0]
        if obj_type == 'player':
            col, row = int(parts[1]), int(parts[2])
            player = (col, row)
        elif obj_type == 'finish':
            col, row, w, h = int(parts[1]), int(parts[2]), int(parts[3]), int(parts[4])
            finish = (col, row, w, h)
        elif obj_type in ('coin', 'gem'):
            col, row = int(parts[1]), int(parts[2])
            collectibles.append((obj_type, col, row, line_num))
        else:
            print(f"[!] Строка {line_num}: неизвестный объект '{obj_type}'")

    return grid, width, height, player, finish, collectibles


def check_issues(grid, width, height, player, finish, collectibles):
    issues = []

    def is_inside(c, r):
        return 0 <= c < width and 0 <= r < height

    if player:
        pc, pr = player
        if not is_inside(pc, pr):
            issues.append(f"Игрок ({pc}, {pr}) за пределами карты")
        elif grid[pr][pc] in SOLID_TILES:
            issues.append(f"Игрок ({pc}, {pr}) застрял в твёрдом тайле '{grid[pr][pc]}'")
        elif pr > 0 and grid[pr - 1][pc] in SOLID_TILES:
            issues.append(f"Игрок ({pc}, {pr}) упирается головой в '{grid[pr - 1][pc]}' (хитбокс 40x80 = 2 тайла в высоту!)")
        elif pr + 1 < height and grid[pr + 1][pc] not in SOLID_TILES:
            issues.append(f"Игрок ({pc}, {pr}) висит в воздухе над '{grid[pr + 1][pc]}'")

    if finish:
        fc, fr, fw, fh = finish
        for r in range(fr, fr + fh):
            for c in range(fc, fc + fw):
                if is_inside(c, r) and grid[r][c] in SOLID_TILES:
                    issues.append(f"Зона финиша перекрывает твёрдый тайл '{grid[r][c]}' в ({c}, {r})")
                    break

    seen = {}
    for obj_type, c, r, line_num in collectibles:
        if not is_inside(c, r):
            issues.append(f"Строка {line_num}: {obj_type} ({c}, {r}) за пределами карты")
            continue
        tile = grid[r][c]
        if tile in SOLID_TILES:
            issues.append(f"Строка {line_num}: {obj_type} ({c}, {r}) заспавнился ВНУТРИ стены '{tile}'")
        elif tile == 'L':
            issues.append(f"Строка {line_num}: {obj_type} ({c}, {r}) заспавнился ВНУТРИ лавы 'L'")
        if (c, r) in seen:
            issues.append(f"Строка {line_num}: {obj_type} ({c}, {r}) дублирует позицию с {seen[(c, r)]}")
        seen[(c, r)] = obj_type

    return issues


def render_preview(grid, width, height, player, finish, collectibles, use_color=True):
    overlay = [row[:] for row in grid]

    if finish:
        fc, fr, fw, fh = finish
        for r in range(fr, min(height, fr + fh)):
            for c in range(fc, min(width, fc + fw)):
                if overlay[r][c] in ('.', 'b', 'r'):
                    overlay[r][c] = 'F'

    for obj_type, c, r, _ in collectibles:
        if 0 <= c < width and 0 <= r < height:
            overlay[r][c] = 'c' if obj_type == 'coin' else 'g'

    if player:
        pc, pr = player
        if 0 <= pc < width and 0 <= pr < height:
            overlay[pr][pc] = '@'

    # Линейка координат X сверху (десятки и единицы)
    tens = "    " + "".join(str((c // 10) % 10) if c % 5 == 0 else " " for c in range(width))
    ones = "    " + "".join(str(c % 10) for c in range(width))
    print(tens)
    print(ones)

    for r in range(height):
        row_chars = []
        for c in range(width):
            ch = overlay[r][c]
            if use_color and ch in ANSI_COLORS:
                row_chars.append(f"{ANSI_COLORS[ch]}{ch}{RESET}")
            else:
                row_chars.append(ch)
        print(f"{r:02d}: " + "".join(row_chars))


def main():
    level_path = Path(sys.argv[1]) if len(sys.argv) > 1 else Path("levels/test_level.txt")
    if not level_path.exists():
        print(f"Файл не найден: {level_path}")
        sys.exit(1)

    grid, width, height, player, finish, collectibles = parse_level(level_path)
    issues = check_issues(grid, width, height, player, finish, collectibles)

    coins = sum(1 for t, _, _, _ in collectibles if t == 'coin')
    gems = sum(1 for t, _, _, _ in collectibles if t == 'gem')

    print(f"=== УРОВЕНЬ: {level_path} ({width}x{height}) ===")
    print(f"Игрок: {player} | Финиш: {finish} | Монет (c): {coins} | Самоцветов (g): {gems}")
    print("Легенда: @ — игрок, F — финиш, c — монета, g — самоцвет\n")

    render_preview(grid, width, height, player, finish, collectibles)

    print("\n=== ПРОВЕРКА СПАВНОВ ===")
    if not issues:
        print("OK! Все объекты стоят на свободных клетках.")
    else:
        for issue in issues:
            print(f"[!] {issue}")


if __name__ == "__main__":
    main()