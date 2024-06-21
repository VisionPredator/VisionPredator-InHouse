#include "../VPEngine/framework.h"
#include "GameProcessor.h"
#include "VPEngine.h"

GameProcessor::GameProcessor(HINSTANCE hInstance, std::string title, int width, int height) :VPEngine(hInstance, title, width, height)
{
}

GameProcessor::~GameProcessor()
{

}
