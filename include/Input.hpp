#pragma once

namespace Input {

enum class Key {
    None,
    Up,
    Down,
    Left,
    Right,
    Enter,
    Space,
    Char
};

struct Event {
    Key key{Key::None};
    char ch{0};
};

Event readKey();
void waitAnyKey();

} // namespace Input
