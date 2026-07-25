# Fogbound

Мультиплеерный прототип на **Unreal Engine 5.6** (Blueprints + C++).  
Co-op через **Steam** (listen server, SteamSockets, Advanced Sessions).

## Стек

- Unreal Engine **5.6**
- C++: персонаж, инвентарь, usable items, интерфейсы Interact/Use
- Blueprints: UI, меню, сессии, игровой flow
- Плагины: AdvancedSessions, AdvancedSteamSessions, SteamSockets

## Требования

- UE 5.6
- Visual Studio (для сборки C++)
- Steam (для онлайн-сессий)

## Обязательный ассет: HorrorBasement

Папка `Content/HorrorBasement/` **не входит в репозиторий** (слишком большой для GitHub).

Её нужно скачать отдельно и положить в проект:

1. Скачай пак **HorrorBasement** (Epic Marketplace / Fab — тот же, что использовался в проекте).
2. Распакуй / скопируй содержимое так, чтобы путь был:

```text
Fogbound/Content/HorrorBasement/
```

3. Открой `Fogbound.uproject` — Unreal подхватит ассеты после рестарта редактора при необходимости.

Без этой папки уровни и объекты, которые на неё ссылаются, будут с missing assets.

## Быстрый старт

1. Клонируй репозиторий:

```bash
git clone git@github.com:Nikody456/Fogbound.git
cd Fogbound
```

2. Установи **HorrorBasement** в `Content/HorrorBasement/` (см. выше).
3. Открой `Fogbound.uproject` → Compile / открой в Editor.
4. Для мультиплеера запусти через Steam / listen server по текущему flow в Main Menu.

## Структура (кратко)

| Путь | Что внутри |
|------|------------|
| `Source/Fogbound/` | C++ gameplay (BaseCharacter, Inventory, UsableItem) |
| `Content/_Main/` | Свои BP, UI, карты, контроллеры |
| `Content/HorrorBasement/` | Внешний пак (локально, не в git) |
| `Plugins/` | AdvancedSessions / AdvancedSteamSessions |
| `Config/` | Настройки проекта, Steam, input |

## Примечание по git

Крупный контент `HorrorBasement` игнорируется в `.gitignore` из‑за лимита размера push на GitHub (~2 GiB). Остальной проект в репозитории.
