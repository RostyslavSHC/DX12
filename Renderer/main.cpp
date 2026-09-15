#include "GameController.h" // GameController

// Entry point, main function
int WINAPI main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    GameController* gc = &GameController::GetInstance(); // Get the game controller
    gc->Initialize(hInstance, nShowCmd); // Initialize
    gc->Run(); // Run the game loop
}
