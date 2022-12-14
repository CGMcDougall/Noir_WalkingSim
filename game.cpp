#include <iostream>
#include <time.h>
#include <sstream>

#include "game.h"
#include "path_config.h"
#include <random>
#include <glm/gtx/string_cast.hpp>

namespace game {

// Some configuration constants
// They are written here as global variables, but ideally they should be loaded from a configuration file

// Main window settings
const std::string window_title_g = "Demo";

//const unsigned int window_width_g = 800;
//const unsigned int window_height_g = 600;

const unsigned int window_width_g = 800*2;
const unsigned int window_height_g = 600*1.5;


const bool window_full_screen_g = false;

// Viewport and camera settings
float camera_near_clip_distance_g = 0.01;
float camera_far_clip_distance_g = 1000.0;
float camera_fov_g = 20.0; // Field-of-view of camera
const glm::vec3 viewport_background_color_g(0.0, 0.0, 0.0);
glm::vec3 camera_position_g(0.5, 5.0, 20.0);
glm::vec3 camera_look_at_g(0.0, 5.0, 0.0);
glm::vec3 camera_up_g(0.0, 1.0, 0.0);

// Materials 
const std::string material_directory_g = MATERIAL_DIRECTORY;

//Cigarette* game::Cig = NULL;

Game::Game(void){

    // Don't do work in the constructor, leave it for the Init() function
}


void Game::Init(void){

    // Run all initialization steps
    InitWindow();
    InitView();
    InitEventHandlers();

    // Set variables
    animating_ = true;
    blur_ = false;
    noir_ = true;
    cameraLocked_ = true;

    try {
        //am.Init(NULL);
    }
    catch (std::exception& e) {
        std::cout<<"Problem occured with audio device" << std::endl;
    }
    
}

       
void Game::InitWindow(void){

    // Initialize the window management library (GLFW)
    if (!glfwInit()){
        throw(GameException(std::string("Could not initialize the GLFW library")));
    }

    // Create a window and its OpenGL context
    if (window_full_screen_g){
        window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g.c_str(), glfwGetPrimaryMonitor(), NULL);
    } else {
        window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g.c_str(), NULL, NULL);
    }
    if (!window_){
        glfwTerminate();
        throw(GameException(std::string("Could not create window")));
    }

    // Make the window's context the current one
    glfwMakeContextCurrent(window_);

    // Initialize the GLEW library to access OpenGL extensions
    // Need to do it after initializing an OpenGL context
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK){
        throw(GameException(std::string("Could not initialize the GLEW library: ")+std::string((const char *) glewGetErrorString(err))));
    }
}


void Game::InitView(void){

    // Set up z-buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Set viewport
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    glViewport(0, 0, width, height);

    // Set up camera
    // Set current view
    camera_.SetView(camera_position_g, camera_look_at_g, camera_up_g);
    // Set projection
    camera_.SetProjection(camera_fov_g, camera_near_clip_distance_g, camera_far_clip_distance_g, width, height);
}


void Game::InitEventHandlers(void){

    // Set event callbacks
    glfwSetKeyCallback(window_, KeyCallback);
    glfwSetFramebufferSizeCallback(window_, ResizeCallback);

    glfwSetCursorPosCallback(window_, CursorCallback);
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //if(glfwRawMouseMotionSupported())glfwSetInputMode(window_, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    
    oldMouseX = 0;
    oldMouseY = 0;

   
    // Set pointer to game object, so that callbacks can access it
    glfwSetWindowUserPointer(window_, (void *) this);
}


void Game::SetupResources(void){

    // Particle Shaders
    std::string filename = std::string(MATERIAL_DIRECTORY) + std::string("\\particle");
    resman_.LoadResource(Material, "ParticleMaterial", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\rain_particles");
    resman_.LoadResource(Material, "RainMaterial", filename.c_str());


    // Material Shaders
    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\noir");
    resman_.LoadResource(Material, "Noir", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\road_noir");
    resman_.LoadResource(Material, "RoadNoir", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\cigarette");
    resman_.LoadResource(Material, "Cigarette", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\metal");
    resman_.LoadResource(Material, "Metal", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\tiled_brick");
    resman_.LoadResource(Material, "TiledBrick", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\skybox");
    resman_.LoadResource(Material, "Skybox", filename.c_str());


    // Environment Objects
    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/StraightRoad1.obj");
    resman_.LoadResource(Mesh, "Rd1", filename.c_str());
  
    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/Car2.obj");
    resman_.LoadResource(Mesh, "car", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/Street_Lamp_TS.obj");
    resman_.LoadResource(Mesh, "streetlamp", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/Grate/GrateModel.obj");
    resman_.LoadResource(Mesh, "Grate", filename.c_str());
    
    /* filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/TrafficLight/Traffic_Lights.obj");
    resman_.LoadResource(Mesh, "TrafficLight", filename.c_str());*/

    resman_.CreateCylinder("BranchCylinder", BRANCH_LENGTH, 0.4, 10, 10);

    // Misc Objects
    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/Cig/Cig.obj");
    resman_.LoadResource(Mesh, "Cig", filename.c_str());


    // Building Objects
   /*filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/Building.obj");
    resman_.LoadResource(Mesh, "Building2", filename.c_str());*/

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/Building1.obj");
    resman_.LoadResource(Mesh, "OldHouse", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/Buildings/Building2.obj");
    resman_.LoadResource(Mesh, "B2", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/Buildings/Building3.obj");
    resman_.LoadResource(Mesh, "B3", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/Buildings/CentralBuilding.obj");
    resman_.LoadResource(Mesh, "centralBuilding", filename.c_str());

    // Textures
    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/rocky.png");
    resman_.LoadResource(Texture, "RockyTexture", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/barkTexture.png");
    resman_.LoadResource(Texture, "BarkTexture", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/RedTempText.png");
    resman_.LoadResource(Texture, "RedTexture", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/BlueTempText.png");
    resman_.LoadResource(Texture, "BlueTexture", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/Cig/CigText.png");
    resman_.LoadResource(Texture, "CigText", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/smoke1.png");
    resman_.LoadResource(Texture, "SmokeText", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/GreenTempText.png");
    resman_.LoadResource(Texture, "GreenTexture", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/tileableBrick.png");
    resman_.LoadResource(Texture, "TileableBrickTexture", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/tileableBrick2.png");
    resman_.LoadResource(Texture, "TileableBrick2Texture", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/BuildingText3.png");
    resman_.LoadResource(Texture, "TileableBuildingTexture", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/asphalt.png");
    resman_.LoadResource(Texture, "AsphaltTexture", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/Street_Lamp_Textures/Fasce_Lampada_SH_BaseColor.png");
    resman_.LoadResource(Texture, "LampTexture", filename.c_str());
    
  /*  filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/Body_Metallic1.png");
    resman_.LoadResource(Texture, "Car1Text", filename.c_str());*/

    /*filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/nightSkyBox.png");
    resman_.LoadResource(Texture, "NightTexture", filename.c_str());*/


    //auido .wav file
    filename = std::string(MATERIAL_DIRECTORY).append("\\Assets/rain.wav");
    //rainIndex = am.AddSound(filename.c_str());

    // Load material for screen-space effect
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/screen_space");
    resman_.LoadResource(Material, "ScreenSpaceMaterial", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("/noir_filter");
    resman_.LoadResource(Material, "NoirFilter", filename.c_str());


    // Cube maps
    resman_.CreateCube("Cube");
    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/nightSky/space_");
    resman_.LoadResource(CubeMap, "SpaceSkyBox", filename.c_str());


    // Create particles
    resman_.CreateRainParticles("RainParticles");
    
    resman_.CreateSmokeParticles("SmokeParticles", 2000);
    
    // Setup drawing to texture
    scene_.SetupDrawToTexture();
}


void Game::SetupScene(void){

    // Set background color for the scene
    scene_.SetBackgroundColor(viewport_background_color_g);

    game::SceneNode* playerHead = CreateInstance("PlayerHead", "Cube", "Metal");

    // Create particles
    game::SceneNode* rainParticles = CreateInstance("RainInstance", "RainParticles", "RainMaterial");

    // Create skybox
    game::SceneNode* skyBox = CreateInstance("SkyboxInstance", "Cube", "Skybox", "SpaceSkyBox");

    //game::SceneNode *Car = CreateInstance("Car1", "car", "Noir");
    //Car->SetScale(glm::vec3(0.2, 0.2, 0.2));

   /* game::SceneNode* Building = CreateInstance("one", "car", "Noir");
    Building->SetPosition(glm::vec3(20, 0, -20));
    Building->SetScale(glm::vec3(1, 1, 1)*0.1f);*/

   /* game::SceneNode* Building2 = CreateInstance("two", "TrafficLight", "Noir");
    Building2->SetPosition(glm::vec3(-20, 0, -20));
    Building2->SetScale(glm::vec3(1, 1, 1)*8.0f);*/

    /*am.SetSoundPosition(rainIndex, 0.0f, 0.0f, 0.0f);
    am.SetLoop(rainIndex, true);
    am.PlaySound(rainIndex);*/
   

    game::Cigarette* cig = CreateCigaretteInstance("Cigarette", "Cig", "Noir", "CigText");
    //cig->giveSmokeParticle(smoke1);
    glm::vec3 pos = camera_.GetPosition();
    cig->SetJoint(glm::vec3(pos.x + 0.1, pos.y - 0.1, pos.z - 0.7));
    cig->SetPosition(glm::vec3(pos.x + 0.1, pos.y - 0.1, pos.z - 0.7));
    //Cig = cig;
    
    //CreateRoad(5);
    cig->SetPosition(glm::vec3(0, -0.11, -0.5));
    cig->SetParent(playerHead);
    playerHead->AddChild(cig);
    
    game::SceneNode* smokeParticles = CreateInstance("SmokeInstance", "SmokeParticles", "Cigarette", "SmokeText");
    smokeParticles->SetScale(glm::vec3(0.01));
    smokeParticles->SetPosition(glm::vec3(0.04, 0.01, -0.2));
    smokeParticles->SetParent(cig);
    cig->AddChild(smokeParticles);

    CreateRoad(5);

}


void Game::MainLoop(void){

    // Loop while the user did not close the window
    while (!glfwWindowShouldClose(window_)){

        // Important to attach cigarette to camera
        SceneNode* playerHead = scene_.GetNode("PlayerHead");
        playerHead->SetPosition(camera_.GetPosition());
        playerHead->SetOrientation(camera_.GetOrientation());

        SceneNode* skybox = scene_.GetNode("SkyboxInstance");
        skybox->SetPosition(camera_.GetPosition());

        SceneNode* rain = scene_.GetNode("RainInstance");
        rain->SetPosition(playerHead->GetPosition());
        
        if (animating_) {
            static double last_time = 0;
            double current_time = glfwGetTime();
            if ((current_time - last_time) > 0.05) {
                scene_.Update();
                last_time = current_time;
            }
        }
        
        if (blur_) {
            // Draw the scene to a texture
            scene_.DrawToTexture(&camera_);

            // the texture
            scene_.DisplayTexture(resman_.GetResource("ScreenSpaceMaterial")->GetResource());

        } else if (noir_) {
            // Draw the scene to a texture
            scene_.DrawToTexture(&camera_);

            // the texture
            scene_.DisplayTexture(resman_.GetResource("NoirFilter")->GetResource());
        }
        else {
            // Draw the scene
            scene_.Draw(&camera_);
        }

        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);

        // Update other events like input handling
        glfwPollEvents();
    }
}

void Game::CursorCallback(GLFWwindow* window, double xPos, double yPos) {

    void* ptr = glfwGetWindowUserPointer(window);
    Game* game = (Game*)ptr;

    double xCord = 0;
    double yCord = 0;
    glfwGetCursorPos(window, &xCord, &yCord);
   

   /* int width, height;
    glfwGetWindowSize(window, &width, &height);*/

   /* xCord =  (xCord - (width/2))/(width/2);
    yCord = (yCord - (height / 2))/(height/2);*/

    //std::cout << "now --> " << xCord <<"-"<<yCord<< std::endl;

    //game->camera_.SetOrientation(glm::quat(0.0f, glm::vec3(0, 0, 1)));
    float rot_factor(glm::pi<float>() / 180);
    //float xMag = glm::abs(xCord);
    //float yMag = glm::abs(yCord);
    float mag = 1.5;

    glm::quat r = glm::quat(0,glm::vec3(0,0,0));

    float trans_factor = 1.0;
    if (yCord < 0) {
        game->camera_.Pitch(rot_factor * mag);
        r = glm::normalize(glm::angleAxis(-rot_factor * mag, game->camera_.GetSide()));
    }
    if (yCord > 0) {
        game->camera_.Pitch(-rot_factor * mag);
        r = glm::normalize(glm::angleAxis(rot_factor * mag, game->camera_.GetSide()));
    }
    if (xCord < 0) {
        r = game->camera_.Yaw(rot_factor * mag);
        r = glm::normalize(glm::angleAxis(-rot_factor * mag, game->camera_.GetUp()));
    }
    if (xCord > 0) {
        r = game->camera_.Yaw(-rot_factor * mag);
        r = glm::normalize(glm::angleAxis(rot_factor * mag, game->camera_.GetUp()));
    }

    glfwSetCursorPos(window, 0, 0);
    
}

void Game::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){

    // Get user data with a pointer to the game class
    void* ptr = glfwGetWindowUserPointer(window);
    Game *game = (Game *) ptr;

    
    // Quit game if 'q' is pressed
    if (key == GLFW_KEY_Q && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    // Stop animation if space bar is pressed
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
        game->animating_ = (game->animating_ == true) ? false : true;
    }

    glm::vec3 forwardVec = game->camera_.GetForward();
    forwardVec = glm::vec3(forwardVec.x, forwardVec.y, forwardVec.z);

    // View control
    float rot_factor(glm::pi<float>() / 180);
    float trans_factor = 1.0;

    if (key == GLFW_KEY_W) {
        if (game->cameraLocked_) {
            game->camera_.Translate(glm::vec3(forwardVec.x, 0, forwardVec.z) * trans_factor);
        }
        else {
            game->camera_.Translate(forwardVec * trans_factor);
        }
        

    }
    if (key == GLFW_KEY_S) {
        if (game->cameraLocked_) {
            game->camera_.Translate(-glm::vec3(forwardVec.x, 0, forwardVec.z) * trans_factor);
        }
        else {
            game->camera_.Translate(-forwardVec * trans_factor);
        }
        
    }
    if (key == GLFW_KEY_A) {
        game->camera_.Translate(-game->camera_.GetSide() * trans_factor);
    }
    if (key == GLFW_KEY_D) {
        game->camera_.Translate(game->camera_.GetSide() * trans_factor);
    }
    if (key == GLFW_KEY_B) {
        game->blur_ = !game->blur_;
    }

    std::cout << glm::to_string(game->camera_.GetPosition()) << std::endl;

    if (game->cameraLocked_) {
        glm::vec3 pos = game->camera_.GetPosition();
        if (pos.x < -8 && (pos.z > -205 || pos.z < -225))pos.x = -8;
        if (pos.x > 9 && (pos.z > -188 || pos.z < -225))pos.x = 9;
        if (pos.z > 20)pos.z = 20;
        if (pos.z < -225)pos.z = -225;
        pos.y = 5.0f;

        game->camera_.SetPosition(pos);
    }
    
}


void Game::ResizeCallback(GLFWwindow* window, int width, int height){

    // Set up viewport and camera projection based on new window size
    glViewport(0, 0, width, height);
    void* ptr = glfwGetWindowUserPointer(window);
    Game *game = (Game *) ptr;
    game->camera_.SetProjection(camera_fov_g, camera_near_clip_distance_g, camera_far_clip_distance_g, width, height);
}


Game::~Game(){
    
    glfwTerminate();
}


Streetlamp* Game::CreateStreetlampInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name) {
    // Get resources
    Resource* geom = resman_.GetResource(object_name);
    if (!geom) {
        throw(GameException(std::string("Could not find resource \"") + object_name + std::string("\"")));
    }

    Resource* mat = resman_.GetResource(material_name);
    if (!mat) {
        throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
    }

    Resource* tex = NULL;
    if (texture_name != "") {
        tex = resman_.GetResource(texture_name);
        if (!tex) {
            throw(GameException(std::string("Could not find resource \"") + texture_name + std::string("\"")));
        }
    }

    // Create streetlamp instance
    Streetlamp* streetlamp = new Streetlamp(entity_name, geom, mat, tex);
    scene_.AddNode(streetlamp);
    streetlamps_.push_back(streetlamp);
    return streetlamp;

}

Cigarette* Game::CreateCigaretteInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name)
{
    // Get resources for Cigarette
    Resource* geom = resman_.GetResource(object_name);
    if (!geom) {
        throw(GameException(std::string("Could not find resource \"") + object_name + std::string("\"")));
    }

    Resource* mat = resman_.GetResource(material_name);
    if (!mat) {
        throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
    }

    Resource* tex = NULL;
    if (texture_name != "") {
        tex = resman_.GetResource(texture_name);
        if (!tex) {
            throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
        }
    }

    // Create cigarette instance
    Cigarette *cig = new Cigarette(entity_name, geom, mat,tex);
    scene_.AddNode(cig);
    return cig;
}


//makes a road that extends in the -z direction, with streetlights on rotating sides
void Game::CreateRoad(int num_roads) {

    bool left = true;



    for (int i = 0; i < num_roads; i++) {

        bool even = (i % 2 == 0);
        bool third = (i % 3 == 0);

        // Create instance name
        std::stringstream ss;
        ss << i;
        std::string index = ss.str();
        std::string roadname = "RoadInstance" + index;

        // Create road instance
        game::SceneNode* Road = CreateInstance(roadname, "Rd1", "RoadNoir", "AsphaltTexture");

        Road->SetPosition(glm::vec3(0, -1, -i * 49));

        //std::cout << glm::to_string(Road->GetPosition()) << std::endl;

        if (third) {
            //Buildings

           /* std::string buildingName1 = "BuildingInstanceLeft" + index;
            std::string buildingName2 = "BuildingInstanceRight" + index;

            float buildingScale = 3;

            game::SceneNode* Building1 = CreateInstance(buildingName1, "OldHouse", "TiledBrick","TileableBrickTexture");
           
            Building1->SetScale(glm::vec3(.1, .1, .1) * buildingScale);
            Building1->Rotate(glm::quat(-1.0f, glm::vec3(0, 1, 0)));
            Building1->SetPosition(glm::vec3(-34, -1, (-17 * i) - 22));


            game::SceneNode* Building2 = CreateInstance(buildingName2, "OldHouse", "TiledBrick", "TileableBrickTexture");
            Building2->SetScale(glm::vec3(.1, .1, .1) * buildingScale);
            Building2->Rotate(glm::quat(1.0f, glm::vec3(0, 1, 0)));
            Building2->SetPosition(glm::vec3(34, -1, (-17 * i)-22));*/

            float buildingScale = 9;
            game::SceneNode* Building = CreateInstance("rightBuildingLong", "B3", "TiledBrick", "TileableBuildingTexture");
            Building->SetScale(glm::vec3(.1, .1, .1) * buildingScale);
            //Building->Rotate(glm::quat(-1.0f, glm::vec3(0, 1, 0)));
            Building->SetPosition(glm::vec3(21, -1, (-35*i)-30));

        }

        
        
        if (i == num_roads - 1) {
            game::SceneNode* CentralBuilding = CreateInstance("CentralBuilding", "centralBuilding", "RoadNoir", "GreenTexture");
            CentralBuilding->SetPosition(glm::vec3(0,-1.5, (- i * 49) - 70));
            CentralBuilding->Scale(glm::vec3(1,1,1)*0.5f);
            
            game::SceneNode* Road2 = CreateInstance("landing1", "Rd1", "RoadNoir", "AsphaltTexture");
            game::SceneNode* Road3 = CreateInstance("landing2", "Rd1", "RoadNoir", "AsphaltTexture");
            game::SceneNode* Road4 = CreateInstance("landing3", "Rd1", "RoadNoir", "AsphaltTexture");
            
            float zCord = (-i * 49) - 20;
            Road2->SetPosition(glm::vec3(48,-1, zCord));
            Road2->Rotate(glm::quat(1,glm::vec3(0,1,0)));
            Road3->SetPosition(glm::vec3(0,-1, zCord));
            Road3->Rotate(glm::quat(1, glm::vec3(0, 1, 0)));
            Road4->SetPosition(glm::vec3(-48,-1, zCord));
            Road4->Rotate(glm::quat(1, glm::vec3(0, 1, 0)));

            CreateTree(3, glm::vec3(22, 0, zCord+20));
            game::SceneNode* r = CreateInstance("treeHolder", "Rd1", "RoadNoir", "AsphaltTexture");
            r->SetPosition(glm::vec3(22, -1.25f, zCord + 20));
            //Branch* b = new Branch();
            //game::SceneNode* Tree = CreateInstance("Tree","")

        }

        if (even) {

            std::string lampname = "LampInstance" + index;

            //Street Lamps
            Streetlamp* StreetLamp = CreateStreetlampInstance(lampname, "streetlamp", "RoadNoir", "LampTexture");
            StreetLamp->SetScale(glm::vec3(.33f, 1, .33f));
            if (left) {
                StreetLamp->SetPosition(glm::vec3(-9, -1, -i * 48));
                StreetLamp->SetOrientation(glm::quat(3.0f, glm::vec3(0, 1, 0)));
                StreetLamp->setLightPos(StreetLamp->GetOrientation() * StreetLamp->GetScale() * glm::vec3(2, 8, 0));
                left = false;
            }
            else {
                StreetLamp->SetPosition(glm::vec3(9, -1, -i * 48));
                StreetLamp->SetOrientation(glm::quat(-3.0f, glm::vec3(0, 1, 0)));
                StreetLamp->setLightPos(StreetLamp->GetOrientation() * StreetLamp->GetScale() * glm::vec3(2, 8, 0));
                left = true;
            }

            scene_.AddDirectionalLight((StreetLamp->GetPosition() + StreetLamp->getLightPos()));

        }


    }


    CreateBuildings(glm::vec3(-34, -1, -10), num_roads * 49);

    float buildingScale = 9;
    game::SceneNode* Building = CreateInstance("rightBuildingLong", "B3", "TiledBrick", "TileableBuildingTexture");
    Building->SetScale(glm::vec3(.1, .1, .1)* buildingScale);
    Building->Rotate(glm::quat(1.0f, glm::vec3(0, 1, 0)));
    Building->SetPosition(glm::vec3(0,-1,30));
    
 
       
}


void Game::CreateBuildings(glm::vec3 initPos, float room) {

    float nextSpot = initPos.z;
    
    //std::cout << "proc once" << std::endl;
  

    int index = 0;
    while (abs(nextSpot) +30 < room) {
        
        //Set name
        std::string buildingName;
        if (initPos.x < 0) buildingName = "BuildingInstanceLeft" + index;
        else buildingName = "BuildingInstanceRight" + index;


        //std::cout << rand() % 3 << std::endl;
        int r = rand() % 2;


        //float buildingScale = 7;
        //game::SceneNode* Building = CreateInstance(buildingName, "B3", "TiledBrick", "TileableBrickTexture");
        //Building->SetScale(glm::vec3(.1, .1, .1) * buildingScale);
        ////Building->Rotate(glm::quat(-1.0f, glm::vec3(0, 1, 0)));
        //Building->SetPosition(glm::vec3(-23, -1, nextSpot));
        ////nextSpot -= 29;
        int notOne = 0;
        if (index > 0)notOne = 1;

        if (index == 0) {
            game::SceneNode* Road = CreateInstance("Alley", "Rd1", "RoadNoir", "AsphaltTexture");
            Road->SetPosition(glm::vec3(-20, -1,  -55));
            //game::SceneNode* Car
            game::SceneNode *Car = CreateInstance("Car1", "car", "Noir");
            Car->SetScale(glm::vec3(1, 1, 1)*0.04f);
            Car->Rotate(glm::quat(1.0f,glm::vec3(0,1,0)));
            Car->SetPosition(glm::vec3(-20, -0.5f, -41));
        }

        if (r == 1) {
            float buildingScale = 9;
            game::SceneNode* Building = CreateInstance(buildingName, "B2", "TiledBrick", "TileableBrick2Texture");
            Building->SetScale(glm::vec3(.1, .1, .1) * buildingScale);
            Building->Rotate(glm::quat(-1.0f, glm::vec3(0, 1, 0)));
            Building->SetPosition(glm::vec3(-23, -1, nextSpot - (0*notOne)));
            nextSpot -= 30;
        }
        
        else {
            float buildingScale = 3;
            if (abs(nextSpot - 50) + 10 > room)continue;
            game::SceneNode* Building= CreateInstance(buildingName, "OldHouse", "TiledBrick", "TileableBrickTexture");
            Building->SetScale(glm::vec3(.1, .1, .1) * buildingScale);
            Building->Rotate(glm::quat(-1.0f, glm::vec3(0, 1, 0)));
            Building->SetPosition(glm::vec3(-34, -1, nextSpot- (10 * notOne)));
            nextSpot -= 50;
        }
        

        ////room -= 50;
        


        index++;
        //std::cout << "made new building" << std::endl;

        //disable later
        if (index > 10)break;
    }

   
   /* std::string buildingName1 = "BuildingInstanceLeft" + index;
    std::string buildingName2 = "BuildingInstanceRight" + index;

    float buildingScale = 3;

    game::SceneNode* Building1 = CreateInstance(buildingName1, "OldHouse", "TiledBrick", "TileableBrickTexture");

    Building1->SetScale(glm::vec3(.1, .1, .1) * buildingScale);
    Building1->Rotate(glm::quat(-1.0f, glm::vec3(0, 1, 0)));
    Building1->SetPosition(glm::vec3(-34, -1, (-17 * i) - 22));


    game::SceneNode* Building2 = CreateInstance(buildingName2, "OldHouse", "TiledBrick", "TileableBrickTexture");
    Building2->SetScale(glm::vec3(.1, .1, .1) * buildingScale);
    Building2->Rotate(glm::quat(1.0f, glm::vec3(0, 1, 0)));
    Building2->SetPosition(glm::vec3(34, -1, (-17 * i) - 22));*/

}

Branch* Game::CreateBranchInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name, Branch* parent_branch, int depth, int id_in_set, int branch_per_level) {
    // Get resources
    Resource* geom = resman_.GetResource(object_name);
    if (!geom) {
        throw(GameException(std::string("Could not find resource \"") + object_name + std::string("\"")));
    }

    Resource* mat = resman_.GetResource(material_name);
    if (!mat) {
        throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
    }

    Resource* tex = NULL;
    if (texture_name != "") {
        tex = resman_.GetResource(texture_name);
        if (!tex) {
            throw(GameException(std::string("Could not find resource \"") + texture_name + std::string("\"")));
        }
    }

    // Create branch instance
    Branch* branch = new Branch(entity_name, geom, mat, tex);

    // Set relations
    branch->SetParent(parent_branch);

    // For most branches (not the trunk), set up the patterned attributes
    if (parent_branch != NULL) {
        parent_branch->AddChild(branch);
        glm::vec3 p_scale = parent_branch->GetScale();
        glm::vec3 scale = glm::vec3(p_scale.x*0.75, p_scale.y*0.9, p_scale.z*0.75); // I wanted the branches to stay long
        branch->SetPosition(glm::vec3(scale.x * -BRANCH_LENGTH/2.5, (BRANCH_LENGTH/1.6), 0));
        branch->SetScale(scale);
        branch->Rotate(glm::quat(1, 0, 0, glm::half_pi<float>() * 0.2)); // Rotate a bit outwards

        std::random_device rand;
        std::mt19937 gen(rand());
        std::uniform_int_distribution<> dist(-0.5, 0.5);

        glm::quat ang_m = glm::normalize(glm::angleAxis(id_in_set * 2 * glm::pi<float>() / branch_per_level + dist(gen), glm::vec3(0, 1, 0))); // Branches are evenly spaced
        branch->Orbit(glm::vec3(scale.x * -BRANCH_LENGTH/2.5, 0, 0), ang_m);
    }
    branch->SetDepth(depth); // For different ranges of sway depending on size of branch

    scene_.AddNode(branch);
    return branch;
}

void Game::CreateTree(int branch_per_level, glm::vec3 position) {
    std::stringstream ss;
    ss << 0;
    std::string index = ss.str();
    std::string name = "BranchInstance" + index;
    ss.clear();

    // Create branch instance
    Branch* trunk = CreateBranchInstance(name, "BranchCylinder", "RoadNoir", "BarkTexture", NULL, 0, 0, 0);
    // Trunk is a special branch which has its parameters 
    trunk->SetPosition(position);
    trunk->SetOrientation(glm::quat(0, glm::vec3(0.0, 1.0, 0.0)));
    trunk->SetUp(glm::vec3(0.0, 1.0, 0.0));
    for (int i = 0; i < branch_per_level; i++) {

        // Setup naming convention depth+index
        ss << 1 << i;
        name = "BranchInstance" + index;
        ss.clear();

        // Create and set up new branch.
        Branch* new_branch = CreateBranchInstance(name, "BranchCylinder", "RoadNoir", "BarkTexture", trunk, 1, i, branch_per_level);

        // Track the parent for the next layer
        Branch* depth1_branch = new_branch;

        // Next two tiers do the same thing
        for (int j = 0; j < branch_per_level; j++) {

            ss << 2 << i;
            name = "BranchInstance" + index;
            ss.clear();

            Branch* new_branch = CreateBranchInstance(name, "BranchCylinder", "RoadNoir", "BarkTexture", depth1_branch, 2, j, branch_per_level);

            Branch* depth2_branch = new_branch;

            for (int k = 0; k < branch_per_level; k++) {

                ss << 3 << i;
                name = "BranchInstance" + index;
                ss.clear();

                Branch* new_branch = CreateBranchInstance(name, "BranchCylinder", "RoadNoir", "BarkTexture", depth2_branch, 3, k, branch_per_level);
            }
        }
    }
}

SceneNode *Game::CreateInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name){

    Resource *geom = resman_.GetResource(object_name);
    if (!geom){
        throw(GameException(std::string("Could not find resource \"")+object_name+std::string("\"")));
    }

    Resource *mat = resman_.GetResource(material_name);
    if (!mat){
        throw(GameException(std::string("Could not find resource \"")+material_name+std::string("\"")));
    }

    Resource *tex = NULL;
    if (texture_name != ""){
        tex = resman_.GetResource(texture_name);
        if (!tex){
            throw(GameException(std::string("Could not find resource \"")+texture_name+std::string("\"")));
        }
    }

    SceneNode *scn = scene_.CreateNode(entity_name, geom, mat, tex);
    return scn;
}

} // namespace game
