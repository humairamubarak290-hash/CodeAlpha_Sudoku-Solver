#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vector>
#include <string>
#include <cstdlib>

// -------------------------------------------------------------------
// Sudoku Solver Core (Backtracking)
// -------------------------------------------------------------------
class SudokuSolver {
private:
    int grid[9][9];

    bool isSafe(int row, int col, int num) const {
        for (int c = 0; c < 9; ++c)
            if (grid[row][c] == num) return false;

        for (int r = 0; r < 9; ++r)
            if (grid[r][col] == num) return false;

        int startRow = row - row % 3;
        int startCol = col - col % 3;
        for (int r = 0; r < 3; ++r)
            for (int c = 0; c < 3; ++c)
                if (grid[startRow + r][startCol + c] == num) return false;

        return true;
    }

    bool solveRecursive() {
        for (int row = 0; row < 9; ++row) {
            for (int col = 0; col < 9; ++col) {
                if (grid[row][col] == 0) {
                    for (int num = 1; num <= 9; ++num) {
                        if (isSafe(row, col, num)) {
                            grid[row][col] = num;
                            if (solveRecursive()) return true;
                            grid[row][col] = 0;
                        }
                    }
                    return false;
                }
            }
        }
        return true;
    }

public:
    SudokuSolver() { clear(); }

    void clear() {
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                grid[i][j] = 0;
    }

    void setCell(int row, int col, int value) {
        if (row >= 0 && row < 9 && col >= 0 && col < 9 && value >= 0 && value <= 9)
            grid[row][col] = value;
    }

    int getCell(int row, int col) const {
        return grid[row][col];
    }

    bool solve() {
        return solveRecursive();
    }
};

// -------------------------------------------------------------------
// GUI
// -------------------------------------------------------------------
const int CELL_SIZE = 40;
const int GRID_OFFSET_X = 20;
const int GRID_OFFSET_Y = 40;
const int BUTTON_WIDTH = 80;
const int BUTTON_HEIGHT = 30;

HWND hEdit[9][9];
SudokuSolver solver;
HWND hStatus;
HWND hSolveBtn, hClearBtn;
HWND g_hwnd; // global window handle

// -------------------------------------------------------------------

void UpdateGUI() {
    char buf[2];
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            int val = solver.getCell(r, c);
            if (val != 0) {
                buf[0] = '0' + val;
                buf[1] = '\0';
                SetWindowTextA(hEdit[r][c], buf);
            } else {
                SetWindowTextA(hEdit[r][c], "");
            }
        }
    }
}

void ReadFromGUI() {
    char buf[2] = {0, 0};
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            GetWindowTextA(hEdit[r][c], buf, 2);
            if (buf[0] >= '1' && buf[0] <= '9')
                solver.setCell(r, c, buf[0] - '0');
            else
                solver.setCell(r, c, 0);
        }
    }
}

void SetStatusMessage(const char* msg) {
    SetWindowTextA(hStatus, msg);
}

void OnSolve() {
    ReadFromGUI();
    SetStatusMessage("Solving...");
    if (solver.solve()) {
        UpdateGUI();
        SetStatusMessage("Solved!");
    } else {
        SetStatusMessage("No solution exists!");
    }
    InvalidateRect(g_hwnd, NULL, TRUE); // redraw grid
}

void OnClear() {
    solver.clear();
    UpdateGUI();
    SetStatusMessage("Grid cleared.");
    InvalidateRect(g_hwnd, NULL, TRUE); // redraw grid
}

// -------------------------------------------------------------------

void CreateEditControls(HWND hwnd) {
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            int x = GRID_OFFSET_X + c * CELL_SIZE;
            int y = GRID_OFFSET_Y + r * CELL_SIZE;

            hEdit[r][c] = CreateWindowExA(
                WS_EX_CLIENTEDGE, "EDIT", "",
                WS_CHILD | WS_VISIBLE | ES_CENTER | ES_NUMBER | WS_BORDER,
                x, y, CELL_SIZE, CELL_SIZE,
                hwnd, NULL, GetModuleHandle(NULL), NULL);

            SendMessageA(hEdit[r][c], EM_LIMITTEXT, 1, 0);
        }
    }
}

// -------------------------------------------------------------------

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {

    case WM_CREATE: {
        g_hwnd = hwnd;

        CreateEditControls(hwnd);

        hSolveBtn = CreateWindowExA(0, "BUTTON", "Solve",
            WS_CHILD | WS_VISIBLE,
            GRID_OFFSET_X, GRID_OFFSET_Y + 9 * CELL_SIZE + 10,
            BUTTON_WIDTH, BUTTON_HEIGHT,
            hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);

        hClearBtn = CreateWindowExA(0, "BUTTON", "Clear",
            WS_CHILD | WS_VISIBLE,
            GRID_OFFSET_X + BUTTON_WIDTH + 10, GRID_OFFSET_Y + 9 * CELL_SIZE + 10,
            BUTTON_WIDTH, BUTTON_HEIGHT,
            hwnd, (HMENU)2, GetModuleHandle(NULL), NULL);

        hStatus = CreateWindowExA(0, "STATIC", "Ready",
            WS_CHILD | WS_VISIBLE | SS_SUNKEN,
            GRID_OFFSET_X, GRID_OFFSET_Y + 9 * CELL_SIZE + 50,
            250, 25, hwnd, NULL, GetModuleHandle(NULL), NULL);
        break;
    }

    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case 1: OnSolve(); break;
        case 2: OnClear(); break;
        }
        break;
    }

    // ⭐ DRAW GRID LINES HERE
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        HPEN thinPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HPEN thickPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));

        HPEN oldPen = (HPEN)SelectObject(hdc, thinPen);

        for (int i = 0; i <= 9; i++) {
            int x = GRID_OFFSET_X + i * CELL_SIZE;
            int y = GRID_OFFSET_Y + i * CELL_SIZE;

            SelectObject(hdc, (i % 3 == 0) ? thickPen : thinPen);

            // vertical
            MoveToEx(hdc, x, GRID_OFFSET_Y, NULL);
            LineTo(hdc, x, GRID_OFFSET_Y + 9 * CELL_SIZE);

            // horizontal
            MoveToEx(hdc, GRID_OFFSET_X, y, NULL);
            LineTo(hdc, GRID_OFFSET_X + 9 * CELL_SIZE, y);
        }

        SelectObject(hdc, oldPen);
        DeleteObject(thinPen);
        DeleteObject(thickPen);

        EndPaint(hwnd, &ps);
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProcA(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// -------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {

    const char CLASS_NAME[] = "SudokuSolverWindow";

    WNDCLASSEXA wc = {0};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassExA(&wc);

    int windowWidth = GRID_OFFSET_X * 2 + 9 * CELL_SIZE;
    int windowHeight = GRID_OFFSET_Y + 9 * CELL_SIZE + 100;

    HWND hwnd = CreateWindowExA(0, CLASS_NAME, "Sudoku Solver",
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,
        CW_USEDEFAULT, CW_USEDEFAULT,
        windowWidth, windowHeight,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}