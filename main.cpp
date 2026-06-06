// read from xml file
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "scene_reader.h"
#include "scene.h"
#include "renderer.h"

using namespace std;

int main()
{
    SceneReader sceneReader;

    Renderer renderer;
    renderer.RenderScene(sceneReader.ReadScene("example5.xml"));

    return 0;
}