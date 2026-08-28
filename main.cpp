#include <iostream>
#include <format>
#include <utility>
#include <ranges>
#include <algorithm>
#include <random>
#include <thread>
#include <stop_token>
#include <chrono>

int main()
{
    enum class CELL{START, OPEN, CLOSED, END};
    enum class ACTION{UP, DOWN, LEFT, RIGHT};

    constexpr int rows = 20,
                  columns = 30,
                  states = rows * columns,
                  actions = 4;

    CELL grid[rows][columns] = {
        { CELL::START, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN },
        { CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN },
        { CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::CLOSED, CELL::CLOSED, CELL::OPEN },
        { CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::CLOSED, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED },
        { CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN },
        { CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN },
        { CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::CLOSED, CELL::CLOSED, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::CLOSED, CELL::OPEN },
        { CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN },
        { CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::CLOSED, CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN },
        { CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN },
        { CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN },
        { CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::CLOSED, CELL::CLOSED, CELL::OPEN, CELL::CLOSED },
        { CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::OPEN },
        { CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN },
        { CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::CLOSED, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN },
        { CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN },
        { CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN },
        { CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::CLOSED, CELL::CLOSED, CELL::CLOSED, CELL::CLOSED, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN },
        { CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN },
        { CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::CLOSED, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::OPEN, CELL::END }
    };

    double qTable[states][actions]{};

    constexpr double learningRate = 0.1,
                     discount = 0.9;
    double epsilon = 0.5;

    std::random_device rd;
    std::uniform_real_distribution<double> randomChance(0, 1);
    std::uniform_int_distribution<int> actionDist(0, actions - 1);

    std::cout << "\033[?25l"; // cursor off
    std::cout << "\033[2J"; // clear screen

    {
        std::jthread thr{[&](std::stop_token st) {
            int agentRow = 0,
                agentColumn = 0,
                stepsTaken = 0;

            while (!st.stop_requested())
            {
                std::cout << "\033[H";
                for (int row = 0; row < rows; ++row)
                {
                    for (int column = 0; column < columns; ++column)
                        switch (grid[row][column])
                        {
                            case CELL::START:
                                std::cout << "\033[38;2;255;255;0m_"; // start cell is yellow underscore
                                break;
                            case CELL::OPEN:
                                std::cout << "\033[38;2;255;255;255m "; // open cell is blank
                                break;
                            case CELL::CLOSED:
                                std::cout << "\033[38;2;255;0;0m█"; // closed cell is red
                                break;
                            case CELL::END:
                                std::cout << "\033[38;2;0;255;0m_"; // end cell is green underscore
                                break;
                        }

                    std::cout << '\n';
                }
                std::cout << std::format("\033[{};{}H", agentRow + 1, agentColumn + 1); // move cursor
                std::cout << "\033[38;2;0;255;255mo"; // agent is a cyan circle
                std::cout << std::flush;

                if ((grid[agentRow][agentColumn] != CELL::END) && (stepsTaken < states * 3))
                {
                    ACTION actionTaken;

                    if (randomChance(rd) > epsilon)
                    {
                        const int state = agentRow * columns + agentColumn;
                        auto& stateRow = qTable[state];
                        /*
                         * stateRow[0] = how good is UP
                         * stateRow[1] = how good is DOWN
                         * stateRow[2] = how good is LEFT
                         * stateRow[3] = how good is RIGHT
                         */

                        actionTaken
                        = static_cast<ACTION>(std::ranges::max_element(stateRow) - std::ranges::begin(stateRow));
                    }
                    else
                        actionTaken = static_cast<ACTION>(actionDist(rd));

                    int newAgentRow = agentRow,
                        newAgentColumn = agentColumn;
                    double reward = -0.1;

                    switch (actionTaken)
                    {
                        case ACTION::UP:
                        {
                            if ((agentRow > 0) && (grid[agentRow - 1][agentColumn] != CELL::CLOSED))
                                newAgentRow = agentRow - 1;
                            else
                                reward = -1;

                            break;
                        }
                        case ACTION::DOWN:
                        {
                            if ((agentRow + 1 < rows) && (grid[agentRow + 1][agentColumn] != CELL::CLOSED))
                                newAgentRow = agentRow + 1;
                            else
                                reward = -1;

                            break;
                        }
                        case ACTION::LEFT:
                        {
                            if ((agentColumn > 0) && (grid[agentRow][agentColumn - 1] != CELL::CLOSED))
                                newAgentColumn = agentColumn - 1;
                            else
                                reward = -1;

                            break;
                        }
                        case ACTION::RIGHT:
                        {
                            if ((agentColumn + 1 < columns) && (grid[agentRow][agentColumn + 1] != CELL::CLOSED))
                                newAgentColumn = agentColumn + 1;
                            else
                                reward = -1;

                            break;
                        }
                    }

                    if (grid[newAgentRow][newAgentColumn] == CELL::END)
                        reward = 100;

                    const int state = agentRow * columns + agentColumn,
                              newState = newAgentRow * columns + newAgentColumn;

                    auto& stateRow = qTable[state];
                    auto& newStateRow = qTable[newState];

                    const int actionIndex = std::to_underlying(actionTaken);
                    stateRow[actionIndex]
                    += learningRate * (reward + discount * std::ranges::max(newStateRow) - stateRow[actionIndex]);

                    agentRow = newAgentRow;
                    agentColumn = newAgentColumn;
                    ++stepsTaken;
                }
                else
                {
                    agentRow = agentColumn = 0;
                    stepsTaken = 0;
                    epsilon *= 0.99;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }};
        std::cin.get();
    }

    std::cout << "\033[?25h"; // restore cursor
}