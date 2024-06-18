#include "GameProcessor.h"
#include "VPEngine.h"
#include "SystemManager.h"

GameProcessor::GameProcessor(HINSTANCE hInstance, std::string title, int width, int height) :VPEngine(hInstance, title, width, height)
{
}

GameProcessor::~GameProcessor()
{

}
