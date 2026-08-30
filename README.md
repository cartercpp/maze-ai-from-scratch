# maze-ai-from-scratch

A Q-learning agent that learns to walk a maze — implemented from scratch in C++23.

No ML libraries. A Q-table, an epsilon-greedy policy, a hardcoded grid, and a live terminal render.

YouTube video coming soon :)

## What you will see

A 20x30 maze in the terminal:

- yellow `_` — start
- green `_` — goal
- red wall blocks
- cyan `o` — the agent

The agent starts clumsy. After enough episodes it finds the goal more directly. Press **Enter** to quit.

## How the agent learns

This is tabular **Q-learning**.

State = cell index `row * columns + column`  
Actions = `UP`, `DOWN`, `LEFT`, `RIGHT`

Update after every step:

```
Q(s, a) += learningRate * (reward + discount * max Q(s', *) - Q(s, a))
```

### Rewards

| Event | Reward |
|---|---|
| Ordinary step | `-0.1` |
| Walk into a wall or off the map | `-1` (agent stays put) |
| Reach the goal | `+100` |

### Exploration

Epsilon-greedy:

- with probability `1 - epsilon`, take the action with the highest Q-value
- otherwise pick a random action

After each episode, `epsilon *= 0.99`. An episode ends when the agent hits the goal or takes `states * 3` steps.

Hyperparameters in `main.cpp`:

| Name | Default |
|---|---|
| `learningRate` | `0.1` |
| `discount` | `0.9` |
| starting `epsilon` | `0.5` |

## Requirements

- A C++23 compiler (`std::jthread`, `std::to_underlying`, `<format>`, `<ranges>`)
- CMake 4.3+
- A terminal that understands ANSI / truecolor and cursor positioning

## Build and run

```bash
git clone https://github.com/cartercpp/maze-ai-from-scratch.git
cd maze-ai-from-scratch
cmake -S . -B build
cmake --build build
./build/MazeAIFromScratch
```

## Project layout

```
CMakeLists.txt
main.cpp    # maze, Q-table, training loop, renderer
```

The maze is a compile-time `CELL grid[20][30]`. Change walls, start, or end there.

## Notes

- The Q-table is zero-initialized. Early episodes look random. That is expected.
- The binary named `main` in the repo is a leftover build artifact. Prefer building with CMake.
- Cursor is hidden while the demo runs and restored on exit.
