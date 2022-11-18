#include <iostream>
#include <time.h>
#include <sstream>

#include "game.h"
#include "path_config.h"

namespace game {

// Some configuration constants
// They are written here as global variables, but ideally they should be loaded from a configuration file

// Main window settings
const std::string window_title_g = "Demo";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
const bool window_full_screen_g = false;

// Viewport and camera settings
float camera_near_clip_distance_g = 0.01;
float camera_far_clip_distance_g = 1000.0;
float camera_fov_g = 20.0; // Field-of-view of camera
const glm::vec3 viewport_background_color_g(0.0, 0.0, 0.0);
glm::vec3 camera_position_g(0.5, 0.5, 10.0);
glm::vec3 camera_look_at_g(0.0, 0.0, 0.0);
glm::vec3 camera_up_g(0.0, 1.0, 0.0);

// Materials 
const std::string material_directory_g = MATERIAL_DIRECTORY;


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

    // Load material to be applied to particles
    std::string filename = std::string(MATERIAL_DIRECTORY) + std::string("\\particle");
    resman_.LoadResource(Material, "ParticleMaterial", filename.c_str());

   filename = std::string(MATERIAL_DIRECTORY) + std::string("\\noir");
    resman_.LoadResource(Material, "Noir", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\metal");
    resman_.LoadResource(Material, "Metal", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/StraightRoad1.obj");
    resman_.LoadResource(Mesh, "Rd1", filename.c_str());
  

   /* filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/Building.obj");
    resman_.LoadResource(Mesh, "Building2", filename.c_str());*/
    
    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/Car2.obj");
    resman_.LoadResource(Mesh, "car", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/Street_Lamp_TS.obj");
    resman_.LoadResource(Mesh, "streetlamp", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/Building1.obj");
    resman_.LoadResource(Mesh, "OldHouse", filename.c_str());


    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/centralBuilding.obj");
    resman_.LoadResource(Mesh, "centralBuilding", filename.c_str());
    

    /*filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/PostOffice.obj");
    resman_.LoadResource(Mesh, "Rd1", filename.c_str());*/

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/rocky.png");
    resman_.LoadResource(Texture, "RockyTexture", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/RedTempText.png");
    resman_.LoadResource(Texture, "RedTexture", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/BlueTempText.png");
    resman_.LoadResource(Texture, "BlueTexture", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/GreenTempText.png");
    resman_.LoadResource(Texture, "GreenTexture", filename.c_str());

  /*  filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/Body_Metallic1.png");
    resman_.LoadResource(Texture, "Car1Text", filename.c_str());*/



    /*filename = std::string(MATERIAL_DIRECTORY) + std::string("\\Assets/BrickBuildText.png");
    resman_.LoadResource(Texture, "BrickText", filename.c_str());*/


    // Create particles for explosion
    //resman_.CreateSphereParticles("SphereParticles");
    

    
}


void Game::SetupScene(void){

    // Set background color for the scene
    scene_.SetBackgroundColor(viewport_background_color_g);

    // Create particles
    //game::SceneNode *particles = CreateInstance("ParticleInstance1", "SphereParticles", "ParticleMaterial");
    //game::SceneNode *Car = CreateInstance("Car1", "car", "Noir");
    //Car->SetScale(glm::vec3(0.2, 0.2, 0.2));

    //game::SceneNode* Road = CreateInstance("Road1", "Rd1", "Noir");
    ////Road->SetScale(glm::vec3(0.2, 0.2, 0.2));

    //game::SceneNode* StreetLamp = CreateInstance("StreetLamp1", "streetlamp", "Noir", "RockyTexture");
    //StreetLamp->SetScale(glm::vec3(10, 10, 10));

    /*game::SceneNode* Building = CreateInstance("centralBuilding1", "centralBuilding", "Noir");
    Building->SetPosition(glm::vec3(0, 0, -20));
    Building->SetScale(glm::vec3(0.2, 0.2, 0.2));*/

    CreateRoad(2);

}


void Game::MainLoop(void){

    // Loop while the user did not close the window
    while (!glfwWindowShouldClose(window_)){

        //SceneNode* n = scene_.GetNode("Car1");
        ////if (n != NULL)std::cout << "LOADED AND HERE" << std::endl;
        ////else std::cout << "dumb ass, maybe get it to work"<< std::endl;
        //n->SetPosition(glm::vec3(0, 0, 0));

        //n = scene_.GetNode("StreetLamp1");
        //n->SetPosition(glm::vec3(10, 0, 0));
        //

        //n = scene_.GetNode("Road1");
        //n->SetPosition(glm::vec3(0, 3, 0));

        // Draw the scene
        scene_.Draw(&camera_);

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
    float mag = 0.2;

    float trans_factor = 1.0;
    if (yCord < 0) {
        game->camera_.Pitch(rot_factor * mag);
    }
    if (yCord > 0) {
        game->camera_.Pitch(-rot_factor * mag);
    }
    if (xCord < 0) {
        game->camera_.Yaw(rot_factor * mag);
    }
    if (xCord > 0) {
        game->camera_.Yaw(-rot_factor * mag);
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
   /* if (key == GLFW_KEY_UP){
        game->camera_.Pitch(rot_factor);
    }
    if (key == GLFW_KEY_DOWN){
        game->camera_.Pitch(-rot_factor);
    }
    if (key == GLFW_KEY_LEFT){
        game->camera_.Yaw(rot_factor);
    }
    if (key == GLFW_KEY_RIGHT){
        game->camera_.Yaw(-rot_factor);
    }
    if (key == GLFW_KEY_S){
        game->camera_.Roll(-rot_factor);
    }
    if (key == GLFW_KEY_X){
        game->camera_.Roll(rot_factor);
    }
    if (key == GLFW_KEY_A){
        game->camera_.Translate(game->camera_.GetForward()*trans_factor);
    }
    if (key == GLFW_KEY_Z){
        game->camera_.Translate(-game->camera_.GetForward()*trans_factor);
    }
    if (key == GLFW_KEY_J){
        game->camera_.Translate(-game->camera_.GetSide()*trans_factor);
    }
    if (key == GLFW_KEY_L){
        game->camera_.Translate(game->camera_.GetSide()*trans_factor);
    }
    if (key == GLFW_KEY_I){
        game->camera_.Translate(game->camera_.GetUp()*trans_factor);
    }
    if (key == GLFW_KEY_K){
        game->camera_.Translate(-game->camera_.GetUp()*trans_factor);
    }*/

    if (key == GLFW_KEY_W) {
        game->camera_.Translate(forwardVec * trans_factor);
    }
    if (key == GLFW_KEY_S) {
        game->camera_.Translate(-forwardVec * trans_factor);
    }
    if (key == GLFW_KEY_A) {
        game->camera_.Translate(-game->camera_.GetSide() * trans_factor);
    }
    if (key == GLFW_KEY_D) {
        game->camera_.Translate(game->camera_.GetSide() * trans_factor);
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


Asteroid *Game::CreateAsteroidInstance(std::string entity_name, std::string object_name, std::string material_name){

    // Get resources
    Resource *geom = resman_.GetResource(object_name);
    if (!geom){
        throw(GameException(std::string("Could not find resource \"")+object_name+std::string("\"")));
    }

    Resource *mat = resman_.GetResource(material_name);
    if (!mat){
        throw(GameException(std::string("Could not find resource \"")+material_name+std::string("\"")));
    }

    // Create asteroid instance
    Asteroid *ast = new Asteroid(entity_name, geom, mat);
    scene_.AddNode(ast);
    return ast;
}


void Game::CreateAsteroidField(int num_asteroids){

    // Create a number of asteroid instances
    for (int i = 0; i < num_asteroids; i++){
        // Create instance name
        std::stringstream ss;
        ss << i;
        std::string index = ss.str();
        std::string name = "AsteroidInstance" + index;

        // Create asteroid instance
        Asteroid *ast = CreateAsteroidInstance(name, "SimpleSphereMesh", "ObjectMaterial");

        // Set attributes of asteroid: random position, orientation, and
        // angular momentum
        ast->SetPosition(glm::vec3(-300.0 + 600.0*((float) rand() / RAND_MAX), -300.0 + 600.0*((float) rand() / RAND_MAX), 600.0*((float) rand() / RAND_MAX)));
        ast->SetOrientation(glm::normalize(glm::angleAxis(glm::pi<float>()*((float) rand() / RAND_MAX), glm::vec3(((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX)))));
        ast->SetAngM(glm::normalize(glm::angleAxis(0.05f*glm::pi<float>()*((float) rand() / RAND_MAX), glm::vec3(((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX)))));
    }
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

        // Create asteroid instance
        game::SceneNode* Road = CreateInstance(roadname, "Rd1", "Noir","RockyTexture");
        
        if (even) {
            
            std::string lampname = "LampInstance" + index;
            
            //Street Lamps
            game::SceneNode* StreetLamp = CreateInstance(lampname, "streetlamp", "Noir", "RedTexture");
            StreetLamp->SetScale(glm::vec3(.33f, 1, .33f));
            if (left) {
                StreetLamp->SetPosition(glm::vec3(-9, -1, -i * 48));
                StreetLamp->SetOrientation(glm::quat(3.0f, glm::vec3(0, 1, 0)));
                left = false;
            }
            else {
                StreetLamp->SetPosition(glm::vec3(9, -1, -i * 48));
                StreetLamp->SetOrientation(glm::quat(-3.0f, glm::vec3(0, 1, 0)));
                left = true;
            }

            
        }
           

        if (third) {
            //Buildings

            std::string buildingName1 = "BuildingInstanceLeft" + index;
            std::string buildingName2 = "BuildingInstanceRight" + index;

            float buildingScale = 3;

            game::SceneNode* Building1 = CreateInstance(buildingName1, "OldHouse", "Noir","BlueTexture");
           
            Building1->SetScale(glm::vec3(.1, .1, .1) * buildingScale);
            Building1->Rotate(glm::quat(-1.0f, glm::vec3(0, 1, 0)));
            Building1->SetPosition(glm::vec3(-34, -1, (-17 * i) - 22));


            game::SceneNode* Building2 = CreateInstance(buildingName2, "OldHouse", "Noir", "BlueTexture");
            Building2->SetScale(glm::vec3(.1, .1, .1) * buildingScale);
            Building2->Rotate(glm::quat(1.0f, glm::vec3(0, 1, 0)));
            Building2->SetPosition(glm::vec3(34, -1, (-17 * i)-22));

        }

        // Set attributes of asteroid: random position, orientation, and
        // angular momentum
        Road->SetPosition(glm::vec3(0, -1,-i * 49));
        
        if (i == num_roads - 1) {
            game::SceneNode* CentralBuilding = CreateInstance("CentralBuilding", "centralBuilding", "Noir");
            CentralBuilding->SetPosition(glm::vec3(0,-1, - i * 113));
            CentralBuilding->Scale(glm::vec3(1,1,1)*50.0f);
            
            game::SceneNode* Road2 = CreateInstance("landing1", "Rd1", "Noir", "RockyTexture");
            game::SceneNode* Road3 = CreateInstance("landing2", "Rd1", "Noir", "RockyTexture");
            game::SceneNode* Road4 = CreateInstance("landing3", "Rd1", "Noir", "RockyTexture");
            
            Road2->SetPosition(glm::vec3(48,-1, -i * 70));
            Road2->Rotate(glm::quat(1,glm::vec3(0,1,0)));
            Road3->SetPosition(glm::vec3(0,-1, -i * 70));
            Road3->Rotate(glm::quat(1, glm::vec3(0, 1, 0)));
            Road4->SetPosition(glm::vec3(-48,-1, -i * 70));
            Road4->Rotate(glm::quat(1, glm::vec3(0, 1, 0)));

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
            throw(GameException(std::string("Could not find resource \"")+material_name+std::string("\"")));
        }
    }

    SceneNode *scn = scene_.CreateNode(entity_name, geom, mat, tex);
    return scn;
}

} // namespace game
